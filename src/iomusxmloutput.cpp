/////////////////////////////////////////////////////////////////////////////
// Name:        iomusxmloutput.cpp
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "iomusxmloutput.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <limits>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

//----------------------------------------------------------------------------

#include "accid.h"
#include "beam.h"
#include "clef.h"
#include "comparison.h"
#include "doc.h"
#include "durationinterface.h"
#include "fraction.h"
#include "functor.h"
#include "instrdef.h"
#include "keysig.h"
#include "label.h"
#include "layer.h"
#include "layerdef.h"
#include "layerelement.h"
#include "measure.h"
#include "metersig.h"
#include "metersiggrp.h"
#include "mnum.h"
#include "note.h"
#include "scoredef.h"
#include "staff.h"
#include "staffdef.h"
#include "transposition.h"
#include "vrv.h"

//----------------------------------------------------------------------------

#include "pugixml.hpp"

namespace vrv {

#ifndef NO_MUSICXML_SUPPORT
namespace musicxmloutput {

    struct MusicXmlEvent {
        const LayerElement *element;
        Fraction duration;
        const char *type;
        int dots;
        const char *beamValue = NULL;
    };

    struct MusicXmlVoice {
        int staff;
        int voice;
        const InstrDef *instrument;
        std::vector<MusicXmlEvent> events;
    };

    struct MusicXmlVoiceDef {
        int number = 0;
        const InstrDef *instrument = NULL;
    };

    struct MusicXmlStaff {
        const StaffDef *definition;
        Fraction measureDuration;
        std::map<int, MusicXmlVoiceDef> voices;
    };

    struct MusicXmlPart {
        const Object *definition;
        const InstrDef *instrument;
        std::string id;
        std::vector<MusicXmlStaff> staves;
        std::vector<std::vector<MusicXmlVoice>> measures;
        std::map<std::string, const InstrDef *> layerInstruments;
    };

    class CollectMusicXmlMeasuresFunctor : public ConstFunctor {
    public:
        bool ImplementsEndInterface() const override { return false; }

        FunctorCode VisitMeasure(const Measure *measure) override
        {
            m_measures.push_back(measure);
            return FUNCTOR_SIBLINGS;
        }

    public:
        std::vector<const Measure *> m_measures;
    };

    const char *GetMusicXmlNoteType(data_DURATION duration)
    {
        static const char *const types[] = { "long", "breve", "whole", "half", "quarter", "eighth", "16th", "32nd",
            "64th", "128th", "256th", "512th", "1024th" };
        if (duration < DURATION_long || duration > DURATION_1024) return NULL;
        return types[duration - DURATION_long];
    }

    const char *GetMusicXmlAccidental(data_ACCIDENTAL_WRITTEN accidental)
    {
        switch (accidental) {
            case ACCIDENTAL_WRITTEN_NONE: return "";
            case ACCIDENTAL_WRITTEN_f: return "flat";
            case ACCIDENTAL_WRITTEN_ff: return "flat-flat";
            case ACCIDENTAL_WRITTEN_n: return "natural";
            case ACCIDENTAL_WRITTEN_s: return "sharp";
            case ACCIDENTAL_WRITTEN_ss:
            case ACCIDENTAL_WRITTEN_x: return "double-sharp";
            default: return NULL;
        }
    }

    bool IsSupportedMusicXmlNote(const Note *note)
    {
        const int octave = note->HasOct() ? note->GetOct() : note->GetOctDefault();
        if (!note->HasPname() || octave < 0 || octave > 9 || note->IsGraceNote() || note->HasStaff()
            || note->HasPnameGes() || note->HasOctGes() || note->HasTie()) {
            return false;
        }
        const Accid *accid = note->GetDrawingAccid();
        return !accid
            || (GetMusicXmlAccidental(accid->GetAccid())
                && GetMusicXmlAccidental(Att::AccidentalGesturalToWritten(accid->GetAccidGes())));
    }

    template <typename Element>
    const Element *GetInitialAttribute(const ScoreDef *scoreDef, const StaffDef *staffDef, ClassId classId)
    {
        const Object *attribute = staffDef->FindDescendantByType(classId, 1);
        if (!attribute) attribute = scoreDef->FindDescendantByType(classId, 1);
        return vrv_cast<const Element *>(attribute);
    }

    std::pair<int, int> GetMusicXmlMeter(const MeterSig *meter)
    {
        if (!meter) return { 4, 4 };
        if (meter->HasCount() && meter->GetCount().first.size() != 1) return { 0, 0 };
        if (meter->GetSym() == METERSIGN_open) return { 0, 0 };
        const bool cut = (meter->GetSym() == METERSIGN_cut);
        return { meter->HasCount() ? meter->GetCount().first.front() : (cut ? 2 : 4),
            meter->HasUnit() ? meter->GetUnit() : (cut ? 2 : 4) };
    }

    bool AddMusicXmlEvent(const LayerElement *element, Fraction measureDuration, MusicXmlVoice &voice)
    {
        const DurationInterface *duration = element->GetDurationInterface();
        const ClassId classId = element->GetClassId();
        if (element->HasSameas() || element->IsGraceNote()) return false;
        if (classId == MREST || classId == MSPACE) {
            voice.events.push_back({ element, measureDuration, NULL, 0 });
            return true;
        }
        if (classId != NOTE && classId != REST && classId != SPACE && classId != CHORD) return false;
        if (classId == NOTE && !IsSupportedMusicXmlNote(vrv_cast<const Note *>(element))) return false;
        if (classId == CHORD) {
            const ArrayOfConstObjects children = element->GetChildren();
            int notes = 0;
            for (const Object *child : children) {
                if (!child->Is(NOTE)) continue;
                const Note *note = vrv_cast<const Note *>(child);
                if (!IsSupportedMusicXmlNote(note) || note->HasDur() || note->HasDots()) return false;
                ++notes;
            }
            if (notes < 2) return false;
        }

        if (duration->IsMensuralDur() || duration->HasNum() || duration->HasNumbase() || duration->HasStaff()
            || duration->HasDurGes() || duration->HasDotsGes()) {
            return false;
        }
        const char *type = GetMusicXmlNoteType(duration->GetActualDur());
        const int dots = duration->HasDots() ? duration->GetDots() : 0;
        if (!type || dots < 0 || dots > 8) return false;
        // Verovio durations are fractions of a whole note. MusicXML divisions are per quarter note.
        voice.events.push_back({ element, duration->GetInterfaceAlignmentDuration(1, 1) * 4, type, dots });
        return true;
    }

    bool AddMusicXmlBeam(const Beam *beam, Fraction measureDuration, MusicXmlVoice &voice)
    {
        if (beam->HasSameas() || beam->HasBeamWith() || (beam->HasForm() && beam->GetForm() != beamRend_FORM_norm)) {
            return false;
        }
        const ArrayOfConstObjects children = beam->GetChildren();
        if (children.size() < 2 || children.front()->Is(REST) || children.back()->Is(REST)) {
            return false;
        }

        const size_t first = voice.events.size();
        for (const Object *child : children) {
            if (!child->Is(NOTE) && !child->Is(CHORD) && !child->Is(REST)) {
                return false;
            }
            const LayerElement *element = vrv_cast<const LayerElement *>(child);
            if (element->GetDurationInterface()->GetActualDur() != DURATION_8
                || !AddMusicXmlEvent(element, measureDuration, voice)) {
                return false;
            }
            voice.events.back().beamValue = "continue";
        }
        voice.events.at(first).beamValue = "begin";
        voice.events.back().beamValue = "end";
        return true;
    }

    // MusicXML uses one cursor for the entire part-measure. Each MEI layer is emitted as a separate run on this cursor.
    bool CollectMusicXmlVoice(const Layer *layer, Fraction measureDuration, MusicXmlVoice &voice)
    {
        for (const Object *object : layer->GetChildren()) {
            if (object->Is(BEAM)) {
                if (AddMusicXmlBeam(vrv_cast<const Beam *>(object), measureDuration, voice)) continue;
            }
            else {
                const LayerElement *element = dynamic_cast<const LayerElement *>(object);
                if (element && AddMusicXmlEvent(element, measureDuration, voice)) continue;
            }
            LogError("MusicXML output does not support '%s' in a layer (xml:id '%s')", object->GetClassName().c_str(),
                object->GetID().c_str());
            return false;
        }
        return true;
    }

    void AssignMusicXmlVoices(MusicXmlPart &part)
    {
        // MEI can reuse a layer number on different staves. Keep the first occurrence and reserve the
        // remaining numbers before assigning new ones.
        std::set<int> usedVoices;
        for (MusicXmlStaff &staff : part.staves) {
            for (std::pair<const int, MusicXmlVoiceDef> &entry : staff.voices) {
                if (entry.first > 0 && usedVoices.insert(entry.first).second) entry.second.number = entry.first;
            }
        }
        int nextVoice = 1;
        for (MusicXmlStaff &staff : part.staves) {
            for (std::pair<const int, MusicXmlVoiceDef> &entry : staff.voices) {
                if (entry.second.number > 0) continue;
                while (!usedVoices.insert(nextVoice).second) ++nextVoice;
                entry.second.number = nextVoice++;
            }
        }
    }

    bool CollectMusicXmlPartMeasure(MusicXmlPart &part, const Measure *measure, int &divisions)
    {
        std::vector<MusicXmlVoice> &voices = part.measures.emplace_back();
        int staffNumber = 0;
        for (const MusicXmlStaff &staff : part.staves) {
            ++staffNumber;
            const int xmlStaff = part.staves.size() > 1 ? staffNumber : 0;
            AttNIntegerComparison matchStaff(STAFF, staff.definition->GetN());
            const Staff *staffElement = vrv_cast<const Staff *>(measure->FindDescendantByComparison(&matchStaff, 1));
            const size_t first = voices.size();
            if (staffElement) {
                for (const Object *child : staffElement->GetChildren()) {
                    if (!child->Is(LAYER)) continue;
                    const Layer *layer = vrv_cast<const Layer *>(child);
                    const std::map<int, MusicXmlVoiceDef>::const_iterator voiceDef = staff.voices.find(layer->GetN());
                    if (voiceDef == staff.voices.end()) continue;
                    MusicXmlVoice voice{ xmlStaff, voiceDef->second.number, voiceDef->second.instrument, {} };
                    if (!CollectMusicXmlVoice(layer, staff.measureDuration, voice)) return false;
                    if (!voice.events.empty()) voices.push_back(std::move(voice));
                }
            }
            if (voices.size() == first) {
                // A silent staff still needs a voice run so re-import associates the next run with its own staff.
                const MusicXmlVoiceDef &voiceDef = staff.voices.begin()->second;
                voices.push_back(
                    { xmlStaff, voiceDef.number, voiceDef.instrument, { { NULL, staff.measureDuration, NULL, 0 } } });
            }
        }
        std::sort(voices.begin(), voices.end(), [](const MusicXmlVoice &one, const MusicXmlVoice &two) {
            return std::tie(one.staff, one.voice) < std::tie(two.staff, two.voice);
        });
        for (const MusicXmlVoice &voice : voices) {
            if (voice.instrument) {
                const std::pair<std::map<std::string, const InstrDef *>::iterator, bool> entry
                    = part.layerInstruments.emplace(voice.instrument->GetID(), voice.instrument);
                if (entry.first->second != voice.instrument) {
                    LogError("MusicXML output requires unique instrDef IDs (duplicate '%s')",
                        voice.instrument->GetID().c_str());
                    return false;
                }
            }
            for (const MusicXmlEvent &event : voice.events) {
                divisions = std::lcm(divisions, event.duration.GetDenominator());
                // Bound the resolution for consumers that calculate the measure size using integer arithmetic.
                if (divisions > 16384) {
                    LogError("MusicXML output duration resolution exceeds 16384 divisions");
                    return false;
                }
            }
        }
        return true;
    }

    std::string GetMusicXmlPartName(const MusicXmlPart &part)
    {
        if (part.instrument && part.instrument->HasLabel()) return part.instrument->GetLabel();
        const Label *label = vrv_cast<const Label *>(part.definition->GetFirst(LABEL));
        if (label) return UTF32to8(label->GetText());
        const AttLabelled *labelled = dynamic_cast<const AttLabelled *>(part.definition);
        return labelled ? labelled->GetLabel() : "";
    }

    int GetMusicXmlTicks(Fraction duration, int divisions)
    {
        // The plan chooses divisions that are multiples of every event denominator.
        return duration.GetNumerator() * (divisions / duration.GetDenominator());
    }

    void WriteMusicXmlAttributes(
        pugi::xml_node measure, const ScoreDef *scoreDef, const MusicXmlPart &part, int divisions)
    {
        pugi::xml_node attributes = measure.append_child("attributes");
        attributes.append_child("divisions").text().set(divisions);
        int staffNumber = 0;
        for (const MusicXmlStaff &staff : part.staves) {
            ++staffNumber;
            const KeySig *key = GetInitialAttribute<KeySig>(scoreDef, staff.definition, KEYSIG);
            if (!key) continue;
            pugi::xml_node keyNode = attributes.append_child("key");
            if (part.staves.size() > 1) keyNode.append_attribute("number") = staffNumber;
            keyNode.append_child("fifths").text().set(key->GetFifthsInt());
        }
        staffNumber = 0;
        for (const MusicXmlStaff &staff : part.staves) {
            ++staffNumber;
            const MeterSig *meter = GetInitialAttribute<MeterSig>(scoreDef, staff.definition, METERSIG);
            if (!meter) continue;
            pugi::xml_node time = attributes.append_child("time");
            if (part.staves.size() > 1) time.append_attribute("number") = staffNumber;
            if (meter->GetSym() == METERSIGN_common) time.append_attribute("symbol") = "common";
            if (meter->GetSym() == METERSIGN_cut) time.append_attribute("symbol") = "cut";
            if (meter->GetVisible() == BOOLEAN_false) time.append_attribute("print-object") = "no";
            const std::pair<int, int> meterValues = GetMusicXmlMeter(meter);
            time.append_child("beats").text().set(meterValues.first);
            time.append_child("beat-type").text().set(meterValues.second);
        }
        if (part.staves.size() > 1) attributes.append_child("staves").text().set(static_cast<int>(part.staves.size()));
        staffNumber = 0;
        for (const MusicXmlStaff &staff : part.staves) {
            ++staffNumber;
            const Clef *clef = GetInitialAttribute<Clef>(scoreDef, staff.definition, CLEF);
            if (!clef || !clef->HasShape()) continue;
            pugi::xml_node clefNode = attributes.append_child("clef");
            if (part.staves.size() > 1) clefNode.append_attribute("number") = staffNumber;
            clefNode.append_child("sign").text().set(clef->AttClefShape::ClefshapeToStr(clef->GetShape()).c_str());
            if (clef->HasLine()) clefNode.append_child("line").text().set(clef->GetLine());
        }
    }

    void WriteMusicXmlNote(pugi::xml_node measure, const MusicXmlEvent &event, const Note *note, bool chordTone,
        int divisions, const MusicXmlVoice &voice)
    {
        pugi::xml_node node = measure.append_child("note");
        const Accid *accid = NULL;
        if (chordTone) node.append_child("chord");
        if (note) {
            pugi::xml_node pitch = node.append_child("pitch");
            const char step[] = { "CDEFGAB"[note->GetPname() - PITCHNAME_c], '\0' };
            pitch.append_child("step").text().set(step);
            accid = note->GetDrawingAccid();
            const int alter = accid ? TransPitch::GetChromaticAlteration(accid->GetAccidGes(), accid->GetAccid()) : 0;
            if (alter) pitch.append_child("alter").text().set(alter);
            pitch.append_child("octave").text().set(note->HasOct() ? note->GetOct() : note->GetOctDefault());
            if (note->GetVisible() == BOOLEAN_false) node.append_attribute("print-object") = "no";
        }
        else {
            pugi::xml_node rest = node.append_child("rest");
            if (!event.type) rest.append_attribute("measure") = "yes";
            if (!event.element || event.element->Is(SPACE) || event.element->Is(MSPACE)) {
                node.append_attribute("print-object") = "no";
            }
        }
        node.append_child("duration").text().set(GetMusicXmlTicks(event.duration, divisions));
        if (voice.instrument)
            node.append_child("instrument").append_attribute("id") = voice.instrument->GetID().c_str();
        node.append_child("voice").text().set(voice.voice);
        if (event.type) node.append_child("type").text().set(event.type);
        for (int i = 0; i < event.dots; ++i) node.append_child("dot");
        if (accid && accid->HasAccid()) {
            node.append_child("accidental").text().set(GetMusicXmlAccidental(accid->GetAccid()));
        }
        if (voice.staff > 0) node.append_child("staff").text().set(voice.staff);
        // A chord is a single beam position. Write it on the chord's first MusicXML note.
        if (event.beamValue && !chordTone) {
            pugi::xml_node beam = node.append_child("beam");
            beam.append_attribute("number") = 1;
            beam.text().set(event.beamValue);
        }
    }

    void WriteMusicXmlVoice(pugi::xml_node measure, const MusicXmlVoice &voice, int divisions)
    {
        for (const MusicXmlEvent &event : voice.events) {
            if (event.element && event.element->Is(CHORD)) {
                bool chordTone = false;
                for (const Object *child : event.element->GetChildren()) {
                    if (!child->Is(NOTE)) continue;
                    WriteMusicXmlNote(measure, event, vrv_cast<const Note *>(child), chordTone, divisions, voice);
                    chordTone = true;
                }
            }
            else {
                const Note *note = dynamic_cast<const Note *>(event.element);
                WriteMusicXmlNote(measure, event, note, false, divisions, voice);
            }
        }
    }

} // namespace musicxmloutput
#endif

MusicXmlOutput::MusicXmlOutput(Doc *doc) : Output(doc) {}

std::string MusicXmlOutput::Export()
{
#ifdef NO_MUSICXML_SUPPORT
    LogError("MusicXML support is disabled");
    return "";
#else
    const Doc *doc = m_doc;
    const ListOfConstObjects movements = doc->FindAllDescendantsByType(MDIV, false);
    const ListOfConstObjects scores = doc->FindAllDescendantsByType(SCORE, false);
    const ScoreDef *scoreDef = doc->GetFirstScoreDef();
    if (movements.size() != 1 || scores.size() != 1 || !scoreDef || doc->FindDescendantByType(PART)) {
        LogError("MusicXML output requires one mdiv containing a score");
        return "";
    }
    if (doc->FindDescendantByType(SCOREDEF)) {
        LogError("MusicXML output does not support mid-score scoreDef changes yet");
        return "";
    }
    musicxmloutput::CollectMusicXmlMeasuresFunctor collectMeasures;
    doc->Process(collectMeasures);
    const std::vector<const Measure *> &measures = collectMeasures.m_measures;
    const ListOfConstObjects staffDefs = scoreDef->FindAllDescendantsByType(STAFFDEF);
    if (measures.empty() || staffDefs.empty()) {
        LogError("MusicXML output requires at least one measure and staff definition");
        return "";
    }

    std::map<int, std::set<int>> layersByStaff;
    for (const Measure *measure : measures) {
        if (!measure->IsMeasuredMusic()) {
            LogError("MusicXML output requires measured common notation");
            return "";
        }
        for (const Object *object : measure->FindAllDescendantsByType(LAYER, false)) {
            const Layer *layer = vrv_cast<const Layer *>(object);
            const Staff *staff = vrv_cast<const Staff *>(layer->GetFirstAncestor(STAFF));
            if (!staff || staff->IsOssia() || layer->GetParent() != staff || !scoreDef->GetStaffDef(staff->GetN())
                || layer->GetN() > std::numeric_limits<short>::max()) {
                LogError("MusicXML output cannot map layer '%s' to a staff and voice", layer->GetID().c_str());
                return "";
            }
            layersByStaff[staff->GetN()].insert(layer->GetN());
        }
    }

    std::vector<musicxmloutput::MusicXmlPart> parts;
    std::map<const Object *, size_t> partByDefinition;
    Fraction expectedMeasureDuration;
    for (const Object *object : staffDefs) {
        const StaffDef *staffDef = vrv_cast<const StaffDef *>(object);
        const data_NOTATIONTYPE notation = staffDef->GetNotationtype();
        if (notation != NOTATIONTYPE_NONE && notation != NOTATIONTYPE_cmn) {
            LogError("MusicXML output requires common notation");
            return "";
        }
        const MeterSig *meter = musicxmloutput::GetInitialAttribute<MeterSig>(scoreDef, staffDef, METERSIG);
        const std::pair<int, int> meterValues = musicxmloutput::GetMusicXmlMeter(meter);
        const KeySig *key = musicxmloutput::GetInitialAttribute<KeySig>(scoreDef, staffDef, KEYSIG);
        const Clef *clef = musicxmloutput::GetInitialAttribute<Clef>(scoreDef, staffDef, CLEF);
        if (meterValues.first <= 0 || meterValues.first > 128 || meterValues.second <= 0 || meterValues.second > 1024
            || musicxmloutput::GetInitialAttribute<MeterSigGrp>(scoreDef, staffDef, METERSIGGRP)
            || (key && key->HasNonAttribKeyAccidChildren()) || staffDef->HasTransDiat() || staffDef->HasTransSemi()
            || (clef
                && (clef->HasDis()
                    || (clef->HasShape() && clef->GetShape() != CLEFSHAPE_G && clef->GetShape() != CLEFSHAPE_F
                        && clef->GetShape() != CLEFSHAPE_C)))) {
            LogError("MusicXML output does not support the initial attributes for staff %d yet", staffDef->GetN());
            return "";
        }
        const Fraction measureDuration(meterValues.first * 4, meterValues.second);
        if (!expectedMeasureDuration.GetNumerator()) expectedMeasureDuration = measureDuration;
        if (measureDuration != expectedMeasureDuration) {
            LogError("MusicXML output requires the same measure duration on every staff");
            return "";
        }

        const Object *definition = staffDef;
        const Object *instrument = staffDef->GetFirst(INSTRDEF);
        for (const Object *parent = staffDef->GetParent(); !instrument && parent && parent->Is(STAFFGRP);
            parent = parent->GetParent()) {
            instrument = parent->GetFirst(INSTRDEF);
            if (instrument) definition = parent;
        }
        // Layer instruments belong to voices within the part defined by the staff or staff group.
        const std::pair<std::map<const Object *, size_t>::iterator, bool> position
            = partByDefinition.emplace(definition, parts.size());
        if (position.second) {
            parts.push_back({ definition, vrv_cast<const InstrDef *>(instrument), instrument ? instrument->GetID() : "",
                {}, {}, {} });
        }
        musicxmloutput::MusicXmlPart &part = parts[position.first->second];
        part.staves.push_back({ staffDef, measureDuration, {} });
        musicxmloutput::MusicXmlStaff &staff = part.staves.back();

        std::set<int> &layers = layersByStaff[staffDef->GetN()];
        if (layers.empty()) layers.insert(1);
        for (int layerNumber : layers) staff.voices.try_emplace(layerNumber);
        for (const Object *child : staffDef->GetChildren()) {
            if (!child->Is(LAYERDEF)) continue;
            const LayerDef *layerDef = vrv_cast<const LayerDef *>(child);
            const std::map<int, musicxmloutput::MusicXmlVoiceDef>::iterator voice = staff.voices.find(layerDef->GetN());
            if (voice == staff.voices.end()) continue;
            voice->second.instrument = vrv_cast<const InstrDef *>(layerDef->GetFirst(INSTRDEF));
        }
    }

    int divisions = expectedMeasureDuration.GetDenominator();
    std::set<std::string> ids;
    for (musicxmloutput::MusicXmlPart &part : parts) {
        musicxmloutput::AssignMusicXmlVoices(part);
        for (const Measure *measure : measures) {
            if (!musicxmloutput::CollectMusicXmlPartMeasure(part, measure, divisions)) return "";
        }
        if (!part.id.empty() && !ids.insert(part.id).second) {
            LogError("MusicXML output requires unique instrDef IDs (duplicate '%s')", part.id.c_str());
            return "";
        }
        for (const std::pair<const std::string, const InstrDef *> &entry : part.layerInstruments) {
            if (entry.first.empty() || !ids.insert(entry.first).second) {
                LogError("MusicXML output requires a nonempty, unique ID for each layer instrument ('%s')",
                    entry.first.c_str());
                return "";
            }
        }
    }
    int nextId = 1;
    for (musicxmloutput::MusicXmlPart &part : parts) {
        if (part.id.empty()) {
            do {
                part.id = "P" + std::to_string(nextId++);
            } while (!ids.insert(part.id).second);
        }
    }

    pugi::xml_document xml;
    pugi::xml_node root = xml.append_child("score-partwise");
    root.append_attribute("version") = "4.0";
    const pugi::xml_node title = doc->m_header.select_node("/meiHead/fileDesc/titleStmt/title[1]").node();
    if (*title.text().as_string()) root.append_child("movement-title").text().set(title.text().as_string());
    root.append_child("identification")
        .append_child("encoding")
        .append_child("software")
        .text()
        .set("Verovio MusicXML Export");
    pugi::xml_node partList = root.append_child("part-list");
    for (const musicxmloutput::MusicXmlPart &part : parts) {
        pugi::xml_node scorePart = partList.append_child("score-part");
        scorePart.append_attribute("id") = part.id.c_str();
        const std::string name = musicxmloutput::GetMusicXmlPartName(part);
        pugi::xml_node partName = scorePart.append_child("part-name");
        partName.text().set(name.c_str());
        if (name.empty()) partName.append_attribute("print-object") = "no";
        for (const std::pair<const std::string, const InstrDef *> &entry : part.layerInstruments) {
            pugi::xml_node scoreInstrument = scorePart.append_child("score-instrument");
            scoreInstrument.append_attribute("id") = entry.first.c_str();
            const std::string instrumentName = entry.second->HasLabel() ? entry.second->GetLabel() : entry.first;
            scoreInstrument.append_child("instrument-name").text().set(instrumentName.c_str());
        }
    }
    std::vector<bool> implicitMeasures(measures.size(), false);
    for (const musicxmloutput::MusicXmlPart &part : parts) {
        pugi::xml_node partNode = root.append_child("part");
        partNode.append_attribute("id") = part.id.c_str();
        for (size_t i = 0; i < measures.size(); ++i) {
            const Measure *measure = measures[i];
            pugi::xml_node measureNode = partNode.append_child("measure");
            const std::string number = measure->HasN() ? measure->GetN() : std::to_string(i + 1);
            measureNode.append_attribute("number") = number.c_str();
            if (i == 0) musicxmloutput::WriteMusicXmlAttributes(measureNode, scoreDef, part, divisions);
            int cursor = 0;
            int furthest = 0;
            for (const musicxmloutput::MusicXmlVoice &voice : part.measures[i]) {
                if (cursor) measureNode.append_child("backup").append_child("duration").text().set(cursor);
                cursor = 0;
                for (const musicxmloutput::MusicXmlEvent &event : voice.events) {
                    const int ticks = musicxmloutput::GetMusicXmlTicks(event.duration, divisions);
                    if (ticks > std::numeric_limits<int>::max() - cursor) {
                        LogError("MusicXML output measure duration overflow");
                        return "";
                    }
                    cursor += ticks;
                }
                musicxmloutput::WriteMusicXmlVoice(measureNode, voice, divisions);
                furthest = std::max(furthest, cursor);
            }
            // End on the last occupied beat even when the final voice is shorter.
            if (cursor < furthest) {
                measureNode.append_child("forward").append_child("duration").text().set(furthest - cursor);
            }
            if (&part == &parts.front()) {
                const MNum *mNum = vrv_cast<const MNum *>(measure->GetFirst(MNUM));
                implicitMeasures[i] = measure->GetMetcon() == BOOLEAN_false
                    || (mNum && !mNum->IsGenerated() && mNum->GetText().empty())
                    || furthest != musicxmloutput::GetMusicXmlTicks(expectedMeasureDuration, divisions);
            }
            if (implicitMeasures[i]) measureNode.append_attribute("implicit") = "yes";
        }
    }
    std::set<ClassId> omitted;
    for (const Measure *measure : measures) {
        for (const Object *child : measure->GetChildren()) {
            if (!child->IsControlElement() || child->Is(MNUM) || !omitted.insert(child->GetClassId()).second) continue;
            LogWarning("MusicXML output omits '%s' elements", child->GetClassName().c_str());
        }
    }
    std::ostringstream output;
    xml.save(output, "  ");
    return output.str();
#endif
}

} // namespace vrv

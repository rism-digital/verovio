/////////////////////////////////////////////////////////////////////////////
// Name:        iomusxmloutput.cpp
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "iomusxmloutput.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <limits>
#include <map>
#include <numeric>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>

//----------------------------------------------------------------------------

#include "accid.h"
#include "beam.h"
#include "caesura.h"
#include "clef.h"
#include "comparison.h"
#include "controlelement.h"
#include "doc.h"
#include "durationinterface.h"
#include "dynam.h"
#include "fermata.h"
#include "fraction.h"
#include "functor.h"
#include "hairpin.h"
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
#include "mordent.h"
#include "note.h"
#include "scoredef.h"
#include "slur.h"
#include "smufl.h"
#include "staff.h"
#include "staffdef.h"
#include "timeinterface.h"
#include "transposition.h"
#include "trill.h"
#include "turn.h"
#include "vrv.h"

//----------------------------------------------------------------------------

#include "pugixml.hpp"

namespace vrv {

#ifndef NO_MUSICXML_SUPPORT
namespace musicxmloutput {

    constexpr int maxMusicXmlDivisions = 16384;

    struct MusicXmlEvent {
        const LayerElement *element;
        Fraction duration;
        const char *type;
        int dots;
        const char *beamValue = NULL;
    };

    struct MusicXmlSlurEndpoint {
        const Slur *slur;
        int number;
        bool start;
    };

    struct MusicXmlOrnament {
        const ControlElement *element;
        int lineNumber = 0;
    };

    struct MusicXmlNoteControls {
        bool tieStart = false;
        bool tieStop = false;
        std::vector<MusicXmlSlurEndpoint> slurs;
        std::vector<const ControlElement *> pauses;
        std::vector<MusicXmlOrnament> ornaments;
        int trillStop = 0;
    };

    using MusicXmlNoteControlMap = std::map<const LayerElement *, MusicXmlNoteControls>;

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
        int meterUnit;
        std::map<int, MusicXmlVoiceDef> voices;
    };

    struct MusicXmlWedge {
        const Hairpin *hairpin;
        int number;
        bool start;
    };

    struct MusicXmlDirection {
        std::variant<std::string, MusicXmlWedge> marking;
        data_STAFFREL placement;
        int staff;
        Fraction offset;
    };

    struct MusicXmlBarlineFermatas {
        std::vector<const Fermata *> left;
        std::vector<const Fermata *> right;
    };

    struct MusicXmlPart {
        const Object *definition;
        const InstrDef *instrument;
        std::string id;
        std::vector<MusicXmlStaff> staves;
        std::vector<std::vector<MusicXmlVoice>> measures;
        std::map<std::string, const InstrDef *> layerInstruments;
        std::vector<std::vector<MusicXmlDirection>> directions;
        std::map<size_t, MusicXmlBarlineFermatas> barlineFermatas;
    };

    struct MusicXmlElementPosition {
        size_t part;
        size_t measure;
        size_t order;
        Fraction offset;
        int staff;
        int voice;
    };

    using MusicXmlElementPositionMap = std::map<const LayerElement *, MusicXmlElementPosition>;
    using MusicXmlStaffPositionMap = std::map<int, std::pair<size_t, size_t>>;

    struct MusicXmlAnchor {
        size_t measure;
        std::variant<Fraction, double> position;
    };

    struct MusicXmlHairpinSpan {
        const Hairpin *hairpin;
        size_t part;
        int staff;
        size_t startMeasure;
        size_t endMeasure;
        Fraction startOffset;
        Fraction endOffset;
    };

    struct MusicXmlSlurSpan {
        const Slur *slur;
        const Note *start;
        const Note *end;
        size_t part;
        size_t first;
        size_t last;
    };

    struct MusicXmlTrillSpan {
        const Note *start;
        const Note *end;
        size_t ornament;
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

    bool AddMusicXmlTie(const TimeSpanningInterface &span, MusicXmlNoteControlMap &controls)
    {
        if (!span.HasStartid() || !span.HasEndid()) return false;
        const Note *start = dynamic_cast<const Note *>(span.GetStart());
        const Note *end = dynamic_cast<const Note *>(span.GetEnd());
        if (!start || !end || !start->IsEnharmonicWith(end) || !Object::IsPreOrdered(start, end)) {
            return false;
        }

        const Layer *startLayer = vrv_cast<const Layer *>(start->GetFirstAncestor(LAYER));
        const Layer *endLayer = vrv_cast<const Layer *>(end->GetFirstAncestor(LAYER));
        const Staff *startStaff = start->GetAncestorStaff(ANCESTOR_ONLY, false);
        const Staff *endStaff = end->GetAncestorStaff(ANCESTOR_ONLY, false);
        if (!startLayer || !endLayer || !startStaff || !endStaff) return false;
        if (startStaff->GetN() != endStaff->GetN() || startLayer->GetN() != endLayer->GetN()) return false;
        const Chord *startChord = start->IsChordTone();
        if (startChord && startChord == end->IsChordTone()) return false;

        const MusicXmlNoteControlMap::const_iterator startControls = controls.find(start);
        const MusicXmlNoteControlMap::const_iterator endControls = controls.find(end);
        if ((startControls != controls.end() && startControls->second.tieStart)
            || (endControls != controls.end() && endControls->second.tieStop)) {
            return false;
        }
        controls[start].tieStart = true;
        controls[end].tieStop = true;
        return true;
    }

    const Note *GetMusicXmlNoteAnchor(const LayerElement *element)
    {
        if (element && element->Is(CHORD)) {
            // MusicXML attaches notation for the whole chord to its first note.
            return vrv_cast<const Note *>(element->GetFirst(NOTE));
        }
        return dynamic_cast<const Note *>(element);
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
                if (divisions > maxMusicXmlDivisions) {
                    LogError("MusicXML output duration resolution exceeds 16384 divisions");
                    return false;
                }
            }
        }
        return true;
    }

    template <typename Callback> void ForEachMusicXmlNote(const MusicXmlEvent &event, Callback callback)
    {
        if (event.element && event.element->Is(CHORD)) {
            bool chordTone = false;
            for (const Object *child : event.element->GetChildren()) {
                if (!child->Is(NOTE)) continue;
                callback(vrv_cast<const Note *>(child), chordTone);
                chordTone = true;
            }
        }
        else {
            callback(dynamic_cast<const Note *>(event.element), false);
        }
    }

    int GetMusicXmlTicks(Fraction duration, int divisions)
    {
        // The plan chooses divisions that are multiples of every event denominator.
        return duration.GetNumerator() * (divisions / duration.GetDenominator());
    }

    bool CollectMusicXmlElementPositions(
        const std::vector<MusicXmlPart> &parts, int divisions, MusicXmlElementPositionMap &positions)
    {
        // The order of notes in a MusicXML part can differ from score time when a measure has multiple voices.
        for (size_t part = 0; part < parts.size(); ++part) {
            size_t order = 0;
            const std::vector<std::vector<MusicXmlVoice>> &measures = parts.at(part).measures;
            for (size_t measure = 0; measure < measures.size(); ++measure) {
                for (const MusicXmlVoice &voice : measures.at(measure)) {
                    int tick = 0;
                    for (const MusicXmlEvent &event : voice.events) {
                        const Fraction offset(tick, divisions);
                        if (event.element && !event.element->Is(NOTE)) {
                            positions.emplace(event.element,
                                MusicXmlElementPosition{ part, measure, order, offset, voice.staff, voice.voice });
                        }
                        ForEachMusicXmlNote(
                            event, [&positions, part, measure, &order, offset, &voice](const Note *note, bool) {
                                if (note) {
                                    positions.emplace(note,
                                        MusicXmlElementPosition{
                                            part, measure, order++, offset, voice.staff, voice.voice });
                                }
                            });
                        const int duration = GetMusicXmlTicks(event.duration, divisions);
                        if (duration > std::numeric_limits<int>::max() - tick) {
                            LogError("MusicXML output measure duration overflow");
                            return false;
                        }
                        tick += duration;
                    }
                }
            }
        }
        return true;
    }

    void CollectMusicXmlSlurs(const std::vector<const Slur *> &slurs, const MusicXmlElementPositionMap &positions,
        MusicXmlNoteControlMap &controls)
    {
        std::vector<MusicXmlSlurSpan> spans;
        for (const Slur *slur : slurs) {
            const Note *start = GetMusicXmlNoteAnchor(slur->GetStart());
            const Note *end = GetMusicXmlNoteAnchor(slur->GetEnd());
            const MusicXmlElementPositionMap::const_iterator startPosition = positions.find(start);
            const MusicXmlElementPositionMap::const_iterator endPosition = positions.find(end);
            if (!slur->HasStartid() || !slur->HasEndid() || start == end || startPosition == positions.end()
                || endPosition == positions.end() || startPosition->second.part != endPosition->second.part
                || startPosition->second.measure > endPosition->second.measure) {
                LogWarning("MusicXML output omits unsupported slur '%s'", slur->GetID().c_str());
                continue;
            }
            const size_t first = std::min(startPosition->second.order, endPosition->second.order);
            const size_t last = std::max(startPosition->second.order, endPosition->second.order);
            spans.push_back({ slur, start, end, startPosition->second.part, first, last });
        }
        std::stable_sort(spans.begin(), spans.end(), [](const MusicXmlSlurSpan &one, const MusicXmlSlurSpan &two) {
            return std::tie(one.part, one.first, one.last) < std::tie(two.part, two.first, two.last);
        });

        // MusicXML number-level allows 16 numbers per part. Spans that meet at a note can share a number
        // because the earlier span's endpoint is written first.
        std::array<size_t, 16> availableAt{};
        size_t part = 0;
        for (const MusicXmlSlurSpan &span : spans) {
            if (span.part != part) {
                availableAt.fill(0);
                part = span.part;
            }
            const std::array<size_t, 16>::iterator available = std::find_if(
                availableAt.begin(), availableAt.end(), [&span](size_t last) { return last <= span.first; });
            if (available == availableAt.end()) {
                LogWarning("MusicXML output omits slur '%s' because all 16 slur numbers are in use",
                    span.slur->GetID().c_str());
                continue;
            }
            const int number = static_cast<int>(available - availableAt.begin()) + 1;
            *available = span.last;
            controls[span.start].slurs.push_back({ span.slur, number, true });
            controls[span.end].slurs.push_back({ span.slur, number, false });
        }
    }

    std::optional<Fraction> GetMusicXmlTimestampOffset(double beat, const MusicXmlStaff &staff, int divisions)
    {
        const double quarters = (beat - 1.0) * 4.0 / staff.meterUnit;
        if (!std::isfinite(quarters) || quarters < 0.0 || quarters > staff.measureDuration.ToDouble()) {
            return std::nullopt;
        }

        // Recover the rhythm from the MEI double without changing any already planned ticks.
        for (int resolution = divisions; resolution <= maxMusicXmlDivisions; resolution += divisions) {
            const double ticks = quarters * resolution;
            const double rounded = std::round(ticks);
            if (std::abs(ticks - rounded) < 1e-8) return Fraction(static_cast<int>(rounded), resolution);
        }
        return std::nullopt;
    }

    std::optional<MusicXmlAnchor> GetMusicXmlIdAnchor(
        const LayerElement *element, const MusicXmlElementPositionMap &positions)
    {
        const MusicXmlElementPositionMap::const_iterator position = positions.find(element);
        if (position == positions.end()) return std::nullopt;
        return MusicXmlAnchor{ position->second.measure, position->second.offset };
    }

    std::optional<MusicXmlAnchor> GetMusicXmlStartAnchor(
        const TimePointInterface &control, size_t measure, const MusicXmlElementPositionMap &positions)
    {
        if (control.HasStartid()) return GetMusicXmlIdAnchor(control.GetStart(), positions);
        if (control.HasTstamp()) return MusicXmlAnchor{ measure, control.GetTstamp() };
        return std::nullopt;
    }

    std::optional<MusicXmlAnchor> GetMusicXmlEndAnchor(const TimeSpanningInterface &control, size_t measure,
        size_t measureCount, const MusicXmlElementPositionMap &positions)
    {
        if (control.HasEndid()) return GetMusicXmlIdAnchor(control.GetEnd(), positions);
        if (!control.HasTstamp2()) return std::nullopt;
        const data_MEASUREBEAT beat = control.GetTstamp2();
        if (beat.first < 0 || static_cast<size_t>(beat.first) >= measureCount - measure) return std::nullopt;
        return MusicXmlAnchor{ measure + static_cast<size_t>(beat.first), beat.second };
    }

    std::optional<Fraction> GetMusicXmlDirectionOffset(
        const MusicXmlAnchor &anchor, const MusicXmlStaff &staff, int divisions)
    {
        if (const Fraction *offset = std::get_if<Fraction>(&anchor.position)) {
            if (offset->ToDouble() > staff.measureDuration.ToDouble()) return std::nullopt;
            return *offset;
        }
        return GetMusicXmlTimestampOffset(std::get<double>(anchor.position), staff, divisions);
    }

    MusicXmlStaffPositionMap GetMusicXmlStaffPositions(const std::vector<MusicXmlPart> &parts)
    {
        MusicXmlStaffPositionMap staffPositions;
        for (size_t part = 0; part < parts.size(); ++part) {
            const std::vector<MusicXmlStaff> &staves = parts.at(part).staves;
            for (size_t staff = 0; staff < staves.size(); ++staff) {
                staffPositions.emplace(staves.at(staff).definition->GetN(), std::pair{ part, staff });
            }
        }
        return staffPositions;
    }

    bool IsMusicXmlPauseAnchor(const LayerElement *element)
    {
        return element && element->IsAnyOf(std::array{ NOTE, CHORD, REST, MREST });
    }

    enum class MusicXmlEventMatch { Onset, Before, OnsetOrBefore, End };

    const LayerElement *FindMusicXmlEventAtOffset(const std::vector<MusicXmlVoice> &voices, int staff, double tick,
        MusicXmlEventMatch match, int divisions, int &endTick, int voiceNumber = 0, bool notesOnly = false)
    {
        const LayerElement *target = NULL;
        int latest = -1;
        for (const MusicXmlVoice &voice : voices) {
            int onset = 0;
            for (const MusicXmlEvent &event : voice.events) {
                const int end = onset + GetMusicXmlTicks(event.duration, divisions);
                if (voice.staff == staff && (!voiceNumber || voice.voice == voiceNumber) && onset > latest
                    && IsMusicXmlPauseAnchor(event.element)
                    && (!notesOnly || event.element->IsAnyOf(std::array{ NOTE, CHORD }))) {
                    const double delta = tick - (match == MusicXmlEventMatch::End ? end : onset);
                    const bool samePosition = std::abs(delta) < 1e-8;
                    if ((samePosition && match != MusicXmlEventMatch::Before)
                        || (delta >= 1e-8
                            && (match == MusicXmlEventMatch::Before || match == MusicXmlEventMatch::OnsetOrBefore))) {
                        target = event.element;
                        latest = onset;
                    }
                }
                onset = end;
            }
            endTick = std::max(endTick, onset);
        }
        return target;
    }

    double GetMusicXmlAnchorOffset(const MusicXmlAnchor &anchor, const MusicXmlStaff &staff)
    {
        if (const double *beat = std::get_if<double>(&anchor.position)) return (*beat - 1.0) * 4.0 / staff.meterUnit;
        return std::get<Fraction>(anchor.position).ToDouble();
    }

    bool AddMusicXmlBarlineFermata(MusicXmlPart &part, size_t measure, bool left, const Fermata *fermata)
    {
        MusicXmlBarlineFermatas &barlines = part.barlineFermatas[measure];
        std::vector<const Fermata *> &marks = left ? barlines.left : barlines.right;
        if (std::find(marks.begin(), marks.end(), fermata) != marks.end()) return true;
        // MusicXML permits one fermata above and one below each barline.
        if (marks.size() == 2) return false;
        marks.push_back(fermata);
        return true;
    }

    void CollectMusicXmlPauses(const std::vector<std::pair<size_t, const ControlElement *>> &pauses,
        const std::vector<const Measure *> &measures, const MusicXmlElementPositionMap &positions,
        const MusicXmlStaffPositionMap &staffPositions, std::vector<MusicXmlPart> &parts, int divisions,
        MusicXmlNoteControlMap &controls)
    {
        for (const std::pair<size_t, const ControlElement *> &entry : pauses) {
            const ControlElement *pause = entry.second;
            const Fermata *fermata = dynamic_cast<const Fermata *>(pause);
            const TimePointInterface *timePoint = pause->GetTimePointInterface();
            const std::optional<MusicXmlAnchor> anchor = GetMusicXmlStartAnchor(*timePoint, entry.first, positions);
            bool collected = false;
            if (anchor) {
                const LayerElement *start = timePoint->HasStartid() ? timePoint->GetStart() : NULL;
                const Staff *startStaff = start ? start->GetAncestorStaff(ANCESTOR_ONLY, false) : NULL;
                for (const Staff *staff : timePoint->GetTstampStaves(measures.at(anchor->measure), pause)) {
                    const MusicXmlStaffPositionMap::const_iterator position = staffPositions.find(staff->GetN());
                    if (position == staffPositions.end()) continue;
                    MusicXmlPart &part = parts.at(position->second.first);
                    const size_t staffIndex = position->second.second;
                    const int xmlStaff = part.staves.size() > 1 ? static_cast<int>(staffIndex) + 1 : 0;

                    const LayerElement *target = start;
                    if (!start || startStaff != staff) {
                        const double *beat = std::get_if<double>(&anchor->position);
                        if (fermata && beat && *beat == 0.0) {
                            collected |= AddMusicXmlBarlineFermata(part, anchor->measure, true, fermata);
                            continue;
                        }
                        const double offset = GetMusicXmlAnchorOffset(*anchor, part.staves.at(staffIndex));
                        if (!std::isfinite(offset) || offset < 0.0) continue;
                        const double tick = offset * divisions;
                        int endTick = 0;
                        // A timestamped breath or caesura belongs to the preceding event.
                        const MusicXmlEventMatch match = fermata ? MusicXmlEventMatch::Onset
                            : start                              ? MusicXmlEventMatch::OnsetOrBefore
                                                                 : MusicXmlEventMatch::Before;
                        target = FindMusicXmlEventAtOffset(
                            part.measures.at(anchor->measure), xmlStaff, tick, match, divisions, endTick);
                        if (fermata && beat && std::abs(tick - endTick) < 1e-8) {
                            collected |= AddMusicXmlBarlineFermata(part, anchor->measure, false, fermata);
                            continue;
                        }
                        if (tick > endTick + 1e-8) continue;
                    }
                    if (!IsMusicXmlPauseAnchor(target)) continue;
                    if (target->Is(CHORD)) target = GetMusicXmlNoteAnchor(target);
                    if (!target) continue;
                    controls[target].pauses.push_back(pause);
                    collected = true;
                }
            }
            if (!collected) {
                LogWarning(
                    "MusicXML output could not place %s '%s'", pause->GetClassName().c_str(), pause->GetID().c_str());
            }
        }
    }

    const Note *FindMusicXmlTrillEnd(const MusicXmlAnchor &anchor, const MusicXmlPart &part, size_t staffIndex,
        const MusicXmlElementPosition &start, int divisions)
    {
        const double offset = GetMusicXmlAnchorOffset(anchor, part.staves.at(staffIndex));
        if (!std::isfinite(offset) || offset < 0.0 || anchor.measure < start.measure) return NULL;

        size_t measure = anchor.measure;
        double tick = offset * divisions;
        // MusicXML puts the stop on the last covered note, while MEI uses the position where the line ends.
        if (tick == 0.0) {
            if (measure == start.measure) return NULL;
            const std::vector<MusicXmlVoice> &voices = part.measures.at(--measure);
            const std::vector<MusicXmlVoice>::const_iterator voice = std::find_if(voices.begin(), voices.end(),
                [&start](const MusicXmlVoice &entry) { return entry.voice == start.voice; });
            if (voice == voices.end()) return NULL;
            for (const MusicXmlEvent &event : voice->events) tick += GetMusicXmlTicks(event.duration, divisions);
        }
        int endTick = 0;
        const LayerElement *end = FindMusicXmlEventAtOffset(part.measures.at(measure), start.staff, tick,
            MusicXmlEventMatch::End, divisions, endTick, start.voice, true);
        if (end && end->Is(CHORD)) return vrv_cast<const Note *>(end->GetLast(NOTE));
        return dynamic_cast<const Note *>(end);
    }

    void AddMusicXmlTrillLines(std::vector<MusicXmlTrillSpan> &spans, const MusicXmlElementPositionMap &positions,
        MusicXmlNoteControlMap &controls)
    {
        std::stable_sort(
            spans.begin(), spans.end(), [&positions](const MusicXmlTrillSpan &one, const MusicXmlTrillSpan &two) {
                const MusicXmlElementPosition &left = positions.at(one.start);
                const MusicXmlElementPosition &right = positions.at(two.start);
                return std::tie(left.part, left.order) < std::tie(right.part, right.order);
            });
        std::array<std::optional<size_t>, 16> availableAt{};
        size_t part = 0;
        for (const MusicXmlTrillSpan &span : spans) {
            const MusicXmlElementPosition &start = positions.at(span.start);
            if (start.part != part) {
                availableAt.fill(std::nullopt);
                part = start.part;
            }
            const std::array<std::optional<size_t>, 16>::iterator available = std::find_if(availableAt.begin(),
                availableAt.end(), [&start](std::optional<size_t> end) { return !end || *end < start.order; });
            MusicXmlOrnament &ornament = controls[span.start].ornaments.at(span.ornament);
            const MusicXmlNoteControlMap::const_iterator end = controls.find(span.end);
            if (end != controls.end() && end->second.trillStop) {
                LogWarning(
                    "MusicXML output omits the extender of trill '%s' because another trill ends on the same note",
                    ornament.element->GetID().c_str());
                continue;
            }
            if (available == availableAt.end()) {
                LogWarning(
                    "MusicXML output omits the extender of trill '%s' because all 16 wavy-line numbers are in use",
                    ornament.element->GetID().c_str());
                continue;
            }
            const int number = static_cast<int>(available - availableAt.begin()) + 1;
            *available = positions.at(span.end).order;
            ornament.lineNumber = number;
            controls[span.end].trillStop = number;
        }
    }

    void CollectMusicXmlOrnaments(const std::vector<std::pair<size_t, const ControlElement *>> &ornaments,
        const std::vector<const Measure *> &measures, const MusicXmlElementPositionMap &positions,
        const MusicXmlStaffPositionMap &staffPositions, const std::vector<MusicXmlPart> &parts, int divisions,
        MusicXmlNoteControlMap &controls)
    {
        std::vector<MusicXmlTrillSpan> spans;
        for (const std::pair<size_t, const ControlElement *> &entry : ornaments) {
            const ControlElement *ornament = entry.second;
            const Trill *trill = dynamic_cast<const Trill *>(ornament);
            const Turn *turn = dynamic_cast<const Turn *>(ornament);
            const TimePointInterface &timePoint = *ornament->GetTimePointInterface();
            const std::optional<MusicXmlAnchor> anchor = GetMusicXmlStartAnchor(timePoint, entry.first, positions);
            bool collected = false;
            if (anchor) {
                const LayerElement *start = timePoint.HasStartid() ? timePoint.GetStart() : NULL;
                const Staff *startStaff = start ? start->GetAncestorStaff(ANCESTOR_ONLY, false) : NULL;
                for (const Staff *staff : timePoint.GetTstampStaves(measures.at(anchor->measure), ornament)) {
                    const MusicXmlStaffPositionMap::const_iterator position = staffPositions.find(staff->GetN());
                    if (position == staffPositions.end()) continue;
                    const MusicXmlPart &part = parts.at(position->second.first);
                    const size_t staffIndex = position->second.second;
                    const int xmlStaff = part.staves.size() > 1 ? static_cast<int>(staffIndex) + 1 : 0;
                    const LayerElement *target = start;
                    if (!start || startStaff != staff) {
                        const double offset = GetMusicXmlAnchorOffset(*anchor, part.staves.at(staffIndex));
                        if (!std::isfinite(offset) || offset < 0.0) continue;
                        const MusicXmlEventMatch match = (turn && turn->GetDelayed() == BOOLEAN_true)
                            ? MusicXmlEventMatch::OnsetOrBefore
                            : MusicXmlEventMatch::Onset;
                        int endTick = 0;
                        const double tick = offset * divisions;
                        target = FindMusicXmlEventAtOffset(
                            part.measures.at(anchor->measure), xmlStaff, tick, match, divisions, endTick, 0, true);
                        if (tick > endTick + 1e-8) continue;
                    }
                    const Note *note = GetMusicXmlNoteAnchor(target);
                    if (!note) continue;
                    const Note *endNote = NULL;
                    if (trill && trill->GetExtender() != BOOLEAN_false
                        && (trill->HasEndid() || trill->HasTstamp2() || trill->GetExtender() == BOOLEAN_true)) {
                        const std::optional<MusicXmlAnchor> end
                            = GetMusicXmlEndAnchor(*trill, entry.first, measures.size(), positions);
                        const MusicXmlElementPosition &startPosition = positions.at(note);
                        if (end) {
                            endNote = FindMusicXmlTrillEnd(*end, part, staffIndex, startPosition, divisions);
                        }
                        if (!endNote || positions.at(endNote).order < startPosition.order) {
                            LogWarning(
                                "MusicXML output could not place the extender of trill '%s'", trill->GetID().c_str());
                            endNote = NULL;
                        }
                    }
                    if (trill && trill->GetLstartsym() == LINESTARTENDSYMBOL_none && !endNote) continue;
                    std::vector<MusicXmlOrnament> &noteOrnaments = controls[note].ornaments;
                    // The importer reads one ornament of each kind per MusicXML note.
                    if (std::any_of(
                            noteOrnaments.begin(), noteOrnaments.end(), [&ornament](const MusicXmlOrnament &mark) {
                                return mark.element->GetClassId() == ornament->GetClassId();
                            })) {
                        continue;
                    }
                    if (endNote) spans.push_back({ note, endNote, noteOrnaments.size() });
                    noteOrnaments.push_back({ ornament });
                    collected = true;
                }
            }
            if (!collected) {
                LogWarning("MusicXML output could not place %s '%s'", ornament->GetClassName().c_str(),
                    ornament->GetID().c_str());
            }
        }
        AddMusicXmlTrillLines(spans, positions, controls);
    }

    void AddMusicXmlDirection(MusicXmlPart &part, size_t measure, MusicXmlDirection direction)
    {
        if (part.directions.empty()) part.directions.resize(part.measures.size());
        if (direction.placement == STAFFREL_between) direction.placement = STAFFREL_below;
        part.directions.at(measure).push_back(std::move(direction));
    }

    void CollectMusicXmlDynamics(const std::vector<std::pair<size_t, const Dynam *>> &dynamics,
        const std::vector<const Measure *> &measures, const MusicXmlElementPositionMap &positions,
        const MusicXmlStaffPositionMap &staffPositions, std::vector<MusicXmlPart> &parts, int &divisions)
    {
        for (const std::pair<size_t, const Dynam *> &entry : dynamics) {
            const Dynam *dynam = entry.second;
            const std::string text = UTF32to8(dynam->GetText());
            if (text.empty() || dynam->IsExtenderElement() || dynam->HasEndid() || dynam->HasTstamp2()) {
                LogWarning("MusicXML output omits unsupported dynam '%s'", dynam->GetID().c_str());
                continue;
            }

            const std::optional<MusicXmlAnchor> start = GetMusicXmlStartAnchor(*dynam, entry.first, positions);
            if (!start) {
                LogWarning("MusicXML output could not locate dynam '%s'", dynam->GetID().c_str());
                continue;
            }
            bool collected = false;
            for (const Staff *source : dynam->GetTstampStaves(measures.at(start->measure), dynam)) {
                const MusicXmlStaffPositionMap::const_iterator position = staffPositions.find(source->GetN());
                if (position == staffPositions.end()) continue;
                MusicXmlPart &part = parts.at(position->second.first);
                const size_t staffIndex = position->second.second;
                const MusicXmlStaff &staff = part.staves.at(staffIndex);
                const std::optional<Fraction> offset = GetMusicXmlDirectionOffset(*start, staff, divisions);
                if (!offset) continue;

                divisions = std::lcm(divisions, offset->GetDenominator());
                AddMusicXmlDirection(
                    part, start->measure, { text, dynam->GetPlace(), static_cast<int>(staffIndex) + 1, *offset });
                collected = true;
            }
            if (!collected) {
                LogWarning("MusicXML output could not place dynam '%s' on a staff at a supported offset",
                    dynam->GetID().c_str());
            }
        }
    }

    void PlanMusicXmlHairpinSpans(const std::vector<std::pair<size_t, const Hairpin *>> &hairpins,
        const std::vector<const Measure *> &measures, const MusicXmlElementPositionMap &positions,
        const MusicXmlStaffPositionMap &staffPositions, const std::vector<MusicXmlPart> &parts,
        std::vector<MusicXmlHairpinSpan> &spans, int &divisions)
    {
        for (const std::pair<size_t, const Hairpin *> &entry : hairpins) {
            const Hairpin *hairpin = entry.second;
            const std::optional<MusicXmlAnchor> start = GetMusicXmlStartAnchor(*hairpin, entry.first, positions);
            const std::optional<MusicXmlAnchor> end
                = GetMusicXmlEndAnchor(*hairpin, entry.first, measures.size(), positions);
            if (!start || !end || end->measure < start->measure
                || (hairpin->GetForm() != hairpinLog_FORM_cres && hairpin->GetForm() != hairpinLog_FORM_dim)) {
                LogWarning("MusicXML output omits unsupported hairpin '%s'", hairpin->GetID().c_str());
                continue;
            }

            bool collected = false;
            for (const Staff *source : hairpin->GetTstampStaves(measures.at(start->measure), hairpin)) {
                const MusicXmlStaffPositionMap::const_iterator position = staffPositions.find(source->GetN());
                if (position == staffPositions.end()) continue;
                const size_t partIndex = position->second.first;
                const size_t staffIndex = position->second.second;
                const MusicXmlStaff &staff = parts.at(partIndex).staves.at(staffIndex);
                const std::optional<Fraction> startOffset = GetMusicXmlDirectionOffset(*start, staff, divisions);
                if (!startOffset) continue;
                int resolution = std::lcm(divisions, startOffset->GetDenominator());
                const std::optional<Fraction> endOffset = GetMusicXmlDirectionOffset(*end, staff, resolution);
                if (!endOffset) continue;
                resolution = std::lcm(resolution, endOffset->GetDenominator());
                if (end->measure == start->measure
                    && GetMusicXmlTicks(*endOffset, resolution) <= GetMusicXmlTicks(*startOffset, resolution)) {
                    continue;
                }

                divisions = resolution;
                spans.push_back({ hairpin, partIndex, static_cast<int>(staffIndex) + 1, start->measure, end->measure,
                    *startOffset, *endOffset });
                collected = true;
            }
            if (!collected) {
                LogWarning(
                    "MusicXML output could not place hairpin '%s' at supported offsets", hairpin->GetID().c_str());
            }
        }
    }

    void AddMusicXmlHairpins(
        std::vector<MusicXmlHairpinSpan> &spans, std::vector<MusicXmlPart> &parts, int plannedDivisions, int &divisions)
    {
        if (spans.empty()) return;
        std::stable_sort(spans.begin(), spans.end(),
            [plannedDivisions](const MusicXmlHairpinSpan &left, const MusicXmlHairpinSpan &right) {
                return std::tuple{ left.part, left.startMeasure, GetMusicXmlTicks(left.startOffset, plannedDivisions),
                    left.staff }
                < std::tuple{ right.part, right.startMeasure, GetMusicXmlTicks(right.startOffset, plannedDivisions),
                      right.staff };
            });

        // Number wedges in the order they appear in each part. Reuse a number after its stop.
        std::array<std::pair<size_t, int>, 16> availableAt{};
        size_t partIndex = 0;
        for (const MusicXmlHairpinSpan &span : spans) {
            if (partIndex != span.part) {
                partIndex = span.part;
                availableAt.fill({ 0, 0 });
            }
            const std::pair<size_t, int> start{ span.startMeasure,
                GetMusicXmlTicks(span.startOffset, plannedDivisions) };
            const std::array<std::pair<size_t, int>, 16>::iterator available = std::find_if(availableAt.begin(),
                availableAt.end(), [&start](const std::pair<size_t, int> &end) { return end <= start; });
            if (available == availableAt.end()) {
                LogWarning("MusicXML output omits hairpin '%s' because all 16 wedge numbers are in use",
                    span.hairpin->GetID().c_str());
                continue;
            }
            const int number = static_cast<int>(available - availableAt.begin()) + 1;
            *available = { span.endMeasure, GetMusicXmlTicks(span.endOffset, plannedDivisions) };
            divisions = std::lcm(divisions, span.startOffset.GetDenominator());
            divisions = std::lcm(divisions, span.endOffset.GetDenominator());
            MusicXmlPart &part = parts.at(partIndex);
            const data_STAFFREL placement = span.hairpin->GetPlace();
            AddMusicXmlDirection(part, span.startMeasure,
                { MusicXmlWedge{ span.hairpin, number, true }, placement, span.staff, span.startOffset });
            AddMusicXmlDirection(part, span.endMeasure,
                { MusicXmlWedge{ span.hairpin, number, false }, placement, span.staff, span.endOffset });
        }

        for (MusicXmlPart &part : parts) {
            for (std::vector<MusicXmlDirection> &directions : part.directions) {
                // Keep dynamics in their existing order. Write wedge endpoints chronologically, with a reused
                // number's stop first when a span ends at the next start.
                std::stable_sort(directions.begin(), directions.end(),
                    [divisions](const MusicXmlDirection &left, const MusicXmlDirection &right) {
                        const bool leftWedge = std::holds_alternative<MusicXmlWedge>(left.marking);
                        const bool rightWedge = std::holds_alternative<MusicXmlWedge>(right.marking);
                        if (leftWedge != rightWedge) return rightWedge;
                        return leftWedge
                            && GetMusicXmlTicks(left.offset, divisions) < GetMusicXmlTicks(right.offset, divisions);
                    });
            }
        }
    }

    bool CollectMusicXmlControls(const std::vector<const Measure *> &measures, std::vector<MusicXmlPart> &parts,
        int &divisions, MusicXmlNoteControlMap &controls)
    {
        std::vector<const Slur *> slurs;
        std::vector<std::pair<size_t, const Dynam *>> dynamics;
        std::vector<std::pair<size_t, const Hairpin *>> hairpins;
        std::vector<std::pair<size_t, const ControlElement *>> pauses;
        std::vector<std::pair<size_t, const ControlElement *>> ornaments;
        ClassIdsComparison pauseTypes({ FERMATA, CAESURA, BREATH });
        ClassIdsComparison ornamentTypes({ TRILL, MORDENT, TURN });
        for (size_t i = 0; i < measures.size(); ++i) {
            const Measure *measure = measures.at(i);
            for (const Object *tie : measure->FindAllDescendantsByType(TIE)) {
                // PrepareData resolves the ID references to the exact notes, including tones within chords.
                const TimeSpanningInterface *span = tie->GetTimeSpanningInterface();
                if (!span || !AddMusicXmlTie(*span, controls)) {
                    LogWarning("MusicXML output omits unsupported tie '%s'", tie->GetID().c_str());
                }
            }
            for (const Object *slur : measure->FindAllDescendantsByType(SLUR)) {
                slurs.push_back(vrv_cast<const Slur *>(slur));
            }
            for (const Object *dynam : measure->FindAllDescendantsByType(DYNAM)) {
                dynamics.emplace_back(i, vrv_cast<const Dynam *>(dynam));
            }
            for (const Object *hairpin : measure->FindAllDescendantsByType(HAIRPIN)) {
                hairpins.emplace_back(i, vrv_cast<const Hairpin *>(hairpin));
            }
            ListOfConstObjects measurePauses;
            measure->FindAllDescendantsByComparison(&measurePauses, &pauseTypes);
            for (const Object *pause : measurePauses) {
                pauses.emplace_back(i, vrv_cast<const ControlElement *>(pause));
            }
            ListOfConstObjects measureOrnaments;
            measure->FindAllDescendantsByComparison(&measureOrnaments, &ornamentTypes);
            for (const Object *ornament : measureOrnaments) {
                ornaments.emplace_back(i, vrv_cast<const ControlElement *>(ornament));
            }
        }
        if (slurs.empty() && dynamics.empty() && hairpins.empty() && pauses.empty() && ornaments.empty()) return true;
        MusicXmlElementPositionMap positions;
        if (!CollectMusicXmlElementPositions(parts, divisions, positions)) return false;
        CollectMusicXmlSlurs(slurs, positions, controls);
        if (dynamics.empty() && hairpins.empty() && pauses.empty() && ornaments.empty()) return true;
        const MusicXmlStaffPositionMap staffPositions = GetMusicXmlStaffPositions(parts);
        CollectMusicXmlPauses(pauses, measures, positions, staffPositions, parts, divisions, controls);
        CollectMusicXmlOrnaments(ornaments, measures, positions, staffPositions, parts, divisions, controls);
        CollectMusicXmlDynamics(dynamics, measures, positions, staffPositions, parts, divisions);
        if (!hairpins.empty()) {
            std::vector<MusicXmlHairpinSpan> spans;
            int plannedDivisions = divisions;
            PlanMusicXmlHairpinSpans(hairpins, measures, positions, staffPositions, parts, spans, plannedDivisions);
            AddMusicXmlHairpins(spans, parts, plannedDivisions, divisions);
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

    void WriteMusicXmlDynamic(pugi::xml_node type, const std::string &text)
    {
        static const std::set<std::string> symbols{ "p", "pp", "ppp", "pppp", "ppppp", "pppppp", "f", "ff", "fff",
            "ffff", "fffff", "ffffff", "mp", "mf", "sf", "sfp", "sfpp", "fp", "rf", "rfz", "sfz", "sffz", "fz", "n",
            "pf", "sfzp" };

        pugi::xml_node dynamics = type.append_child("dynamics");
        if (symbols.contains(text)) {
            dynamics.append_child(text.c_str());
        }
        else {
            dynamics.append_child("other-dynamics").text().set(text.c_str());
        }
    }

    void WriteMusicXmlWedge(pugi::xml_node type, const MusicXmlWedge &direction)
    {
        const Hairpin *hairpin = direction.hairpin;
        const bool crescendo = hairpin->GetForm() == hairpinLog_FORM_cres;
        pugi::xml_node wedge = type.append_child("wedge");
        wedge.append_attribute("type") = direction.start ? (crescendo ? "crescendo" : "diminuendo") : "stop";
        wedge.append_attribute("number") = direction.number;
        if (direction.start) {
            if (hairpin->HasLform()) {
                wedge.append_attribute("line-type")
                    = hairpin->AttLineRendBase::LineformToStr(hairpin->GetLform()).c_str();
            }
            const std::string &color = hairpin->GetColor();
            if ((color.size() == 7 || color.size() == 9) && color.front() == '#'
                && color.find_first_not_of("0123456789ABCDEF", 1) == std::string::npos) {
                wedge.append_attribute("color") = color.c_str();
            }
        }
        // A niente circle belongs at the closed end, the spread at the open end.
        if (direction.start == crescendo) {
            if (hairpin->HasNiente()) {
                wedge.append_attribute("niente") = hairpin->GetNiente() == BOOLEAN_true ? "yes" : "no";
            }
        }
        else if (hairpin->GetOpening().GetType() == MEASUREMENTTYPE_vu) {
            const double spread = hairpin->GetOpening().GetVu() * 5.0;
            if (std::isfinite(spread) && spread >= 0.0) {
                wedge.append_attribute("spread") = std::to_string(spread).c_str();
            }
        }
    }

    void WriteMusicXmlDirection(pugi::xml_node measure, const MusicXmlDirection &direction, int divisions)
    {
        pugi::xml_node node = measure.append_child("direction");
        if (direction.placement == STAFFREL_above) node.append_attribute("placement") = "above";
        if (direction.placement == STAFFREL_below) node.append_attribute("placement") = "below";
        pugi::xml_node type = node.append_child("direction-type");
        if (const std::string *text = std::get_if<std::string>(&direction.marking)) {
            WriteMusicXmlDynamic(type, *text);
        }
        else {
            WriteMusicXmlWedge(type, std::get<MusicXmlWedge>(direction.marking));
        }
        const int offset = GetMusicXmlTicks(direction.offset, divisions);
        if (offset) node.append_child("offset").text().set(offset);
        node.append_child("staff").text().set(direction.staff);
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

    void WriteMusicXmlTieEndpoints(pugi::xml_node parent, const char *tag, const MusicXmlNoteControls &controls)
    {
        // At the middle of a chain, close the current tie before opening the next.
        if (controls.tieStop) parent.append_child(tag).append_attribute("type") = "stop";
        if (controls.tieStart) parent.append_child(tag).append_attribute("type") = "start";
    }

    void WriteMusicXmlColor(pugi::xml_node node, const ControlElement &control)
    {
        std::string color = control.GetColor();
        // MusicXML uses uppercase hexadecimal RGB (or ARGB).
        if (color.size() == 7 && color.front() == '#'
            && std::all_of(color.begin() + 1, color.end(), [](unsigned char c) { return std::isxdigit(c); })) {
            std::transform(color.begin() + 1, color.end(), color.begin() + 1,
                [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
            node.append_attribute("color") = color.c_str();
        }
    }

    const char *GetMusicXmlFermataShape(const Fermata &fermata, char32_t glyph)
    {
        switch (glyph) {
            case SMUFL_E4C2_fermataVeryShortAbove:
            case SMUFL_E4C3_fermataVeryShortBelow: return "double-angled";
            case SMUFL_E4C4_fermataShortAbove:
            case SMUFL_E4C5_fermataShortBelow: return "angled";
            case SMUFL_E4C6_fermataLongAbove:
            case SMUFL_E4C7_fermataLongBelow: return "square";
            case SMUFL_E4C8_fermataVeryLongAbove:
            case SMUFL_E4C9_fermataVeryLongBelow: return "double-square";
            case 0xE4CA:
            case 0xE4CB: return "double-dot";
            case 0xE4CC:
            case 0xE4CD: return "half-curve";
            case 0xE4D6: return "curlew";
            case SMUFL_E4C0_fermataAbove:
            case SMUFL_E4C1_fermataBelow: return "normal";
            default: break;
        }
        if (fermata.GetShape() == fermataVis_SHAPE_angular) return "angled";
        if (fermata.GetShape() == fermataVis_SHAPE_square) return "square";
        return "normal";
    }

    void WriteMusicXmlFermata(pugi::xml_node parent, const Fermata &fermata)
    {
        const char32_t glyph = fermata.GetFermataGlyph();
        const data_VERTICALALIGNMENT alignment = Fermata::GetVerticalAlignment(glyph);
        const bool inverted = alignment == VERTICALALIGNMENT_bottom || glyph == 0xE4CB || glyph == 0xE4CD
            || (alignment == VERTICALALIGNMENT_middle && glyph != 0xE4CA && glyph != 0xE4CC
                && (fermata.GetForm() == fermataVis_FORM_inv
                    || (!fermata.HasForm() && fermata.GetPlace() == STAFFREL_below)));
        pugi::xml_node node = parent.append_child("fermata");
        node.append_attribute("type") = inverted ? "inverted" : "upright";
        node.text().set(GetMusicXmlFermataShape(fermata, glyph));
        WriteMusicXmlColor(node, fermata);
    }

    void WriteMusicXmlBarlineFermatas(
        pugi::xml_node measure, const std::vector<const Fermata *> &fermatas, const char *location)
    {
        if (fermatas.empty()) return;
        pugi::xml_node barline = measure.append_child("barline");
        barline.append_attribute("location") = location;
        for (const Fermata *fermata : fermatas) {
            WriteMusicXmlFermata(barline, *fermata);
        }
    }

    void WriteMusicXmlPauseArticulation(pugi::xml_node articulations, const ControlElement &pause)
    {
        const Caesura *caesura = dynamic_cast<const Caesura *>(&pause);
        pugi::xml_node node = articulations.append_child(caesura ? "caesura" : "breath-mark");
        const AttPlacementRelStaff *placement = dynamic_cast<const AttPlacementRelStaff *>(&pause);
        if (placement->GetPlace() == STAFFREL_above) node.append_attribute("placement") = "above";
        if (placement->GetPlace() == STAFFREL_below) node.append_attribute("placement") = "below";
        if (caesura) {
            switch (caesura->GetCaesuraGlyph()) {
                case SMUFL_E4D2_caesuraThick: node.text().set("thick"); break;
                case SMUFL_E4D3_caesuraShort: node.text().set("short"); break;
                case SMUFL_E4D4_caesuraCurved: node.text().set("curved"); break;
                case SMUFL_E4D7_caesuraSingleStroke: node.text().set("single"); break;
                default: break;
            }
        }
        WriteMusicXmlColor(node, pause);
    }

    void WriteMusicXmlOrnamentAccidental(pugi::xml_node ornaments, const ControlElement &ornament,
        data_ACCIDENTAL_WRITTEN accidental, const char *placement)
    {
        if (accidental == ACCIDENTAL_WRITTEN_NONE) return;
        const char *value = GetMusicXmlAccidental(accidental);
        switch (accidental) {
            case ACCIDENTAL_WRITTEN_ss: value = "sharp-sharp"; break;
            case ACCIDENTAL_WRITTEN_tf: value = "triple-flat"; break;
            case ACCIDENTAL_WRITTEN_ts: value = "triple-sharp"; break;
            case ACCIDENTAL_WRITTEN_nf: value = "natural-flat"; break;
            case ACCIDENTAL_WRITTEN_ns: value = "natural-sharp"; break;
            case ACCIDENTAL_WRITTEN_1qf: value = "quarter-flat"; break;
            case ACCIDENTAL_WRITTEN_1qs: value = "quarter-sharp"; break;
            case ACCIDENTAL_WRITTEN_3qf: value = "three-quarters-flat"; break;
            case ACCIDENTAL_WRITTEN_3qs: value = "three-quarters-sharp"; break;
            default: break;
        }
        if (!value) {
            LogWarning("MusicXML output omits an unsupported accidental on %s '%s'", ornament.GetClassName().c_str(),
                ornament.GetID().c_str());
            return;
        }
        pugi::xml_node mark = ornaments.append_child("accidental-mark");
        mark.append_attribute("placement") = placement;
        mark.text().set(value);
    }

    pugi::xml_node WriteMusicXmlWavyLine(pugi::xml_node ornaments, const char *type, int number)
    {
        pugi::xml_node line = ornaments.append_child("wavy-line");
        line.append_attribute("type") = type;
        line.append_attribute("number") = number;
        return line;
    }

    const char *GetMusicXmlTurnName(const Turn &turn, char32_t glyph)
    {
        const bool inverted = turn.GetForm() == turnLog_FORM_lower || glyph == SMUFL_E568_ornamentTurnInverted;
        if (turn.GetDelayed() == BOOLEAN_true) {
            if (inverted || glyph == SMUFL_E56A_ornamentTurnUp || glyph == SMUFL_E56B_ornamentTurnUpS) return NULL;
            return "delayed-turn";
        }
        if (glyph == SMUFL_E56A_ornamentTurnUp) return "vertical-turn";
        if (glyph == SMUFL_E56B_ornamentTurnUpS) return "inverted-vertical-turn";
        return inverted ? "inverted-turn" : "turn";
    }

    void WriteMusicXmlOrnamentStyle(pugi::xml_node node, const ControlElement &ornament)
    {
        const AttPlacementRelStaff *placement = dynamic_cast<const AttPlacementRelStaff *>(&ornament);
        if (placement->GetPlace() == STAFFREL_above) node.append_attribute("placement") = "above";
        if (placement->GetPlace() == STAFFREL_below) node.append_attribute("placement") = "below";
        WriteMusicXmlColor(node, ornament);
    }

    void WriteMusicXmlOrnament(pugi::xml_node notations, const MusicXmlOrnament &marking)
    {
        const ControlElement &ornament = *marking.element;
        const Trill *trill = dynamic_cast<const Trill *>(&ornament);
        const Mordent *mordent = dynamic_cast<const Mordent *>(&ornament);
        const Turn *turn = dynamic_cast<const Turn *>(&ornament);
        const bool lineOnly = trill && trill->GetLstartsym() == LINESTARTENDSYMBOL_none;
        if (lineOnly && !marking.lineNumber) return;
        const char32_t turnGlyph = turn ? turn->GetTurnGlyph() : 0;
        const char *turnName = turn ? GetMusicXmlTurnName(*turn, turnGlyph) : NULL;
        if (turn && !turnName) {
            LogWarning("MusicXML output omits unsupported delayed turn '%s'", turn->GetID().c_str());
            return;
        }
        pugi::xml_node ornaments = notations.append_child("ornaments");
        pugi::xml_node symbol;
        if (trill) {
            symbol = lineOnly ? WriteMusicXmlWavyLine(ornaments, "start", marking.lineNumber)
                              : ornaments.append_child("trill-mark");
        }
        else if (mordent) {
            symbol
                = ornaments.append_child(mordent->GetForm() == mordentLog_FORM_upper ? "inverted-mordent" : "mordent");
            if (mordent->HasLong()) {
                symbol.append_attribute("long") = mordent->GetLong() == BOOLEAN_true ? "yes" : "no";
            }
        }
        else {
            symbol = ornaments.append_child(turnName);
            if (turnGlyph == SMUFL_E569_ornamentTurnSlash) symbol.append_attribute("slash") = "yes";
        }
        WriteMusicXmlOrnamentStyle(symbol, ornament);
        const AttOrnamentAccid *accid = dynamic_cast<const AttOrnamentAccid *>(&ornament);
        if (mordent || lineOnly) {
            // The importer only reads nested accidentals for turns and trills with a trill-mark.
            if (accid->HasAccidupper() || accid->HasAccidlower()) {
                LogWarning("MusicXML output omits unsupported accidentals on %s '%s'", ornament.GetClassName().c_str(),
                    ornament.GetID().c_str());
            }
        }
        else {
            WriteMusicXmlOrnamentAccidental(ornaments, ornament, accid->GetAccidupper(), "above");
            WriteMusicXmlOrnamentAccidental(ornaments, ornament, accid->GetAccidlower(), "below");
        }
        if (marking.lineNumber && !lineOnly) {
            WriteMusicXmlOrnamentStyle(WriteMusicXmlWavyLine(ornaments, "start", marking.lineNumber), ornament);
        }
    }

    void WriteMusicXmlNotations(pugi::xml_node note, const MusicXmlNoteControls &controls)
    {
        pugi::xml_node notations = note.append_child("notations");
        WriteMusicXmlTieEndpoints(notations, "tied", controls);
        for (const MusicXmlSlurEndpoint &endpoint : controls.slurs) {
            pugi::xml_node slur = notations.append_child("slur");
            slur.append_attribute("type") = endpoint.start ? "start" : "stop";
            slur.append_attribute("number") = endpoint.number;
            if (!endpoint.start) continue;
            const curvature_CURVEDIR curvedir = endpoint.slur->GetCurvedir();
            if (curvedir == curvature_CURVEDIR_above) slur.append_attribute("placement") = "above";
            if (curvedir == curvature_CURVEDIR_below) slur.append_attribute("placement") = "below";
            if (endpoint.slur->HasLform()) {
                slur.append_attribute("line-type")
                    = endpoint.slur->AttLineRendBase::LineformToStr(endpoint.slur->GetLform()).c_str();
            }
        }
        for (const ControlElement *pause : controls.pauses) {
            if (pause->Is(FERMATA)) {
                WriteMusicXmlFermata(notations, *vrv_cast<const Fermata *>(pause));
            }
        }
        pugi::xml_node articulations;
        for (const ControlElement *pause : controls.pauses) {
            if (!pause->Is(FERMATA)) {
                if (!articulations) articulations = notations.append_child("articulations");
                WriteMusicXmlPauseArticulation(articulations, *pause);
            }
        }
        // Use the same order as the importer when several kinds of ornaments share a note.
        for (ClassId kind : { MORDENT, TRILL, TURN }) {
            const std::vector<MusicXmlOrnament>::const_iterator ornament = std::find_if(controls.ornaments.begin(),
                controls.ornaments.end(), [kind](const MusicXmlOrnament &mark) { return mark.element->Is(kind); });
            if (ornament != controls.ornaments.end()) WriteMusicXmlOrnament(notations, *ornament);
        }
        if (controls.trillStop) {
            WriteMusicXmlWavyLine(notations.append_child("ornaments"), "stop", controls.trillStop);
        }
        if (!notations.first_child()) note.remove_child(notations);
    }

    void WriteMusicXmlNote(pugi::xml_node measure, const MusicXmlEvent &event, const Note *note, bool chordTone,
        int divisions, const MusicXmlVoice &voice, const MusicXmlNoteControlMap &noteControls)
    {
        pugi::xml_node node = measure.append_child("note");
        const MusicXmlNoteControlMap::const_iterator control = noteControls.find(note ? note : event.element);
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
        if (control != noteControls.end()) WriteMusicXmlTieEndpoints(node, "tie", control->second);
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
        if (control != noteControls.end()) {
            WriteMusicXmlNotations(node, control->second);
        }
    }

    void WriteMusicXmlVoice(
        pugi::xml_node measure, const MusicXmlVoice &voice, int divisions, const MusicXmlNoteControlMap &noteControls)
    {
        for (const MusicXmlEvent &event : voice.events) {
            ForEachMusicXmlNote(event, [&](const Note *note, bool chordTone) {
                WriteMusicXmlNote(measure, event, note, chordTone, divisions, voice, noteControls);
            });
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
                {}, {}, {}, {}, {} });
        }
        musicxmloutput::MusicXmlPart &part = parts[position.first->second];
        part.staves.push_back({ staffDef, measureDuration, meterValues.second, {} });
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

    musicxmloutput::MusicXmlNoteControlMap noteControls;
    if (!musicxmloutput::CollectMusicXmlControls(measures, parts, divisions, noteControls)) return "";

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
            const std::map<size_t, musicxmloutput::MusicXmlBarlineFermatas>::const_iterator barlines
                = part.barlineFermatas.find(i);
            if (barlines != part.barlineFermatas.end()) {
                musicxmloutput::WriteMusicXmlBarlineFermatas(measureNode, barlines->second.left, "left");
            }
            if (i == 0) musicxmloutput::WriteMusicXmlAttributes(measureNode, scoreDef, part, divisions);
            // Directions are written at cursor zero, so their offsets are relative to the start of the measure.
            if (!part.directions.empty()) {
                for (const musicxmloutput::MusicXmlDirection &direction : part.directions.at(i)) {
                    musicxmloutput::WriteMusicXmlDirection(measureNode, direction, divisions);
                }
            }
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
                musicxmloutput::WriteMusicXmlVoice(measureNode, voice, divisions, noteControls);
                furthest = std::max(furthest, cursor);
            }
            // End on the last occupied beat even when the final voice is shorter.
            if (cursor < furthest) {
                measureNode.append_child("forward").append_child("duration").text().set(furthest - cursor);
            }
            if (barlines != part.barlineFermatas.end()) {
                musicxmloutput::WriteMusicXmlBarlineFermatas(measureNode, barlines->second.right, "right");
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
            if (!child->IsControlElement()
                || child->IsAnyOf(
                    std::array{ BREATH, CAESURA, DYNAM, FERMATA, HAIRPIN, MNUM, MORDENT, SLUR, TIE, TRILL, TURN })
                || !omitted.insert(child->GetClassId()).second) {
                continue;
            }
            LogWarning("MusicXML output omits '%s' elements", child->GetClassName().c_str());
        }
    }
    std::ostringstream output;
    xml.save(output, "  ");
    return output.str();
#endif
}

} // namespace vrv

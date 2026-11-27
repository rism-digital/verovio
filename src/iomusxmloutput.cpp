/////////////////////////////////////////////////////////////////////////////
// Name:        iomusxmloutput.cpp
// Author:      Laurent Pugin (LLM assisted coding)
// Created:     22/11/2026
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "iomusxmloutput.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <array>
#include <cctype>
#include <charconv>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <limits>
#include <locale>
#include <map>
#include <numeric>
#include <optional>
#include <regex>
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
#include "dir.h"
#include "doc.h"
#include "durationinterface.h"
#include "dynam.h"
#include "fermata.h"
#include "fraction.h"
#include "functor.h"
#include "gracegrp.h"
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
#include "multirest.h"
#include "note.h"
#include "rend.h"
#include "scoredef.h"
#include "slur.h"
#include "smufl.h"
#include "staff.h"
#include "staffdef.h"
#include "syl.h"
#include "tempo.h"
#include "text.h"
#include "timeinterface.h"
#include "transposition.h"
#include "trill.h"
#include "tuplet.h"
#include "turn.h"
#include "verse.h"
#include "vrv.h"

//----------------------------------------------------------------------------

#include "pugixml.hpp"

namespace vrv {

#ifndef NO_MUSICXML_SUPPORT
namespace musicxmloutput {

    constexpr int maxMusicXmlDivisions = 16384;
    constexpr int maxMusicXmlBeamLevels = 8;

    enum class MusicXmlBeamValue : std::uint8_t { None, Begin, Continue, End, ForwardHook, BackwardHook };

    struct MusicXmlEvent {
        const LayerElement *element;
        Fraction duration;
        const char *type;
        int dots;
        const Tuplet *tuplet = NULL;
        data_GRACE grace = GRACE_NONE;
        bool tupletStart = false;
        bool tupletStop = false;
        std::array<MusicXmlBeamValue, maxMusicXmlBeamLevels> beams{};
        int startTick = 0;
        int endTick = 0;
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

    struct MusicXmlLyric {
        const Verse *verse;
        std::vector<const Syl *> syllables;
        bool extend = false;
    };

    struct MusicXmlNoteControls {
        bool tieStart = false;
        bool tieStop = false;
        std::vector<MusicXmlSlurEndpoint> slurs;
        std::vector<const ControlElement *> pauses;
        std::vector<MusicXmlOrnament> ornaments;
        std::map<int, MusicXmlLyric> lyrics;
        std::set<int> lyricStops;
        int trillStop = 0;
    };

    using MusicXmlNoteControlMap = std::map<const LayerElement *, MusicXmlNoteControls>;

    struct MusicXmlClefChange {
        size_t beforeEvent;
        const Clef *clef;
    };

    struct MusicXmlVoice {
        int staff;
        int voice;
        const InstrDef *instrument;
        std::vector<MusicXmlEvent> events;
        int durationTicks = 0;
        std::vector<MusicXmlClefChange> clefs{};
    };

    struct MusicXmlMeter {
        Fraction GetDuration() const { return Fraction(count * 4, unit); }

        int count;
        int unit;
    };

    struct MusicXmlMeasureSignatures {
        const KeySig *key = NULL;
        const MeterSig *time = NULL;
    };

    struct MusicXmlMeasure {
        std::vector<MusicXmlVoice> voices;
        int durationTicks = 0;
        std::optional<MusicXmlMeter> meterOverride;
        const MultiRest *multiRest = NULL;
    };

    struct MusicXmlVoiceDef {
        int number = 0;
        const InstrDef *instrument = NULL;
    };

    struct MusicXmlStaff {
        const StaffDef *definition;
        MusicXmlMeter meter;
        const Clef *clef;
        std::map<int, MusicXmlVoiceDef> voices;
    };

    struct MusicXmlWords {
        const Dir *directive;
        std::string text;
    };

    struct MusicXmlTempo {
        std::string text;
        std::string language;
        const char *beatUnit = NULL;
        std::string perMinute;
        int dots = 0;
        bool parentheses = false;
        std::string bpm;
    };

    enum class MusicXmlDirectionKind { Dynamic, Words, Wedge, Tempo };
    enum class MusicXmlDirectionEndpoint { Point, Start, Stop };

    struct MusicXmlDirection {
        MusicXmlDirectionKind GetKind() const
        {
            if (std::holds_alternative<std::string>(marking)) return MusicXmlDirectionKind::Dynamic;
            if (std::holds_alternative<MusicXmlWords>(marking)) return MusicXmlDirectionKind::Words;
            if (std::holds_alternative<const Hairpin *>(marking)) return MusicXmlDirectionKind::Wedge;
            return MusicXmlDirectionKind::Tempo;
        }

        std::variant<std::string, MusicXmlWords, const Hairpin *, MusicXmlTempo> marking;
        data_STAFFREL placement;
        int staff;
        Fraction offset;
        MusicXmlDirectionEndpoint endpoint = MusicXmlDirectionEndpoint::Point;
        int number = 0;
    };

    struct MusicXmlBarline {
        const char *style = NULL;
        const char *repeat = NULL;
    };

    struct MusicXmlMeasureBarlines {
        MusicXmlBarline left;
        MusicXmlBarline right;
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
        std::vector<MusicXmlMeasure> measures;
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

    struct MusicXmlDirectionTarget {
        int GetStaffNumber() const { return static_cast<int>(staff) + 1; }

        size_t part;
        size_t staff;
        Fraction offset;
    };

    using MusicXmlDirectionTime = std::pair<size_t, int>;

    struct MusicXmlDirectionSpan {
        // The index refers to words already collected in the start measure.
        std::variant<size_t, const Hairpin *> source;
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

        FunctorCode VisitScoreDef(const ScoreDef *scoreDef) override
        {
            m_scoreDefs.emplace_back(m_measures.size(), scoreDef);
            return FUNCTOR_SIBLINGS;
        }

    public:
        std::vector<const Measure *> m_measures;
        std::vector<std::pair<size_t, const ScoreDef *>> m_scoreDefs;
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
        if (!note->HasPname() || octave < 0 || octave > 9 || note->HasStaff() || note->HasPnameGes()
            || note->HasOctGes() || note->HasTie()) {
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

    const Clef *ResolveMusicXmlClef(const Clef *clef)
    {
        if (!clef->HasSameas()) return clef;
        const Clef *target = dynamic_cast<const Clef *>(clef->GetSameasLink());
        return target && !target->HasSameas() ? target : NULL;
    }

    bool IsSupportedMusicXmlClef(const Clef &clef)
    {
        if (clef.HasDis() && !clef.HasDisPlace()) return false;
        return clef.GetShape() == CLEFSHAPE_G || clef.GetShape() == CLEFSHAPE_F || clef.GetShape() == CLEFSHAPE_C;
    }

    std::optional<MusicXmlMeter> GetMusicXmlMeter(const MeterSig *meter)
    {
        if (!meter) return MusicXmlMeter{ 4, 4 };
        if ((meter->HasCount() && meter->GetCount().first.size() != 1) || meter->GetSym() == METERSIGN_open) {
            return std::nullopt;
        }
        const bool cut = (meter->GetSym() == METERSIGN_cut);
        const int count = meter->HasCount() ? meter->GetCount().first.front() : (cut ? 2 : 4);
        const int unit = meter->HasUnit() ? meter->GetUnit() : (cut ? 2 : 4);
        if (count <= 0 || count > 128 || unit <= 0 || unit > 1024) {
            return std::nullopt;
        }
        return MusicXmlMeter{ count, unit };
    }

    const MusicXmlMeter &GetMusicXmlMeasureMeter(const MusicXmlPart &part, size_t measure, size_t staff)
    {
        const std::optional<MusicXmlMeter> &meter = part.measures.at(measure).meterOverride;
        return meter ? *meter : part.staves.at(staff).meter;
    }

    bool CollectMusicXmlMeasureSignatures(const std::vector<std::pair<size_t, const ScoreDef *>> &scoreDefs,
        std::vector<MusicXmlMeasureSignatures> &signatures)
    {
        for (const std::pair<size_t, const ScoreDef *> &entry : scoreDefs) {
            const ScoreDef *scoreDef = entry.second;
            ArrayOfStrAttr attributes;
            scoreDef->GetAttributes(&attributes);
            // The importer can attach ppq to a signature change. The export resolution is computed for the score.
            if (std::any_of(attributes.begin(), attributes.end(),
                    [](const std::pair<std::string, std::string> &attribute) { return attribute.first != "ppq"; })) {
                LogError("MusicXML output supports only key and time signature changes in scoreDef '%s'",
                    scoreDef->GetID().c_str());
                return false;
            }
            MusicXmlMeasureSignatures change;
            for (const Object *child : scoreDef->GetChildren()) {
                if (child->Is(KEYSIG) && !vrv_cast<const KeySig *>(child)->HasNonAttribKeyAccidChildren()) {
                    change.key = vrv_cast<const KeySig *>(child);
                }
                else if (child->Is(METERSIG) && GetMusicXmlMeter(vrv_cast<const MeterSig *>(child))) {
                    change.time = vrv_cast<const MeterSig *>(child);
                }
                else {
                    LogError("MusicXML output does not support '%s' in a mid-score scoreDef (xml:id '%s')",
                        child->GetClassName().c_str(), child->GetID().c_str());
                    return false;
                }
            }
            if (entry.first < signatures.size()) {
                MusicXmlMeasureSignatures &measure = signatures.at(entry.first);
                if (change.key) measure.key = change.key;
                if (change.time) measure.time = change.time;
            }
        }
        return true;
    }

    data_GRACE GetMusicXmlGrace(const LayerElement *element)
    {
        const AttGraced *graced = dynamic_cast<const AttGraced *>(element);
        if (graced && graced->HasGrace()) return graced->GetGrace();
        const GraceGrp *group = vrv_cast<const GraceGrp *>(element->GetFirstAncestor(GRACEGRP));
        if (!group) return GRACE_NONE;
        return group->HasGrace() ? group->GetGrace() : GRACE_acc;
    }

    bool AddMusicXmlEvent(
        const LayerElement *element, Fraction measureDuration, MusicXmlVoice &voice, const Tuplet *tuplet)
    {
        const DurationInterface *duration = element->GetDurationInterface();
        const ClassId classId = element->GetClassId();
        if (element->HasSameas()) return false;
        const data_GRACE grace = GetMusicXmlGrace(element);
        // The importer has no grace representation for rests or spaces.
        if (grace != GRACE_NONE && classId != NOTE && classId != CHORD) {
            return false;
        }
        if (classId == MREST || classId == MSPACE) {
            if (tuplet) return false;
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
        if (grace != GRACE_NONE) {
            voice.events.push_back({ element, Fraction(), type, dots, NULL, grace });
            return true;
        }
        // Verovio durations are fractions of a whole note. MusicXML divisions are per quarter note.
        const Fraction written = duration->GetInterfaceAlignmentDuration(1, 1) * 4;
        const Fraction sounding = tuplet ? written * Fraction(tuplet->GetNumbase(), tuplet->GetNum()) : written;
        if (tuplet && classId == REST && tuplet->GetNum() != tuplet->GetNumbase()
            && sounding.ToDouble() == measureDuration.ToDouble()
            && std::none_of(voice.events.begin(), voice.events.end(),
                [](const MusicXmlEvent &event) { return event.grace == GRACE_NONE; })) {
            // The importer would interpret this as a measure rest, which cannot be a child of a tuplet.
            return false;
        }
        voice.events.push_back({ element, sounding, type, dots, tuplet });
        return true;
    }

    struct MusicXmlBeamPosition {
        bool ConnectsTo(const MusicXmlBeamPosition &next, int level) const
        {
            return level <= levels && level <= next.levels && (breaksec <= 0 || level <= breaksec);
        }

        size_t index;
        int levels;
        int breaksec;
    };

    bool PlanMusicXmlBeam(std::vector<MusicXmlEvent> &events, size_t first, bool nested)
    {
        const bool allGrace = std::all_of(events.cbegin() + first, events.cend(),
            [](const MusicXmlEvent &event) { return event.grace != GRACE_NONE; });
        // Only grace beams can be nested. A regular beam runs past the grace notes inside it.
        if (nested && !allGrace) return false;
        std::vector<size_t> members;
        for (size_t i = first; i < events.size(); ++i) {
            if ((events.at(i).grace != GRACE_NONE) == allGrace) members.push_back(i);
        }
        if (members.size() < 2 || events.at(members.front()).element->Is(REST)
            || events.at(members.back()).element->Is(REST)) {
            return false;
        }
        const MusicXmlEvent &start = events.at(members.front());
        if (start.tupletStart) {
            // The importer uses the next tuplet stop and the first beam stop to order the two containers.
            if (start.tupletStop) return false;
            bool graceBeamEnded = false;
            for (size_t i = members.front() + 1; i < members.back(); ++i) {
                const MusicXmlEvent &event = events.at(i);
                if (event.tupletStop) {
                    if (graceBeamEnded) {
                        LogError("MusicXML output cannot preserve a tuplet ending within a beam after a grace beam");
                        return false;
                    }
                    break;
                }
                if (event.grace != GRACE_NONE && event.beams.front() == MusicXmlBeamValue::End) {
                    graceBeamEnded = true;
                }
            }
        }

        std::vector<MusicXmlBeamPosition> positions;
        for (size_t i : members) {
            MusicXmlEvent &event = events.at(i);
            if (event.beams.front() != MusicXmlBeamValue::None) return false;
            event.beams.front() = MusicXmlBeamValue::Continue;
            // Secondary beams connect sounding notes and can pass across rests.
            if (event.element->Is(REST)) continue;
            const DurationInterface *duration = event.element->GetDurationInterface();
            const data_DURATION written = duration->GetActualDur();
            if (written < DURATION_8 || written > DURATION_1024) return false;
            positions.push_back({ i, written - DURATION_4, duration->GetBreaksec() });
        }
        events.at(members.front()).beams.front() = MusicXmlBeamValue::Begin;
        events.at(members.back()).beams.front() = MusicXmlBeamValue::End;

        for (size_t i = 0; i < positions.size(); ++i) {
            const MusicXmlBeamPosition &position = positions.at(i);
            MusicXmlEvent &event = events.at(position.index);
            const MusicXmlBeamPosition *previous = i ? &positions.at(i - 1) : NULL;
            const MusicXmlBeamPosition *next = i + 1 < positions.size() ? &positions.at(i + 1) : NULL;
            for (int level = 2; level <= position.levels; ++level) {
                const bool left = previous && previous->ConnectsTo(position, level);
                const bool right = next && position.ConnectsTo(*next, level);
                MusicXmlBeamValue &value = event.beams.at(level - 1);
                if (right) {
                    value = left ? MusicXmlBeamValue::Continue : MusicXmlBeamValue::Begin;
                    for (size_t rest = position.index + 1; rest < next->index; ++rest) {
                        if (events.at(rest).element->Is(REST)) {
                            events.at(rest).beams.at(level - 1) = MusicXmlBeamValue::Continue;
                        }
                    }
                }
                else if (left) {
                    value = MusicXmlBeamValue::End;
                }
                else if (!previous) {
                    value = MusicXmlBeamValue::ForwardHook;
                }
                else if (!next) {
                    value = MusicXmlBeamValue::BackwardHook;
                }
                else if (previous->levels >= level && previous->breaksec > 0 && previous->breaksec < level) {
                    value = events.at(position.index - 1).element->Is(REST) ? MusicXmlBeamValue::BackwardHook
                                                                            : MusicXmlBeamValue::ForwardHook;
                }
                else {
                    // Match the secondary hook direction implied by the surrounding beams.
                    const MusicXmlBeamValue lower = event.beams.at(level - 2);
                    const bool backward = level == 2 || previous->ConnectsTo(position, level - 1)
                        || lower == MusicXmlBeamValue::BackwardHook;
                    value = backward ? MusicXmlBeamValue::BackwardHook : MusicXmlBeamValue::ForwardHook;
                }
            }

            // The importer can retain an explicit break only when a secondary beam ends here.
            if (next && position.breaksec > 0 && position.breaksec < std::min(position.levels, next->levels)
                && event.beams.at(position.breaksec) != MusicXmlBeamValue::End) {
                return false;
            }
        }
        return true;
    }

    bool AddMusicXmlLayerElement(const LayerElement *element, Fraction measureDuration, MusicXmlVoice &voice,
        const Tuplet *tuplet = NULL, bool inBeam = false)
    {
        if (element->Is(CLEF)) {
            const Clef *clef = vrv_cast<const Clef *>(element);
            const Clef *source = ResolveMusicXmlClef(clef);
            if (!source || !IsSupportedMusicXmlClef(*source)) return false;
            voice.clefs.push_back({ voice.events.size(), clef });
            return true;
        }
        const Beam *beam = dynamic_cast<const Beam *>(element);
        const Tuplet *group = dynamic_cast<const Tuplet *>(element);
        const bool graceGroup = element->Is(GRACEGRP);
        if (!beam && !group && !graceGroup) {
            if (inBeam && !element->IsAnyOf(std::array{ NOTE, CHORD, REST })) {
                return false;
            }
            return AddMusicXmlEvent(element, measureDuration, voice, tuplet);
        }

        if (element->HasSameas()) return false;
        if (beam && (beam->HasBeamWith() || (beam->HasForm() && beam->GetForm() != beamRend_FORM_norm))) {
            return false;
        }
        if (group
            && (tuplet || group->GetNum() <= 0 || group->GetNumbase() <= 0
                || group->GetNum() > std::numeric_limits<short>::max()
                || group->GetNumbase() > std::numeric_limits<short>::max())) {
            return false;
        }

        size_t first = voice.events.size();
        for (const Object *child : element->GetChildren()) {
            if (group && child->IsAnyOf(std::array{ TUPLET_BRACKET, TUPLET_NUM })) continue;
            const LayerElement *member = dynamic_cast<const LayerElement *>(child);
            if (!member
                || !AddMusicXmlLayerElement(member, measureDuration, voice, group ? group : tuplet, beam || inBeam)) {
                return false;
            }
        }
        if (first == voice.events.size()) return false;
        if (graceGroup) return true;
        if (beam) return PlanMusicXmlBeam(voice.events, first, inBeam);
        while (first < voice.events.size() && voice.events.at(first).grace != GRACE_NONE) ++first;
        if (first == voice.events.size()) return false;
        size_t last = voice.events.size() - 1;
        while (voice.events.at(last).grace != GRACE_NONE) --last;
        voice.events.at(first).tupletStart = true;
        voice.events.at(last).tupletStop = true;
        return true;
    }

    // MusicXML uses one cursor for the entire part-measure. Each MEI layer is emitted as a separate run on this cursor.
    bool CollectMusicXmlVoice(const Layer *layer, Fraction measureDuration, MusicXmlVoice &voice)
    {
        for (const Object *object : layer->GetChildren()) {
            const LayerElement *element = dynamic_cast<const LayerElement *>(object);
            if (element && AddMusicXmlLayerElement(element, measureDuration, voice)) continue;
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

    bool AddMusicXmlDuration(Fraction duration, int &divisions)
    {
        // Bound the resolution for consumers that calculate the measure size using integer arithmetic.
        const int denominator = duration.GetDenominator();
        if (denominator <= maxMusicXmlDivisions) {
            divisions = std::lcm(divisions, denominator);
            if (divisions <= maxMusicXmlDivisions) return true;
        }
        LogError("MusicXML output duration resolution exceeds 16384 divisions");
        return false;
    }

    bool CollectMusicXmlMultiRests(const std::vector<const Measure *> &measures, std::vector<const MultiRest *> &rests)
    {
        size_t expandedCount = measures.size();
        for (size_t i = 0; i < measures.size(); ++i) {
            const Measure *measure = measures[i];
            const MultiRest *rest = vrv_cast<const MultiRest *>(measure->FindDescendantByType(MULTIREST));
            if (!rest) continue;
            if (rest->GetNum() < 2
                || expandedCount > static_cast<size_t>(std::numeric_limits<int>::max() - rest->GetNum() + 1)) {
                LogError("MusicXML output cannot expand multiRest '%s' with count %d", rest->GetID().c_str(),
                    rest->GetNum());
                return false;
            }
            // The importer skips the covered measures in every part.
            for (const Object *layer : measure->FindAllDescendantsByType(LAYER, false)) {
                bool hasRest = false;
                for (const Object *child : layer->GetChildren()) {
                    // Initial attributes with a multiple-rest marker can import as a clef before the rest.
                    if (i == 0 && !hasRest && child->Is(CLEF)) continue;
                    const MultiRest *other = dynamic_cast<const MultiRest *>(child);
                    if (hasRest
                        || (other && (other->GetNum() != rest->GetNum() || other->GetBlock() != rest->GetBlock()))
                        || !child->IsAnyOf(std::array{ MULTIREST, MREST, MSPACE })) {
                        LogError("MusicXML output requires matching multi-measure rests or silence in measure '%s'",
                            measure->GetID().c_str());
                        return false;
                    }
                    hasRest = true;
                }
            }
            expandedCount += rest->GetNum() - 1;
            rests[i] = rest;
        }
        return true;
    }

    bool CollectMusicXmlPartMeasure(MusicXmlPart &part, const Measure *measure, const MultiRest *multiRest,
        std::optional<MusicXmlMeter> meterOverride, int &divisions)
    {
        MusicXmlMeasure &planned = part.measures.emplace_back();
        planned.meterOverride = meterOverride;
        planned.multiRest = multiRest;
        std::vector<MusicXmlVoice> &voices = planned.voices;
        int staffNumber = 0;
        for (const MusicXmlStaff &staff : part.staves) {
            ++staffNumber;
            const Fraction measureDuration = (meterOverride ? *meterOverride : staff.meter).GetDuration();
            if (!AddMusicXmlDuration(measureDuration, divisions)) return false;
            const int xmlStaff = part.staves.size() > 1 ? staffNumber : 0;
            AttNIntegerComparison matchStaff(STAFF, staff.definition->GetN());
            const Staff *staffElement = vrv_cast<const Staff *>(measure->FindDescendantByComparison(&matchStaff, 1));
            if (multiRest) {
                // Write a timed measure rest on each staff of every expanded measure.
                MusicXmlVoice &voice = voices.emplace_back(MusicXmlVoice{ xmlStaff, staffNumber, NULL, {} });
                if (staffElement) {
                    for (const Object *object : staffElement->FindAllDescendantsByType(CLEF)) {
                        if (!AddMusicXmlLayerElement(vrv_cast<const Clef *>(object), measureDuration, voice)) {
                            LogError("MusicXML output does not support clef '%s'", object->GetID().c_str());
                            return false;
                        }
                    }
                }
                voice.events.push_back({ multiRest, measureDuration, NULL, 0 });
                continue;
            }
            const size_t first = voices.size();
            if (staffElement) {
                for (const Object *child : staffElement->GetChildren()) {
                    if (!child->Is(LAYER)) continue;
                    const Layer *layer = vrv_cast<const Layer *>(child);
                    const std::map<int, MusicXmlVoiceDef>::const_iterator voiceDef = staff.voices.find(layer->GetN());
                    if (voiceDef == staff.voices.end()) continue;
                    MusicXmlVoice voice{ xmlStaff, voiceDef->second.number, voiceDef->second.instrument, {} };
                    if (!CollectMusicXmlVoice(layer, measureDuration, voice)) return false;
                    if (voice.events.empty() && !voice.clefs.empty()) {
                        voice.events.push_back({ NULL, measureDuration, NULL, 0 });
                    }
                    if (!voice.events.empty()) voices.push_back(std::move(voice));
                }
            }
            if (voices.size() == first) {
                // A silent staff still needs a voice run so re-import associates the next run with its own staff.
                const MusicXmlVoiceDef &voiceDef = staff.voices.begin()->second;
                voices.push_back(
                    { xmlStaff, voiceDef.number, voiceDef.instrument, { { NULL, measureDuration, NULL, 0 } } });
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
                if (!AddMusicXmlDuration(event.duration, divisions)) return false;
            }
        }
        return true;
    }

    bool PlaceMusicXmlTrailingClefs(MusicXmlMeasure &measure)
    {
        for (size_t i = 0; i + 1 < measure.voices.size(); ++i) {
            MusicXmlVoice &voice = measure.voices.at(i);
            if (measure.voices.at(i + 1).staff != voice.staff) continue;
            const std::vector<MusicXmlClefChange>::iterator trailing
                = std::find_if(voice.clefs.begin(), voice.clefs.end(),
                    [&voice](const MusicXmlClefChange &change) { return change.beforeEvent == voice.events.size(); });
            if (trailing == voice.clefs.end()) continue;

            // A clef immediately before a backup can be assigned to the next voice by the importer.
            // Place it at the same score time in a following voice that has an event boundary there.
            MusicXmlVoice *target = NULL;
            size_t beforeEvent = 0;
            for (size_t j = i + 1; j < measure.voices.size(); ++j) {
                MusicXmlVoice &candidate = measure.voices.at(j);
                if (candidate.staff != voice.staff) break;
                const std::vector<MusicXmlEvent>::const_iterator event
                    = std::lower_bound(candidate.events.cbegin(), candidate.events.cend(), voice.durationTicks,
                        [](const MusicXmlEvent &entry, int tick) { return entry.startTick < tick; });
                const bool last = j + 1 == measure.voices.size() || measure.voices.at(j + 1).staff != voice.staff;
                if (event != candidate.events.cend() && event->startTick == voice.durationTicks) {
                    target = &candidate;
                    beforeEvent = static_cast<size_t>(event - candidate.events.cbegin());
                    break;
                }
                if (last && candidate.durationTicks == voice.durationTicks) {
                    target = &candidate;
                    beforeEvent = candidate.events.size();
                }
            }
            if (!target) {
                LogError("MusicXML output cannot preserve the trailing clef '%s' across voices",
                    trailing->clef->GetID().c_str());
                return false;
            }

            const std::vector<MusicXmlClefChange>::iterator insert
                = std::lower_bound(target->clefs.begin(), target->clefs.end(), beforeEvent,
                    [](const MusicXmlClefChange &change, size_t index) { return change.beforeEvent < index; });
            for (std::vector<MusicXmlClefChange>::iterator change = trailing; change != voice.clefs.end(); ++change) {
                change->beforeEvent = beforeEvent;
            }
            target->clefs.insert(insert, trailing, voice.clefs.end());
            voice.clefs.erase(trailing, voice.clefs.end());
        }
        return true;
    }

    bool PlanMusicXmlClefChanges(MusicXmlPart &part)
    {
        for (size_t i = 0; i < part.measures.size(); ++i) {
            MusicXmlMeasure &measure = part.measures.at(i);
            std::map<const Clef *, int> sourceVoices;
            for (const MusicXmlVoice &voice : measure.voices) {
                for (const MusicXmlClefChange &change : voice.clefs) {
                    if (!change.clef->HasSameas()) sourceVoices.emplace(change.clef, voice.voice);
                }
            }
            for (MusicXmlVoice &voice : measure.voices) {
                for (MusicXmlClefChange &change : voice.clefs) {
                    const Clef *source = ResolveMusicXmlClef(change.clef);
                    const std::map<const Clef *, int>::const_iterator owner = sourceVoices.find(source);
                    // The importer uses sameas copies to apply a clef to the other voices on its staff.
                    if (owner != sourceVoices.end() && owner->second != voice.voice
                        && source->GetFirstAncestor(STAFF) == change.clef->GetFirstAncestor(STAFF)) {
                        change.clef = NULL;
                    }
                    else if (i == 0 && change.beforeEvent == 0) {
                        // First-measure clefs at score time zero are imported as initial attributes.
                        part.staves.at(voice.staff ? voice.staff - 1 : 0).clef = source;
                        change.clef = NULL;
                    }
                    else {
                        change.clef = source;
                    }
                }
                std::erase_if(voice.clefs, [](const MusicXmlClefChange &change) { return !change.clef; });
            }
            if (!PlaceMusicXmlTrailingClefs(measure)) return false;
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

    bool PlanMusicXmlEventTiming(std::vector<MusicXmlPart> &parts, int divisions)
    {
        // Directions may require finer divisions. Keep event timing in rhythm ticks until writing.
        for (MusicXmlPart &part : parts) {
            for (MusicXmlMeasure &measure : part.measures) {
                for (MusicXmlVoice &voice : measure.voices) {
                    int tick = 0;
                    for (MusicXmlEvent &event : voice.events) {
                        const int scale = divisions / event.duration.GetDenominator();
                        if (event.duration.GetNumerator() > (std::numeric_limits<int>::max() - tick) / scale) {
                            LogError("MusicXML output measure duration overflow");
                            return false;
                        }
                        event.startTick = tick;
                        tick += event.duration.GetNumerator() * scale;
                        event.endTick = tick;
                    }
                    voice.durationTicks = tick;
                    measure.durationTicks = std::max(measure.durationTicks, tick);
                }
            }
        }
        return true;
    }

    void CollectMusicXmlElementPositions(
        const std::vector<MusicXmlPart> &parts, int divisions, MusicXmlElementPositionMap &positions)
    {
        // The order of notes in a MusicXML part can differ from score time when a measure has multiple voices.
        for (size_t part = 0; part < parts.size(); ++part) {
            size_t order = 0;
            const std::vector<MusicXmlMeasure> &measures = parts.at(part).measures;
            for (size_t measure = 0; measure < measures.size(); ++measure) {
                for (const MusicXmlVoice &voice : measures.at(measure).voices) {
                    for (const MusicXmlEvent &event : voice.events) {
                        const Fraction offset(event.startTick, divisions);
                        if (event.element && !event.element->IsAnyOf(std::array{ NOTE, MULTIREST })) {
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
                    }
                }
            }
        }
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

    std::optional<Fraction> GetMusicXmlTimestampOffset(double beat, const MusicXmlMeter &meter, int divisions)
    {
        const double quarters = (beat - 1.0) * 4.0 / meter.unit;
        if (!std::isfinite(quarters) || quarters < 0.0 || quarters > meter.GetDuration().ToDouble()) {
            return std::nullopt;
        }

        // Prefer an exact grid position. MEI reads timestamps through float, so also accept the first
        // position on the grid that rounds to the same float when the stored beat has that precision.
        const bool isFloat = beat == static_cast<float>(beat);
        std::optional<Fraction> roundedOffset;
        for (int resolution = divisions; resolution <= maxMusicXmlDivisions; resolution += divisions) {
            const double ticks = quarters * resolution;
            const double rounded = std::round(ticks);
            if (std::abs(ticks - rounded) < 1e-8) return Fraction(static_cast<int>(rounded), resolution);
            if (isFloat && !roundedOffset) {
                const double gridBeat = 1.0 + (rounded / resolution) * meter.unit / 4.0;
                if (static_cast<float>(gridBeat) == beat) roundedOffset.emplace(static_cast<int>(rounded), resolution);
            }
        }
        return roundedOffset;
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
        const MusicXmlAnchor &anchor, const MusicXmlMeter &meter, int divisions)
    {
        if (const Fraction *offset = std::get_if<Fraction>(&anchor.position)) {
            if (offset->ToDouble() > meter.GetDuration().ToDouble()) return std::nullopt;
            return *offset;
        }
        return GetMusicXmlTimestampOffset(std::get<double>(anchor.position), meter, divisions);
    }

    bool HasMusicXmlMultiRest(const MusicXmlPart &part, size_t first, size_t last)
    {
        return std::any_of(part.measures.begin() + first, part.measures.begin() + last + 1,
            [](const MusicXmlMeasure &measure) { return measure.multiRest != NULL; });
    }

    std::optional<Fraction> GetMusicXmlDirectionEndOffset(const std::optional<MusicXmlAnchor> &end, size_t startMeasure,
        Fraction startOffset, const MusicXmlPart &part, size_t staff, int &divisions)
    {
        if (!end || end->measure < startMeasure) return std::nullopt;
        // The importer counts expanded measures when it calculates @tstamp2.
        if (HasMusicXmlMultiRest(part, startMeasure, end->measure)) return std::nullopt;
        const std::optional<Fraction> offset
            = GetMusicXmlDirectionOffset(*end, GetMusicXmlMeasureMeter(part, end->measure, staff), divisions);
        if (!offset) return std::nullopt;
        const int resolution = std::lcm(divisions, offset->GetDenominator());
        if (end->measure == startMeasure
            && GetMusicXmlTicks(*offset, resolution) <= GetMusicXmlTicks(startOffset, resolution)) {
            return std::nullopt;
        }
        divisions = resolution;
        return offset;
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
        MusicXmlEventMatch match, int voiceNumber = 0, bool notesOnly = false)
    {
        const LayerElement *target = NULL;
        int latest = -1;
        for (const MusicXmlVoice &voice : voices) {
            if (voice.staff != staff || (voiceNumber && voice.voice != voiceNumber)) continue;
            for (const MusicXmlEvent &event : voice.events) {
                if (event.grace != GRACE_NONE || event.startTick <= latest || !IsMusicXmlPauseAnchor(event.element)
                    || (notesOnly && !event.element->IsAnyOf(std::array{ NOTE, CHORD }))) {
                    continue;
                }
                const double delta = tick - (match == MusicXmlEventMatch::End ? event.endTick : event.startTick);
                if ((std::abs(delta) < 1e-8 && match != MusicXmlEventMatch::Before)
                    || (delta >= 1e-8
                        && (match == MusicXmlEventMatch::Before || match == MusicXmlEventMatch::OnsetOrBefore))) {
                    target = event.element;
                    latest = event.startTick;
                }
            }
        }
        return target;
    }

    double GetMusicXmlAnchorOffset(const MusicXmlAnchor &anchor, const MusicXmlMeter &meter)
    {
        if (const double *beat = std::get_if<double>(&anchor.position)) return (*beat - 1.0) * 4.0 / meter.unit;
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
                    const MusicXmlMeasure &measure = part.measures.at(anchor->measure);

                    const LayerElement *target = start;
                    if (!start || startStaff != staff) {
                        const double *beat = std::get_if<double>(&anchor->position);
                        if (fermata && beat && *beat == 0.0) {
                            if (!measure.multiRest || position->second.first == 0) {
                                collected |= AddMusicXmlBarlineFermata(part, anchor->measure, true, fermata);
                            }
                            continue;
                        }
                        const double offset = GetMusicXmlAnchorOffset(
                            *anchor, GetMusicXmlMeasureMeter(part, anchor->measure, staffIndex));
                        if (!std::isfinite(offset) || offset < 0.0) continue;
                        const double tick = offset * divisions;
                        // A timestamped breath or caesura belongs to the preceding event.
                        const MusicXmlEventMatch match = fermata ? MusicXmlEventMatch::Onset
                            : start                              ? MusicXmlEventMatch::OnsetOrBefore
                                                                 : MusicXmlEventMatch::Before;
                        target = FindMusicXmlEventAtOffset(measure.voices, xmlStaff, tick, match);
                        if (fermata && beat && std::abs(tick - measure.durationTicks) < 1e-8) {
                            // The importer skips the closing measure's rest and has no end cursor for the fermata.
                            if (!measure.multiRest) {
                                collected |= AddMusicXmlBarlineFermata(part, anchor->measure, false, fermata);
                            }
                            continue;
                        }
                        if (tick > measure.durationTicks + 1e-8) continue;
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
        const double offset
            = GetMusicXmlAnchorOffset(anchor, GetMusicXmlMeasureMeter(part, anchor.measure, staffIndex));
        if (!std::isfinite(offset) || offset < 0.0 || anchor.measure < start.measure) return NULL;

        size_t measure = anchor.measure;
        double tick = offset * divisions;
        // MusicXML puts the stop on the last covered note, while MEI uses the position where the line ends.
        if (tick == 0.0) {
            if (measure == start.measure) return NULL;
            const std::vector<MusicXmlVoice> &voices = part.measures.at(--measure).voices;
            const std::vector<MusicXmlVoice>::const_iterator voice = std::find_if(voices.begin(), voices.end(),
                [&start](const MusicXmlVoice &entry) { return entry.voice == start.voice; });
            if (voice == voices.end()) return NULL;
            tick = voice->durationTicks;
        }
        if (HasMusicXmlMultiRest(part, start.measure, measure)) return NULL;
        const LayerElement *end = FindMusicXmlEventAtOffset(
            part.measures.at(measure).voices, start.staff, tick, MusicXmlEventMatch::End, start.voice, true);
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
                        const double offset = GetMusicXmlAnchorOffset(
                            *anchor, GetMusicXmlMeasureMeter(part, anchor->measure, staffIndex));
                        if (!std::isfinite(offset) || offset < 0.0) continue;
                        const MusicXmlEventMatch match = (turn && turn->GetDelayed() == BOOLEAN_true)
                            ? MusicXmlEventMatch::OnsetOrBefore
                            : MusicXmlEventMatch::Onset;
                        const double tick = offset * divisions;
                        const MusicXmlMeasure &measure = part.measures.at(anchor->measure);
                        target = FindMusicXmlEventAtOffset(measure.voices, xmlStaff, tick, match, 0, true);
                        if (tick > measure.durationTicks + 1e-8) continue;
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

    template <typename Collect>
    bool CollectMusicXmlDirectionTargets(const MusicXmlAnchor &anchor, const std::vector<const Staff *> &staves,
        const MusicXmlStaffPositionMap &staffPositions, const std::vector<MusicXmlPart> &parts, int &divisions,
        Collect collect)
    {
        bool collected = false;
        for (const Staff *source : staves) {
            const MusicXmlStaffPositionMap::const_iterator position = staffPositions.find(source->GetN());
            if (position == staffPositions.end()) continue;
            const size_t part = position->second.first;
            const size_t staff = position->second.second;
            // Later parts stop parsing at the multiple-rest marker before reaching directions.
            if (part > 0 && parts.at(part).measures.at(anchor.measure).multiRest) continue;
            const std::optional<Fraction> offset = GetMusicXmlDirectionOffset(
                anchor, GetMusicXmlMeasureMeter(parts.at(part), anchor.measure, staff), divisions);
            if (!offset) continue;

            // A rejected target must not change the resolution available to subsequent targets.
            int resolution = std::lcm(divisions, offset->GetDenominator());
            if (collect(MusicXmlDirectionTarget{ part, staff, *offset }, resolution)) {
                divisions = resolution;
                collected = true;
            }
        }
        return collected;
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
            const bool collected
                = CollectMusicXmlDirectionTargets(*start, dynam->GetTstampStaves(measures.at(start->measure), dynam),
                    staffPositions, parts, divisions, [&](const MusicXmlDirectionTarget &target, int &) {
                        AddMusicXmlDirection(parts.at(target.part), start->measure,
                            { text, dynam->GetPlace(), target.GetStaffNumber(), target.offset });
                        return true;
                    });
            if (!collected) {
                LogWarning("MusicXML output could not place dynam '%s' on a staff at a supported offset",
                    dynam->GetID().c_str());
            }
        }
    }

    std::u32string GetMusicXmlText(const ListOfConstObjects &children)
    {
        std::u32string text;
        for (const Object *child : children) {
            if (child->Is(LB)) {
                text += U'\n';
            }
            else {
                text += vrv_cast<const Text *>(child)->GetText();
            }
        }
        // The importer splits words into lines and discards the final line break.
        while (!text.empty() && text.back() == U'\n') text.pop_back();
        return text;
    }

    void AddMusicXmlDirectionSpans(std::vector<MusicXmlDirectionSpan> &spans, std::vector<MusicXmlPart> &parts,
        int plannedDivisions, int &divisions)
    {
        std::stable_sort(spans.begin(), spans.end(),
            [plannedDivisions](const MusicXmlDirectionSpan &left, const MusicXmlDirectionSpan &right) {
                return std::tuple{ left.part, left.startMeasure, GetMusicXmlTicks(left.startOffset, plannedDivisions),
                    left.staff }
                < std::tuple{ right.part, right.startMeasure, GetMusicXmlTicks(right.startOffset, plannedDivisions),
                      right.staff };
            });

        // Each call numbers a single kind of span. A number can be reused as soon as the previous span stops.
        std::array<MusicXmlDirectionTime, 16> availableAt{};
        size_t partIndex = 0;
        for (const MusicXmlDirectionSpan &span : spans) {
            if (partIndex != span.part) {
                partIndex = span.part;
                availableAt.fill({ 0, 0 });
            }
            const MusicXmlDirectionTime startTime{ span.startMeasure,
                GetMusicXmlTicks(span.startOffset, plannedDivisions) };
            const std::array<MusicXmlDirectionTime, 16>::iterator available = std::find_if(availableAt.begin(),
                availableAt.end(), [&startTime](MusicXmlDirectionTime end) { return end <= startTime; });
            const int number
                = available == availableAt.end() ? 0 : static_cast<int>(available - availableAt.begin()) + 1;
            MusicXmlPart &part = parts.at(span.part);
            if (const size_t *startIndex = std::get_if<size_t>(&span.source)) {
                MusicXmlDirection &start = part.directions.at(span.startMeasure).at(*startIndex);
                const Dir *directive = std::get<MusicXmlWords>(start.marking).directive;
                if (!number) {
                    LogWarning("MusicXML output omits the extender of dir '%s' because all 16 numbers are in use",
                        directive->GetID().c_str());
                    continue;
                }
                start.endpoint = MusicXmlDirectionEndpoint::Start;
                start.number = number;
                AddMusicXmlDirection(part, span.endMeasure,
                    { MusicXmlWords{ directive, {} }, start.placement, span.staff, span.endOffset,
                        MusicXmlDirectionEndpoint::Stop, number });
            }
            else {
                const Hairpin *hairpin = std::get<const Hairpin *>(span.source);
                if (!number) {
                    LogWarning("MusicXML output omits hairpin '%s' because all 16 wedge numbers are in use",
                        hairpin->GetID().c_str());
                    continue;
                }
                AddMusicXmlDirection(part, span.startMeasure,
                    { hairpin, hairpin->GetPlace(), span.staff, span.startOffset, MusicXmlDirectionEndpoint::Start,
                        number });
                AddMusicXmlDirection(part, span.endMeasure,
                    { hairpin, hairpin->GetPlace(), span.staff, span.endOffset, MusicXmlDirectionEndpoint::Stop,
                        number });
            }
            *available = { span.endMeasure, GetMusicXmlTicks(span.endOffset, plannedDivisions) };
            divisions = std::lcm(divisions, span.startOffset.GetDenominator());
            divisions = std::lcm(divisions, span.endOffset.GetDenominator());
        }
    }

    void CollectMusicXmlDirectives(const std::vector<std::pair<size_t, const Dir *>> &directives,
        const std::vector<const Measure *> &measures, const MusicXmlElementPositionMap &positions,
        const MusicXmlStaffPositionMap &staffPositions, std::vector<MusicXmlPart> &parts, int &divisions)
    {
        std::vector<MusicXmlDirectionSpan> spans;
        for (const std::pair<size_t, const Dir *> &entry : directives) {
            const Dir *directive = entry.second;
            const std::string text = UTF32to8(GetMusicXmlText(directive->GetList()));
            // The importer treats these words as dynamics even when encoded as a text direction.
            if (directive->IsStageDir() || text.find_first_not_of(" \t\r\n") == std::string::npos
                || text.starts_with("cresc") || text.starts_with("dim") || text.starts_with("decresc")) {
                LogWarning("MusicXML output omits unsupported %s '%s'", directive->GetClassName().c_str(),
                    directive->GetID().c_str());
                continue;
            }

            const std::optional<MusicXmlAnchor> start = GetMusicXmlStartAnchor(*directive, entry.first, positions);
            if (!start) {
                LogWarning("MusicXML output could not locate dir '%s'", directive->GetID().c_str());
                continue;
            }
            const std::optional<MusicXmlAnchor> end = directive->IsExtenderElement()
                ? GetMusicXmlEndAnchor(*directive, entry.first, measures.size(), positions)
                : std::nullopt;
            bool extended = false;
            const bool collected = CollectMusicXmlDirectionTargets(*start,
                directive->GetTstampStaves(measures.at(start->measure), directive), staffPositions, parts, divisions,
                [&](const MusicXmlDirectionTarget &target, int &resolution) {
                    MusicXmlPart &part = parts.at(target.part);
                    AddMusicXmlDirection(part, start->measure,
                        { MusicXmlWords{ directive, text }, directive->GetPlace(), target.GetStaffNumber(),
                            target.offset });
                    const std::optional<Fraction> endOffset = GetMusicXmlDirectionEndOffset(
                        end, start->measure, target.offset, part, target.staff, resolution);
                    if (endOffset) {
                        spans.push_back({ part.directions.at(start->measure).size() - 1, target.part,
                            target.GetStaffNumber(), start->measure, end->measure, target.offset, *endOffset });
                        extended = true;
                    }
                    return true;
                });
            if (!collected) {
                LogWarning("MusicXML output could not place dir '%s' on a staff at a supported offset",
                    directive->GetID().c_str());
            }
            else if (directive->IsExtenderElement() && !extended) {
                LogWarning("MusicXML output could not place the extender of dir '%s'", directive->GetID().c_str());
            }
        }
        AddMusicXmlDirectionSpans(spans, parts, divisions, divisions);
    }

    std::string MusicXmlTempoDecimal(double value)
    {
        if (!std::isfinite(value) || value <= 0.0) return "";
        // MusicXML sound/tempo has an XML decimal value, which cannot use an exponent.
        const int exponent = static_cast<int>(std::floor(std::log10(value)));
        const int places = std::max(0, std::numeric_limits<double>::digits10 - 1 - exponent);
        std::ostringstream decimal;
        decimal.imbue(std::locale::classic());
        decimal << std::fixed << std::setprecision(places) << value;
        std::string text = decimal.str();
        if (places > 0) {
            text.erase(text.find_last_not_of('0') + 1);
            if (text.back() == '.') text.pop_back();
        }
        return text;
    }

    void ExtractMusicXmlMetronomeText(std::u32string &text, MusicXmlTempo &marking, data_DURATION unit, double mm)
    {
        static constexpr char32_t glyphs[] = { SMUFL_ECA0_metNoteDoubleWhole, SMUFL_ECA2_metNoteWhole,
            SMUFL_ECA3_metNoteHalfUp, SMUFL_ECA5_metNoteQuarterUp, SMUFL_ECA7_metNote8thUp, SMUFL_ECA9_metNote16thUp,
            SMUFL_ECAB_metNote32ndUp, SMUFL_ECAD_metNote64thUp, SMUFL_ECAF_metNote128thUp, SMUFL_ECB1_metNote256thUp,
            SMUFL_ECB3_metNote512thUp, SMUFL_ECB5_metNote1024thUp };
        const char32_t glyph = glyphs[unit - DURATION_breve];
        const char32_t noteGlyph = glyph - (SMUFL_ECA0_metNoteDoubleWhole - SMUFL_E1D0_noteDoubleWhole);
        const std::u32string beatGlyphs{ glyph, noteGlyph, unit == DURATION_4 ? U'\u2669' : glyph };
        const size_t beat = text.find_last_of(beatGlyphs);
        if (beat == std::u32string::npos) return;
        const size_t equals = text.find(U'=', beat + 1);
        if (equals == std::u32string::npos
            || text.substr(beat + 1, equals - beat - 1).find_first_not_of(U" \t.\uECB7") != std::u32string::npos) {
            return;
        }
        const size_t first = text.find_first_not_of(U" \t", equals + 1);
        if (first == std::u32string::npos) return;
        const size_t beforeBeat = beat == 0 ? std::u32string::npos : text.find_last_not_of(U" \t", beat - 1);
        const size_t last = text.find_last_not_of(U" \t");
        const bool parentheses = beforeBeat < beat && text[beforeBeat] == U'(' && text[last] == U')';
        const std::string perMinute = UTF32to8(text.substr(first, last + 1 - first - (parentheses ? 1 : 0)));
        const size_t number = perMinute.find_first_of("0123456789");
        if (number == std::string::npos || std::strtod(perMinute.c_str() + number, NULL) != mm) return;

        // The importer stores the metronome glyphs and printed rate in the MEI text as well as the tempo attributes.
        marking.perMinute = perMinute;
        marking.parentheses = parentheses;
        text.resize(parentheses ? beforeBeat : beat);
        while (!text.empty() && (text.back() == U' ' || text.back() == U'\t')) text.pop_back();
    }

    void CollectMusicXmlTempos(const std::vector<std::pair<size_t, const Tempo *>> &tempos,
        const std::vector<const Measure *> &measures, const MusicXmlElementPositionMap &positions,
        const MusicXmlStaffPositionMap &staffPositions, std::vector<MusicXmlPart> &parts, int &divisions)
    {
        ClassIdsComparison textTypes({ LB, TEXT });
        for (const std::pair<size_t, const Tempo *> &entry : tempos) {
            const Tempo *tempo = entry.second;
            MusicXmlTempo marking;
            marking.language = tempo->GetLang();
            if (tempo->HasMidiBpm()) marking.bpm = MusicXmlTempoDecimal(tempo->GetMidiBpm());

            ListOfConstObjects textElements;
            tempo->FindAllDescendantsByComparison(&textElements, &textTypes);
            std::u32string text = GetMusicXmlText(textElements);
            const data_DURATION unit = tempo->HasMmUnit() ? tempo->GetMmUnit() : DURATION_4;
            const int dots = tempo->HasMmDots() ? tempo->GetMmDots() : 0;
            if (tempo->HasMm() && unit >= DURATION_breve && unit <= DURATION_1024 && dots >= 0 && dots <= 8) {
                marking.perMinute = MusicXmlTempoDecimal(tempo->GetMm());
                if (!marking.perMinute.empty()) {
                    marking.beatUnit = GetMusicXmlNoteType(unit);
                    marking.dots = dots;
                    ExtractMusicXmlMetronomeText(text, marking, unit, tempo->GetMm());
                }
            }
            marking.text = UTF32to8(text);
            if (marking.text.find_first_not_of(" \t\r\n") == std::string::npos) marking.text.clear();
            if (!marking.beatUnit && marking.bpm.empty()) {
                // The importer recognizes a tempo direction by a metronome or sound tempo, and reads words alone as
                // dir.
                LogWarning("MusicXML output omits tempo '%s' without a supported metronome mark or MIDI BPM",
                    tempo->GetID().c_str());
                continue;
            }
            if (tempo->IsExtenderElement() || tempo->HasEndid() || tempo->HasTstamp2()) {
                LogWarning("MusicXML output omits the unsupported span of tempo '%s'", tempo->GetID().c_str());
            }

            const std::optional<MusicXmlAnchor> start = GetMusicXmlStartAnchor(*tempo, entry.first, positions);
            if (!start) {
                LogWarning("MusicXML output could not locate tempo '%s'", tempo->GetID().c_str());
                continue;
            }
            const Measure *measure = measures.at(start->measure);
            std::vector<const Staff *> staves = tempo->GetTstampStaves(measure, tempo);
            if (staves.empty() && !tempo->HasStaff() && !tempo->HasPart()) {
                if (const Staff *staff = measure->GetFirstStaff()) staves.push_back(staff);
            }
            const bool collected = CollectMusicXmlDirectionTargets(
                *start, staves, staffPositions, parts, divisions, [&](const MusicXmlDirectionTarget &target, int &) {
                    AddMusicXmlDirection(parts.at(target.part), start->measure,
                        { marking, tempo->GetPlace(), target.GetStaffNumber(), target.offset });
                    return true;
                });
            if (!collected) {
                LogWarning("MusicXML output could not place tempo '%s' on a staff at a supported offset",
                    tempo->GetID().c_str());
            }
        }
    }

    void PlanMusicXmlHairpinSpans(const std::vector<std::pair<size_t, const Hairpin *>> &hairpins,
        const std::vector<const Measure *> &measures, const MusicXmlElementPositionMap &positions,
        const MusicXmlStaffPositionMap &staffPositions, const std::vector<MusicXmlPart> &parts,
        std::vector<MusicXmlDirectionSpan> &spans, int &divisions)
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

            const bool collected = CollectMusicXmlDirectionTargets(*start,
                hairpin->GetTstampStaves(measures.at(start->measure), hairpin), staffPositions, parts, divisions,
                [&](const MusicXmlDirectionTarget &target, int &resolution) {
                    const std::optional<Fraction> endOffset = GetMusicXmlDirectionEndOffset(
                        end, start->measure, target.offset, parts.at(target.part), target.staff, resolution);
                    if (!endOffset) return false;
                    spans.push_back({ hairpin, target.part, target.GetStaffNumber(), start->measure, end->measure,
                        target.offset, *endOffset });
                    return true;
                });
            if (!collected) {
                LogWarning(
                    "MusicXML output could not place hairpin '%s' at supported offsets", hairpin->GetID().c_str());
            }
        }
    }

    void SortMusicXmlDirections(MusicXmlPart &part, int divisions)
    {
        for (std::vector<MusicXmlDirection> &directions : part.directions) {
            // Keep dynamics in their existing order and sort each kind of extender chronologically.
            std::stable_sort(directions.begin(), directions.end(),
                [divisions](const MusicXmlDirection &left, const MusicXmlDirection &right) {
                    const MusicXmlDirectionKind kind = left.GetKind();
                    if (kind != right.GetKind()) return kind < right.GetKind();
                    if (kind == MusicXmlDirectionKind::Dynamic) return false;
                    const int leftTick = GetMusicXmlTicks(left.offset, divisions);
                    const int rightTick = GetMusicXmlTicks(right.offset, divisions);
                    if (leftTick != rightTick) return leftTick < rightTick;
                    // Reused numbers must be closed before they are opened again at the same offset.
                    return left.endpoint == MusicXmlDirectionEndpoint::Stop
                        && right.endpoint != MusicXmlDirectionEndpoint::Stop;
                });
        }
    }

    void AddMusicXmlLyric(const LayerElement *anchor, const Verse *verse, std::vector<const Syl *> syllables,
        const MusicXmlElementPositionMap &positions, MusicXmlNoteControlMap &controls)
    {
        if (syllables.empty()) return;
        const int number = verse ? std::max(1, verse->GetN()) : 1;
        const Syl *last = syllables.back();
        if (number > std::numeric_limits<short>::max()
            || !controls[anchor].lyrics.try_emplace(number, MusicXmlLyric{ verse, std::move(syllables) }).second) {
            LogWarning("MusicXML output omits unsupported lyric '%s' for verse %d", last->GetID().c_str(), number);
            return;
        }
        if (last->GetCon() != sylLog_CON_u || last->GetWordpos() == sylLog_WORDPOS_i
            || last->GetWordpos() == sylLog_WORDPOS_m) {
            return;
        }

        const MusicXmlElementPosition &start = positions.at(anchor);
        const MusicXmlElementPositionMap::const_iterator end = positions.find(last->GetEnd());
        if (end == positions.end() || start.part != end->second.part || start.staff != end->second.staff
            || start.voice != end->second.voice || start.order >= end->second.order) {
            LogWarning("MusicXML output could not place the extender of syllable '%s'", last->GetID().c_str());
            return;
        }
        // MEI ends a melisma at the note before the next syllable, including an empty syllable.
        controls[anchor].lyrics.at(number).extend = true;
        controls[end->first].lyricStops.insert(number);
    }

    void CollectMusicXmlElementLyrics(const LayerElement *owner, const LayerElement *anchor,
        const MusicXmlElementPositionMap &positions, MusicXmlNoteControlMap &controls)
    {
        std::vector<const Syl *> directSyllables;
        for (const Object *child : owner->GetChildren()) {
            if (const Verse *verse = dynamic_cast<const Verse *>(child)) {
                std::vector<const Syl *> syllables;
                for (const Object *member : verse->GetChildren()) {
                    if (const Syl *syl = dynamic_cast<const Syl *>(member)) {
                        if (!syl->IsEmpty()) syllables.push_back(syl);
                    }
                    else if (!member->Is(LABEL)) {
                        LogWarning("MusicXML output omits '%s' in verse '%s'", member->GetClassName().c_str(),
                            verse->GetID().c_str());
                    }
                }
                AddMusicXmlLyric(anchor, verse, std::move(syllables), positions, controls);
            }
            else if (const Syl *syl = dynamic_cast<const Syl *>(child)) {
                if (!syl->IsEmpty()) directSyllables.push_back(syl);
            }
            else if (child->Is(REFRAIN)) {
                LogWarning("MusicXML output omits refrain '%s'", child->GetID().c_str());
            }
        }
        AddMusicXmlLyric(anchor, NULL, std::move(directSyllables), positions, controls);
    }

    void CollectMusicXmlLyrics(const MusicXmlElementPositionMap &positions, MusicXmlNoteControlMap &controls)
    {
        for (const std::pair<const LayerElement *const, MusicXmlElementPosition> &entry : positions) {
            const LayerElement *element = entry.first;
            if (element->Is(CHORD)) {
                CollectMusicXmlElementLyrics(element, element, positions, controls);
                // The importer puts the first chord tone's lyrics on the chord itself.
                CollectMusicXmlElementLyrics(GetMusicXmlNoteAnchor(element), element, positions, controls);
            }
            else if (const Note *note = dynamic_cast<const Note *>(element)) {
                const Chord *chord = note->IsChordTone();
                if (!chord || GetMusicXmlNoteAnchor(chord) != note) {
                    CollectMusicXmlElementLyrics(note, note, positions, controls);
                }
            }
        }
    }

    class CollectMusicXmlMeasureControlsFunctor : public ConstFunctor {
    public:
        bool ImplementsEndInterface() const override { return false; }

        FunctorCode VisitControlElement(const ControlElement *control) override
        {
            m_controls.push_back(control);
            return FUNCTOR_CONTINUE;
        }

        FunctorCode VisitSyl(const Syl *) override
        {
            m_hasLyrics = true;
            return FUNCTOR_CONTINUE;
        }

    public:
        std::vector<const ControlElement *> m_controls;
        bool m_hasLyrics = false;
    };

    void CollectMusicXmlControls(const std::vector<const Measure *> &measures, std::vector<MusicXmlPart> &parts,
        int &divisions, MusicXmlNoteControlMap &controls)
    {
        std::vector<const Slur *> slurs;
        std::vector<std::pair<size_t, const Dir *>> directives;
        std::vector<std::pair<size_t, const Dynam *>> dynamics;
        std::vector<std::pair<size_t, const Hairpin *>> hairpins;
        std::vector<std::pair<size_t, const Tempo *>> tempos;
        std::vector<std::pair<size_t, const ControlElement *>> pauses;
        std::vector<std::pair<size_t, const ControlElement *>> ornaments;
        CollectMusicXmlMeasureControlsFunctor collect;
        for (size_t i = 0; i < measures.size(); ++i) {
            collect.m_controls.clear();
            measures.at(i)->Process(collect);
            for (const ControlElement *control : collect.m_controls) {
                switch (control->GetClassId()) {
                    case TIE: {
                        // PrepareData resolves the ID references to the exact notes, including tones within chords.
                        const TimeSpanningInterface *span = control->GetTimeSpanningInterface();
                        if (!span || !AddMusicXmlTie(*span, controls)) {
                            LogWarning("MusicXML output omits unsupported tie '%s'", control->GetID().c_str());
                        }
                        break;
                    }
                    case SLUR: slurs.push_back(vrv_cast<const Slur *>(control)); break;
                    case DIR: directives.emplace_back(i, vrv_cast<const Dir *>(control)); break;
                    case DYNAM: dynamics.emplace_back(i, vrv_cast<const Dynam *>(control)); break;
                    case HAIRPIN: hairpins.emplace_back(i, vrv_cast<const Hairpin *>(control)); break;
                    case TEMPO: tempos.emplace_back(i, vrv_cast<const Tempo *>(control)); break;
                    case BREATH:
                    case CAESURA:
                    case FERMATA: pauses.emplace_back(i, control); break;
                    case MORDENT:
                    case TRILL:
                    case TURN: ornaments.emplace_back(i, control); break;
                    default: break;
                }
            }
        }
        const bool hasStaffControls = !directives.empty() || !dynamics.empty() || !hairpins.empty() || !tempos.empty()
            || !pauses.empty() || !ornaments.empty();
        if (slurs.empty() && !hasStaffControls && !collect.m_hasLyrics) return;
        MusicXmlElementPositionMap positions;
        CollectMusicXmlElementPositions(parts, divisions, positions);
        if (collect.m_hasLyrics) CollectMusicXmlLyrics(positions, controls);
        CollectMusicXmlSlurs(slurs, positions, controls);
        if (!hasStaffControls) return;
        const MusicXmlStaffPositionMap staffPositions = GetMusicXmlStaffPositions(parts);
        CollectMusicXmlPauses(pauses, measures, positions, staffPositions, parts, divisions, controls);
        CollectMusicXmlOrnaments(ornaments, measures, positions, staffPositions, parts, divisions, controls);
        CollectMusicXmlDynamics(dynamics, measures, positions, staffPositions, parts, divisions);
        if (!hairpins.empty()) {
            std::vector<MusicXmlDirectionSpan> spans;
            int plannedDivisions = divisions;
            PlanMusicXmlHairpinSpans(hairpins, measures, positions, staffPositions, parts, spans, plannedDivisions);
            AddMusicXmlDirectionSpans(spans, parts, plannedDivisions, divisions);
        }
        CollectMusicXmlDirectives(directives, measures, positions, staffPositions, parts, divisions);
        CollectMusicXmlTempos(tempos, measures, positions, staffPositions, parts, divisions);
        for (MusicXmlPart &part : parts) SortMusicXmlDirections(part, divisions);
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

    void WriteMusicXmlWedge(pugi::xml_node type, const Hairpin *hairpin, int number, bool start)
    {
        const bool crescendo = hairpin->GetForm() == hairpinLog_FORM_cres;
        pugi::xml_node wedge = type.append_child("wedge");
        wedge.append_attribute("type") = start ? (crescendo ? "crescendo" : "diminuendo") : "stop";
        wedge.append_attribute("number") = number;
        if (start) {
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
        if (start == crescendo) {
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

    void WriteMusicXmlWords(pugi::xml_node direction, const MusicXmlWords &marking, int number, bool stop)
    {
        if (!stop) {
            pugi::xml_node words = direction.append_child("direction-type").append_child("words");
            words.text().set(marking.text.c_str());
            if (marking.directive->HasLang()) words.append_attribute("xml:lang") = marking.directive->GetLang().c_str();
        }
        if (!number) return;

        // The importer attaches a line to the words when it follows them in its own direction-type.
        const data_LINEFORM form = marking.directive->GetLform();
        const bool dashed = form == LINEFORM_NONE || form == LINEFORM_dashed;
        pugi::xml_node line = direction.append_child("direction-type").append_child(dashed ? "dashes" : "bracket");
        line.append_attribute("type") = stop ? "stop" : "start";
        line.append_attribute("number") = number;
        if (!dashed) {
            line.append_attribute("line-end") = "none";
            line.append_attribute("line-type") = marking.directive->AttLineRendBase::LineformToStr(form).c_str();
        }
    }

    void WriteMusicXmlTempo(pugi::xml_node direction, const MusicXmlTempo &marking)
    {
        if (!marking.text.empty()) {
            pugi::xml_node words = direction.append_child("direction-type").append_child("words");
            words.text().set(marking.text.c_str());
            // The importer supplies Italian as the default language for tempo words.
            if (!marking.language.empty() && marking.language != "it") {
                words.append_attribute("xml:lang") = marking.language.c_str();
            }
        }
        if (!marking.beatUnit && !marking.text.empty()) return;
        pugi::xml_node metronome = direction.append_child("direction-type").append_child("metronome");
        if (!marking.beatUnit) metronome.append_attribute("print-object") = "no";
        if (marking.parentheses) metronome.append_attribute("parentheses") = "yes";
        metronome.append_child("beat-unit").text().set(marking.beatUnit ? marking.beatUnit : "quarter");
        for (int i = 0; i < marking.dots; ++i) metronome.append_child("beat-unit-dot");
        metronome.append_child("per-minute")
            .text()
            .set(marking.beatUnit ? marking.perMinute.c_str() : marking.bpm.c_str());
    }

    void WriteMusicXmlDirection(pugi::xml_node measure, const MusicXmlDirection &direction, int divisions)
    {
        pugi::xml_node node = measure.append_child("direction");
        if (direction.placement == STAFFREL_above) node.append_attribute("placement") = "above";
        if (direction.placement == STAFFREL_below) node.append_attribute("placement") = "below";
        const MusicXmlTempo *tempo = std::get_if<MusicXmlTempo>(&direction.marking);
        if (const std::string *text = std::get_if<std::string>(&direction.marking)) {
            WriteMusicXmlDynamic(node.append_child("direction-type"), *text);
        }
        else if (const MusicXmlWords *words = std::get_if<MusicXmlWords>(&direction.marking)) {
            WriteMusicXmlWords(node, *words, direction.number, direction.endpoint == MusicXmlDirectionEndpoint::Stop);
        }
        else if (tempo) {
            WriteMusicXmlTempo(node, *tempo);
        }
        else {
            WriteMusicXmlWedge(node.append_child("direction-type"), std::get<const Hairpin *>(direction.marking),
                direction.number, direction.endpoint == MusicXmlDirectionEndpoint::Start);
        }
        const int offset = GetMusicXmlTicks(direction.offset, divisions);
        if (offset) {
            pugi::xml_node offsetNode = node.append_child("offset");
            offsetNode.text().set(offset);
            if (tempo && !tempo->bpm.empty()) offsetNode.append_attribute("sound") = "yes";
        }
        node.append_child("staff").text().set(direction.staff);
        if (tempo && !tempo->bpm.empty()) node.append_child("sound").append_attribute("tempo") = tempo->bpm.c_str();
    }

    void WriteMusicXmlKey(pugi::xml_node attributes, const KeySig &key, int staff = 0)
    {
        pugi::xml_node node = attributes.append_child("key");
        if (staff) node.append_attribute("number") = staff;
        if (key.GetVisible() == BOOLEAN_false) node.append_attribute("print-object") = "no";
        node.append_child("fifths").text().set(key.GetFifthsInt());
        if (key.HasMode()) node.append_child("mode").text().set(key.AttKeySigAnl::ModeToStr(key.GetMode()).c_str());
    }

    void WriteMusicXmlTime(pugi::xml_node attributes, const MeterSig &meter, const MusicXmlMeter &values, int staff = 0)
    {
        pugi::xml_node time = attributes.append_child("time");
        if (staff) time.append_attribute("number") = staff;
        if (meter.GetSym() == METERSIGN_common) {
            time.append_attribute("symbol") = "common";
        }
        else if (meter.GetSym() == METERSIGN_cut) {
            time.append_attribute("symbol") = "cut";
        }
        else if (meter.GetForm() == METERFORM_num) {
            time.append_attribute("symbol") = "single-number";
        }
        if (meter.GetVisible() == BOOLEAN_false) time.append_attribute("print-object") = "no";
        time.append_child("beats").text().set(values.count);
        time.append_child("beat-type").text().set(values.unit);
    }

    void WriteMusicXmlClef(pugi::xml_node attributes, const Clef &clef, int staff, bool afterBarline = false)
    {
        pugi::xml_node node = attributes.append_child("clef");
        if (staff) node.append_attribute("number") = staff;
        if (afterBarline) node.append_attribute("after-barline") = "yes";
        if (clef.GetVisible() == BOOLEAN_false) node.append_attribute("print-object") = "no";
        node.append_child("sign").text().set(clef.AttClefShape::ClefshapeToStr(clef.GetShape()).c_str());
        const int defaultLine = clef.GetShape() == CLEFSHAPE_G ? 2 : (clef.GetShape() == CLEFSHAPE_F ? 4 : 3);
        node.append_child("line").text().set(clef.HasLine() ? clef.GetLine() : defaultLine);

        int octaves;
        switch (clef.GetDis()) {
            case OCTAVE_DIS_8: octaves = 1; break;
            case OCTAVE_DIS_15: octaves = 2; break;
            case OCTAVE_DIS_22: octaves = 3; break;
            default: return;
        }
        if (clef.GetDisPlace() == STAFFREL_basic_below) octaves = -octaves;
        node.append_child("clef-octave-change").text().set(octaves);
    }

    pugi::xml_node WriteMusicXmlAttributes(pugi::xml_node measure, const ScoreDef *scoreDef, const MusicXmlPart &part,
        const MusicXmlMeasureSignatures &change, size_t index, int divisions, bool separateInitialClefs)
    {
        if (index) {
            if (!change.key && !change.time) return {};
            pugi::xml_node attributes = measure.append_child("attributes");
            if (change.key) WriteMusicXmlKey(attributes, *change.key);
            if (change.time) {
                WriteMusicXmlTime(attributes, *change.time, GetMusicXmlMeasureMeter(part, index, 0));
            }
            return attributes;
        }

        pugi::xml_node attributes = measure.append_child("attributes");
        attributes.append_child("divisions").text().set(divisions);
        const bool multipleStaves = part.staves.size() > 1;
        for (size_t i = 0; i < part.staves.size(); ++i) {
            const KeySig *key
                = change.key ? change.key : GetInitialAttribute<KeySig>(scoreDef, part.staves.at(i).definition, KEYSIG);
            if (key) {
                WriteMusicXmlKey(attributes, *key, multipleStaves ? static_cast<int>(i) + 1 : 0);
            }
        }
        for (size_t i = 0; i < part.staves.size(); ++i) {
            const MeterSig *meter = change.time
                ? change.time
                : GetInitialAttribute<MeterSig>(scoreDef, part.staves.at(i).definition, METERSIG);
            if (meter) {
                WriteMusicXmlTime(attributes, *meter, GetMusicXmlMeasureMeter(part, index, i),
                    multipleStaves ? static_cast<int>(i) + 1 : 0);
            }
        }
        if (multipleStaves) attributes.append_child("staves").text().set(static_cast<int>(part.staves.size()));
        // A later part's initial clefs are read as changes if they share the multiple-rest marker's attributes.
        pugi::xml_node clefs = separateInitialClefs ? pugi::xml_node() : attributes;
        int staffNumber = 0;
        for (const MusicXmlStaff &staff : part.staves) {
            ++staffNumber;
            if (staff.clef && staff.clef->HasShape()) {
                if (!clefs) clefs = measure.append_child("attributes");
                WriteMusicXmlClef(clefs, *staff.clef, multipleStaves ? staffNumber : 0);
            }
        }
        return attributes;
    }

    void WriteMusicXmlMultiRest(pugi::xml_node attributes, const MultiRest &multiRest)
    {
        pugi::xml_node rest = attributes.append_child("measure-style").append_child("multiple-rest");
        rest.text().set(multiRest.GetNum());
        if (multiRest.HasBlock())
            rest.append_attribute("use-symbols") = multiRest.GetBlock() == BOOLEAN_true ? "no" : "yes";
    }

    std::string GetMusicXmlMeasureNumber(const Measure &measure, size_t logicalNumber, int offset)
    {
        if (!measure.HasN()) return std::to_string(logicalNumber + offset);
        const std::string number = measure.GetN();
        if (!offset) return number;
        int base = 0;
        const char *end = number.data() + number.size();
        const std::from_chars_result parsed = std::from_chars(number.data(), end, base);
        if (parsed.ec == std::errc() && parsed.ptr == end && base <= std::numeric_limits<int>::max() - offset) {
            return std::to_string(base + offset);
        }
        return number + "." + std::to_string(offset);
    }

    void WriteMusicXmlTieEndpoints(pugi::xml_node parent, const char *tag, const MusicXmlNoteControls &controls)
    {
        // At the middle of a chain, close the current tie before opening the next.
        if (controls.tieStop) parent.append_child(tag).append_attribute("type") = "stop";
        if (controls.tieStart) parent.append_child(tag).append_attribute("type") = "start";
    }

    void WriteMusicXmlColor(pugi::xml_node node, const AttColor &element)
    {
        std::string color = element.GetColor();
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

    MusicXmlBarline GetMusicXmlBarline(const Measure &measure, data_BARRENDITION rendition, bool left)
    {
        // The importer represents these two right-barline styles as single barlines with fixed dimensions.
        if (!left && (rendition == BARRENDITION_NONE || rendition == BARRENDITION_single)
            && measure.GetBarLen() == 4.0) {
            if (measure.GetBarPlace() == 2) return { "short" };
            if (measure.GetBarPlace() == 6) return { "tick" };
        }
        switch (rendition) {
            case BARRENDITION_NONE: return {};
            case BARRENDITION_dashed: return { "dashed" };
            case BARRENDITION_dotted: return { "dotted" };
            case BARRENDITION_dbl: return { "light-light" };
            case BARRENDITION_dblheavy: return { "heavy-heavy" };
            case BARRENDITION_end: return { "light-heavy" };
            case BARRENDITION_heavy: return { "heavy" };
            case BARRENDITION_invis: return { "none" };
            case BARRENDITION_rptstart: return { "heavy-light", "forward" };
            case BARRENDITION_rptend: return { "light-heavy", "backward" };
            case BARRENDITION_single: return { "regular" };
            default: break;
        }
        LogWarning("MusicXML output omits unsupported %s barline '%s' on measure '%s'", left ? "left" : "right",
            measure.AttMeasureLog::BarrenditionToStr(rendition).c_str(), measure.GetID().c_str());
        return {};
    }

    std::vector<MusicXmlMeasureBarlines> CollectMusicXmlBarlines(const std::vector<const Measure *> &measures)
    {
        std::vector<MusicXmlMeasureBarlines> barlines;
        barlines.reserve(measures.size());
        for (size_t i = 0; i < measures.size(); ++i) {
            const Measure &measure = *measures[i];
            data_BARRENDITION left = measure.GetLeft();
            data_BARRENDITION right = measure.GetRight();
            const data_BARRENDITION previous = i > 0 ? measures[i - 1]->GetRight() : BARRENDITION_NONE;
            const data_BARRENDITION next = i + 1 < measures.size() ? measures[i + 1]->GetLeft() : BARRENDITION_NONE;

            // Place forward repeats at the start of a measure and backward repeats at its end.
            // A combined repeat belongs to both measures at the boundary.
            if (left == BARRENDITION_rptboth || previous == BARRENDITION_rptboth || previous == BARRENDITION_rptstart) {
                left = BARRENDITION_rptstart;
            }
            else if (left == BARRENDITION_rptend) {
                if (i == 0) LogWarning("MusicXML output omits a backward repeat before the first measure");
                left = BARRENDITION_NONE;
            }
            if (right == BARRENDITION_rptboth || next == BARRENDITION_rptboth || next == BARRENDITION_rptend) {
                right = BARRENDITION_rptend;
            }
            else if (right == BARRENDITION_rptstart) {
                if (i + 1 == measures.size())
                    LogWarning("MusicXML output omits a forward repeat after the last measure");
                right = BARRENDITION_NONE;
            }
            barlines.push_back({ GetMusicXmlBarline(measure, left, true), GetMusicXmlBarline(measure, right, false) });
        }
        return barlines;
    }

    void WriteMusicXmlBarline(pugi::xml_node measure, const MusicXmlBarline &barline,
        const std::vector<const Fermata *> &fermatas, const char *location)
    {
        if (!barline.style && fermatas.empty()) return;
        pugi::xml_node node = measure.append_child("barline");
        node.append_attribute("location") = location;
        if (barline.style) node.append_child("bar-style").text().set(barline.style);
        for (const Fermata *fermata : fermatas) {
            WriteMusicXmlFermata(node, *fermata);
        }
        if (barline.repeat) node.append_child("repeat").append_attribute("direction") = barline.repeat;
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

    void WriteMusicXmlTuplet(pugi::xml_node notations, const MusicXmlEvent &event)
    {
        const Tuplet *tuplet = event.tuplet;
        if (event.tupletStart) {
            pugi::xml_node start = notations.append_child("tuplet");
            start.append_attribute("type") = "start";
            if (tuplet->HasBracketVisible()) {
                start.append_attribute("bracket") = tuplet->GetBracketVisible() == BOOLEAN_true ? "yes" : "no";
            }
            if (tuplet->GetNumVisible() == BOOLEAN_false) {
                start.append_attribute("show-number") = "none";
            }
            else if (tuplet->HasNumFormat()) {
                start.append_attribute("show-number")
                    = tuplet->GetNumFormat() == tupletVis_NUMFORMAT_ratio ? "both" : "actual";
            }
            // MusicXML has one placement for both the number and the bracket.
            const data_STAFFREL_basic place = (tuplet->HasNumPlace() && tuplet->GetNumVisible() != BOOLEAN_false)
                ? tuplet->GetNumPlace()
                : tuplet->GetBracketPlace();
            if (place == STAFFREL_basic_above) start.append_attribute("placement") = "above";
            if (place == STAFFREL_basic_below) start.append_attribute("placement") = "below";
        }
        if (event.tupletStop) notations.append_child("tuplet").append_attribute("type") = "stop";
    }

    const char *GetMusicXmlElision(sylLog_CON connector)
    {
        switch (connector) {
            case sylLog_CON_d: return "-";
            case sylLog_CON_u: return "_";
            case sylLog_CON_t: return "~";
            case sylLog_CON_c: return "^";
            case sylLog_CON_v: return "\u02C7";
            case sylLog_CON_i: return "\u2040";
            case sylLog_CON_b: return "\u203F";
            default: return "\u00A0";
        }
    }

    void WriteMusicXmlSyllable(pugi::xml_node lyric, const Syl &syl, const Verse *verse)
    {
        const char *syllabic = "single";
        switch (syl.GetWordpos()) {
            case sylLog_WORDPOS_i: syllabic = "begin"; break;
            case sylLog_WORDPOS_m: syllabic = "middle"; break;
            case sylLog_WORDPOS_t: syllabic = "end"; break;
            default: break;
        }
        lyric.append_child("syllabic").text().set(syllabic);
        pugi::xml_node text = lyric.append_child("text");
        text.text().set(UTF32to8(syl.GetText()).c_str());

        std::string lang = verse ? verse->GetLang() : "";
        data_FONTSTYLE style = verse ? verse->GetFontstyle() : FONTSTYLE_NONE;
        data_FONTWEIGHT weight = verse ? verse->GetFontweight() : FONTWEIGHT_NONE;
        if (syl.HasLang()) lang = syl.GetLang();
        if (syl.HasFontstyle()) style = syl.GetFontstyle();
        if (syl.HasFontweight()) weight = syl.GetFontweight();
        // MusicXML import preserves strike-through on a whole syllable.
        if (syl.GetChildCount() == 1) {
            if (const Rend *rend = dynamic_cast<const Rend *>(syl.GetChild(0))) {
                if (rend->HasLang()) lang = rend->GetLang();
                if (rend->HasFontstyle()) style = rend->GetFontstyle();
                if (rend->HasFontweight()) weight = rend->GetFontweight();
                if (rend->GetRend() == TEXTRENDITION_line_through) text.append_attribute("line-through") = 1;
            }
        }
        if (!lang.empty()) text.append_attribute("xml:lang") = lang.c_str();
        if (style == FONTSTYLE_italic) text.append_attribute("font-style") = "italic";
        if (style == FONTSTYLE_normal) text.append_attribute("font-style") = "normal";
        if (weight == FONTWEIGHT_bold) text.append_attribute("font-weight") = "bold";
        if (weight == FONTWEIGHT_normal) text.append_attribute("font-weight") = "normal";
    }

    void WriteMusicXmlLyrics(pugi::xml_node note, const MusicXmlNoteControls &controls)
    {
        for (const std::pair<const int, MusicXmlLyric> &entry : controls.lyrics) {
            const MusicXmlLyric &plan = entry.second;
            pugi::xml_node lyric = note.append_child("lyric");
            lyric.append_attribute("number") = entry.first;
            const Verse *verse = plan.verse;
            if (verse) {
                if (verse->HasLabel()) lyric.append_attribute("name") = verse->GetLabel().c_str();
                if (verse->GetPlace() == STAFFREL_above) lyric.append_attribute("placement") = "above";
                if (verse->GetPlace() == STAFFREL_below) lyric.append_attribute("placement") = "below";
                WriteMusicXmlColor(lyric, *verse);
                if (const Label *label = vrv_cast<const Label *>(verse->GetFirst(LABEL))) {
                    const std::string text = UTF32to8(label->GetText());
                    // The importer recognizes a numeric lyric label before an elision.
                    static const std::regex numericLabel("[^[:alpha:]]*\\d[^[:alpha:]]*");
                    if (std::regex_match(text, numericLabel)) {
                        lyric.append_child("text").text().set(text.c_str());
                        lyric.append_child("elision").text().set("\u00A0");
                    }
                    else {
                        LogWarning("MusicXML output omits non-numeric label on verse '%s'", verse->GetID().c_str());
                    }
                }
            }
            const Syl *previous = NULL;
            for (const Syl *syl : plan.syllables) {
                if (previous) lyric.append_child("elision").text().set(GetMusicXmlElision(previous->GetCon()));
                WriteMusicXmlSyllable(lyric, *syl, verse);
                previous = syl;
            }
            if (plan.extend) lyric.append_child("extend").append_attribute("type") = "start";
        }
        for (int number : controls.lyricStops) {
            pugi::xml_node lyric = note.append_child("lyric");
            lyric.append_attribute("number") = number;
            lyric.append_child("extend").append_attribute("type") = "stop";
        }
    }

    void WriteMusicXmlNote(pugi::xml_node measure, const MusicXmlEvent &event, const Note *note, bool chordTone,
        int duration, const MusicXmlVoice &voice, const MusicXmlNoteControlMap &noteControls)
    {
        pugi::xml_node node = measure.append_child("note");
        const MusicXmlNoteControlMap::const_iterator control = noteControls.find(note ? note : event.element);
        const Accid *accid = NULL;
        if (event.grace != GRACE_NONE) {
            pugi::xml_node grace = node.append_child("grace");
            if (event.grace == GRACE_unacc) grace.append_attribute("slash") = "yes";
        }
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
        if (event.grace == GRACE_NONE) node.append_child("duration").text().set(duration);
        if (control != noteControls.end()) WriteMusicXmlTieEndpoints(node, "tie", control->second);
        if (voice.instrument)
            node.append_child("instrument").append_attribute("id") = voice.instrument->GetID().c_str();
        node.append_child("voice").text().set(voice.voice);
        if (event.type) node.append_child("type").text().set(event.type);
        for (int i = 0; i < event.dots; ++i) node.append_child("dot");
        if (accid && accid->HasAccid()) {
            node.append_child("accidental").text().set(GetMusicXmlAccidental(accid->GetAccid()));
        }
        if (event.tuplet) {
            pugi::xml_node modification = node.append_child("time-modification");
            modification.append_child("actual-notes").text().set(event.tuplet->GetNum());
            modification.append_child("normal-notes").text().set(event.tuplet->GetNumbase());
        }
        if (event.grace != GRACE_NONE && !chordTone) {
            if (const AttStems *stems = dynamic_cast<const AttStems *>(event.element)) {
                if (stems->GetStemVisible() == BOOLEAN_false) {
                    node.append_child("stem").text().set("none");
                }
                else if (stems->GetStemDir() == STEMDIRECTION_up) {
                    node.append_child("stem").text().set("up");
                }
                else if (stems->GetStemDir() == STEMDIRECTION_down) {
                    node.append_child("stem").text().set("down");
                }
            }
        }
        if (voice.staff > 0) node.append_child("staff").text().set(voice.staff);
        // A chord is a single beam position. Write it on the chord's first MusicXML note.
        if (!chordTone) {
            static const char *const values[] = { "", "begin", "continue", "end", "forward hook", "backward hook" };
            int number = 0;
            for (const MusicXmlBeamValue value : event.beams) {
                if (value == MusicXmlBeamValue::None) break;
                pugi::xml_node beam = node.append_child("beam");
                beam.append_attribute("number") = ++number;
                beam.text().set(values[static_cast<int>(value)]);
            }
        }
        if (control != noteControls.end()) {
            WriteMusicXmlNotations(node, control->second);
        }
        if (!chordTone && (event.tupletStart || event.tupletStop)) {
            pugi::xml_node notations = node.child("notations");
            if (!notations) notations = node.append_child("notations");
            WriteMusicXmlTuplet(notations, event);
        }
        if (note) {
            const MusicXmlNoteControlMap::const_iterator lyrics = noteControls.find(chordTone ? note : event.element);
            if (lyrics != noteControls.end()) WriteMusicXmlLyrics(node, lyrics->second);
        }
    }

    void WriteMusicXmlVoice(
        pugi::xml_node measure, const MusicXmlVoice &voice, int tickScale, const MusicXmlNoteControlMap &noteControls)
    {
        size_t clefIndex = 0;
        for (size_t i = 0;; ++i) {
            while (clefIndex < voice.clefs.size() && voice.clefs.at(clefIndex).beforeEvent == i) {
                const int tick = i < voice.events.size() ? voice.events.at(i).startTick : voice.durationTicks;
                WriteMusicXmlClef(
                    measure.append_child("attributes"), *voice.clefs.at(clefIndex).clef, voice.staff, tick == 0);
                ++clefIndex;
            }
            if (i == voice.events.size()) break;
            const MusicXmlEvent &event = voice.events.at(i);
            const int duration = (event.endTick - event.startTick) * tickScale;
            ForEachMusicXmlNote(event, [&](const Note *note, bool chordTone) {
                WriteMusicXmlNote(measure, event, note, chordTone, duration, voice, noteControls);
            });
        }
    }

    void WriteMusicXmlMeasureVoices(pugi::xml_node measure, const MusicXmlMeasure &planned, int tickScale,
        const MusicXmlNoteControlMap &noteControls)
    {
        int cursor = 0;
        bool firstVoice = true;
        for (const MusicXmlVoice &voice : planned.voices) {
            if (!firstVoice && !cursor) {
                // A grace-only voice leaves the cursor at zero. Use a positive backup to switch voices.
                measure.append_child("forward").append_child("duration").text().set(1);
                cursor = 1;
            }
            if (cursor) measure.append_child("backup").append_child("duration").text().set(cursor);
            WriteMusicXmlVoice(measure, voice, tickScale, noteControls);
            cursor = voice.durationTicks * tickScale;
            firstVoice = false;
        }
        // End on the last occupied beat even when the final voice is shorter.
        const int furthest = planned.durationTicks * tickScale;
        if (cursor < furthest) {
            measure.append_child("forward").append_child("duration").text().set(furthest - cursor);
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
    musicxmloutput::CollectMusicXmlMeasuresFunctor collectMeasures;
    doc->Process(collectMeasures);
    const std::vector<const Measure *> &measures = collectMeasures.m_measures;
    const ListOfConstObjects staffDefs = scoreDef->FindAllDescendantsByType(STAFFDEF);
    if (measures.empty() || staffDefs.empty()) {
        LogError("MusicXML output requires at least one measure and staff definition");
        return "";
    }
    std::vector<musicxmloutput::MusicXmlMeasureSignatures> signatures(measures.size());
    if (!musicxmloutput::CollectMusicXmlMeasureSignatures(collectMeasures.m_scoreDefs, signatures)) return "";
    std::vector<const MultiRest *> multiRests(measures.size(), NULL);
    if (!musicxmloutput::CollectMusicXmlMultiRests(measures, multiRests)) return "";

    std::map<int, std::set<int>> layersByStaff;
    for (size_t i = 0; i < measures.size(); ++i) {
        const Measure *measure = measures[i];
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
            if (!multiRests[i]) layersByStaff[staff->GetN()].insert(layer->GetN());
        }
    }

    std::vector<musicxmloutput::MusicXmlPart> parts;
    std::map<const Object *, size_t> partByDefinition;
    Fraction initialMeasureDuration;
    for (const Object *object : staffDefs) {
        const StaffDef *staffDef = vrv_cast<const StaffDef *>(object);
        const data_NOTATIONTYPE notation = staffDef->GetNotationtype();
        if (notation != NOTATIONTYPE_NONE && notation != NOTATIONTYPE_cmn) {
            LogError("MusicXML output requires common notation");
            return "";
        }
        const MeterSig *meter = musicxmloutput::GetInitialAttribute<MeterSig>(scoreDef, staffDef, METERSIG);
        const std::optional<musicxmloutput::MusicXmlMeter> meterValues = musicxmloutput::GetMusicXmlMeter(meter);
        const KeySig *key = musicxmloutput::GetInitialAttribute<KeySig>(scoreDef, staffDef, KEYSIG);
        const Clef *clef = musicxmloutput::GetInitialAttribute<Clef>(scoreDef, staffDef, CLEF);
        if (!meterValues || musicxmloutput::GetInitialAttribute<MeterSigGrp>(scoreDef, staffDef, METERSIGGRP)
            || (key && key->HasNonAttribKeyAccidChildren()) || staffDef->HasTransDiat() || staffDef->HasTransSemi()
            || (clef && clef->HasShape() && !musicxmloutput::IsSupportedMusicXmlClef(*clef))) {
            LogError("MusicXML output does not support the initial attributes for staff %d yet", staffDef->GetN());
            return "";
        }
        const Fraction measureDuration = meterValues->GetDuration();
        if (!initialMeasureDuration.GetNumerator()) initialMeasureDuration = measureDuration;
        if (measureDuration != initialMeasureDuration) {
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
        part.staves.push_back({ staffDef, *meterValues, clef, {} });
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

    int divisions = 1;
    std::set<std::string> ids;
    for (musicxmloutput::MusicXmlPart &part : parts) {
        musicxmloutput::AssignMusicXmlVoices(part);
        std::optional<musicxmloutput::MusicXmlMeter> meterOverride;
        for (size_t i = 0; i < measures.size(); ++i) {
            if (signatures[i].time) meterOverride = musicxmloutput::GetMusicXmlMeter(signatures[i].time);
            if (!musicxmloutput::CollectMusicXmlPartMeasure(
                    part, measures[i], multiRests[i], meterOverride, divisions)) {
                return "";
            }
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

    const int rhythmDivisions = divisions;
    if (!musicxmloutput::PlanMusicXmlEventTiming(parts, rhythmDivisions)) return "";
    for (musicxmloutput::MusicXmlPart &part : parts) {
        if (!musicxmloutput::PlanMusicXmlClefChanges(part)) return "";
    }
    musicxmloutput::MusicXmlNoteControlMap noteControls;
    musicxmloutput::CollectMusicXmlControls(measures, parts, divisions, noteControls);
    const int tickScale = divisions / rhythmDivisions;

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
    const std::vector<musicxmloutput::MusicXmlMeasureBarlines> measureBarlines
        = musicxmloutput::CollectMusicXmlBarlines(measures);
    const musicxmloutput::MusicXmlBarlineFermatas noFermatas;
    for (const musicxmloutput::MusicXmlPart &part : parts) {
        pugi::xml_node partNode = root.append_child("part");
        partNode.append_attribute("id") = part.id.c_str();
        const bool firstPart = &part == &parts.front();
        size_t logicalNumber = 1;
        for (size_t i = 0; i < measures.size(); ++i) {
            const Measure *measure = measures[i];
            const musicxmloutput::MusicXmlMeasure &planned = part.measures[i];
            const MultiRest *multiRest = planned.multiRest;
            if (planned.durationTicks > std::numeric_limits<int>::max() / tickScale) {
                LogError("MusicXML output measure duration overflow");
                return "";
            }
            pugi::xml_node measureNode = partNode.append_child("measure");
            measureNode.append_attribute("number")
                = musicxmloutput::GetMusicXmlMeasureNumber(*measure, logicalNumber, 0).c_str();
            const std::map<size_t, musicxmloutput::MusicXmlBarlineFermatas>::const_iterator partFermatas
                = part.barlineFermatas.find(i);
            const musicxmloutput::MusicXmlBarlineFermatas &fermatas
                = partFermatas != part.barlineFermatas.end() ? partFermatas->second : noFermatas;
            const musicxmloutput::MusicXmlMeasureBarlines &barlines = measureBarlines[i];
            if (!multiRest || firstPart) {
                musicxmloutput::WriteMusicXmlBarline(measureNode, barlines.left, fermatas.left, "left");
            }
            pugi::xml_node attributes = musicxmloutput::WriteMusicXmlAttributes(
                measureNode, scoreDef, part, signatures[i], i, divisions, multiRest && !firstPart);
            if (multiRest && !firstPart) {
                // In later parts the importer already knows the span and expects its marker in the first element.
                if (!attributes) attributes = measureNode.append_child("attributes");
                musicxmloutput::WriteMusicXmlMultiRest(attributes, *multiRest);
                musicxmloutput::WriteMusicXmlBarline(measureNode, barlines.left, fermatas.left, "left");
            }
            // Directions are written at cursor zero, so their offsets are relative to the start of the measure.
            if (!part.directions.empty()) {
                for (const musicxmloutput::MusicXmlDirection &direction : part.directions.at(i)) {
                    musicxmloutput::WriteMusicXmlDirection(measureNode, direction, divisions);
                }
            }
            if (multiRest && firstPart) {
                musicxmloutput::WriteMusicXmlMultiRest(measureNode.append_child("attributes"), *multiRest);
            }
            musicxmloutput::WriteMusicXmlMeasureVoices(measureNode, planned, tickScale, noteControls);
            pugi::xml_node finalMeasure = measureNode;
            const int count = multiRest ? multiRest->GetNum() : 1;
            for (int offset = 1; offset < count; ++offset) {
                finalMeasure = partNode.append_child("measure");
                finalMeasure.append_attribute("number")
                    = musicxmloutput::GetMusicXmlMeasureNumber(*measure, logicalNumber, offset).c_str();
                musicxmloutput::WriteMusicXmlMeasureVoices(finalMeasure, planned, tickScale, noteControls);
            }
            logicalNumber += count;
            musicxmloutput::WriteMusicXmlBarline(finalMeasure, barlines.right, fermatas.right, "right");
            if (firstPart) {
                const MNum *mNum = vrv_cast<const MNum *>(measure->GetFirst(MNUM));
                const Fraction expectedDuration = musicxmloutput::GetMusicXmlMeasureMeter(part, i, 0).GetDuration();
                implicitMeasures[i] = measure->GetMetcon() == BOOLEAN_false
                    || (mNum && !mNum->IsGenerated() && mNum->GetText().empty())
                    || planned.durationTicks * tickScale
                        != musicxmloutput::GetMusicXmlTicks(expectedDuration, divisions);
            }
            if (implicitMeasures[i]) measureNode.append_attribute("implicit") = "yes";
        }
    }
    std::set<ClassId> omitted;
    for (const Measure *measure : measures) {
        for (const Object *child : measure->GetChildren()) {
            if (!child->IsControlElement()
                || child->IsAnyOf(std::array{
                    BREATH, CAESURA, DIR, DYNAM, FERMATA, HAIRPIN, MNUM, MORDENT, SLUR, TEMPO, TIE, TRILL, TURN })
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

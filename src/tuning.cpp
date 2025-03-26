/////////////////////////////////////////////////////////////////////////////
// Name:        tuning.cpp
// Author:      Laurent Pugin
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "tuning.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <string>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "course.h"
#include "editorial.h"
#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// Tuning
//----------------------------------------------------------------------------

static const ClassRegistrar<Tuning> s_factory("tuning", TUNING);

Tuning::Tuning() : Object(TUNING), AttTuningLog()
{
    this->RegisterAttClass(ATT_TUNINGLOG);

    this->Reset();
}

Tuning::~Tuning() {}

void Tuning::Reset()
{
    Object::Reset();
    this->ResetTuningLog();
}

bool Tuning::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ COURSE };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else if (Object::IsEditorialElement(classId)) {
        return true;
    }
    else {
        return false;
    }
}

int Tuning::CalcPitchPos(int course, data_NOTATIONTYPE notationType, int lines, int listSize, int index, int loc,
    int tabLine, int tabAnchorline, bool topAlign) const
{
    switch (notationType) {
        case NOTATIONTYPE_tab_lute_french:
            // all courses >= 7 are positioned above line 0
            return (lines - std::min(course, 7)) * 2 + 1; // above the line
        case NOTATIONTYPE_tab_lute_italian:
            // all courses >= 7 are positioned on line 7
            return (std::min(course, 7) - 1) * 2;
        case NOTATIONTYPE_tab_lute_german:
            if (tabLine != 0) {
                // explicit position, 1st priority
                return (tabLine - 1) * 2;
            }
            else if (loc != MEI_UNSET) {
                // explicit position, 2nd priority
                return loc;
            }
            else if (tabAnchorline != 0) {
                // align bottom note to given anchor line, but don't extend chord above the top line, 3rd priority
                return (std::min(tabAnchorline - 1, lines - listSize) + index) * 2;
            }
            else if (topAlign) {
                // align top note with top line, joint 4th priority (default positioning)
                return (lines - listSize + index) * 2;
            }
            else {
                // align bottom note with bottom line, joint 4th priority
                return index * 2;
            }
        case NOTATIONTYPE_tab_guitar: [[fallthrough]];
        default: return abs(course - lines) * 2;
    }
}

int Tuning::CalcPitchNumber(int course, int fret, data_NOTATIONTYPE notationType) const
{
    // Use <tuning><course>s, if available
    // else use @tuning.standard, if available
    // else use @notationType

    // Do we have the tuning for this course?
    AttNNumberLikeComparison cnc(COURSE, std::to_string(course));
    const Course *courseTuning = vrv_cast<const Course *>(this->FindDescendantByComparison(&cnc));

    if (courseTuning && courseTuning->HasPname() && courseTuning->HasOct()) {

        // Distance in semitones from the octave's starting C to the given note
        int midiBase = 0;
        switch (courseTuning->GetPname()) {
            case PITCHNAME_c: midiBase = 0; break;
            case PITCHNAME_d: midiBase = 2; break;
            case PITCHNAME_e: midiBase = 4; break;
            case PITCHNAME_f: midiBase = 5; break;
            case PITCHNAME_g: midiBase = 7; break;
            case PITCHNAME_a: midiBase = 9; break;
            case PITCHNAME_b: midiBase = 11; break;
            case PITCHNAME_NONE: break;
        }

        const int octave = courseTuning->GetOct();

        // As this does not represent historical notation of any kind the only
        // accidentals we should ever see are "s" and "f"
        int alter = 0;
        if (courseTuning->HasAccid()) {
            if (courseTuning->GetAccid() == ACCIDENTAL_WRITTEN_s)
                alter = 1;
            else if (courseTuning->GetAccid() == ACCIDENTAL_WRITTEN_f)
                alter = -1;
        }

        return (octave + 1) * 12 + midiBase + alter + fret; // MIDI note C4 = 60
    }

    // no <tuning><course> specified, fall back to @tuning.standard

    // modern guitar                           E4  B3  G3  D3  A2  E2
    static const int guitarStandardPitch[] = { 64, 59, 55, 50, 45, 40 };

    // modern guitar drop D                 E4  B3  G3  D3  A2  D2
    static const int guitarDropDPitch[] = { 64, 59, 55, 50, 45, 38 };

    // modern guitar open D                 D4  A3  F#3 D3  A2  D2
    static const int guitarOpenDPitch[] = { 62, 57, 54, 50, 45, 38 };

    // modern guitar open G                 D4  B3  G3  D3  G2  D2
    static const int guitarOpenGPitch[] = { 62, 59, 55, 50, 43, 38 };

    // modern guitar open A                 E4  C#4 A3  E3  A2  E2
    static const int guitarOpenAPitch[] = { 64, 61, 57, 52, 45, 40 };

    // 6 course renaissance lute                 G4  D4  A3  F3  C3  G2
    static const int luteRenaissance6Pitch[] = { 67, 62, 57, 53, 48, 43 };

    // baroque lute D major                  F#4 D4  A3  F#3 D3  A2  G2  F#2 E2  D2  C#2 B1  A1
    static const int luteBaroqueDMajor[] = { 66, 62, 57, 54, 50, 45, 43, 42, 40, 38, 37, 35, 33 };

    // baroque lute D minor                  F4  D4  A3  F3  D3  A2  G2  F2  E2  D2  C2  B1  A1
    static const int luteBaroqueDMinor[] = { 65, 62, 57, 53, 50, 45, 43, 41, 40, 38, 36, 35, 33 };

    const int *pitch = nullptr;
    int pitchSize = 0;

    switch (this->GetTuningStandard()) {
        case COURSETUNING_guitar_standard:
            pitch = guitarStandardPitch;
            pitchSize = sizeof(guitarStandardPitch);
            break;
        case COURSETUNING_guitar_drop_D:
            pitch = guitarDropDPitch;
            pitchSize = sizeof(guitarDropDPitch);
            break;

        case COURSETUNING_guitar_open_D:
            pitch = guitarOpenDPitch;
            pitchSize = sizeof(guitarOpenDPitch);
            break;

        case COURSETUNING_guitar_open_G:
            pitch = guitarOpenGPitch;
            pitchSize = sizeof(guitarOpenGPitch);
            break;

        case COURSETUNING_guitar_open_A:
            pitch = guitarOpenAPitch;
            pitchSize = sizeof(guitarOpenAPitch);
            break;

        case COURSETUNING_lute_renaissance_6:
            pitch = luteRenaissance6Pitch;
            pitchSize = sizeof(luteRenaissance6Pitch);
            break;

        case COURSETUNING_lute_baroque_d_major:
            pitch = luteBaroqueDMajor;
            pitchSize = sizeof(luteBaroqueDMajor);
            break;

        case COURSETUNING_lute_baroque_d_minor:
            pitch = luteBaroqueDMinor;
            pitchSize = sizeof(luteBaroqueDMinor);
            break;

        case COURSETUNING_NONE: [[fallthrough]];

        default:
            // @tuning.standard is not specified, use @notationType

            if (notationType == NOTATIONTYPE_tab_lute_french || notationType == NOTATIONTYPE_tab_lute_italian
                || notationType == NOTATIONTYPE_tab_lute_german) {
                // lute tablature, assume 6 course renaissance lute
                pitch = luteRenaissance6Pitch;
                pitchSize = sizeof(luteRenaissance6Pitch);
            }
            else {
                // assume modern guitar
                pitch = guitarStandardPitch;
                pitchSize = sizeof(guitarStandardPitch);
            }
            break;
    }

    return (course > 0 && course <= pitchSize) ? pitch[course - 1] + fret : 0;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

FunctorCode Tuning::Accept(Functor &functor)
{
    return functor.VisitTuning(this);
}

FunctorCode Tuning::Accept(ConstFunctor &functor) const
{
    return functor.VisitTuning(this);
}

FunctorCode Tuning::AcceptEnd(Functor &functor)
{
    return functor.VisitTuningEnd(this);
}

FunctorCode Tuning::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitTuningEnd(this);
}

} // namespace vrv

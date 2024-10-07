/////////////////////////////////////////////////////////////////////////////
// Name:        calcdotsfunctor.cpp
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "calcdotsfunctor.h"

//----------------------------------------------------------------------------

#include "chord.h"
#include "doc.h"
#include "elementpart.h"
#include "note.h"
#include "rest.h"
#include "smufl.h"
#include "staff.h"

namespace vrv {

//----------------------------------------------------------------------------
// CalcDotsFunctor
//----------------------------------------------------------------------------

CalcDotsFunctor::CalcDotsFunctor(Doc *doc) : DocFunctor(doc)
{
    m_chordDots = NULL;
    m_chordDrawingX = 0;
    m_chordStemDir = STEMDIRECTION_NONE;
}

FunctorCode CalcDotsFunctor::VisitChord(Chord *chord)
{
    // if the chord isn't visible, stop here
    if (!chord->IsVisible()) {
        return FUNCTOR_SIBLINGS;
    }
    // if there aren't dot, stop here but only if no note has a dot
    if (chord->GetDots() < 1) {
        if (!chord->HasNoteWithDots()) {
            return FUNCTOR_SIBLINGS;
        }
        else {
            return FUNCTOR_CONTINUE;
        }
    }

    Dots *dots = vrv_cast<Dots *>(chord->FindDescendantByType(DOTS, 1));
    assert(dots);

    m_chordDots = dots;
    m_chordDrawingX = chord->GetDrawingX();
    m_chordStemDir = chord->GetDrawingStemDir();

    dots->SetMapOfDotLocs(chord->CalcOptimalDotLocations());

    return FUNCTOR_CONTINUE;
}

FunctorCode CalcDotsFunctor::VisitNote(Note *note)
{
    if (!note->IsVisible()) {
        return FUNCTOR_SIBLINGS;
    }

    Staff *staff = note->GetAncestorStaff(RESOLVE_CROSS_STAFF);
    const int staffSize = staff->m_drawingStaffSize;
    const bool drawingCueSize = note->GetDrawingCueSize();

    Dots *dots = NULL;
    Chord *chord = note->IsChordTone();

    // The shift to the left when a stem flag requires it
    int flagShift = 0;
    int radius = note->GetDrawingRadius(m_doc);

    if (chord && (chord->GetDots() > 0)) {
        dots = m_chordDots;
        assert(dots);

        // Stem up, shorter than 4th and not in beam
        if ((note->GetDots() > 0) && (m_chordStemDir == STEMDIRECTION_up) && (note->GetDrawingDur() > DURATION_4)
            && !note->IsInBeam()) {
            // Shift according to the flag width if the top note is not flipped
            if ((note == chord->GetTopNote()) && !note->GetFlippedNotehead()) {
                // HARDCODED
                flagShift += m_doc->GetGlyphWidth(SMUFL_E240_flag8thUp, staffSize, drawingCueSize) * 0.8;
            }
        }

        int xRel = note->GetDrawingX() - m_chordDrawingX + 2 * radius + flagShift;
        dots->SetDrawingXRel(std::max(dots->GetDrawingXRel(), xRel));
    }
    if (note->GetDots() > 0) {
        // For single notes we need here to set the dot loc
        dots = vrv_cast<Dots *>(note->FindDescendantByType(DOTS, 1));
        assert(dots);

        MapOfDotLocs dotLocs = note->CalcOptimalDotLocations();
        dots->SetMapOfDotLocs(dotLocs);

        const int dotLocShift = *(dotLocs.cbegin()->second.rbegin()) - note->GetDrawingLoc();

        // Stem up, shorter than 4th and not in beam
        if (const int shift = dots->GetFlagShift(); shift) {
            flagShift += shift;
        }
        else if ((note->GetDrawingStemDir() == STEMDIRECTION_up) && !note->IsInBeam() && (note->GetDrawingStemLen() < 3)
            && (this->IsDotOverlappingWithFlag(note, staffSize, dotLocShift))) {
            // HARDCODED
            const int shift = m_doc->GetGlyphWidth(SMUFL_E240_flag8thUp, staffSize, drawingCueSize) * 0.8;
            flagShift += shift;
            dots->SetFlagShift(shift);
        }

        int xRel = 2 * radius + flagShift;
        dots->SetDrawingXRel(std::max(dots->GetDrawingXRel(), xRel));
    }

    return FUNCTOR_SIBLINGS;
}

FunctorCode CalcDotsFunctor::VisitRest(Rest *rest)
{
    // We currently have no dots object with mensural rests
    if (rest->IsMensuralDur()) {
        return FUNCTOR_SIBLINGS;
    }

    // Nothing to do
    if ((rest->GetDur() <= DURATION_breve) || (rest->GetDots() < 1)) {
        return FUNCTOR_SIBLINGS;
    }

    Staff *staff = rest->GetAncestorStaff(RESOLVE_CROSS_STAFF);
    const bool drawingCueSize = rest->GetDrawingCueSize();
    const int staffSize = staff->m_drawingStaffSize;

    // For single rests we need here to set the dot loc
    Dots *dots = vrv_cast<Dots *>(rest->FindDescendantByType(DOTS, 1));
    assert(dots);

    std::set<int> &dotLocs = dots->ModifyDotLocsForStaff(staff);
    int loc = rest->GetDrawingLoc();

    // if it's on a staff line to start with, we need to compensate here and add a full unit like DrawDots would
    if ((loc % 2) == 0) {
        loc += 1;
    }

    switch (rest->GetActualDur()) {
        case DURATION_32:
        case DURATION_64: loc += 2; break;
        case DURATION_128:
        case DURATION_256: loc += 4; break;
        case DURATION_512: loc += 6; break;
        case DURATION_1024: loc += 8; break;
        default: break;
    }

    dotLocs.insert(loc);

    // HARDCODED
    int xRel = m_doc->GetDrawingUnit(staffSize) * 2.5;
    if (drawingCueSize) xRel = m_doc->GetCueSize(xRel);
    if (rest->GetDur() > DURATION_2) {
        xRel = m_doc->GetGlyphWidth(rest->GetRestGlyph(), staff->m_drawingStaffSize, drawingCueSize);
    }
    dots->SetDrawingXRel(std::max(dots->GetDrawingXRel(), xRel));

    return FUNCTOR_SIBLINGS;
}

bool CalcDotsFunctor::IsDotOverlappingWithFlag(const Note *note, const int staffSize, int dotLocShift) const
{
    const Object *stem = note->GetFirst(STEM);
    if (!stem) return false;

    const Flag *flag = vrv_cast<const Flag *>(stem->GetFirst(FLAG));
    if (!flag || (flag->m_drawingNbFlags == 0)) return false;

    // for the purposes of vertical spacing we care only up to 16th flags - shorter ones grow upwards
    char32_t flagGlyph = SMUFL_E242_flag16thUp;
    data_DURATION dur = note->GetDur();
    if (dur < DURATION_16) flagGlyph = flag->GetFlagGlyph(note->GetDrawingStemDir());
    const int flagHeight = m_doc->GetGlyphHeight(flagGlyph, staffSize, note->GetDrawingCueSize());

    const int dotMargin = flag->GetDrawingY() - note->GetDrawingY() - flagHeight - note->GetDrawingRadius(m_doc) / 2
        - dotLocShift * m_doc->GetDrawingUnit(staffSize);

    return dotMargin < 0;
}

} // namespace vrv

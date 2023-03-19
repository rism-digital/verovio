/////////////////////////////////////////////////////////////////////////////
// Name:        adjusttupletsxfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjusttupletsxfunctor.h"

//----------------------------------------------------------------------------

#include "elementpart.h"
#include "tuplet.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustTupletsXFunctor
//----------------------------------------------------------------------------

AdjustTupletsXFunctor::AdjustTupletsXFunctor(Doc *doc) : DocFunctor(doc) {}

FunctorCode AdjustTupletsXFunctor::VisitTuplet(Tuplet *tuplet)
{
    // Nothing to do if there is no number
    if (!tuplet->HasNum()) {
        return FUNCTOR_SIBLINGS;
    }

    // Nothing to do if the bracket and the num are not visible
    if ((tuplet->GetBracketVisible() == BOOLEAN_false) && (tuplet->GetNumVisible() == BOOLEAN_false)) {
        return FUNCTOR_SIBLINGS;
    }

    // Nothing we can do if the pointers to the left and right are not set
    if (!tuplet->GetDrawingLeft() || !tuplet->GetDrawingRight()) {
        return FUNCTOR_SIBLINGS;
    }

    assert(tuplet->GetDrawingBracketPos() != STAFFREL_basic_NONE);

    // Careful: this will not work if the tuplet has editorial markup (one child) and then notes + one beam
    Beam *beamParent = vrv_cast<Beam *>(tuplet->GetFirstAncestor(BEAM, MAX_BEAM_DEPTH));
    // Is the tuplet contained in a beam?
    if (beamParent) {
        tuplet->SetBracketAlignedBeam(beamParent);
    }
    Beam *beamChild = vrv_cast<Beam *>(tuplet->FindDescendantByType(BEAM));
    // Do we contain a beam?
    if (beamChild) {
        if ((tuplet->GetChildCount(NOTE) == 0) && (tuplet->GetChildCount(CHORD) == 0)
            && (tuplet->GetChildCount(BEAM) == 1)) {
            tuplet->SetBracketAlignedBeam(beamChild);
        }
    }

    tuplet->SetNumAlignedBeam(tuplet->GetBracketAlignedBeam());

    // Cancel alignment of the bracket with the beam if position and stem direction are not concordant
    if (tuplet->GetBracketAlignedBeam() && (tuplet->GetBracketAlignedBeam()->m_drawingPlace == BEAMPLACE_above)
        && (tuplet->GetDrawingBracketPos() == STAFFREL_basic_below)) {
        tuplet->SetBracketAlignedBeam(NULL);
    }
    else if (tuplet->GetBracketAlignedBeam()
        && ((tuplet->GetBracketAlignedBeam()->m_drawingPlace == BEAMPLACE_below)
            && (tuplet->GetDrawingBracketPos() == STAFFREL_basic_above))) {
        tuplet->SetBracketAlignedBeam(NULL);
    }

    // Cancel alignment of the num with the beam if position and stem direction are not concordant
    if (tuplet->GetNumAlignedBeam() && (tuplet->GetNumAlignedBeam()->m_drawingPlace == BEAMPLACE_above)
        && (tuplet->GetDrawingNumPos() == STAFFREL_basic_below)) {
        tuplet->SetNumAlignedBeam(NULL);
    }
    else if (tuplet->GetNumAlignedBeam()
        && ((tuplet->GetNumAlignedBeam()->m_drawingPlace == BEAMPLACE_below)
            && (tuplet->GetDrawingNumPos() == STAFFREL_basic_above))) {
        tuplet->SetNumAlignedBeam(NULL);
    }

    int xRelLeft;
    int xRelRight;
    tuplet->GetDrawingLeftRightXRel(xRelLeft, xRelRight, m_doc);

    TupletBracket *tupletBracket = vrv_cast<TupletBracket *>(tuplet->GetFirst(TUPLET_BRACKET));
    if (tupletBracket && (tuplet->GetBracketVisible() != BOOLEAN_false)) {
        tupletBracket->SetDrawingXRelLeft(xRelLeft);
        tupletBracket->SetDrawingXRelRight(xRelRight);
    }

    TupletNum *tupletNum = vrv_cast<TupletNum *>(tuplet->GetFirst(TUPLET_NUM));
    if (tupletNum && (tuplet->GetNumVisible() != BOOLEAN_false)) {
        // We have a bracket and the num is not on its opposite side
        if (tupletBracket && (tuplet->GetDrawingNumPos() == tuplet->GetDrawingBracketPos())) {
            tupletNum->SetAlignedBracket(tupletBracket);
        }
        else {
            tupletNum->SetAlignedBracket(NULL);
        }
    }

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv

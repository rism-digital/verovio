/////////////////////////////////////////////////////////////////////////////
// Name:        adjustclefchangesfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustclefchangesfunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "staff.h"
#include "vrv.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustClefChangesFunctor
//----------------------------------------------------------------------------

AdjustClefChangesFunctor::AdjustClefChangesFunctor(Doc *doc) : DocFunctor(doc)
{
    m_aligner = NULL;
}

FunctorCode AdjustClefChangesFunctor::VisitClef(Clef *clef)
{
    if (clef->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    assert(clef->GetAlignment());
    if (clef->GetAlignment()->GetType() != ALIGNMENT_CLEF) return FUNCTOR_CONTINUE;

    if (!clef->HasContentBB()) return FUNCTOR_CONTINUE;

    assert(m_aligner);

    Staff *staff = clef->GetAncestorStaff();

    // Create ad comparison object for each type / @n
    std::vector<int> ns;
    // -1 for barline attributes that need to be taken into account each time
    ns.push_back(BARLINE_REFERENCES);
    ns.push_back(clef->m_crossStaff ? clef->m_crossStaff->GetN() : staff->GetN());
    AttNIntegerAnyComparison matchStaff(ALIGNMENT_REFERENCE, ns);

    // Look if we have a grace aligner just after the clef.
    // Limitation: clef changes are always aligned before grace notes, even if appearing after in the encoding.
    // To overcome this limitation we will need to rethink alignment, or (better) use <graceGrp> and have the
    // <clef> within it at the right place. Then the Clef should use the grace aligner and not the measure aligner.
    GraceAligner *graceAligner = NULL;
    Alignment *nextAlignment = vrv_cast<Alignment *>(m_aligner->GetNext(clef->GetAlignment()));
    if (nextAlignment && nextAlignment->GetType() == ALIGNMENT_GRACENOTE) {
        // If we have one, then check if we have one for our staff (or all staves with --grace-rhythm-align)
        int graceAlignerId = m_doc->GetOptions()->m_graceRhythmAlign.GetValue() ? 0 : staff->GetN();
        if (nextAlignment->HasGraceAligner(graceAlignerId)) {
            graceAligner = nextAlignment->GetGraceAligner(graceAlignerId);
        }
    }

    // No grace aligner, look for the next alignment with something on that staff
    if (!graceAligner) {
        nextAlignment = NULL;
        // Look for the next reference - here we start with the next alignment, because otherwise it will find the
        // reference to the Clef in its own children
        Object *next = m_aligner->FindNextChild(&matchStaff, m_aligner->GetNext(clef->GetAlignment()));
        if (next) {
            nextAlignment = vrv_cast<Alignment *>(next->GetParent());
            assert(nextAlignment);
        }
    }

    Alignment *previousAlignment = NULL;
    // Look for the previous reference on this staff (or a barline)
    Object *previous = m_aligner->FindPreviousChild(&matchStaff, clef->GetAlignment());
    if (previous) {
        // We looked for the first alignment reference - we actually need the parent alignment
        previousAlignment = vrv_cast<Alignment *>(previous->GetParent());
        assert(previousAlignment);
    }

    // This should never happen because we always have at least barline alignments - even empty
    if (!previousAlignment || !nextAlignment) {
        LogDebug("No alignment found before and after the clef change");
        return FUNCTOR_CONTINUE;
    }

    // AdjustXPosFunctor can have spread the alignment apart.
    // We want them to point to the same position. Otherwise, adjusting proportionally
    // (below) will yield displacements.
    clef->GetAlignment()->SetXRel(nextAlignment->GetXRel());

    int previousLeft, previousRight;
    previousAlignment->GetLeftRight(ns, previousLeft, previousRight);
    // This typically happens with invisible barlines. Just take the position of the alignment
    if (previousRight == VRV_UNSET) previousRight = previousAlignment->GetXRel();

    // Get the right position of the grace group or of the next element
    int nextLeft, nextRight;
    if (graceAligner) {
        nextLeft = graceAligner->GetGraceGroupLeft(staff->GetN());
    }
    else {
        nextAlignment->GetLeftRight(ns, nextLeft, nextRight);
    }
    // This typically happens with invisible barlines or with --grace-rhythm-align but no grace on that staff
    if (nextLeft == -VRV_UNSET) nextLeft = nextAlignment->GetXRel();

    const int unit = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    const int selfRight = clef->GetContentRight() + m_doc->GetRightMargin(clef) * unit;
    // First move it to the left if necessary
    if (selfRight > nextLeft) {
        clef->SetDrawingXRel(clef->GetDrawingXRel() - selfRight + nextLeft);
    }
    // Then look if it overlaps on the right and make room if necessary
    const int selfLeft = clef->GetContentLeft() - m_doc->GetLeftMargin(clef) * unit;
    if (selfLeft < previousRight) {
        ArrayOfAdjustmentTuples boundaries{ std::make_tuple(
            previousAlignment, clef->GetAlignment(), previousRight - selfLeft) };
        m_aligner->AdjustProportionally(boundaries);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustClefChangesFunctor::VisitMeasure(Measure *measure)
{
    m_aligner = &measure->m_measureAligner;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

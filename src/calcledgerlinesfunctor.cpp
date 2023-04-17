/////////////////////////////////////////////////////////////////////////////
// Name:        calcledgerlinesfunctor.cpp
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "calcledgerlinesfunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "note.h"
#include "staff.h"

namespace vrv {

//----------------------------------------------------------------------------
// CalcLedgerLinesFunctor
//----------------------------------------------------------------------------

CalcLedgerLinesFunctor::CalcLedgerLinesFunctor(Doc *doc) : DocFunctor(doc) {}

FunctorCode CalcLedgerLinesFunctor::VisitNote(Note *note)
{
    if (note->GetVisible() == BOOLEAN_false) {
        return FUNCTOR_SIBLINGS;
    }

    if (!note->IsVisible()) {
        return FUNCTOR_SIBLINGS;
    }

    Staff *staff = note->GetAncestorStaff(RESOLVE_CROSS_STAFF);
    const int staffSize = staff->m_drawingStaffSize;
    const int staffX = staff->GetDrawingX();
    const bool drawingCueSize = note->GetDrawingCueSize();
    const int radius = note->GetDrawingRadius(m_doc);

    /************** Ledger lines: **************/

    int linesAbove = 0;
    int linesBelow = 0;

    if (!note->HasLedgerLines(linesAbove, linesBelow, staff)) return FUNCTOR_SIBLINGS;

    const int extension = m_doc->GetDrawingLedgerLineExtension(staffSize, drawingCueSize);
    const int left = note->GetDrawingX() - extension - staffX;
    int right = note->GetDrawingX() + 2 * radius + extension - staffX;

    if (linesAbove > 0) {
        staff->AddLedgerLineAbove(linesAbove, left, right, extension, drawingCueSize);
    }
    else {
        staff->AddLedgerLineBelow(linesBelow, left, right, extension, drawingCueSize);
    }

    return FUNCTOR_SIBLINGS;
}

FunctorCode CalcLedgerLinesFunctor::VisitStaffEnd(Staff *staff)
{
    const int extension = m_doc->GetDrawingLedgerLineExtension(staff->m_drawingStaffSize, false);
    const int minExtension = m_doc->GetDrawingMinimalLedgerLineExtension(staff->m_drawingStaffSize, false);
    const double cueScaling = m_doc->GetCueScaling();
    this->AdjustLedgerLines(
        staff->GetLedgerLinesAbove(), staff->GetLedgerLinesAboveCue(), cueScaling, extension, minExtension);
    this->AdjustLedgerLines(
        staff->GetLedgerLinesBelow(), staff->GetLedgerLinesBelowCue(), cueScaling, extension, minExtension);

    return FUNCTOR_CONTINUE;
}

void CalcLedgerLinesFunctor::AdjustLedgerLines(
    ArrayOfLedgerLines &lines, ArrayOfLedgerLines &cueLines, double cueScaling, int extension, int minExtension)
{
    assert(minExtension <= extension);

    // By construction, any overlaps or small gaps in outer dash lines must also occur in the most inner dash line.
    // Thus it suffices to resolve any problems in the inner dash line and apply the adjustments to corresponding
    // dashes further away from the staff.

    struct Adjustment {
        int left; // the left boundary of the dash
        int right; // the right boundary of the dash
        bool isCue; // whether the dash is cue
        int delta; // the shortening which will be applied on both sides
    };

    // For each dash on the inner line (both cue and normal) we construct an adjustment with zero delta
    // and sort them
    std::vector<Adjustment> adjustments;
    using DashType = std::pair<int, int>;
    if (!lines.empty()) {
        for (const DashType &dash : lines.at(0).m_dashes) {
            adjustments.push_back({ dash.first, dash.second, false, 0 });
        }
    }
    if (!cueLines.empty()) {
        for (const DashType &dash : cueLines.at(0).m_dashes) {
            adjustments.push_back({ dash.first, dash.second, true, 0 });
        }
    }

    std::sort(adjustments.begin(), adjustments.end(), [](const Adjustment &adj1, const Adjustment &adj2) {
        if (adj1.left < adj2.left) return true;
        return ((adj1.left == adj2.left) && (adj1.right < adj2.right));
    });

    // By comparing successive dashes we compute the necessary adjustment (delta) for each of them
    const int defaultGap = 100 * extension; // A large value which should not trigger any adjustments
    int leftGapProportion = defaultGap; // The proportion of the left gap belonging to the current dash
    int rightGapProportion = defaultGap; // The proportion of the right gap belonging to the current dash

    using IterType = std::vector<Adjustment>::iterator;
    for (IterType iterAdj = adjustments.begin(); iterAdj != adjustments.end(); ++iterAdj) {
        // Calculate the right gap and whether the next dash is cue
        IterType iterNextAdj = std::next(iterAdj);
        const int rightGap = (iterNextAdj != adjustments.end()) ? iterNextAdj->left - iterAdj->right : defaultGap;
        const bool nextIsCue = (iterNextAdj != adjustments.end()) ? iterNextAdj->isCue : false;

        // Calculate the proportion of the right gap belonging to the current dash and the proportion belonging to the
        // next dash
        const double currentCueScale = (iterAdj->isCue) ? cueScaling : 1.0;
        const double nextCueScale = nextIsCue ? cueScaling : 1.0;
        rightGapProportion = currentCueScale / (currentCueScale + nextCueScale) * rightGap;
        const int nextLeftGapProportion = nextCueScale / (currentCueScale + nextCueScale) * rightGap;

        // The gap between successive dashes should be at least one extension
        const int minGapProportion = std::min(leftGapProportion, rightGapProportion);
        if (minGapProportion < currentCueScale * extension / 2.0) {
            const int minTotal = minGapProportion + currentCueScale * extension;
            const int newExtension = std::max<int>(2 * minTotal / 3, currentCueScale * minExtension);
            iterAdj->delta = currentCueScale * extension - newExtension;
            assert(iterAdj->delta >= 0);
        }

        leftGapProportion = nextLeftGapProportion;
    }

    // Finally, we transfer the adjustments to all ledger lines
    // We thus ensure that all dashes on the same note/chord obtain the same ledger line extension
    for (const Adjustment &adjustment : adjustments) {
        if (adjustment.delta > 0) {
            ArrayOfLedgerLines &linesToAdjust = adjustment.isCue ? cueLines : lines;
            for (LedgerLine &line : linesToAdjust) {
                std::list<DashType>::iterator iterDash
                    = std::find_if(line.m_dashes.begin(), line.m_dashes.end(), [&adjustment](const DashType &dash) {
                          return ((dash.first >= adjustment.left) && (dash.second <= adjustment.right));
                      });
                if (iterDash != line.m_dashes.end()) {
                    iterDash->first += adjustment.delta;
                    iterDash->second -= adjustment.delta;
                }
            }
        }
    }
}

} // namespace vrv

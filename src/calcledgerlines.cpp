/////////////////////////////////////////////////////////////////////////////
// Name:        calcledgerlines.cpp
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "calcledgerlines.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "note.h"
#include "staff.h"

namespace vrv {

//----------------------------------------------------------------------------
// CalcLedgerLines
//----------------------------------------------------------------------------

CalcLedgerLines::CalcLedgerLines(Doc *doc) : DocFunctor(doc) {}

FunctorCode CalcLedgerLines::VisitNote(Note *note)
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

FunctorCode CalcLedgerLines::VisitStaffEnd(Staff *staff)
{
    const int extension = m_doc->GetDrawingLedgerLineExtension(staff->m_drawingStaffSize, false);
    const int minExtension = m_doc->GetDrawingMinimalLedgerLineExtension(staff->m_drawingStaffSize, false);
    const double cueScaling = m_doc->GetCueScaling();
    staff->AdjustLedgerLines(
        staff->GetLedgerLinesAbove(), staff->GetLedgerLinesAboveCue(), cueScaling, extension, minExtension);
    staff->AdjustLedgerLines(
        staff->GetLedgerLinesBelow(), staff->GetLedgerLinesBelowCue(), cueScaling, extension, minExtension);

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

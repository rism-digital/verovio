/////////////////////////////////////////////////////////////////////////////
// Name:        calcledgerlines.cpp
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "calcledgerlines.h"

//----------------------------------------------------------------------------

#include "note.h"
#include "staff.h"

namespace vrv {

//----------------------------------------------------------------------------
// CalcLedgerLines
//----------------------------------------------------------------------------

CalcLedgerLines::CalcLedgerLines(Doc *doc) : DocFunctor(doc) {}

FunctorCode CalcLedgerLines::VisitNote(Note *note)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CalcLedgerLines::VisitStaffEnd(Staff *staff)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv

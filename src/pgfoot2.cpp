/////////////////////////////////////////////////////////////////////////////
// Name:        pgfoot2.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "pgfoot2.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "doc.h"

namespace vrv {

//----------------------------------------------------------------------------
// PgFoot2
//----------------------------------------------------------------------------

static const ClassRegistrar<PgFoot2> s_factory("pgFoot2", PGFOOT2);

PgFoot2::PgFoot2() : RunningElement(PGFOOT2, "pgfoot2-")
{
    this->Reset();
}

PgFoot2::~PgFoot2() {}

void PgFoot2::Reset()
{
    RunningElement::Reset();
}

int PgFoot2::GetTotalHeight(const Doc *doc) const
{
    assert(doc);

    int height = this->GetContentHeight();
    if (height > 0) {
        const int unit = doc->GetDrawingUnit(100);
        height += doc->GetOptions()->m_topMarginPgFooter.GetValue() * unit;
    }
    return height;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

} // namespace vrv

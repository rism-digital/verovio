/////////////////////////////////////////////////////////////////////////////
// Name:        pghead2.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "pghead2.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "doc.h"
#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// PgHead2
//----------------------------------------------------------------------------

static const ClassRegistrar<PgHead2> s_factory("pgHead2", PGHEAD2);

PgHead2::PgHead2() : RunningElement(PGHEAD2, "pghead2-")
{
    this->Reset();
}

PgHead2::~PgHead2() {}

void PgHead2::Reset()
{
    RunningElement::Reset();
}

int PgHead2::GetTotalHeight(const Doc *doc) const
{
    assert(doc);

    int height = this->GetContentHeight();
    if (height > 0) {
        const int unit = doc->GetDrawingUnit(100);
        height += doc->GetOptions()->m_bottomMarginPgHead.GetValue() * unit;
    }
    return height;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

FunctorCode PgHead2::Accept(MutableFunctor &functor)
{
    return functor.VisitPgHead2(this);
}

FunctorCode PgHead2::Accept(ConstFunctor &functor) const
{
    return functor.VisitPgHead2(this);
}

FunctorCode PgHead2::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitPgHead2End(this);
}

FunctorCode PgHead2::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitPgHead2End(this);
}

} // namespace vrv

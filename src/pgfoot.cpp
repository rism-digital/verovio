/////////////////////////////////////////////////////////////////////////////
// Name:        pgfoot.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "pgfoot.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "doc.h"
#include "functor.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// PgFoot
//----------------------------------------------------------------------------

static const ClassRegistrar<PgFoot> s_factory("pgFoot", PGFOOT);

PgFoot::PgFoot() : RunningElement(PGFOOT)
{
    this->Reset();
}

PgFoot::~PgFoot() {}

void PgFoot::Reset()
{
    RunningElement::Reset();
}

int PgFoot::GetTotalHeight(const Doc *doc) const
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

FunctorCode PgFoot::Accept(Functor &functor)
{
    return functor.VisitPgFoot(this);
}

FunctorCode PgFoot::Accept(ConstFunctor &functor) const
{
    return functor.VisitPgFoot(this);
}

FunctorCode PgFoot::AcceptEnd(Functor &functor)
{
    return functor.VisitPgFootEnd(this);
}

FunctorCode PgFoot::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitPgFootEnd(this);
}

} // namespace vrv

/////////////////////////////////////////////////////////////////////////////
// Name:        pgfoot.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "pgfoot.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// PgFoot
//----------------------------------------------------------------------------

static const ClassRegistrar<PgFoot> s_factory("pgFoot", PGFOOT);

PgFoot::PgFoot() : RunningElement(PGFOOT, "pgfoot-")
{
    Reset();
}

PgFoot::~PgFoot() {}

void PgFoot::Reset()
{
    RunningElement::Reset();
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

} // namespace vrv

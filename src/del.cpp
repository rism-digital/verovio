/////////////////////////////////////////////////////////////////////////////
// Name:        del.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "del.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Del
//----------------------------------------------------------------------------

static const ClassRegistrar<Del> s_factory("del", DEL);

Del::Del() : EditorialElement(DEL, "del-"), AttSource()
{
    RegisterAttClass(ATT_SOURCE);

    Reset();
}

Del::~Del() {}

void Del::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// functor methods
//----------------------------------------------------------------------------

} // namespace vrv

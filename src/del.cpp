/////////////////////////////////////////////////////////////////////////////
// Name:        del.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "del.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Del
//----------------------------------------------------------------------------

static const ClassRegistrar<Del> s_factory("del", DEL);

Del::Del() : EditorialElement(DEL), AttSource()
{
    this->RegisterAttClass(ATT_SOURCE);

    this->Reset();
}

Del::~Del() {}

void Del::Reset()
{
    EditorialElement::Reset();
    this->ResetSource();
}

//----------------------------------------------------------------------------
// functor methods
//----------------------------------------------------------------------------

} // namespace vrv

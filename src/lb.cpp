/////////////////////////////////////////////////////////////////////////////
// Name:        lb.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "lb.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "text.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Lb
//----------------------------------------------------------------------------

// static ClassRegistrar<Lb> s_factory("lb", LB);

Lb::Lb() : TextElement("lb-")
{
    Reset();
}

Lb::~Lb() {}

void Lb::Reset() {}

} // namespace vrv

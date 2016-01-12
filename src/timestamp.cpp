/////////////////////////////////////////////////////////////////////////////
// Name:        timestamp.cpp
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "timestamp.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// TimestampAttr
//----------------------------------------------------------------------------

TimestampAttr::TimestampAttr() : LayerElement("tstp-")
{
    Reset();
}

TimestampAttr::~TimestampAttr()
{
}

void TimestampAttr::Reset()
{
    LayerElement::Reset();
}

} // namespace vrv
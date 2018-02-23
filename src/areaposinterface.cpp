/////////////////////////////////////////////////////////////////////////////
// Name:        areaposinterface.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "areaposinterface.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AreaPosInterface
//----------------------------------------------------------------------------

AreaPosInterface::AreaPosInterface() : Interface(), AttHorizontalAlign(), AttVerticalAlign()
{
    RegisterInterfaceAttClass(ATT_HORIZONTALALIGN);
    RegisterInterfaceAttClass(ATT_VERTICALALIGN);

    Reset();
}

AreaPosInterface::~AreaPosInterface() {}

void AreaPosInterface::Reset()
{
    ResetHorizontalAlign();
    ResetVerticalAlign();
}

} // namespace vrv

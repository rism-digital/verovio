/////////////////////////////////////////////////////////////////////////////
// Name:        areaposinterface.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "areaposinterface.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AreaPosInterface
//----------------------------------------------------------------------------

AreaPosInterface::AreaPosInterface() : Interface(), AttHorizontalAlign(), AttVerticalAlign()
{
    this->RegisterInterfaceAttClass(ATT_HORIZONTALALIGN);
    this->RegisterInterfaceAttClass(ATT_VERTICALALIGN);

    this->Reset();
}

AreaPosInterface::~AreaPosInterface() {}

void AreaPosInterface::Reset()
{
    this->ResetHorizontalAlign();
    this->ResetVerticalAlign();
}

} // namespace vrv

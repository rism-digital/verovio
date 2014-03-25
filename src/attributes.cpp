/////////////////////////////////////////////////////////////////////////////
// Name:        attributes.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "attributes.h"

//----------------------------------------------------------------------------

#include "vrvdef.h"

namespace vrv {

//----------------------------------------------------------------------------
// Common
//----------------------------------------------------------------------------

Common::Common()
{
    this->ClearAttCommon();
}

Common::~Common()
{
}

void Common::ClearAttCommon()
{
    m_n = VRV_UNSET;   
}

} // namespace vrv
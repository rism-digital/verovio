/////////////////////////////////////////////////////////////////////////////
// Name:        textdirinterface.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "textdirinterface.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// TextDirInterface
//----------------------------------------------------------------------------

TextDirInterface::TextDirInterface() : Interface(), AttCommon(), AttPlacement(), AttStaffident()
{
    RegisterInterfaceAttClass(ATT_COMMON);
    RegisterInterfaceAttClass(ATT_PLACEMENT);
    RegisterInterfaceAttClass(ATT_STAFFIDENT);

    Reset();
}

TextDirInterface::~TextDirInterface()
{
}

void TextDirInterface::Reset()
{
    ResetCommon();
    ResetPlacement();
    ResetStaffident();
}

} // namespace vrv

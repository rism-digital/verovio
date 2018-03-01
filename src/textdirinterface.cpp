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

TextDirInterface::TextDirInterface() : Interface(), AttPlacement()
{
    RegisterInterfaceAttClass(ATT_PLACEMENT);

    Reset();
}

TextDirInterface::~TextDirInterface() {}

void TextDirInterface::Reset()
{
    ResetPlacement();
}

} // namespace vrv

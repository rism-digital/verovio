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

TextDirInterface::TextDirInterface():
    AttCommon(),
    AttPlacement(),
    AttStaffident()
{
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
        
//----------------------------------------------------------------------------
// TempoInterface
//----------------------------------------------------------------------------

TempoInterface::TempoInterface():
    TextDirInterface()
{
    Reset();
}


TempoInterface::~TempoInterface()
{
}

void TempoInterface::Reset()
{
    TextDirInterface::Reset();
}
    
} // namespace vrv

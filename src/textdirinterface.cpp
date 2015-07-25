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

#include "layerelement.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// TextDirInterface
//----------------------------------------------------------------------------

TextDirInterface::TextDirInterface():
    AttCommon()
{
    Reset();
}


TextDirInterface::~TextDirInterface()
{
}    
    
void TextDirInterface::Reset()
{
    AttCommon::Reset();
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
    TempoInterface::Reset();
}
    
} // namespace vrv

/////////////////////////////////////////////////////////////////////////////
// Name:        textdirective.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "textdirective.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "floatingelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Tempo
//----------------------------------------------------------------------------

Tempo::Tempo():
    FloatingElement("tempo-"), TextDirInterface()
{
    RegisterInterface( TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface() );
    
    Reset();
}


Tempo::~Tempo()
{
}    
    
void Tempo::Reset()
{
    FloatingElement::Reset();
    TextDirInterface::Reset();
}
    
} // namespace vrv

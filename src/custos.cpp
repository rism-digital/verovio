/////////////////////////////////////////////////////////////////////////////
// Name:        custos.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "custos.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// Custos
//----------------------------------------------------------------------------

Custos::Custos():
    LayerElement("custos-"), PositionInterface()
{
    RegisterInterface( PositionInterface::GetAttClasses(), PositionInterface::IsInterface() );
    
    Reset();
}

Custos::~Custos()
{
}
    
void Custos::Reset()
{
    LayerElement::Reset();
    PositionInterface::Reset();
}    

} // namespace vrv

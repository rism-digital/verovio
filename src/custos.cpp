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

Custos::Custos() : LayerElement("custos-"), PositionInterface(), AttColor(), AttCommonPart()
{
    RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_COMMONPART);

    Reset();
}

Custos::~Custos()
{
}

void Custos::Reset()
{
    LayerElement::Reset();
    PositionInterface::Reset();
    ResetColor();
    ResetCommonPart();
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Custos::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetDrawing(functorParams);
    PositionInterface::InterfaceResetDrawing(functorParams, this);

    return FUNCTOR_CONTINUE;
};

int Custos::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    LayerElement::ResetHorizontalAlignment(functorParams);
    PositionInterface::InterfaceResetHorizontalAlignment(functorParams, this);

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

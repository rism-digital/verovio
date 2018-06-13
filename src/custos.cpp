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

Custos::Custos() : LayerElement("custos-"), FacsimileInterface(), PitchInterface(), PositionInterface(), AttColor() 
{
    RegisterInterface(FacsimileInterface::GetAttClasses(), FacsimileInterface::IsInterface());
    RegisterInterface(PitchInterface::GetAttClasses(), PitchInterface::IsInterface());
    RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);

    Reset();
}

Custos::~Custos() {}

void Custos::Reset()
{
    LayerElement::Reset();
    FacsimileInterface::Reset();
    PitchInterface::Reset();
    PositionInterface::Reset();
    ResetColor();
}

int Custos::GetDrawingX() const
{
    if (this->HasFacs()) {
        return FacsimileInterface::GetDrawingX();
    }
    else {
        return LayerElement::GetDrawingX();
    }
}

int Custos::GetDrawingY() const
{
    if (this->HasFacs()) {
        return FacsimileInterface::GetDrawingY();
    }
    else {
        return LayerElement::GetDrawingY();
    }
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
}

int Custos::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    LayerElement::ResetHorizontalAlignment(functorParams);
    PositionInterface::InterfaceResetHorizontalAlignment(functorParams, this);

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

/////////////////////////////////////////////////////////////////////////////
// Name:        mrest.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "mrest.h"

namespace vrv {

//----------------------------------------------------------------------------
// MRest
//----------------------------------------------------------------------------

// FIXME MEI 4.0.0
MRest::MRest() : LayerElement("mrest-"), PositionInterface(), AttVisibility(), AttFermataPresent()/*, AttRelativesize()*/
{
    RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    RegisterAttClass(ATT_VISIBILITY);
    RegisterAttClass(ATT_FERMATAPRESENT);
    // FIXME MEI 4.0.0
    //RegisterAttClass(ATT_RELATIVESIZE);

    Reset();
}

MRest::~MRest()
{
}

void MRest::Reset()
{
    LayerElement::Reset();
    PositionInterface::Reset();
    ResetVisibility();
    ResetFermataPresent();
    // FIXME MEI 4.0.0
    //ResetRelativesize();
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int MRest::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetDrawing(functorParams);
    PositionInterface::InterfaceResetDrawing(functorParams, this);

    return FUNCTOR_CONTINUE;
};

int MRest::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    LayerElement::ResetHorizontalAlignment(functorParams);
    PositionInterface::InterfaceResetHorizontalAlignment(functorParams, this);

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

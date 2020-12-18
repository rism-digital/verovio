/////////////////////////////////////////////////////////////////////////////
// Name:        mrest.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "mrest.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "fermata.h"
#include "functorparams.h"

namespace vrv {

//----------------------------------------------------------------------------
// MRest
//----------------------------------------------------------------------------

MRest::MRest() : LayerElement("mrest-"), PositionInterface(), AttColor(), AttCue(), AttFermataPresent(), AttVisibility()
{
    RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_CUE);
    RegisterAttClass(ATT_FERMATAPRESENT);
    RegisterAttClass(ATT_VISIBILITY);

    Reset();
}

MRest::~MRest() {}

void MRest::Reset()
{
    LayerElement::Reset();
    PositionInterface::Reset();
    ResetColor();
    ResetCue();
    ResetFermataPresent();
    ResetVisibility();
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int MRest::ConvertMarkupAnalytical(FunctorParams *functorParams)
{
    ConvertMarkupAnalyticalParams *params = vrv_params_cast<ConvertMarkupAnalyticalParams *>(functorParams);
    assert(params);

    if (this->HasFermata()) {
        Fermata *fermata = new Fermata();
        fermata->ConvertFromAnalyticalMarkup(this, this->GetUuid(), params);
    }

    return FUNCTOR_CONTINUE;
}

int MRest::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetDrawing(functorParams);
    PositionInterface::InterfaceResetDrawing(functorParams, this);

    return FUNCTOR_CONTINUE;
}

int MRest::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    LayerElement::ResetHorizontalAlignment(functorParams);
    PositionInterface::InterfaceResetHorizontalAlignment(functorParams, this);

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

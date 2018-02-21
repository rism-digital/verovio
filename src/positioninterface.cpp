/////////////////////////////////////////////////////////////////////////////
// Name:        positioninterface.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "positioninterface.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "layer.h"
#include "pitchinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// PositionInterface
//----------------------------------------------------------------------------

PositionInterface::PositionInterface() : Interface(), AttStaffLoc(), AttStaffLocPitched()
{
    RegisterInterfaceAttClass(ATT_STAFFLOC);
    RegisterInterfaceAttClass(ATT_STAFFLOCPITCHED);

    Reset();
}

PositionInterface::~PositionInterface() {}

void PositionInterface::Reset()
{
    ResetStaffLoc();
    ResetStaffLocPitched();

    m_drawingLoc = 0;
}

bool PositionInterface::HasIdenticalPositionInterface(PositionInterface *otherPositionInterface)
{
    if (!otherPositionInterface) {
        return false;
    }
    if (this->GetLoc() != otherPositionInterface->GetLoc()) {
        return false;
    }
    if (this->GetOloc() != otherPositionInterface->GetOloc()) {
        return false;
    }
    if (this->GetPloc() != otherPositionInterface->GetPloc()) {
        return false;
    }
    return true;
}

int PositionInterface::CalcDrawingLoc(Layer *layer, LayerElement *element)
{
    assert(layer);

    m_drawingLoc = 0;
    if (this->HasPloc() && this->HasOloc()) {
        m_drawingLoc = PitchInterface::CalcLoc(this->GetPloc(), this->GetOloc(), layer->GetClefLocOffset(element));
    }
    else if (this->HasLoc()) {
        m_drawingLoc = this->GetLoc();
    }
    return m_drawingLoc;
}

//----------------------------------------------------------------------------
// Interface pseudo functor (redirected)
//----------------------------------------------------------------------------

int PositionInterface::InterfaceResetDrawing(FunctorParams *functorParams, Object *object)
{
    m_drawingLoc = 0;

    return FUNCTOR_CONTINUE;
}

int PositionInterface::InterfaceResetHorizontalAlignment(FunctorParams *functorParams, Object *object)
{
    m_drawingLoc = 0;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

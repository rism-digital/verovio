/////////////////////////////////////////////////////////////////////////////
// Name:        positioninterface.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "positioninterface.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "layer.h"
#include "pitchinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// PositionInterface
//----------------------------------------------------------------------------

PositionInterface::PositionInterface() : Interface(), AttStaffLoc(), AttStaffLocPitched()
{
    this->RegisterInterfaceAttClass(ATT_STAFFLOC);
    this->RegisterInterfaceAttClass(ATT_STAFFLOCPITCHED);

    this->Reset();
}

PositionInterface::~PositionInterface() {}

void PositionInterface::Reset()
{
    this->ResetStaffLoc();
    this->ResetStaffLocPitched();

    m_drawingLoc = 0;
}

bool PositionInterface::HasIdenticalPositionInterface(const PositionInterface *otherPositionInterface) const
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

int PositionInterface::CalcDrawingLoc(const Layer *layer, const LayerElement *element)
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

int PositionInterface::InterfaceResetData(FunctorParams *functorParams, Object *object)
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

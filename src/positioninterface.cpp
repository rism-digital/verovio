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
#include "staff.h"

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

bool PositionInterface::HasLedgerLines(int &linesAbove, int &linesBelow, const Staff *staff) const
{
    if (!staff) return false;

    if (staff->IsTabLuteFrench() || staff->IsTabLuteGerman() || staff->IsTabLuteItalian()) {
        // French and German tablature do not use ledger lines.
        // Italian tablature does use a single ledger line for 7th course, and compressed
        // ledger lines for fretted 8th and above, but not for open 8th and above. So
        // rather than use the CMN ledger line handling we draw our own.
        // Guitar tablature has been left as originally implemented.
        linesAbove = 0;
        linesBelow = 0;
        return false;
    }

    linesAbove = (this->GetDrawingLoc() - staff->m_drawingLines * 2 + 2) / 2;
    linesBelow = -(this->GetDrawingLoc()) / 2;

    linesAbove = std::max(linesAbove, 0);
    linesBelow = std::max(linesBelow, 0);

    return ((linesAbove > 0) || (linesBelow > 0));

    return false;
}

//----------------------------------------------------------------------------
// Interface pseudo functor (redirected)
//----------------------------------------------------------------------------

FunctorCode PositionInterface::InterfaceResetData(ResetDataFunctor &functor, Object *object)
{
    m_drawingLoc = 0;

    return FUNCTOR_CONTINUE;
}

FunctorCode PositionInterface::InterfaceResetHorizontalAlignment(
    ResetHorizontalAlignmentFunctor &functor, Object *object)
{
    m_drawingLoc = 0;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

/////////////////////////////////////////////////////////////////////////////
// Name:        zone.cpp
// Author:      Juliette Regimbal
// Created:     05/06/2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "zone.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "object.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Zone
//----------------------------------------------------------------------------
Zone::Zone() : Object("zone-"), AttTyped(), AttCoordinated()
{
    RegisterAttClass(ATT_TYPED);
    RegisterAttClass(ATT_COORDINATED);
    Reset();
}
Zone::~Zone() {}
void Zone::Reset()
{
    ResetTyped();
    ResetCoordinated();
}

void Zone::ShiftByXY(int xDiff, int yDiff)
{
    this->SetUlx(this->GetUlx() + xDiff);
    this->SetLrx(this->GetLrx() + xDiff);
    this->SetUly(this->GetUly() + yDiff);
    this->SetLry(this->GetLry() + yDiff);
}

int Zone::GetLogicalUly()
{
    return (this->GetUly());
}

int Zone::GetLogicalLry()
{
    return (this->GetLry());
}

} // namespace vrv

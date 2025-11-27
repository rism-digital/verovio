/////////////////////////////////////////////////////////////////////////////
// Name:        offsetinterface.cpp
// Author:      Laurent Pugin
// Created:     2025
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "offsetinterface.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// OffsetInterface
//----------------------------------------------------------------------------

OffsetInterface::OffsetInterface() : Interface(), AttVisualOffsetHo(), AttVisualOffsetVo()
{
    this->RegisterInterfaceAttClass(ATT_VISUALOFFSETHO);
    this->RegisterInterfaceAttClass(ATT_VISUALOFFSETVO);

    this->Reset();
}

OffsetInterface::~OffsetInterface() {}

void OffsetInterface::Reset()
{
    this->ResetVisualOffsetHo();
    this->ResetVisualOffsetVo();
}

//----------------------------------------------------------------------------
// OffsetSpanningInterface
//----------------------------------------------------------------------------

OffsetSpanningInterface::OffsetSpanningInterface() : Interface(), AttVisualOffset2Ho(), AttVisualOffset2Vo()
{
    this->RegisterInterfaceAttClass(ATT_VISUALOFFSET2HO);
    this->RegisterInterfaceAttClass(ATT_VISUALOFFSET2VO);

    this->Reset();
}

OffsetSpanningInterface::~OffsetSpanningInterface() {}

void OffsetSpanningInterface::Reset()
{
    this->ResetVisualOffset2Ho();
    this->ResetVisualOffset2Vo();
}

//----------------------------------------------------------------------------
// Interface pseudo functor (redirected)
//----------------------------------------------------------------------------

FunctorCode OffsetInterface::InterfaceResetData(ResetDataFunctor &functor, Object *object)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode OffsetSpanningInterface::InterfaceResetData(ResetDataFunctor &functor, Object *object)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv

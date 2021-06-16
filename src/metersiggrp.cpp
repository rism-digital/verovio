/////////////////////////////////////////////////////////////////////////////
// Name:        metersiggrp.cpp
// Author:      Andriy Makarchuk
// Created:     2021 
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "metersiggrp.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {
//----------------------------------------------------------------------------
// MeterSigGrp
//----------------------------------------------------------------------------

// static ClassRegistrar<MeterSigGrp> s_factory("meterSigGrp", METERSIGGRP);

MeterSigGrp::MeterSigGrp()
    : Object("metersiggrp-")
    , ObjectListInterface()
    , LinkingInterface()
    , AttBasic()
    , AttLabelled()
    , AttTyped()
    , AttMeterSigGrpLog()
{
    RegisterInterface(LinkingInterface::GetAttClasses(), LinkingInterface::IsInterface());
    RegisterAttClass(ATT_BASIC);
    RegisterAttClass(ATT_LABELLED);
    RegisterAttClass(ATT_TYPED);
    RegisterAttClass(ATT_METERSIGGRPLOG);

    Reset();
}

MeterSigGrp::~MeterSigGrp() {}

void MeterSigGrp::Reset()
{
    Object::Reset();
    LinkingInterface::Reset();
    ResetBasic();
    ResetLabelled();
    ResetTyped();
    ResetMeterSigGrpLog();
}

bool MeterSigGrp::IsSupportedChild(Object *child)
{
    if (child->Is(METERSIG)) {
        assert(dynamic_cast<MeterSig *>(child));
    }
    else {
        return false;
    }
    return true;
}
} // namespace vrv
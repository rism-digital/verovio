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
    , AttMeterSigGrpLog()
    , AttTyped()
{
    RegisterInterface(LinkingInterface::GetAttClasses(), LinkingInterface::IsInterface());
    RegisterAttClass(ATT_BASIC);
    RegisterAttClass(ATT_LABELLED);
    RegisterAttClass(ATT_METERSIGGRPLOG);
    RegisterAttClass(ATT_TYPED);

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

void MeterSigGrp::FilterList(ArrayOfObjects *childList)
{
    // We want to keep only MeterSig
    childList->erase(std::remove_if(childList->begin(), childList->end(),
                         [&](const Object *object) -> bool { return !object->Is(METERSIG); }),
        childList->end());
}

} // namespace vrv
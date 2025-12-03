/////////////////////////////////////////////////////////////////////////////
// Name:        ossia.cpp
// Author:      Klaus Rettinghaus
// Created:     2025
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "ossia.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <cassert>
#include <math.h>

//----------------------------------------------------------------------------

#include "staff.h"
#include "vrv.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// Ossia
//----------------------------------------------------------------------------

static const ClassRegistrar<Ossia> s_factory("ossia", OSSIA);

Ossia::Ossia() : Object(OSSIA), AttTyped()
{
    this->RegisterAttClass(ATT_TYPED);

    this->Reset();
}

Ossia::~Ossia()
{
    // We need to delete own objects
    this->Reset();
}

void Ossia::CloneReset()
{
    Object::CloneReset();
}

void Ossia::Reset()
{
    Object::Reset();
    this->ResetTyped();
}

bool Ossia::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ STAFF };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else if (Object::IsEditorialElement(classId)) {
        return true;
    }
    else {
        return false;
    }
}

} // namespace vrv

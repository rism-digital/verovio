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

static const ClassRegistrar<Ossia> s_factory("measure", MEASURE);

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
    else if (Object::IsControlElement(classId)) {
        return true;
    }
    else if (Object::IsEditorialElement(classId)) {
        return true;
    }
    else {
        return false;
    }
}

void Ossia::AddChildBack(Object *child)
{
    if (!this->IsSupportedChild(child->GetClassId()) || !this->AddChildAdditionalCheck(child)) {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        return;
    }

    child->SetParent(this);
    ArrayOfObjects &children = this->GetChildrenForModification();
    if (children.empty()) {
        children.push_back(child);
    }
    else if (children.back()->Is(STAFF)) {
        children.push_back(child);
    }
    else {
        for (auto it = children.begin(); it != children.end(); ++it) {
            if (!(*it)->Is(STAFF)) {
                children.insert(it, child);
                break;
            }
        }
    }
    Modify();
}

bool Ossia::AddChildAdditionalCheck(Object *child)
{
    if (child->Is(STAFF)) {
        Staff *staff = vrv_cast<Staff *>(child);
        assert(staff);
        if (staff && (staff->GetN() < 1)) {
            // This is not 100% safe if we have a <app> and <rdg> with more than
            // one staff as a previous child.
            staff->SetN(this->GetChildCount());
        }
    }
    return (Object::AddChildAdditionalCheck(child));
}

} // namespace vrv

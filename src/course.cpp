/////////////////////////////////////////////////////////////////////////////
// Name:        course.cpp
// Author:      Laurent Pugin
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "course.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// Course
//----------------------------------------------------------------------------

Course::Course() : Object("course-"), AttNNumberLike()
{
    RegisterAttClass(ATT_NNUMBERLIKE);

    Reset();
}

Course::~Course() {}

void Course::Reset()
{
    Object::Reset();
    ResetNNumberLike();
}

void Course::AddChild(Object *child)
{
    // Nothing for now
    assert(false);

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

} // namespace vrv

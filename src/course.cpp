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

static const ClassRegistrar<Course> s_factory("course", COURSE);

Course::Course() : Object(COURSE, "course-"), AttNNumberLike()
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

bool Course::IsSupportedChild(Object *child)
{
    // Nothing for now
    return false;
}

} // namespace vrv

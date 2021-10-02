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

Course::Course() : Object("course-"), AttAccidental(), AttNNumberLike(), AttOctave(), AttPitch()
{
    RegisterAttClass(ATT_ACCIDENTAL);
    RegisterAttClass(ATT_NNUMBERLIKE);
    RegisterAttClass(ATT_OCTAVE);
    RegisterAttClass(ATT_PITCH);

    Reset();
}

Course::~Course() {}

void Course::Reset()
{
    Object::Reset();
    ResetAccidental();
    ResetNNumberLike();
    ResetOctave();
    ResetPitch();
}

bool Course::IsSupportedChild(Object *child)
{
    // Nothing for now
    return false;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

} // namespace vrv

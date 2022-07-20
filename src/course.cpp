/////////////////////////////////////////////////////////////////////////////
// Name:        course.cpp
// Author:      Laurent Pugin
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "course.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// Course
//----------------------------------------------------------------------------

static const ClassRegistrar<Course> s_factory("course", COURSE);

Course::Course() : Object(COURSE, "course-"), AttAccidental(), AttNNumberLike(), AttOctave(), AttPitch()
{
    this->RegisterAttClass(ATT_ACCIDENTAL);
    this->RegisterAttClass(ATT_NNUMBERLIKE);
    this->RegisterAttClass(ATT_OCTAVE);
    this->RegisterAttClass(ATT_PITCH);

    this->Reset();
}

Course::~Course() {}

void Course::Reset()
{
    Object::Reset();
    this->ResetAccidental();
    this->ResetNNumberLike();
    this->ResetOctave();
    this->ResetPitch();
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

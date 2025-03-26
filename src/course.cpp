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

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// Course
//----------------------------------------------------------------------------

static const ClassRegistrar<Course> s_factory("course", COURSE);

Course::Course() : Object(COURSE), AttAccidental(), AttNNumberLike(), AttOctave(), AttPitch()
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

bool Course::IsSupportedChild(ClassId classId)
{
    // Nothing for now
    return false;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

FunctorCode Course::Accept(Functor &functor)
{
    return functor.VisitCourse(this);
}

FunctorCode Course::Accept(ConstFunctor &functor) const
{
    return functor.VisitCourse(this);
}

FunctorCode Course::AcceptEnd(Functor &functor)
{
    return functor.VisitCourseEnd(this);
}

FunctorCode Course::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitCourseEnd(this);
}

} // namespace vrv

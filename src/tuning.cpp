/////////////////////////////////////////////////////////////////////////////
// Name:        tuning.cpp
// Author:      Laurent Pugin
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "tuning.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "course.h"
#include "editorial.h"

namespace vrv {

//----------------------------------------------------------------------------
// Tuning
//----------------------------------------------------------------------------

Tuning::Tuning() : Object("tuning-"), AttCourseLog()
{
    RegisterAttClass(ATT_COURSELOG);

    Reset();
}

Tuning::~Tuning() {}

void Tuning::Reset()
{
    Object::Reset();
    ResetCourseLog();
}

bool Tuning::IsSupportedChild(Object *child)
{
    if (child->Is(COURSE)) {
        assert(dynamic_cast<Course *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

int Tuning::CalcPitchPos(int course, data_NOTATIONTYPE notationType, int lines)
{
    if (notationType == NOTATIONTYPE_tab_guitar) {
        return abs(course - lines) * 2;
    }
    else {
        return (course - 1) * 2;
    }
}

} // namespace vrv

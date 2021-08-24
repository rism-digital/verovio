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

static const ClassRegistrar<Tuning> s_factory("tuning", TUNING);

Tuning::Tuning() : Object(TUNING, "tuning-"), AttCourseLog()
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
    switch (notationType) {
        case NOTATIONTYPE_tab_lute_french:
            // all courses >= 7 are positioned above line 0
            return (lines - std::min(course, 7)) * 2 + 1; // above the line
        case NOTATIONTYPE_tab_lute_italian: return (course - 1) * 2;
        case NOTATIONTYPE_tab_guitar: [[fallthrough]];
        default: return abs(course - lines) * 2;
    }
}

} // namespace vrv

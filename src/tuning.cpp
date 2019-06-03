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

Tuning::~Tuning()
{
}

void Tuning::Reset()
{
    Object::Reset();
    ResetCourseLog();
}

void Tuning::AddChild(Object *child)
{
    if (child->Is(COURSE)) {
        assert(dynamic_cast<Course *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }
    
    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}
    
int Tuning::CalcPitchPos(int course)
{
    return (course - 1) * 2;
}

} // namespace vrv

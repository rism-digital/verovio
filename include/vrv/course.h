/////////////////////////////////////////////////////////////////////////////
// Name:        course.h
// Author:      Laurent Pugin
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_COURSE_H__
#define __VRV_COURSE_H__

#include "atts_shared.h"
#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// Course
//----------------------------------------------------------------------------

/**
 * This class models the MEI <course> element.
 */
class Course : public Object, public AttNNumberLike {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Course();
    virtual ~Course();
    virtual Object *Clone() const { return new Course(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Course"; };
    ///@}

    /**
     * Add an element to a element.
     */
    virtual bool IsSupportedChild(Object *object);

protected:
    //
private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif

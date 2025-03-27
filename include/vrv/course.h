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
class Course : public Object, public AttAccidental, public AttNNumberLike, public AttOctave, public AttPitch {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Course();
    virtual ~Course();
    Object *Clone() const override { return new Course(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "course"; }
    ///@}

    /**
     * Add an element to a element.
     */
    bool IsSupportedChild(ClassId classId) override;

    //----------//
    // Functors //
    //----------//

    /**
     * Interface for class functor visitation
     */
    ///@{
    FunctorCode Accept(Functor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(Functor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
    ///@}

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

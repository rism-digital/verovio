/////////////////////////////////////////////////////////////////////////////
// Name:        lv.h
// Author:      Andriy Makarchuk
// Created:     1/07/2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_LV_H__
#define __VRV_LV_H__

#include "controlelement.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Lv
//----------------------------------------------------------------------------

/**
 * This class models the MEI <lv> element.
 */
class Lv : public ControlElement,
           public TimeSpanningInterface,
           public AttColor,
           public AttCurvature,
           public AttCurveRend {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Lv();
    virtual ~Lv();
    virtual Object *Clone() const { return new Lv(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Lv"; }
    virtual ClassId GetClassId() const { return LV; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual TimePointInterface *GetTimePointInterface() { return this; }
    virtual TimeSpanningInterface *GetTimeSpanningInterface() { return this; }
    ///@}

    //----------//
    // Functors //
    //----------//

private:
    //
public:
    //
private:
}; // lv

} // namespace vrv

#endif // __VRV_LV_H__

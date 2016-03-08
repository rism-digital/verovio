/////////////////////////////////////////////////////////////////////////////
// Name:        slur.h
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SLUR_H__
#define __VRV_SLUR_H__

#include "floatingelement.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Slur
//----------------------------------------------------------------------------

class Slur : public FloatingElement, public TimeSpanningInterface, public AttCurvature {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Slur();
    virtual ~Slur();
    virtual void Reset();
    virtual std::string GetClassName() { return "Slur"; };
    virtual ClassId Is() { return SLUR; };
    ///@}

    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    virtual TimeSpanningInterface *GetTimeSpanningInterface() { return dynamic_cast<TimeSpanningInterface *>(this); }

    //----------//
    // Functors //
    //----------//

    /**
     * Reset the drawing values before calling PrepareDrawing after changes.
     */
    virtual int ResetDrawing(ArrayPtrVoid *params);

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif

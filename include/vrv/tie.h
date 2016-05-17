/////////////////////////////////////////////////////////////////////////////
// Name:        tie.h
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TIE_H__
#define __VRV_TIE_H__

#include "floatingelement.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Tie
//----------------------------------------------------------------------------

class Tie : public FloatingElement, public TimeSpanningInterface, public AttCurvature {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Tie();
    virtual ~Tie();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Tie"; };
    virtual ClassId Is() const { return TIE; };
    ///@}

    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    virtual TimeSpanningInterface *GetTimeSpanningInterface() { return dynamic_cast<TimeSpanningInterface *>(this); }

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        tie.h
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TIE_H__
#define __VRV_TIE_H__

#include "controlelement.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Tie
//----------------------------------------------------------------------------

class Tie : public ControlElement, public TimeSpanningInterface, public AttColor, public AttCurvature {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Tie();
    virtual ~Tie();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Tie"; }
    virtual ClassId GetClassId() const { return TIE; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    virtual TimeSpanningInterface *GetTimeSpanningInterface() { return dynamic_cast<TimeSpanningInterface *>(this); }
    ///@}

    //----------//
    // Functors //
    //----------//

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        pedal.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PEDAL_H__
#define __VRV_PEDAL_H__

#include "atts_cmn.h"
#include "measureelement.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Pedal
//----------------------------------------------------------------------------

class Pedal : public MeasureElement, public TimePointInterface, public AttPedalLog, public AttPlacement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Pedal();
    virtual ~Pedal();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Pedal"; }
    virtual ClassId Is() const { return PEDAL; }
    ///@}

    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }

    //----------//
    // Functors //
    //----------//

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

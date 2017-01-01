/////////////////////////////////////////////////////////////////////////////
// Name:        hairpin.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_HAIRPIN_H__
#define __VRV_HAIRPIN_H__

#include "atts_cmn.h"
#include "controlelement.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Hairpin
//----------------------------------------------------------------------------

class Hairpin : public ControlElement,
                public TimeSpanningInterface,
                public AttColor,
                public AttHairpinLog,
                public AttPlacement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Hairpin();
    virtual ~Hairpin();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Hairpin"; }
    virtual ClassId Is() const { return HAIRPIN; }
    ///@}

    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    virtual TimeSpanningInterface *GetTimeSpanningInterface() { return dynamic_cast<TimeSpanningInterface *>(this); }

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

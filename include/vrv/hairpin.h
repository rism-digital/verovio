/////////////////////////////////////////////////////////////////////////////
// Name:        hairpin.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_HAIRPIN_H__
#define __VRV_HAIRPIN_H__

#include "atts_cmn.h"
#include "floatingelement.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Hairpin
//----------------------------------------------------------------------------

class Hairpin : public FloatingElement, public TimeSpanningInterface, public AttHairpinLog, public AttPlacement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Hairpin();
    virtual ~Hairpin();
    virtual void Reset();
    virtual std::string GetClassName() { return "Hairpin"; };
    virtual ClassId Is() { return HAIRPIN; };
    ///@}

    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    virtual TimeSpanningInterface *GetTimeSpanningInterface() { return dynamic_cast<TimeSpanningInterface *>(this); }

    //----------//
    // Functors //
    //----------//

    /**
     * Align the content of a staff vertically.
     * See Object::AlignVertically
     */
    virtual int AlignVertically(ArrayPtrVoid *params);

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

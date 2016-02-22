/////////////////////////////////////////////////////////////////////////////
// Name:        hairpin.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_HAIRPIN_H__
#define __VRV_HAIRPIN_H__

#include "floatingelement.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Hairpin
//----------------------------------------------------------------------------

class Hairpin : public FloatingElement, public TimeSpanningInterface {
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

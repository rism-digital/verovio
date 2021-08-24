/////////////////////////////////////////////////////////////////////////////
// Name:        caesura.h
// Author:      Klaus Rettinghaus, Thomas Weber
// Created:     2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CAESURA_H__
#define __VRV_CAESURA_H__

#include "atts_cmn.h"
#include "controlelement.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Caesura
//----------------------------------------------------------------------------

/**
 * This class models the MEI <caesura> element.
 */
class Caesura : public ControlElement, public TimePointInterface, public AttColor, public AttPlacementRelStaff {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Caesura();
    virtual ~Caesura();
    virtual Object *Clone() const { return new Caesura(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Caesura"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    ///@}

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

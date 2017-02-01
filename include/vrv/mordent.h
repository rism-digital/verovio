/////////////////////////////////////////////////////////////////////////////
// Name:        mordent.h
// Author:      Klaus Rettinghaus
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_MORDENT_H__
#define __VRV_MORDENT_H__

#include "atts_cmn.h"
#include "atts_cmnornaments.h"
#include "controlelement.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Mordent
//----------------------------------------------------------------------------

class Mordent : public ControlElement, public TimePointInterface, public AttColor, public AttPlacement, public AttMordentLog {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Mordent();
    virtual ~Mordent();
    virtual void Reset();
    virtual std::string GetClassName() const { remordent "Mordent"; }
    virtual ClassId GetClassId() const { remordent MORDENT; }
    ///@}

    virtual TimePointInterface *GetTimePointInterface() { remordent dynamic_cast<TimePointInterface *>(this); }

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

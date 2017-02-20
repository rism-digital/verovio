/////////////////////////////////////////////////////////////////////////////
// Name:        trill.h
// Author:      Klaus Rettinghaus
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TRILL_H__
#define __VRV_TRILL_H__

#include "atts_cmn.h"
#include "atts_cmnornaments.h"
#include "controlelement.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Trill
//----------------------------------------------------------------------------

class Trill : public ControlElement, public TimePointInterface, public AttColor, public AttOrnamentaccid, public AttPlacement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Trill();
    virtual ~Trill();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Trill"; }
    virtual ClassId GetClassId() const { return TRILL; }
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

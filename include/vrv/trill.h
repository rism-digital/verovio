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

/**
 * This class models the MEI <trill> element.
 */
class Trill : public ControlElement,
              public TimeSpanningInterface,
              public AttColor,
              public AttOrnamentAccid,
              public AttPlacement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Trill();
    virtual ~Trill();
    virtual Object *Clone() const { return new Trill(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Trill"; }
    virtual ClassId GetClassId() const { return TRILL; }
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

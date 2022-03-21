/////////////////////////////////////////////////////////////////////////////
// Name:        breath.h
// Author:      Klaus Rettinghaus
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_BREATH_H__
#define __VRV_BREATH_H__

#include "atts_cmn.h"
#include "controlelement.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Breath
//----------------------------------------------------------------------------

/**
 * This class models the MEI <breath> element.
 */
class Breath : public ControlElement, public TimePointInterface, public AttColor, public AttPlacementRelStaff {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Breath();
    virtual ~Breath();
    Object *Clone() const override { return new Breath(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Breath"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    TimePointInterface *GetTimePointInterface() override { return vrv_cast<TimePointInterface *>(this); }
    const TimePointInterface *GetTimePointInterface() const override
    {
        return vrv_cast<const TimePointInterface *>(this);
    }
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

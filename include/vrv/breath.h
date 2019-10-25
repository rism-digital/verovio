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
class Breath : public ControlElement, public TimePointInterface, public AttColor, public AttPlacement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Breath();
    virtual ~Breath();
    virtual Object *Clone() const { return new Breath(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Breath"; }
    virtual ClassId GetClassId() const { return BREATH; }
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

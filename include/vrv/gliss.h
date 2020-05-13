/////////////////////////////////////////////////////////////////////////////
// Name:        gliss.h
// Author:      Klaus Rettinghaus
// Created:     2020
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_GLISS_H__
#define __VRV_GLISS_H__

#include "atts_cmn.h"
#include "controlelement.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Gliss
//----------------------------------------------------------------------------

/**
 * This class models the MEI <gliss> element.
 */
class Gliss : public ControlElement,
              public TimeSpanningInterface,
              public AttColor,
              public AttLineRend,
              public AttLineRendBase,
              public AttNNumberLike {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Gliss();
    virtual ~Gliss();
    virtual Object *Clone() const { return new Gliss(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Gliss"; }
    virtual ClassId GetClassId() const { return GLISS; }
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
};

} // namespace vrv

#endif

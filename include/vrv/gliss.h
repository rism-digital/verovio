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
    Object *Clone() const override { return new Gliss(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Gliss"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    TimePointInterface *GetTimePointInterface() override { return dynamic_cast<TimePointInterface *>(this); }
    TimeSpanningInterface *GetTimeSpanningInterface() override { return dynamic_cast<TimeSpanningInterface *>(this); }
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

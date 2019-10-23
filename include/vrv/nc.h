/////////////////////////////////////////////////////////////////////////////
// Name:        nc.h
// Author:      Andrew Tran
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_NC_H__
#define __VRV_NC_H__

#include <assert.h>

//----------------------------------------------------------------------------

#include "atts_analytical.h"
#include "atts_neumes.h"
#include "atts_shared.h"
#include "durationinterface.h"
#include "layerelement.h"
#include "pitchinterface.h"
#include "positioninterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Nc
//----------------------------------------------------------------------------

/**
 * This class models the MEI <nc> element.
 */

class Nc : public LayerElement,
           public DurationInterface,
           public PitchInterface,
           public PositionInterface,
           public AttColor,
           public AttIntervalMelodic,
           public AttNcForm {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Nc();
    virtual ~Nc();
    virtual Object *Clone() const { return new Nc(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Nc"; }
    virtual ClassId GetClassId() const { return NC; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual DurationInterface *GetDurationInterface() { return dynamic_cast<DurationInterface *>(this); }
    virtual PitchInterface *GetPitchInterface() { return dynamic_cast<PitchInterface *>(this); }
    ///@}

private:
};

} // namespace vrv

#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        nc.h
// Author:      Andrew Tran
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_NC_H__
#define __VRV_NC_H__

#include <cassert>

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
    Object *Clone() const override { return new Nc(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Nc"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    DurationInterface *GetDurationInterface() override { return vrv_cast<DurationInterface *>(this); }
    const DurationInterface *GetDurationInterface() const override { return vrv_cast<const DurationInterface *>(this); }
    PitchInterface *GetPitchInterface() override { return vrv_cast<PitchInterface *>(this); }
    const PitchInterface *GetPitchInterface() const override { return vrv_cast<const PitchInterface *>(this); }
    ///@}

private:
};

} // namespace vrv

#endif

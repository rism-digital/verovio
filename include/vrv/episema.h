/////////////////////////////////////////////////////////////////////////////
// Name:        episema.h
// Author:      David Rizo
// Created:     2026
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_EPISEMA_H__
#define __VRV_EPISEMA_H__

#include "atts_shared.h"
#include "atts_visual.h"
#include "layerelement.h"
#include "offsetinterface.h"
#include "pitchinterface.h"
#include "positioninterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Episema
//----------------------------------------------------------------------------

class Episema : public LayerElement,
                public OffsetInterface,
                public PitchInterface,
                public PositionInterface,
                public AttColor,
                public AttEpisemaVis {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Episema();
    virtual ~Episema();
    Object *Clone() const override { return new Episema(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "episema"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    OffsetInterface *GetOffsetInterface() override { return vrv_cast<OffsetInterface *>(this); }
    const OffsetInterface *GetOffsetInterface() const override { return vrv_cast<const OffsetInterface *>(this); }
    PitchInterface *GetPitchInterface() override { return vrv_cast<PitchInterface *>(this); }
    const PitchInterface *GetPitchInterface() const override { return vrv_cast<const PitchInterface *>(this); }
    PositionInterface *GetPositionInterface() override { return vrv_cast<PositionInterface *>(this); }
    const PositionInterface *GetPositionInterface() const override { return vrv_cast<const PositionInterface *>(this); }
    ///@}

    /** Override the method since alignment is required */
    bool HasToBeAligned() const override { return true; }

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif

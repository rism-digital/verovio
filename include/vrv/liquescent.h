/////////////////////////////////////////////////////////////////////////////
// Name:        liquescent.h
// Author:      Gabby Halpin
// Created:     2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_LIQUESCENT_H__
#define __VRV_LIQUESCENT_H__

#include "atts_analytical.h"
#include "atts_shared.h"
#include "layerelement.h"
#include "pitchinterface.h"
#include "positioninterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Liquescent
//----------------------------------------------------------------------------

class Liquescent : public LayerElement, public PitchInterface, public PositionInterface, public AttColor {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Liquescent();
    virtual ~Liquescent();
    Object *Clone() const override { return new Liquescent(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "liquescent"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
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

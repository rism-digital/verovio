/////////////////////////////////////////////////////////////////////////////
// Name:        quilisma.h
// Author:      Klaus Rettinghaus
// Created:     2024
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_quilisma_H__
#define __VRV_quilisma_H__

#include "atts_analytical.h"
#include "atts_shared.h"
#include "layerelement.h"
#include "pitchinterface.h"
#include "positioninterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// quilisma
//----------------------------------------------------------------------------

class Quilisma : public LayerElement, public PitchInterface, public PositionInterface, public AttColor {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Quilisma();
    virtual ~Quilisma();
    Object *Clone() const override { return new Quilisma(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "quilisma"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    PitchInterface *GetPitchInterface() override { return vrv_cast<PitchInterface *>(this); }
    const PitchInterface *GetPitchInterface() const override { return vrv_cast<const PitchInterface *>(this); }
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

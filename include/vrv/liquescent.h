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
    virtual Object *Clone() const { return new Liquescent(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Liquescent"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual PitchInterface *GetPitchInterface() { return dynamic_cast<PitchInterface *>(this); }
    ///@}

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif

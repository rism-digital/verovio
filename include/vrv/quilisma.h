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
    virtual Object *Clone() const { return new Quilisma(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Quilisma"; }
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

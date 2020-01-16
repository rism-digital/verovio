/////////////////////////////////////////////////////////////////////////////
// Name:        custos.h
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CUSTOS_H__
#define __VRV_CUSTOS_H__

#include "atts_analytical.h"
#include "atts_shared.h"
#include "layerelement.h"
#include "pitchinterface.h"
#include "positioninterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Custos
//----------------------------------------------------------------------------

class Custos : public LayerElement, public PitchInterface, public PositionInterface, public AttColor {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Custos();
    virtual ~Custos();
    virtual Object *Clone() const { return new Custos(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Custos"; }
    virtual ClassId GetClassId() const { return CUSTOS; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual PitchInterface *GetPitchInterface() { return dynamic_cast<PitchInterface *>(this); }
    ///@}

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

    /**
     * See Object::ResetHorizontalAlignment
     */
    virtual int ResetHorizontalAlignment(FunctorParams *functorParams);

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif

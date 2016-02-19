/////////////////////////////////////////////////////////////////////////////
// Name:        custos.h
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CUSTOS_H__
#define __VRV_CUSTOS_H__

#include "layerelement.h"
#include "positioninterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Custos
//----------------------------------------------------------------------------

class Custos : public LayerElement, public PositionInterface {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Custos();
    virtual ~Custos();
    virtual void Reset();
    virtual std::string GetClassName() { return "Custos"; };
    virtual ClassId Is() { return CUSTOS; };
    ///@}

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() { return true; };

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        plica.h
// Author:      Laurent Pugin
// Created:     2020
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PLICA_H__
#define __VRV_PLICA_H__

#include "atts_mensural.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Plica
//----------------------------------------------------------------------------

class Plica : public LayerElement, public AttPlicaVis {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Plica();
    virtual ~Plica();
    virtual Object *Clone() const { return new Plica(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Plica"; }
    virtual ClassId GetClassId() const { return PLICA; }

    //----------//
    // Functors //
    //----------//

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif

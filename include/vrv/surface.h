/////////////////////////////////////////////////////////////////////////////
// Name:        surface.h
// Author:      Juliette Regimbal
// Created:     05/06/2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SURFACE_H__
#define __VRV_SURFACE_H__

#include <cassert>

//----------------------------------------------------------------------------

#include "atts_analytical.h"
#include "atts_shared.h"
#include "object.h"
#include "vrvdef.h"

namespace vrv {

//----------------------------------------------------------------------------
// Surface
//----------------------------------------------------------------------------
/**
 * Implements the surface element
 * in MEI
 */

class Surface : public Object, public AttTyped, public AttCoordinated, public AttCoordinatedUl {
public:
    /**
     * @name Constructors, destructors, reset, and class name methods
     */
    ///@{
    Surface();
    virtual ~Surface();
    Object *Clone() const override { return new Surface(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "surface"; }
    ///@}
    bool IsSupportedChild(ClassId classId) override;

    int GetMaxX() const;
    int GetMaxY() const;

    //----------//
    // Functors //
    //----------//

    /**
     * Interface for class functor visitation
     */
    ///@{
    FunctorCode Accept(Functor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(Functor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
    ///@}

protected:
    //
private:
    //
};

} // namespace vrv

#endif

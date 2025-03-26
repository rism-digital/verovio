/////////////////////////////////////////////////////////////////////////////
// Name:        graphic.h
// Author:      Werner Goebl
// Created:     26/09/2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_GRAPHIC_H__
#define __VRV_GRAPHIC_H__

#include <cassert>

//----------------------------------------------------------------------------

#include "atts_analytical.h"
#include "atts_shared.h"
#include "object.h"
#include "vrvdef.h"

namespace vrv {

//----------------------------------------------------------------------------
// Graphic
//----------------------------------------------------------------------------
/**
 * Implements the graphic element
 * in MEI
 */

class Graphic : public Object, public AttPointing, public AttWidth, public AttHeight, public AttTyped {
public:
    /**
     * @name Constructors, destructors, reset, and class name methods
     */
    ///@{
    Graphic();
    virtual ~Graphic();
    Object *Clone() const override { return new Graphic(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "graphic"; }
    ///@}

    /**
     * @name Getters for the drawing width and height.
     */
    ///@{
    int GetDrawingWidth(int unit, int staffSize) const;
    int GetDrawingHeight(int unit, int staffSize) const;
    ///@}

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

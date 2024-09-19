/////////////////////////////////////////////////////////////////////////////
// Name:        genericlayerelement.h
// Author:      Laurent Pugin
// Created:     2024
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_GENERICLAYERELEMENT_H__
#define __VRV_GENERICLAYERELEMENT_H__

#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// GenericLayerElement
//----------------------------------------------------------------------------

/**
 * This class holds generic elements existing within MEI <layer> but not supported by Verovio
 */
class GenericLayerElement : public LayerElement {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    GenericLayerElement();
    virtual ~GenericLayerElement();
    Object *Clone() const override { return new GenericLayerElement(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Space"; }
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

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif

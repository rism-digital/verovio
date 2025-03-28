/////////////////////////////////////////////////////////////////////////////
// Name:        layerdef.h
// Author:      Andriy Makarchuk
// Created:     2022/02/28
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_LAYERDEF_H__
#define __VRV_LAYERDEF_H__

#include "atts_mei.h"
#include "atts_midi.h"
#include "atts_shared.h"
#include "object.h"

namespace vrv {

class LayerDef : public Object, public AttLabelled, public AttNInteger, public AttTyped {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    LayerDef();
    virtual ~LayerDef();
    Object *Clone() const override { return new LayerDef(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "layerDef"; }
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    bool IsSupportedChild(ClassId classId) override;
    ///@}

    /**
     * Interface for class functor visitation
     */
    ///@{
    FunctorCode Accept(Functor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(Functor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
    ///@}
};

} // namespace vrv

#endif //__VRV_LAYERDEF_H__

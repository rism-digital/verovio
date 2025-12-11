/////////////////////////////////////////////////////////////////////////////
// Name:        adjustossiastaffdeffunctor.h
// Author:      Laurent Pugin
// Created:     2025
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ADJUSTOSSIASTAFFDEFFUNCTOR_H__
#define __VRV_ADJUSTOSSIASTAFFDEFFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// AdjustOssiaStaffDefFunctor
//----------------------------------------------------------------------------

/**
 * This class adjusts the position of the dots for multiple layers.
 */
class AdjustOssiaStaffDefFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustOssiaStaffDefFunctor(Doc *doc);
    virtual ~AdjustOssiaStaffDefFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitAlignment(Alignment *alignment) override;
    FunctorCode VisitLayerElement(LayerElement *layerElement) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The key signature maxmimal width;
    int m_keySigWidth;
    // The current staff size
    int m_staffSize;
};

} // namespace vrv

#endif // __VRV_ADJUSTDOTSFUNCTOR_H__

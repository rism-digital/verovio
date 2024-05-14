/////////////////////////////////////////////////////////////////////////////
// Name:        adjustyrelfortranscriptionfunctor.h
// Author:      Yinan Zhou
// Created:     2024
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ADJUSTYRELFORTRANSCRIPTIONFUNCTOR_H__
#define __VRV_ADJUSTYRELFORTRANSCRIPTIONFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// AdjustYRelForTranscriptionFunctor
//----------------------------------------------------------------------------

/**
 * This class adjusts the YRel positions taking into account the bounding boxes.
 */
class AdjustYRelForTranscriptionFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustYRelForTranscriptionFunctor();
    virtual ~AdjustYRelForTranscriptionFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitLayerElement(LayerElement *layerElement) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif // __VRV_ADJUSTYRELFORTRANSCRIPTIONFUNCTOR_H__

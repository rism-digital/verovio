/////////////////////////////////////////////////////////////////////////////
// Name:        adjustxrelfortranscriptionfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ADJUSTXRELFORTRANSCRIPTIONFUNCTOR_H__
#define __VRV_ADJUSTXRELFORTRANSCRIPTIONFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// AdjustXRelForTranscriptionFunctor
//----------------------------------------------------------------------------

/**
 * This class adjusts the XRel positions taking into account the bounding boxes.
 */
class AdjustXRelForTranscriptionFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustXRelForTranscriptionFunctor();
    virtual ~AdjustXRelForTranscriptionFunctor() = default;
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

#endif // __VRV_ADJUSTXRELFORTRANSCRIPTIONFUNCTOR_H__

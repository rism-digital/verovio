/////////////////////////////////////////////////////////////////////////////
// Name:        adjustxrelfortranscriptionfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustxrelfortranscriptionfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustXRelForTranscriptionFunctor
//----------------------------------------------------------------------------

AdjustXRelForTranscriptionFunctor::AdjustXRelForTranscriptionFunctor() : Functor() {}

FunctorCode AdjustXRelForTranscriptionFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (layerElement->m_drawingFacsX == VRV_UNSET) return FUNCTOR_CONTINUE;

    if (layerElement->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    if (!layerElement->HasSelfBB()) return FUNCTOR_CONTINUE;

    layerElement->SetDrawingXRel(-layerElement->GetSelfX1());

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

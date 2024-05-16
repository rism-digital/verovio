/////////////////////////////////////////////////////////////////////////////
// Name:        adjustyrelfortranscriptionfunctor.cpp
// Author:      Yinan Zhou
// Created:     2024
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustyrelfortranscriptionfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustYRelForTranscriptionFunctor
//----------------------------------------------------------------------------

AdjustYRelForTranscriptionFunctor::AdjustYRelForTranscriptionFunctor() : Functor() {}

FunctorCode AdjustYRelForTranscriptionFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (layerElement->m_drawingFacsY == VRV_UNSET) return FUNCTOR_CONTINUE;

    if (layerElement->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    if (!layerElement->HasSelfBB()) return FUNCTOR_CONTINUE;

    layerElement->SetDrawingYRel(-layerElement->GetSelfY1());

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

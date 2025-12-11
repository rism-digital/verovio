/////////////////////////////////////////////////////////////////////////////
// Name:        adjustossiastaffdeffunctor.cpp
// Author:      Laurent Pugin
// Created:     2025
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustossiastaffdeffunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "elementpart.h"
#include "staff.h"
#include "system.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustOssiaStaffDefFunctor
//----------------------------------------------------------------------------

AdjustOssiaStaffDefFunctor::AdjustOssiaStaffDefFunctor(Doc *doc) : DocFunctor(doc) {}

FunctorCode AdjustOssiaStaffDefFunctor::VisitAlignment(Alignment *alignment)
{
    if (alignment->GetType() >= ALIGNMENT_MEASURE_START) return FUNCTOR_SIBLINGS;

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustOssiaStaffDefFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (layerElement->Is(KEYSIG)) {
        int width = layerElement->GetContentX1() + layerElement->GetContentX2();
        layerElement->SetDrawingXRel(-width - m_doc->GetDrawingUnit(m_staffSize));
        m_keySigWidth = std::max(width, m_keySigWidth);
    }
    else if (layerElement->Is(CLEF)) {
        int width = layerElement->GetContentX1() + layerElement->GetContentX2();
        layerElement->SetDrawingXRel(-width - 2 * m_doc->GetDrawingUnit(m_staffSize) - m_keySigWidth);
    }

    return FUNCTOR_SIBLINGS;
}

FunctorCode AdjustOssiaStaffDefFunctor::VisitMeasure(Measure *measure)
{
    m_keySigWidth = 0;
    m_staffSize = 100;

    // Process on measure aligner backwards
    this->SetDirection(BACKWARD);
    measure->m_measureAligner.Process(*this);
    this->SetDirection(FORWARD);

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv

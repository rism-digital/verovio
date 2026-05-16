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
#include "ossia.h"
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

    if (alignment->GetType() == ALIGNMENT_SCOREDEF_OSSIA_KEYSIG) {
        m_keySigAlignment = alignment;
    }
    else if (alignment->GetType() == ALIGNMENT_SCOREDEF_OSSIA_CLEF) {
        m_clefAlignment = alignment;
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustOssiaStaffDefFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (layerElement->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    const int unit = m_doc->GetDrawingUnit(m_staffSize);

    if (layerElement->Is(KEYSIG)) {
        int width = layerElement->GetContentX1() + layerElement->GetContentX2() + unit;
        m_keySigWidth = std::max(width, m_keySigWidth);
    }
    else if (layerElement->Is(CLEF)) {
        int width = layerElement->GetContentX1() + layerElement->GetContentX2() + unit;
        m_clefWidth = std::max(width, m_clefWidth);
    }
    Ossia *ossia = vrv_cast<Ossia *>(layerElement->GetFirstAncestor(OSSIA));
    assert(ossia);
    m_ossias.push_back(ossia);

    return FUNCTOR_SIBLINGS;
}

FunctorCode AdjustOssiaStaffDefFunctor::VisitMeasure(Measure *measure)
{
    m_keySigWidth = 0;
    m_clefWidth = 0;
    m_staffSize = 100;

    m_keySigAlignment = NULL;
    m_clefAlignment = NULL;

    m_ossias.clear();

    // Process on measure aligner backwards
    this->SetDirection(BACKWARD);
    measure->m_measureAligner.Process(*this);
    this->SetDirection(FORWARD);

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustOssiaStaffDefFunctor::VisitMeasureEnd(Measure *measure)
{
    if (m_keySigAlignment) {
        m_keySigAlignment->SetXRel(-m_keySigWidth);
    }
    if (m_clefAlignment) {
        m_clefAlignment->SetXRel(-m_keySigWidth - m_clefWidth);
    }

    m_ossias.unique();

    for (auto ossia : m_ossias) {
        ossia->SetClefAlignment(m_clefAlignment);
        ossia->SetKeySigAlignment(m_keySigAlignment);
    }

    return FUNCTOR_SIBLINGS;
}

FunctorCode AdjustOssiaStaffDefFunctor::VisitStaff(Staff *staff)
{
    return FUNCTOR_SIBLINGS;
}

} // namespace vrv

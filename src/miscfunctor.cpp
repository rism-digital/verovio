/////////////////////////////////////////////////////////////////////////////
// Name:        miscfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "miscfunctor.h"

//----------------------------------------------------------------------------

#include "page.h"
#include "staff.h"
#include "system.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// ApplyPPUFactorFunctor
//----------------------------------------------------------------------------

ApplyPPUFactorFunctor::ApplyPPUFactorFunctor()
{
    m_page = NULL;
}

FunctorCode ApplyPPUFactorFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (layerElement->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    if (layerElement->m_xAbs != VRV_UNSET) layerElement->m_xAbs /= m_page->GetPPUFactor();

    return FUNCTOR_CONTINUE;
}

FunctorCode ApplyPPUFactorFunctor::VisitMeasure(Measure *measure)
{
    if (measure->m_xAbs != VRV_UNSET) measure->m_xAbs /= m_page->GetPPUFactor();
    if (measure->m_xAbs2 != VRV_UNSET) measure->m_xAbs2 /= m_page->GetPPUFactor();

    return FUNCTOR_CONTINUE;
}

FunctorCode ApplyPPUFactorFunctor::VisitPage(Page *page)
{
    m_page = page;
    m_page->m_pageWidth /= m_page->GetPPUFactor();
    m_page->m_pageHeight /= m_page->GetPPUFactor();
    m_page->m_pageMarginBottom /= m_page->GetPPUFactor();
    m_page->m_pageMarginLeft /= m_page->GetPPUFactor();
    m_page->m_pageMarginRight /= m_page->GetPPUFactor();
    m_page->m_pageMarginTop /= m_page->GetPPUFactor();

    return FUNCTOR_CONTINUE;
}

FunctorCode ApplyPPUFactorFunctor::VisitStaff(Staff *staff)
{
    if (staff->m_yAbs != VRV_UNSET) staff->m_yAbs /= m_page->GetPPUFactor();

    return FUNCTOR_CONTINUE;
}

FunctorCode ApplyPPUFactorFunctor::VisitSystem(System *system)
{
    if (system->m_xAbs != VRV_UNSET) system->m_xAbs /= m_page->GetPPUFactor();
    if (system->m_yAbs != VRV_UNSET) system->m_yAbs /= m_page->GetPPUFactor();
    system->m_systemLeftMar *= m_page->GetPPUFactor();
    system->m_systemRightMar *= m_page->GetPPUFactor();

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// GetAlignmentLeftRightFunctor
//----------------------------------------------------------------------------

GetAlignmentLeftRightFunctor::GetAlignmentLeftRightFunctor()
{
    m_minLeft = -VRV_UNSET;
    m_maxRight = VRV_UNSET;
}

FunctorCode GetAlignmentLeftRightFunctor::VisitObject(const Object *object)
{
    if (!object->IsLayerElement()) return FUNCTOR_CONTINUE;

    if (!object->HasSelfBB() || object->HasEmptyBB()) return FUNCTOR_CONTINUE;

    if (object->Is(m_excludeClasses)) return FUNCTOR_CONTINUE;

    m_minLeft = std::min(m_minLeft, object->GetSelfLeft());
    m_maxRight = std::max(m_maxRight, object->GetSelfRight());

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

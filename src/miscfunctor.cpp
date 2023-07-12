/////////////////////////////////////////////////////////////////////////////
// Name:        miscfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "miscfunctor.h"

//----------------------------------------------------------------------------

#include "layer.h"
#include "page.h"
#include "staff.h"
#include "system.h"
#include "verse.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// ApplyPPUFactorFunctor
//----------------------------------------------------------------------------

ApplyPPUFactorFunctor::ApplyPPUFactorFunctor() : Functor()
{
    m_ppuFactor = 1.0;
}

FunctorCode ApplyPPUFactorFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (layerElement->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    if (layerElement->m_xAbs != VRV_UNSET) layerElement->m_xAbs /= m_ppuFactor;

    return FUNCTOR_CONTINUE;
}

FunctorCode ApplyPPUFactorFunctor::VisitMeasure(Measure *measure)
{
    if (measure->m_xAbs != VRV_UNSET) measure->m_xAbs /= m_ppuFactor;
    if (measure->m_xAbs2 != VRV_UNSET) measure->m_xAbs2 /= m_ppuFactor;

    return FUNCTOR_CONTINUE;
}

FunctorCode ApplyPPUFactorFunctor::VisitPage(Page *page)
{
    m_ppuFactor = page->GetPPUFactor();

    page->m_pageWidth /= m_ppuFactor;
    page->m_pageHeight /= m_ppuFactor;
    page->m_pageMarginBottom /= m_ppuFactor;
    page->m_pageMarginLeft /= m_ppuFactor;
    page->m_pageMarginRight /= m_ppuFactor;
    page->m_pageMarginTop /= m_ppuFactor;

    return FUNCTOR_CONTINUE;
}

FunctorCode ApplyPPUFactorFunctor::VisitStaff(Staff *staff)
{
    if (staff->m_yAbs != VRV_UNSET) staff->m_yAbs /= m_ppuFactor;

    return FUNCTOR_CONTINUE;
}

FunctorCode ApplyPPUFactorFunctor::VisitSystem(System *system)
{
    if (system->m_xAbs != VRV_UNSET) system->m_xAbs /= m_ppuFactor;
    if (system->m_yAbs != VRV_UNSET) system->m_yAbs /= m_ppuFactor;
    system->m_systemLeftMar *= m_ppuFactor;
    system->m_systemRightMar *= m_ppuFactor;

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// GetAlignmentLeftRightFunctor
//----------------------------------------------------------------------------

GetAlignmentLeftRightFunctor::GetAlignmentLeftRightFunctor() : ConstFunctor()
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

//----------------------------------------------------------------------------
// InitProcessingListsFunctor
//----------------------------------------------------------------------------

InitProcessingListsFunctor::InitProcessingListsFunctor() : ConstFunctor() {}

FunctorCode InitProcessingListsFunctor::VisitLayer(const Layer *layer)
{
    const Staff *staff = vrv_cast<const Staff *>(layer->GetFirstAncestor(STAFF));
    assert(staff);
    m_layerTree.child[staff->GetN()].child[layer->GetN()];

    return FUNCTOR_CONTINUE;
}

FunctorCode InitProcessingListsFunctor::VisitVerse(const Verse *verse)
{
    const Staff *staff = verse->GetAncestorStaff();
    const Layer *layer = vrv_cast<const Layer *>(verse->GetFirstAncestor(LAYER));
    assert(layer);

    m_verseTree.child[staff->GetN()].child[layer->GetN()].child[verse->GetN()];

    return FUNCTOR_SIBLINGS;
}

//----------------------------------------------------------------------------
// ReorderByXPosFunctor
//----------------------------------------------------------------------------

ReorderByXPosFunctor::ReorderByXPosFunctor() : Functor() {}

FunctorCode ReorderByXPosFunctor::VisitObject(Object *object)
{
    if (object->GetFacsimileInterface()) {
        if (object->GetFacsimileInterface()->HasFacs()) {
            return FUNCTOR_SIBLINGS; // This would have already been reordered.
        }
    }

    ArrayOfObjects &children = object->GetChildrenForModification();
    std::stable_sort(children.begin(), children.end(), Object::sortByUlx);

    object->Modify();

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

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
    m_page = NULL;
}

FunctorCode ApplyPPUFactorFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (layerElement->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    if (layerElement->m_drawingFacsX != VRV_UNSET) layerElement->m_drawingFacsX /= m_page->GetPPUFactor();

    return FUNCTOR_CONTINUE;
}

FunctorCode ApplyPPUFactorFunctor::VisitMeasure(Measure *measure)
{
    if (measure->m_drawingFacsX1 != VRV_UNSET) measure->m_drawingFacsX1 /= m_page->GetPPUFactor();
    if (measure->m_drawingFacsX2 != VRV_UNSET) measure->m_drawingFacsX2 /= m_page->GetPPUFactor();

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
    if (staff->m_drawingFacsY != VRV_UNSET) staff->m_drawingFacsY /= m_page->GetPPUFactor();

    return FUNCTOR_CONTINUE;
}

FunctorCode ApplyPPUFactorFunctor::VisitSystem(System *system)
{
    if (system->m_drawingFacsX != VRV_UNSET) system->m_drawingFacsX /= m_page->GetPPUFactor();
    if (system->m_drawingFacsY != VRV_UNSET) system->m_drawingFacsY /= m_page->GetPPUFactor();
    system->m_systemLeftMar *= m_page->GetPPUFactor();
    system->m_systemRightMar *= m_page->GetPPUFactor();

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

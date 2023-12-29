/////////////////////////////////////////////////////////////////////////////
// Name:        facsimilefunctor.cpp
// Author:      Laurent Pugin
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "facsimilefunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "layerelement.h"
#include "measure.h"
#include "page.h"
#include "pb.h"
#include "sb.h"
#include "staff.h"
#include "surface.h"
#include "system.h"
#include "vrv.h"
#include "zone.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// SyncFromFacsimileFunctor
//----------------------------------------------------------------------------

SyncFromFacsimileFunctor::SyncFromFacsimileFunctor(Doc *doc) : Functor()
{
    m_doc = doc;
    m_view.SetDoc(doc);
    m_currentPage = NULL;
    m_currentSystem = NULL;
}

FunctorCode SyncFromFacsimileFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (!layerElement->Is({ NOTE, REST })) return FUNCTOR_CONTINUE;

    Zone *zone = layerElement->GetZone();
    assert(zone);
    layerElement->m_drawingFacsX = m_view.ToLogicalX(zone->GetUlx() * DEFINITION_FACTOR);

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncFromFacsimileFunctor::VisitMeasure(Measure *measure)
{
    Zone *zone = measure->GetZone();
    assert(zone);
    measure->m_drawingFacsX1 = m_view.ToLogicalX(zone->GetUlx() * DEFINITION_FACTOR);
    measure->m_drawingFacsX2 = m_view.ToLogicalX(zone->GetLrx() * DEFINITION_FACTOR);

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncFromFacsimileFunctor::VisitPage(Page *page)
{
    m_currentPage = page;
    m_doc->SetDrawingPage(m_currentPage->GetIdx());

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncFromFacsimileFunctor::VisitPb(Pb *pb)
{
    // This would happen if we run the functor on data not converted to page-based
    assert(m_currentPage);

    Zone *zone = pb->GetZone();
    assert(zone && zone->GetParent());
    Surface *surface = (zone->GetParent()->Is(SURFACE)) ? vrv_cast<Surface *>(zone->GetParent()) : NULL;
    // Use the parent surface attributes if given
    if (surface && surface->HasLrx() && surface->HasLry()) {
        m_currentPage->m_pageHeight = surface->GetLry() * DEFINITION_FACTOR;
        m_currentPage->m_pageWidth = surface->GetLrx() * DEFINITION_FACTOR;
    }
    // Fallback on zone
    else {
        m_currentPage->m_pageHeight = zone->GetLry() * DEFINITION_FACTOR;
        m_currentPage->m_pageWidth = zone->GetLrx() * DEFINITION_FACTOR;
    }
    // Update the page size to have to View::ToLogicalX/Y valid
    m_doc->UpdatePageDrawingSizes();

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncFromFacsimileFunctor::VisitSb(Sb *sb)
{
    // This would happen if we run the functor on data not converted to page-based
    assert(m_currentSystem);

    Zone *zone = sb->GetZone();
    assert(zone);
    m_currentSystem->m_drawingFacsX = m_view.ToLogicalX(zone->GetUlx() * DEFINITION_FACTOR);
    m_currentSystem->m_drawingFacsY = m_view.ToLogicalY(zone->GetUly() * DEFINITION_FACTOR);

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncFromFacsimileFunctor::VisitStaff(Staff *staff)
{
    Zone *zone = staff->GetZone();
    assert(zone);
    staff->m_drawingFacsY = m_view.ToLogicalY(zone->GetUly() * DEFINITION_FACTOR);

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncFromFacsimileFunctor::VisitSystem(System *system)
{
    m_currentSystem = system;

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// SyncToFacsimileFunctor
//----------------------------------------------------------------------------

SyncToFacsimileFunctor::SyncToFacsimileFunctor(Doc *doc) : Functor()
{
    m_doc = doc;
    m_view.SetDoc(doc);
    m_surface = NULL;
    m_currentPage = NULL;
    m_currentSystem = NULL;
    m_pageMarginTop = 0;
    m_pageMarginLeft = 0;
}

FunctorCode SyncToFacsimileFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (!layerElement->Is({ NOTE, REST })) return FUNCTOR_CONTINUE;

    Zone *zone = this->GetZone(layerElement, layerElement->GetClassName());
    zone->SetUlx(m_view.ToDeviceContextX(layerElement->GetDrawingX()) / DEFINITION_FACTOR + m_pageMarginLeft);

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncToFacsimileFunctor::VisitMeasure(Measure *measure)
{
    Zone *zone = this->GetZone(measure, measure->GetClassName());
    zone->SetUlx(m_view.ToDeviceContextX(measure->GetDrawingX()) / DEFINITION_FACTOR + m_pageMarginLeft);
    zone->SetLrx(
        m_view.ToDeviceContextX(measure->GetDrawingX() + measure->GetWidth()) / DEFINITION_FACTOR + m_pageMarginLeft);

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncToFacsimileFunctor::VisitPage(Page *page)
{
    m_currentPage = page;
    m_doc->SetDrawingPage(page->GetIdx());
    page->LayOut();
    //
    m_surface = new Surface();
    assert(m_doc->GetFacsimile());
    m_doc->GetFacsimile()->AddChild(m_surface);
    m_surface->SetLrx(m_doc->m_drawingPageWidth / DEFINITION_FACTOR);
    m_surface->SetLry(m_doc->m_drawingPageHeight / DEFINITION_FACTOR);
    // Because the facsimile output zone positions include the margins, we will add them to each zone
    m_pageMarginTop = m_doc->m_drawingPageMarginTop / DEFINITION_FACTOR;
    m_pageMarginLeft = m_doc->m_drawingPageMarginLeft / DEFINITION_FACTOR;

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncToFacsimileFunctor::VisitPb(Pb *pb)
{
    Zone *zone = this->GetZone(pb, pb->GetClassName());
    // The Pb zone values are currently not used in SyncFromFacsimileFunctor because the
    // page sizes are synced from the parent Surface and zone positions include margins
    zone->SetUlx(m_pageMarginLeft);
    zone->SetUly(m_pageMarginTop);
    zone->SetLrx(m_doc->m_drawingPageContentWidth / DEFINITION_FACTOR + m_pageMarginLeft);
    zone->SetLry(m_doc->m_drawingPageContentHeight / DEFINITION_FACTOR + m_pageMarginTop);

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncToFacsimileFunctor::VisitSb(Sb *sb)
{
    Zone *zone = this->GetZone(sb, sb->GetClassName());
    zone->SetUlx(m_view.ToDeviceContextX(m_currentSystem->GetDrawingX()) / DEFINITION_FACTOR + m_pageMarginLeft);
    zone->SetUly(m_view.ToDeviceContextY(m_currentSystem->GetDrawingY()) / DEFINITION_FACTOR + m_pageMarginTop);

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncToFacsimileFunctor::VisitStaff(Staff *staff)
{
    Zone *zone = this->GetZone(staff, staff->GetClassName());
    zone->SetUly(m_view.ToDeviceContextY(staff->GetDrawingY()) / DEFINITION_FACTOR + m_pageMarginTop);

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncToFacsimileFunctor::VisitSystem(System *system)
{
    m_currentSystem = system;

    return FUNCTOR_CONTINUE;
}

Zone *SyncToFacsimileFunctor::GetZone(FacsimileInterface *interface, std::string type)
{
    if (interface->GetZone()) {
        // Here we should probably check if the zone is a child of m_surface
        return interface->GetZone();
    }
    else {
        Zone *zone = new Zone();
        std::transform(type.begin(), type.end(), type.begin(), ::tolower);
        zone->SetType(type);
        m_surface->AddChild(zone);
        interface->SetFacs(StringFormat("#%s", zone->GetID().c_str()));
        interface->AttachZone(zone);
        return interface->GetZone();
    }
}

} // namespace vrv

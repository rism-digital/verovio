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

SyncFromFacsimileFunctor::SyncFromFacsimileFunctor() : Functor()
{
    m_currentPage = NULL;
    m_currentSystem = NULL;
}

FunctorCode SyncFromFacsimileFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (!layerElement->Is({ NOTE, REST })) return FUNCTOR_CONTINUE;

    Zone *zone = layerElement->GetZone();
    assert(zone);
    layerElement->m_xAbs = zone->GetUlx() * DEFINITION_FACTOR;

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncFromFacsimileFunctor::VisitMeasure(Measure *measure)
{
    Zone *zone = measure->GetZone();
    assert(zone);
    measure->m_xAbs = zone->GetUlx() * DEFINITION_FACTOR;
    measure->m_xAbs2 = zone->GetLrx() * DEFINITION_FACTOR;

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncFromFacsimileFunctor::VisitPage(Page *page)
{
    m_currentPage = page;

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncFromFacsimileFunctor::VisitPb(Pb *pb)
{
    // This would happen if we run the functor on data not converted to page-based
    assert(m_currentPage);

    Zone *zone = pb->GetZone();
    assert(zone);
    m_currentPage->m_pageHeight = zone->GetLry() * DEFINITION_FACTOR;
    m_currentPage->m_pageWidth = zone->GetLrx() * DEFINITION_FACTOR;

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncFromFacsimileFunctor::VisitSb(Sb *sb)
{
    // This would happen if we run the functor on data not converted to page-based
    assert(m_currentSystem);

    Zone *zone = sb->GetZone();
    assert(zone);
    m_currentSystem->m_xAbs = zone->GetUlx() * DEFINITION_FACTOR;
    m_currentSystem->m_yAbs = zone->GetUly() * DEFINITION_FACTOR;

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncFromFacsimileFunctor::VisitStaff(Staff *staff)
{
    Zone *zone = staff->GetZone();
    assert(zone);
    staff->m_yAbs = zone->GetUly() * DEFINITION_FACTOR;

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
}

FunctorCode SyncToFacsimileFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (!layerElement->Is({ NOTE, REST })) return FUNCTOR_CONTINUE;

    // layerElement->m_xAbs = zone->GetUlx() * DEFINITION_FACTOR;
    Zone *zone = this->GetZone(layerElement, layerElement->GetClassName());
    zone->SetUlx(m_view.ToLogicalX(layerElement->GetDrawingX()) / DEFINITION_FACTOR);

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncToFacsimileFunctor::VisitMeasure(Measure *measure)
{
    // measure->m_xAbs = zone->GetUlx() * DEFINITION_FACTOR;
    // measure->m_xAbs2 = zone->GetLrx() * DEFINITION_FACTOR;
    Zone *zone = this->GetZone(measure, measure->GetClassName());
    zone->SetUlx(measure->GetDrawingX() / DEFINITION_FACTOR);
    zone->SetLrx(measure->GetDrawingX() + measure->GetWidth() / DEFINITION_FACTOR);

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncToFacsimileFunctor::VisitPage(Page *page)
{
    m_currentPage = page;
    m_doc->SetDrawingPage(page->GetIdx());
    page->LayOut();
    //
    Surface *surface = new Surface();
    assert(m_doc->GetFacsimile());
    m_doc->GetFacsimile()->AddChild(surface);
    surface->SetLrx(m_doc->m_drawingPageWidth / DEFINITION_FACTOR);
    surface->SetLry(m_doc->m_drawingPageHeight / DEFINITION_FACTOR);
    m_surface = new Surface();
    surface->AddChild(m_surface);
    m_surface->SetUlx(m_doc->m_drawingPageMarginLeft / DEFINITION_FACTOR);
    m_surface->SetUly(m_doc->m_drawingPageMarginRight / DEFINITION_FACTOR);
    m_surface->SetLrx(m_doc->m_drawingPageContentWidth / DEFINITION_FACTOR);
    m_surface->SetLry(m_doc->m_drawingPageContentHeight / DEFINITION_FACTOR);

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncToFacsimileFunctor::VisitPb(Pb *pb)
{
    // m_currentPage->m_pageHeight = zone->GetLry() * DEFINITION_FACTOR;
    // m_currentPage->m_pageWidth = zone->GetLrx() * DEFINITION_FACTOR;
    // Zone *zone = this->GetZone(pb, pb->GetClassName());

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncToFacsimileFunctor::VisitSb(Sb *sb)
{
    // m_currentSystem->m_xAbs = zone->GetUlx() * DEFINITION_FACTOR;
    // m_currentSystem->m_yAbs = zone->GetUly() * DEFINITION_FACTOR;
    Zone *zone = this->GetZone(sb, sb->GetClassName());
    zone->SetUlx(m_view.ToLogicalX(m_currentSystem->GetDrawingX()) / DEFINITION_FACTOR);
    zone->SetUly(m_view.ToLogicalY(m_currentSystem->GetDrawingY()) / DEFINITION_FACTOR);

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncToFacsimileFunctor::VisitStaff(Staff *staff)
{
    // staff->m_yAbs = zone->GetUly() * DEFINITION_FACTOR;
    Zone *zone = this->GetZone(staff, staff->GetClassName());
    zone->SetUly(m_view.ToLogicalY(staff->GetDrawingY()) / DEFINITION_FACTOR);

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

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
#include "miscfunctor.h"
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
    m_pageMarginTop = 0;
    m_pageMarginLeft = 0;
    m_ppuFactor = 1.0;
}

FunctorCode SyncFromFacsimileFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (!layerElement->Is({ ACCID, BARLINE, CLEF, CUSTOS, DIVLINE, DOT, LIQUESCENT, NC, NOTE, REST, SYL }))
        return FUNCTOR_CONTINUE;

    Zone *zone = layerElement->GetZone();
    assert(zone);
    layerElement->m_drawingFacsX = m_view.ToLogicalX(zone->GetUlx() * DEFINITION_FACTOR - m_pageMarginLeft);
    if (m_currentNeumeLine && layerElement->Is({ ACCID, SYL })) {
        layerElement->m_drawingFacsY = m_view.ToLogicalY(zone->GetUly() * DEFINITION_FACTOR - m_pageMarginTop);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncFromFacsimileFunctor::VisitMeasure(Measure *measure)
{
    // neon specific code - measure have no zone, we will use the staff one in VisitStaff
    if (measure->IsNeumeLine()) {
        m_currentNeumeLine = measure;
    }
    else {
        Zone *zone = measure->GetZone();
        assert(zone);
        measure->m_drawingFacsX1 = m_view.ToLogicalX(zone->GetUlx() * DEFINITION_FACTOR - m_pageMarginLeft);
        measure->m_drawingFacsX2 = m_view.ToLogicalX(zone->GetLrx() * DEFINITION_FACTOR - m_pageMarginLeft);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncFromFacsimileFunctor::VisitPage(Page *page)
{
    m_staffZones.clear();
    m_currentPage = page;
    m_doc->SetDrawingPage(m_currentPage->GetIdx());

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncFromFacsimileFunctor::VisitPageEnd(Page *page)
{
    // Used for adjusting staff size in neon - filled in VisitStaff
    if (!m_staffZones.empty()) {
        // Since we multiply all values by the DEFINITION_FACTOR, set it as PPU for neon facs
        m_ppuFactor = DEFINITION_FACTOR;
    }

    // The staff size is calculated based on the zone height and takes into acocunt the rotation
    for (auto &[staff, zone] : m_staffZones) {
        double rotate = (zone->HasRotate()) ? zone->GetRotate() : 0.0;
        int yDiff
            = zone->GetLry() - zone->GetUly() - (zone->GetLrx() - zone->GetUlx()) * tan(abs(rotate) * M_PI / 180.0);
        staff->m_drawingStaffSize
            = 100 * yDiff / (m_doc->GetOptions()->m_unit.GetValue() * 2 * (staff->m_drawingLines - 1));
        staff->SetDrawingRotation(rotate);
    }

    m_currentPage->SetPPUFactor(m_ppuFactor);
    if (m_currentPage->GetPPUFactor() != 1.0) {
        ApplyPPUFactorFunctor applyPPUFactor;
        m_currentPage->Process(applyPPUFactor);
        m_doc->UpdatePageDrawingSizes();
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncFromFacsimileFunctor::VisitPb(Pb *pb)
{
    // This would happen if we run the functor on data not converted to page-based
    assert(m_currentPage);

    Zone *zone = pb->GetZone();
    Surface *surface = pb->GetSurface();
    if (!surface && zone && zone->GetParent())
        surface = (zone->GetParent()->Is(SURFACE)) ? vrv_cast<Surface *>(zone->GetParent()) : NULL;
    assert(zone || surface);
    // Use the (parent) surface attributes if given
    if (surface && surface->HasLrx() && surface->HasLry()) {
        m_currentPage->m_pageHeight = surface->GetLry() * DEFINITION_FACTOR;
        m_currentPage->m_pageWidth = surface->GetLrx() * DEFINITION_FACTOR;
        // Read the ppu factor from surface@type
        std::string surfaceType = surface->GetType();
        if (surfaceType.starts_with("ppu:")) {
            std::string ppuFactorStr = surfaceType.substr(surfaceType.find(":") + 1);
            if (vrv::IsValidDouble(ppuFactorStr)) {
                m_ppuFactor = std::strtod(ppuFactorStr.c_str(), NULL);
                m_ppuFactor *= DEFINITION_FACTOR / m_doc->GetOptions()->m_unit.GetValue();
            }
        }
        // Read margins
        if (zone && zone->HasUlx() && zone->HasUly() && zone->HasLrx() && zone->HasLry()) {
            m_pageMarginTop = zone->GetUly() * DEFINITION_FACTOR;
            m_pageMarginLeft = zone->GetUlx() * DEFINITION_FACTOR;
            m_currentPage->m_pageMarginTop = m_pageMarginTop;
            // Calculate the bottom margin looking at the surface lry and zone lry
            m_currentPage->m_pageMarginBottom = m_currentPage->m_pageHeight - zone->GetLry() * DEFINITION_FACTOR;
            m_currentPage->m_pageMarginLeft = m_pageMarginLeft;
            // Calculate the right margin looking at the surface lrx and zone lrx
            m_currentPage->m_pageMarginRight = m_currentPage->m_pageWidth - zone->GetLrx() * DEFINITION_FACTOR;
            ;
            m_doc->UpdatePageDrawingSizes();
        }
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
    m_currentSystem->m_drawingFacsX = m_view.ToLogicalX(zone->GetUlx() * DEFINITION_FACTOR - m_pageMarginLeft);
    m_currentSystem->m_drawingFacsY = m_view.ToLogicalY(zone->GetUly() * DEFINITION_FACTOR - m_pageMarginTop);

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncFromFacsimileFunctor::VisitStaff(Staff *staff)
{
    Zone *zone = staff->GetZone();
    assert(zone);
    staff->m_drawingFacsY = m_view.ToLogicalY(zone->GetUly() * DEFINITION_FACTOR - m_pageMarginTop);

    // neon specific code - set the position of the pseudo measure (neume line)
    if (m_currentNeumeLine) {
        m_currentNeumeLine->m_drawingFacsX1 = m_view.ToLogicalX(zone->GetUlx() * DEFINITION_FACTOR - m_pageMarginLeft);
        m_currentNeumeLine->m_drawingFacsX2 = m_view.ToLogicalX(zone->GetLrx() * DEFINITION_FACTOR - m_pageMarginLeft);
        m_staffZones[staff] = zone;

        // The staff slope is going up. The y left position needs to be adjusted accordingly
        if (zone->HasRotate() && zone->GetRotate() < 0) {
            staff->m_drawingFacsY = staff->m_drawingFacsY
                + (m_currentNeumeLine->m_drawingFacsX2 - m_currentNeumeLine->m_drawingFacsX1)
                    * tan(zone->GetRotate() * M_PI / 180.0);
        }
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncFromFacsimileFunctor::VisitSystem(System *system)
{
    m_currentSystem = system;
    m_currentNeumeLine = NULL;

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// SyncToFacsimileFunctor
//----------------------------------------------------------------------------

SyncToFacsimileFunctor::SyncToFacsimileFunctor(Doc *doc, double ppuFactor) : Functor()
{
    m_doc = doc;
    m_ppuFactor = ppuFactor;
    m_view.SetDoc(doc);
    m_surface = NULL;
    m_currentPage = NULL;
    m_currentSystem = NULL;
    m_pageMarginTop = 0;
    m_pageMarginLeft = 0;
    m_currentNeumeLine = false;
}

FunctorCode SyncToFacsimileFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (!layerElement->Is({ ACCID, BARLINE, CLEF, CUSTOS, DOT, DIVLINE, LIQUESCENT, NC, NOTE, REST, SYL }))
        return FUNCTOR_CONTINUE;

    Zone *zone = this->GetZone(layerElement, layerElement->GetClassName());
    zone->SetUlx(m_view.ToDeviceContextX(layerElement->GetDrawingX()) / DEFINITION_FACTOR + m_pageMarginLeft);
    if (m_currentNeumeLine && layerElement->Is({ ACCID, SYL })) {
        zone->SetUly(m_view.ToDeviceContextY(layerElement->GetDrawingY()) / DEFINITION_FACTOR + m_pageMarginTop);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncToFacsimileFunctor::VisitMeasure(Measure *measure)
{
    Zone *zone = this->GetZone(measure, measure->GetClassName());
    zone->SetUlx(m_view.ToDeviceContextX(measure->GetDrawingX()) / DEFINITION_FACTOR + m_pageMarginLeft);
    zone->SetLrx(
        m_view.ToDeviceContextX(measure->GetDrawingX() + measure->GetWidth()) / DEFINITION_FACTOR + m_pageMarginLeft);

    m_currentNeumeLine = measure->IsNeumeLine();

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncToFacsimileFunctor::VisitPage(Page *page)
{
    // This is required since processing Pb will create or select the Surface
    if (!page->FindDescendantByType(PB)) {
        LogWarning("Page without <pb> skipped when synching to facsimile");
        return FUNCTOR_SIBLINGS;
    }

    m_currentPage = page;
    m_doc->SetDrawingPage(page->GetIdx());
    page->LayOut();
    // From a previously loaded facsimile
    if (page->GetPPUFactor() != 1.0) {
        m_ppuFactor = page->GetPPUFactor();
    }
    // When generating a facsimile with a scale option
    else {
        page->SetPPUFactor(m_ppuFactor);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncToFacsimileFunctor::VisitPageEnd(Page *page)
{
    if (m_ppuFactor != 1.0) {
        ApplyPPUFactorFunctor applyPPUFactor(m_currentPage);
        m_surface->Process(applyPPUFactor);
        m_surface->SetType(
            StringFormat("ppu:%f", m_ppuFactor * m_doc->GetOptions()->m_unit.GetValue() / DEFINITION_FACTOR));
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode SyncToFacsimileFunctor::VisitPb(Pb *pb)
{
    Zone *zone = pb->GetZone();
    // Assume to be creating the facsimile data from scratch
    if (!pb->GetZone()) {
        // Also create a new surface
        m_surface = new Surface();
        assert(m_doc->GetFacsimile());
        m_doc->GetFacsimile()->AddChild(m_surface);
        zone = this->GetZone(pb, pb->GetClassName());
    }
    // We already have some facsimile data - retrieve the surface ancestor
    else {
        m_surface = vrv_cast<Surface *>(zone->GetFirstAncestor(SURFACE));
    }
    assert(m_surface);
    assert(zone);

    m_surface->SetLrx(m_doc->m_drawingPageWidth / DEFINITION_FACTOR);
    m_surface->SetLry(m_doc->m_drawingPageHeight / DEFINITION_FACTOR);
    // Because the facsimile output zone positions include the margins, we will add them to each zone
    m_pageMarginTop = m_doc->m_drawingPageMarginTop / DEFINITION_FACTOR;
    m_pageMarginLeft = m_doc->m_drawingPageMarginLeft / DEFINITION_FACTOR;

    // The Pb zone values are currently not used in SyncFromFacsimileFunctor because the
    // page sizes are synced from the parent Surface and zone positions include margins
    zone->SetUlx(m_pageMarginLeft);
    zone->SetUly(m_pageMarginTop);
    // Use the page content size to factor in the bottom and right margins
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
    assert(m_surface);

    if (interface->GetZone()) {
        // Here we should probably check if the zone is a child of m_surface
        assert(interface->GetZone()->GetParent() == m_surface);
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

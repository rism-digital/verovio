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
    if (!layerElement->Is({NOTE, REST})) return FUNCTOR_CONTINUE;
    
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
    /*
    assert(zone);
    m_currentSystem->m_xAbs = zone->GetUlx() * DEFINITION_FACTOR;
    m_currentSystem->m_yAbs = zone->GetUly() * DEFINITION_FACTOR;
    */
    
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


} // namespace vrv

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

#include "staff.h"
#include "vrv.h"
#include "zone.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// SyncFromFacsimileFunctor
//----------------------------------------------------------------------------

SyncFromFacsimileFunctor::SyncFromFacsimileFunctor() : Functor()
{
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

FunctorCode SyncFromFacsimileFunctor::VisitStaff(Staff *staff)
{
    Zone *zone = staff->GetZone();
    assert(zone);
    staff->m_yAbs = zone->GetUly() * DEFINITION_FACTOR;
    
    return FUNCTOR_CONTINUE;
}

} // namespace vrv

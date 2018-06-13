/////////////////////////////////////////////////////////////////////////////
// Name:        facsimileinterface.cpp
// Author:      Juliette Regimbal 
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "facsimileinterface.h"

//---------------------------------------------------------------------------

#include <assert.h>

//---------------------------------------------------------------------------

#include "facsimile.h"
#include "view.h"
#include "vrv.h"

namespace vrv {

FacsimileInterface::FacsimileInterface() : Interface(), AttFacsimile()
{
    RegisterInterfaceAttClass(ATT_FACSIMILE);
    Reset();
}

FacsimileInterface::~FacsimileInterface() {}

void FacsimileInterface::Reset()
{
    ResetFacsimile();
    this->SetZone(nullptr);
}

int FacsimileInterface::GetDrawingX() const
{
    assert(m_zone);
    return m_zone->GetUlx() * m_zone->m_facsScale;
}

int FacsimileInterface::GetDrawingY() const
{
    assert(m_zone);
    int y = (GetSurfaceY() - m_zone->GetLogicalUly()) * m_zone->m_facsScale;
    return y;
}

int FacsimileInterface::GetWidth() const
{
    assert(m_zone);
    return m_zone->m_facsScale * (m_zone->GetLrx() - m_zone->GetUlx());
}

int FacsimileInterface::GetSurfaceY() const
{
    assert(m_zone);
    Surface *surface = dynamic_cast<Surface *>(m_zone->GetFirstParent(SURFACE));
    assert(surface);
    if (surface->HasLry()) {
        return surface->GetLry();
    }
    else {
        int max = 0;
        AttComparison ac(ZONE);
        ArrayOfObjects zones;
        surface->FindAllChildByComparison(&zones, &ac);
        for (auto iter = zones.begin(); iter != zones.end(); iter++) {
            Zone *zone = dynamic_cast<Zone *>(*iter);
            max = (zone->GetLry() > max) ? zone->GetLry() : max;
        }
        return max; 
    }
}
}

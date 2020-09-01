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

#include "doc.h"
#include "facsimile.h"
#include "functorparams.h"
#include "surface.h"
#include "syllable.h"
#include "view.h"
#include "vrv.h"
#include "zone.h"

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
    this->SetZone(NULL);
}

int FacsimileInterface::GetDrawingX() const
{
    assert(m_zone);
    return m_zone->GetUlx();
}

int FacsimileInterface::GetDrawingY() const
{
    assert(m_zone);
    int y;
    if (m_zone->GetRotate() >= 0) {
        y = m_zone->GetLogicalUly();
    }
    else {
        y = m_zone->GetLogicalUly() - (m_zone->GetLrx() - m_zone->GetUlx()) * tan(m_zone->GetRotate() * M_PI / 180.0);
    }
    return y;
}

int FacsimileInterface::GetWidth() const
{
    assert(m_zone);
    return m_zone->GetLrx() - m_zone->GetUlx();
}

int FacsimileInterface::GetHeight() const
{
    assert(m_zone);
    return m_zone->GetLogicalLry() - m_zone->GetLogicalUly();
}

double FacsimileInterface::GetDrawingRotate() const
{
    assert(m_zone);
    return m_zone->GetRotate();
}

int FacsimileInterface::GetSurfaceY() const
{
    assert(m_zone);
    Surface *surface = vrv_cast<Surface *>(m_zone->GetFirstAncestor(SURFACE));
    assert(surface);
    if (surface->HasLry()) {
        return surface->GetLry();
    }
    else {
        return surface->GetMaxY();
    }
}

void FacsimileInterface::SetZone(Zone *zone)
{
    if (m_zone != NULL) {
        Object *parent = m_zone->GetParent();
        if (!parent->DeleteChild(m_zone)) {
            printf("Failed to delete zone with ID %s\n", m_zone->GetUuid().c_str());
        }
    }
    m_zone = zone;
    if (m_zone == NULL) {
        this->SetFacs("");
    }
    else {
        this->SetFacs("#" + m_zone->GetUuid());
    }
}

} // namespace vrv

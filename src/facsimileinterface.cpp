/////////////////////////////////////////////////////////////////////////////
// Name:        facsimileinterface.cpp
// Author:      Juliette Regimbal
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "facsimileinterface.h"

//---------------------------------------------------------------------------

#include <cassert>

//---------------------------------------------------------------------------

#include "doc.h"
#include "facsimile.h"
#include "preparedatafunctor.h"
#include "surface.h"
#include "syllable.h"
#include "view.h"
#include "vrv.h"
#include "zone.h"

namespace vrv {

FacsimileInterface::FacsimileInterface() : Interface(), AttFacsimile()
{
    this->RegisterInterfaceAttClass(ATT_FACSIMILE);
    this->Reset();
}

FacsimileInterface::~FacsimileInterface() {}

void FacsimileInterface::Reset()
{
    this->ResetFacsimile();

    m_zone = NULL;
    m_surface = NULL;
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
    return surface->GetMaxY();
}

void FacsimileInterface::AttachZone(Zone *zone)
{
    if (m_zone != NULL) {
        Object *parent = m_zone->GetParent();
        if (!parent->DeleteChild(m_zone)) {
            printf("Failed to delete zone with ID %s\n", m_zone->GetID().c_str());
        }
    }
    m_zone = zone;
    if (m_zone == NULL) {
        this->SetFacs("");
    }
    else {
        this->SetFacs("#" + m_zone->GetID());
    }
}

//----------------------------------------------------------------------------
// Interface pseudo functor (redirected)
//----------------------------------------------------------------------------

FunctorCode FacsimileInterface::InterfacePrepareFacsimile(PrepareFacsimileFunctor &functor, Object *object)
{
    assert(functor.GetFacsimile());
    Facsimile *facsimile = functor.GetFacsimile();
    std::string facsID = ExtractIDFragment(this->GetFacs());
    Object *facsDescendant = facsimile->FindDescendantByID(facsID);
    if (!facsDescendant) {
        LogWarning("Could not find @facs '%s' in facsimile element", facsID.c_str());
        return FUNCTOR_CONTINUE;
    }

    if (facsDescendant->Is(ZONE)) {
        m_zone = vrv_cast<Zone *>(facsDescendant);
        assert(m_zone);
    }
    else if (facsDescendant->Is(SURFACE)) {
        m_surface = vrv_cast<Surface *>(facsDescendant);
        assert(m_surface);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode FacsimileInterface::InterfaceResetData(ResetDataFunctor &functor, Object *object)
{
    m_zone = NULL;
    m_surface = NULL;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

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
    // this method should only be called in staff->GetDrawingRotate()
    // since this method cannot validate the m_useRotate option
    // while the staff->GetDrawingRotate() method can

    assert(m_zone);
    return m_zone->GetRotate();
}

int FacsimileInterface::GetSurfaceY() const
{
    assert(m_zone);
    Surface *surface = dynamic_cast<Surface *>(m_zone->GetFirstAncestor(SURFACE));
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

int FacsimileInterface::InterfaceSetChildZones(FunctorParams *functorParams, Object *object)
{
    SetChildZonesParams *params = dynamic_cast<SetChildZonesParams *>(functorParams);
    assert(params);

    if (this->HasFacs() && (this->GetZone() == NULL)) {
        assert(params->m_doc);
        assert(params->m_doc->GetFacsimile());
        // Facs should be a URI so check for # fragment identifier and strip it
        std::string facsUuid = this->GetFacs()[0] == '#' ? this->GetFacs().substr(1) : this->GetFacs();
        Zone *zone = params->m_doc->GetFacsimile()->FindZoneByUuid(facsUuid);
        if (zone == NULL) {
            LogError("Could not find a zone of UUID %s", facsUuid.c_str());
            return FUNCTOR_STOP;
        }
        this->SetZone(zone);
    }
    else if (!this->HasFacs() && object->Is(SYL) && params->m_doc->GetOptions()->m_createDefaultSylBBox.GetValue()) {
        Zone *zone = new Zone();
        // if the syl's syllable parent has facs then use that as the bounding box
        FacsimileInterface *syllableFi = NULL;
        const int offsetUly = 100;
        const int offsetLrx = 100;
        const int offsetLry = 200;
        Syllable *syllable = dynamic_cast<Syllable *>(object->GetFirstAncestor(SYLLABLE));
        assert(syllable);
        if (syllable->GetFacsimileInterface()->HasFacs()) {
            syllableFi = syllable->GetFacsimileInterface();
            Zone *tempZone = dynamic_cast<Zone *>(syllableFi->GetZone());
            assert(tempZone);
            zone->SetUlx(tempZone->GetUlx());
            zone->SetUly(tempZone->GetUly() + offsetUly);
            zone->SetLrx(tempZone->GetLrx() + offsetLrx);
            zone->SetLry(tempZone->GetLry() + offsetLry);
            Surface *surface = dynamic_cast<Surface *>(params->m_doc->GetFacsimile()->FindDescendantByType(SURFACE));
            assert(surface);
            surface->AddChild(zone);
            this->SetZone(zone);
        }
        // otherwise get bounds that comprises all neumes in the syllable
        else {
            Syllable *parentSyllable = dynamic_cast<Syllable *>(object->GetFirstAncestor(SYLLABLE));
            assert(parentSyllable);
            int ulx, uly, lrx, lry;
            if (parentSyllable->GenerateZoneBounds(&ulx, &uly, &lrx, &lry)) {
                if (ulx == 0 || uly == 0 || lrx == 0 || lry == 0) {
                    LogWarning("Zero value when generating bbox from %s: (%d, %d, %d, %d)",
                        parentSyllable->GetUuid().c_str(), ulx, uly, lrx, lry);
                }

                Zone *zone = new Zone();
                zone->SetUlx(ulx);
                zone->SetUly(uly + offsetUly);
                zone->SetLrx(lrx + offsetLrx);
                zone->SetLry(lry + offsetLry);
                Surface *surface = dynamic_cast<Surface *>(params->m_doc->GetFacsimile()->FindDescendantByType(SURFACE));
                assert(surface);
                surface->AddChild(zone);
                this->SetZone(zone);
            }
            else {
                LogWarning("Failed to create zone for %s of type %s", object->GetUuid().c_str(),
                    object->GetClassName().c_str());
            }
        }
    }
    return FUNCTOR_CONTINUE;
}
} // namespace vrv

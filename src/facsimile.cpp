/////////////////////////////////////////////////////////////////////////////
// Name:        facsimile.cpp
// Author:      Juliette Regimbal
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "facsimile.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "surface.h"
#include "vrv.h"
#include "zone.h"

namespace vrv {

//----------------------------------------------------------------------------
// Facsimile
//----------------------------------------------------------------------------

static const ClassRegistrar<Facsimile> s_factory("facsimile", FACSIMILE);

Facsimile::Facsimile() : Object(FACSIMILE, "facsimile-"), AttTyped() {}

Facsimile::~Facsimile() {}

void Facsimile::Reset() {}

bool Facsimile::IsSupportedChild(Object *object)
{
    if (object->Is(SURFACE)) {
        assert(dynamic_cast<Surface *>(object));
    }
    else {
        LogError("Unsupported child '%s' of facsimile", object->GetClassName().c_str());
        return false;
    }
    return true;
}

Zone *Facsimile::FindZoneByID(const std::string &zoneId)
{
    return dynamic_cast<Zone *>(this->FindDescendantByID(zoneId));
}

const Zone *Facsimile::FindZoneByID(const std::string &zoneId) const
{
    return dynamic_cast<const Zone *>(this->FindDescendantByID(zoneId));
}

int Facsimile::GetMaxX() const
{
    ListOfConstObjects surfaces = this->FindAllDescendantsByType(SURFACE);

    int max = 0;
    for (auto iter = surfaces.begin(); iter != surfaces.end(); ++iter) {
        const Surface *surface = vrv_cast<const Surface *>(*iter);
        assert(surface);
        max = (surface->GetMaxX() > max) ? surface->GetMaxX() : max;
    }
    return max;
}

int Facsimile::GetMaxY() const
{
    ListOfConstObjects surfaces = this->FindAllDescendantsByType(SURFACE);

    int max = 0;
    for (auto iter = surfaces.begin(); iter != surfaces.end(); ++iter) {
        const Surface *surface = vrv_cast<const Surface *>(*iter);
        assert(surface);
        max = (surface->GetMaxY() > max) ? surface->GetMaxY() : max;
    }
    return max;
}

} // namespace vrv

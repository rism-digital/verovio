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
#include "functor.h"
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
    for (const Object *object : surfaces) {
        const Surface *surface = vrv_cast<const Surface *>(object);
        assert(surface);
        max = (surface->GetMaxX() > max) ? surface->GetMaxX() : max;
    }
    return max;
}

int Facsimile::GetMaxY() const
{
    ListOfConstObjects surfaces = this->FindAllDescendantsByType(SURFACE);

    int max = 0;
    for (const Object *object : surfaces) {
        const Surface *surface = vrv_cast<const Surface *>(object);
        assert(surface);
        max = (surface->GetMaxY() > max) ? surface->GetMaxY() : max;
    }
    return max;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

FunctorCode Facsimile::Accept(Functor &functor)
{
    return functor.VisitFacsimile(this);
}

FunctorCode Facsimile::Accept(ConstFunctor &functor) const
{
    return functor.VisitFacsimile(this);
}

FunctorCode Facsimile::AcceptEnd(Functor &functor)
{
    return functor.VisitFacsimileEnd(this);
}

FunctorCode Facsimile::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitFacsimileEnd(this);
}

} // namespace vrv

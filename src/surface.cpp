/////////////////////////////////////////////////////////////////////////////
// Name:        surface.cpp
// Author:      Juliette Regimbal
// Created:     05/06/2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "surface.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "facsimile.h"
#include "functor.h"
#include "graphic.h"
#include "vrv.h"
#include "zone.h"

namespace vrv {

//----------------------------------------------------------------------------
// Surface
//----------------------------------------------------------------------------

static const ClassRegistrar<Surface> s_factory("surface", SURFACE);

Surface::Surface() : Object(SURFACE), AttTyped(), AttCoordinated()
{
    this->RegisterAttClass(ATT_TYPED);
    this->RegisterAttClass(ATT_COORDINATED);
    this->RegisterAttClass(ATT_COORDINATEDUL);
    this->Reset();
}

Surface::~Surface() {}

void Surface::Reset()
{
    this->ResetTyped();
    this->ResetCoordinated();
    this->ResetCoordinatedUl();
}

bool Surface::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ GRAPHIC, ZONE };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else {
        return false;
    }
}

int Surface::GetMaxX() const
{
    if (this->HasLrx()) return this->GetLrx();
    int max = 0;
    ListOfConstObjects zones = this->FindAllDescendantsByType(ZONE);
    for (const Object *object : zones) {
        const Zone *zone = vrv_cast<const Zone *>(object);
        assert(zone);
        max = (zone->GetLrx() > max) ? zone->GetLrx() : max;
    }
    return max;
}

int Surface::GetMaxY() const
{
    if (this->HasLry()) return this->GetLry();
    int max = 0;
    ListOfConstObjects zones = this->FindAllDescendantsByType(ZONE);
    for (const Object *object : zones) {
        const Zone *zone = vrv_cast<const Zone *>(object);
        assert(zone);
        max = (zone->GetLry() > max) ? zone->GetLry() : max;
    }
    return max;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

FunctorCode Surface::Accept(Functor &functor)
{
    return functor.VisitSurface(this);
}

FunctorCode Surface::Accept(ConstFunctor &functor) const
{
    return functor.VisitSurface(this);
}

FunctorCode Surface::AcceptEnd(Functor &functor)
{
    return functor.VisitSurfaceEnd(this);
}

FunctorCode Surface::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitSurfaceEnd(this);
}

} // namespace vrv

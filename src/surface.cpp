/////////////////////////////////////////////////////////////////////////////
// Name:        surface.cpp
// Author:      Juliette Regimbal
// Created:     05/06/2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "surface.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "facsimile.h"
#include "object.h"
#include "vrv.h"
#include "zone.h"

namespace vrv {

//----------------------------------------------------------------------------
// Surface
//----------------------------------------------------------------------------

static const ClassRegistrar<Surface> s_factory("surface", SURFACE);

Surface::Surface() : Object(SURFACE, "surface-"), AttTyped(), AttCoordinated()
{
    this->RegisterAttClass(ATT_TYPED);
    this->RegisterAttClass(ATT_COORDINATED);
    this->Reset();
}

Surface::~Surface() {}

void Surface::Reset()
{
    this->ResetTyped();
    this->ResetCoordinated();
}

bool Surface::IsSupportedChild(Object *object)
{
    // TODO Add support for graphic tag
    if (object->Is(ZONE)) {
        assert(dynamic_cast<Zone *>(object));
    }
    else {
        LogError("Unsupported child '%s' of surface", object->GetClassName().c_str());
        return false;
    }
    return true;
}

int Surface::GetMaxX() const
{
    if (this->HasLrx()) return this->GetLrx();
    int max = 0;
    ListOfConstObjects zones = this->FindAllDescendantsByType(ZONE);
    for (auto iter = zones.begin(); iter != zones.end(); ++iter) {
        const Zone *zone = vrv_cast<const Zone *>(*iter);
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
    for (auto iter = zones.begin(); iter != zones.end(); ++iter) {
        const Zone *zone = vrv_cast<const Zone *>(*iter);
        assert(zone);
        max = (zone->GetLry() > max) ? zone->GetLry() : max;
    }
    return max;
}

} // namespace vrv

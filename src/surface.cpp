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
Surface::Surface() : Object("surface-"), AttTyped(), AttCoordinated()
{
    RegisterAttClass(ATT_TYPED);
    RegisterAttClass(ATT_COORDINATED);
    Reset();
}
Surface::~Surface() {}
void Surface::Reset()
{
    ResetTyped();
    ResetCoordinated();
}

void Surface::AddChild(Object *object)
{
    // TODO Add support for graphic tag
    if (object->Is(ZONE)) {
        object->SetParent(this);
        this->m_children.push_back(object);
        Modify();
    }
    else if (object->GetClassName() == "graphic") {
        LogWarning("The graphic element is currently not supported.");
    }
    else {
        LogError("Unsupported child '%s' of surface", object->GetClassName().c_str());
        assert(false);
    }
}

int Surface::GetMaxX()
{
    if (HasLrx()) return GetLrx();
    int max = 0;
    ClassIdComparison ac(ZONE);
    ArrayOfObjects zones;
    FindAllDescendantByComparison(&zones, &ac);
    for (auto iter = zones.begin(); iter != zones.end(); iter++) {
        Zone *zone = dynamic_cast<Zone *>(*iter);
        assert(zone);
        max = (zone->GetLrx() > max) ? zone->GetLrx() : max;
    }
    return max;
}

int Surface::GetMaxY()
{
    if (HasLry()) return GetLry();
    int max = 0;
    ClassIdComparison ac(ZONE);
    ArrayOfObjects zones;
    FindAllDescendantByComparison(&zones, &ac);
    for (auto iter = zones.begin(); iter != zones.end(); iter++) {
        Zone *zone = dynamic_cast<Zone *>(*iter);
        assert(zone);
        max = (zone->GetLry() > max) ? zone->GetLry() : max;
    }
    return max;
}

} // namespace vrv

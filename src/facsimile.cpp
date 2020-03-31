/////////////////////////////////////////////////////////////////////////////
// Name:        facsimile.cpp
// Author:      Juliette Regimbal
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "facsimile.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "object.h"
#include "surface.h"
#include "vrv.h"
#include "zone.h"

namespace vrv {

//----------------------------------------------------------------------------
// Facsimile
//----------------------------------------------------------------------------
Facsimile::Facsimile() : Object("facsimile-") {}
Facsimile::~Facsimile() {}
void Facsimile::Reset() {}

void Facsimile::AddChild(Object *object)
{
    if (object->Is(SURFACE)) {
        object->SetParent(this);
        this->m_children.push_back(object);
        Modify();
    }
    else {
        LogError("Unsupported child '%s' of facsimile", object->GetClassName().c_str());
        assert(false);
    }
}

Zone *Facsimile::FindZoneByUuid(std::string zoneId)
{
    return dynamic_cast<Zone *>(this->FindDescendantByUuid(zoneId));
}

int Facsimile::GetMaxX()
{
    ClassIdComparison ac(SURFACE);
    ArrayOfObjects surfaces;
    this->FindAllDescendantByComparison(&surfaces, &ac);

    int max = 0;
    for (auto iter = surfaces.begin(); iter != surfaces.end(); iter++) {
        Surface *surface = dynamic_cast<Surface *>(*iter);
        assert(surface);
        max = (surface->GetMaxX() > max) ? surface->GetMaxX() : max;
    }
    return max;
}

int Facsimile::GetMaxY()
{
    ClassIdComparison ac(SURFACE);
    ArrayOfObjects surfaces;
    this->FindAllDescendantByComparison(&surfaces, &ac);

    int max = 0;
    for (auto iter = surfaces.begin(); iter != surfaces.end(); iter++) {
        Surface *surface = dynamic_cast<Surface *>(*iter);
        assert(surface);
        max = (surface->GetMaxY() > max) ? surface->GetMaxY() : max;
    }
    return max;
}

} // namespace vrv

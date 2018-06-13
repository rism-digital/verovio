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

#include "attcomparison.h"
#include "object.h"
#include "vrv.h"

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
    return dynamic_cast<Zone *>(this->FindChildByUuid(zoneId));
}

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
    //TODO Add support for graphic tag
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

//----------------------------------------------------------------------------
// Zone 
//----------------------------------------------------------------------------
Zone::Zone() : Object("zone-"), AttTyped(), AttCoordinated() 
{
    RegisterAttClass(ATT_TYPED);
    RegisterAttClass(ATT_COORDINATED);
    Reset();
}
Zone::~Zone() {}
void Zone::Reset()
{
    ResetTyped();
    ResetCoordinated();
}

int Zone::GetLogicalUly()
{
    Surface *surface = dynamic_cast<Surface *>(this->GetFirstParent(SURFACE));
    assert(surface);
    return abs(surface->GetLry() - this->GetUly());
}

int Zone::GetLogicalLry()
{
    Surface *surface = dynamic_cast<Surface *>(this->GetFirstParent(SURFACE));
    assert(surface);
    return abs(surface->GetLry() - this->GetLry());
}
}

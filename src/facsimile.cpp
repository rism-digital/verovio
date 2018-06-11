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
        this->m_children.push_back(object);
    }
    else {
        LogError("Unsupported child '%s' of facsimile", object->GetClassName().c_str());
    }
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
        this->m_children.push_back(object);
    }
    else {
        LogError("Unsupported child '%s' of surface", object->GetClassName().c_str());
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
}

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
void Reset() {}
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

//----------------------------------------------------------------------------
// Zone 
//----------------------------------------------------------------------------
Zone::Zone() : Object(), AttTyped(), AttCoordinated() 
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

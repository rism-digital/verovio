/////////////////////////////////////////////////////////////////////////////
// Name:        restore.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "restore.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Restore
//----------------------------------------------------------------------------

Restore::Restore() : EditorialElement("restore-"), AttSource()
{
    RegisterAttClass(ATT_SOURCE);

    Reset();
}

Restore::~Restore() {}

void Restore::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// functor methods
//----------------------------------------------------------------------------

} // namespace vrv

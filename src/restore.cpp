/////////////////////////////////////////////////////////////////////////////
// Name:        restore.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "restore.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Restore
//----------------------------------------------------------------------------

static const ClassRegistrar<Restore> s_factory("restore", RESTORE);

Restore::Restore() : EditorialElement(RESTORE), AttSource()
{
    this->RegisterAttClass(ATT_SOURCE);

    this->Reset();
}

Restore::~Restore() {}

void Restore::Reset()
{
    EditorialElement::Reset();
    this->ResetSource();
}

//----------------------------------------------------------------------------
// functor methods
//----------------------------------------------------------------------------

} // namespace vrv

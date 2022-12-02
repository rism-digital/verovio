/////////////////////////////////////////////////////////////////////////////
// Name:        symboldef.cpp
// Author:      Laurent Pugin
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "symboldef.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// SymbolDef
//----------------------------------------------------------------------------

static const ClassRegistrar<SymbolDef> s_factory("symbolDef", SYMBOLDEF);

SymbolDef::SymbolDef() : Object(SYMBOLDEF, "symdef-")
{
    this->Reset();
}

SymbolDef::~SymbolDef() {}

void SymbolDef::Reset()
{
    Object::Reset();
}

} // namespace vrv

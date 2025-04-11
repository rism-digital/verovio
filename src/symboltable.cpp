/////////////////////////////////////////////////////////////////////////////
// Name:        symboltable.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "symboltable.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "symboldef.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// SymbolTable
//----------------------------------------------------------------------------

static const ClassRegistrar<SymbolTable> s_factory("symbolTable", SYMBOLTABLE);

SymbolTable::SymbolTable() : Object(SYMBOLTABLE)
{
    this->Reset();
}

SymbolTable::~SymbolTable() {}

void SymbolTable::Reset()
{
    Object::Reset();
}

bool SymbolTable::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ SYMBOLDEF };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else {
        return false;
    }
}

} // namespace vrv

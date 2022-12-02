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

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// SymbolTable
//----------------------------------------------------------------------------

static const ClassRegistrar<SymbolTable> s_factory("symbolTable", SYMBOLTABLE);

SymbolTable::SymbolTable() : Object(SYMBOLTABLE, "symtable-")
{
    this->Reset();
}

SymbolTable::~SymbolTable() {}

void SymbolTable::Reset()
{
    Object::Reset();
}

} // namespace vrv

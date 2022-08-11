/////////////////////////////////////////////////////////////////////////////
// Name:        symbol.cpp
// Author:      Laurent Pugin
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "symbol.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Symbol
//----------------------------------------------------------------------------

static const ClassRegistrar<Symbol> s_factory("symbol", SYMBOL);

Symbol::Symbol() : Object(SYMBOL, "symbol-")
{
    this->Reset();
}

Symbol::~Symbol() {}

void Symbol::Reset()
{
    Object::Reset();

    m_visibility = Visible;
}

bool Symbol::IsSupportedChild(Object *child)
{
    return false;
}

} // namespace vrv

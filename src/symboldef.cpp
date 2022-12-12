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

#include "svg.h"
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

    m_originalParent = NULL;
}

bool SymbolDef::IsSupportedChild(Object *child)
{
    if (child->Is(SVG)) {
        assert(dynamic_cast<Svg *>(child));
    }
    else {
        return false;
    }
    return true;
}

void SymbolDef::SetTemporaryParent(Object *parent)
{
    assert(GetParent() && !m_originalParent);
    m_originalParent = this->GetParent();
    this->ResetParent();
    this->SetParent(parent);
}

void SymbolDef::ResetTemporaryParent()
{
    assert(GetParent() && m_originalParent);
    this->ResetParent();
    this->SetParent(m_originalParent);
    m_originalParent = NULL;
}

} // namespace vrv

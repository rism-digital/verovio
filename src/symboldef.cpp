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

#include "doc.h"
#include "graphic.h"
#include "svg.h"
#include "symbol.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// SymbolDef
//----------------------------------------------------------------------------

static const ClassRegistrar<SymbolDef> s_factory("symbolDef", SYMBOLDEF);

SymbolDef::SymbolDef() : Object(SYMBOLDEF)
{
    this->Reset();
}

SymbolDef::~SymbolDef() {}

void SymbolDef::Reset()
{
    Object::Reset();

    m_originalParent = NULL;
}

bool SymbolDef::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ GRAPHIC, SVG, SYMBOL };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else {
        return false;
    }
}

int SymbolDef::GetSymbolWidth(Doc *doc, int staffSize, bool dimin) const
{
    return this->GetSymbolSize(doc, staffSize, dimin).first;
}

int SymbolDef::GetSymbolHeight(Doc *doc, int staffSize, bool dimin) const
{
    return this->GetSymbolSize(doc, staffSize, dimin).second;
}

std::pair<int, int> SymbolDef::GetSymbolSize(Doc *doc, int staffSize, bool dimin) const
{
    assert(doc);

    const int unit = doc->GetDrawingUnit(staffSize);

    int height = 0;
    int width = 0;

    for (const Object *child : this->GetChildren()) {
        if (child->Is(SVG)) {
            const Svg *svg = vrv_cast<const Svg *>(child);
            assert(svg);
            height = std::max(height, svg->GetHeight() * staffSize / 100);
            width = std::max(width, svg->GetWidth() * staffSize / 100);
        }
        else if (child->Is(GRAPHIC)) {
            const Graphic *graphic = vrv_cast<const Graphic *>(child);
            assert(graphic);
            height = std::max(height, graphic->GetDrawingHeight(unit, staffSize));
            width = std::max(width, graphic->GetDrawingWidth(unit, staffSize));
        }
    }

    if (dimin) {
        height = height * doc->GetOptions()->m_graceFactor.GetValue();
        width = width * doc->GetOptions()->m_graceFactor.GetValue();
    }

    return { width, height };
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

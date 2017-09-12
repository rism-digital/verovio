/////////////////////////////////////////////////////////////////////////////
// Name:        neume.cpp
// Author:      Andrew Tran
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "neume.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "artic.h"
#include "attcomparison.h"
#include "doc.h"
#include "editorial.h"
#include "elementpart.h"
#include "functorparams.h"
#include "glyph.h"
#include "layer.h"
#include "slur.h"
#include "smufl.h"
#include "staff.h"
#include "syl.h"
#include "tie.h"
#include "verse.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Note
//----------------------------------------------------------------------------

Neume::Neume()
    : LayerElement("neume-"),
      ObjectListInterface(),
      AttColor()
{
    RegisterAttClass(ATT_COLOR);

    Reset();
}

Neume::~Neume()
{

}

void Neume::AddChild(Object *child)
{
    if (child->Is(NC)) {
        assert(dynamic_cast<Nc *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

void Neume::Reset()
{
    LayerElement::Reset();
    ResetColor();
}

} // namespace vrv

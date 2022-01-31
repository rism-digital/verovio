/////////////////////////////////////////////////////////////////////////////
// Name:        tabdursym.cpp
// Author:      Laurent Pugin
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "tabdursym.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "elementpart.h"
#include "functorparams.h"
#include "layer.h"
#include "staff.h"

namespace vrv {

//----------------------------------------------------------------------------
// TabDurSym
//----------------------------------------------------------------------------

static const ClassRegistrar<TabDurSym> s_factory("tabDurSym", TABDURSYM);

TabDurSym::TabDurSym() : LayerElement(TABDURSYM, "tabdursym-"), StemmedDrawingInterface(), AttNNumberLike()
{
    this->RegisterAttClass(ATT_NNUMBERLIKE);

    this->Reset();
}

TabDurSym::~TabDurSym() {}

void TabDurSym::Reset()
{
    LayerElement::Reset();
    StemmedDrawingInterface::Reset();
    this->ResetNNumberLike();
}

bool TabDurSym::IsSupportedChild(Object *child)
{
    if (child->Is(STEM)) {
        assert(dynamic_cast<Stem *>(child));
    }
    else {
        return false;
    }
    return true;
}

void TabDurSym::AddChild(Object *child)
{
    if (!this->IsSupportedChild(child)) {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        return;
    }

    child->SetParent(this);

    ArrayOfObjects *children = this->GetChildrenForModification();

    // Stem are always added by PrepareLayerElementParts (for now) and we want them to be in the front
    // for the drawing order in the SVG output
    if (child->Is(STEM)) {
        children->insert(children->begin(), child);
    }
    else {
        children->push_back(child);
    }
    Modify();
}

Point TabDurSym::GetStemUpSE(Doc *doc, int staffSize, bool isCueSize)
{
    Point p(0, 0);
    return p;
}

Point TabDurSym::GetStemDownNW(Doc *doc, int staffSize, bool isCueSize)
{
    Point p(0, 0);
    return p;
}

int TabDurSym::CalcStemLenInThirdUnits(Staff *staff, data_STEMDIRECTION stemDir)
{
    assert(staff);

    if ((stemDir != STEMDIRECTION_down) && (stemDir != STEMDIRECTION_up)) {
        return 0;
    }

    int baseStem = STANDARD_STEMLENGTH * 3;

    return baseStem;
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int TabDurSym::PrepareLayerElementParts(FunctorParams *functorParams)
{
}

int TabDurSym::CalcStem(FunctorParams *functorParams)
{
}

} // namespace vrv

/////////////////////////////////////////////////////////////////////////////
// Name:        tabdursym.cpp
// Author:      Laurent Pugin
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "tabdursym.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "doc.h"
#include "elementpart.h"
#include "functor.h"
#include "layer.h"
#include "note.h"
#include "staff.h"
#include "stem.h"
#include "tabgrp.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// TabDurSym
//----------------------------------------------------------------------------

static const ClassRegistrar<TabDurSym> s_factory("tabDurSym", TABDURSYM);

TabDurSym::TabDurSym()
    : LayerElement(TABDURSYM), StemmedDrawingInterface(), AttNNumberLike(), AttStringtab(), AttVisualOffsetVo()
{
    this->RegisterAttClass(ATT_NNUMBERLIKE);
    this->RegisterAttClass(ATT_STRINGTAB);
    this->RegisterAttClass(ATT_VISUALOFFSETVO);

    this->Reset();
}

TabDurSym::~TabDurSym() {}

void TabDurSym::Reset()
{
    LayerElement::Reset();
    StemmedDrawingInterface::Reset();
    this->ResetNNumberLike();
    this->ResetStringtab();
    this->ResetVisualOffsetVo();
}

bool TabDurSym::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ STEM };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else {
        return false;
    }
}

void TabDurSym::AddChild(Object *child)
{
    if (!this->IsSupportedChild(child->GetClassId()) || !this->AddChildAdditionalCheck(child)) {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        return;
    }

    child->SetParent(this);

    ArrayOfObjects &children = this->GetChildrenForModification();

    // Stem are always added by PrepareLayerElementParts (for now) and we want them to be in the front
    // for the drawing order in the SVG output
    if (child->Is(STEM)) {
        children.insert(children.begin(), child);
    }
    else {
        children.push_back(child);
    }
    this->Modify();
}

void TabDurSym::AdjustDrawingYRel(const Staff *staff, const Doc *doc)
{
    assert(staff);
    assert(doc);

    int yRel = (staff->m_drawingLines - 1) * doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);

    // For stems outside add a margin to the tabDurSym - otherwise attached to the staff line
    if (staff->IsTabWithStemsOutside()) {
        double spacingRatio = (staff->IsTabLuteFrench() || staff->IsTabLuteGerman()) ? 2.0 : 1.0;
        yRel += doc->GetDrawingUnit(staff->m_drawingStaffSize) * spacingRatio;
    }

    this->SetDrawingYRel(-yRel);
}

Point TabDurSym::GetStemUpSE(const Doc *doc, int staffSize, bool isCueSize) const
{
    return Point();
}

Point TabDurSym::GetStemDownNW(const Doc *doc, int staffSize, bool isCueSize) const
{
    return Point();
}

int TabDurSym::CalcStemLenInThirdUnits(const Staff *staff, data_STEMDIRECTION stemDir) const
{
    assert(staff);

    if ((stemDir != STEMDIRECTION_down) && (stemDir != STEMDIRECTION_up)) {
        return 0;
    }

    int baseStem = STANDARD_STEMLENGTH_TAB * 3;

    // Shorter for german lute tablature to match ryhthm glyphs
    if (staff->IsTabLuteGerman()) baseStem -= 3;
    // One unit longer for guitar tablature
    else if (staff->IsTabGuitar())
        baseStem += 3;

    // One unit longer for stems inside the staff
    if (!staff->IsTabWithStemsOutside()) baseStem += 3;

    return baseStem;
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

FunctorCode TabDurSym::Accept(Functor &functor)
{
    return functor.VisitTabDurSym(this);
}

FunctorCode TabDurSym::Accept(ConstFunctor &functor) const
{
    return functor.VisitTabDurSym(this);
}

FunctorCode TabDurSym::AcceptEnd(Functor &functor)
{
    return functor.VisitTabDurSymEnd(this);
}

FunctorCode TabDurSym::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitTabDurSymEnd(this);
}

} // namespace vrv

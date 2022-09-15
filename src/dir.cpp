/////////////////////////////////////////////////////////////////////////////
// Name:        dir.cpp
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "dir.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "editorial.h"
#include "functorparams.h"
#include "symbol.h"
#include "text.h"
#include "verticalaligner.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Dir
//----------------------------------------------------------------------------

static const ClassRegistrar<Dir> s_factory("dir", DIR);

Dir::Dir()
    : ControlElement(DIR, "dir-")
    , TextListInterface()
    , TextDirInterface()
    , TimeSpanningInterface()
    , AttExtender()
    , AttLang()
    , AttLineRendBase()
    , AttVerticalGroup()
{
    this->RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());
    this->RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    this->RegisterAttClass(ATT_LANG);
    this->RegisterAttClass(ATT_EXTENDER);
    this->RegisterAttClass(ATT_LINERENDBASE);
    this->RegisterAttClass(ATT_VERTICALGROUP);

    this->Reset();
}

Dir::~Dir() {}

void Dir::Reset()
{
    ControlElement::Reset();
    TextDirInterface::Reset();
    TimeSpanningInterface::Reset();
    this->ResetExtender();
    this->ResetLang();
    this->ResetLineRendBase();
    this->ResetVerticalGroup();
}

bool Dir::IsSupportedChild(Object *child)
{
    if (child->Is({ LB, REND, SYMBOL, TEXT })) {
        assert(dynamic_cast<TextElement *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

bool Dir::AreChildrenAlignedTo(data_HORIZONTALALIGNMENT alignment) const
{
    ArrayOfConstObjects children = this->GetChildren();
    bool hasHalign = std::any_of(children.begin(), children.end(), [&alignment](const Object *child) {
        const AttHorizontalAlign *hAlign = dynamic_cast<const AttHorizontalAlign *>(child);
        return (hAlign && (hAlign->GetHalign() == alignment));
    });
    return hasHalign;
}

//----------------------------------------------------------------------------
// Dir functor methods
//----------------------------------------------------------------------------

int Dir::PrepareFloatingGrps(FunctorParams *)
{
    if (this->HasVgrp()) {
        this->SetDrawingGrpId(-this->GetVgrp());
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

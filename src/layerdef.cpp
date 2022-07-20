/////////////////////////////////////////////////////////////////////////////
// Name:        layerdef.h
// Author:      Andriy Makarchuk
// Created:     2022/02/28
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "layerdef.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "instrdef.h"
#include "label.h"
#include "labelabbr.h"

namespace vrv {

//----------------------------------------------------------------------------
// LayerDef
//----------------------------------------------------------------------------

static const ClassRegistrar<LayerDef> s_factory("layerDef", LAYERDEF);

LayerDef::LayerDef() : Object(LAYERDEF, "layerdef-"), AttLabelled(), AttNInteger(), AttTyped()
{
    this->RegisterAttClass(ATT_LABELLED);
    this->RegisterAttClass(ATT_NINTEGER);
    this->RegisterAttClass(ATT_TYPED);

    this->Reset();
}

LayerDef::~LayerDef() {}

void LayerDef::Reset()
{
    Object::Reset();
    this->ResetLabelled();
    this->ResetNInteger();
    this->ResetTyped();
}

bool LayerDef::IsSupportedChild(Object *child)
{
    if (child->Is(INSTRDEF)) {
        assert(dynamic_cast<InstrDef *>(child));
    }
    else if (child->Is(LABEL)) {
        assert(dynamic_cast<Label *>(child));
    }
    else if (child->Is(LABELABBR)) {
        assert(dynamic_cast<LabelAbbr *>(child));
    }
    else {
        return false;
    }
    return true;
}

} // namespace vrv

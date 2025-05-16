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

#include "functor.h"
#include "instrdef.h"
#include "label.h"
#include "labelabbr.h"

namespace vrv {

//----------------------------------------------------------------------------
// LayerDef
//----------------------------------------------------------------------------

static const ClassRegistrar<LayerDef> s_factory("layerDef", LAYERDEF);

LayerDef::LayerDef() : Object(LAYERDEF), AttLabelled(), AttNInteger(), AttTyped()
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

bool LayerDef::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ INSTRDEF, LABEL, LABELABBR };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else {
        return false;
    }
}

FunctorCode LayerDef::Accept(Functor &functor)
{
    return functor.VisitLayerDef(this);
}

FunctorCode LayerDef::Accept(ConstFunctor &functor) const
{
    return functor.VisitLayerDef(this);
}

FunctorCode LayerDef::AcceptEnd(Functor &functor)
{
    return functor.VisitLayerDefEnd(this);
}

FunctorCode LayerDef::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitLayerDefEnd(this);
}

} // namespace vrv

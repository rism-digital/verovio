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
#include "functor.h"
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

//----------------------------------------------------------------------------
// Dir functor methods
//----------------------------------------------------------------------------

FunctorCode Dir::Accept(MutableFunctor &functor)
{
    return functor.VisitDir(this);
}

FunctorCode Dir::Accept(ConstFunctor &functor) const
{
    return functor.VisitDir(this);
}

FunctorCode Dir::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitDirEnd(this);
}

FunctorCode Dir::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitDirEnd(this);
}

} // namespace vrv

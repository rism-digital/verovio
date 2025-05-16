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
    : ControlElement(DIR)
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

bool Dir::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ LB, REND, SYMBOL, TEXT };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else if (Object::IsEditorialElement(classId)) {
        return true;
    }
    else {
        return false;
    }
}

//----------------------------------------------------------------------------
// Dir functor methods
//----------------------------------------------------------------------------

FunctorCode Dir::Accept(Functor &functor)
{
    return functor.VisitDir(this);
}

FunctorCode Dir::Accept(ConstFunctor &functor) const
{
    return functor.VisitDir(this);
}

FunctorCode Dir::AcceptEnd(Functor &functor)
{
    return functor.VisitDirEnd(this);
}

FunctorCode Dir::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitDirEnd(this);
}

} // namespace vrv

/////////////////////////////////////////////////////////////////////////////
// Name:        reh.cpp
// Author:      Klaus Rettinghaus
// Created:     2020
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "reh.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "functor.h"
#include "measure.h"
#include "text.h"
#include "verticalaligner.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Reh
//----------------------------------------------------------------------------

static const ClassRegistrar<Reh> s_factory("reh", REH);

Reh::Reh()
    : ControlElement(REH, "reh-"), TextDirInterface(), TimePointInterface(), AttColor(), AttLang(), AttVerticalGroup()
{
    this->RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());
    this->RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_LANG);
    this->RegisterAttClass(ATT_VERTICALGROUP);

    this->Reset();
}

Reh::~Reh() {}

void Reh::Reset()
{
    ControlElement::Reset();
    TextDirInterface::Reset();
    TimePointInterface::Reset();
    this->ResetColor();
    this->ResetLang();
    this->ResetVerticalGroup();
}

bool Reh::IsSupportedChild(Object *child)
{
    if (child->Is({ LB, REND, TEXT })) {
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
// Reh functor methods
//----------------------------------------------------------------------------

FunctorCode Reh::Accept(Functor &functor)
{
    return functor.VisitReh(this);
}

FunctorCode Reh::Accept(ConstFunctor &functor) const
{
    return functor.VisitReh(this);
}

FunctorCode Reh::AcceptEnd(Functor &functor)
{
    return functor.VisitRehEnd(this);
}

FunctorCode Reh::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitRehEnd(this);
}

} // namespace vrv

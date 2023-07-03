/////////////////////////////////////////////////////////////////////////////
// Name:        labelabbr.cpp
// Author:      Klaus Rettinghaus
// Created:     07/03/2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "labelabbr.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "functor.h"
#include "text.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// LabelAbbr
//----------------------------------------------------------------------------

static const ClassRegistrar<LabelAbbr> s_factory("labelAbbr", LABELABBR);

LabelAbbr::LabelAbbr() : Object(LABELABBR, "labelAbbr-"), TextListInterface()
{
    this->Reset();
}

LabelAbbr::~LabelAbbr() {}

void LabelAbbr::Reset()
{
    Object::Reset();
}

bool LabelAbbr::IsSupportedChild(Object *child)
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
// Functor methods
//----------------------------------------------------------------------------

FunctorCode LabelAbbr::Accept(Functor &functor)
{
    return functor.VisitLabelAbbr(this);
}

FunctorCode LabelAbbr::Accept(ConstFunctor &functor) const
{
    return functor.VisitLabelAbbr(this);
}

FunctorCode LabelAbbr::AcceptEnd(Functor &functor)
{
    return functor.VisitLabelAbbrEnd(this);
}

FunctorCode LabelAbbr::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitLabelAbbrEnd(this);
}

} // namespace vrv

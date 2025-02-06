/////////////////////////////////////////////////////////////////////////////
// Name:        cpmark.cpp
// Author:      Laurent Pugin
// Created:     2024
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "cpmark.h"

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
// CpMark
//----------------------------------------------------------------------------

static const ClassRegistrar<CpMark> s_factory("cpMark", CPMARK);

CpMark::CpMark() : ControlElement(CPMARK, "cpmark-"), TextListInterface(), TextDirInterface(), TimeSpanningInterface()
{
    this->RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());
    this->RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());

    this->Reset();
}

CpMark::~CpMark() {}

void CpMark::Reset()
{
    ControlElement::Reset();
    TextDirInterface::Reset();
    TimeSpanningInterface::Reset();
}

bool CpMark::IsSupportedChild(Object *child)
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
// CpMark functor methods
//----------------------------------------------------------------------------

FunctorCode CpMark::Accept(Functor &functor)
{
    return functor.VisitCpMark(this);
}

FunctorCode CpMark::Accept(ConstFunctor &functor) const
{
    return functor.VisitCpMark(this);
}

FunctorCode CpMark::AcceptEnd(Functor &functor)
{
    return functor.VisitCpMarkEnd(this);
}

FunctorCode CpMark::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitCpMarkEnd(this);
}

} // namespace vrv

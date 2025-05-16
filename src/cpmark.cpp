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

CpMark::CpMark() : ControlElement(CPMARK), TextListInterface(), TextDirInterface(), TimeSpanningInterface()
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

bool CpMark::IsSupportedChild(ClassId classId)
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

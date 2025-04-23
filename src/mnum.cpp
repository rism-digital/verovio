/////////////////////////////////////////////////////////////////////////////
// Name:        mnum.cpp
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "mnum.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "functor.h"
#include "text.h"
#include "verticalaligner.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// MNum
//----------------------------------------------------------------------------

MNum::MNum()
    : ControlElement(MNUM), TextListInterface(), TextDirInterface(), TimePointInterface(), AttLang(), AttTypography()
{
    this->RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());
    this->RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    this->RegisterAttClass(ATT_LANG);
    this->RegisterAttClass(ATT_TYPOGRAPHY);

    this->Reset();
}

MNum::~MNum() {}

void MNum::Reset()
{
    ControlElement::Reset();
    TextDirInterface::Reset();
    TimePointInterface::Reset();
    this->ResetLang();
    this->ResetTypography();

    m_isGenerated = false;
}

bool MNum::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ REND, TEXT };

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
// MNum functor methods
//----------------------------------------------------------------------------

static const ClassRegistrar<MNum> s_factory("mNum", MNUM);

FunctorCode MNum::Accept(Functor &functor)
{
    return functor.VisitMNum(this);
}

FunctorCode MNum::Accept(ConstFunctor &functor) const
{
    return functor.VisitMNum(this);
}

FunctorCode MNum::AcceptEnd(Functor &functor)
{
    return functor.VisitMNumEnd(this);
}

FunctorCode MNum::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitMNumEnd(this);
}

} // namespace vrv

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
    : ControlElement(MNUM, "mnum-")
    , TextListInterface()
    , TextDirInterface()
    , TimePointInterface()
    , AttColor()
    , AttLang()
    , AttTypography()
{
    this->RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());
    this->RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);
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
    this->ResetColor();
    this->ResetLang();
    this->ResetTypography();

    m_isGenerated = false;
}

bool MNum::IsSupportedChild(Object *child)
{
    if (child->Is({ REND, TEXT })) {
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

/////////////////////////////////////////////////////////////////////////////
// Name:        choice.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "choice.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "abbr.h"
#include "corr.h"
#include "expan.h"
#include "orig.h"
#include "reg.h"
#include "sic.h"
#include "unclear.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Choice
//----------------------------------------------------------------------------

static const ClassRegistrar<Choice> s_factory("choice", CHOICE);

Choice::Choice() : EditorialElement(CHOICE)
{
    m_level = EDITORIAL_UNDEFINED;

    this->Reset();
}

Choice::Choice(EditorialLevel level) : EditorialElement(CHOICE)
{
    m_level = level;

    this->Reset();
}

void Choice::Reset()
{
    EditorialElement::Reset();
}

Choice::~Choice() {}

bool Choice::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ ABBR, CHOICE, CORR, EXPAN, ORIG, REG, SIC, UNCLEAR };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else {
        return false;
    }
}

//----------------------------------------------------------------------------
// functor methods
//----------------------------------------------------------------------------

} // namespace vrv

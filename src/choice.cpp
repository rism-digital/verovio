/////////////////////////////////////////////////////////////////////////////
// Name:        choice.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "choice.h"

//----------------------------------------------------------------------------

#include <assert.h>

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

Choice::Choice() : EditorialElement("choice-")
{
    m_level = EDITORIAL_UNDEFINED;

    Reset();
}

Choice::Choice(EditorialLevel level) : EditorialElement("choice-")
{
    m_level = level;

    Reset();
}

void Choice::Reset()
{
    EditorialElement::Reset();
}

Choice::~Choice() {}

bool Choice::IsSupportedChild(Object *child)
{
    if (child->Is(ABBR)) {
        assert(dynamic_cast<Abbr *>(child));
    }
    else if (child->Is(CHOICE)) {
        assert(dynamic_cast<Choice *>(child));
    }
    else if (child->Is(CORR)) {
        assert(dynamic_cast<Corr *>(child));
    }
    else if (child->Is(EXPAN)) {
        assert(dynamic_cast<Expan *>(child));
    }
    else if (child->Is(ORIG)) {
        assert(dynamic_cast<Orig *>(child));
    }
    else if (child->Is(REG)) {
        assert(dynamic_cast<Reg *>(child));
    }
    else if (child->Is(SIC)) {
        assert(dynamic_cast<Sic *>(child));
    }
    else if (child->Is(UNCLEAR)) {
        assert(dynamic_cast<Unclear *>(child));
    }
    else {
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------
// functor methods
//----------------------------------------------------------------------------

} // namespace vrv

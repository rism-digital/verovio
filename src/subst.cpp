/////////////////////////////////////////////////////////////////////////////
// Name:        subst.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "subst.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "add.h"
#include "del.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Subst
//----------------------------------------------------------------------------

static const ClassRegistrar<Subst> s_factory("subst", SUBST);

Subst::Subst() : EditorialElement(SUBST, "subst-")
{
    m_level = EDITORIAL_UNDEFINED;

    this->Reset();
}

Subst::Subst(EditorialLevel level) : EditorialElement(SUBST, "subst-")
{
    m_level = level;

    this->Reset();
}

void Subst::Reset()
{
    EditorialElement::Reset();
}

Subst::~Subst() {}

bool Subst::IsSupportedChild(Object *child)
{
    if (child->Is(ADD)) {
        assert(dynamic_cast<Add *>(child));
    }
    else if (child->Is(DEL)) {
        assert(dynamic_cast<Del *>(child));
    }
    else if (child->Is(SUBST)) {
        assert(dynamic_cast<Subst *>(child));
    }
    else {
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------
// Subst functor methods
//----------------------------------------------------------------------------

} // namespace vrv

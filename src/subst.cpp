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

Subst::Subst() : EditorialElement(SUBST)
{
    m_level = EDITORIAL_UNDEFINED;

    this->Reset();
}

Subst::Subst(EditorialLevel level) : EditorialElement(SUBST)
{
    m_level = level;

    this->Reset();
}

void Subst::Reset()
{
    EditorialElement::Reset();
}

Subst::~Subst() {}

bool Subst::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ ADD, DEL, SUBST };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else {
        return false;
    }
}

//----------------------------------------------------------------------------
// Subst functor methods
//----------------------------------------------------------------------------

} // namespace vrv

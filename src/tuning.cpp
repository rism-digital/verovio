/////////////////////////////////////////////////////////////////////////////
// Name:        tuning.cpp
// Author:      Laurent Pugin
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "tuning.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// Tuning
//----------------------------------------------------------------------------

Tuning::Tuning() : Object("element-"), AttNNumberLike()
{
    RegisterAttClass(ATT_NNUMBERLIKE);

    Reset();
}

Tuning::~Tuning()
{
}

void Tuning::Reset()
{
    Object::Reset();
    ResetNNumberLike();
}

void Tuning::AddChild(Object *child)
{
    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

} // namespace vrv

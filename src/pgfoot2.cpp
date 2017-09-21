/////////////////////////////////////////////////////////////////////////////
// Name:        pgfoot2.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "pgfoot2.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// PgFoot2
//----------------------------------------------------------------------------

PgFoot2::PgFoot2() : RunningElement("pgfoot2-")
{
    Reset();
}

PgFoot2::~PgFoot2()
{
}

void PgFoot2::Reset()
{
    RunningElement::Reset();
}

void PgFoot2::AddChild(Object *child)
{
    /*
    if (child->Is(SYSTEM)) {
        assert(dynamic_cast<System *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }
    */

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------
    
} // namespace vrv

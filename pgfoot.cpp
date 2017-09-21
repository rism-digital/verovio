/////////////////////////////////////////////////////////////////////////////
// Name:        pgfoot.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "pgfoot.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// PgFoot
//----------------------------------------------------------------------------

PgFoot::PgFoot() : RunningElement("pgfoot-")
{
    Reset();
}

PgFoot::~PgFoot()
{
}

void PgFoot::Reset()
{
    RunningElement::Reset();
}

void PgFoot::AddChild(Object *child)
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

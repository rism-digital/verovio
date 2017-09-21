/////////////////////////////////////////////////////////////////////////////
// Name:        pghead2.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "pghead2.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// PgHead2
//----------------------------------------------------------------------------

PgHead2::PgHead2() : RunningElement("pghead2-")
{
    Reset();
}

PgHead2::~PgHead2()
{
}

void PgHead2::Reset()
{
    RunningElement::Reset();
}

void PgHead2::AddChild(Object *child)
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

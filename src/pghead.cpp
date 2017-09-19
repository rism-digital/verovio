/////////////////////////////////////////////////////////////////////////////
// Name:        pghead.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "pghead.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// PgHead
//----------------------------------------------------------------------------

PgHead::PgHead() : FloatingObject("pghead-")
{
    Reset();
}

PgHead::~PgHead()
{
}

void PgHead::Reset()
{
    FloatingObject::Reset();
}

void PgHead::AddChild(Object *child)
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

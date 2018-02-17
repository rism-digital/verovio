/////////////////////////////////////////////////////////////////////////////
// Name:        pages.cpp
// Author:      Laurent Pugin
// Created:     2018/02/15
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "pages.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "page.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Pages
//----------------------------------------------------------------------------

Pages::Pages() : Object("pages-")
{
    Reset();
}

Pages::~Pages() {}

void Pages::Reset()
{
    Object::Reset();
}

void Pages::AddChild(Object *child)
{
    if (child->Is(PAGE)) {
        assert(dynamic_cast<Page *>(child));
    }
    else if (child->Is(SCOREDEF)) {
        assert(dynamic_cast<ScoreDef *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

} // namespace vrv

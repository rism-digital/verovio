/////////////////////////////////////////////////////////////////////////////
// Name:        app.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "app.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "lem.h"
#include "rdg.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// App
//----------------------------------------------------------------------------

App::App() : EditorialElement("app-")
{
    m_level = EDITORIAL_UNDEFINED;

    Reset();
}

App::App(EditorialLevel level) : EditorialElement("app-")
{
    m_level = level;

    Reset();
}

void App::Reset()
{
    EditorialElement::Reset();
}

App::~App() {}

bool App::IsSupportedChild(Object *child)
{
    if (child->Is(LEM)) {
        assert(dynamic_cast<Lem *>(child));
    }
    else if (child->Is(RDG)) {
        assert(dynamic_cast<Rdg *>(child));
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

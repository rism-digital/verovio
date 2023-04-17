/////////////////////////////////////////////////////////////////////////////
// Name:        app.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "app.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "lem.h"
#include "rdg.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// App
//----------------------------------------------------------------------------

static const ClassRegistrar<App> s_factory("app", APP);

App::App() : EditorialElement(APP, "app-")
{
    m_level = EDITORIAL_UNDEFINED;

    this->Reset();
}

App::App(EditorialLevel level) : EditorialElement(APP, "app-")
{
    m_level = level;

    this->Reset();
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

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

App::App() : EditorialElement(APP)
{
    m_level = EDITORIAL_UNDEFINED;

    this->Reset();
}

App::App(EditorialLevel level) : EditorialElement(APP)
{
    m_level = level;

    this->Reset();
}

void App::Reset()
{
    EditorialElement::Reset();
}

App::~App() {}

bool App::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ LEM, RDG };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else {
        return false;
    }
}

//----------------------------------------------------------------------------
// functor methods
//----------------------------------------------------------------------------

} // namespace vrv

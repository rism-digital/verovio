/////////////////////////////////////////////////////////////////////////////
// Name:        io.cpp
// Author:      Laurent Pugin
// Created:     2012
// Copyright (c) Laurent Pugin. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "io.h"

//----------------------------------------------------------------------------

#include <assert.h>

namespace vrv {

//----------------------------------------------------------------------------
// Output
//----------------------------------------------------------------------------

Output::Output(Doc *doc)
{
    assert(doc);
    m_doc = doc;
}

Output::~Output() {}

//----------------------------------------------------------------------------
// Input
//----------------------------------------------------------------------------

Input::Input(Doc *doc)
{
    assert(doc);
    m_doc = doc;
    Init();
}

void Input::Init()
{
    m_hasLayoutInformation = false;
}

Input::~Input() {}

} // namespace vrv

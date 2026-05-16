/////////////////////////////////////////////////////////////////////////////
// Name:        iobase.cpp
// Author:      Laurent Pugin
// Created:     2012
// Copyright (c) Laurent Pugin. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "iobase.h"

//----------------------------------------------------------------------------

#include <cassert>

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
    this->Init();
}

void Input::Init()
{
    m_layoutInformation = LAYOUT_NONE;
}

Input::~Input() {}

} // namespace vrv

/////////////////////////////////////////////////////////////////////////////
// Name:        iovolpiano.cpp
// Author:      Laurent Pugin
// Created:     2024
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "iovolpiano.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <fstream>
#include <sstream>
#include <string>

//----------------------------------------------------------------------------

#include "doc.h"
#include "vrv.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// VolpianoInput
//----------------------------------------------------------------------------

VolpianoInput::VolpianoInput(Doc *doc) : Input(doc) {}

VolpianoInput::~VolpianoInput() {}

//////////////////////////////////////////////////////////////////////////

bool VolpianoInput::Import(const std::string &volpiano)
{
    std::istringstream in_stream(volpiano);
    ParseVolpiano(in_stream);
    return true;
}

void VolpianoInput::ParseVolpiano(std::istream &infile) {}

} // namespace vrv

/////////////////////////////////////////////////////////////////////////////
// Name:        editortoolkit_cmn.cpp
// Author:      Laurent Pugin, Juliette Regimbal, Zoe McLennan
// Created:     04/06/2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "editortoolkit_cmn.h"

//--------------------------------------------------------------------------------

#include <locale>
#include <set>

//--------------------------------------------------------------------------------

namespace vrv {

EditorToolkitCMN::EditorToolkitCMN(Doc *doc, View *view) : EditorToolkitShared(doc, view) {}

EditorToolkitCMN::~EditorToolkitCMN() {}

} // namespace vrv

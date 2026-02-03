/////////////////////////////////////////////////////////////////////////////
// Name:        editortoolkit_cmn.h
// Author:      Juliette Regimbal
// Created:     04/06/2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_EDITOR_TOOLKIT_MENSURAL_H__
#define __VRV_EDITOR_TOOLKIT_MENSURAL_H__

#include <cmath>
#include <string>
#include <utility>

//--------------------------------------------------------------------------------

#include "doc.h"
#include "editortoolkit_shared.h"
#include "view.h"

#include "jsonxx.h"

namespace vrv {

//--------------------------------------------------------------------------------
// EditorToolkitMensural
//--------------------------------------------------------------------------------

class EditorToolkitMensural : public EditorToolkitShared {
public:
    EditorToolkitMensural(Doc *doc, View *view) : EditorToolkitShared(doc, view) {}
};
} // namespace vrv

#endif

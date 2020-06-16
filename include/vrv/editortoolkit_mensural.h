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
#include "editortoolkit.h"
#include "view.h"

#include "jsonxx.h"

namespace vrv {

//--------------------------------------------------------------------------------
// EditorToolkitMensural
//--------------------------------------------------------------------------------

class EditorToolkitMensural : public EditorToolkit {
public:
    EditorToolkitMensural(Doc *doc, View *view) : EditorToolkit(doc, view) {}
    bool ParseEditorAction(const std::string &json_editorAction)
    {
        LogError("Mensural editor toolkit is unimplemented!");
        return false;
    }

    /**
     * Get information on the last editor function used
     */
};
} // namespace vrv

#endif

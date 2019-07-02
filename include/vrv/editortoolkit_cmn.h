/////////////////////////////////////////////////////////////////////////////
// Name:        editortoolkit_cmn.h
// Author:      Juliette Regimbal
// Created:     04/06/2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_EDITOR_TOOLKIT_CMN_H__
#define __VRV_EDITOR_TOOLKIT_CMN_H__

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
// EditorToolkitCMN
//--------------------------------------------------------------------------------

class EditorToolkitCMN : public EditorToolkit {
public:
    EditorToolkitCMN(Doc * doc, View *view) : EditorToolkit(doc, view) {}
#ifdef USE_EMSCRIPTEN
    bool ParseEditorAction(const std::string &json_editorAction, bool isChain=false);

    /**
     * Experimental editor functions.
     */
    ///@{
    bool Drag(std::string elementId, int x, int y);
    bool Insert(std::string elementType, std::string startId, std::string endId);
    bool Set(std::string elementId, std::string attrType, std::string attrValue);
    ///@}
#endif

protected:

#ifdef USE_EMSCRIPTEN
    /**
     * Parse JSON instructions for experimental editor functions.
     */
    ///@{
    bool ParseDragAction(jsonxx::Object param, std::string *elementId, int *x, int *y);
    bool ParseInsertAction(jsonxx::Object param, std::string *elementType, std::string *startId, std::string *endId);
    bool ParseSetAction(jsonxx::Object param, std::string *elementId, std::string *attrType, std::string *attrValue);
    ///@}
#endif
};
} // namespace vrv

#endif

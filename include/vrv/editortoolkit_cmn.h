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
    bool ParseEditorAction(const std::string &json_editorAction);

protected:

    /**
     * Parse JSON instructions for experimental editor functions.
     */
    ///@{
    bool Chain(jsonxx::Array actions);
    bool ParseDragAction(jsonxx::Object param, std::string &elementId, int &x, int &y);
    bool ParseKeyDownAction(jsonxx::Object param, std::string &elementid, int &key, bool &shiftKey, bool &ctrlKey);
    bool ParseInsertAction(jsonxx::Object param, std::string &elementType, std::string &startid, std::string &endid);
    bool ParseSetAction(jsonxx::Object param, std::string &elementId, std::string &attribute, std::string &value);
    ///@}

    /**
     * Experimental editor functions.
     */
    ///@{
    bool Drag(std::string elementId, int x, int y);
    bool KeyDown(std::string elementId, int key, bool shiftKey, bool ctrlKey);
    bool Insert(std::string elementType, std::string startid, std::string endid);
    bool Set(std::string elementId, std::string attribute, std::string value);
    ///@}

    Object *GetElement(std::string &elementId);

public:
    //
protected:
    std::string m_chainedId;
};
} // namespace vrv

#endif

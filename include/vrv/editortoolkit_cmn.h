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
    EditorToolkitCMN(Doc *doc, View *view) : EditorToolkit(doc, view) {}
    virtual bool ParseEditorAction(const std::string &json_editorAction)
    {
        return ParseEditorAction(json_editorAction, false);
    }
    bool ParseEditorAction(const std::string &json_editorAction, bool commitOnly = false);
    virtual std::string EditInfo();

protected:
    /**
     * Parse JSON instructions for experimental editor functions.
     */
    ///@{
    bool Chain(jsonxx::Array actions);
    bool ParseDeleteAction(jsonxx::Object param, std::string &elementId);
    bool ParseDragAction(jsonxx::Object param, std::string &elementId, int &x, int &y);
    bool ParseKeyDownAction(jsonxx::Object param, std::string &elementid, int &key, bool &shiftKey, bool &ctrlKey);
    bool ParseInsertAction(jsonxx::Object param, std::string &elementType, std::string &startid, std::string &endid);
    bool ParseSetAction(jsonxx::Object param, std::string &elementId, std::string &attribute, std::string &value);
    ///@}

    /**
     * Experimental editor functions.
     */
    ///@{
    bool Delete(std::string &elementId);
    bool Drag(std::string &elementId, int x, int y);
    bool KeyDown(std::string &elementId, int key, bool shiftKey, bool ctrlKey);
    bool Insert(std::string &elementType, std::string const &startid, std::string const &endid);
    bool Insert(std::string &elementType, std::string const &startid);
    bool Set(std::string &elementId, std::string const &attribute, std::string const &value);
    ///@}

    bool InsertNote(Object *object);

    bool DeleteNote(Note *note);

    Object *GetElement(std::string &elementId);

public:
    //
protected:
    std::string m_chainedId;
};
} // namespace vrv

#endif

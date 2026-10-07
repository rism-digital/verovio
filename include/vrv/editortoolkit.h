/////////////////////////////////////////////////////////////////////////////
// Name:        editortoolkit.h
// Author:      Laurent Pugin, Juliette Regimbal
// Created:     16/05/2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_EDITOR_TOOLKIT_H__
#define __VRV_EDITOR_TOOLKIT_H__

#include <cmath>
#include <string>
#include <utility>

//--------------------------------------------------------------------------------

#include "doc.h"
#include "view.h"

#include "jsonxx.h"

namespace vrv {

class Cursor;

//--------------------------------------------------------------------------------
// EditorToolkit
//--------------------------------------------------------------------------------

class EditorToolkit {
public:
    EditorToolkit(Doc *doc, View *view)
    {
        m_doc = doc;
        m_view = view;
        m_editStatus.reset();

        m_selectionId = "";
        m_selectionClassId = UNSPECIFIED;
        m_selectionSecondaryId = "";
        m_cursor = NULL;

        m_options = 0;
    }
    virtual ~EditorToolkit() {}

    /**
     * In child classes, this parses the provided editor action and then performs the correct action.
     */
    virtual bool ParseEditorAction(const std::string &json_editorAction) = 0;
    /**
     * Get status of the editor (undo, selection, etc.)
     */
    virtual std::string EditStatus() { return m_editStatus.json(); }
    /**
     * Get response on the last editor function used
     */
    virtual std::string EditResponse() { return m_editResponse.json(); }

    /**
     * Increase the option change count
     */
    void OptionsChanged() { m_options++; }

#ifndef NO_EDIT_SUPPORT
protected:
    void ResetSelect();
    bool AppendChild(std::string &elementId, const std::string &elementName, bool unique);
    bool InsertBefore(std::string &elementId, const std::string &elementName);
    bool InsertAfter(std::string &elementId, const std::string &elementName);
    Object *GetElement(const std::string &elementId);
    Object *PrepareInsertion(Object *parent, const std::string &elementName);
    Object *ResolveElement(std::string &elementId, bool chain = true);
    bool InsertMode() const { return (m_cursor); }
#endif

protected:
    std::string m_chainedId;
    std::string m_selectionId;
    ClassId m_selectionClassId;
    std::string m_selectionSecondaryId;
    Cursor *m_cursor;

    Doc *m_doc;
    View *m_view;
    jsonxx::Object m_editStatus;
    jsonxx::Object m_editResponse;

    /** Record option changes  */
    int m_options;
};
} // namespace vrv

#endif

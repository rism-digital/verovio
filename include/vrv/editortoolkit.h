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

//--------------------------------------------------------------------------------
// EditorToolkit
//--------------------------------------------------------------------------------

class EditorToolkit {
public:
    EditorToolkit(Doc *doc, View *view)
    {
        m_doc = doc;
        m_view = view;
        m_editInfo = "";
    }
    virtual ~EditorToolkit() {}

    /**
     * In child classes, this parses the provided editor action and then performs the correct action.
     */
    virtual bool ParseEditorAction(const std::string &json_editorAction) = 0;
    /**
     * Get information on the last editor function used
     */
    virtual std::string EditInfo() { return m_editInfo; }

protected:
    Doc *m_doc;
    View *m_view;
    std::string m_editInfo;
};
} // namespace vrv

#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        editortoolkit.h
// Author:      Laurent Pugin, Juliette Regimbal
// Created:     16/05/2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_EDITOR_TOOLKIT_H__
#define __VRV_EDITOR_TOOLKIT_H__

#include <string>

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
    EditorToolkit(Doc *doc, View *view) { m_doc = doc; m_view = view; }

#ifdef USE_EMSCRIPTEN
    bool ParseEditorAction(const std::string &json_editorAction);
    std::string ParseQueryAction(const std::string &json_editorAction);

    /**
     * Experimental editor functions.
     */
    ///@{
    bool Drag(std::string elementId, int x, int y);
    bool Insert(std::string elementType, std::string startId, std::string endId);
    bool Set(std::string elementId, std::string attrType, std::string attrValue);
    ///@}
    
    /**
     * Get information on neume group by ID.
     */
    std::string GetNeumeInfo(std::string elementId);
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
    bool ParseNeumeInfoAction(jsonxx::Object param, std::string *elementId);
    ///@}
#endif

    Doc *m_doc;
    View *m_view;
};

} // namespace vrv

#endif

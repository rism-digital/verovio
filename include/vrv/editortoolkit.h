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

    Doc *m_doc;
    View *m_view;
};

} // namespace vrv

#endif

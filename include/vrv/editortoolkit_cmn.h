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

class EditorTreeObject;

//--------------------------------------------------------------------------------
// EditorToolkitCMN
//--------------------------------------------------------------------------------

class EditorToolkitCMN : public EditorToolkit {
public:
    EditorToolkitCMN(Doc *doc, View *view);
    bool ParseEditorAction(const std::string &json_editorAction) override
    {
        return ParseEditorAction(json_editorAction, false);
    }
    bool ParseEditorAction(const std::string &json_editorAction, bool commitOnly = false);
    std::string EditInfo() override;
    
protected:
    /**
     * Parse JSON instructions for experimental editor functions.
     */
    ///@{
    bool Chain(jsonxx::Array actions);
    bool ParseContextAction(jsonxx::Object param, std::string &elementId, bool &scores, bool &sections);
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
    
    bool ContextForElement(std::string &elementId);
    bool ContextForScores(bool editInfo);
    bool ContextForSections(bool editInfo);
    
    Object *GetElement(std::string &elementId);
    
    void ContextForObject(const Object *object, jsonxx::Object &element, bool recursive = false);
    void ContextForObjects(const ListOfConstObjects &objects, jsonxx::Array &siblings);
    void ContextForReferences(const ListOfObjectAttNamePairs &objects, jsonxx::Array &links);

public:
    //
protected:
    std::string m_chainedId;
    
    EditorTreeObject *m_scoreContext;
    EditorTreeObject *m_sectionContext;
};

//----------------------------------------------------------------------------
// EditorTreeObject
//----------------------------------------------------------------------------

/**
 * This class stores an alignment position elements will point to
 */
class EditorTreeObject : public Object, public VisibilityDrawingInterface {

public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    EditorTreeObject(const Object *object);
    virtual ~EditorTreeObject() {};
    void Reset() override;
    std::string GetClassName() const override { return m_className; }
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    bool IsSupportedChild(ClassId classId) override { return true; }
    ///@}
    
private:
    //
public:
    std::string m_className;
    const Object *m_object;

private:
    //
};

} // namespace vrv

#endif

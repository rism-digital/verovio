/////////////////////////////////////////////////////////////////////////////
// Name:        editortoolkit_cmn.h
// Author:      Juliette Regimbal
// Created:     04/06/2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_EDITOR_TOOLKIT_CMN_H__
#define __VRV_EDITOR_TOOLKIT_CMN_H__

#include <cmath>
#include <deque>
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
    virtual ~EditorToolkitCMN();
    bool ParseEditorAction(const std::string &json_editorAction) override
    {
        return ParseEditorAction(json_editorAction, false);
    }
    bool ParseEditorAction(const std::string &json_editorAction, bool commitOnly = false);
    std::string EditInfo() override;

protected:
#ifndef NO_EDIT_SUPPORT
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

    void PrepareUndo();
    std::string GetCurrentState();
    bool ReloadState(const std::string &data);
    void TrimUndoMemory();
    bool CanUndo() const;
    bool CanRedo() const;
    bool Undo();
    bool Redo();

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

    void ClearContext();
    bool ContextForElement(std::string &elementId);
    bool ContextForScores(bool editInfo);
    bool ContextForSections(bool editInfo);

    Object *GetElement(std::string &elementId);

    void ContextForObject(const Object *object, jsonxx::Object &element, bool recursive = false);
    void ContextForObjects(const ArrayOfConstObjects &objects, jsonxx::Array &siblings);
    void ContextForReferences(const ListOfObjectAttNamePairs &objects, jsonxx::Array &links);

    ArrayOfConstObjects GetScoreBasedChildrenFor(const Object *object);

public:
    //
protected:
    std::string m_chainedId;

    bool m_undoPrepared;
    std::deque<std::string> m_undoStack;
    std::deque<std::string> m_redoStack;
    size_t m_undoMemoryUsage = 0;

    EditorTreeObject *m_scoreContext;
    EditorTreeObject *m_sectionContext;
    EditorTreeObject *m_currentContext;
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
     * Reset method resets all attribute classes
     */
    ///@{
    EditorTreeObject(const Object *object, bool ownChildren);
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

    /**
     * @name Getter to interfaces
     */
    ///@{
    VisibilityDrawingInterface *GetVisibilityDrawingInterface() override
    {
        return vrv_cast<VisibilityDrawingInterface *>(this);
    }
    const VisibilityDrawingInterface *GetVisibilityDrawingInterface() const override
    {
        return vrv_cast<const VisibilityDrawingInterface *>(this);
    }
    ///@}

    ArrayOfConstObjects GetChildObjects() const;

private:
    //
public:
    std::string m_className;
    const Object *m_object;

private:
    //
#endif /* NO_EDIT_SUPPORT */
};

} // namespace vrv

#endif

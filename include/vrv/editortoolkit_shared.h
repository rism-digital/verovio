/////////////////////////////////////////////////////////////////////////////
// Name:        editortoolkit_shared.h
// Author:      Laurent Pugin
// Created:     03/02/2026
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_EDITOR_TOOLKIT_SHARED_H__
#define __VRV_EDITOR_TOOLKIT_SHARED_H__

#include <cmath>
#include <deque>
#include <string>
#include <utility>

//--------------------------------------------------------------------------------

#include "cursor.h"
#include "doc.h"
#include "editortoolkit.h"
#include "view.h"

#include "jsonxx.h"

namespace vrv {

class EditorTreeObject;

//--------------------------------------------------------------------------------
// EditorToolkitShared
//--------------------------------------------------------------------------------

class EditorToolkitShared : public EditorToolkit {
public:
    EditorToolkitShared(Doc *doc, View *view);
    virtual ~EditorToolkitShared();
    bool ParseEditorAction(const std::string &json_editorAction) override
    {
        return ParseEditorAction(json_editorAction, false);
    }
    bool ParseEditorAction(const std::string &json_editorAction, bool commitOnly = false);

protected:
#ifndef NO_EDIT_SUPPORT

    enum SelectCustom : int8_t { SELECT_NONE = 0, SELECT_NOTE, SELECT_TEXT_PARENT };

    struct State {
        std::string data;
        std::string status;
        int options;
    };

    enum DeleteNavigation : int8_t { DELETE_NO_NAVIGATON = 0, DELETE_BACKSPACE, DELETE_FORWARD };

    /**
     * Parse JSON instructions for experimental editor functions.
     */
    ///@{
    bool Chain(const jsonxx::Array &actions);
    bool ParseContextAction(const jsonxx::Object &param, std::string &elementId, bool &scores, bool &sections);
    bool ParseDeleteAction(const jsonxx::Object &param, std::string &elementId, DeleteNavigation &navigation);
    bool ParseDragAction(const jsonxx::Object &param, std::string &elementId, int &x, int &y);
    bool ParseKeyDownAction(
        const jsonxx::Object &param, std::string &elementid, int &key, bool &shiftKey, bool &ctrlKey);
    bool ParseInsertAction(
        const jsonxx::Object &param, std::string &elementName, std::string &elementId, std::string &insertMode);
    bool ParseInsertControlAction(
        const jsonxx::Object &param, std::string &elementName, std::string &startId, std::string &endId);
    bool ParseNavigate(const jsonxx::Object &param, std::string &elementId, int &direction);
    bool ParsePropertiesAction(const jsonxx::Object &param, std::string &scoreDef);
    bool ParseResetCursorAction(const jsonxx::Object &param, bool &maintainChordMode);
    bool ParseSelectAction(const jsonxx::Object &param, std::string &elementId, bool &secondary, SelectCustom &custom);
    bool ParseSetAction(
        const jsonxx::Object &param, std::string &elementId, std::string &attribute, std::string &value);
    bool ParseSetCursorAction(
        const jsonxx::Object &param, std::string &elementId, Cursor::InputMode &inputMode, bool &chordMode);
    bool ParseUpdateCursorAction(
        const jsonxx::Object &param, bool &restMode, bool &chordMode, Cursor::TieMode &tieMode);
    bool ParseUpdatePitchAction(const jsonxx::Object &param, std::string &elementId, data_PITCHNAME &pname, int &oct,
        data_ACCIDENTAL_WRITTEN &accid, int &midi);

    ///@}

    void SetEditStatus();
    void ReloadEditStatus(const std::string &statusStr, bool insertMode);
    void PrepareUndo(bool ignoreInsertMode = false);
    std::string GetCurrentState();
    bool ReloadState(const State &state);
    void TrimUndoMemory();
    bool CanUndo() const;
    bool CanRedo() const;
    bool Undo();
    bool Redo();

    /**
     * Experimental editor functions.
     */
    ///@{
    bool SetCursor(std::string &elementId, Cursor::InputMode inputMode, bool chordMode);
    bool UpdateCursor(bool restMode, bool chordMode, Cursor::TieMode tieMode);
    bool ResetCursor(bool maintainChordMode);
    bool Delete(std::string &elementId, DeleteNavigation navigation);
    bool Drag(std::string &elementId, int x, int y);
    bool InsertControl(std::string &elementName, std::string &startId, std::string &endId);
    bool KeyDown(std::string &elementId, int key, bool shiftKey, bool ctrlKey);
    bool Navigate(std::string &elementId, const int &direction);
    bool Select(std::string &elementId, bool secondary, SelectCustom custom);
    bool Set(std::string &elementId, const std::string &attribute, const std::string &value);
    bool UpdatePitch(std::string &elementId, data_PITCHNAME pname, int oct, data_ACCIDENTAL_WRITTEN accid, int midi);

    ///@}

    void ClearContext();
    bool ContextForElement(std::string &elementId);
    bool ContextForScores(bool updateResponse);
    bool ContextForSections(bool updateResponse);

    bool GetScoreDef();
    bool SetScoreDef(const std::string scoreDef);

    void ContextForObject(const Object *object, jsonxx::Object &element, bool recursive = false);
    void ContextForObjects(const ArrayOfConstObjects &objects, jsonxx::Array &siblings);
    void ContextForReferences(const ListOfObjectAttNamePairs &objects, jsonxx::Array &links);

    ArrayOfConstObjects GetScoreBasedChildrenFor(const Object *object);

    void CollectReferringObjects(
        const Object *element, std::set<std::string> &toDelete, std::set<const Object *> &visited);
    void PostProcessDeleteObjects(const Object *element, std::set<std::string> &toPostProcess);
    void PostProcessDelete(const std::string &elementId);

    void PostEditRestriction(Object *element);

    void MoveCursor(LayerElement *element, bool maintainChordMode = false);

    data_ACCIDENTAL_WRITTEN GetAccidBefore(const LayerElement *element, data_PITCHNAME pname, int oct);
    std::pair<data_ACCIDENTAL_WRITTEN, bool> GetActualAccid(Object *element, data_ACCIDENTAL_WRITTEN accid);

    const Measure *GetPreviousMeasure(const Measure *measure);
    const Staff *GetPreviousStaff(const Staff *staff);
    const Layer *GetPreviousLayer(const Layer *layer);
    const Measure *GetNextMeasure(const Measure *measure);
    const Staff *GetNextStaff(const Staff *staff);
    const Layer *GetNextLayer(const Layer *layer);

public:
    //
protected:
    bool m_undoPrepared;

    std::deque<State> m_undoStack;
    std::deque<State> m_redoStack;
    size_t m_undoMemoryUsage = 0;

    EditorTreeObject *m_scoreContext;
    EditorTreeObject *m_sectionContext;
    EditorTreeObject *m_currentContext;

private:
    struct MidiSpelling {
        data_PITCHNAME pname;
        data_ACCIDENTAL_WRITTEN accid;
    };

    MidiSpelling SpellMidi(int midi, const data_KEYSIGNATURE &keySig);
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

/////////////////////////////////////////////////////////////////////////////
// Name:        editortoolkit_shared.cpp
// Author:      Laurent Pugin
// Created:     03/02/2026
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "editortoolkit_shared.h"

//--------------------------------------------------------------------------------

#include <locale>
#include <set>

//--------------------------------------------------------------------------------

#include "alignfunctor.h"
#include "chord.h"
#include "clef.h"
#include "comparison.h"
#include "cursor.h"
#include "dir.h"
#include "dynam.h"
#include "editfunctor.h"
#include "editorial.h"
#include "editortoolkit_cmn.h"
#include "findfunctor.h"
#include "hairpin.h"
#include "iomei.h"
#include "layer.h"
#include "mdiv.h"
#include "measure.h"
#include "mnum.h"
#include "note.h"
#include "page.h"
#include "pages.h"
#include "plistinterface.h"
#include "rend.h"
#include "rest.h"
#include "slur.h"
#include "staff.h"
#include "surface.h"
#include "symboldef.h"
#include "system.h"
#include "systemelement.h"
#include "text.h"
#include "tie.h"
#include "timeinterface.h"
#include "vrv.h"
#include "zone.h"

//--------------------------------------------------------------------------------

#define UNDO_MEMORY_LIMIT (256 * 1024 * 1024) // 256 MB

namespace vrv {

EditorToolkitShared::EditorToolkitShared(Doc *doc, View *view) : EditorToolkit(doc, view)
{
    m_undoPrepared = false;
    m_scoreContext = NULL;
    m_sectionContext = NULL;
    m_currentContext = NULL;

    this->SetEditStatus();
}

EditorToolkitShared::~EditorToolkitShared()
{
#ifndef NO_EDIT_SUPPORT
    this->ClearContext();
#endif
}

bool EditorToolkitShared::ParseEditorAction(const std::string &json_editorAction, bool commitOnly)
{
#ifndef NO_EDIT_SUPPORT
    jsonxx::Object json;

    // Read JSON actions
    if (!json.parse(json_editorAction)) {
        LogError("Cannot parse JSON std::string.");
        return false;
    }

    if (!json.has<jsonxx::String>("action")) {
        LogWarning("Incorrectly formatted JSON action.");
    }

    std::string action = json.get<jsonxx::String>("action");

    if (action != "context" && action != "properties") {
        m_doc->SetFocus();
    }

    // Action without parameter
    if (action == "commit") {
        m_doc->PrepareData();
        m_doc->ScoreDefSetCurrentDoc(true);
        m_doc->RefreshLayout();
        m_undoPrepared = false;
        this->SetEditStatus();
        return true;
    }

    // Undo and redo - also without parameter
    if ((action == "undo") || (action == "redo")) {
        this->ClearContext();
        if (action == "undo") {
            this->Undo();
        }
        else {
            this->Redo();
        }
        m_doc->PrepareData();
        m_doc->ScoreDefSetCurrentDoc(true);
        m_undoPrepared = false;
        this->SetEditStatus();
        return true;
    }

    if (commitOnly) {
        // Only process commit actions
        return false;
    }

    if (!json.has<jsonxx::Object>("param") && !json.has<jsonxx::Array>("param")) {
        LogWarning("Incorrectly formatted JSON param.");
    }

    if (action == "chain") {
        if (!json.has<jsonxx::Array>("param")) {
            LogError("Incorrectly formatted JSON action");
            return false;
        }
        return this->Chain(json.get<jsonxx::Array>("param"));
    }
    else if (action == "context") {
        std::string elementId;
        bool scores;
        bool sections;
        if (this->ParseContextAction(json.get<jsonxx::Object>("param"), elementId, scores, sections)) {
            if (scores) {
                return this->ContextForScores(true);
            }
            else if (sections) {
                return this->ContextForSections(true);
            }
            else {
                return this->ContextForElement(elementId);
            }
        }
        LogWarning("Could not parse the context action");
    }
    else if (action == "delete") {
        std::string elementId;
        bool backspace;
        if (this->ParseDeleteAction(json.get<jsonxx::Object>("param"), elementId, backspace)) {
            this->PrepareUndo();
            return (this->Delete(elementId, backspace));
        }
        LogWarning("Could not parse the delete action");
    }
    else if (action == "drag") {
        std::string elementId;
        int x, y;
        if (this->ParseDragAction(json.get<jsonxx::Object>("param"), elementId, x, y)) {
            this->PrepareUndo();
            return (this->Drag(elementId, x, y));
        }
        LogWarning("Could not parse the drag action");
    }
    else if (action == "insert") {
        std::string elementName, elementId, insertMode;
        if (this->ParseInsertAction(json.get<jsonxx::Object>("param"), elementName, elementId, insertMode)) {
            this->PrepareUndo();
            // LogInfo("%s %s %s", elementName.c_str(), elementId.c_str(), insertMode.c_str());
            if (insertMode == "appendChild") {
                return (this->AppendChild(elementId, elementName, false));
            }
            else if (insertMode == "appendChildNoDuplicate") {
                return (this->AppendChild(elementId, elementName, true));
            }
            else if (insertMode == "insertBefore") {
                return (this->InsertBefore(elementId, elementName));
            }
            else if (insertMode == "insertAfter") {
                return (this->InsertAfter(elementId, elementName));
            }
        }
        LogWarning("Could not parse the insert action");
    }
    else if (action == "insertControl") {
        std::string elementName, startId, endId;
        if (this->ParseInsertControlAction(json.get<jsonxx::Object>("param"), elementName, startId, endId)) {
            this->PrepareUndo();
            // LogInfo("%s %s %s", elementName.c_str(), elementId.c_str(), insertMode.c_str());
            return (this->InsertControl(elementName, startId, endId));
        }
        LogWarning("Could not parse the insertControl action");
    }
    else if (action == "insertCursorByDur") {
        EditorToolkitCMN *editorToolkitCMN = dynamic_cast<EditorToolkitCMN *>(this);
        if (editorToolkitCMN) {
            return editorToolkitCMN->ParseEditorCMNAction(json);
        }
        LogWarning("Action insertCursorByDur available in CMN only");
    }
    else if (action == "insertCursorByPitch") {
        EditorToolkitCMN *editorToolkitCMN = dynamic_cast<EditorToolkitCMN *>(this);
        if (editorToolkitCMN) {
            return editorToolkitCMN->ParseEditorCMNAction(json);
        }
        LogWarning("Action insertCursorByPitch available in CMN only");
    }
    else if (action == "insertMeasure") {
        EditorToolkitCMN *editorToolkitCMN = dynamic_cast<EditorToolkitCMN *>(this);
        if (editorToolkitCMN) {
            return editorToolkitCMN->ParseEditorCMNAction(json);
        }
        LogWarning("Action insertMeasure available in CMN only");
    }
    else if (action == "insertNote") {
        EditorToolkitCMN *editorToolkitCMN = dynamic_cast<EditorToolkitCMN *>(this);
        if (editorToolkitCMN) {
            return editorToolkitCMN->ParseEditorCMNAction(json);
        }
        LogWarning("Action insertNote available in CMN only");
    }
    else if (action == "insertRest") {
        EditorToolkitCMN *editorToolkitCMN = dynamic_cast<EditorToolkitCMN *>(this);
        if (editorToolkitCMN) {
            return editorToolkitCMN->ParseEditorCMNAction(json);
        }
        LogWarning("Action insertRest available in CMN only");
    }
    else if (action == "keyDown") {
        std::string elementId;
        int key;
        bool shiftKey, ctrlKey;
        if (this->ParseKeyDownAction(json.get<jsonxx::Object>("param"), elementId, key, shiftKey, ctrlKey)) {
            this->PrepareUndo();
            return (this->KeyDown(elementId, key, shiftKey, ctrlKey));
        }
        LogWarning("Could not parse the keyDown action");
    }
    else if (action == "navigate") {
        std::string elementId;
        int direction;
        if (this->ParseNavigate(json.get<jsonxx::Object>("param"), elementId, direction)) {
            return this->Navigate(elementId, direction);
        }
        LogWarning("Could not parse the navigate action");
    }
    else if (action == "properties") {
        std::string scoreDef;
        if (this->ParsePropertiesAction(json.get<jsonxx::Object>("param"), scoreDef)) {
            if (scoreDef.empty()) {
                return this->GetScoreDef();
            }
            else {
                return this->SetScoreDef(scoreDef);
            }
        }
    }
    else if (action == "resetCursor") {
        bool maintainChordMode;
        if (this->ParseResetCursorAction(json.get<jsonxx::Object>("param"), maintainChordMode)) {
            this->PrepareUndo();
            return (this->ResetCursor(maintainChordMode));
        }
        LogWarning("Could not parse the resetCursor action");
    }
    else if (action == "select") {
        std::string elementId;
        bool secondary;
        if (this->ParseSelectAction(json.get<jsonxx::Object>("param"), elementId, secondary)) {
            return this->Select(elementId, secondary);
        }
    }
    else if (action == "set") {
        std::string elementId, attribute, value;
        if (this->ParseSetAction(json.get<jsonxx::Object>("param"), elementId, attribute, value)) {
            this->PrepareUndo();
            return (this->Set(elementId, attribute, value));
        }
        LogWarning("Could not parse the set action");
    }
    else if (action == "setCursor") {
        std::string elementId;
        Cursor::InputMode inputMode;
        bool chordMode;
        if (this->ParseSetCursorAction(json.get<jsonxx::Object>("param"), elementId, inputMode, chordMode)) {
            this->PrepareUndo();
            return (this->SetCursor(elementId, inputMode, chordMode));
        }
        LogWarning("Could not parse the setCursor action");
    }
    else if (action == "updateCursor") {
        bool restMode;
        bool chordMode;
        if (this->ParseUpdateCursorAction(json.get<jsonxx::Object>("param"), restMode, chordMode)) {
            this->PrepareUndo();
            return (this->UpdateCursor(restMode, chordMode));
        }
        LogWarning("Could not parse the setCursor action");
    }
    else if (action == "updatePitch") {
        std::string elementId;
        data_PITCHNAME pname;
        int oct;
        data_ACCIDENTAL_WRITTEN accid;
        int midi;
        if (this->ParseUpdatePitchAction(json.get<jsonxx::Object>("param"), elementId, pname, oct, accid, midi)) {
            this->PrepareUndo();
            return (this->UpdatePitch(elementId, pname, oct, accid, midi));
        }
        LogWarning("Could not parse the updatePitch action");
    }
    else {
        LogWarning("Unknown action type '%s'.", action.c_str());
    }
    return false;
#else /* NO_EDIT_SUPPORT */
    LogError("Editor functions are not supported in this build.");
    return false;
#endif /* NO_EDIT_SUPPORT */
}

#ifndef NO_EDIT_SUPPORT
bool EditorToolkitShared::ParseContextAction(
    const jsonxx::Object &param, std::string &elementId, bool &scores, bool &sections)
{
    scores = false;
    sections = false;
    if (param.has<jsonxx::String>("elementId")) {
        elementId = param.get<jsonxx::String>("elementId");
        return true;
    }
    else if (param.has<jsonxx::String>("document")) {
        scores = (param.get<jsonxx::String>("document") == "scores");
        sections = !scores;
        return true;
    }
    return false;
}

bool EditorToolkitShared::ParseDeleteAction(const jsonxx::Object &param, std::string &elementId, bool &backspace)
{
    backspace = false;
    if (!param.has<jsonxx::String>("elementId")) return false;
    elementId = param.get<jsonxx::String>("elementId");
    if (param.has<jsonxx::Boolean>("backspace")) backspace = param.get<jsonxx::Boolean>("backspace");
    return true;
}

bool EditorToolkitShared::ParseDragAction(const jsonxx::Object &param, std::string &elementId, int &x, int &y)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    elementId = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::Number>("x")) return false;
    x = param.get<jsonxx::Number>("x");
    if (!param.has<jsonxx::Number>("y")) return false;
    y = param.get<jsonxx::Number>("y");
    return true;
}

bool EditorToolkitShared::ParseInsertAction(
    const jsonxx::Object &param, std::string &elementName, std::string &elementId, std::string &insertMode)
{
    if (!param.has<jsonxx::String>("elementName")) return false;
    elementName = param.get<jsonxx::String>("elementName");
    if (!param.has<jsonxx::String>("elementId")) return false;
    elementId = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::String>("insertMode")) return false;
    insertMode = param.get<jsonxx::String>("insertMode");
    return true;
}

bool EditorToolkitShared::ParseInsertControlAction(
    const jsonxx::Object &param, std::string &elementName, std::string &startId, std::string &endId)
{
    if (!param.has<jsonxx::String>("elementName")) return false;
    elementName = param.get<jsonxx::String>("elementName");
    if (!param.has<jsonxx::String>("startId")) return false;
    startId = param.get<jsonxx::String>("startId");
    if (!param.has<jsonxx::String>("endId")) return true;
    endId = param.get<jsonxx::String>("endId");
    return true;
}

bool EditorToolkitShared::ParseKeyDownAction(
    const jsonxx::Object &param, std::string &elementId, int &key, bool &shiftKey, bool &ctrlKey)
{
    // assign optional member
    shiftKey = false;
    ctrlKey = false;

    if (!param.has<jsonxx::String>("elementId")) return false;
    elementId = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::Number>("key")) return false;
    key = param.get<jsonxx::Number>("key");
    // optional
    if (param.has<jsonxx::Boolean>("shiftKey")) {
        shiftKey = param.get<jsonxx::Boolean>("shiftKey");
    }
    if (param.has<jsonxx::Boolean>("ctrlKey")) {
        ctrlKey = param.get<jsonxx::Boolean>("ctrlKey");
    }
    return true;
}

bool EditorToolkitShared::ParseNavigate(const jsonxx::Object &param, std::string &elementId, int &direction)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    elementId = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::Number>("direction")) return false;
    direction = param.get<jsonxx::Number>("direction");
    return true;
}

bool EditorToolkitShared::ParsePropertiesAction(const jsonxx::Object &param, std::string &scoreDef)
{
    scoreDef = "";
    if (param.has<jsonxx::String>("scoreDef")) {
        scoreDef = param.get<jsonxx::String>("scoreDef");
        return true;
    }
    return true;
}

bool EditorToolkitShared::ParseResetCursorAction(const jsonxx::Object &param, bool &maintainChordMode)
{
    maintainChordMode = false;

    if (param.has<jsonxx::Boolean>("maintainChordMode"))
        maintainChordMode = param.get<jsonxx::Boolean>("maintainChordMode");

    return true;
}

bool EditorToolkitShared::ParseSelectAction(const jsonxx::Object &param, std::string &elementId, bool &secondary)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    elementId = param.get<jsonxx::String>("elementId");
    secondary = false;
    if (param.has<jsonxx::Boolean>("secondary")) {
        secondary = param.get<jsonxx::Boolean>("secondary");
    }
    return true;
}

bool EditorToolkitShared::ParseSetAction(
    const jsonxx::Object &param, std::string &elementId, std::string &attribute, std::string &value)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    elementId = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::String>("attribute")) return false;
    attribute = param.get<jsonxx::String>("attribute");
    if (!param.has<jsonxx::String>("value")) return false;
    value = param.get<jsonxx::String>("value");
    return true;
}

bool EditorToolkitShared::ParseSetCursorAction(
    const jsonxx::Object &param, std::string &elementId, Cursor::InputMode &inputMode, bool &chordMode)
{
    elementId = "";
    inputMode = Cursor::PITCH_FIRST;
    chordMode = false;

    if (param.has<jsonxx::String>("elementId")) elementId = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::String>("inputMode")) return false;
    inputMode = (param.get<jsonxx::String>("inputMode") == "pitchFirst") ? Cursor::PITCH_FIRST : Cursor::DURATION_FIRST;
    if (!param.has<jsonxx::Boolean>("chordMode")) return false;
    chordMode = param.get<jsonxx::Boolean>("chordMode");

    return true;
}

bool EditorToolkitShared::ParseUpdateCursorAction(const jsonxx::Object &param, bool &restMode, bool &chordMode)
{
    chordMode = false;
    restMode = false;

    if (param.has<jsonxx::Boolean>("chordMode"))
        chordMode = param.get<jsonxx::Boolean>("chordMode");
    else if (param.has<jsonxx::Boolean>("restMode"))
        restMode = param.get<jsonxx::Boolean>("restMode");

    return true;
}

bool EditorToolkitShared::ParseUpdatePitchAction(const jsonxx::Object &param, std::string &elementId,
    data_PITCHNAME &pname, int &oct, data_ACCIDENTAL_WRITTEN &accid, int &midi)
{
    pname = PITCHNAME_NONE;
    oct = VRV_UNSET;
    accid = ACCIDENTAL_WRITTEN_NONE;
    midi = VRV_UNSET;
    Note noteConverter;
    Accid accidConverter;

    if (!param.has<jsonxx::String>("elementId")) return false;
    elementId = param.get<jsonxx::String>("elementId");

    if (param.has<jsonxx::String>("pname"))
        pname = noteConverter.AttPitch::StrToPitchname(param.get<jsonxx::String>("pname"));
    if (param.has<jsonxx::Number>("oct")) oct = param.get<jsonxx::Number>("oct");
    if (param.has<jsonxx::String>("accid"))
        accid = accidConverter.AttAccidental::StrToAccidentalWritten(param.get<jsonxx::String>("accid"));
    if (param.has<jsonxx::Number>("midi")) midi = param.get<jsonxx::Number>("midi");
    LogInfo("%c %d %d", pname, oct, midi);

    return true;
}

void EditorToolkitShared::PrepareUndo()
{
    if (this->InsertMode()) return;

    // We already have a prepared undo - nothing to prepare
    if (m_undoPrepared) return;

    std::string state = this->GetCurrentState();
    m_undoStack.push_back(state);
    m_undoMemoryUsage += state.size();
    // When new edit happens, redo stack is cleared
    while (!m_redoStack.empty()) {
        m_undoMemoryUsage -= m_redoStack.back().size();
        m_redoStack.pop_back();
    }
    TrimUndoMemory();
    // Set the flag
    m_undoPrepared = true;
}

void EditorToolkitShared::SetEditStatus()
{
    m_editStatus.reset();
    m_editStatus.import("chainedId", m_chainedId);
    m_editStatus.import("canUndo", this->CanUndo());
    m_editStatus.import("canRedo", this->CanRedo());
    m_editStatus.import("isMensuralMusicOnly", m_doc->IsMensuralMusicOnly());
    m_editStatus.import("insertMode", this->InsertMode());
    if (!m_selectionId.empty()) {
        jsonxx::Object selection;
        selection.import("id", m_selectionId);
        selection.import("element", ObjectFactory::GetInstance()->GetClassName(m_selectionClassId));
        if (!m_selectionSecondaryId.empty()) selection.import("secondaryId", m_selectionSecondaryId);
        m_editStatus << "selection" << selection;
    }
    if (this->InsertMode()) {
        jsonxx::Object insertion;
        insertion.import("oct", (m_cursor->HasOct() ? static_cast<int>(m_cursor->GetOct()) : 4));
        insertion.import(
            "pname", (m_cursor->HasPname() ? m_cursor->AttPitch::PitchnameToStr(m_cursor->GetPname()) : "c"));
        insertion.import(
            "dur", (m_cursor->HasDur() ? m_cursor->AttDurationLog::DurationToStr(m_cursor->GetDur()) : "4"));
        insertion.import("dots", (m_cursor->HasDots()) ? m_cursor->GetDots() : 0);
        insertion.import("chordMode", (m_cursor->IsChordMode()));
        insertion.import("restMode", (m_cursor->IsRestMode()));
        insertion.import("accid",
            (m_cursor->HasAccid()
                    ? m_cursor->GetAccidElement()->AttAccidental::AccidentalWrittenToStr(m_cursor->GetAccid())
                    : ""));
        insertion.import("accidImplicit", m_cursor->IsAccidImplicit());
        m_editStatus << "insertion" << insertion;
    }
}

std::string EditorToolkitShared::GetCurrentState()
{
    MEIOutput meioutput(m_doc);
    meioutput.SetSerializing(true);
    meioutput.SetBasic(false);
    meioutput.SetScoreBasedMEI(false);
    return meioutput.Export();
}

bool EditorToolkitShared::ReloadState(const std::string &data)
{
    MEIInput meiinput(m_doc);
    meiinput.SetDeserializing(true);
    return meiinput.Import(data);
}

bool EditorToolkitShared::CanUndo() const
{
    return (!m_undoStack.empty());
}

bool EditorToolkitShared::CanRedo() const
{
    return (!m_redoStack.empty());
}

bool EditorToolkitShared::Undo()
{
    if (!CanUndo()) return false;

    std::string currentState = this->GetCurrentState();
    m_redoStack.push_back(currentState);

    // Pop the previous state from undo stack
    std::string previous = m_undoStack.back();
    m_undoStack.pop_back();

    return ReloadState(previous);
}

bool EditorToolkitShared::Redo()
{
    if (!CanRedo()) return false;

    std::string currentState = this->GetCurrentState();
    m_undoStack.push_back(currentState);

    // Pop redo state and load it
    std::string redoState = m_redoStack.back();
    m_redoStack.pop_back();

    return ReloadState(redoState);
}

void EditorToolkitShared::TrimUndoMemory()
{
    // Drop the oldest undo entries if we exceed the limit
    while ((m_undoMemoryUsage > UNDO_MEMORY_LIMIT) && !m_undoStack.empty()) {
        m_undoMemoryUsage -= m_undoStack.front().size();
        m_undoStack.pop_front();
    }
    LogInfo("Undo stack size: %dMB", m_undoMemoryUsage / 1024 / 1024);
}

bool EditorToolkitShared::Chain(const jsonxx::Array &actions)
{
    bool status = true;
    m_chainedId = "";
    for (int i = 0; i < (int)actions.size(); ++i) {
        status = this->ParseEditorAction(actions.get<jsonxx::Object>(i).json(), !status);
    }
    return status;
}

bool EditorToolkitShared::SetCursor(std::string &elementId, Cursor::InputMode inputMode, bool chordMode)
{
    Layer *layer = NULL;
    LayerElement *position = NULL;
    Object *element = this->ResolveElement(elementId);
    bool updateAccid = false;
    if (element->Is(STAFF)) {
        updateAccid = true;
        layer = vrv_cast<Layer *>(element->FindDescendantByType(LAYER));
    }
    else if (element->Is(LAYER)) {
        updateAccid = true;
        layer = vrv_cast<Layer *>(element);
    }
    else if (element->IsLayerElement()) {
        layer = vrv_cast<Layer *>(element->GetFirstAncestor(LAYER));
        position = vrv_cast<LayerElement *>(element);
    }
    CursorFunctor cursorFunctor(layer, position);
    m_doc->Process(cursorFunctor);
    m_cursor = cursorFunctor.GetCursor();

    // Get the accid from the layer key signature
    if (m_cursor) {
        if (chordMode && m_cursor->GetPosition() && m_cursor->GetPosition()->Is({ NOTE, CHORD })) {
            m_cursor->SetRestMode(false);
            m_cursor->SetChordMode(Cursor::ChordMode::EDIT_EXISTING);
        }
        if (updateAccid) {
            std::string placeholder;
            this->UpdatePitch(
                placeholder, m_cursor->GetPname(), m_cursor->GetOct(), ACCIDENTAL_WRITTEN_NONE, VRV_UNSET);
        }
    }

    this->SetEditStatus();

    return true;
}

bool EditorToolkitShared::UpdateCursor(bool restMode, bool chordMode)
{
    if (!InsertMode()) return true;

    if (chordMode) {
        m_cursor->SetRestMode(false);
        m_cursor->SetChordMode(Cursor::ChordMode::NEW);
    }
    else if (m_cursor->GetInputMode() == Cursor::PITCH_FIRST) {
        m_cursor->SetRestMode(restMode);
    }

    this->SetEditStatus();

    return true;
}

bool EditorToolkitShared::ResetCursor(bool maintainChordMode)
{
    if (this->InsertMode() && m_cursor->IsChordMode()) {
        if (m_cursor->GetPosition()) {
            // Use the bottom pitch
            if (m_cursor->GetPosition()->Is(CHORD)) {
                Chord *chord = vrv_cast<Chord *>(m_cursor->GetPosition());
                assert(chord);
                Note *note = chord->GetBottomNote();
                std::string placeholder;
                this->UpdatePitch(placeholder, note->GetPname(), note->GetOct(), ACCIDENTAL_WRITTEN_NONE, VRV_UNSET);
            }
            this->MoveCursor(m_cursor->GetPosition(), maintainChordMode);
        }
        if (m_cursor) {
            if (maintainChordMode) {
                this->UpdateCursor(false, true);
            }
            else {
                m_cursor->SetChordMode(Cursor::ChordMode::NONE);
            }
        }
        return true;
    }

    CursorFunctor cursorFunctor(NULL, NULL);
    m_doc->Process(cursorFunctor);
    m_cursor = NULL;

    this->SetEditStatus();

    return true;
}

bool EditorToolkitShared::Delete(std::string &elementId, bool backspace)
{
    if (this->InsertMode()) return true;

    Object *element = this->ResolveElement(elementId);

    if (!element) return false;

    int direction = (backspace) ? 37 : 39;
    this->Navigate(elementId, direction);
    if (m_chainedId.empty() && element->GetParent()) m_chainedId = element->GetParent()->GetID();

    // Find referring objects
    std::set<std::string> objectsToDelete;
    SetOfConstObjects visited;
    objectsToDelete.insert(element->GetID());
    this->CollectReferringObjects(element, objectsToDelete, visited);
    for (auto id : objectsToDelete) {
        Object *toDelete = m_doc->FindDescendantByID(id);
        if (toDelete && toDelete->GetParent()) toDelete->GetParent()->DeleteChild(toDelete);
    }

    if (!m_chainedId.empty() && !m_doc->FindDescendantByID(m_chainedId)) m_chainedId = "";

    this->ClearContext();
    this->SetEditStatus();
    return true;
}

void EditorToolkitShared::CollectReferringObjects(
    const Object *element, std::set<std::string> &objectsToDelete, SetOfConstObjects &visited)
{
    assert(element);

    if (visited.find(element) != visited.end()) return;
    visited.insert(element);

    // First check all children
    for (int i = 0; i < element->GetChildCount(); ++i) {
        const Object *child = element->GetChild(i);
        if (!child) continue;

        CollectReferringObjects(child, objectsToDelete, visited);
    }

    // Then find objects referring to this object
    ListOfObjectAttNamePairs referringObjects;
    FindAllReferringObjectsFunctor findAllReferringObjects(element, &referringObjects);
    m_doc->Process(findAllReferringObjects);

    for (ListOfObjectAttNamePairs::iterator it = referringObjects.begin(); it != referringObjects.end(); ++it) {
        const Object *referringObject = it->first;

        if (referringObject == NULL) continue;
        if (referringObject == element) continue;

        objectsToDelete.insert(referringObject->GetID());

        CollectReferringObjects(referringObject, objectsToDelete, visited);
    }
}

bool EditorToolkitShared::Drag(std::string &elementId, int x, int y)
{
    if (this->InsertMode()) return true;

    Object *element = this->ResolveElement(elementId);
    if (!element) return false;

    // For elements whose y-position corresponds to a certain pitch
    if (element->HasInterface(INTERFACE_PITCH)) {
        Layer *layer = vrv_cast<Layer *>(element->GetFirstAncestor(LAYER));
        if (!layer) return false;
        int oct;
        data_PITCHNAME pname
            = (data_PITCHNAME)m_view->CalculatePitchCode(layer, m_view->ToLogicalY(y), element->GetDrawingX(), &oct);
        element->GetPitchInterface()->SetPname(pname);
        element->GetPitchInterface()->SetOct(oct);

        return true;
    }
    return false;
}

bool EditorToolkitShared::InsertControl(std::string &elementName, std::string &startId, std::string &endId)
{
    if (this->InsertMode()) return true;

    Object *start = this->ResolveElement(startId, false);
    if (!start) return false;

    Measure *measure = vrv_cast<Measure *>(start->GetFirstAncestor(MEASURE));
    if (!measure) return false;

    Object *childElement = this->PrepareInsertion(measure, elementName);
    if (!childElement) return false;

    if (!measure->AddChild(childElement)) {
        delete childElement;
        return false;
    }

    TimePointInterface *timePointInterface = childElement->GetTimePointInterface();
    if (timePointInterface) timePointInterface->SetStartid("#" + startId);

    if (!endId.empty()) {
        Object *end = this->ResolveElement(endId, false);
        if (!end) return false;

        TimeSpanningInterface *timeSpanningInterface = childElement->GetTimeSpanningInterface();
        if (timeSpanningInterface && !endId.empty()) timeSpanningInterface->SetEndid("#" + endId);
    }

    return true;
}

bool EditorToolkitShared::KeyDown(std::string &elementId, int key, bool shiftKey, bool ctrlKey)
{
    Object *element = (m_cursor) ? m_cursor : this->ResolveElement(elementId);
    if (!element) return false;

    // For elements whose y-position corresponds to a certain pitch
    if (element->HasInterface(INTERFACE_PITCH)) {
        PitchInterface *interface = element->GetPitchInterface();
        assert(interface);
        int step;
        switch (key) {
            case KEY_UP: step = 1; break;
            case KEY_DOWN: step = -1; break;
            default: step = 0;
        }
        if (ctrlKey) step *= 7;
        interface->AdjustPitchByOffset(step);
        // This will reset the accidental also for octave shifts
        this->UpdatePitch(elementId, interface->GetPname(), interface->GetOct(), ACCIDENTAL_WRITTEN_NONE, VRV_UNSET);
    }
    if (element->HasInterface(INTERFACE_DURATION)) {
        DurationInterface *interface = element->GetDurationInterface();
        assert(interface);
        switch (key) {
            case KEY_LEFT: interface->DecreaseCMNDuration(); break;
            case KEY_RIGHT: interface->IncreaseCMNDuration(); break;
            case KEY_DOT: interface->HasDots() ? interface->ResetAugmentDots() : interface->SetDots(1);
            default: break;
        }
    }

    this->SetEditStatus();

    return true;
}

bool EditorToolkitShared::Navigate(std::string &elementId, const int &direction)
{
    static auto classIds = { CHORD, MREST, NOTE, REST };

    if (this->InsertMode()) return true;

    // disable horizontal navigation for now
    if (direction != 37 && direction != 39) return true;

    const bool forward = (direction == 39);

    m_chainedId = "";
    this->SetEditStatus();

    const Object *element = this->GetElement(elementId);
    if (!element) return false;

    const LayerElement *layerElement = dynamic_cast<const LayerElement *>(element);
    if (!layerElement) return true;

    const Layer *layer = vrv_cast<const Layer *>(layerElement->GetFirstAncestor(LAYER));
    if (!layer) return true;

    const LayerElement *result = layerElement;

    while (result) {
        // keycode left
        result = (forward) ? layer->GetNextInLayer(result) : layer->GetPreviousInLayer(result);

        if (!result || (layerElement->GetAlignment() == result->GetAlignment())) continue;

        if (result->IsAnyOf(classIds)) break;
    }

    if (!result) {
        ClassIdsComparison matches(classIds);
        if (forward) {
            layer = this->GetNextLayer(layer);
            if (!layer) return true;
            result = vrv_cast<const LayerElement *>(layer->FindDescendantByComparison(&matches));
        }
        else {
            layer = this->GetPreviousLayer(layer);
            if (!layer) return true;
            result = vrv_cast<const LayerElement *>(
                layer->FindDescendantByComparison(&matches, UNLIMITED_DEPTH, BACKWARD));
        }
    }

    if (result) {
        if (result->Is(NOTE)) {
            const Note *note = vrv_cast<const Note *>(result);
            assert(note);
            if (note->IsChordTone()) result = note->IsChordTone();
        }
        if (result->Is(CHORD)) {
            const Chord *chord = vrv_cast<const Chord *>(result);
            assert(chord);
            result = chord->GetTopNote();
        }
    }

    if (result) m_chainedId = result->GetID();

    this->SetEditStatus();
    return true;
}

bool EditorToolkitShared::Select(std::string &elementId, bool secondary)
{
    if (this->InsertMode()) return true;

    if (secondary) {
        m_selectionSecondaryId = "";
        if (!m_selectionId.empty() && Object::IsLayerElement(m_selectionClassId)) {
            Object *element = this->GetElement(elementId);
            if (!element) return false;
            m_selectionSecondaryId = elementId;
        }
    }
    else {
        m_selectionId = "";
        m_selectionClassId = UNSPECIFIED;
        m_selectionSecondaryId = "";

        Object *element = this->GetElement(elementId);
        if (!element) return false;

        m_selectionId = elementId;
        m_selectionClassId = element->GetClassId();
    }

    this->SetEditStatus();

    return true;
}

bool EditorToolkitShared::Set(std::string &elementId, const std::string &attribute, const std::string &value)
{
    static const std::array<const char *, 5> allowCursor = { "oct", "pname", "dots", "dur", "accid" };

    // Restrict set action on cursor
    if (this->InsertMode() && (std::find(allowCursor.begin(), allowCursor.end(), attribute) == allowCursor.end()))
        return true;

    Object *element = (m_cursor) ? m_cursor : this->ResolveElement(elementId);
    if (!element) return false;

    if (m_cursor && attribute == "accid") {
        element = m_cursor->GetAccidElement();
    }

    bool success = false;
    if (element->Is(TEXT) && (attribute == "text")) {
        Text *text = vrv_cast<Text *>(element);
        assert(text);
        text->SetText(UTF8to32(value));
        success = true;
    }
    else if (AttModule::SetAnalytical(element, attribute, value)) {
        success = true;
    }
    else if (AttModule::SetCmn(element, attribute, value)) {
        success = true;
    }
    else if (AttModule::SetCmnornaments(element, attribute, value)) {
        success = true;
    }
    else if (AttModule::SetCritapp(element, attribute, value)) {
        success = true;
    }
    else if (AttModule::SetEdittrans(element, attribute, value)) {
        success = true;
    }
    else if (AttModule::SetExternalsymbols(element, attribute, value)) {
        success = true;
    }
    else if (AttModule::SetFacsimile(element, attribute, value)) {
        success = true;
    }
    else if (AttModule::SetFigtable(element, attribute, value)) {
        success = true;
    }
    else if (AttModule::SetFingering(element, attribute, value)) {
        success = true;
    }
    else if (AttModule::SetGestural(element, attribute, value)) {
        success = true;
    }
    else if (AttModule::SetHarmony(element, attribute, value)) {
        success = true;
    }
    else if (AttModule::SetHeader(element, attribute, value)) {
        success = true;
    }
    else if (AttModule::SetMei(element, attribute, value)) {
        success = true;
    }
    else if (AttModule::SetMensural(element, attribute, value)) {
        success = true;
    }
    else if (AttModule::SetMidi(element, attribute, value)) {
        success = true;
    }
    else if (AttModule::SetNeumes(element, attribute, value)) {
        success = true;
    }
    else if (AttModule::SetPagebased(element, attribute, value)) {
        success = true;
    }
    else if (AttModule::SetPerformance(element, attribute, value)) {
        success = true;
    }
    else if (AttModule::SetShared(element, attribute, value)) {
        success = true;
    }
    else if (AttModule::SetStringtab(element, attribute, value)) {
        success = true;
    }
    else if (AttModule::SetUsersymbols(element, attribute, value)) {
        success = true;
    }
    else if (AttModule::SetVisual(element, attribute, value)) {
        success = true;
    }

    this->SetEditStatus();

    return success;
}

bool EditorToolkitShared::UpdatePitch(
    std::string &elementId, data_PITCHNAME pname, int oct, data_ACCIDENTAL_WRITTEN accid, int midi)
{
    Object *element = (m_cursor) ? m_cursor : this->ResolveElement(elementId);
    if (!element) return false;

    // For elements whose y-position corresponds to a certain pitch
    if (!element->HasInterface(INTERFACE_PITCH)) return true;

    const Layer *layer = vrv_cast<const Layer *>(element->GetFirstAncestor(LAYER));
    assert(layer);

    PitchInterface *interface = element->GetPitchInterface();

    if (pname != PITCHNAME_NONE) {
        interface->SetPname(pname);
        if (oct != VRV_UNSET) interface->SetOct(oct);
        if (m_cursor) m_cursor->SetAccid(accid);
    }
    else if (midi != VRV_UNSET) {
        data_KEYSIGNATURE keySig;
        if (layer->GetCurrentKeySig()) keySig = layer->GetCurrentKeySig()->ConvertToSig();
        MidiSpelling spelling = this->SpellMidi(midi, keySig);
        interface->SetPname(spelling.pname);
        accid = spelling.accid;
        if (accid == ACCIDENTAL_WRITTEN_NONE) accid = ACCIDENTAL_WRITTEN_n;
        interface->SetOct(midi / 12 - 1);
    }

    if (!m_cursor) {
        Accid *accid = vrv_cast<Accid *>(element->FindDescendantByType(ACCID, 1));
        element->DeleteChild(accid);
    }
    else {
        m_cursor->SetAccid(ACCIDENTAL_WRITTEN_NONE);
    }

    const LayerElement *reference = vrv_cast<const LayerElement *>(element);
    assert(reference);
    data_ACCIDENTAL_WRITTEN previousAccid = this->GetAccidBefore(
        (m_cursor ? m_cursor->GetPosition() : reference), interface->GetPname(), interface->GetOct());

    if (previousAccid == ACCIDENTAL_WRITTEN_NONE) {
        MapOfOctavedPitchAccid currentAccids;
        if (layer->GetCurrentKeySig()) layer->GetCurrentKeySig()->FillMap(currentAccids);
        const int octavedPitch = interface->GetPname() + interface->GetOct() * 7;
        previousAccid = currentAccids.contains(octavedPitch) ? currentAccids.at(octavedPitch) : ACCIDENTAL_WRITTEN_NONE;
    }

    data_ACCIDENTAL_WRITTEN actualAccid = ACCIDENTAL_WRITTEN_NONE;
    bool isImplicit = true;
    if (accid != ACCIDENTAL_WRITTEN_NONE) {
        if (accid != ACCIDENTAL_WRITTEN_n || previousAccid != ACCIDENTAL_WRITTEN_NONE) {
            actualAccid = accid;
            isImplicit = (accid == previousAccid);
        }
    }
    else if (previousAccid != ACCIDENTAL_WRITTEN_NONE) {
        actualAccid = previousAccid;
    }
    if (actualAccid != ACCIDENTAL_WRITTEN_NONE) {
        if (m_cursor) {
            m_cursor->SetAccid(actualAccid);
            m_cursor->SetAccidImplicit(isImplicit);
        }
        else if (element->IsSupportedChild(ACCID)) {
            Accid *accidElement = new Accid();
            if (isImplicit) {
                accidElement->SetAccidGes(Att::AccidentalWrittenToGestural(actualAccid));
            }
            else {
                accidElement->SetAccid(actualAccid);
            }
            element->AddChild(accidElement);
            this->ClearContext();
        }
    }

    this->SetEditStatus();

    return true;
}

data_ACCIDENTAL_WRITTEN EditorToolkitShared::GetAccidBefore(const LayerElement *element, data_PITCHNAME pname, int oct)
{
    if (!element) return ACCIDENTAL_WRITTEN_NONE;

    const Layer *layer = vrv_cast<const Layer *>(element->GetFirstAncestor(LAYER));
    assert(layer);
    ListOfConstObjects notes = layer->FindAllDescendantsByType(NOTE);
    bool breakAtNext = false;
    data_ACCIDENTAL_WRITTEN previous = ACCIDENTAL_WRITTEN_NONE;
    for (auto &object : notes) {
        const Note *note = vrv_cast<const Note *>(object);
        assert(note);
        // Still go through all the notes because it might be a chord
        if (element->GetAlignment() == note->GetAlignment()) {
            breakAtNext = true;
        }
        else if (breakAtNext) {
            break;
        }
        if (note->GetPname() != pname || note->GetOct() != oct) continue;
        const Accid *accid = note->GetDrawingAccid();
        if (accid) {
            previous
                = (accid->HasAccidGes()) ? Att::AccidentalGesturalToWritten(accid->GetAccidGes()) : accid->GetAccid();
        }
    }

    return previous;
}

bool EditorToolkitShared::ContextForScores(bool updateResponse)
{
    if (!m_scoreContext) {
        m_scoreContext = new EditorTreeObject(m_doc, false);
        ScoreContextFunctor scoreContextFunctor(m_scoreContext);
        m_doc->Process(scoreContextFunctor);
    }
    m_currentContext = m_scoreContext;

    if (!updateResponse) return true;

    m_editResponse.reset();

    // The target object
    jsonxx::Object jsonObject;
    this->ContextForObject(m_scoreContext, jsonObject, true);

    m_editResponse = jsonObject;

    return true;
}

bool EditorToolkitShared::ContextForSections(bool updateResponse)
{
    if (!m_sectionContext) {
        m_sectionContext = new EditorTreeObject(m_doc, false);
        SectionContextFunctor sectionContextFunctor(m_sectionContext);
        m_doc->Process(sectionContextFunctor);
    }
    m_currentContext = m_sectionContext;

    if (!updateResponse) return true;

    m_editResponse.reset();

    // The target object
    jsonxx::Object jsonObject;
    this->ContextForObject(m_sectionContext, jsonObject, true);

    m_editResponse = jsonObject;

    return true;
}

void EditorToolkitShared::ClearContext()
{
    if (m_sectionContext) {
        delete m_sectionContext;
        m_sectionContext = NULL;
    }
}

bool EditorToolkitShared::ContextForElement(std::string &elementId)
{
    m_editResponse.reset();

    // Make sure we have a section tree - this also sets m_currentContext
    this->ContextForSections(false);
    assert(m_sectionContext);

    bool hasTargetID = (elementId != "");
    const Object *object = NULL;
    if (hasTargetID) {
        object = this->ResolveElement(elementId);
    }
    // Retrieve the context from the first measure in the document
    else {
        object = m_doc->FindDescendantByType(MEASURE);
    }
    // We cannot continue without object
    if (!object || !object->GetParent()) return false;

    // Keep a pointer to the orignal object for the attributes
    const Object *originalObject = object;
    ArrayOfConstObjects siblings;
    ArrayOfConstObjects::iterator targetIt;

    const Object *contextRoot = NULL;

    // If the object parent (context root) is a sytem, this means it must be selected from the MEI section context tree
    // - and so must its siblings
    if (object->GetParent()->Is(SYSTEM)) {
        const Object *editorTreeObject = m_sectionContext->FindDescendantByID(object->GetID());
        if (!editorTreeObject) {
            return false;
        }
        // If the object is a milestone, the we must look for it in the editor tree
        if (object->IsMilestoneElement()) object = editorTreeObject;
        contextRoot = editorTreeObject->GetParent();
        siblings = this->GetScoreBasedChildrenFor(contextRoot);
        targetIt = std::find(siblings.begin(), siblings.end(), object);
        // It is not found in the siblings, something is wrong
        if (targetIt == siblings.end()) return false;
    }
    else {
        contextRoot = object->GetParent();
        siblings = object->GetParent()->GetChildren();
        targetIt = std::find(siblings.begin(), siblings.end(), object);
        // This should not happen
        if (targetIt == siblings.end()) return false;
    }
    assert(contextRoot);

    ArrayOfConstObjects previousSiblings;
    if (targetIt != siblings.begin()) std::copy(siblings.begin(), targetIt, std::back_inserter(previousSiblings));

    ArrayOfConstObjects followingSiblings;
    if (targetIt != siblings.end())
        std::copy(std::next(targetIt), siblings.end(), std::back_inserter(followingSiblings));

    ArrayOfConstObjects ancestors;
    // Reserved size for optimizing loop filling
    ancestors.reserve(10);
    jsonxx::Array jsonAncestors;

    // Look for ancestors starting from the object parent
    const Object *current = object;
    while (current->GetParent()) {
        if (current->GetParent()->Is(SYSTEM)) {
            // Switch to the MEI sectionContext tree
            current = m_sectionContext->FindDescendantByID(current->GetID());
            if (!current || !current->GetParent()) return false;
        }
        // For non-measured music, skip the parent measure object
        if (current->GetParent()->Is(MEASURE)) {
            const Measure *measure = vrv_cast<const Measure *>(current->GetParent());
            assert(measure);
            if (!measure->IsMeasuredMusic()) {
                current = current->GetParent();
                continue;
            }
        }
        // Top element in the score subtree
        if (current->GetParent()->Is(SCORE)) break;
        current = current->GetParent();
        ancestors.push_back(current);
    }
    this->ContextForObjects(ancestors, jsonAncestors);
    m_editResponse << "ancestors" << jsonAncestors;

    jsonxx::Object jsonContextRoot;
    this->ContextForObject(contextRoot, jsonContextRoot);
    jsonxx::Array jsonContext;

    // Preceeding siblings
    jsonxx::Array elements;
    this->ContextForObjects(previousSiblings, elements);
    jsonContext << elements;

    // The target object
    jsonxx::Object jsonObject;
    this->ContextForObject(object, jsonObject);
    // Include its children, but only if we specified a target ID
    if (hasTargetID) {
        jsonxx::Array jsonObjectChildren;
        ArrayOfConstObjects objectChildren;
        if (dynamic_cast<const EditorTreeObject *>(object)) {
            objectChildren = this->GetScoreBasedChildrenFor(object);
        }
        else {
            objectChildren = object->GetChildren();
        }
        this->ContextForObjects(objectChildren, jsonObjectChildren);
        if (!jsonObjectChildren.empty()) jsonObject << "children" << jsonObjectChildren;
    }
    // Add it to the list
    jsonContext << jsonObject;

    // Following siblings
    this->ContextForObjects(followingSiblings, elements);
    jsonContext << elements;

    // Add all children of to context (include target and surrounding siblings)
    jsonContextRoot << "children" << jsonContext;
    m_editResponse << "context" << jsonContextRoot;

    // Stop here without targetID, but still add empty objects or arrays to the info
    if (!hasTargetID) {
        m_editResponse << "object" << jsonxx::Object();
        m_editResponse << "referringElements" << jsonxx::Array();
        m_editResponse << "referencedElements" << jsonxx::Array();
        return true;
    }

    // Inlude all attributes
    ArrayOfStrAttr attributes;
    originalObject->GetAttributes(&attributes);
    jsonxx::Object jsonAttributes;
    for (const auto &attribute : attributes) {
        jsonAttributes << attribute.first << attribute.second;
    }
    jsonObject << "attributes" << jsonAttributes;
    std::string textStr;
    if (!dynamic_cast<const EditorTreeObject *>(object) && object->Is(TEXT)) {
        const Text *text = vrv_cast<const Text *>(object);
        assert(text);
        jsonObject << "text" << UTF32to8(text->GetText());
    }
    m_editResponse << "object" << jsonObject;

    // Find referring objects
    ListOfObjectAttNamePairs referringObjects;
    FindAllReferringObjectsFunctor findAllReferringObjects(object, &referringObjects);
    m_doc->Process(findAllReferringObjects);
    this->ContextForReferences(referringObjects, elements);
    m_editResponse << "referringElements" << elements;

    // Find referenced objects
    ListOfObjectAttNamePairs referencedObjects;
    FindAllReferencedObjectsFunctor findAllReferencedObjects(NULL, &referencedObjects);
    object->Process(findAllReferencedObjects, 0);
    this->ContextForReferences(referencedObjects, elements);
    m_editResponse << "referencedElements" << elements;

    return true;
}

void EditorToolkitShared::ContextForObject(const Object *object, jsonxx::Object &element, bool recursive)
{
    element << "element" << object->GetClassName();
    element << "id" << object->GetID();
    jsonxx::Object attributes;
    if (object->HasAttClass(ATT_NINTEGER)) {
        const AttNInteger *att = dynamic_cast<const AttNInteger *>(object);
        assert(att);
        attributes << "n" << att->GetN();
    }
    if (object->HasAttClass(ATT_NNUMBERLIKE)) {
        const AttNNumberLike *att = dynamic_cast<const AttNNumberLike *>(object);
        assert(att);
        attributes << "n" << att->GetN();
    }
    if (!attributes.empty()) {
        element << "attributes" << attributes;
    }

    ArrayOfConstObjects children;
    // First check that this is an EditorTreeObject
    if (dynamic_cast<const EditorTreeObject *>(object)) {
        children = this->GetScoreBasedChildrenFor(object);
    }
    else {
        children = object->GetChildren();
    }
    // Remove children that are added as element parts (never exist in EditorTreeObject)
    children.erase(std::remove_if(children.begin(), children.end(),
                       [](const Object *item) {
                           return item->IsAnyOf(std::array{ DOTS, FLAG, STEM, TUPLET_NUM, TUPLET_BRACKET });
                       }),
        children.end());

    if (children.size() > 0) {
        // If we do not call it recusrively, still include an empty array
        jsonxx::Array jsonChildren;
        if (recursive) {
            for (auto child : children) {
                jsonxx::Object jsonChild;
                this->ContextForObject(child, jsonChild, true);
                jsonChildren << jsonChild;
            }
        }
        element << "children" << jsonChildren;
    }
    else {
        element << "isLeaf" << true;
    }
}

void EditorToolkitShared::ContextForObjects(const ArrayOfConstObjects &objects, jsonxx::Array &elements)
{
    elements.reset();

    for (const Object *object : objects) {
        if (object->Is(MNUM)) {
            const MNum *mNum = vrv_cast<const MNum *>(object);
            assert(mNum);
            if (mNum->IsGenerated()) continue;
        }
        if (object->IsAttribute()) continue;
        if (object->IsAnyOf(std::array{ DOTS, FLAG, STEM, TUPLET_NUM, TUPLET_BRACKET })) continue;

        jsonxx::Object element;
        this->ContextForObject(object, element);
        elements << element;
    }
}

void EditorToolkitShared::ContextForReferences(
    const ListOfObjectAttNamePairs &objectAttNames, jsonxx::Array &references)
{
    references.reset();

    for (auto &objectAttName : objectAttNames) {
        jsonxx::Object element;
        this->ContextForObject(objectAttName.first, element);
        element << "referenceAttribute" << objectAttName.second;
        references << element;
    }
}

ArrayOfConstObjects EditorToolkitShared::GetScoreBasedChildrenFor(const Object *object)
{
    // m_currentContext is set by ContextForScores or ContextForSections
    assert(m_currentContext);
    const EditorTreeObject *editorTreeObject = (m_currentContext->GetID() == object->GetID())
        ? vrv_cast<const EditorTreeObject *>(object)
        : vrv_cast<const EditorTreeObject *>(m_currentContext->FindDescendantByID(object->GetID()));
    if (!editorTreeObject) {
        return ArrayOfConstObjects();
    }
    return editorTreeObject->GetChildObjects();
}

bool EditorToolkitShared::GetScoreDef()
{
    m_editResponse.reset();

    MEIOutputExtended output(m_doc);

    m_editResponse = output.ExportScoreDef();

    return true;
}

bool EditorToolkitShared::SetScoreDef(const std::string scoreDef)
{
    return true;
}

void EditorToolkitShared::MoveCursor(LayerElement *element, bool maintainChordMode)
{
    assert(m_cursor);

    if (!element) return;

    const Object *object = element;

    Layer *layer = vrv_cast<Layer *>(element->GetFirstAncestor(LAYER));
    assert(layer);

    ClassIdsComparison comparison({ CHORD, NOTE, REST });

    if (m_cursor->GetChordMode() == Cursor::ChordMode::NEW) {
        m_cursor->SetChordMode(Cursor::ChordMode::EDIT_NEW);
    }
    // Last element in the layer, check if we need to move to the next measure (or exit inputMode)
    else if (element == layer->FindDescendantByComparison(&comparison, UNLIMITED_DEPTH, BACKWARD)) {
        AlignMeterParams params;
        params.meterSig = layer->GetCurrentMeterSig();
        Fraction position = (m_cursor->GetAlignment()) ? m_cursor->GetAlignment()->GetTime() : 0;
        // Duration of the chord in chord editing mode is already included in the alignment
        Fraction duration
            = (m_cursor->IsChordEditMode()) ? 0 : element->GetAlignmentDuration(params, true, NOTATIONTYPE_cmn);
        Fraction measureDuration = Fraction(params.meterSig->GetUnitAsDur()) * params.meterSig->GetTotalCount();
        // Assume 4/4 by default
        if (measureDuration == 0) measureDuration = 4;
        if ((position + duration) >= measureDuration) {
            object = this->GetNextLayer(layer);
            m_cursor->SetAccidImplicit(false);
        }
    }

    if (object) {
        m_selectionId = object->GetID();
        m_chainedId = m_selectionId;
        m_selectionClassId = object->GetClassId();
        this->SetCursor(m_selectionId, m_cursor->GetInputMode(), false);
    }
    else {
        // Exit inputMode
        m_cursor->SetChordMode(Cursor::ChordMode::NONE);
        this->ResetCursor(false);
    }
}

const Measure *EditorToolkitShared::GetPreviousMeasure(const Measure *measure)
{
    const System *system = vrv_cast<const System *>(measure->GetFirstAncestor(SYSTEM));
    assert(system);

    const Measure *previousMeasure = vrv_cast<const Measure *>(system->GetPrevious(measure, MEASURE));
    if (previousMeasure) return previousMeasure;

    const Page *page = vrv_cast<const Page *>(system->GetFirstAncestor(PAGE));
    assert(page);

    const System *previousSystem = vrv_cast<const System *>(page->GetPrevious(system, SYSTEM));
    if (!previousSystem) {
        const Page *previousPage = vrv_cast<const Page *>(m_doc->GetPages()->GetPrevious(page, PAGE));
        if (!previousPage) return NULL;

        previousSystem = vrv_cast<const System *>(previousPage->GetLast(SYSTEM));
        if (!previousSystem) return NULL;
    }

    return vrv_cast<const Measure *>(previousSystem->GetLast(MEASURE));
}

const Staff *EditorToolkitShared::GetPreviousStaff(const Staff *staff)
{
    const Measure *measure = vrv_cast<const Measure *>(staff->GetFirstAncestor(MEASURE));
    assert(measure);

    const Measure *previousMeasure = GetPreviousMeasure(measure);
    if (!previousMeasure) return NULL;

    AttNIntegerComparison staffNComparison(STAFF, staff->GetN());
    return vrv_cast<const Staff *>(previousMeasure->FindDescendantByComparison(&staffNComparison));
}

const Layer *EditorToolkitShared::GetPreviousLayer(const Layer *layer)
{
    const Staff *staff = vrv_cast<const Staff *>(layer->GetFirstAncestor(STAFF));
    assert(staff);

    const Staff *previousStaff = GetPreviousStaff(staff);
    if (!previousStaff) return NULL;

    AttNIntegerComparison layerNComparison(LAYER, layer->GetN());
    return vrv_cast<const Layer *>(previousStaff->FindDescendantByComparison(&layerNComparison));
}

const Measure *EditorToolkitShared::GetNextMeasure(const Measure *measure)
{
    const System *system = vrv_cast<const System *>(measure->GetFirstAncestor(SYSTEM));
    assert(system);

    const Measure *nextMeasure = vrv_cast<const Measure *>(system->GetNext(measure, MEASURE));
    if (nextMeasure) return nextMeasure;

    const Page *page = vrv_cast<const Page *>(system->GetFirstAncestor(PAGE));
    assert(page);

    const System *nextSystem = vrv_cast<const System *>(page->GetNext(system, SYSTEM));
    if (!nextSystem) {
        const Page *nextPage = vrv_cast<const Page *>(m_doc->GetPages()->GetNext(page, PAGE));
        if (!nextPage) return NULL;

        nextSystem = vrv_cast<const System *>(nextPage->GetFirst(SYSTEM));
        if (!nextSystem) return NULL;
    }
    return vrv_cast<const Measure *>(nextSystem->GetFirst(MEASURE));
}

const Staff *EditorToolkitShared::GetNextStaff(const Staff *staff)
{
    const Measure *measure = vrv_cast<const Measure *>(staff->GetFirstAncestor(MEASURE));
    assert(measure);

    const Measure *nextMeasure = GetNextMeasure(measure);
    if (!nextMeasure) return NULL;

    AttNIntegerComparison staffNComparison(STAFF, staff->GetN());
    return vrv_cast<const Staff *>(nextMeasure->FindDescendantByComparison(&staffNComparison));
}

const Layer *EditorToolkitShared::GetNextLayer(const Layer *layer)
{
    const Staff *staff = vrv_cast<const Staff *>(layer->GetFirstAncestor(STAFF));
    assert(staff);

    const Staff *nextStaff = GetNextStaff(staff);
    if (!nextStaff) return NULL;

    AttNIntegerComparison layerNComparison(LAYER, layer->GetN());
    return vrv_cast<const Layer *>(nextStaff->FindDescendantByComparison(&layerNComparison));
}

EditorToolkitShared::MidiSpelling EditorToolkitShared::SpellMidi(int midi, const data_KEYSIGNATURE &keySig)
{
    static constexpr EditorToolkitShared::MidiSpelling sharpTable[12]
        = { { PITCHNAME_c, ACCIDENTAL_WRITTEN_NONE }, { PITCHNAME_c, ACCIDENTAL_WRITTEN_s },
              { PITCHNAME_d, ACCIDENTAL_WRITTEN_NONE }, { PITCHNAME_d, ACCIDENTAL_WRITTEN_s },
              { PITCHNAME_e, ACCIDENTAL_WRITTEN_NONE }, { PITCHNAME_f, ACCIDENTAL_WRITTEN_NONE },
              { PITCHNAME_f, ACCIDENTAL_WRITTEN_s }, { PITCHNAME_g, ACCIDENTAL_WRITTEN_NONE },
              { PITCHNAME_g, ACCIDENTAL_WRITTEN_s }, { PITCHNAME_a, ACCIDENTAL_WRITTEN_NONE },
              { PITCHNAME_a, ACCIDENTAL_WRITTEN_s }, { PITCHNAME_b, ACCIDENTAL_WRITTEN_NONE } };

    static constexpr EditorToolkitShared::MidiSpelling flatTable[12]
        = { { PITCHNAME_c, ACCIDENTAL_WRITTEN_NONE }, { PITCHNAME_d, ACCIDENTAL_WRITTEN_f },
              { PITCHNAME_d, ACCIDENTAL_WRITTEN_NONE }, { PITCHNAME_e, ACCIDENTAL_WRITTEN_f },
              { PITCHNAME_e, ACCIDENTAL_WRITTEN_NONE }, { PITCHNAME_f, ACCIDENTAL_WRITTEN_NONE },
              { PITCHNAME_f, ACCIDENTAL_WRITTEN_s }, { PITCHNAME_g, ACCIDENTAL_WRITTEN_NONE },
              { PITCHNAME_a, ACCIDENTAL_WRITTEN_f }, { PITCHNAME_a, ACCIDENTAL_WRITTEN_NONE },
              { PITCHNAME_b, ACCIDENTAL_WRITTEN_f }, { PITCHNAME_b, ACCIDENTAL_WRITTEN_NONE } };

    int pc = ((midi % 12) + 12) % 12;
    assert(pc >= 0 && pc > 12);

    bool flatKey = keySig.second == ACCIDENTAL_WRITTEN_f;

    return flatKey ? flatTable[pc] : sharpTable[pc];
}

//----------------------------------------------------------------------------
// EditorTreeObject
//----------------------------------------------------------------------------

EditorTreeObject::EditorTreeObject(const Object *object, bool ownChildren)
    : Object(object->GetClassId()), VisibilityDrawingInterface()
{
    this->Reset();

    this->SetID(object->GetID());
    m_className = object->GetClassName();
    if (this->IsEditorialElement() || this->Is(MDIV) || this->IsSystemElement()) {
        const VisibilityDrawingInterface *interface = object->GetVisibilityDrawingInterface();
        assert(interface);
        //  If we keep them hidden, then other functors will no process them.
        this->SetVisibility(interface->IsHidden() ? Hidden : Visible);
        // this->SetVisibility(Visible);
    }
    m_object = (ownChildren) ? object : NULL;
}

void EditorTreeObject::Reset()
{
    Object::Reset();
    VisibilityDrawingInterface::Reset();
}

ArrayOfConstObjects EditorTreeObject::GetChildObjects() const
{
    ArrayOfConstObjects childObjects;
    childObjects.reserve(this->GetChildCount());
    for (auto child : this->GetChildren()) {
        const EditorTreeObject *editorTreeChild = vrv_cast<const EditorTreeObject *>(child);
        // For non-measured music, get the measure object children
        if (editorTreeChild->Is(MEASURE) && editorTreeChild->m_object) {
            const Measure *measure = vrv_cast<const Measure *>(editorTreeChild->m_object);
            assert(measure);
            if (!measure->IsMeasuredMusic()) {
                ArrayOfConstObjects measureChildren = measure->GetChildren();
                std::copy(measureChildren.begin(), measureChildren.end(), std::back_inserter(childObjects));
                return childObjects;
            }
        }
        childObjects.push_back((editorTreeChild->m_object ? editorTreeChild->m_object : editorTreeChild));
    }

    return childObjects;
}

#endif /* NO_EDIT_SUPPORT */

} // namespace vrv

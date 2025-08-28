/////////////////////////////////////////////////////////////////////////////
// Name:        editortoolkit_cmn.cpp
// Author:      Laurent Pugin, Juliette Regimbal, Zoe McLennan
// Created:     04/06/2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "editortoolkit_cmn.h"

//--------------------------------------------------------------------------------

#include <locale>
#include <set>

//--------------------------------------------------------------------------------

#include "chord.h"
#include "clef.h"
#include "comparison.h"
#include "dir.h"
#include "dynam.h"
#include "editfunctor.h"
#include "editorial.h"
#include "findfunctor.h"
#include "hairpin.h"
#include "iomei.h"
#include "layer.h"
#include "mdiv.h"
#include "measure.h"
#include "mnum.h"
#include "note.h"
#include "page.h"
#include "plistinterface.h"
#include "rend.h"
#include "rest.h"
#include "slur.h"
#include "staff.h"
#include "surface.h"
#include "symboldef.h"
#include "systemelement.h"
#include "text.h"
#include "tie.h"
#include "timeinterface.h"
#include "vrv.h"
#include "zone.h"

//--------------------------------------------------------------------------------

#define CHAINED_ID "[chained-id]"

#define UNDO_MEMORY_LIMIT (256 * 1024 * 1024) // 256 MB

namespace vrv {

EditorToolkitCMN::EditorToolkitCMN(Doc *doc, View *view) : EditorToolkit(doc, view)
{
    m_undoPrepared = false;
    m_scoreContext = NULL;
    m_sectionContext = NULL;
    m_currentContext = NULL;
}

EditorToolkitCMN::~EditorToolkitCMN()
{
#ifndef NO_EDIT_SUPPORT
    this->ClearContext();
#endif
}

std::string EditorToolkitCMN::EditInfo()
{
    return m_editInfo.json();
}

bool EditorToolkitCMN::ParseEditorAction(const std::string &json_editorAction, bool commitOnly)
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

    if (action != "context") {
        m_doc->SetFocus();
    }

    // Action without parameter
    if (action == "commit") {
        m_doc->PrepareData();
        m_doc->ScoreDefSetCurrentDoc(true);
        m_doc->RefreshLayout();
        m_undoPrepared = false;
        m_editInfo.reset();
        m_editInfo.import("chainedId", m_chainedId);
        m_editInfo.import("canUndo", this->CanUndo());
        m_editInfo.import("canRedo", this->CanRedo());
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
        m_editInfo.reset();
        m_editInfo.import("chainedId", m_chainedId);
        m_editInfo.import("canUndo", this->CanUndo());
        m_editInfo.import("canRedo", this->CanRedo());
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
        if (this->ParseDeleteAction(json.get<jsonxx::Object>("param"), elementId)) {
            this->PrepareUndo();
            return (this->Delete(elementId));
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
    else if (action == "insert") {
        std::string elementType, startid, endid;
        if (this->ParseInsertAction(json.get<jsonxx::Object>("param"), elementType, startid, endid)) {
            this->PrepareUndo();
            if (endid == "") {
                return (this->Insert(elementType, startid));
            }
            else {
                return (this->Insert(elementType, startid, endid));
            }
        }
        LogWarning("Could not parse the insert action");
    }
    else if (action == "set") {
        std::string elementId, attribute, value;
        if (this->ParseSetAction(json.get<jsonxx::Object>("param"), elementId, attribute, value)) {
            this->PrepareUndo();
            return (this->Set(elementId, attribute, value));
        }
        LogWarning("Could not parse the set action");
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
bool EditorToolkitCMN::ParseContextAction(jsonxx::Object param, std::string &elementId, bool &scores, bool &sections)
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

bool EditorToolkitCMN::ParseDeleteAction(jsonxx::Object param, std::string &elementId)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    elementId = param.get<jsonxx::String>("elementId");
    return true;
}

bool EditorToolkitCMN::ParseDragAction(jsonxx::Object param, std::string &elementId, int &x, int &y)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    elementId = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::Number>("x")) return false;
    x = param.get<jsonxx::Number>("x");
    if (!param.has<jsonxx::Number>("y")) return false;
    y = param.get<jsonxx::Number>("y");
    return true;
}

bool EditorToolkitCMN::ParseInsertAction(
    jsonxx::Object param, std::string &elementType, std::string &startid, std::string &endid)
{
    // assign optional member
    endid = "";

    if (!param.has<jsonxx::String>("elementType")) return false;
    elementType = param.get<jsonxx::String>("elementType");
    if (!param.has<jsonxx::String>("startid")) return false;
    startid = param.get<jsonxx::String>("startid");
    // optional
    if (param.has<jsonxx::String>("endid")) {
        endid = param.get<jsonxx::String>("endid");
    }
    return true;
}

bool EditorToolkitCMN::ParseKeyDownAction(
    jsonxx::Object param, std::string &elementId, int &key, bool &shiftKey, bool &ctrlKey)
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

bool EditorToolkitCMN::ParseSetAction(
    jsonxx::Object param, std::string &elementId, std::string &attribute, std::string &value)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    elementId = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::String>("attribute")) return false;
    attribute = param.get<jsonxx::String>("attribute");
    if (!param.has<jsonxx::String>("value")) return false;
    value = param.get<jsonxx::String>("value");
    return true;
}

void EditorToolkitCMN::PrepareUndo()
{
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

std::string EditorToolkitCMN::GetCurrentState()
{
    MEIOutput meioutput(m_doc);
    meioutput.SetSerializing(true);
    meioutput.SetBasic(false);
    meioutput.SetScoreBasedMEI(false);
    return meioutput.Export();
}

bool EditorToolkitCMN::ReloadState(const std::string &data)
{
    MEIInput meiinput(m_doc);
    meiinput.SetDeserializing(true);
    return meiinput.Import(data);
}

bool EditorToolkitCMN::CanUndo() const
{
    return (!m_undoStack.empty());
}

bool EditorToolkitCMN::CanRedo() const
{
    return (!m_redoStack.empty());
}

bool EditorToolkitCMN::Undo()
{
    if (!CanUndo()) return false;

    std::string currentState = this->GetCurrentState();
    m_redoStack.push_back(currentState);

    // Pop the previous state from undo stack
    std::string previous = m_undoStack.back();
    m_undoStack.pop_back();

    return ReloadState(previous);
}

bool EditorToolkitCMN::Redo()
{
    if (!CanRedo()) return false;

    std::string currentState = this->GetCurrentState();
    m_undoStack.push_back(currentState);

    // Pop redo state and load it
    std::string redoState = m_redoStack.back();
    m_redoStack.pop_back();

    return ReloadState(redoState);
}

void EditorToolkitCMN::TrimUndoMemory()
{
    // Drop the oldest undo entries if we exceed the limit
    while ((m_undoMemoryUsage > UNDO_MEMORY_LIMIT) && !m_undoStack.empty()) {
        m_undoMemoryUsage -= m_undoStack.front().size();
        m_undoStack.pop_front();
    }
    LogInfo("Undo stack size: %dMB", m_undoMemoryUsage / 1024 / 1024);
}

bool EditorToolkitCMN::Chain(jsonxx::Array actions)
{
    bool status = true;
    m_chainedId = "";
    for (int i = 0; i < (int)actions.size(); ++i) {
        status = this->ParseEditorAction(actions.get<jsonxx::Object>(i).json(), !status);
    }
    return status;
}

bool EditorToolkitCMN::Delete(std::string &elementId)
{
    Object *element = this->GetElement(elementId);
    if (!element) return false;
    if (element->Is(NOTE)) {
        return this->DeleteNote(vrv_cast<Note *>(element));
    }
    return false;
}

bool EditorToolkitCMN::Drag(std::string &elementId, int x, int y)
{
    Object *element = this->GetElement(elementId);
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

bool EditorToolkitCMN::KeyDown(std::string &elementId, int key, bool shiftKey, bool ctrlKey)
{
    Object *element = this->GetElement(elementId);
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
        interface->AdjustPitchByOffset(step);
        return true;
    }
    return false;
}

bool EditorToolkitCMN::Insert(std::string &elementType, std::string const &startid, std::string const &endid)
{
    if (!m_doc->GetDrawingPage()) return false;

    Object *start = m_doc->GetDrawingPage()->FindDescendantByID(startid);
    Object *end = m_doc->GetDrawingPage()->FindDescendantByID(endid);
    // Check if both start and end elements exist
    if (!start || !end) {
        LogInfo("Elements start and end ids '%s' and '%s' could not be found", startid.c_str(), endid.c_str());
        return false;
    }
    // Check if it is a LayerElement
    if (!dynamic_cast<LayerElement *>(start)) {
        LogInfo("Element '%s' is not supported as start element", start->GetClassName().c_str());
        return false;
    }
    if (!dynamic_cast<LayerElement *>(end)) {
        LogInfo("Element '%s' is not supported as end element", start->GetClassName().c_str());
        return false;
    }

    Measure *measure = vrv_cast<Measure *>(start->GetFirstAncestor(MEASURE));
    assert(measure);

    ControlElement *element = NULL;
    if (elementType == "slur") {
        element = new Slur();
    }
    else if (elementType == "tie") {
        element = new Tie();
    }
    else if (elementType == "hairpin") {
        element = new Hairpin();
    }
    else {
        LogInfo("Inserting control event '%s' is not supported", elementType.c_str());
        return false;
    }

    assert(element);
    TimeSpanningInterface *interface = element->GetTimeSpanningInterface();
    assert(interface);
    measure->AddChild(element);
    interface->SetStartid("#" + startid);
    interface->SetEndid("#" + endid);

    m_chainedId = element->GetID();

    return true;
}

bool EditorToolkitCMN::Insert(std::string &elementType, std::string const &startid)
{
    if (!m_doc->GetDrawingPage()) return false;

    Object *start = m_doc->GetDrawingPage()->FindDescendantByID(startid);
    // Check if both start and end elements exist
    if (!start) {
        LogInfo("Element start id '%s' could not be found", startid.c_str());
        return false;
    }
    if (elementType == "note") {
        return this->InsertNote(start);
    }
    // Check if it is a LayerElement
    if (!dynamic_cast<LayerElement *>(start)) {
        LogInfo("Element '%s' is not supported as start element", start->GetClassName().c_str());
        return false;
    }

    Measure *measure = vrv_cast<Measure *>(start->GetFirstAncestor(MEASURE));
    assert(measure);

    ControlElement *element = NULL;
    if (elementType == "dir") {
        element = new Dir();
    }
    else if (elementType == "dynam") {
        element = new Dynam();
    }
    else {
        LogInfo("Inserting control event '%s' is not supported", elementType.c_str());
        return false;
    }

    assert(element);
    TimeSpanningInterface *interface = element->GetTimeSpanningInterface();
    assert(interface);
    measure->AddChild(element);
    interface->SetStartid("#" + startid);

    m_chainedId = element->GetID();

    return true;
}

bool EditorToolkitCMN::Set(std::string &elementId, std::string const &attribute, std::string const &value)
{
    Object *element = this->GetElement(elementId);
    if (!element) return false;

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

    return success;
}

Object *EditorToolkitCMN::GetElement(std::string &elementId)
{
    if (elementId == CHAINED_ID) {
        elementId = m_chainedId;
    }
    else {
        m_chainedId = elementId;
    }

    return this->GetElement(elementId);
}

bool EditorToolkitCMN::InsertNote(Object *object)
{
    assert(object);

    if (!object->Is({ CHORD, NOTE, REST })) {
        LogInfo("Inserting a note is possible only in a chord, note or rest");
        return false;
    }

    if (object->Is(CHORD)) {
        Chord *currentChord = vrv_cast<Chord *>(object);
        assert(currentChord);
        Note *note = new Note();
        currentChord->AddChild(note);
        m_chainedId = note->GetID();
        return true;
    }
    else if (object->Is(NOTE)) {
        Note *currentNote = vrv_cast<Note *>(object);
        assert(currentNote);

        Chord *currentChord = currentNote->IsChordTone();
        if (currentChord) {
            Note *note = new Note();
            currentChord->AddChild(note);
            m_chainedId = note->GetID();
            return true;
        }

        if (currentNote->HasEditorialContent()) {
            LogInfo("Inserting a note where a note has editorial content is not "
                    "possible");
            return false;
        }

        ListOfObjects lyric;
        ClassIdsComparison lyricsComparison({ VERSE, SYL });
        currentNote->FindAllDescendantsByComparison(&lyric, &lyricsComparison);
        if (!lyric.empty()) {
            LogInfo("Inserting a note where a note has lyric content is not possible");
            return false;
        }
        Chord *chord = new Chord();
        chord->DurationInterface::operator=(*currentNote);
        chord->AttCue::operator=(*currentNote);
        chord->AttGraced::operator=(*currentNote);
        chord->AttStems::operator=(*currentNote);
        chord->AttStemsCmn::operator=(*currentNote);
        currentNote->DurationInterface::Reset();
        currentNote->ResetCue();
        currentNote->ResetGraced();
        currentNote->ResetStems();
        currentNote->ResetStemsCmn();
        Object *parent = currentNote->GetParent();
        assert(parent);
        parent->ReplaceChild(currentNote, chord);
        chord->AddChild(currentNote);

        Note *note = new Note();
        chord->AddChild(note);

        ListOfObjects artics = currentNote->FindAllDescendantsByType(ARTIC);
        for (Object *artic : artics) {
            artic->MoveItselfTo(chord);
        }
        currentNote->ClearRelinquishedChildren();

        m_chainedId = note->GetID();
        return true;
    }
    else if (object->Is(REST)) {
        Rest *rest = vrv_cast<Rest *>(object);
        assert(rest);
        Note *note = new Note();
        note->DurationInterface::operator=(*rest);
        Object *parent = rest->GetParent();
        assert(parent);
        parent->ReplaceChild(rest, note);
        delete rest;
        m_chainedId = note->GetID();
        return true;
    }
    return false;
}

bool EditorToolkitCMN::DeleteNote(Note *note)
{
    assert(note);

    Chord *chord = note->IsChordTone();
    Beam *beam = note->GetAncestorBeam();
    if (chord) {
        if (chord->HasEditorialContent()) {
            LogInfo("Deleting a note in a chord that has editorial content is not "
                    "possible");
            return false;
        }
        int count = chord->GetChildCount(NOTE, UNLIMITED_DEPTH);
        if (count == 2) {
            Note *otherNote = chord->GetTopNote();
            if (note == otherNote) {
                otherNote = chord->GetBottomNote();
            }
            assert(otherNote && (otherNote != note));
            otherNote->DurationInterface::operator=(*chord);
            otherNote->AttCue::operator=(*chord);
            otherNote->AttGraced::operator=(*chord);
            otherNote->AttStems::operator=(*chord);
            otherNote->AttStemsCmn::operator=(*chord);
            Object *parent = chord->GetParent();
            assert(parent);
            chord->DetachChild(otherNote->GetIdx());
            parent->ReplaceChild(chord, otherNote);

            ListOfObjects artics = chord->FindAllDescendantsByType(ARTIC, false, 1);
            for (Object *artic : artics) {
                artic->MoveItselfTo(otherNote);
            }
            m_chainedId = chord->GetID();
            delete chord;
            return true;
        }
        else if (count > 2) {
            chord->DeleteChild(note);
            m_chainedId = chord->GetID();
            return true;
        }
        // Handle cases of chords with one single note
        else {
            Rest *rest = new Rest();
            rest->DurationInterface::operator=(*chord);
            Object *parent = chord->GetParent();
            assert(parent);
            parent->ReplaceChild(chord, rest);
            delete chord;
            return true;
        }
    }
    else if (beam) {
        // If the beam has exactly 2 notes (take apart and leave a single note and a
        // rest)
        if ((int)beam->m_beamSegment.GetElementCoordRefs()->size() == 2) {
            bool insertBefore = true;
            LayerElement *otherElement = beam->m_beamSegment.GetElementCoordRefs()->back()->m_element;
            if (note == otherElement) {
                insertBefore = false;
                otherElement = beam->m_beamSegment.GetElementCoordRefs()->front()->m_element;
            }
            assert(otherElement && (otherElement != note));
            Rest *rest = new Rest();
            rest->DurationInterface::operator=(*note);
            Object *parent = beam->GetParent();
            assert(parent);
            if (insertBefore) {
                parent->InsertBefore(beam, rest);
            }
            else {
                parent->InsertAfter(beam, rest);
            }
            beam->DetachChild(otherElement->GetIdx());
            parent->ReplaceChild(beam, otherElement);
            delete beam;
            m_chainedId = rest->GetID();
            return true;
        }
        // If the beam has more than 2 and this is first
        else if (beam->IsFirstIn(note)) {
            Rest *rest = new Rest();
            rest->DurationInterface::operator=(*note);
            Object *parent = beam->GetParent();
            assert(parent);
            parent->InsertBefore(beam, rest);
            beam->DeleteChild(note);
            m_chainedId = rest->GetID();
        }
        // If the beam has more than 2 and this is last
        else if (beam->IsLastIn(note)) {
            Rest *rest = new Rest();
            rest->DurationInterface::operator=(*note);
            Object *parent = beam->GetParent();
            assert(parent);
            parent->InsertAfter(beam, rest);
            beam->DeleteChild(note);
            m_chainedId = rest->GetID();
        }
        // If the beam has more than 2 and this in the middle
        else {
            Rest *rest = new Rest();
            rest->DurationInterface::operator=(*note);
            beam->ReplaceChild(note, rest);
            delete note;
            m_chainedId = rest->GetID();
        }
        // All but the first IF statement branches lead here
        /* Clearing the coords here fixes an error where the children get updated,
         * but the internal m_beamElementCoordRefs does not.  By clearing it, the
         * system is forced to update that structure to reflect the current
         * children. */
        beam->ClearCoords();
        return true;
    }
    else {
        // Deal with just a single note (Not in beam or chord)
        Rest *rest = new Rest();
        rest->DurationInterface::operator=(*note);
        Object *parent = note->GetParent();
        assert(parent);
        parent->ReplaceChild(note, rest);
        delete note;
        m_chainedId = rest->GetID();
        return true;
    }
}

bool EditorToolkitCMN::ContextForScores(bool editInfo)
{
    if (!m_scoreContext) {
        m_scoreContext = new EditorTreeObject(m_doc, false);
        ScoreContextFunctor scoreContextFunctor(m_scoreContext);
        m_doc->Process(scoreContextFunctor);
    }
    m_currentContext = m_scoreContext;

    if (!editInfo) return true;

    m_editInfo.reset();

    // The target object
    jsonxx::Object jsonObject;
    this->ContextForObject(m_scoreContext, jsonObject, true);

    m_editInfo = jsonObject;

    return true;
}

bool EditorToolkitCMN::ContextForSections(bool editInfo)
{
    if (!m_sectionContext) {
        m_sectionContext = new EditorTreeObject(m_doc, false);
        SectionContextFunctor sectionContextFunctor(m_sectionContext);
        m_doc->Process(sectionContextFunctor);
    }
    m_currentContext = m_sectionContext;

    if (!editInfo) return true;

    m_editInfo.reset();

    // The target object
    jsonxx::Object jsonObject;
    this->ContextForObject(m_sectionContext, jsonObject, true);

    m_editInfo = jsonObject;

    return true;
}

void EditorToolkitCMN::ClearContext()
{
    if (m_sectionContext) {
        delete m_sectionContext;
        m_sectionContext = NULL;
    }
}

bool EditorToolkitCMN::ContextForElement(std::string &elementId)
{
    m_editInfo.reset();

    // Make sure we have a section tree - this also sets m_currentContext
    this->ContextForSections(false);
    assert(m_sectionContext);

    bool hasTargetID = (elementId != "");
    const Object *object = NULL;
    if (hasTargetID) {
        object = this->GetElement(elementId);
    }
    // Retrieve the context from the first measure in the document
    else {
        object = m_doc->FindDescendantByType(MEASURE);
    }
    // We cannot continue without object
    if (!object || !object->GetParent()) return false;

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
        // Top element in the score subtree
        if (current->GetParent()->Is(SCORE)) break;
        current = current->GetParent();
        ancestors.push_back(current);
    }
    this->ContextForObjects(ancestors, jsonAncestors);
    m_editInfo << "ancestors" << jsonAncestors;

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
    m_editInfo << "context" << jsonContextRoot;

    // Stop here without targetID, but still add empty objects or arrays to the info
    if (!hasTargetID) {
        m_editInfo << "object" << jsonxx::Object();
        m_editInfo << "referringElements" << jsonxx::Array();
        m_editInfo << "referencedElements" << jsonxx::Array();
        return true;
    }

    // Inlude all attributes
    ArrayOfStrAttr attributes;
    object->GetAttributes(&attributes);
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
    m_editInfo << "object" << jsonObject;

    // Find referring objects
    ListOfObjectAttNamePairs referringObjects;
    FindAllReferringObjectsFunctor findAllReferringObjects(object, &referringObjects);
    m_doc->Process(findAllReferringObjects);
    this->ContextForReferences(referringObjects, elements);
    m_editInfo << "referringElements" << elements;

    // Find referenced objects
    ListOfObjectAttNamePairs referencedObjects;
    FindAllReferencedObjectsFunctor findAllReferencedObjects(NULL, &referencedObjects);
    object->Process(findAllReferencedObjects, 0);
    this->ContextForReferences(referencedObjects, elements);
    m_editInfo << "referencedElements" << elements;

    return true;
}

void EditorToolkitCMN::ContextForObject(const Object *object, jsonxx::Object &element, bool recursive)
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
                       [](const Object *item) { return item->Is({ DOTS, FLAG, STEM, TUPLET_NUM, TUPLET_BRACKET }); }),
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

void EditorToolkitCMN::ContextForObjects(const ArrayOfConstObjects &objects, jsonxx::Array &elements)
{
    elements.reset();

    for (const Object *object : objects) {
        if (object->Is(MNUM)) {
            const MNum *mNum = vrv_cast<const MNum *>(object);
            assert(mNum);
            if (mNum->IsGenerated()) continue;
        }
        if (object->IsAttribute()) continue;
        if (object->Is({ DOTS, FLAG, STEM, TUPLET_NUM, TUPLET_BRACKET })) continue;

        jsonxx::Object element;
        this->ContextForObject(object, element);
        elements << element;
    }
}

void EditorToolkitCMN::ContextForReferences(const ListOfObjectAttNamePairs &objectAttNames, jsonxx::Array &references)
{
    references.reset();

    for (auto &objectAttName : objectAttNames) {
        jsonxx::Object element;
        this->ContextForObject(objectAttName.first, element);
        element << "referenceAttribute" << objectAttName.second;
        references << element;
    }
}

ArrayOfConstObjects EditorToolkitCMN::GetScoreBasedChildrenFor(const Object *object)
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

        childObjects.push_back((editorTreeChild->m_object ? editorTreeChild->m_object : editorTreeChild));
    }
    return childObjects;
}

#endif /* NO_EDIT_SUPPORT */

} // namespace vrv

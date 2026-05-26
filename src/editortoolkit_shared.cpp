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

    this->SetEditInfo();
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
        this->SetEditInfo();
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
        this->SetEditInfo();
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
bool EditorToolkitShared::ParseContextAction(jsonxx::Object param, std::string &elementId, bool &scores, bool &sections)
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

bool EditorToolkitShared::ParseDeleteAction(jsonxx::Object param, std::string &elementId)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    elementId = param.get<jsonxx::String>("elementId");
    return true;
}

bool EditorToolkitShared::ParseDragAction(jsonxx::Object param, std::string &elementId, int &x, int &y)
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
    jsonxx::Object param, std::string &elementName, std::string &elementId, std::string &insertMode)
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
    jsonxx::Object param, std::string &elementName, std::string &startId, std::string &endId)
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

bool EditorToolkitShared::ParseNavigate(jsonxx::Object param, std::string &elementId, int &direction)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    elementId = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::Number>("direction")) return false;
    direction = param.get<jsonxx::Number>("direction");
    return true;
}

bool EditorToolkitShared::ParsePropertiesAction(jsonxx::Object param, std::string &scoreDef)
{
    scoreDef = "";
    if (param.has<jsonxx::String>("scoreDef")) {
        scoreDef = param.get<jsonxx::String>("scoreDef");
        return true;
    }
    return true;
}

bool EditorToolkitShared::ParseSetAction(
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

void EditorToolkitShared::PrepareUndo()
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

void EditorToolkitShared::SetEditInfo()
{
    m_editInfo.reset();
    m_editInfo.import("chainedId", m_chainedId);
    m_editInfo.import("canUndo", this->CanUndo());
    m_editInfo.import("canRedo", this->CanRedo());
    m_editInfo.import("isMensuralMusicOnly", m_doc->IsMensuralMusicOnly());
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

bool EditorToolkitShared::Chain(jsonxx::Array actions)
{
    bool status = true;
    m_chainedId = "";
    for (int i = 0; i < (int)actions.size(); ++i) {
        status = this->ParseEditorAction(actions.get<jsonxx::Object>(i).json(), !status);
    }
    return status;
}

bool EditorToolkitShared::Delete(std::string &elementId)
{
    Object *element = this->GetChainedElement(elementId);

    if (!element) return false;

    this->Navigate(elementId, 37);
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
    this->SetEditInfo();
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
    Object *element = this->GetChainedElement(elementId);
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

bool EditorToolkitShared::InsertControl(
    const std::string &elementName, const std::string startId, const std::string endId)
{
    Object *start = this->GetElement(startId);
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

    TimeSpanningInterface *timeSpanningInterface = childElement->GetTimeSpanningInterface();
    if (timeSpanningInterface && !endId.empty()) timeSpanningInterface->SetEndid("#" + endId);

    return true;
}

bool EditorToolkitShared::KeyDown(std::string &elementId, int key, bool shiftKey, bool ctrlKey)
{
    Object *element = this->GetChainedElement(elementId);
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

bool EditorToolkitShared::Navigate(std::string &elementId, const int &direction)
{
    static auto classIds = { CHORD, MREST, NOTE, REST };

    const bool forward = (direction == 39);

    m_chainedId = "";
    this->SetEditInfo();

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

        if (result->Is(classIds)) break;
    }

    if (!result) {
        ClassIdsComparison matches(classIds);
        if (forward) {
            const Staff *staff = vrv_cast<const Staff *>(layer->GetFirstAncestor(STAFF));
            assert(staff);
            const Measure *measure = vrv_cast<const Measure *>(staff->GetFirstAncestor(MEASURE));
            assert(measure);
            const System *system = vrv_cast<const System *>(measure->GetFirstAncestor(SYSTEM));
            assert(system);
            const Measure *nextMeasure = vrv_cast<const Measure *>(system->GetNext(measure, MEASURE));
            if (!nextMeasure) {
                const Page *page = vrv_cast<const Page *>(system->GetFirstAncestor(PAGE));
                assert(page);
                const System *nextSystem = vrv_cast<const System *>(page->GetNext(system, SYSTEM));
                if (!nextSystem) {
                    const Page *nextPage = vrv_cast<const Page *>(m_doc->GetPages()->GetNext(page, PAGE));
                    if (!nextPage) return true;
                    nextSystem = vrv_cast<const System *>(nextPage->GetFirst(SYSTEM));
                    if (!nextSystem) return true;
                }
                nextMeasure = vrv_cast<const Measure *>(nextSystem->GetFirst(MEASURE));
                if (!nextMeasure) return true;
            }
            AttNIntegerComparison staffNComparison(STAFF, staff->GetN());
            const Staff *nextStaff
                = vrv_cast<const Staff *>(nextMeasure->FindDescendantByComparison(&staffNComparison));
            if (!nextStaff) return true;
            AttNIntegerComparison layerNComparison(LAYER, layer->GetN());
            layer = vrv_cast<const Layer *>(nextStaff->FindDescendantByComparison(&layerNComparison));
            if (!layer) return true;
            result = vrv_cast<const LayerElement *>(layer->FindDescendantByComparison(&matches));
        }
        else {
            const Staff *staff = vrv_cast<const Staff *>(layer->GetFirstAncestor(STAFF));
            assert(staff);
            const Measure *measure = vrv_cast<const Measure *>(staff->GetFirstAncestor(MEASURE));
            assert(measure);
            const System *system = vrv_cast<const System *>(measure->GetFirstAncestor(SYSTEM));
            assert(system);
            const Measure *previousMeasure = vrv_cast<const Measure *>(system->GetPrevious(measure, MEASURE));
            if (!previousMeasure) {
                const Page *page = vrv_cast<const Page *>(system->GetFirstAncestor(PAGE));
                assert(page);
                const System *previousSystem = vrv_cast<const System *>(page->GetPrevious(system, SYSTEM));
                if (!previousSystem) {
                    const Page *previousPage = vrv_cast<const Page *>(m_doc->GetPages()->GetPrevious(page, PAGE));
                    if (!previousPage) return true;

                    previousSystem = vrv_cast<const System *>(previousPage->GetLast(SYSTEM));
                    if (!previousSystem) return true;
                }
                previousMeasure = vrv_cast<const Measure *>(previousSystem->GetLast(MEASURE));
                if (!previousMeasure) return true;
            }
            AttNIntegerComparison staffNComparison(STAFF, staff->GetN());
            const Staff *previousStaff
                = vrv_cast<const Staff *>(previousMeasure->FindDescendantByComparison(&staffNComparison));
            if (!previousStaff) return true;
            AttNIntegerComparison layerNComparison(LAYER, layer->GetN());
            layer = vrv_cast<const Layer *>(previousStaff->FindDescendantByComparison(&layerNComparison));
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

    this->SetEditInfo();
    return true;
}

bool EditorToolkitShared::Set(std::string &elementId, std::string const &attribute, std::string const &value)
{
    Object *element = this->GetChainedElement(elementId);
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

bool EditorToolkitShared::ContextForScores(bool editInfo)
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

bool EditorToolkitShared::ContextForSections(bool editInfo)
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

void EditorToolkitShared::ClearContext()
{
    if (m_sectionContext) {
        delete m_sectionContext;
        m_sectionContext = NULL;
    }
}

bool EditorToolkitShared::ContextForElement(std::string &elementId)
{
    m_editInfo.reset();

    // Make sure we have a section tree - this also sets m_currentContext
    this->ContextForSections(false);
    assert(m_sectionContext);

    bool hasTargetID = (elementId != "");
    const Object *object = NULL;
    if (hasTargetID) {
        object = this->GetChainedElement(elementId);
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
        if (object->Is({ DOTS, FLAG, STEM, TUPLET_NUM, TUPLET_BRACKET })) continue;

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
    m_editInfo.reset();

    MEIOutputExtended output(m_doc);

    m_editInfo = output.ExportScoreDef();

    return true;
}

bool EditorToolkitShared::SetScoreDef(const std::string scoreDef)
{
    return true;
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

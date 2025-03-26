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
#include "findfunctor.h"
#include "hairpin.h"
#include "layer.h"
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
#include "tie.h"
#include "timeinterface.h"
#include "vrv.h"
#include "zone.h"

//--------------------------------------------------------------------------------

#define CHAINED_ID "[chained-id]"

namespace vrv {

std::string EditorToolkitCMN::EditInfo()
{
    return m_editInfo.json();
}

bool EditorToolkitCMN::ParseEditorAction(const std::string &json_editorAction, bool commitOnly)
{
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

    m_doc->SetFocus();

    // Action without parameter
    if (action == "commit") {
        m_doc->PrepareData();
        m_doc->ScoreDefSetCurrentDoc(true);
        m_doc->RefreshLayout();
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
        bool contentOnly;
        if (this->ParseContextAction(json.get<jsonxx::Object>("param"), elementId, contentOnly)) {
            return this->Context(elementId, contentOnly);
        }
        LogWarning("Could not parse the context action");
    }
    else if (action == "delete") {
        std::string elementId;
        if (this->ParseDeleteAction(json.get<jsonxx::Object>("param"), elementId)) {
            return this->Delete(elementId);
        }
        LogWarning("Could not parse the delete action");
    }
    else if (action == "drag") {
        std::string elementId;
        int x, y;
        if (this->ParseDragAction(json.get<jsonxx::Object>("param"), elementId, x, y)) {
            return this->Drag(elementId, x, y);
        }
        LogWarning("Could not parse the drag action");
    }
    else if (action == "keyDown") {
        std::string elementId;
        int key;
        bool shiftKey, ctrlKey;
        if (this->ParseKeyDownAction(json.get<jsonxx::Object>("param"), elementId, key, shiftKey, ctrlKey)) {
            return this->KeyDown(elementId, key, shiftKey, ctrlKey);
        }
        LogWarning("Could not parse the keyDown action");
    }
    else if (action == "insert") {
        std::string elementType, startid, endid;
        if (this->ParseInsertAction(json.get<jsonxx::Object>("param"), elementType, startid, endid)) {
            if (endid == "") {
                return this->Insert(elementType, startid);
            }
            else {
                return this->Insert(elementType, startid, endid);
            }
        }
        LogWarning("Could not parse the insert action");
    }
    else if (action == "set") {
        std::string elementId, attribute, value;
        if (this->ParseSetAction(json.get<jsonxx::Object>("param"), elementId, attribute, value)) {
            return this->Set(elementId, attribute, value);
        }
        LogWarning("Could not parse the set action");
    }
    else {
        LogWarning("Unknown action type '%s'.", action.c_str());
    }
    return false;
}

bool EditorToolkitCMN::ParseContextAction(jsonxx::Object param, std::string &elementId, bool &contentOnly)
{
    contentOnly = false;
    if (!param.has<jsonxx::String>("elementId")) return false;
    elementId = param.get<jsonxx::String>("elementId");
    // optional
    if (param.has<jsonxx::Boolean>("contentOnly")) {
        contentOnly = param.get<jsonxx::Boolean>("contentOnly");
    }
    return true;
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

bool EditorToolkitCMN::Chain(jsonxx::Array actions)
{
    bool status = true;
    m_chainedId = "";
    for (int i = 0; i < (int)actions.size(); ++i) {
        status = this->ParseEditorAction(actions.get<jsonxx::Object>(i).json(), !status);
        m_editInfo.import("uuid", m_chainedId);
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
    m_editInfo.import("uuid", element->GetID());

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
    m_editInfo.import("uuid", element->GetID());

    return true;
}

bool EditorToolkitCMN::Set(std::string &elementId, std::string const &attribute, std::string const &value)
{
    Object *element = this->GetElement(elementId);
    if (!element) return false;

    bool success = false;
    if (AttModule::SetAnalytical(element, attribute, value))
        success = true;
    else if (AttModule::SetCmn(element, attribute, value))
        success = true;
    else if (AttModule::SetCmnornaments(element, attribute, value))
        success = true;
    else if (AttModule::SetCritapp(element, attribute, value))
        success = true;
    else if (AttModule::SetExternalsymbols(element, attribute, value))
        success = true;
    else if (AttModule::SetFacsimile(element, attribute, value))
        success = true;
    else if (AttModule::SetGestural(element, attribute, value))
        success = true;
    else if (AttModule::SetMei(element, attribute, value))
        success = true;
    else if (AttModule::SetMensural(element, attribute, value))
        success = true;
    else if (AttModule::SetMidi(element, attribute, value))
        success = true;
    else if (AttModule::SetNeumes(element, attribute, value))
        success = true;
    else if (AttModule::SetPagebased(element, attribute, value))
        success = true;
    else if (AttModule::SetShared(element, attribute, value))
        success = true;
    else if (AttModule::SetVisual(element, attribute, value))
        success = true;
    if (success) {
        return true;
    }
    return false;
}

Object *EditorToolkitCMN::GetElement(std::string &elementId)
{
    if (elementId == CHAINED_ID) {
        elementId = m_chainedId;
    }
    else {
        m_chainedId = elementId;
    }

    Object *element = NULL;

    // Try to get the element on the current drawing page
    if (m_doc->GetDrawingPage()) {
        element = m_doc->GetDrawingPage()->FindDescendantByID(elementId);
    }
    // If it wasn't there, try on the whole doc
    if (!element) {
        element = m_doc->FindDescendantByID(elementId);
    }

    return element;
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

bool EditorToolkitCMN::Context(std::string &elementId, bool contentOnly)
{
    m_editInfo.reset();

    Object *object = this->GetElement(elementId);
    if (!object || !object->GetParent()) return false;

    ListOfConstObjects objects;
    ListOfConstObjects::iterator targetIt;

    if (object->GetParent()->Is(SYSTEM)) {
        ChildOfClassIdComparison systemChild(SYSTEM);
        // System children are level 4, no needt to go deeper
        m_doc->FindAllDescendantsByComparison(&objects, &systemChild, 4);

        targetIt = std::find(objects.begin(), objects.end(), object);
        // This should not happen
        if (targetIt == objects.end()) return false;

        // Lambda to check if the object is a delimiter (ending, section, milestone end)
        auto isSectionDelimiter
            = [](const Object *item) { return (item->Is({ ENDING, SECTION, SYSTEM_MILESTONE_END })); };

        // Find the last occurrence of a delimiter  before the target - erase before it if found
        auto delimiterBefore = std::find_if(
            std::make_reverse_iterator(targetIt), std::make_reverse_iterator(objects.begin()), isSectionDelimiter)
                                   .base();
        objects.erase(objects.begin(), delimiterBefore);

        // Find the first occurrence of delimiter after the target - eraase after it if found
        auto delimiterAfter = std::find_if(std::next(targetIt), objects.end(), isSectionDelimiter);
        if (delimiterAfter != objects.end()) objects.erase(delimiterAfter, objects.end());
    }
    else {
        const ArrayOfObjects &objectsArr = object->GetParent()->GetChildren();
        std::copy(objectsArr.begin(), objectsArr.end(), std::back_inserter(objects));
        targetIt = std::find(objects.begin(), objects.end(), object);
        // This should not happen
        if (targetIt == objects.end()) return false;
    }
    ListOfConstObjects previous;
    if (targetIt != objects.begin()) std::copy(objects.begin(), targetIt, std::back_inserter(previous));

    ListOfConstObjects following;
    if (targetIt != objects.end()) std::copy(std::next(targetIt), objects.end(), std::back_inserter(following));

    // Build the json context
    
    jsonxx::Object section;
    section << "id" << 0;
    section << "element" << ".";
    jsonxx::Object jsonContext;
    
    ListOfConstObjects ancestors;
    jsonxx::Array jsonAncestors;
    
    const Object *context = object->GetParent();
    const Object *parent = context->GetParent();
    
    if (!context->Is(SYSTEM)) {
        // Look for additional ancestors
        while (parent) {
            if (parent->Is(SYSTEM)) break;
            ancestors.push_back(parent);
            parent = parent->GetParent();
        }
        this->ContextForObjects(ancestors, jsonAncestors);
        // Also add the section (.) as ancestor
        jsonAncestors << section;
        
        this->ContextForObject(context, jsonContext);
    }
    else {
        jsonContext = section;
    }

    // Ancestors in addition to the parent of the target (context object)
    m_editInfo << "ancestors" << jsonAncestors;
    
    // Build the list of children in which the target is included
    jsonxx::Array contextChildren;
    
    // Preceeding siblings
    jsonxx::Array elements;
    this->ContextForObjects(previous, elements);
    contextChildren << elements;

    // The target object
    jsonxx::Object jsonObject;
    this->ContextForObject(object, jsonObject);
    // Inlude all attributes
    ArrayOfStrAttr attributes;
    object->GetAttributes(&attributes);
    jsonxx::Object jsonAttributes;
    for (const auto &attribute : attributes) {
        jsonAttributes << attribute.first << attribute.second;
    }
    jsonObject << "attributes" << jsonAttributes;
    // Include its children
    jsonxx::Array jsonObjectChildren;
    ListOfConstObjects objectChildren;
    const ArrayOfObjects &objectsArr = object->GetChildren();
    std::copy(objectsArr.begin(), objectsArr.end(), std::back_inserter(objectChildren));
    this->ContextForObjects(objectChildren, jsonObjectChildren);
    if (!jsonObjectChildren.empty()) jsonObject << "children" << jsonObjectChildren;
    // Add it to the list
    contextChildren << jsonObject;
    
    // Following siblings
    this->ContextForObjects(following, elements);
    contextChildren << elements;
    
    // Add all children of to context (include target and surrounding siblings)
    jsonContext << "children" << contextChildren;
    m_editInfo << "context" << jsonContext;

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

void EditorToolkitCMN::ContextForObject(const Object *object, jsonxx::Object &element)
{
    element << "name" << object->GetClassName();
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
}

void EditorToolkitCMN::ContextForObjects(const ListOfConstObjects &objects, jsonxx::Array &elements)
{
    elements.reset();

    for (const Object *object : objects) {
        if (object->Is(MNUM)) {
            const MNum *mNum = vrv_cast<const MNum *>(object);
            assert(mNum);
            if (mNum->IsGenerated()) continue;
        }
        if (object->IsAttribute()) continue;
        if (object->Is({DOTS, FLAG, STEM, TUPLET_NUM, TUPLET_BRACKET})) continue;
        
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

} // namespace vrv

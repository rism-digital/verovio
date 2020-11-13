/////////////////////////////////////////////////////////////////////////////
// Name:        editortoolkit_cmn.cpp
// Author:      Laurent Pugin, Juliette Regimbal, Zoe McLennan
// Created:     04/06/2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "editortoolkit_cmn.h"

//--------------------------------------------------------------------------------

#include <codecvt>
#include <locale>
#include <set>

//--------------------------------------------------------------------------------

#include "chord.h"
#include "clef.h"
#include "comparison.h"
#include "hairpin.h"
#include "layer.h"
#include "measure.h"
#include "note.h"
#include "page.h"
#include "rend.h"
#include "rest.h"
#include "slur.h"
#include "staff.h"
#include "vrv.h"

//--------------------------------------------------------------------------------

#include "jsonxx.h"

//--------------------------------------------------------------------------------

#define CHAINED_ID "[chained-id]"

namespace vrv {

std::string EditorToolkitCMN::EditInfo()
{
    return m_chainedId;
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

    // Action without parameter
    if (action == "commit") {
        m_doc->PrepareDrawing();
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
    for (int i = 0; i < (int)actions.size(); i++) {
        status = this->ParseEditorAction(actions.get<jsonxx::Object>(i).json(), !status);
        m_editInfo = m_chainedId;
    }
    return status;
}

bool EditorToolkitCMN::Delete(std::string &elementId)
{
    Object *element = this->GetElement(elementId);
    if (!element) return false;

    if (element->Is(NOTE)) {
        return this->DeleteNote(dynamic_cast<Note *>(element));
    }
    return false;
}

bool EditorToolkitCMN::Drag(std::string &elementId, int x, int y)
{
    Object *element = this->GetElement(elementId);
    if (!element) return false;

    // For elements whose y-position corresponds to a certain pitch
    if (element->HasInterface(INTERFACE_PITCH)) {
        Layer *layer = dynamic_cast<Layer *>(element->GetFirstAncestor(LAYER));
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

    Object *start = m_doc->GetDrawingPage()->FindDescendantByUuid(startid);
    Object *end = m_doc->GetDrawingPage()->FindDescendantByUuid(endid);
    // Check if both start and end elements exist
    if (!start || !end) {
        LogMessage("Elements start and end ids '%s' and '%s' could not be found", startid.c_str(), endid.c_str());
        return false;
    }
    // Check if it is a LayerElement
    if (!dynamic_cast<LayerElement *>(start)) {
        LogMessage("Element '%s' is not supported as start element", start->GetClassName().c_str());
        return false;
    }
    if (!dynamic_cast<LayerElement *>(end)) {
        LogMessage("Element '%s' is not supported as end element", start->GetClassName().c_str());
        return false;
    }

    Measure *measure = vrv_cast<Measure *>(start->GetFirstAncestor(MEASURE));
    assert(measure);

    ControlElement *element = NULL;
    if (elementType == "slur") {
        element = new Slur();
    }
    else if (elementType == "hairpin") {
        element = new Hairpin();
    }
    else {
        LogMessage("Inserting control event '%s' is not supported", elementType.c_str());
        return false;
    }

    assert(element);
    TimeSpanningInterface *interface = element->GetTimeSpanningInterface();
    assert(interface);
    measure->AddChild(element);
    interface->SetStartid(startid);
    interface->SetEndid(endid);

    this->m_chainedId = element->GetUuid();

    return true;
}

bool EditorToolkitCMN::Insert(std::string &elementType, std::string const &startid)
{
    if (!m_doc->GetDrawingPage()) return false;

    Object *start = m_doc->GetDrawingPage()->FindDescendantByUuid(startid);
    // Check if both start and end elements exist
    if (!start) {
        LogMessage("Element start id '%s' could not be found", startid.c_str());
        return false;
    }
    if (elementType == "note") {
        return this->InsertNote(start);
    }
    // Check if it is a LayerElement
    if (!dynamic_cast<LayerElement *>(start)) {
        LogMessage("Element '%s' is not supported as start element", start->GetClassName().c_str());
        return false;
    }

    /*
    Measure *measure = vrv_cast<Measure *>(start->GetFirstAncestor(MEASURE));
    assert(measure);

    ControlElement *element = NULL;
    if (elementType == "dynam") {
        element = new Slur();
    }
    else if (elementType == "hairpin") {
        element = new Hairpin();
    }
    else {
        LogMessage("Inserting control event '%s' is not supported", elementType.c_str());
        return false;
    }

    assert(element);
    TimeSpanningInterface *interface = element->GetTimeSpanningInterface();
    assert(interface);
    measure->AddChild(element);
    interface->SetStartid(startid);
    interface->SetEndid(endid);

    this->m_chainedId = element->GetUuid();
    */

    return true;
}

bool EditorToolkitCMN::Set(std::string &elementId, std::string const &attribute, std::string const &value)
{
    Object *element = this->GetElement(elementId);
    if (!element) return false;

    bool success = false;
    if (Att::SetAnalytical(element, attribute, value))
        success = true;
    else if (Att::SetCmn(element, attribute, value))
        success = true;
    else if (Att::SetCmnornaments(element, attribute, value))
        success = true;
    else if (Att::SetCritapp(element, attribute, value))
        success = true;
    else if (Att::SetExternalsymbols(element, attribute, value))
        success = true;
    else if (Att::SetFacsimile(element, attribute, value))
        success = true;
    else if (Att::SetGestural(element, attribute, value))
        success = true;
    else if (Att::SetMei(element, attribute, value))
        success = true;
    else if (Att::SetMensural(element, attribute, value))
        success = true;
    else if (Att::SetMidi(element, attribute, value))
        success = true;
    else if (Att::SetNeumes(element, attribute, value))
        success = true;
    else if (Att::SetPagebased(element, attribute, value))
        success = true;
    else if (Att::SetShared(element, attribute, value))
        success = true;
    else if (Att::SetVisual(element, attribute, value))
        success = true;
    if (success) {
        return true;
    }
    return false;
}

Object *EditorToolkitCMN::GetElement(std::string &elementId)
{
    if (elementId == CHAINED_ID) {
        elementId = this->m_chainedId;
    }
    else {
        this->m_chainedId = elementId;
    }

    Object *element = NULL;

    // Try to get the element on the current drawing page
    if (m_doc->GetDrawingPage()) {
        element = m_doc->GetDrawingPage()->FindDescendantByUuid(elementId);
    }
    // If it wasn't there, try on the whole doc
    if (!element) {
        element = m_doc->FindDescendantByUuid(elementId);
    }

    return element;
}

bool EditorToolkitCMN::InsertNote(Object *object)
{
    assert(object);

    if (!object->Is({ CHORD, NOTE, REST })) {
        LogMessage("Inserting a note is possible only in a chord, note or rest");
        return false;
    }

    if (object->Is(CHORD)) {
        Chord *currentChord = vrv_cast<Chord *>(object);
        assert(currentChord);
        Note *note = new Note();
        currentChord->AddChild(note);
        this->m_chainedId = note->GetUuid();
        return true;
    }
    else if (object->Is(NOTE)) {
        Note *currentNote = vrv_cast<Note *>(object);
        assert(currentNote);

        Chord *currentChord = currentNote->IsChordTone();
        if (currentChord) {
            Note *note = new Note();
            currentChord->AddChild(note);
            this->m_chainedId = note->GetUuid();
            return true;
        }

        if (currentNote->HasEditorialContent()) {
            LogMessage("Inserting a note where a note has editorial content is not possible");
            return false;
        }

        ListOfObjects lyric;
        ClassIdsComparison lyricsComparison({ VERSE, SYL });
        currentNote->FindAllDescendantByComparison(&lyric, &lyricsComparison);
        if (!lyric.empty()) {
            LogMessage("Inserting a note where a note has lyric content is not possible");
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

        ListOfObjects artics;
        ClassIdComparison articComparison(ARTIC);
        currentNote->FindAllDescendantByComparison(&artics, &articComparison);
        for (auto &artic : artics) {
            artic->MoveItselfTo(chord);
        }
        currentNote->ClearRelinquishedChildren();

        this->m_chainedId = note->GetUuid();
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
        this->m_chainedId = note->GetUuid();
        return true;
    }
    return false;
}

bool EditorToolkitCMN::DeleteNote(Note *note)
{
    assert(note);

    Chord *chord = note->IsChordTone();
    Beam *beam = note->IsInBeam();

    if (chord) {
        if (chord->HasEditorialContent()) {
            LogMessage("Deleting a note in a chord that has editorial content is not possible");
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

            ListOfObjects artics;
            ClassIdComparison articComparison(ARTIC);
            chord->FindAllDescendantByComparison(&artics, &articComparison, 1);
            for (auto &artic : artics) {
                artic->MoveItselfTo(otherNote);
            }
            this->m_chainedId = chord->GetUuid();
            delete chord;
            return true;
        }
        else if (count > 2) {
            chord->DeleteChild(note);
            this->m_chainedId = chord->GetUuid();
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
            this->m_chainedId = rest->GetUuid();
            return true;
        }
        if (beam->IsFirstInBeam(note)) {
            Rest *rest = new Rest();
            rest->DurationInterface::operator=(*note);
            Object *parent = beam->GetParent();
            assert(parent);
            parent->InsertBefore(beam, rest);
            beam->DeleteChild(note);
            this->m_chainedId = rest->GetUuid();
            return true;
        }
        else if (beam->IsLastInBeam(note)) {
            Rest *rest = new Rest();
            rest->DurationInterface::operator=(*note);
            Object *parent = beam->GetParent();
            assert(parent);
            parent->InsertAfter(beam, rest);
            beam->DeleteChild(note);
            this->m_chainedId = rest->GetUuid();
            return true;
        }
        else {
            Rest *rest = new Rest();
            rest->DurationInterface::operator=(*note);
            beam->ReplaceChild(note, rest);
            delete note;
            this->m_chainedId = rest->GetUuid();
            return true;
        }
    }
    else {
        Rest *rest = new Rest();
        rest->DurationInterface::operator=(*note);
        Object *parent = note->GetParent();
        assert(parent);
        parent->ReplaceChild(note, rest);
        delete note;
        this->m_chainedId = rest->GetUuid();
        return true;
    }
}

} // namespace vrv

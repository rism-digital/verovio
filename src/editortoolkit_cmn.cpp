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

#include "comparison.h"
#include "cursor.h"
#include "layer.h"
#include "miscfunctor.h"
#include "note.h"
#include "staff.h"

namespace vrv {

EditorToolkitCMN::EditorToolkitCMN(Doc *doc, View *view) : EditorToolkitShared(doc, view) {}

EditorToolkitCMN::~EditorToolkitCMN() {}

#ifndef NO_EDIT_SUPPORT
bool EditorToolkitCMN::ParseEditorCMNAction(const jsonxx::Object &json)
{
    std::string action = json.get<jsonxx::String>("action");

    if (action == "insertMeasure") {
        std::string elementId;
        int number;
        bool insertBefore;
        if (this->ParseInsertMeasureAction(json.get<jsonxx::Object>("param"), elementId, number, insertBefore)) {
            this->PrepareUndo();
            return (this->InsertMeasure(elementId, number, insertBefore));
        }
        LogWarning("Could not parse the insertMeasure action");
    }
    else if (action == "insertNote") {
        std::string elementId;
        data_PITCHNAME pname;
        int oct;
        data_DURATION dur;
        bool chordMode;
        if (this->ParseInsertNoteAction(json.get<jsonxx::Object>("param"), elementId, pname, oct, dur, chordMode)) {
            this->PrepareUndo();
            return (this->InsertNote(elementId, pname, oct, dur, chordMode));
        }
        LogWarning("Could not parse the insertNote action");
    }
    return false;
}

bool EditorToolkitCMN::ParseInsertMeasureAction(
    const jsonxx::Object &param, std::string &elementId, int &number, bool &insertBefore)
{
    number = 0;
    elementId = "";
    insertBefore = false;

    if (param.has<jsonxx::String>("elementId")) elementId = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::Number>("number")) return false;
    number = param.get<jsonxx::Number>("number");
    if (param.has<jsonxx::Boolean>("insertBefore")) insertBefore = param.get<jsonxx::Boolean>("insertBefore");

    return true;
}

bool EditorToolkitCMN::ParseInsertNoteAction(const jsonxx::Object &param, std::string &elementId, data_PITCHNAME &pname,
    int &oct, data_DURATION &dur, bool &chordMode)
{
    chordMode = false;
    pname = PITCHNAME_NONE;
    oct = VRV_UNSET;
    dur = DURATION_NONE;
    Note note;
    if (!param.has<jsonxx::String>("elementId")) return false;
    elementId = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::String>("pname")) return false;
    pname = note.AttPitch::StrToPitchname(param.get<jsonxx::String>("pname"));
    if (!param.has<jsonxx::Number>("oct")) return false;
    oct = param.get<jsonxx::Number>("oct");
    // At least one of the two
    if (!param.has<jsonxx::String>("dur") && !param.has<jsonxx::Boolean>("chordMode")) return false;

    if (param.has<jsonxx::String>("dur")) dur = note.AttPitch::StrToDuration(param.get<jsonxx::String>("dur"));

    if (param.has<jsonxx::Boolean>("chordMode")) chordMode = param.get<jsonxx::Boolean>("chordMode");

    return true;
}

bool EditorToolkitCMN::InsertMeasure(std::string &elementId, int number, bool insertBefore)
{
    bool endInsert = (elementId.empty());
    Measure *measure = NULL;

    int measureN = VRV_UNSET;
    if (endInsert) {
        measure = vrv_cast<Measure *>(m_doc->FindDescendantByType(MEASURE, UNLIMITED_DEPTH, BACKWARD));
    }
    else {
        measure = vrv_cast<Measure *>(this->ResolveElement(elementId, false));
    }

    if (!measure) return false;

    if (endInsert && IsValidInteger(measure->GetN())) measureN = std::stoi(measure->GetN());

    InitProcessingListsFunctor initProcessingLists;
    measure->Process(initProcessingLists);
    const IntTree &layerTree = initProcessingLists.GetLayerTree();

    for (int i = 0; i < number; i++) {
        Measure *newMeasure = new Measure();
        if (endInsert && (i == 0) && measure->HasRight()) {
            newMeasure->SetRight(measure->GetRight());
            measure->SetRight(BARRENDITION_NONE);
        }
        if (measureN != VRV_UNSET) {
            newMeasure->SetN(StringFormat("%d", measureN + number - i));
        }

        // Now we can process by layer and move their content to (measure) segments
        for (const auto &staves : layerTree.child) {
            Staff *staff = new Staff(staves.first);
            newMeasure->AddChildBack(staff);
            for (const auto &layers : staves.second.child) {
                Layer *layer = new Layer();
                layer->SetN(layers.first);
                staff->AddChild(layer);
            }
        }
        if (insertBefore) {
            measure->GetParent()->InsertBefore(measure, newMeasure);
        }
        else {
            measure->GetParent()->InsertAfter(measure, newMeasure);
        }
        m_chainedId = newMeasure->GetID();
    }

    this->ClearContext();

    return true;
}

bool EditorToolkitCMN::InsertNote(
    const std::string &elementId, data_PITCHNAME pname, int oct, data_DURATION dur, bool chordMode)
{
    if (chordMode) return this->InsertNoteInChordMode(elementId, pname, oct);

    Object *target = NULL;
    if (this->InsertMode()) {
        target = (m_cursor->HasPosition()) ? m_cursor->GetPosition() : m_cursor->GetParent();
        oct = m_cursor->GetOct();
        pname = m_cursor->GetPname();
    }
    else {
        target = this->GetElement(elementId);
    }
    if (!target || !target->Is({ CHORD, LAYER, NOTE, REST })) return false;

    if (target->Is(NOTE)) {
        Note *note = vrv_cast<Note *>(target);
        if (note->IsChordTone()) target = note->IsChordTone();
    }

    Object *previousElement = NULL;
    Object *targetContainer = NULL;
    if (!target->Is(LAYER)) {
        Object *targetParent = target->GetParent();
        // Inserting a note within a tuplet or a beam
        if (targetParent && targetParent->Is({ BEAM, TUPLET }) && targetParent->GetLast() != target) {
            previousElement = target;
            targetContainer = targetParent;
        }
        // Otherwise always insert in the layer
        else {
            previousElement = target->GetLastAncestorNot(LAYER);
            if (!previousElement) return false;
            targetContainer = previousElement->GetParent();
            assert(targetContainer && targetContainer->Is(LAYER));
        }
    }
    else {
        targetContainer = target;
    }

    Note *note = vrv_cast<Note *>(this->PrepareInsertion(targetContainer, "note"));
    if (!note) return false;

    note->SetPname(pname);
    note->SetOct(oct);
    note->SetDur(dur);

    if (m_cursor && m_cursor->HasAccid()) {
        Accid *accid = new Accid();
        m_cursor->GetAccidValue(accid);
        note->AddChild(accid);
        m_cursor->SetAccid(ACCIDENTAL_WRITTEN_NONE);
    }
    if (m_cursor && m_cursor->HasDots()) {
        note->SetDots(m_cursor->GetDots());
    }

    if (previousElement) {
        targetContainer->InsertAfter(previousElement, note);
    }
    else {
        targetContainer->InsertChild(note, 0);
    }

    if (note->IsInBeam()) {
        note->SetDur(std::max(DURATION_8, dur));
    }

    this->ClearContext();
    this->SetEditStatus();

    if (InsertMode()) this->MoveCursor(note);

    return true;
}

bool EditorToolkitCMN::InsertNoteInChordMode(const std::string &elementId, data_PITCHNAME pname, int oct)
{
    Object *target = this->GetElement(elementId);
    if (!target) return false;

    Chord *chord = NULL;
    Note *targetNote = NULL;

    if (target->Is(CHORD)) {
        chord = vrv_cast<Chord *>(target);
    }
    else if (target->Is(NOTE)) {
        targetNote = vrv_cast<Note *>(target);
        assert(targetNote);
        chord = targetNote->IsChordTone();
    }

    if (!chord && !targetNote) return false;

    if (!chord) {
        if (targetNote->HasEditorialContent()) {
            LogInfo("Inserting a note where a note has editorial content is not possible");
            return false;
        }

        ListOfObjects lyric;
        ClassIdsComparison lyricsComparison({ VERSE, SYL });
        targetNote->FindAllDescendantsByComparison(&lyric, &lyricsComparison);
        if (!lyric.empty()) {
            LogInfo("Inserting a note where a note has lyric content is not possible");
            return false;
        }
        chord = new Chord();
        chord->DurationInterface::operator=(*targetNote);
        chord->AttCue::operator=(*targetNote);
        chord->AttGraced::operator=(*targetNote);
        chord->AttStems::operator=(*targetNote);
        chord->AttStemsCmn::operator=(*targetNote);
        targetNote->DurationInterface::Reset();
        targetNote->ResetCue();
        targetNote->ResetGraced();
        targetNote->ResetStems();
        targetNote->ResetStemsCmn();
        Object *parent = targetNote->GetParent();
        assert(parent);
        parent->ReplaceChild(targetNote, chord);
        chord->AddChild(targetNote);

        ListOfObjects artics = targetNote->FindAllDescendantsByType(ARTIC);
        for (Object *artic : artics) {
            artic->MoveItselfTo(chord);
        }
        targetNote->ClearRelinquishedChildren();
    }

    Note *note = vrv_cast<Note *>(this->PrepareInsertion(chord, "note"));
    if (!note) return false;

    note->SetPname(pname);
    note->SetOct(oct);

    chord->AddChild(note);

    this->ClearContext();
    this->SetEditStatus();

    return true;
}

#endif

} // namespace vrv

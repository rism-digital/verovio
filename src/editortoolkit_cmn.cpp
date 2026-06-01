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
#include "note.h"

namespace vrv {

EditorToolkitCMN::EditorToolkitCMN(Doc *doc, View *view) : EditorToolkitShared(doc, view) {}

EditorToolkitCMN::~EditorToolkitCMN() {}

bool EditorToolkitCMN::ParseEditorCMNAction(const jsonxx::Object &json)
{
    std::string action = json.get<jsonxx::String>("action");

#ifndef NO_EDIT_SUPPORT
    if (action == "insertNote") {
        std::string contextId;
        data_PITCHNAME pname;
        int oct;
        data_DURATION dur;
        bool chordMode;
        if (this->ParseInsertNoteAction(json.get<jsonxx::Object>("param"), contextId, pname, oct, dur, chordMode)) {
            this->PrepareUndo();
            // LogInfo("%s %s %s", elementName.c_str(), elementId.c_str(), insertMode.c_str());
            return (this->InsertNote(contextId, pname, oct, dur, chordMode));
        }
        LogWarning("Could not parse the insertNote action");
    }
    return false;

#endif
    return false;
}

bool EditorToolkitCMN::ParseInsertNoteAction(
    jsonxx::Object param, std::string &targetId, data_PITCHNAME &pname, int &oct, data_DURATION &dur, bool &chordMode)
{
    chordMode = false;
    pname = PITCHNAME_NONE;
    oct = VRV_UNSET;
    dur = DURATION_NONE;
    Note note;
    if (!param.has<jsonxx::String>("targetId")) return false;
    targetId = param.get<jsonxx::String>("targetId");
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

bool EditorToolkitCMN::InsertNote(
    const std::string &targetId, data_PITCHNAME pname, int oct, data_DURATION dur, bool chordMode)
{
    if (chordMode) return this->InsertNoteInChordMode(targetId, pname, oct);

    Object *target = this->GetElement(targetId);
    if (!target || !target->Is({ CHORD, LAYER, NOTE })) return false;

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

    if (previousElement) {
        targetContainer->InsertAfter(previousElement, note);
    }
    else {
        targetContainer->AddChild(note);
    }

    if (note->IsInBeam()) {
        note->SetDur(std::max(DURATION_8, dur));
    }

    return true;
}

bool EditorToolkitCMN::InsertNoteInChordMode(const std::string &targetId, data_PITCHNAME pname, int oct)
{
    Object *target = this->GetElement(targetId);
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

    return true;
}

} // namespace vrv

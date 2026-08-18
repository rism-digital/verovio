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

#include "alignfunctor.h"
#include "comparison.h"
#include "cursor.h"
#include "layer.h"
#include "miscfunctor.h"
#include "note.h"
#include "rest.h"
#include "staff.h"
#include "tie.h"

namespace vrv {

EditorToolkitCMN::EditorToolkitCMN(Doc *doc, View *view) : EditorToolkitShared(doc, view) {}

EditorToolkitCMN::~EditorToolkitCMN() {}

#ifndef NO_EDIT_SUPPORT
bool EditorToolkitCMN::ParseEditorCMNAction(const jsonxx::Object &json)
{
    std::string action = json.get<jsonxx::String>("action");

    if (action == "insertCursorByDur") {
        data_DURATION dur;
        int dots;
        if (this->ParseInsertCursorByDurAction(json.get<jsonxx::Object>("param"), dur, dots)) {
            this->PrepareUndo();
            return (this->InsertCursorByDur(dur, dots));
        }
        LogWarning("Could not parse the insertCursorByDur action");
    }
    else if (action == "insertCursorByPitch") {
        data_PITCHNAME pname;
        int oct;
        data_ACCIDENTAL_WRITTEN accid;
        int midi;
        if (this->ParseInsertCursorByPitchAction(json.get<jsonxx::Object>("param"), pname, oct, accid, midi)) {
            this->PrepareUndo();
            return (this->InsertCursorByPitch(pname, oct, accid, midi));
        }
        LogWarning("Could not parse the insertCursorByPitch action");
    }
    else if (action == "insertCursorByType") {
        CursorInsertType insertType;
        if (this->ParseInsertCursorByTypeAction(json.get<jsonxx::Object>("param"), insertType)) {
            this->PrepareUndo();
            return (this->InsertCursorByType(insertType));
        }
        LogWarning("Could not parse the insertCursorByType action");
    }
    else if (action == "insertCursorContainer") {
        CursorContainer container;
        if (this->ParseInsertCursorContainerAction(json.get<jsonxx::Object>("param"), container)) {
            this->PrepareUndo();
            return (this->InsertCursorContainer(container));
        }
        LogWarning("Could not parse the insertCursorContainer action");
    }
    else if (action == "insertMeasure") {
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
        data_ACCIDENTAL_WRITTEN accid;
        data_ACCIDENTAL_GESTURAL accidGes;
        data_DURATION dur;
        int dots;
        bool chordMode;
        if (this->ParseInsertNoteAction(
                json.get<jsonxx::Object>("param"), elementId, pname, oct, accid, accidGes, dur, dots, chordMode)) {
            this->PrepareUndo();
            return (this->InsertNote(elementId, pname, oct, accid, accidGes, dur, dots, chordMode));
        }
        LogWarning("Could not parse the insertNote action");
    }
    else if (action == "insertRest") {
        std::string elementId;
        data_DURATION dur;
        int dots;
        if (this->ParseInsertRestAction(json.get<jsonxx::Object>("param"), elementId, dur, dots)) {
            this->PrepareUndo();
            return (this->InsertRest(elementId, dur, dots));
        }
        LogWarning("Could not parse the insertRest action");
    }
    else if (action == "resetCursorContainer") {
        CursorContainer container;
        if (this->ParseResetCursorContainerAction(json.get<jsonxx::Object>("param"), container)) {
            this->PrepareUndo();
            return (this->ResetCursorContainer(container));
        }
        LogWarning("Could not parse the resetCursorContainer action");
    }
    return false;
}

bool EditorToolkitCMN::ParseInsertCursorByDurAction(const jsonxx::Object &param, data_DURATION &dur, int &dots)
{
    dur = DURATION_NONE;
    dots = VRV_UNSET;
    Note noteConverter;

    if (!param.has<jsonxx::String>("dur")) return false;
    dur = noteConverter.AttDurationLog::StrToDuration(param.get<jsonxx::String>("dur"));

    if (param.has<jsonxx::Number>("dots")) dots = param.get<jsonxx::Number>("dots");

    return true;
}

bool EditorToolkitCMN::ParseInsertCursorByPitchAction(
    const jsonxx::Object &param, data_PITCHNAME &pname, int &oct, data_ACCIDENTAL_WRITTEN &accid, int &midi)
{
    pname = PITCHNAME_NONE;
    oct = VRV_UNSET;
    accid = ACCIDENTAL_WRITTEN_NONE;
    midi = VRV_UNSET;
    Note noteConverter;
    Accid accidConverter;

    // At least one of the two
    if (param.has<jsonxx::String>("pname")) {
        pname = noteConverter.AttPitch::StrToPitchname(param.get<jsonxx::String>("pname"));
    }
    else if (param.has<jsonxx::Number>("midi")) {
        midi = param.get<jsonxx::Number>("midi");
    }
    else {
        return false;
    }

    if (param.has<jsonxx::Number>("oct")) oct = param.get<jsonxx::Number>("oct");
    if (param.has<jsonxx::String>("accid"))
        accid = accidConverter.AttAccidental::StrToAccidentalWritten(param.get<jsonxx::String>("accid"));

    return true;
}

bool EditorToolkitCMN::ParseInsertCursorByTypeAction(const jsonxx::Object &param, CursorInsertType &insertType)
{
    insertType = CursorInsertType::CURSOR_INSERT_NONE;

    if (!param.has<jsonxx::String>("type")) return false;

    if (param.get<jsonxx::String>("type") == "rest") {
        insertType = CURSOR_INSERT_REST;
    }
    else if (param.get<jsonxx::String>("type") == "tie") {
        insertType = CURSOR_INSERT_TIE;
    }
    else if (param.get<jsonxx::String>("type") == "copy") {
        insertType = CURSOR_INSERT_COPY;
    }
    else {
        return false;
    }
    return true;
}

bool EditorToolkitCMN::ParseInsertCursorContainerAction(const jsonxx::Object &param, CursorContainer &container)
{
    container = CursorContainer::CURSOR_CONTAINER_NONE;

    if (!param.has<jsonxx::String>("container")) return false;

    if (param.get<jsonxx::String>("container") == "tuplet") {
        container = CURSOR_CONTAINER_TUPLET;
    }
    else if (param.get<jsonxx::String>("container") == "graceGrp") {
        container = CURSOR_CONTAINER_GRACEGRP;
    }
    else {
        return false;
    }
    return true;
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
    int &oct, data_ACCIDENTAL_WRITTEN &accid, data_ACCIDENTAL_GESTURAL &accidGes, data_DURATION &dur, int &dots,
    bool &chordMode)
{
    chordMode = false;
    pname = PITCHNAME_NONE;
    oct = VRV_UNSET;
    accid = ACCIDENTAL_WRITTEN_NONE;
    accidGes = ACCIDENTAL_GESTURAL_NONE;
    dur = DURATION_NONE;
    dots = VRV_UNSET;
    Note noteConverter;
    Accid accidConverter;

    if (!param.has<jsonxx::String>("elementId")) return false;
    elementId = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::String>("pname")) return false;
    pname = noteConverter.AttPitch::StrToPitchname(param.get<jsonxx::String>("pname"));
    if (!param.has<jsonxx::Number>("oct")) return false;
    oct = param.get<jsonxx::Number>("oct");

    if (param.has<jsonxx::String>("accid"))
        accid = accidConverter.AttAccidental::StrToAccidentalWritten(param.get<jsonxx::String>("accid"));
    if (param.has<jsonxx::String>("accidGes"))
        accidGes = accidConverter.AttAccidentalGes::StrToAccidentalGestural(param.get<jsonxx::String>("accidGes"));

    // At least one of the two
    if (!param.has<jsonxx::String>("dur") && !param.has<jsonxx::Boolean>("chordMode")) return false;

    if (param.has<jsonxx::String>("dur")) dur = noteConverter.AttPitch::StrToDuration(param.get<jsonxx::String>("dur"));

    if (param.has<jsonxx::Number>("dots")) dots = param.get<jsonxx::Number>("dots");

    if (param.has<jsonxx::Boolean>("chordMode")) chordMode = param.get<jsonxx::Boolean>("chordMode");

    return true;
}

bool EditorToolkitCMN::ParseInsertRestAction(
    const jsonxx::Object &param, std::string &elementId, data_DURATION &dur, int &dots)
{
    dur = DURATION_NONE;
    dots = VRV_UNSET;
    Note noteConverter;

    if (!param.has<jsonxx::String>("elementId")) return false;
    elementId = param.get<jsonxx::String>("elementId");

    if (!param.has<jsonxx::String>("dur")) return false;
    dur = noteConverter.AttPitch::StrToDuration(param.get<jsonxx::String>("dur"));

    if (param.has<jsonxx::Number>("dots")) dots = param.get<jsonxx::Number>("dots");

    return true;
}

bool EditorToolkitCMN::ParseResetCursorContainerAction(const jsonxx::Object &param, CursorContainer &container)
{
    container = CursorContainer::CURSOR_CONTAINER_NONE;

    if (!param.has<jsonxx::String>("container")) return false;

    if (param.get<jsonxx::String>("container") == "tuplet") {
        container = CURSOR_CONTAINER_TUPLET;
    }
    else if (param.get<jsonxx::String>("container") == "graceGrp") {
        container = CURSOR_CONTAINER_GRACEGRP;
    }
    else {
        return false;
    }
    return true;
}

bool EditorToolkitCMN::InsertCursorByDur(data_DURATION dur, int dots)
{
    if (!this->InsertMode()) return false;

    if (m_cursor->GetInputMode() != Cursor::InputMode::PITCH_FIRST) return false;

    data_PITCHNAME pname = (m_cursor->HasPname()) ? m_cursor->GetPname() : PITCHNAME_c;
    int oct = (m_cursor->HasOct()) ? m_cursor->GetOct() : 3;
    auto [accid, accidGes] = m_cursor->GetAccidValue();

    if (dots == VRV_UNSET && m_cursor->HasDots()) dots = m_cursor->GetDots();

    std::string id = m_cursor->GetID();

    if (m_cursor->IsTieMode()) {
        return this->CopyCursorPosition(dur, dots, (m_cursor->GetTieMode() == Cursor::TieMode::TIE));
    }
    else if (m_cursor->IsRestMode()) {
        return this->InsertRest(id, dur, dots);
    }
    else {
        return this->InsertNote(id, pname, oct, accid, accidGes, dur, dots, m_cursor->IsChordMode());
    }
}

bool EditorToolkitCMN::InsertCursorByPitch(data_PITCHNAME pname, int oct, data_ACCIDENTAL_WRITTEN accid, int midi)
{
    if (!this->InsertMode()) return false;

    if (m_cursor->GetInputMode() != Cursor::InputMode::DURATION_FIRST) return false;

    if (midi != VRV_UNSET) {
        std::string placeholder = m_cursor->GetID();
        this->UpdatePitch(placeholder, PITCHNAME_NONE, VRV_UNSET, ACCIDENTAL_WRITTEN_NONE, midi);
        pname = m_cursor->GetPname();
        oct = VRV_UNSET;
        accid = ACCIDENTAL_WRITTEN_NONE;
    }

    if (oct == VRV_UNSET) oct = m_cursor->GetOct();

    // Since we did not know the pitch yet we need to calculate that actual accid
    if (pname != PITCHNAME_NONE) m_cursor->SetPname(pname);

    data_ACCIDENTAL_GESTURAL accidGes = ACCIDENTAL_GESTURAL_NONE;
    auto [actualAccid, isImplicit]
        = this->GetActualAccid(m_cursor, (accid == ACCIDENTAL_WRITTEN_NONE) ? m_cursor->GetAccid() : accid);
    m_cursor->SetAccid(actualAccid);
    m_cursor->SetAccidImplicit(isImplicit);
    const auto value = m_cursor->GetAccidValue();
    accid = value.first;
    accidGes = value.second;

    data_DURATION dur = (m_cursor->HasDur()) ? m_cursor->GetDur() : DURATION_4;
    int dots = (m_cursor->HasDots()) ? m_cursor->GetDots() : VRV_UNSET;

    std::string id = m_cursor->GetID();

    return this->InsertNote(id, pname, oct, accid, accidGes, dur, dots, m_cursor->IsChordMode());
}

bool EditorToolkitCMN::InsertCursorByType(CursorInsertType insertType)
{
    if (!this->InsertMode()) return false;

    if (m_cursor->GetInputMode() != Cursor::InputMode::DURATION_FIRST) return false;

    data_DURATION dur = (m_cursor->HasDur()) ? m_cursor->GetDur() : DURATION_4;
    int dots = (m_cursor->HasDots()) ? m_cursor->GetDots() : VRV_UNSET;

    std::string id = m_cursor->GetID();

    m_cursor->SetChordMode(Cursor::ChordMode::CHORD_NONE);

    if (insertType == CURSOR_INSERT_REST) {
        return this->InsertRest(id, dur, dots);
    }
    else {
        return (this->CopyCursorPosition(m_cursor->GetDur(), m_cursor->GetDots(), (insertType == CURSOR_INSERT_TIE)));
    }
}

bool EditorToolkitCMN::InsertCursorContainer(CursorContainer container)
{
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

bool EditorToolkitCMN::InsertNote(const std::string &elementId, data_PITCHNAME pname, int oct,
    data_ACCIDENTAL_WRITTEN accid, data_ACCIDENTAL_GESTURAL accidGes, data_DURATION dur, int dots, bool chordMode)
{
    if (chordMode && (!this->InsertMode() || m_cursor->GetChordMode() != Cursor::ChordMode::NEW)) {
        return this->InsertNoteInChordMode(elementId, pname, oct, accid, accidGes);
    }

    Object *target = NULL;
    if (this->InsertMode()) {
        target = (m_cursor->HasPosition()) ? m_cursor->GetPosition() : m_cursor->GetParent();
    }
    else {
        target = this->GetElement(elementId);
    }
    if (!target || !target->IsAnyOf(std::array{ CHORD, LAYER, NOTE, REST })) return false;

    if (target->Is(NOTE)) {
        Note *note = vrv_cast<Note *>(target);
        if (note->IsChordTone()) target = note->IsChordTone();
    }

    auto [targetContainer, previousElement] = this->GetTargetContainerFor(target);
    if (!targetContainer) return false;

    Note *note = vrv_cast<Note *>(this->PrepareInsertion(targetContainer, "note"));
    if (!note) return false;

    this->SetNoteAttributes(note, pname, oct, accid, accidGes);

    note->SetDur(dur);
    if (dots != VRV_UNSET) {
        note->SetDots(dots);
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
    else if (this->InsertMode() && (note->GetDur() > DURATION_4)) {
        this->AutoBeam(note);
    }

    this->ClearContext();
    this->SetEditStatus();

    if (this->InsertMode()) {
        this->MoveCursor(note);
        if (chordMode) {
            m_cursor->AdjustPitchByOffset(4);
            std::string placeholder = m_cursor->GetID();
            this->UpdatePitch(
                placeholder, m_cursor->GetPname(), m_cursor->GetOct(), ACCIDENTAL_WRITTEN_NONE, VRV_UNSET);
        }
        if (m_cursor->GetInputMode() == Cursor::InputMode::DURATION_FIRST) {
            m_cursor->SetAccid(ACCIDENTAL_WRITTEN_NONE);
            m_cursor->SetAccidImplicit(false);
        }
    }

    return true;
}

bool EditorToolkitCMN::ResetCursorContainer(CursorContainer container)
{
    return true;
}

std::pair<Object *, Object *> EditorToolkitCMN::GetTargetContainerFor(Object *target)
{
    Object *previousElement = NULL;
    Object *targetContainer = NULL;
    if (!target->Is(LAYER)) {
        Object *targetParent = target->GetParent();
        // Inserting a note within a tuplet or a beam
        if (targetParent && targetParent->IsAnyOf(std::array{ BEAM, TUPLET }) && targetParent->GetLast() != target) {
            previousElement = target;
            targetContainer = targetParent;
        }
        // Otherwise always insert in the layer
        else {
            previousElement = target->GetLastAncestorNot(LAYER);
            if (!previousElement) return { NULL, NULL };
            targetContainer = previousElement->GetParent();
            assert(targetContainer && targetContainer->Is(LAYER));
        }
    }
    else {
        targetContainer = target;
    }
    return { targetContainer, previousElement };
}

bool EditorToolkitCMN::InsertNoteInChordMode(const std::string &elementId, data_PITCHNAME pname, int oct,
    data_ACCIDENTAL_WRITTEN accid, data_ACCIDENTAL_GESTURAL accidGes)
{
    Object *target = NULL;
    if (this->InsertMode()) {
        target = (m_cursor->HasPosition()) ? m_cursor->GetPosition() : m_cursor->GetParent();
    }
    else {
        target = this->GetElement(elementId);
    }
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

        if (this->InsertMode()) m_cursor->SetPosition(chord);
    }

    Note *note = vrv_cast<Note *>(this->PrepareInsertion(chord, "note"));
    if (!note) return NULL;

    this->SetNoteAttributes(note, pname, oct, accid, accidGes);

    chord->AddChild(note);

    if (this->InsertMode()) {
        if (m_cursor->GetInputMode() == Cursor::PITCH_FIRST) m_cursor->AdjustPitchByOffset(4);
        std::string placeholder = m_cursor->GetID();
        this->UpdatePitch(placeholder, m_cursor->GetPname(), m_cursor->GetOct(), ACCIDENTAL_WRITTEN_NONE, VRV_UNSET);
    }

    this->ClearContext();
    this->SetEditStatus();

    return true;
}

void EditorToolkitCMN::SetNoteAttributes(
    Note *note, data_PITCHNAME pname, int oct, data_ACCIDENTAL_WRITTEN accid, data_ACCIDENTAL_GESTURAL accidGes)
{
    assert(note);

    note->SetPname(pname);
    note->SetOct(oct);

    if (accid != ACCIDENTAL_WRITTEN_NONE) {
        Accid *accidElement = new Accid();
        accidElement->SetAccid(accid);
        note->AddChild(accidElement);
    }
    else if (accidGes != ACCIDENTAL_GESTURAL_NONE) {
        Accid *accidElement = new Accid();
        accidElement->SetAccidGes(accidGes);
        note->AddChild(accidElement);
    }
}

bool EditorToolkitCMN::InsertRest(const std::string &elementId, data_DURATION dur, int dots)
{
    Object *target = NULL;
    if (this->InsertMode()) {
        target = (m_cursor->HasPosition()) ? m_cursor->GetPosition() : m_cursor->GetParent();
    }
    else {
        target = this->GetElement(elementId);
    }
    if (!target || !target->IsAnyOf(std::array{ CHORD, LAYER, NOTE, REST })) return false;

    if (target->Is(NOTE)) {
        Note *note = vrv_cast<Note *>(target);
        if (note->IsChordTone()) target = note->IsChordTone();
    }

    auto [targetContainer, previousElement] = this->GetTargetContainerFor(target);
    if (!targetContainer) return false;

    Rest *rest = vrv_cast<Rest *>(this->PrepareInsertion(targetContainer, "rest"));
    if (!rest) return false;

    rest->SetDur(dur);

    if (dots != VRV_UNSET) {
        rest->SetDots(dots);
    }

    if (previousElement) {
        targetContainer->InsertAfter(previousElement, rest);
    }
    else {
        targetContainer->InsertChild(rest, 0);
    }

    if (rest->IsInBeam()) {
        rest->SetDur(std::max(DURATION_8, dur));
    }
    else if (this->InsertMode() && (rest->GetDur() > DURATION_4)) {
        this->AutoBeam(rest);
    }

    this->ClearContext();
    this->SetEditStatus();

    if (InsertMode()) this->MoveCursor(rest);

    return true;
}

void EditorToolkitCMN::AutoBeam(LayerElement *noteOrRest)
{
    assert(m_cursor);

    // Not sure we actually want to autobeam rest - disabled for now
    // if (!noteOrRest->IsAnyOf(std::array{NOTE, REST})) return;
    if (!noteOrRest->IsAnyOf(std::array{ CHORD, NOTE })) return;

    Layer *layer = vrv_cast<Layer *>(noteOrRest->GetFirstAncestor(LAYER));
    assert(layer);

    // Auto-beam with chord and notes only - rests could be added
    static auto classIds = { CHORD, NOTE };

    LayerElement *result = noteOrRest;

    while (result) {
        result = layer->GetPreviousInLayer(result);

        if (!result || (noteOrRest->GetAlignment() == result->GetAlignment())) continue;

        if (result->IsAnyOf(classIds)) break;
    }

    if (!result) return;

    if (result->Is(NOTE)) {
        Note *previousNote = vrv_cast<Note *>(result);
        assert(noteOrRest);
        LayerElement *chord = previousNote->IsChordTone();
        if (chord) result = chord;
    }

    DurationInterface *interface = result->GetDurationInterface();
    assert(interface);

    if (interface->GetDur() < DURATION_8) return;

    AlignMeterParams params;
    params.meterSig = layer->GetCurrentMeterSig();
    assert(params.meterSig);
    const int meterCount = (params.meterSig->GetTotalCount() == 0) ? 4 : params.meterSig->GetTotalCount();
    const int meterUnit = (params.meterSig->GetUnit() == VRV_UNSET) ? meterCount : params.meterSig->GetUnit();

    Fraction position = (m_cursor->GetAlignment()) ? m_cursor->GetAlignment()->GetTime() : 0;
    // Use compound-meter grouping for meters such as 6/8, 9/8 and 12/8.
    // Simple meters use one denominator unit per beat:
    //   4/4 -> 4 groups of 1/4
    // Compound meters use groups of three denominator units:
    //   6/8 -> 2 groups of 3/8

    const bool isCompoundMeter = ((meterCount % 3 == 0) && params.meterSig->GetUnit() == 8);

    Fraction beatDuration = Fraction(1, meterUnit);
    if (isCompoundMeter) beatDuration = beatDuration * 3;

    // A note beginning on a new beat must not be joined to the preceding
    // beam. Do not apply this at the beginning of the measure.
    if (position > 0) {
        const Fraction beatPosition = position / beatDuration;
        if (beatPosition.GetDenominator() == 1) return;
    }

    if (result->IsInBeam()) {
        Object *previousParent = noteOrRest->GetParent();
        Beam *beam = result->GetAncestorBeam();
        noteOrRest->MoveItselfTo(beam);
        previousParent->ClearRelinquishedChildren();
    }
    else {
        Object *previousParent = result->GetParent();
        assert(previousParent);
        Beam *beam = new Beam();
        previousParent->AddChild(beam);
        result->MoveItselfTo(beam);
        noteOrRest->MoveItselfTo(beam);
        previousParent->ClearRelinquishedChildren();
    }
}

bool EditorToolkitCMN::CopyCursorPosition(data_DURATION dur, int dots, bool tie)
{
    if (!this->InsertMode()) return false;

    const LayerElement *copyFrom = (m_cursor->HasPosition()) ? m_cursor->GetPosition() : NULL;
    if (!copyFrom) {
        const Layer *layer = vrv_cast<const Layer *>(m_cursor->GetParent());
        assert(layer);
        const Layer *previousLayer = this->GetPreviousLayer(layer);
        if (previousLayer) {
            ClassIdsComparison comparison({ CHORD, NOTE });
            copyFrom = vrv_cast<const LayerElement *>(
                previousLayer->FindDescendantByComparison(&comparison, UNLIMITED_DEPTH, BACKWARD));
        }
    }

    if (!copyFrom || !copyFrom->IsAnyOf(std::array{ CHORD, NOTE })) return false;

    // Make sure we copy the whole chord
    if (copyFrom->Is(NOTE)) {
        const Note *note = vrv_cast<const Note *>(copyFrom);
        if (note->IsChordTone()) copyFrom = note->IsChordTone();
    }

    LayerElement *copy = vrv_cast<LayerElement *>(copyFrom->Clone());
    copy->CloneReset();
    DurationInterface *durInterface = copy->GetDurationInterface();
    assert(durInterface);
    durInterface->SetDur(dur);
    durInterface->SetDots(dots);

    Object *target = (m_cursor->HasPosition()) ? m_cursor->GetPosition() : m_cursor->GetParent();
    if (target->Is(NOTE)) {
        Note *note = vrv_cast<Note *>(target);
        if (note->IsChordTone()) target = note->IsChordTone();
    }

    auto [targetContainer, previousElement] = this->GetTargetContainerFor(target);
    if (!targetContainer) return false;

    if (previousElement) {
        targetContainer->InsertAfter(previousElement, copy);
    }
    else {
        targetContainer->InsertChild(copy, 0);
    }
    if (tie) TieElements(copyFrom, copy);

    if (copy->IsInBeam()) {
        durInterface->SetDur(std::max(DURATION_8, dur));
    }
    else if (durInterface->GetDur() > DURATION_4) {
        this->AutoBeam(copy);
    }

    if (tie || copyFrom->GetFirstAncestor(MEASURE) == copy->GetFirstAncestor(MEASURE)) {
        if (copy->Is(CHORD)) {
            ListOfObjects endNotes = copy->FindAllDescendantsByType(NOTE);
            for (auto object : endNotes) {
                Note *note = vrv_cast<Note *>(object);
                assert(note);
                Accid *accid = note->GetDrawingAccid();
                if (accid && accid->HasAccid()) {
                    accid->SetAccidGes(note->AccidentalWrittenToGestural(accid->GetAccid()));
                    accid->ResetAccidental();
                }
            }
        }
        else {
            Note *note = vrv_cast<Note *>(copy);
            assert(note);
            Accid *accid = note->GetDrawingAccid();
            if (accid && accid->HasAccid()) {
                accid->SetAccidGes(note->AccidentalWrittenToGestural(accid->GetAccid()));
                accid->ResetAccidental();
            }
        }
    }

    this->ClearContext();
    this->SetEditStatus();

    this->MoveCursor(copy);
    if (m_cursor->GetInputMode() == Cursor::InputMode::DURATION_FIRST) {
        m_cursor->SetAccid(ACCIDENTAL_WRITTEN_NONE);
        m_cursor->SetAccidImplicit(false);
    }

    return true;
}

void EditorToolkitCMN::TieElements(const Object *start, const Object *end)
{
    assert(start);
    assert(end);

    // Make sure the tie is between notes or between chords
    if (!start->IsAnyOf(std::array{ CHORD, NOTE })) return;
    if (!end->IsAnyOf(std::array{ CHORD, NOTE })) return;
    if (start->GetClassId() != end->GetClassId()) return;

    Object *measure = const_cast<Object *>(start->GetFirstAncestor(MEASURE));
    assert(measure);

    if (end->Is(CHORD)) {
        ListOfConstObjects startNotes = start->FindAllDescendantsByType(NOTE);
        ListOfConstObjects endNotes = end->FindAllDescendantsByType(NOTE);
        // No note, or not the same number of notes, which should never happen because tied chords are copied
        if (startNotes.empty() || (startNotes.size() != endNotes.size())) return;
        ListOfConstObjects::const_iterator startIter, endIter;
        for (startIter = startNotes.begin(), endIter = endNotes.begin(); startIter != startNotes.end();
            ++startIter, ++endIter) {
            Tie *tie = new Tie();
            measure->AddChild(tie);
            tie->SetStartid("#" + (*startIter)->GetID());
            tie->SetEndid("#" + (*endIter)->GetID());
        }
    }
    else {
        Tie *tie = new Tie();
        measure->AddChild(tie);
        tie->SetStartid("#" + start->GetID());
        tie->SetEndid("#" + end->GetID());
    }
}

#endif

} // namespace vrv

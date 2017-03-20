/////////////////////////////////////////////////////////////////////////////
// Name:        note.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "note.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "artic.h"
#include "attcomparison.h"
#include "doc.h"
#include "editorial.h"
#include "elementpart.h"
#include "functorparams.h"
#include "layer.h"
#include "slur.h"
#include "smufl.h"
#include "staff.h"
#include "syl.h"
#include "tie.h"
#include "verse.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Note
//----------------------------------------------------------------------------

Note::Note()
    : LayerElement("note-")
    , StemmedDrawingInterface()
    , DurationInterface()
    , PitchInterface()
    , AttColor()
    , AttColoration()
    , AttGraced()
    , AttNoteLogMensural()
    , AttRelativesize()
    , AttStems()
    , AttStemsCmn()
    , AttTiepresent()
    , AttVisibility()
{
    RegisterInterface(DurationInterface::GetAttClasses(), DurationInterface::IsInterface());
    RegisterInterface(PitchInterface::GetAttClasses(), PitchInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_COLORATION);
    RegisterAttClass(ATT_GRACED);
    RegisterAttClass(ATT_NOTELOGMENSURAL);
    RegisterAttClass(ATT_RELATIVESIZE);
    RegisterAttClass(ATT_STEMS);
    RegisterAttClass(ATT_STEMSCMN);
    RegisterAttClass(ATT_TIEPRESENT);
    RegisterAttClass(ATT_VISIBILITY);

    m_drawingTieAttr = NULL;

    Reset();
}

Note::~Note()
{
    // This deletes the Tie, Slur, and Accid objects if necessary
    if (m_drawingTieAttr) {
        delete m_drawingTieAttr;
    }
}

void Note::Reset()
{
    LayerElement::Reset();
    StemmedDrawingInterface::Reset();
    DurationInterface::Reset();
    PitchInterface::Reset();
    ResetColor();
    ResetColoration();
    ResetGraced();
    ResetNoteLogMensural();
    ResetRelativesize();
    ResetStems();
    ResetStemsCmn();
    ResetTiepresent();
    ResetVisibility();

    // tie pointers
    ResetDrawingTieAttr();

    d_stemLen = 0;
    m_clusterPosition = 0;
    m_cluster = NULL;

    m_playingOnset = 0.0;
    m_playingOffset = 0.0;
}

void Note::AddChild(Object *child)
{
    // additional verification for accid and artic - this will no be raised with editorial markup, though
    if (child->Is(ACCID)) {
        IsAttributeComparison isAttributeComparison(ACCID);
        if (this->FindChildByAttComparison(&isAttributeComparison))
            LogWarning("Having both @accid or @accid.ges and <accid> child will cause problems");
    }
    else if (child->Is(ARTIC)) {
        IsAttributeComparison isAttributeComparison(ARTIC);
        if (this->FindChildByAttComparison(&isAttributeComparison))
            LogWarning("Having both @artic and <artic> child will cause problems");
    }

    if (child->Is(ACCID)) {
        assert(dynamic_cast<Accid *>(child));
    }
    else if (child->Is(ARTIC)) {
        assert(dynamic_cast<Artic *>(child));
    }
    else if (child->Is(NOTEHEAD)) {
        assert(dynamic_cast<NoteHead *>(child));
    }
    else if (child->Is(STEM)) {
        assert(dynamic_cast<Stem *>(child));
    }
    else if (child->Is(SYL)) {
        assert(dynamic_cast<Syl *>(child));
    }
    else if (child->Is(VERSE)) {
        assert(dynamic_cast<Verse *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    // Stem are always added by PrepareLayerElementParts (for now) and we want them to be in the front
    // for the drawing order in the SVG output
    if (child->Is(STEM))
        m_children.insert(m_children.begin(), child);
    else
        m_children.push_back(child);
    Modify();
}

void Note::SetDrawingTieAttr()
{
    assert(!this->m_drawingTieAttr);
    if (m_drawingTieAttr) return;
    m_drawingTieAttr = new Tie();
    m_drawingTieAttr->SetStart(this);
}

void Note::ResetDrawingTieAttr()
{
    if (m_drawingTieAttr) {
        delete m_drawingTieAttr;
        m_drawingTieAttr = NULL;
    }
}

Accid *Note::GetDrawingAccid()
{
    Accid *accid = dynamic_cast<Accid *>(this->FindChildByType(ACCID));
    return accid;
}

Chord *Note::IsChordTone() const
{
    return dynamic_cast<Chord *>(this->GetFirstParent(CHORD, MAX_CHORD_DEPTH));
}

int Note::GetDrawingDur()
{
    Chord *chordParent = dynamic_cast<Chord *>(this->GetFirstParent(CHORD, MAX_CHORD_DEPTH));
    if (chordParent) {
        return chordParent->GetActualDur();
    }
    else {
        return GetActualDur();
    }
}

bool Note::IsClusterExtreme() const
{
    ChordCluster *cluster = this->m_cluster;
    if (this == cluster->at(0)) return true;
    if (this == cluster->at(cluster->size() - 1))
        return true;
    else
        return false;
}

Point Note::GetStemUpSE(Doc *doc, int staffSize, bool graceSize)
{
    double x = 0.0;
    double y = 312.5;

    Glyph *glyph = Resources::GetGlyph(SMUFL_E0A3_noteheadHalf);
    assert(glyph);
    Point p = doc->ConvertFontPoint(glyph, x, y, staffSize, graceSize);
    return p;
}

Point Note::GetStemDownNW(Doc *doc, int staffSize, bool graceSize)
{
    double x = 0.0;
    double y = -312.5;

    Glyph *glyph = Resources::GetGlyph(SMUFL_E0A3_noteheadHalf);
    assert(glyph);
    Point p = doc->ConvertFontPoint(glyph, x, y, staffSize, graceSize);
    return p;
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Note::CalcDrawingStemDir(FunctorParams *functorParams)
{
    CalcDrawingStemDirParams *params = dynamic_cast<CalcDrawingStemDirParams *>(functorParams);
    assert(params);

    // Set it to NULL first
    params->m_currentNote = NULL;

    // Stems have been calculated previously in Beam or FTrem - siblings becasue flags do not need to
    // be processed either
    if (this->IsInBeam() || this->IsInFTrem()) {
        return FUNCTOR_SIBLINGS;
    }

    if (this->IsChordTone()) {
        assert(params->m_currentChord);
        return FUNCTOR_CONTINUE;
    }

    // This now need should be NULL;
    params->m_currentChord = NULL;

    // No stem
    if (this->GetDur() < DUR_2) {
        LogDebug("Duration is longer than halfnote, there should be no stem");
        return FUNCTOR_SIBLINGS;
    }

    Stem *stem = this->GetDrawingStem();
    if (!stem) {
        LogDebug("Stem is missing, something went wrong");
        return FUNCTOR_SIBLINGS;
    }

    Staff *staff = dynamic_cast<Staff *>(this->GetFirstParent(STAFF));
    assert(staff);
    Layer *layer = dynamic_cast<Layer *>(this->GetFirstParent(LAYER));
    assert(layer);

    if (this->m_crossStaff) staff = this->m_crossStaff;

    // Cache the in params to avoid further lookup
    params->m_currentStaff = staff;
    params->m_currentLayer = layer;
    params->m_currentNote = this;

    int staffSize = staff->m_drawingStaffSize;
    int staffY = staff->GetDrawingY();
    int verticalCenter = staffY - params->m_doc->GetDrawingDoubleUnit(staffSize) * 2;
    bool drawingCueSize = this->IsCueSize();
    int radius = params->m_doc->GetGlyphWidth(SMUFL_E0A3_noteheadHalf, staffSize, drawingCueSize) / 2;
    // adjust the radius in order to take the stem width into account
    radius -= params->m_doc->GetDrawingStemWidth(staffSize) / 2;

    /************ Set the direction ************/

    data_STEMDIRECTION stemDir = STEMDIRECTION_NONE;

    if (stem->HasStemDir()) {
        stemDir = stem->GetStemDir();
    }
    else if (layer->GetDrawingStemDir() != STEMDIRECTION_NONE) {
        stemDir = layer->GetDrawingStemDir();
    }
    else {
        stemDir = (this->GetDrawingY() >= verticalCenter) ? STEMDIRECTION_down : STEMDIRECTION_up;
    }

    this->SetDrawingStemDir(stemDir);

    /************ Set the position and the length ************/

    int baseStem = -params->m_doc->GetDrawingUnit(staffSize) * STANDARD_STEMLENGTH;
    if (drawingCueSize) baseStem = params->m_doc->GetGraceSize(baseStem);

    if (stemDir == STEMDIRECTION_up) {
        Point p = this->GetStemUpSE(params->m_doc, staffSize, drawingCueSize);
        baseStem += p.y;
        stem->SetDrawingYRel(p.y);
        stem->SetDrawingXRel(radius);
        this->SetDrawingStemLen(baseStem);
    }
    else {
        Point p = this->GetStemDownNW(params->m_doc, staffSize, drawingCueSize);
        baseStem -= p.y;
        stem->SetDrawingYRel(p.y);
        stem->SetDrawingXRel(-radius);
        this->SetDrawingStemLen(-baseStem);
    }

    return FUNCTOR_CONTINUE;
}

int Note::PrepareLayerElementParts(FunctorParams *functorParams)
{
    Stem *currentStem = dynamic_cast<Stem *>(this->FindChildByType(STEM));
    Flag *currentFlag = NULL;
    if (currentStem) currentFlag = dynamic_cast<Flag *>(currentStem->FindChildByType(FLAG));

    if ((this->GetDur() > DUR_1) && !this->IsChordTone()) {
        if (!currentStem) {
            currentStem = new Stem();
            this->AddChild(currentStem);
        }
        currentStem->AttStems::operator=(*this);
        currentStem->AttStemsCmn::operator=(*this);
    }
    // This will happen only if the duration has changed
    else if (currentStem) {
        if (this->DeleteChild(currentStem)) {
            currentStem = NULL;
            // The currentFlag (if any) will have been deleted above
            currentFlag = NULL;
        }
    }

    if ((this->GetDur() > DUR_4) && !this->IsInBeam() && !this->IsChordTone()) {
        // We should have a stem at this stage
        assert(currentStem);
        if (!currentFlag) {
            currentFlag = new Flag();
            currentStem->AddChild(currentFlag);
        }
    }
    // This will happen only if the duration has changed (no flag required anymore)
    else if (currentFlag) {
        assert(currentStem);
        if (currentStem->DeleteChild(currentFlag)) currentFlag = NULL;
    }

    SetDrawingStem(currentStem);

    return FUNCTOR_CONTINUE;
};

int Note::PrepareTieAttr(FunctorParams *functorParams)
{
    PrepareTieAttrParams *params = dynamic_cast<PrepareTieAttrParams *>(functorParams);
    assert(params);

    AttTiepresent *check = this;
    // Use the parent chord if there is no @tie on the note
    if (!this->HasTie() && params->m_currentChord) {
        check = params->m_currentChord;
    }
    assert(check);

    std::vector<Note *>::iterator iter = params->m_currentNotes.begin();
    while (iter != params->m_currentNotes.end()) {
        // same octave and same pitch - this is the one!
        if ((this->GetOct() == (*iter)->GetOct()) && (this->GetPname() == (*iter)->GetPname())) {
            // right flag
            if ((check->GetTie() == TIE_m) || (check->GetTie() == TIE_t)) {
                assert((*iter)->GetDrawingTieAttr());
                (*iter)->GetDrawingTieAttr()->SetEnd(this);
            }
            else {
                LogWarning("Expected @tie median or terminal in note '%s', skipping it", this->GetUuid().c_str());
                (*iter)->ResetDrawingTieAttr();
            }
            iter = params->m_currentNotes.erase(iter);
            // we are done for this note
            break;
        }
        iter++;
    }

    if ((check->GetTie() == TIE_m) || (check->GetTie() == TIE_i)) {
        this->SetDrawingTieAttr();
        params->m_currentNotes.push_back(this);
    }

    return FUNCTOR_CONTINUE;
}

int Note::FillStaffCurrentTimeSpanning(FunctorParams *functorParams)
{
    // Pass it to the pseudo functor of the interface
    if (this->m_drawingTieAttr) {
        return this->m_drawingTieAttr->FillStaffCurrentTimeSpanning(functorParams);
    }

    return FUNCTOR_CONTINUE;
}

int Note::PrepareLyrics(FunctorParams *functorParams)
{
    PrepareLyricsParams *params = dynamic_cast<PrepareLyricsParams *>(functorParams);
    assert(params);

    params->m_lastButOneNote = params->m_lastNote;
    params->m_lastNote = this;

    return FUNCTOR_CONTINUE;
}

int Note::PreparePointersByLayer(FunctorParams *functorParams)
{
    PreparePointersByLayerParams *params = dynamic_cast<PreparePointersByLayerParams *>(functorParams);
    assert(params);

    params->m_currentNote = this;

    return FUNCTOR_CONTINUE;
}

int Note::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetDrawing(functorParams);

    this->ResetDrawingTieAttr();

    return FUNCTOR_CONTINUE;
};

} // namespace vrv

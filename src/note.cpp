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
#include "glyph.h"
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
    , AttNoteAnlMensural()
    // FIXME MEI 4.0.0
    //, AttRelativesize()
    , AttStems()
    , AttStemsCmn()
    , AttTiePresent()
    , AttVisibility()
{
    RegisterInterface(DurationInterface::GetAttClasses(), DurationInterface::IsInterface());
    RegisterInterface(PitchInterface::GetAttClasses(), PitchInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_COLORATION);
    RegisterAttClass(ATT_GRACED);
    RegisterAttClass(ATT_NOTEANLMENSURAL);
    // FIXME 4.0.0
    // RegisterAttClass(ATT_RELATIVESIZE);
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
    ResetNoteAnlMensural();
    // FIXME 4.0.0
    // ResetRelativesize();
    ResetStems();
    ResetStemsCmn();
    ResetTiePresent();
    ResetVisibility();

    // tie pointers
    ResetDrawingTieAttr();

    m_clusterPosition = 0;
    m_cluster = NULL;

    m_playingOnset = 0.0;
    m_playingOffset = 0.0;

    m_drawingLoc = 0;
    m_flippedNotehead = false;
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
    else if (child->Is(DOTS)) {
        assert(dynamic_cast<Dots *>(child));
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
    if (child->Is({ DOTS, STEM }))
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

int Note::GetDrawingDur() const
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

bool Note::IsUnissonWith(Note *note, bool ignoreAccid)
{
    if (!ignoreAccid) {
        Accid *accid = this->GetDrawingAccid();
        Accid *noteAccid = note->GetDrawingAccid();
        data_ACCIDENTAL_WRITTEN accidVal = (accid) ? accid->GetAccid() : ACCIDENTAL_WRITTEN_NONE;
        data_ACCIDENTAL_WRITTEN noteAccidVal = (noteAccid) ? noteAccid->GetAccid() : ACCIDENTAL_WRITTEN_NONE;
        if (accidVal != noteAccidVal) return false;
    }

    return ((this->GetPname() == note->GetPname()) && (this->GetOct() == note->GetOct()));
}

void Note::SetCluster(ChordCluster *cluster, int position)
{
    m_cluster = cluster;
    m_clusterPosition = position;
}

int Note::GetDrawingRadius(Doc *doc, int staffSize, bool isCueSize) const
{
    assert(doc);

    wchar_t code = SMUFL_E0A3_noteheadHalf;
    if (this->GetDrawingDur() <= DUR_1) {
        code = SMUFL_E0A2_noteheadWhole;
    }
    return doc->GetGlyphWidth(code, staffSize, isCueSize) / 2;
}

Point Note::GetStemUpSE(Doc *doc, int staffSize, bool graceSize)
{
    int defaultYShift = doc->GetDrawingUnit(staffSize) / 4;
    if (graceSize) defaultYShift = doc->GetCueSize(defaultYShift);
    // x default is always set to the radius for now
    int radius = doc->GetGlyphWidth(SMUFL_E0A3_noteheadHalf, staffSize, graceSize) / 2;
    // adjust the radius in order to take the stem width into account
    radius -= doc->GetDrawingStemWidth(staffSize) / 2;
    Point p(radius, defaultYShift);

    // Here we should get the notehead value
    wchar_t code = SMUFL_E0A4_noteheadBlack;

    // This is never called for now because mensural notes do not have stem/flag children
    // For changingg this, change Note::CalcStem and Note::PrepareLayerElementParts
    if (this->IsMensural()) {
        // For mensural notation, get the code and adjust the default stem position
        code = this->GetMensuralSmuflNoteHead();
        p.y = doc->GetGlyphHeight(code, staffSize, graceSize) / 2;
        p.x = 0;
    }

    // Use the default for standard quarter and half note heads
    if ((code == SMUFL_E0A3_noteheadHalf) || (code == SMUFL_E0A4_noteheadBlack)) {
        return p;
    }

    Glyph *glyph = Resources::GetGlyph(code);
    assert(glyph);

    if (glyph->HasAnchor(SMUFL_stemUpSE)) {
        const Point *anchor = glyph->GetAnchor(SMUFL_stemUpSE);
        assert(anchor);
        p = doc->ConvertFontPoint(glyph, *anchor, staffSize, graceSize);
    }

    return p;
}

Point Note::GetStemDownNW(Doc *doc, int staffSize, bool graceSize)
{
    int defaultYShift = doc->GetDrawingUnit(staffSize) / 4;
    if (graceSize) defaultYShift = doc->GetCueSize(defaultYShift);
    // x default is always set to the radius for now
    int radius = doc->GetGlyphWidth(SMUFL_E0A3_noteheadHalf, staffSize, graceSize) / 2;
    // adjust the radius in order to take the stem width into account
    radius -= doc->GetDrawingStemWidth(staffSize) / 2;
    Point p(-radius, -defaultYShift);

    // Here we should get the notehead value
    wchar_t code = SMUFL_E0A4_noteheadBlack;

    // This is never called for now because mensural notes do not have stem/flag children
    // See comment above
    if (this->IsMensural()) {
        // For mensural notation, get the code and adjust the default stem position
        code = this->GetMensuralSmuflNoteHead();
        p.y = -doc->GetGlyphHeight(code, staffSize, graceSize) / 2;
        p.x = 0;
    }

    // Use the default for standard quarter and half note heads
    if ((code == SMUFL_E0A3_noteheadHalf) || (code == SMUFL_E0A4_noteheadBlack)) {
        return p;
    }

    Glyph *glyph = Resources::GetGlyph(code);
    assert(glyph);

    if (glyph->HasAnchor(SMUFL_stemDownNW)) {
        const Point *anchor = glyph->GetAnchor(SMUFL_stemDownNW);
        assert(anchor);
        p = doc->ConvertFontPoint(glyph, *anchor, staffSize, graceSize);
    }

    return p;
}

wchar_t Note::GetMensuralSmuflNoteHead()
{
    assert(this->IsMensural());

    Staff *staff = dynamic_cast<Staff *>(this->GetFirstParent(STAFF));
    assert(staff);
    bool mensural_black = (staff->m_drawingNotationType == NOTATIONTYPE_mensural_black);

    wchar_t code = 0;
    if (mensural_black) {
        code = SMUFL_E93D_mensuralNoteheadSemiminimaWhite;
    }
    else {
        int drawingDur = this->GetDrawingDur();
        if (this->GetColored()) {
            if (drawingDur == DUR_2)
                code = SMUFL_E93D_mensuralNoteheadSemiminimaWhite;
            else
                code = SMUFL_E93C_mensuralNoteheadMinimaWhite;
        }
        else {
            if (drawingDur == DUR_2)
                code = SMUFL_E93C_mensuralNoteheadMinimaWhite;
            else
                code = SMUFL_E93D_mensuralNoteheadSemiminimaWhite;
        }
    }
    return code;
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Note::CalcStem(FunctorParams *functorParams)
{
    CalcStemParams *params = dynamic_cast<CalcStemParams *>(functorParams);
    assert(params);

    // Stems have been calculated previously in Beam or FTrem - siblings becasue flags do not need to
    // be processed either
    if (this->IsInBeam() || this->IsInFTrem()) {
        return FUNCTOR_SIBLINGS;
    }

    // We currently have no stem object with mensural notes
    if (this->IsMensural()) {
        return FUNCTOR_SIBLINGS;
    }

    if (this->IsChordTone()) {
        assert(params->m_interface);
        return FUNCTOR_CONTINUE;
    }

    // This now need should be NULL and the chord stem length will be 0
    params->m_interface = NULL;
    params->m_chordStemLength = 0;

    // No stem
    if (this->GetActualDur() < DUR_2) {
        // Duration is longer than halfnote, there should be no stem
        assert(!this->GetDrawingStem());
        return FUNCTOR_SIBLINGS;
    }

    Stem *stem = this->GetDrawingStem();
    assert(stem);
    Staff *staff = dynamic_cast<Staff *>(this->GetFirstParent(STAFF));
    assert(staff);
    Layer *layer = dynamic_cast<Layer *>(this->GetFirstParent(LAYER));
    assert(layer);

    if (this->m_crossStaff) staff = this->m_crossStaff;

    // Cache the in params to avoid further lookup
    params->m_staff = staff;
    params->m_layer = layer;
    params->m_interface = this;
    params->m_dur = this->GetActualDur();
    params->m_isGraceNote = this->IsGraceNote();

    int staffSize = staff->m_drawingStaffSize;
    int staffY = staff->GetDrawingY();

    params->m_verticalCenter = staffY - params->m_doc->GetDrawingDoubleUnit(staffSize) * 2;

    /************ Set the direction ************/

    data_STEMDIRECTION stemDir = STEMDIRECTION_NONE;

    if (stem->HasStemDir()) {
        stemDir = stem->GetStemDir();
    }
    else if (layer->GetDrawingStemDir() != STEMDIRECTION_NONE) {
        stemDir = layer->GetDrawingStemDir();
    }
    else {
        stemDir = (this->GetDrawingY() >= params->m_verticalCenter) ? STEMDIRECTION_down : STEMDIRECTION_up;
    }

    this->SetDrawingStemDir(stemDir);

    // Make sure the relative position of the stem is the same
    stem->SetDrawingYRel(0);

    return FUNCTOR_CONTINUE;
}

int Note::CalcChordNoteHeads(FunctorParams *functorParams)
{
    FunctorDocParams *params = dynamic_cast<FunctorDocParams *>(functorParams);
    assert(params);

    Staff *staff = dynamic_cast<Staff *>(this->GetFirstParent(STAFF));
    assert(staff);

    // Nothing to do for notes that are not in a cluster
    if (!this->m_cluster) return FUNCTOR_SIBLINGS;

    if (this->m_crossStaff) staff = this->m_crossStaff;

    bool drawingCueSize = this->IsCueSize();
    int staffSize = staff->m_drawingStaffSize;

    int radius = this->GetDrawingRadius(params->m_doc, staffSize, drawingCueSize);

    /************** notehead direction **************/

    bool flippedNotehead = false;

    // if the note is clustered, calculations are different
    if (this->GetDrawingStemDir() == STEMDIRECTION_down) {
        // stem down/even cluster = noteheads start on left (incorrect side)
        if (this->m_cluster->size() % 2 == 0) {
            flippedNotehead = (this->m_clusterPosition % 2 != 0);
        }
        // else they start on normal side
        else {
            flippedNotehead = (this->m_clusterPosition % 2 == 0);
        }
    }
    else {
        // flipped noteheads start on normal side no matter what
        flippedNotehead = (this->m_clusterPosition % 2 == 0);
    }

    // positions notehead
    if (flippedNotehead) {
        if (this->GetDrawingStemDir() == STEMDIRECTION_up) {
            this->SetDrawingXRel(2 * radius - params->m_doc->GetDrawingStemWidth(staffSize));
        }
        else {
            this->SetDrawingXRel(-2 * radius + params->m_doc->GetDrawingStemWidth(staffSize));
        }
    }

    this->SetFlippedNotehead(flippedNotehead);

    return FUNCTOR_SIBLINGS;
}

int Note::CalcDots(FunctorParams *functorParams)
{
    CalcDotsParams *params = dynamic_cast<CalcDotsParams *>(functorParams);
    assert(params);

    // We currently have no dots object with mensural notes
    if (this->IsMensural()) {
        return FUNCTOR_SIBLINGS;
    }

    Staff *staff = dynamic_cast<Staff *>(this->GetFirstParent(STAFF));
    assert(staff);

    if (this->m_crossStaff) staff = this->m_crossStaff;

    bool drawingCueSize = this->IsCueSize();
    int staffSize = staff->m_drawingStaffSize;

    Dots *dots = NULL;
    Chord *chord = this->IsChordTone();

    // The shift to the left when a stem flag requires it
    int flagShift = 0;

    if (chord) {
        dots = params->m_chordDots;
        assert(dots);

        // Stem up, shorter than 4th and not in beam
        if ((params->m_chordStemDir == STEMDIRECTION_up) && (this->GetDrawingDur() > DUR_4) && !this->IsInBeam()) {
            // Shift according to the flag width if the top note is not flipped
            if ((this == chord->GetTopNote()) && !this->GetFlippedNotehead()) {
                // HARDCODED
                flagShift += params->m_doc->GetGlyphWidth(SMUFL_E240_flag8thUp, staffSize, drawingCueSize) * 0.8;
            }
        }
    }
    else if (this->HasDots()) {
        // For single notes we need here to set the dot loc
        dots = dynamic_cast<Dots *>(this->FindChildByType(DOTS, 1));
        assert(dots);
        params->m_chordDrawingX = this->GetDrawingX();

        std::list<int> *dotLocs = dots->GetDotLocsForStaff(staff);
        int loc = this->GetDrawingLoc();

        // if it's on a staff line to start with, we need to compensate here and add a full unit like DrawDots would
        if ((loc % 2) == 0) {
            loc += 1;
        }
        dotLocs->push_back(loc);

        // Stem up, shorter than 4th and not in beam
        if ((this->GetDrawingStemDir() == STEMDIRECTION_up) && (this->GetDrawingDur() > DUR_4) && !this->IsInBeam()) {
            // HARDCODED
            flagShift += params->m_doc->GetGlyphWidth(SMUFL_E240_flag8thUp, staffSize, drawingCueSize) * 0.8;
        }
    }
    else {
        return FUNCTOR_SIBLINGS;
    }

    int radius = this->GetDrawingRadius(params->m_doc, staffSize, drawingCueSize);
    int xRel = this->GetDrawingX() - params->m_chordDrawingX + radius + flagShift;
    dots->SetDrawingXRel(std::max(dots->GetDrawingXRel(), xRel));

    return FUNCTOR_SIBLINGS;
}

int Note::CalcLedgerLines(FunctorParams *functorParams)
{
    FunctorDocParams *params = dynamic_cast<FunctorDocParams *>(functorParams);
    assert(params);

    Staff *staff = dynamic_cast<Staff *>(this->GetFirstParent(STAFF));
    assert(staff);

    if (this->m_crossStaff) staff = this->m_crossStaff;

    bool drawingCueSize = this->IsCueSize();
    int staffSize = staff->m_drawingStaffSize;
    int staffX = staff->GetDrawingX();
    int radius = GetDrawingRadius(params->m_doc, staffSize, drawingCueSize);

    /************** Ledger lines: **************/

    int linesAbove = (this->GetDrawingLoc() - staff->m_drawingLines * 2 + 2) / 2;
    int linesBelow = -(this->GetDrawingLoc()) / 2;

    if ((linesAbove <= 0) && (linesBelow <= 0)) return FUNCTOR_CONTINUE;

    // HARDCODED
    int leftExtender = 2.5 * params->m_doc->GetDrawingStemWidth(staffSize);
    int rightExtender = 2.5 * params->m_doc->GetDrawingStemWidth(staffSize);
    if (drawingCueSize || (this->GetDrawingDur() >= DUR_8)) {
        leftExtender = 1.75 * params->m_doc->GetDrawingStemWidth(staffSize);
        rightExtender = 1.25 * params->m_doc->GetDrawingStemWidth(staffSize);
    }

    if (drawingCueSize) {
        leftExtender = params->m_doc->GetCueSize(leftExtender);
        rightExtender = params->m_doc->GetCueSize(rightExtender);
    }

    int left = this->GetDrawingX() - radius - leftExtender - staffX;
    int right = this->GetDrawingX() + radius + rightExtender - staffX;

    if (linesAbove > 0) {
        staff->AddLegerLineAbove(linesAbove, left, right, drawingCueSize);
    }
    else {
        staff->AddLegerLineBelow(linesBelow, left, right, drawingCueSize);
    }

    return FUNCTOR_CONTINUE;
}

int Note::PrepareLayerElementParts(FunctorParams *functorParams)
{
    Stem *currentStem = dynamic_cast<Stem *>(this->FindChildByType(STEM, 1));
    Flag *currentFlag = NULL;
    if (currentStem) currentFlag = dynamic_cast<Flag *>(currentStem->FindChildByType(FLAG, 1));

    if ((this->GetActualDur() > DUR_1) && !this->IsChordTone() && !this->IsMensural()) {
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

    if ((this->GetActualDur() > DUR_4) && !this->IsInBeam() && !this->IsChordTone() && !this->IsMensural()) {
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

    if (!this->IsChordTone()) SetDrawingStem(currentStem);

    /************ dots ***********/

    Dots *currentDots = dynamic_cast<Dots *>(this->FindChildByType(DOTS, 1));

    if (this->GetDots() > 0) {
        if (!currentDots) {
            currentDots = new Dots();
            this->AddChild(currentDots);
        }
        currentDots->AttAugmentDots::operator=(*this);
    }
    // This will happen only if the duration has changed
    else if (currentDots) {
        if (this->DeleteChild(currentDots)) {
            currentDots = NULL;
        }
    }

    return FUNCTOR_CONTINUE;
};

int Note::PrepareTieAttr(FunctorParams *functorParams)
{
    PrepareTieAttrParams *params = dynamic_cast<PrepareTieAttrParams *>(functorParams);
    assert(params);

    AttTiePresent *check = this;
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

    m_drawingLoc = 0;
    m_flippedNotehead = false;

    return FUNCTOR_CONTINUE;
};

int Note::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    LayerElement::ResetHorizontalAlignment(functorParams);

    m_drawingLoc = 0;
    m_flippedNotehead = false;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

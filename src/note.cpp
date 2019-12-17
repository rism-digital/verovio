/////////////////////////////////////////////////////////////////////////////
// Name:        note.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "note.h"

#include <iostream>
//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "artic.h"
#include "comparison.h"
#include "doc.h"
#include "editorial.h"
#include "elementpart.h"
#include "fermata.h"
#include "functorparams.h"
#include "glyph.h"
#include "layer.h"
#include "ligature.h"
#include "slur.h"
#include "smufl.h"
#include "staff.h"
#include "syl.h"
#include "tie.h"
#include "transposition.h"
#include "verse.h"
#include "vrv.h"

//----------------------------------------------------------------------------

#include "MidiFile.h"

namespace vrv {

//----------------------------------------------------------------------------
// Note
//----------------------------------------------------------------------------

Note::Note()
    : LayerElement("note-")
    , StemmedDrawingInterface()
    , DurationInterface()
    , PitchInterface()
    , PositionInterface()
    , AttColor()
    , AttColoration()
    , AttCue()
    , AttGraced()
    , AttMidiVelocity()
    , AttNoteAnlMensural()
    , AttStems()
    , AttStemsCmn()
    , AttTiePresent()
    , AttVisibility()
{
    RegisterInterface(DurationInterface::GetAttClasses(), DurationInterface::IsInterface());
    RegisterInterface(PitchInterface::GetAttClasses(), PitchInterface::IsInterface());
    RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_COLORATION);
    RegisterAttClass(ATT_CUE);
    RegisterAttClass(ATT_GRACED);
    RegisterAttClass(ATT_NOTEANLMENSURAL);
    RegisterAttClass(ATT_MIDIVELOCITY);
    RegisterAttClass(ATT_STEMS);
    RegisterAttClass(ATT_STEMSCMN);
    RegisterAttClass(ATT_TIEPRESENT);
    RegisterAttClass(ATT_VISIBILITY);

    Reset();
}

Note::~Note() {}

void Note::Reset()
{
    LayerElement::Reset();
    StemmedDrawingInterface::Reset();
    DurationInterface::Reset();
    PitchInterface::Reset();
    PositionInterface::Reset();
    ResetColor();
    ResetColoration();
    ResetCue();
    ResetGraced();
    ResetNoteAnlMensural();
    ResetMidiVelocity();
    ResetStems();
    ResetStemsCmn();
    ResetTiePresent();
    ResetVisibility();

    m_clusterPosition = 0;
    m_cluster = NULL;

    m_drawingLoc = 0;
    m_flippedNotehead = false;

    m_scoreTimeOnset = 0.0;
    m_scoreTimeOffset = 0.0;
    m_realTimeOnsetMilliseconds = 0;
    m_realTimeOffsetMilliseconds = 0;
    m_scoreTimeTiedDuration = 0.0;

    m_MIDIPitch = -1;
}

bool Note::HasToBeAligned() const
{
    if (!this->IsInLigature()) return true;
    Note *note = const_cast<Note *>(this);
    Ligature *ligature = dynamic_cast<Ligature *>(note->GetFirstAncestor(LIGATURE));
    assert(ligature);
    return ((note == ligature->GetFirstNote()) || (note == ligature->GetLastNote()));
}

void Note::AddChild(Object *child)
{
    // additional verification for accid and artic - this will no be raised with editorial markup, though
    if (child->Is(ACCID)) {
        IsAttributeComparison isAttributeComparison(ACCID);
        if (this->FindDescendantByComparison(&isAttributeComparison))
            LogWarning("Having both @accid or @accid.ges and <accid> child will cause problems");
    }
    else if (child->Is(ARTIC)) {
        IsAttributeComparison isAttributeComparison(ARTIC);
        if (this->FindDescendantByComparison(&isAttributeComparison))
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

Accid *Note::GetDrawingAccid()
{
    Accid *accid = dynamic_cast<Accid *>(this->FindDescendantByType(ACCID));
    return accid;
}

Chord *Note::IsChordTone() const
{
    return dynamic_cast<Chord *>(this->GetFirstAncestor(CHORD, MAX_CHORD_DEPTH));
}

int Note::GetDrawingDur() const
{
    Chord *chordParent = dynamic_cast<Chord *>(this->GetFirstAncestor(CHORD, MAX_CHORD_DEPTH));
    if (chordParent && !this->HasDur()) {
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

Point Note::GetStemUpSE(Doc *doc, int staffSize, bool isCueSize)
{
    int defaultYShift = doc->GetDrawingUnit(staffSize) / 4;
    if (isCueSize) defaultYShift = doc->GetCueSize(defaultYShift);
    // x default is always set to the right for now
    int defaultXShift = doc->GetGlyphWidth(SMUFL_E0A3_noteheadHalf, staffSize, isCueSize);
    // adjust the x shift in order to take the stem width into account
    defaultXShift -= doc->GetDrawingStemWidth(staffSize) / 2;
    Point p(defaultXShift, defaultYShift);

    // Here we should get the notehead value
    wchar_t code = SMUFL_E0A4_noteheadBlack;

    // This is never called for now because mensural notes do not have stem/flag children
    // For changingg this, change Note::CalcStem and Note::PrepareLayerElementParts
    if (this->IsMensural()) {
        // For mensural notation, get the code and adjust the default stem position
        code = this->GetMensuralSmuflNoteHead();
        p.y = doc->GetGlyphHeight(code, staffSize, isCueSize) / 2;
        p.x = doc->GetGlyphWidth(code, staffSize, isCueSize);
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
        p = doc->ConvertFontPoint(glyph, *anchor, staffSize, isCueSize);
    }

    return p;
}

Point Note::GetStemDownNW(Doc *doc, int staffSize, bool isCueSize)
{
    int defaultYShift = doc->GetDrawingUnit(staffSize) / 4;
    if (isCueSize) defaultYShift = doc->GetCueSize(defaultYShift);
    // x default is always set to the left for now
    int defaultXShift = 0;
    // adjust the x shift in order to take the stem width into account
    defaultXShift += doc->GetDrawingStemWidth(staffSize) / 2;
    Point p(defaultXShift, -defaultYShift);

    // Here we should get the notehead value
    wchar_t code = SMUFL_E0A4_noteheadBlack;

    // This is never called for now because mensural notes do not have stem/flag children
    // See comment above
    if (this->IsMensural()) {
        // For mensural notation, get the code and adjust the default stem position
        code = this->GetMensuralSmuflNoteHead();
        p.y = -doc->GetGlyphHeight(code, staffSize, isCueSize) / 2;
        p.x = doc->GetGlyphWidth(code, staffSize, isCueSize);
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
        p = doc->ConvertFontPoint(glyph, *anchor, staffSize, isCueSize);
    }

    return p;
}

wchar_t Note::GetMensuralSmuflNoteHead()
{
    assert(this->IsMensural());

    int drawingDur = this->GetDrawingDur();

    // No SMuFL code used for these values
    if (drawingDur < DUR_1) {
        return 0;
    }

    Staff *staff = dynamic_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff);
    bool mensural_black = (staff->m_drawingNotationType == NOTATIONTYPE_mensural_black);

    wchar_t code = 0;
    if (mensural_black) {
        code = SMUFL_E93D_mensuralNoteheadSemiminimaWhite;
    }
    else {
        if (this->GetColored() == BOOLEAN_true) {
            if (drawingDur > DUR_2) {
                code = SMUFL_E93C_mensuralNoteheadMinimaWhite;
            }
            else {
                code = SMUFL_E93D_mensuralNoteheadSemiminimaWhite;
            }
        }
        else {
            if (drawingDur > DUR_2) {
                code = SMUFL_E93D_mensuralNoteheadSemiminimaWhite;
            }
            else {
                code = SMUFL_E93C_mensuralNoteheadMinimaWhite;
            }
        }
    }
    return code;
}

bool Note::IsVisible()
{
    if (this->HasVisible()) {
        return this->GetVisible() == BOOLEAN_true;
    }
    // if the chord doens't have it, see if all the children are invisible
    else if (GetParent() && GetParent()->Is(CHORD)) {
        Chord *chord = dynamic_cast<Chord *>(GetParent());
        assert(chord);
        return chord->IsVisible();
    }
    return true;
}

void Note::SetScoreTimeOnset(double scoreTime)
{
    m_scoreTimeOnset = scoreTime;
}

void Note::SetRealTimeOnsetSeconds(double timeInSeconds)
{
    m_realTimeOnsetMilliseconds = int(timeInSeconds * 1000.0 + 0.5);
}

void Note::SetScoreTimeOffset(double scoreTime)
{
    m_scoreTimeOffset = scoreTime;
}

void Note::SetRealTimeOffsetSeconds(double timeInSeconds)
{
    m_realTimeOffsetMilliseconds = int(timeInSeconds * 1000.0 + 0.5);
}

void Note::SetScoreTimeTiedDuration(double scoreTime)
{
    m_scoreTimeTiedDuration = scoreTime;
}

void Note::SetMIDIPitch(char pitch)
{
    m_MIDIPitch = pitch;
}

double Note::GetScoreTimeOnset()
{
    return m_scoreTimeOnset;
}

int Note::GetRealTimeOnsetMilliseconds()
{
    return m_realTimeOnsetMilliseconds;
}

double Note::GetScoreTimeOffset()
{
    return m_scoreTimeOffset;
}

int Note::GetRealTimeOffsetMilliseconds()
{
    return m_realTimeOffsetMilliseconds;
}

double Note::GetScoreTimeTiedDuration()
{
    return m_scoreTimeTiedDuration;
}

double Note::GetScoreTimeDuration()
{
    return GetScoreTimeOffset() - GetScoreTimeOnset();
}

char Note::GetMIDIPitch()
{
    return m_MIDIPitch;
}

int Note::GetChromaticAlteration()
{
    Accid *accid = this->GetDrawingAccid();

    if (accid) {
        return TransPitch::GetChromaticAlteration(accid->GetAccidGes(), accid->GetAccid());
    }
    return 0;
}

TransPitch Note::GetTransPitch()
{
    int pname = this->GetPname() - PITCHNAME_c;
    return TransPitch(pname, this->GetChromaticAlteration(), this->GetOct());
}

void Note::UpdateFromTransPitch(const TransPitch &tp)
{
    this->SetPname(tp.GetPitchName());

    Accid *accid = this->GetDrawingAccid();
    bool transposeGesturalAccid = false;
    bool transposeWrittenAccid = false;
    if (!accid) {
        accid = new Accid();
        this->AddChild(accid);
    }
    if (accid->HasAccidGes()) {
        transposeGesturalAccid = true;
    }
    if (accid->HasAccid()) {
        transposeWrittenAccid = true;
    }
    // TODO: Check the case of both existing but having unequal values.
    if (!accid->HasAccidGes() && !accid->HasAccid()) {
        transposeGesturalAccid = true;
    }

    if (transposeGesturalAccid) {
        accid->SetAccidGes(tp.GetAccidG());
    }
    if (transposeWrittenAccid) {
        accid->SetAccid(tp.GetAccidW());
    }

    if (this->GetOct() != tp.m_oct) {
        if (this->HasOctGes()) {
            this->SetOctGes(this->GetOctGes() + tp.m_oct - this->GetOct());
        }
        this->SetOct(tp.m_oct);
    }
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Note::ConvertAnalyticalMarkup(FunctorParams *functorParams)
{
    ConvertAnalyticalMarkupParams *params = dynamic_cast<ConvertAnalyticalMarkupParams *>(functorParams);
    assert(params);

    /****** ties ******/

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
                Tie *tie = new Tie();
                if (!params->m_permanent) {
                    tie->IsAttribute(true);
                }
                tie->SetStartid("#" + (*iter)->GetUuid());
                tie->SetEndid("#" + this->GetUuid());
                params->m_controlEvents.push_back(tie);
            }
            else {
                LogWarning("Expected @tie median or terminal in note '%s', skipping it", this->GetUuid().c_str());
            }
            iter = params->m_currentNotes.erase(iter);
            // we are done for this note
            break;
        }
        ++iter;
    }

    if ((check->GetTie() == TIE_m) || (check->GetTie() == TIE_i)) {
        params->m_currentNotes.push_back(this);
    }

    if (params->m_permanent) {
        this->ResetTiePresent();
    }

    /****** fermata ******/

    if (this->HasFermata()) {
        Fermata *fermata = new Fermata();
        fermata->ConvertFromAnalyticalMarkup(this, this->GetUuid(), params);
    }

    return FUNCTOR_CONTINUE;
}

int Note::CalcStem(FunctorParams *functorParams)
{
    CalcStemParams *params = dynamic_cast<CalcStemParams *>(functorParams);
    assert(params);

    if (!this->IsVisible() || (this->GetStemVisible() == BOOLEAN_false)) {
        return FUNCTOR_SIBLINGS;
    }

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

    Stem *stem = this->GetDrawingStem();
    assert(stem);
    Staff *staff = dynamic_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff);
    Layer *layer = dynamic_cast<Layer *>(this->GetFirstAncestor(LAYER));
    assert(layer);

    if (this->m_crossStaff) staff = this->m_crossStaff;

    // Cache the in params to avoid further lookup
    params->m_staff = staff;
    params->m_layer = layer;
    params->m_interface = this;
    params->m_dur = this->GetActualDur();
    params->m_isGraceNote = this->IsGraceNote();

    int staffSize = staff->m_drawingStaffSize;

    params->m_verticalCenter
        = staff->GetDrawingY() - params->m_doc->GetDrawingUnit(staffSize) * (staff->m_drawingLines - 1);

    /************ Set the direction ************/

    data_STEMDIRECTION layerStemDir;
    data_STEMDIRECTION stemDir = STEMDIRECTION_NONE;

    if (stem->HasStemDir()) {
        stemDir = stem->GetStemDir();
    }
    else if (this->IsGraceNote()) {
        stemDir = STEMDIRECTION_up;
    }
    else if ((layerStemDir = layer->GetDrawingStemDir(this)) != STEMDIRECTION_NONE) {
        stemDir = layerStemDir;
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

    Staff *staff = dynamic_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff);

    // Nothing to do for notes that are not in a cluster
    if (!this->m_cluster) return FUNCTOR_SIBLINGS;

    if (this->m_crossStaff) staff = this->m_crossStaff;

    int staffSize = staff->m_drawingStaffSize;

    int radius = this->GetDrawingRadius(params->m_doc);

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
    if (!this->IsVisible()) {
        return FUNCTOR_SIBLINGS;
    }

    Staff *staff = dynamic_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff);

    if (this->m_crossStaff) staff = this->m_crossStaff;

    bool drawingCueSize = this->GetDrawingCueSize();
    int staffSize = staff->m_drawingStaffSize;

    Dots *dots = NULL;
    Chord *chord = this->IsChordTone();

    // The shift to the left when a stem flag requires it
    int flagShift = 0;

    if (chord && (chord->GetDots() > 0)) {
        dots = params->m_chordDots;
        assert(dots);

        // Stem up, shorter than 4th and not in beam
        if ((this->GetDots() != 0) && (params->m_chordStemDir == STEMDIRECTION_up) && (this->GetDrawingDur() > DUR_4)
            && !this->IsInBeam()) {
            // Shift according to the flag width if the top note is not flipped
            if ((this == chord->GetTopNote()) && !this->GetFlippedNotehead()) {
                // HARDCODED
                flagShift += params->m_doc->GetGlyphWidth(SMUFL_E240_flag8thUp, staffSize, drawingCueSize) * 0.8;
            }
        }
    }
    else if (this->GetDots() > 0) {
        // For single notes we need here to set the dot loc
        dots = dynamic_cast<Dots *>(this->FindDescendantByType(DOTS, 1));
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

    int radius = this->GetDrawingRadius(params->m_doc);
    int xRel = this->GetDrawingX() - params->m_chordDrawingX + 2 * radius + flagShift;
    dots->SetDrawingXRel(std::max(dots->GetDrawingXRel(), xRel));

    return FUNCTOR_SIBLINGS;
}

int Note::CalcLedgerLines(FunctorParams *functorParams)
{
    FunctorDocParams *params = dynamic_cast<FunctorDocParams *>(functorParams);
    assert(params);

    if (this->GetVisible() == BOOLEAN_false) {
        return FUNCTOR_SIBLINGS;
    }

    Staff *staff = dynamic_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff);

    if (!this->IsVisible()) {
        return FUNCTOR_SIBLINGS;
    }

    if (this->m_crossStaff) staff = this->m_crossStaff;

    bool drawingCueSize = this->GetDrawingCueSize();
    int staffSize = staff->m_drawingStaffSize;
    int staffX = staff->GetDrawingX();
    int radius = GetDrawingRadius(params->m_doc);

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

    int left = this->GetDrawingX() - leftExtender - staffX;
    int right = this->GetDrawingX() + 2 * radius + rightExtender - staffX;
    if (this->GetDrawingDur() == DUR_MX) {
        right += 2 * radius;
    }

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
    Stem *currentStem = dynamic_cast<Stem *>(this->FindDescendantByType(STEM, 1));
    Flag *currentFlag = NULL;
    Chord *chord = this->IsChordTone();
    if (currentStem) currentFlag = dynamic_cast<Flag *>(currentStem->FindDescendantByType(FLAG, 1));

    if (!this->IsChordTone() && !this->IsMensural()) {
        if (!currentStem) {
            currentStem = new Stem();
            this->AddChild(currentStem);
        }
        currentStem->AttGraced::operator=(*this);
        currentStem->AttStems::operator=(*this);
        currentStem->AttStemsCmn::operator=(*this);
        if (this->GetActualDur() < DUR_2) {
            currentStem->IsVirtual(true);
        }
    }
    // This will happen only if the duration has changed
    else if (currentStem) {
        if (this->DeleteChild(currentStem)) {
            currentStem = NULL;
            // The currentFlag (if any) will have been deleted above
            currentFlag = NULL;
        }
    }

    if ((this->GetActualDur() > DUR_4) && !this->IsInBeam() && !this->IsInFTrem() && !this->IsChordTone()
        && !this->IsMensural()) {
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

    if (!chord) SetDrawingStem(currentStem);

    /************ dots ***********/

    Dots *currentDots = dynamic_cast<Dots *>(this->FindDescendantByType(DOTS, 1));

    if (this->GetDots() > 0) {
        if (chord && (chord->GetDots() == this->GetDots())) {
            LogWarning(
                "Note '%s' with a @dots attribute with the same value as its chord parent", this->GetUuid().c_str());
        }
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

    /************ Prepare the drawing cue size ************/

    Functor prepareDrawingCueSize(&Object::PrepareDrawingCueSize);
    this->Process(&prepareDrawingCueSize, NULL);

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
    // Call parent one too
    LayerElement::PreparePointersByLayer(functorParams);

    PreparePointersByLayerParams *params = dynamic_cast<PreparePointersByLayerParams *>(functorParams);
    assert(params);

    params->m_currentNote = this;

    return FUNCTOR_CONTINUE;
}

int Note::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetDrawing(functorParams);
    PositionInterface::InterfaceResetDrawing(functorParams, this);

    m_drawingLoc = 0;
    m_flippedNotehead = false;

    return FUNCTOR_CONTINUE;
}

int Note::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    LayerElement::ResetHorizontalAlignment(functorParams);
    PositionInterface::InterfaceResetHorizontalAlignment(functorParams, this);

    m_drawingLoc = 0;
    m_flippedNotehead = false;

    return FUNCTOR_CONTINUE;
}

int Note::GenerateMIDI(FunctorParams *functorParams)
{
    GenerateMIDIParams *params = dynamic_cast<GenerateMIDIParams *>(functorParams);
    assert(params);

    Note *note = dynamic_cast<Note *>(this->ThisOrSameasAsLink());
    assert(note);

    // If the note is a secondary tied note, then ignore it
    if (note->GetScoreTimeTiedDuration() < 0.0) {
        return FUNCTOR_SIBLINGS;
    }

    // For now just ignore grace notes
    if (note->IsGraceNote()) {
        return FUNCTOR_SIBLINGS;
    }

    // Create midi this
    int midiBase = 0;
    data_PITCHNAME pname = note->GetPname();
    switch (pname) {
        case PITCHNAME_c: midiBase = 0; break;
        case PITCHNAME_d: midiBase = 2; break;
        case PITCHNAME_e: midiBase = 4; break;
        case PITCHNAME_f: midiBase = 5; break;
        case PITCHNAME_g: midiBase = 7; break;
        case PITCHNAME_a: midiBase = 9; break;
        case PITCHNAME_b: midiBase = 11; break;
        case PITCHNAME_NONE: break;
    }
    // Check for accidentals
    midiBase += note->GetChromaticAlteration();

    // Adjustment for transposition intruments
    midiBase += params->m_transSemi;

    int oct = note->GetOct();
    if (note->HasOctGes()) oct = note->GetOctGes();

    int pitch = midiBase + (oct + 1) * 12;
    // We do store the MIDIPitch in the note even with a sameas
    this->SetMIDIPitch(pitch);
    int channel = params->m_midiChannel;
    int velocity = MIDI_VELOCITY;
    if (note->HasVel()) velocity = note->GetVel();

    double starttime = params->m_totalTime + note->GetScoreTimeOnset();
    double stoptime = params->m_totalTime + note->GetScoreTimeOffset() + note->GetScoreTimeTiedDuration();

    int tpq = params->m_midiFile->getTPQ();

    params->m_midiFile->addNoteOn(params->m_midiTrack, starttime * tpq, channel, pitch, velocity);
    params->m_midiFile->addNoteOff(params->m_midiTrack, stoptime * tpq, channel, pitch);

    return FUNCTOR_SIBLINGS;
}

int Note::GenerateTimemap(FunctorParams *functorParams)
{
    GenerateTimemapParams *params = dynamic_cast<GenerateTimemapParams *>(functorParams);
    assert(params);

    Note *note = dynamic_cast<Note *>(this->ThisOrSameasAsLink());
    assert(note);

    int realTimeStart = params->m_realTimeOffsetMilliseconds + note->GetRealTimeOnsetMilliseconds();
    double scoreTimeStart = params->m_scoreTimeOffset + note->GetScoreTimeOnset();

    int realTimeEnd = params->m_realTimeOffsetMilliseconds + note->GetRealTimeOffsetMilliseconds();
    double scoreTimeEnd = params->m_scoreTimeOffset + note->GetScoreTimeOffset();

    // Should check if value for realTimeStart already exists and if so, then
    // ensure that it is equal to scoreTimeStart:
    params->realTimeToScoreTime[realTimeStart] = scoreTimeStart;

    // Store the element ID in list to turn on at given time.
    params->realTimeToOnElements[realTimeStart].push_back(this->GetUuid());

    // Should check if value for realTimeEnd already exists and if so, then
    // ensure that it is equal to scoreTimeEnd:
    params->realTimeToScoreTime[realTimeEnd] = scoreTimeEnd;

    // Store the element ID in list to turn off at given time.
    params->realTimeToOffElements[realTimeEnd].push_back(this->GetUuid());

    params->realTimeToTempo[realTimeStart] = params->m_currentTempo;

    return FUNCTOR_SIBLINGS;
}

int Note::Transpose(FunctorParams *functorParams)
{
    TransposeParams *params = dynamic_cast<TransposeParams *>(functorParams);
    assert(params);

    LogDebug("Transposing note");

    TransPitch pitch = this->GetTransPitch();
    params->m_transposer->Transpose(pitch);
    this->UpdateFromTransPitch(pitch);

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv

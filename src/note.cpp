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
#include "plica.h"
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
    , AttExtSym()
    , AttGraced()
    , AttMidiVelocity()
    , AttNoteHeads()
    , AttNoteVisMensural()
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
    RegisterAttClass(ATT_EXTSYM);
    RegisterAttClass(ATT_GRACED);
    RegisterAttClass(ATT_NOTEHEADS);
    RegisterAttClass(ATT_NOTEVISMENSURAL);
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
    ResetExtSym();
    ResetGraced();
    ResetNoteHeads();
    ResetNoteVisMensural();
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

bool Note::IsSupportedChild(Object *child)
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
    else if (child->Is(PLICA)) {
        assert(dynamic_cast<Plica *>(child));
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
        return false;
    }
    return true;
}

void Note::AddChild(Object *child)
{
    if (!this->IsSupportedChild(child)) {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        return;
    }

    child->SetParent(this);

    ArrayOfObjects *children = this->GetChildrenForModification();

    // Stem are always added by PrepareLayerElementParts (for now) and we want them to be in the front
    // for the drawing order in the SVG output
    if (child->Is({ DOTS, STEM })) {
        children->insert(children->begin(), child);
    }
    else {
        children->push_back(child);
    }
    Modify();
}

Accid *Note::GetDrawingAccid()
{
    Accid *accid = dynamic_cast<Accid *>(this->FindDescendantByType(ACCID));
    return accid;
}

bool Note::HasLedgerLines(int &linesAbove, int &linesBelow, Staff *staff)
{
    if (!staff) {
        staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
        assert(staff);
    }

    linesAbove = (this->GetDrawingLoc() - staff->m_drawingLines * 2 + 2) / 2;
    linesBelow = -(this->GetDrawingLoc()) / 2;

    linesAbove = std::max(linesAbove, 0);
    linesBelow = std::max(linesBelow, 0);

    return ((linesAbove > 0) || (linesBelow > 0));
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
    Point p(defaultXShift, defaultYShift);

    // Here we should get the notehead value
    wchar_t code = GetNoteheadGlyph(GetDrawingDur());

    // This is never called for now because mensural notes do not have stem/flag children
    // For changingg this, change Note::CalcStem and Note::PrepareLayerElementParts
    if (this->IsMensuralDur()) {
        // For mensural notation, get the code and adjust the default stem position
        code = this->GetMensuralSmuflNoteHead();
        p.y = doc->GetGlyphHeight(code, staffSize, isCueSize) / 2;
        p.x = doc->GetGlyphWidth(code, staffSize, isCueSize);
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
    Point p(0, -defaultYShift);

    // Here we should get the notehead value
    wchar_t code = GetNoteheadGlyph(GetDrawingDur());

    // This is never called for now because mensural notes do not have stem/flag children
    // See comment above
    if (this->IsMensuralDur()) {
        // For mensural notation, get the code and adjust the default stem position
        code = this->GetMensuralSmuflNoteHead();
        p.y = -doc->GetGlyphHeight(code, staffSize, isCueSize) / 2;
        p.x = doc->GetGlyphWidth(code, staffSize, isCueSize);
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

int Note::CalcStemLenInThirdUnits(Staff *staff)
{
    assert(staff);

    int baseStem = STANDARD_STEMLENGTH * 3;

    int shortening = 0;

    int unitToLine = (this->GetDrawingStemDir() == STEMDIRECTION_up)
        ? -this->GetDrawingLoc() + (staff->m_drawingLines - 1) * 2
        : this->GetDrawingLoc();
    if (unitToLine < 5) {
        switch (unitToLine) {
            case 4: shortening = 1; break;
            case 3: shortening = 2; break;
            case 2: shortening = 3; break;
            case 1: shortening = 4; break;
            case 0: shortening = 5; break;
            default: shortening = 6;
        }
    }

    // Limit shortening with duration shorter than quarter not when not in a beam
    if ((this->GetDrawingDur() > DUR_4) && !this->IsInBeam()) {
        if (this->GetDrawingStemDir() == STEMDIRECTION_up) {
            shortening = std::min(4, shortening);
        }
        else {
            shortening = std::min(3, shortening);
        }
    }

    baseStem -= shortening;

    return baseStem;
}

wchar_t Note::GetMensuralSmuflNoteHead()
{
    assert(this->IsMensuralDur());

    int drawingDur = this->GetDrawingDur();

    // No SMuFL code used for these values
    if (drawingDur < DUR_1) {
        return 0;
    }

    Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
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

wchar_t Note::GetNoteheadGlyph(const int duration) const
{
    static std::map<std::string, wchar_t> additionalNoteheadSymbols
        = { { "noteheadDiamondBlackWide", SMUFL_E0DC_noteheadDiamondBlackWide },
              { "noteheadDiamondWhiteWide", SMUFL_E0DE_noteheadDiamondWhiteWide },
              { "noteheadNull", SMUFL_E0A5_noteheadNull } };

    if (HasGlyphName()) {
        const std::string glyph = GetGlyphName();
        if (additionalNoteheadSymbols.end() == additionalNoteheadSymbols.find(glyph)) {
            return SMUFL_E0A4_noteheadBlack;
        }
        return additionalNoteheadSymbols[glyph];
    }

    switch (GetHeadShape()) {
        case HEADSHAPE_quarter: return SMUFL_E0A4_noteheadBlack;
        case HEADSHAPE_half: return SMUFL_E0A3_noteheadHalf;
        case HEADSHAPE_whole: return SMUFL_E0A2_noteheadWhole;
        // case HEADSHAPE_backslash: return SMUFL_noteheadBackslash;
        // case HEADSHAPE_circle: return SMUFL_E0B3_noteheadCircleX;
        case HEADSHAPE_plus: return SMUFL_E0AF_noteheadPlusBlack;
        case HEADSHAPE_diamond: {
            if (DUR_1 == duration) return SMUFL_E0D9_noteheadDiamondHalf;
            return GetHeadFill() == FILL_void ? SMUFL_E0DD_noteheadDiamondWhite : SMUFL_E0DB_noteheadDiamondBlack;
        }
        // case HEADSHAPE_isotriangle: return SMUFL_E0BC_noteheadTriangleUpHalf;
        // case HEADSHAPE_oval: return SMUFL_noteheadOval;
        // case HEADSHAPE_piewedge: return SMUFL_noteheadPieWedge;
        // case HEADSHAPE_rectangle: return SMUFL_noteheadRectangle;
        // case HEADSHAPE_rtriangle: return SMUFL_noteheadRTriangle;
        // case HEADSHAPE_semicircle: return SMUFL_noteheadSemicircle;
        case HEADSHAPE_slash: return SMUFL_E101_noteheadSlashHorizontalEnds;
        // case HEADSHAPE_square: return SMUFL_noteheadSquare;
        case HEADSHAPE_x: {
            if (DUR_1 == duration) return SMUFL_E0B5_noteheadWholeWithX;
            if (DUR_2 == duration) return SMUFL_E0B6_noteheadHalfWithX;
            return SMUFL_E0A9_noteheadXBlack;
        }
        default: break;
    }

    if (DUR_1 == duration) return SMUFL_E0A2_noteheadWhole;
    if (DUR_2 == duration) return SMUFL_E0A3_noteheadHalf;
    return SMUFL_E0A4_noteheadBlack;
}

bool Note::IsVisible()
{
    if (this->HasVisible()) {
        return this->GetVisible() == BOOLEAN_true;
    }
    // if the chord doens't have it, see if all the children are invisible
    else if (GetParent() && GetParent()->Is(CHORD)) {
        Chord *chord = vrv_cast<Chord *>(GetParent());
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
    // m_realTimeOnsetMilliseconds = int(timeInSeconds * 1000.0 + 0.5);
    m_realTimeOnsetMilliseconds = timeInSeconds * 1000.0;
}

void Note::SetScoreTimeOffset(double scoreTime)
{
    m_scoreTimeOffset = scoreTime;
}

void Note::SetRealTimeOffsetSeconds(double timeInSeconds)
{
    // m_realTimeOffsetMilliseconds = int(timeInSeconds * 1000.0 + 0.5);
    m_realTimeOffsetMilliseconds = timeInSeconds * 1000.0;
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

double Note::GetRealTimeOnsetMilliseconds()
{
    return m_realTimeOnsetMilliseconds;
}

double Note::GetScoreTimeOffset()
{
    return m_scoreTimeOffset;
}

double Note::GetRealTimeOffsetMilliseconds()
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

bool Note::IsDotOverlappingWithFlag(Doc *doc, const int staffSize, bool isDotShifted)
{
    Object *stem = GetFirst(STEM);
    if (!stem) return false;

    Flag *flag = dynamic_cast<Flag *>(stem->GetFirst(FLAG));
    if (!flag) return false;

    // for the purposes of vertical spacing we care only up to 16th flags - shorter ones grow upwards
    wchar_t flagGlyph = SMUFL_E242_flag16thUp;
    data_DURATION dur = this->GetDur();
    if (dur < DURATION_16) flagGlyph = flag->GetSmuflCode(GetDrawingStemDir());
    const int flagHeight = doc->GetGlyphHeight(flagGlyph, staffSize, GetDrawingCueSize());

    const int dotMargin = flag->GetDrawingY() - GetDrawingY() - flagHeight - GetDrawingRadius(doc) / 2
        - (isDotShifted ? doc->GetDrawingUnit(staffSize) : 0);

    return dotMargin < 0;
}

std::pair<int, bool> Note::CalcNoteHorizontalOverlap(
    Doc *doc, const std::vector<LayerElement *> &otherElements, bool isChordElement, bool isLowerElement, bool unison)
{
    Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff);

    bool isInUnison = false;
    int shift = 0;

    for (int i = 0; i < int(otherElements.size()); ++i) {
        int verticalMargin = 0;
        int horizontalMargin = 2 * doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
        bool isUnisonElement = false;
        if (Is(NOTE) && otherElements.at(i)->Is(NOTE)) {
            Note *previousNote = vrv_cast<Note *>(otherElements.at(i));
            assert(previousNote);
            isUnisonElement = IsUnissonWith(previousNote, true);
            // Unisson, look at the duration for the note heads
            if (unison && IsUnissonWith(previousNote, false)) {
                int previousDuration = previousNote->GetDrawingDur();
                const bool isPreviousCoord = previousNote->GetParent()->Is(CHORD);
                bool isEdgeElement = false;
                if (isPreviousCoord) {
                    Chord *parentChord = vrv_cast<Chord *>(previousNote->GetParent());
                    data_STEMDIRECTION stemDir = GetDrawingStemDir();
                    previousDuration = parentChord->GetDur();
                    isEdgeElement = ((STEMDIRECTION_down == stemDir) && (parentChord->GetBottomNote() == previousNote))
                        || ((STEMDIRECTION_up == stemDir) && (parentChord->GetTopNote() == previousNote));
                }
                // Reduce the margin to 0 for whole notes unisson
                else if ((GetDrawingDur() == DUR_1) && (previousDuration == DUR_1)) {
                    horizontalMargin = 0;
                }
                if (!isPreviousCoord || isEdgeElement || isChordElement) {
                    if ((GetDrawingDur() == DUR_2) && (previousDuration == DUR_2)) {
                        isInUnison = true;
                        continue;
                    }
                    else if ((GetDrawingDur() > DUR_2) && (previousDuration > DUR_2)) {
                        isInUnison = true;
                        continue;
                    }
                }
                else {
                    horizontalMargin *= -1;
                }
            }
            else if (previousNote->GetDrawingLoc() - GetDrawingLoc() > 1) {
                continue;
            }
            else if (previousNote->GetDrawingLoc() - GetDrawingLoc() == 1) {
                horizontalMargin = 0;
            }
            else if ((previousNote->GetDrawingLoc() - GetDrawingLoc() < 0)
                && (previousNote->GetDrawingStemDir() != GetDrawingStemDir()) /* && !isChordElement*/) {
                if (previousNote->GetDrawingLoc() - GetDrawingLoc() == -1) {
                    horizontalMargin *= -1;
                }
                else if ((GetDrawingDur() <= DUR_1) && (previousNote->GetDrawingDur() <= DUR_1)) {
                    continue;
                }
                else if (previousNote->m_crossStaff || m_crossStaff)
                    continue;
                else {
                    horizontalMargin *= -1;
                    verticalMargin = horizontalMargin;
                }
            }
        }

        // Nothing to do if we have no vertical overlap
        if (!VerticalSelfOverlap(otherElements.at(i), verticalMargin)) continue;

        // Nothing to do either if we have no horizontal overlap
        if (!HorizontalSelfOverlap(otherElements.at(i), horizontalMargin + shift)) continue;

        if (horizontalMargin < 0 || isLowerElement) {
            shift -= HorizontalRightOverlap(otherElements.at(i), doc, -shift, verticalMargin);
            if (!isUnisonElement) shift -= horizontalMargin;
        }
        else if ((horizontalMargin >= 0) || isChordElement) {
            shift += HorizontalLeftOverlap(otherElements.at(i), doc, horizontalMargin - shift, verticalMargin);

            // Make additional adjustments for cross-staff and unison notes
            if (m_crossStaff) shift -= horizontalMargin;
            if (isInUnison) shift *= -1;
        }
        else {
            // Otherwise move the appropriate parent to the right
            shift -= horizontalMargin
                - HorizontalRightOverlap(otherElements.at(i), doc, horizontalMargin - shift, verticalMargin);
        }
    }

    // If note is not in unison, has accidental and were to be shifted to the right - shift it to the left
    // That way accidental will be near note that actually has accidental and not near lowest-layer note
    if (isChordElement && unison && GetDrawingAccid() && (shift > 0)) shift *= -1;

    return { shift, isInUnison };
}

void Note::AdjustOverlappingLayers(Doc *doc, const std::vector<LayerElement *> &otherElements, bool &isUnison)
{
    if (GetParent()->Is(CHORD)) return;

    auto [margin, isInUnison] = CalcNoteHorizontalOverlap(doc, otherElements, false);
    isUnison = isInUnison;
    if (!isInUnison) SetDrawingXRel(GetDrawingXRel() + margin);
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Note::ConvertMarkupAnalytical(FunctorParams *functorParams)
{
    ConvertMarkupAnalyticalParams *params = vrv_params_cast<ConvertMarkupAnalyticalParams *>(functorParams);
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
    CalcStemParams *params = vrv_params_cast<CalcStemParams *>(functorParams);
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
    if (this->IsMensuralDur()) {
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
    Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff);
    Layer *layer = vrv_cast<Layer *>(this->GetFirstAncestor(LAYER));
    assert(layer);

    if (this->m_crossStaff) {
        staff = this->m_crossStaff;
        layer = this->m_crossLayer;
    }

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
    FunctorDocParams *params = vrv_params_cast<FunctorDocParams *>(functorParams);
    assert(params);

    Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
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
    CalcDotsParams *params = vrv_params_cast<CalcDotsParams *>(functorParams);
    assert(params);

    // We currently have no dots object with mensural notes
    if (this->IsMensuralDur()) {
        return FUNCTOR_SIBLINGS;
    }
    if (!this->IsVisible()) {
        return FUNCTOR_SIBLINGS;
    }

    Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff);

    if (this->m_crossStaff) staff = this->m_crossStaff;

    bool drawingCueSize = this->GetDrawingCueSize();
    int staffSize = staff->m_drawingStaffSize;

    Dots *dots = NULL;
    Chord *chord = this->IsChordTone();

    // The shift to the left when a stem flag requires it
    int flagShift = 0;
    int radius = this->GetDrawingRadius(params->m_doc);

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

        int xRel = this->GetDrawingX() - params->m_chordDrawingX + 2 * radius + flagShift;
        dots->SetDrawingXRel(std::max(dots->GetDrawingXRel(), xRel));
    }
    if (this->GetDots() > 0) {
        // For single notes we need here to set the dot loc
        dots = vrv_cast<Dots *>(this->FindDescendantByType(DOTS, 1));
        assert(dots);

        std::list<int> *dotLocs = dots->GetDotLocsForStaff(staff);
        int loc = this->GetDrawingLoc();

        // if it's on a staff line to start with, we need to compensate here and add a full unit like DrawDots would
        const bool isDotShifted(loc % 2 == 0);
        if (isDotShifted) ++loc;
        dotLocs->push_back(loc);

        // Stem up, shorter than 4th and not in beam
        if ((GetDrawingStemDir() == STEMDIRECTION_up) && (!this->IsInBeam()) && (GetDrawingStemLen() < 3)
            && (IsDotOverlappingWithFlag(params->m_doc, staffSize, isDotShifted))) {
            // HARDCODED
            flagShift += params->m_doc->GetGlyphWidth(SMUFL_E240_flag8thUp, staffSize, drawingCueSize) * 0.8;
        }

        int xRel = 2 * radius + flagShift;
        dots->SetDrawingXRel(std::max(dots->GetDrawingXRel(), xRel));
    }

    return FUNCTOR_SIBLINGS;
}

int Note::CalcLedgerLines(FunctorParams *functorParams)
{
    FunctorDocParams *params = vrv_params_cast<FunctorDocParams *>(functorParams);
    assert(params);

    if (this->GetVisible() == BOOLEAN_false) {
        return FUNCTOR_SIBLINGS;
    }

    Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
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

    int linesAbove = 0;
    int linesBelow = 0;

    if (!this->HasLedgerLines(linesAbove, linesBelow, staff)) return FUNCTOR_CONTINUE;

    const int ledgerLineExtension
        = params->m_doc->GetOptions()->m_ledgerLineExtension.GetValue() * params->m_doc->GetDrawingUnit(staffSize);
    int leftExtender = ledgerLineExtension;
    int rightExtender = ledgerLineExtension;
    if (drawingCueSize || (this->GetDrawingDur() >= DUR_8)) {
        leftExtender = 0.7 * ledgerLineExtension;
        rightExtender = 0.5 * ledgerLineExtension;
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
        staff->AddLedgerLineAbove(linesAbove, left, right, drawingCueSize);
    }
    else {
        staff->AddLedgerLineBelow(linesBelow, left, right, drawingCueSize);
    }

    return FUNCTOR_CONTINUE;
}

int Note::PrepareLayerElementParts(FunctorParams *functorParams)
{
    Stem *currentStem = dynamic_cast<Stem *>(this->FindDescendantByType(STEM, 1));
    Flag *currentFlag = NULL;
    Chord *chord = this->IsChordTone();
    if (currentStem) currentFlag = dynamic_cast<Flag *>(currentStem->GetFirst(FLAG));

    if (!this->IsChordTone() && !this->IsMensuralDur()) {
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
        && !this->IsMensuralDur()) {
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
    PrepareLyricsParams *params = vrv_params_cast<PrepareLyricsParams *>(functorParams);
    assert(params);

    params->m_lastButOneNote = params->m_lastNote;
    params->m_lastNote = this;

    return FUNCTOR_CONTINUE;
}

int Note::PreparePointersByLayer(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::PreparePointersByLayer(functorParams);

    PreparePointersByLayerParams *params = vrv_params_cast<PreparePointersByLayerParams *>(functorParams);
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
    GenerateMIDIParams *params = vrv_params_cast<GenerateMIDIParams *>(functorParams);
    assert(params);

    Note *note = vrv_cast<Note *>(this->ThisOrSameasAsLink());
    assert(note);

    // If the note is a secondary tied note, then ignore it
    if (note->GetScoreTimeTiedDuration() < 0.0) {
        return FUNCTOR_SIBLINGS;
    }

    // For now just ignore grace notes
    if (note->IsGraceNote()) {
        return FUNCTOR_SIBLINGS;
    }

    int pitch = 0;
    if (note->HasPnum()) {
        pitch = note->GetPnum();
    }
    else {
        // calc pitch
        int midiBase = 0;
        data_PITCHNAME pname = note->GetPname();
        if (note->HasPnameGes()) pname = note->GetPnameGes();
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
        int oct = note->GetOct();
        if (note->HasOctGes()) oct = note->GetOctGes();

        // Check for accidentals
        midiBase += note->GetChromaticAlteration();

        // Adjustment for transposition intruments
        midiBase += params->m_transSemi;

        pitch = midiBase + (oct + 1) * 12;
    }

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
    GenerateTimemapParams *params = vrv_params_cast<GenerateTimemapParams *>(functorParams);
    assert(params);

    Note *note = vrv_cast<Note *>(this->ThisOrSameasAsLink());
    assert(note);

    double realTimeStart = params->m_realTimeOffsetMilliseconds + note->GetRealTimeOnsetMilliseconds();
    double scoreTimeStart = params->m_scoreTimeOffset + note->GetScoreTimeOnset();

    double realTimeEnd = params->m_realTimeOffsetMilliseconds + note->GetRealTimeOffsetMilliseconds();
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
    TransposeParams *params = vrv_params_cast<TransposeParams *>(functorParams);
    assert(params);

    if (!this->HasPname()) return FUNCTOR_SIBLINGS;

    LogDebug("Transposing note");

    TransPitch pitch = this->GetTransPitch();
    params->m_transposer->Transpose(pitch);
    this->UpdateFromTransPitch(pitch);

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv

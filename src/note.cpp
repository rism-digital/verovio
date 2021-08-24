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
#include "tabgrp.h"
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

static const ClassRegistrar<Note> s_factory("note", NOTE);

Note::Note()
    : LayerElement(NOTE, "note-")
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
    , AttNoteGesTab()
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
    RegisterAttClass(ATT_NOTEGESTAB);
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
    ResetNoteGesTab();
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
    // additional verification for accid and artic - this will not be raised with editorial markup, though
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

void Note::AlignDotsShift(Note *otherNote)
{
    Dots *dots = vrv_cast<Dots *>(this->FindDescendantByType(DOTS, 1));
    Dots *otherDots = vrv_cast<Dots *>(otherNote->FindDescendantByType(DOTS, 1));
    if (!dots || !otherDots) return;
    if (otherDots->GetFlagShift()) {
        dots->SetFlagShift(otherDots->GetFlagShift());
    }
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
    ChordCluster *cluster = m_cluster;
    if (this == cluster->at(0)) return true;
    if (this == cluster->at(cluster->size() - 1))
        return true;
    else
        return false;
}

TabGrp *Note::IsTabGrpNote() const
{
    return dynamic_cast<TabGrp *>(this->GetFirstAncestor(TABGRP, MAX_TABGRP_DEPTH));
}

std::wstring Note::GetTabFretString(data_NOTATIONTYPE notationType) const
{
    if (notationType == NOTATIONTYPE_tab_lute_italian) {
        std::wstring fretStr;
        int fret = this->GetTabFret();
        // Maximum allowed would be 19 (always bindly addind 1 as first figure)
        if (fret > 9) fretStr.push_back(SMUFL_EBE1_luteItalianFret1);
        switch (fret % 10) {
            case 0: fretStr.push_back(SMUFL_EBE0_luteItalianFret0); break;
            case 1: fretStr.push_back(SMUFL_EBE1_luteItalianFret1); break;
            case 2: fretStr.push_back(SMUFL_EBE2_luteItalianFret2); break;
            case 3: fretStr.push_back(SMUFL_EBE3_luteItalianFret3); break;
            case 4: fretStr.push_back(SMUFL_EBE4_luteItalianFret4); break;
            case 5: fretStr.push_back(SMUFL_EBE5_luteItalianFret5); break;
            case 6: fretStr.push_back(SMUFL_EBE6_luteItalianFret6); break;
            case 7: fretStr.push_back(SMUFL_EBE7_luteItalianFret7); break;
            case 8: fretStr.push_back(SMUFL_EBE8_luteItalianFret8); break;
            case 9: fretStr.push_back(SMUFL_EBE9_luteItalianFret9); break;
            default: break;
        }
        return fretStr;
    }
    else if (notationType == NOTATIONTYPE_tab_lute_french) {
        std::wstring fretStr;
        const int fret = GetTabFret();
        const int course = GetTabCourse();
        if (course >= 11) {
            // french tab uses number 4 ... for courses 11 ..., always open fret a.
            // TODO need Baroque font SMUFL_xxxx_luteDiapason4, 5, 6 ... or somesuch.
            //      Temporary kludge, use SMUFL_EBE4_luteItalianFret4 ... .
            fretStr.push_back(SMUFL_EBE4_luteItalianFret4 + course - 11);
        }
        else {
            // courses 8..10 use slashes followed by fret letter
            if (course >= 8) {
                // TODO need SMUFL_xxxx_luteDiapasonSlash or 3 glyphs "/", "//", "///".
                //      Temporary kludge, use SMUFL_E101_noteheadSlashHorizontalEnds, doesn't
                //      look right but serves as a place holder.
                fretStr = std::wstring(course - 7, SMUFL_E101_noteheadSlashHorizontalEnds);
            }

            static const wchar_t letter[] = {
                SMUFL_EBC0_luteFrenchFretA,
                SMUFL_EBC1_luteFrenchFretB,
                SMUFL_EBC2_luteFrenchFretC,
                SMUFL_EBC3_luteFrenchFretD,
                SMUFL_EBC4_luteFrenchFretE,
                SMUFL_EBC5_luteFrenchFretF,
                SMUFL_EBC6_luteFrenchFretG,
                SMUFL_EBC7_luteFrenchFretH,
                SMUFL_EBC8_luteFrenchFretI,
                SMUFL_EBC9_luteFrenchFretK,
                SMUFL_EBCA_luteFrenchFretL,
                SMUFL_EBCB_luteFrenchFretM,
                SMUFL_EBCC_luteFrenchFretN,
            };

            // TODO what if fret > 12?  Some tablatures use fret p.
            if (fret >= 0 && fret < static_cast<int>(sizeof(letter) / sizeof(letter[0]))) fretStr += letter[fret];
        }
        return fretStr;
    }
    else {
        std::string str = StringFormat("%d", this->GetTabFret());
        return UTF8to16(str);
    }
}

bool Note::IsUnisonWith(Note *note, bool ignoreAccid)
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
    // For changing this, change Note::CalcStem and Note::PrepareLayerElementParts
    if (this->IsMensuralDur()) {
        // For mensural notation, get the code and adjust the default stem position
        code = this->GetMensuralNoteheadGlyph();
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
        code = this->GetMensuralNoteheadGlyph();
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

int Note::CalcStemLenInThirdUnits(Staff *staff, data_STEMDIRECTION stemDir)
{
    assert(staff);

    if ((stemDir != STEMDIRECTION_down) && (stemDir != STEMDIRECTION_up)) {
        return 0;
    }

    int baseStem = STANDARD_STEMLENGTH * 3;

    int shortening = 0;

    int unitToLine = (stemDir == STEMDIRECTION_up) ? -this->GetDrawingLoc() + (staff->m_drawingLines - 1) * 2
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
        if (stemDir == STEMDIRECTION_up) {
            shortening = std::min(4, shortening);
        }
        else {
            shortening = std::min(3, shortening);
        }
    }

    baseStem -= shortening;

    return baseStem;
}

wchar_t Note::GetMensuralNoteheadGlyph() const
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

bool Note::IsVisible() const
{
    if (this->HasVisible()) {
        return this->GetVisible() == BOOLEAN_true;
    }
    // if the chord doens't have it, see if all the children are invisible
    else if (this->GetParent() && this->GetParent()->Is(CHORD)) {
        Chord *chord = vrv_cast<Chord *>(this->GetParent());
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

void Note::CalcMIDIPitch(int shift)
{
    if (this->HasPnum()) {
        m_MIDIPitch = this->GetPnum();
    }
    else {
        int midiBase = 0;
        data_PITCHNAME pname = this->GetPname();
        if (this->HasPnameGes()) pname = this->GetPnameGes();
        switch (pname) {
            case PITCHNAME_c: midiBase = 0; break;
            case PITCHNAME_d: midiBase = 2; break;
            case PITCHNAME_e: midiBase = 4; break;
            case PITCHNAME_f: midiBase = 5; break;
            case PITCHNAME_g: midiBase = 7; break;
            case PITCHNAME_a: midiBase = 9; break;
            case PITCHNAME_b: midiBase = 11; break;
            default: break;
        }

        // Check for accidentals
        midiBase += this->GetChromaticAlteration();

        // Apply shift, i.e. from transposition instruments
        midiBase += shift;

        int oct = this->GetOct();
        if (this->HasOctGes()) oct = this->GetOctGes();

        m_MIDIPitch = midiBase + (oct + 1) * 12;
    }
}

double Note::GetScoreTimeOnset() const
{
    return m_scoreTimeOnset;
}

double Note::GetRealTimeOnsetMilliseconds() const
{
    return m_realTimeOnsetMilliseconds;
}

double Note::GetScoreTimeOffset() const
{
    return m_scoreTimeOffset;
}

double Note::GetRealTimeOffsetMilliseconds() const
{
    return m_realTimeOffsetMilliseconds;
}

double Note::GetScoreTimeTiedDuration() const
{
    return m_scoreTimeTiedDuration;
}

double Note::GetScoreTimeDuration() const
{
    return this->GetScoreTimeOffset() - this->GetScoreTimeOnset();
}

char Note::GetMIDIPitch() const
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
    Object *stem = this->GetFirst(STEM);
    if (!stem) return false;

    Flag *flag = dynamic_cast<Flag *>(stem->GetFirst(FLAG));
    if (!flag) return false;

    // for the purposes of vertical spacing we care only up to 16th flags - shorter ones grow upwards
    wchar_t flagGlyph = SMUFL_E242_flag16thUp;
    data_DURATION dur = this->GetDur();
    if (dur < DURATION_16) flagGlyph = flag->GetFlagGlyph(this->GetDrawingStemDir());
    const int flagHeight = doc->GetGlyphHeight(flagGlyph, staffSize, this->GetDrawingCueSize());

    const int dotMargin = flag->GetDrawingY() - this->GetDrawingY() - flagHeight - this->GetDrawingRadius(doc) / 2
        - (isDotShifted ? doc->GetDrawingUnit(staffSize) : 0);

    return dotMargin < 0;
}

//----------------//
// Static methods //
//----------------//

bool Note::HandleLedgerLineStemCollision(Doc *doc, Staff *staff, Note *note1, Note *note2)
{
    assert(doc);
    assert(staff);
    assert(note1);
    assert(note2);

    if (note1->GetDrawingLoc() == note2->GetDrawingLoc()) return false;
    Note *upperNote = (note1->GetDrawingLoc() > note2->GetDrawingLoc()) ? note1 : note2;
    Note *lowerNote = (note1->GetDrawingLoc() > note2->GetDrawingLoc()) ? note2 : note1;

    if (upperNote->GetDrawingStemDir() != STEMDIRECTION_down) return false;
    if (lowerNote->GetDrawingStemDir() != STEMDIRECTION_up) return false;

    // Count ledger lines
    int linesAboveUpper, linesBelowUpper;
    upperNote->HasLedgerLines(linesAboveUpper, linesBelowUpper, staff);
    int linesAboveLower, linesBelowLower;
    lowerNote->HasLedgerLines(linesAboveLower, linesBelowLower, staff);

    const int unit = doc->GetDrawingUnit(staff->m_drawingStaffSize);

    // If one note has more ledger lines, then check if the stems tip of the other note is outside of the staff on this
    // side
    if (linesBelowLower > linesBelowUpper) {
        Chord *upperChord = upperNote->IsChordTone();
        Stem *upperStem = upperChord ? upperChord->GetDrawingStem() : upperNote->GetDrawingStem();
        if (upperStem) {
            const int staffBottom = staff->GetDrawingY() - 2 * unit * (staff->m_drawingLines - 1);
            const int stemBottom = upperStem->GetSelfBottom();
            if (stemBottom < staffBottom - unit) {
                return true;
            }
        }
    }

    if (linesAboveUpper > linesAboveLower) {
        Chord *lowerChord = lowerNote->IsChordTone();
        Stem *lowerStem = lowerChord ? lowerChord->GetDrawingStem() : lowerNote->GetDrawingStem();
        if (lowerStem) {
            const int staffTop = staff->GetDrawingY();
            const int stemTop = lowerStem->GetSelfTop();
            if (stemTop > staffTop + unit) {
                return true;
            }
        }
    }

    return false;
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

int Note::CalcArtic(FunctorParams *functorParams)
{
    CalcArticParams *params = vrv_params_cast<CalcArticParams *>(functorParams);
    assert(params);

    if (this->IsChordTone()) return FUNCTOR_CONTINUE;

    params->m_parent = this;
    params->m_stemDir = this->GetDrawingStemDir();

    Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff);
    Layer *layer = vrv_cast<Layer *>(this->GetFirstAncestor(LAYER));
    assert(layer);

    params->m_staffAbove = staff;
    params->m_staffBelow = staff;
    params->m_layerAbove = layer;
    params->m_layerBelow = layer;
    params->m_crossStaffAbove = false;
    params->m_crossStaffBelow = false;

    if (m_crossStaff) {
        params->m_staffAbove = m_crossStaff;
        params->m_staffBelow = m_crossStaff;
        params->m_layerAbove = m_crossLayer;
        params->m_layerBelow = m_crossLayer;
        params->m_crossStaffAbove = true;
        params->m_crossStaffBelow = true;
    }

    return FUNCTOR_CONTINUE;
}

int Note::AdjustArtic(FunctorParams *functorParams)
{
    AdjustArticParams *params = vrv_params_cast<AdjustArticParams *>(functorParams);
    assert(params);

    if (this->IsChordTone()) return FUNCTOR_CONTINUE;

    params->m_parent = this;
    params->m_articAbove.clear();
    params->m_articBelow.clear();

    return FUNCTOR_CONTINUE;
}

int Note::CalcStem(FunctorParams *functorParams)
{
    CalcStemParams *params = vrv_params_cast<CalcStemParams *>(functorParams);
    assert(params);

    if (!this->IsVisible() || (this->GetStemVisible() == BOOLEAN_false)) {
        return FUNCTOR_SIBLINGS;
    }

    // Stems have been calculated previously in Beam or fTrem - siblings because flags do not need to
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

    if (m_crossStaff) {
        staff = m_crossStaff;
        layer = m_crossLayer;
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

    bool mixedCue = false;
    if (Chord *chord = this->IsChordTone(); chord != NULL) {
        mixedCue = (chord->GetDrawingCueSize() != this->GetDrawingCueSize());
    }

    // Nothing to do for notes that are not in a cluster and without cue mixing
    if (!m_cluster && !mixedCue) return FUNCTOR_SIBLINGS;

    if (m_crossStaff) staff = m_crossStaff;

    int staffSize = staff->m_drawingStaffSize;

    int diameter = 2 * this->GetDrawingRadius(params->m_doc);

    // If chord consists partially of cue notes we may have to shift the noteheads
    int cueShift = 0;
    if (mixedCue && (this->GetDrawingStemDir() == STEMDIRECTION_up)) {
        const double cueScaling = params->m_doc->GetCueScaling();
        assert(cueScaling > 0.0);

        if (this->GetDrawingCueSize()) {
            // Note is cue and chord is not
            cueShift = (1.0 / cueScaling - 1.0) * diameter; // shift to the right
        }
        else {
            // Chord is cue and note is not
            cueShift = (cueScaling - 1.0) * diameter; // shift to the left
        }
    }

    /************** notehead direction **************/

    bool flippedNotehead = false;

    // if the note is clustered, calculations are different
    if (m_cluster) {
        if (this->GetDrawingStemDir() == STEMDIRECTION_down) {
            // stem down/even cluster = noteheads start on left (incorrect side)
            if (m_cluster->size() % 2 == 0) {
                flippedNotehead = (m_clusterPosition % 2 != 0);
            }
            // else they start on normal side
            else {
                flippedNotehead = (m_clusterPosition % 2 == 0);
            }
        }
        else {
            // flipped noteheads start on normal side no matter what
            flippedNotehead = (m_clusterPosition % 2 == 0);
        }
    }

    // positions notehead
    if (flippedNotehead) {
        if (this->GetDrawingStemDir() == STEMDIRECTION_up) {
            this->SetDrawingXRel(diameter - params->m_doc->GetDrawingStemWidth(staffSize));
        }
        else {
            this->SetDrawingXRel(-diameter + params->m_doc->GetDrawingStemWidth(staffSize));
        }
    }
    this->SetDrawingXRel(this->GetDrawingXRel() + cueShift);

    this->SetFlippedNotehead(flippedNotehead);

    return FUNCTOR_SIBLINGS;
}

MapOfNoteLocs Note::CalcNoteLocations()
{
    Layer *layer = NULL;
    Staff *staff = this->GetCrossStaff(layer);
    if (!staff) staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff);

    MapOfNoteLocs noteLocations;
    noteLocations[staff] = { this->GetDrawingLoc() };
    return noteLocations;
}

MapOfDotLocs Note::CalcDotLocations(int layerCount, bool primary)
{
    const bool isUpwardDirection = (GetDrawingStemDir() == STEMDIRECTION_up) || (layerCount == 1);
    const bool shiftUpwards = (isUpwardDirection && primary) || (!isUpwardDirection && !primary);
    MapOfNoteLocs noteLocs = this->CalcNoteLocations();
    assert(noteLocs.size() == 1);

    MapOfDotLocs dotLocs;
    Staff *staff = noteLocs.cbegin()->first;
    int loc = *noteLocs.cbegin()->second.cbegin();
    if (loc % 2 == 0) loc += (shiftUpwards ? 1 : -1);
    dotLocs[staff] = { loc };
    return dotLocs;
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

    if (m_crossStaff) staff = m_crossStaff;

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

        dots->SetMapOfDotLocs(this->CalcOptimalDotLocations());
        const bool isDotShifted = (this->GetDrawingLoc() % 2 == 0);

        // Stem up, shorter than 4th and not in beam
        if (const int shift = dots->GetFlagShift(); shift) {
            flagShift += shift;
        }
        else if ((GetDrawingStemDir() == STEMDIRECTION_up) && (!this->IsInBeam()) && (GetDrawingStemLen() < 3)
            && (IsDotOverlappingWithFlag(params->m_doc, staffSize, isDotShifted))) {
            // HARDCODED
            const int shift = params->m_doc->GetGlyphWidth(SMUFL_E240_flag8thUp, staffSize, drawingCueSize) * 0.8;
            flagShift += shift;
            dots->SetFlagShift(shift);
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

    if (m_crossStaff) staff = m_crossStaff;

    bool drawingCueSize = this->GetDrawingCueSize();
    int staffSize = staff->m_drawingStaffSize;
    int staffX = staff->GetDrawingX();
    int radius = GetDrawingRadius(params->m_doc);

    /************** Ledger lines: **************/

    int linesAbove = 0;
    int linesBelow = 0;

    if (!this->HasLedgerLines(linesAbove, linesBelow, staff)) return FUNCTOR_SIBLINGS;

    const int extension = params->m_doc->GetDrawingLedgerLineExtension(staffSize, drawingCueSize);
    const int left = this->GetDrawingX() - extension - staffX;
    int right = this->GetDrawingX() + 2 * radius + extension - staffX;
    if (this->GetDrawingDur() == DUR_MX) {
        right += 2 * radius;
    }

    if (linesAbove > 0) {
        staff->AddLedgerLineAbove(linesAbove, left, right, extension, drawingCueSize);
    }
    else {
        staff->AddLedgerLineBelow(linesBelow, left, right, extension, drawingCueSize);
    }

    return FUNCTOR_SIBLINGS;
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
        if (this->GetActualDur() < DUR_2 || (this->GetStemVisible() == BOOLEAN_false)) {
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

    // Skip linked notes
    if (this->HasSameasLink()) {
        return FUNCTOR_SIBLINGS;
    }

    // If the note is a secondary tied note, then ignore it
    if (this->GetScoreTimeTiedDuration() < 0.0) {
        return FUNCTOR_SIBLINGS;
    }

    // For now just ignore grace notes
    if (this->IsGraceNote()) {
        return FUNCTOR_SIBLINGS;
    }

    int channel = params->m_midiChannel;
    int velocity = MIDI_VELOCITY;
    if (this->HasVel()) velocity = this->GetVel();

    double starttime = params->m_totalTime + this->GetScoreTimeOnset();

    int tpq = params->m_midiFile->getTPQ();

    // Check if note was expanded into sequence of short notes due to trills/tremolandi
    // Play either the expanded note sequence or a single note
    if (params->m_expandedNotes.find(this) != params->m_expandedNotes.end()) {
        for (const auto &midiNote : params->m_expandedNotes[this]) {
            double stoptime = starttime + midiNote.duration;

            params->m_midiFile->addNoteOn(params->m_midiTrack, starttime * tpq, channel, midiNote.pitch, velocity);
            params->m_midiFile->addNoteOff(params->m_midiTrack, stoptime * tpq, channel, midiNote.pitch);

            starttime = stoptime;
        }
    }
    else {
        double stoptime = params->m_totalTime + this->GetScoreTimeOffset() + this->GetScoreTimeTiedDuration();

        this->CalcMIDIPitch(params->m_transSemi);
        char pitch = this->GetMIDIPitch();

        params->m_midiFile->addNoteOn(params->m_midiTrack, starttime * tpq, channel, pitch, velocity);
        params->m_midiFile->addNoteOff(params->m_midiTrack, stoptime * tpq, channel, pitch);
    }

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

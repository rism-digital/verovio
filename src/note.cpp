/////////////////////////////////////////////////////////////////////////////
// Name:        note.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "note.h"

#include <iostream>
//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "artic.h"
#include "comparison.h"
#include "doc.h"
#include "editorial.h"
#include "elementpart.h"
#include "fermata.h"
#include "functor.h"
#include "functorparams.h"
#include "glyph.h"
#include "gracegrp.h"
#include "layer.h"
#include "ligature.h"
#include "plica.h"
#include "slur.h"
#include "smufl.h"
#include "staff.h"
#include "stem.h"
#include "syl.h"
#include "tabgrp.h"
#include "tie.h"
#include "timemap.h"
#include "tuning.h"
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
    , AttHarmonicFunction()
    , AttMidiVelocity()
    , AttNoteGesTab()
    , AttNoteHeads()
    , AttNoteVisMensural()
    , AttStems()
    , AttStemsCmn()
    , AttTiePresent()
    , AttVisibility()
{
    this->RegisterInterface(DurationInterface::GetAttClasses(), DurationInterface::IsInterface());
    this->RegisterInterface(PitchInterface::GetAttClasses(), PitchInterface::IsInterface());
    this->RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_COLORATION);
    this->RegisterAttClass(ATT_CUE);
    this->RegisterAttClass(ATT_EXTSYM);
    this->RegisterAttClass(ATT_GRACED);
    this->RegisterAttClass(ATT_HARMONICFUNCTION);
    this->RegisterAttClass(ATT_NOTEGESTAB);
    this->RegisterAttClass(ATT_NOTEHEADS);
    this->RegisterAttClass(ATT_NOTEVISMENSURAL);
    this->RegisterAttClass(ATT_MIDIVELOCITY);
    this->RegisterAttClass(ATT_STEMS);
    this->RegisterAttClass(ATT_STEMSCMN);
    this->RegisterAttClass(ATT_TIEPRESENT);
    this->RegisterAttClass(ATT_VISIBILITY);

    this->Reset();
}

Note::~Note() {}

void Note::Reset()
{
    LayerElement::Reset();
    StemmedDrawingInterface::Reset();
    DurationInterface::Reset();
    PitchInterface::Reset();
    PositionInterface::Reset();
    this->ResetColor();
    this->ResetColoration();
    this->ResetCue();
    this->ResetExtSym();
    this->ResetGraced();
    this->ResetHarmonicFunction();
    this->ResetNoteGesTab();
    this->ResetNoteHeads();
    this->ResetNoteVisMensural();
    this->ResetMidiVelocity();
    this->ResetStems();
    this->ResetStemsCmn();
    this->ResetTiePresent();
    this->ResetVisibility();

    m_clusterPosition = 0;
    m_cluster = NULL;

    m_drawingLoc = 0;
    m_flippedNotehead = false;

    m_stemSameas = NULL;
    m_stemSameasRole = SAMEAS_NONE;
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

    ArrayOfObjects &children = this->GetChildrenForModification();

    // Stem are always added by PrepareLayerElementParts (for now) and we want them to be in the front
    // for the drawing order in the SVG output
    if (child->Is({ DOTS, STEM })) {
        children.insert(children.begin(), child);
    }
    else {
        children.push_back(child);
    }
    Modify();
}

void Note::AlignDotsShift(const Note *otherNote)
{
    Dots *dots = vrv_cast<Dots *>(this->FindDescendantByType(DOTS, 1));
    const Dots *otherDots = vrv_cast<const Dots *>(otherNote->FindDescendantByType(DOTS, 1));
    if (!dots || !otherDots) return;
    if (otherDots->GetFlagShift()) {
        dots->SetFlagShift(otherDots->GetFlagShift());
    }
}

Accid *Note::GetDrawingAccid()
{
    return vrv_cast<Accid *>(this->FindDescendantByType(ACCID));
}

const Accid *Note::GetDrawingAccid() const
{
    return vrv_cast<const Accid *>(this->FindDescendantByType(ACCID));
}

bool Note::HasLedgerLines(int &linesAbove, int &linesBelow, const Staff *staff) const
{
    if (!staff) {
        staff = this->GetAncestorStaff();
    }

    linesAbove = (this->GetDrawingLoc() - staff->m_drawingLines * 2 + 2) / 2;
    linesBelow = -(this->GetDrawingLoc()) / 2;

    linesAbove = std::max(linesAbove, 0);
    linesBelow = std::max(linesBelow, 0);

    return ((linesAbove > 0) || (linesBelow > 0));
}

Chord *Note::IsChordTone()
{
    return vrv_cast<Chord *>(this->GetFirstAncestor(CHORD, MAX_CHORD_DEPTH));
}

const Chord *Note::IsChordTone() const
{
    return vrv_cast<const Chord *>(this->GetFirstAncestor(CHORD, MAX_CHORD_DEPTH));
}

int Note::GetDrawingDur() const
{
    const Chord *chordParent = vrv_cast<const Chord *>(this->GetFirstAncestor(CHORD, MAX_CHORD_DEPTH));
    if (chordParent && !this->HasDur()) {
        return chordParent->GetActualDur();
    }
    else {
        return this->GetActualDur();
    }
}

bool Note::IsClusterExtreme() const
{
    ChordCluster *cluster = m_cluster;
    if (this == cluster->at(0)) return true;
    return (this == cluster->at(cluster->size() - 1)) ? true : false;
}

TabGrp *Note::IsTabGrpNote()
{
    return vrv_cast<TabGrp *>(this->GetFirstAncestor(TABGRP, MAX_TABGRP_DEPTH));
}

const TabGrp *Note::IsTabGrpNote() const
{
    return vrv_cast<const TabGrp *>(this->GetFirstAncestor(TABGRP, MAX_TABGRP_DEPTH));
}

std::u32string Note::GetTabFretString(data_NOTATIONTYPE notationType) const
{
    if (notationType == NOTATIONTYPE_tab_lute_italian) {
        std::u32string fretStr;
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
        std::u32string fretStr;
        const int fret = this->GetTabFret();
        const int course = this->GetTabCourse();
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
                fretStr = std::u32string(course - 7, SMUFL_E101_noteheadSlashHorizontalEnds);
            }

            static const char32_t letter[] = {
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
        return UTF8to32(str);
    }
}

bool Note::IsUnisonWith(const Note *note, bool ignoreAccid) const
{
    if (!ignoreAccid && !this->IsEnharmonicWith(note)) return false;

    return ((this->GetPname() == note->GetPname()) && (this->GetOct() == note->GetOct()));
}

void Note::SetCluster(ChordCluster *cluster, int position)
{
    m_cluster = cluster;
    m_clusterPosition = position;
}

Point Note::GetStemUpSE(const Doc *doc, int staffSize, bool isCueSize) const
{
    int defaultYShift = doc->GetDrawingUnit(staffSize) / 4;
    if (isCueSize) defaultYShift = doc->GetCueSize(defaultYShift);
    // x default is always set to the right for now
    int defaultXShift = doc->GetGlyphWidth(this->GetNoteheadGlyph(this->GetActualDur()), staffSize, isCueSize);
    Point p(defaultXShift, defaultYShift);

    // Here we should get the notehead value
    char32_t code = this->GetNoteheadGlyph(this->GetDrawingDur());

    // This is never called for now because mensural notes do not have stem/flag children
    // For changing this, change CalcStemFunctor::VisitNote and PrepareLayerElementPartsFunctor::VisitNote
    if (this->IsMensuralDur()) {
        // For mensural notation, get the code and adjust the default stem position
        code = this->GetMensuralNoteheadGlyph();
        p.y = doc->GetGlyphHeight(code, staffSize, isCueSize) / 2;
        p.x = doc->GetGlyphWidth(code, staffSize, isCueSize);
    }

    const Resources &resources = doc->GetResources();
    const Glyph *glyph = resources.GetGlyph(code);
    assert(glyph);

    if (glyph->HasAnchor(SMUFL_stemUpSE)) {
        const Point *anchor = glyph->GetAnchor(SMUFL_stemUpSE);
        assert(anchor);
        p = doc->ConvertFontPoint(glyph, *anchor, staffSize, isCueSize);
    }

    return p;
}

Point Note::GetStemDownNW(const Doc *doc, int staffSize, bool isCueSize) const
{
    int defaultYShift = doc->GetDrawingUnit(staffSize) / 4;
    if (isCueSize) defaultYShift = doc->GetCueSize(defaultYShift);
    // x default is always set to the left for now
    Point p(0, -defaultYShift);

    // Here we should get the notehead value
    char32_t code = this->GetNoteheadGlyph(this->GetDrawingDur());

    // This is never called for now because mensural notes do not have stem/flag children
    // See comment above
    if (this->IsMensuralDur()) {
        // For mensural notation, get the code and adjust the default stem position
        code = this->GetMensuralNoteheadGlyph();
        p.y = -doc->GetGlyphHeight(code, staffSize, isCueSize) / 2;
        p.x = doc->GetGlyphWidth(code, staffSize, isCueSize);
    }

    const Resources &resources = doc->GetResources();
    const Glyph *glyph = resources.GetGlyph(code);
    assert(glyph);

    if (glyph->HasAnchor(SMUFL_stemDownNW)) {
        const Point *anchor = glyph->GetAnchor(SMUFL_stemDownNW);
        assert(anchor);
        p = doc->ConvertFontPoint(glyph, *anchor, staffSize, isCueSize);
    }

    return p;
}

int Note::CalcStemLenInThirdUnits(const Staff *staff, data_STEMDIRECTION stemDir) const
{
    assert(staff);

    if ((stemDir != STEMDIRECTION_down) && (stemDir != STEMDIRECTION_up)) {
        return 0;
    }

    int baseStem = (staff->IsTablature()) ? STANDARD_STEMLENGTH_TAB : STANDARD_STEMLENGTH;
    baseStem *= 3;

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

char32_t Note::GetMensuralNoteheadGlyph() const
{
    assert(this->IsMensuralDur());

    int drawingDur = this->GetDrawingDur();

    // No SMuFL code used for these values
    if (drawingDur < DUR_1) {
        return 0;
    }

    const Staff *staff = this->GetAncestorStaff();
    bool mensural_black = (staff->m_drawingNotationType == NOTATIONTYPE_mensural_black);

    char32_t code = 0;
    if (mensural_black) {
        code = SMUFL_E938_mensuralNoteheadSemibrevisBlack;
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

char32_t Note::GetNoteheadGlyph(const int duration) const
{
    static std::map<std::string, char32_t> additionalNoteheadSymbols
        = { { "noteheadDiamondBlackWide", SMUFL_E0DC_noteheadDiamondBlackWide },
              { "noteheadDiamondWhiteWide", SMUFL_E0DE_noteheadDiamondWhiteWide },
              { "noteheadNull", SMUFL_E0A5_noteheadNull } };

    if (this->HasGlyphName()) {
        const std::string glyph = this->GetGlyphName();
        if (additionalNoteheadSymbols.end() == additionalNoteheadSymbols.find(glyph)) {
            return SMUFL_E0A4_noteheadBlack;
        }
        return additionalNoteheadSymbols[glyph];
    }

    switch (this->GetHeadShape()) {
        case HEADSHAPE_quarter: return SMUFL_E0A4_noteheadBlack;
        case HEADSHAPE_half: return SMUFL_E0A3_noteheadHalf;
        case HEADSHAPE_whole: return SMUFL_E0A2_noteheadWhole;
        // case HEADSHAPE_backslash: return SMUFL_noteheadBackslash;
        // case HEADSHAPE_circle: return SMUFL_E0B3_noteheadCircleX;
        case HEADSHAPE_plus: return SMUFL_E0AF_noteheadPlusBlack;
        case HEADSHAPE_diamond: {
            if (DUR_4 > duration) return SMUFL_E0D9_noteheadDiamondHalf;
            return SMUFL_E0DB_noteheadDiamondBlack;
        }
        // case HEADSHAPE_isotriangle: return SMUFL_E0BC_noteheadTriangleUpHalf;
        // case HEADSHAPE_oval: return SMUFL_noteheadOval;
        // case HEADSHAPE_piewedge: return SMUFL_noteheadPieWedge;
        // case HEADSHAPE_rectangle: return SMUFL_noteheadRectangle;
        // case HEADSHAPE_rtriangle: return SMUFL_noteheadRTriangle;
        // case HEADSHAPE_semicircle: return SMUFL_noteheadSemicircle;
        case HEADSHAPE_slash: {
            if (DUR_1 >= duration) return SMUFL_E102_noteheadSlashWhiteWhole;
            if (DUR_2 == duration) return SMUFL_E103_noteheadSlashWhiteHalf;
            return SMUFL_E101_noteheadSlashHorizontalEnds;
        }
        // case HEADSHAPE_square: return SMUFL_noteheadSquare;
        case HEADSHAPE_x: {
            if (DUR_1 == duration) return SMUFL_E0B5_noteheadWholeWithX;
            if (DUR_2 == duration) return SMUFL_E0B6_noteheadHalfWithX;
            return SMUFL_E0A9_noteheadXBlack;
        }
        default: break;
    }

    switch (this->GetHeadMod()) {
        case NOTEHEADMODIFIER_dblwhole: return SMUFL_E0A0_noteheadDoubleWhole;
        default: break;
    }

    if (DUR_BR == duration) return SMUFL_E0A1_noteheadDoubleWholeSquare;
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
        const Chord *chord = vrv_cast<const Chord *>(this->GetParent());
        assert(chord);
        return chord->IsVisible();
    }
    return true;
}

data_STEMDIRECTION Note::CalcStemDirForSameasNote(int verticalCenter)
{
    assert(m_stemSameas);
    assert(m_stemSameas->HasStemSameasNote());
    assert(m_stemSameas->GetStemSameasNote() == this);

    // This is the first of the of the note pair, we need to calculate and set the stem direction
    if (m_stemSameasRole == SAMEAS_UNSET) {
        data_STEMDIRECTION stemDir = STEMDIRECTION_up;
        Note *topNote = (this->GetDrawingY() > m_stemSameas->GetDrawingY()) ? this : m_stemSameas;
        Note *bottomNote = (this->GetDrawingY() > m_stemSameas->GetDrawingY()) ? m_stemSameas : this;
        // First check if we have an encoded stem direction
        if (this->HasStemDir()) {
            stemDir = this->GetStemDir();
        }
        // Otherwise auto determine it
        else {
            const int topY = topNote->GetDrawingY();
            const int bottomY = bottomNote->GetDrawingY();
            const int middlePoint = (topY + bottomY) / 2;
            stemDir = (middlePoint > verticalCenter) ? STEMDIRECTION_down : STEMDIRECTION_up;
        }
        // We also set the role to both notes accordingly
        topNote->m_stemSameasRole = (stemDir == STEMDIRECTION_up) ? SAMEAS_PRIMARY : SAMEAS_SECONDARY;
        bottomNote->m_stemSameasRole = (stemDir == STEMDIRECTION_up) ? SAMEAS_SECONDARY : SAMEAS_PRIMARY;

        this->CalcNoteHeadShiftForSameasNote(m_stemSameas, stemDir);

        return stemDir;
    }
    else {
        // Otherwise use the stem direction set for the other note previously when this method was called for it
        return m_stemSameas->GetDrawingStemDir();
    }
}

void Note::CalcNoteHeadShiftForSameasNote(Note *stemSameas, data_STEMDIRECTION stemDir)
{
    assert(stemSameas);

    if (abs(this->GetDiatonicPitch() - stemSameas->GetDiatonicPitch()) > 1) return;

    Note *noteToShift = this;
    if (stemDir == STEMDIRECTION_up) {
        if (this->GetDrawingY() < stemSameas->GetDrawingY()) noteToShift = stemSameas;
    }
    else {
        if (this->GetDrawingY() > stemSameas->GetDrawingY()) noteToShift = stemSameas;
    }
    // With stem sameas we do not correct the position of the note itself because otherwise the children position,
    // including the one of stem, will change as well. We only flag it. The correction is made in View::DrawNote
    // This can potentially cause some problems with dots or accidentals but can be left like this because it is
    // quite a corner case already.
    noteToShift->SetFlippedNotehead(true);
}

bool Note::IsEnharmonicWith(const Note *note) const
{
    return (this->GetMIDIPitch() == note->GetMIDIPitch());
}

int Note::GetMIDIPitch(const int shift) const
{
    int pitch = 0;

    if (this->HasPnum()) {
        pitch = this->GetPnum();
    }
    else if (this->HasPname() || this->HasPnameGes()) {
        const int pclass = this->GetPitchClass();

        int oct = this->GetOct();
        if (this->HasOctGes()) oct = this->GetOctGes();

        pitch = pclass + (oct + 1) * 12;
    }
    else if (this->HasTabCourse()) {
        // tablature
        const Staff *staff = this->GetAncestorStaff();
        if (staff->m_drawingTuning) {
            pitch = staff->m_drawingTuning->CalcPitchNumber(
                this->GetTabCourse(), this->GetTabFret(), staff->m_drawingNotationType);
        }
    }

    // Apply shift, i.e. from transposition instruments
    return pitch + shift;
}

int Note::GetPitchClass() const
{
    // if (this->HasPclass()) return this->GetPclass();

    data_PITCHNAME pname = this->GetPname();
    if (this->HasPnameGes()) pname = this->GetPnameGes();
    int pitchClass = PnameToPclass(pname);

    // Check for accidentals
    pitchClass += this->GetChromaticAlteration();

    return pitchClass;
}

int Note::GetChromaticAlteration() const
{
    const Accid *accid = this->GetDrawingAccid();

    if (accid) {
        return TransPitch::GetChromaticAlteration(accid->GetAccidGes(), accid->GetAccid());
    }
    return 0;
}

TransPitch Note::GetTransPitch() const
{
    int pname = this->GetPname() - PITCHNAME_c;
    return TransPitch(pname, this->GetChromaticAlteration(), this->GetOct());
}

void Note::UpdateFromTransPitch(const TransPitch &tp, bool hasKeySig)
{
    this->SetPname(tp.GetPitchName());

    Accid *accid = this->GetDrawingAccid();
    if (!accid) {
        accid = new Accid();
        this->AddChild(accid);
    }

    bool transposeGesturalAccid = accid->HasAccidGes();
    bool transposeWrittenAccid = accid->HasAccid();
    // TODO: Check the case of both existing but having unequal values.
    if (!accid->HasAccidGes() && !accid->HasAccid()) {
        transposeGesturalAccid = true;
    }

    // Without key signature prefer written accidentals
    if (!hasKeySig && transposeGesturalAccid) {
        accid->ResetAccidentalGes();
        transposeGesturalAccid = false;
        if (tp.m_accid != 0) transposeWrittenAccid = true;
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

void Note::DeferMIDINote(FunctorParams *functorParams, double shift, bool includeChordSiblings)
{
    GenerateMIDIParams *params = vrv_params_cast<GenerateMIDIParams *>(functorParams);
    assert(params);

    // Recursive call for chords
    Chord *chord = this->IsChordTone();
    if (chord && includeChordSiblings) {
        const ListOfObjects &notes = chord->GetList(chord);

        for (Object *obj : notes) {
            Note *note = vrv_cast<Note *>(obj);
            assert(note);
            note->DeferMIDINote(functorParams, shift, false);
        }
        return;
    }

    // Register the shift
    if (shift < this->GetScoreTimeDuration() + this->GetScoreTimeTiedDuration()) {
        params->m_deferredNotes[this] = shift;
    }
}

void Note::GenerateGraceNoteMIDI(FunctorParams *functorParams, double startTime, int tpq, int channel, int velocity)
{
    GenerateMIDIParams *params = vrv_params_cast<GenerateMIDIParams *>(functorParams);
    assert(params);

    double graceNoteDur = 0.0;
    if (params->m_accentedGraceNote && !params->m_graceNotes.empty()) {
        const double totalDur = this->GetScoreTimeDuration() / 2.0;
        this->DeferMIDINote(functorParams, totalDur, true);
        graceNoteDur = totalDur / params->m_graceNotes.size();
    }
    else {
        graceNoteDur = UNACC_GRACENOTE_DUR * params->m_currentTempo / 60000.0;
        const double totalDur = graceNoteDur * params->m_graceNotes.size();
        if (startTime >= totalDur) {
            startTime -= totalDur;
        }
        else {
            this->DeferMIDINote(functorParams, totalDur, true);
        }
    }

    for (const MIDIChord &chord : params->m_graceNotes) {
        const double stopTime = startTime + graceNoteDur;
        for (int pitch : chord.pitches) {
            params->m_midiFile->addNoteOn(params->m_midiTrack, startTime * tpq, channel, pitch, velocity);
            params->m_midiFile->addNoteOff(params->m_midiTrack, stopTime * tpq, channel, pitch);
        }
        startTime = stopTime;
    }
}

//----------------//
// Static methods //
//----------------//

bool Note::HandleLedgerLineStemCollision(const Doc *doc, const Staff *staff, const Note *note1, const Note *note2)
{
    assert(doc);
    assert(staff);
    assert(note1);
    assert(note2);

    if (note1->GetDrawingLoc() == note2->GetDrawingLoc()) return false;
    const Note *upperNote = (note1->GetDrawingLoc() > note2->GetDrawingLoc()) ? note1 : note2;
    const Note *lowerNote = (note1->GetDrawingLoc() > note2->GetDrawingLoc()) ? note2 : note1;

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
        const Chord *upperChord = upperNote->IsChordTone();
        const Stem *upperStem = upperChord ? upperChord->GetDrawingStem() : upperNote->GetDrawingStem();
        if (upperStem) {
            const int staffBottom = staff->GetDrawingY() - 2 * unit * (staff->m_drawingLines - 1);
            const int stemBottom = upperStem->GetSelfBottom();
            if (stemBottom < staffBottom - unit) {
                return true;
            }
        }
    }

    if (linesAboveUpper > linesAboveLower) {
        const Chord *lowerChord = lowerNote->IsChordTone();
        const Stem *lowerStem = lowerChord ? lowerChord->GetDrawingStem() : lowerNote->GetDrawingStem();
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

int Note::PnameToPclass(data_PITCHNAME pitchName)
{
    switch (pitchName) {
        case PITCHNAME_c: return 0; break;
        case PITCHNAME_d: return 2; break;
        case PITCHNAME_e: return 4; break;
        case PITCHNAME_f: return 5; break;
        case PITCHNAME_g: return 7; break;
        case PITCHNAME_a: return 9; break;
        case PITCHNAME_b: return 11; break;
        default: return 0; break;
    }
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

FunctorCode Note::Accept(MutableFunctor &functor)
{
    return functor.VisitNote(this);
}

FunctorCode Note::Accept(ConstFunctor &functor) const
{
    return functor.VisitNote(this);
}

FunctorCode Note::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitNoteEnd(this);
}

FunctorCode Note::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitNoteEnd(this);
}

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
                tie->SetStartid("#" + (*iter)->GetID());
                tie->SetEndid("#" + this->GetID());
                params->m_controlEvents.push_back(tie);
            }
            else {
                LogWarning("Expected @tie median or terminal in note '%s', skipping it", this->GetID().c_str());
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
        fermata->ConvertFromAnalyticalMarkup(this, this->GetID(), params);
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

    Staff *staff = this->GetAncestorStaff();
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

MapOfNoteLocs Note::CalcNoteLocations(NotePredicate predicate) const
{
    if (predicate && !predicate(this)) return {};

    const Staff *staff = this->GetAncestorStaff(RESOLVE_CROSS_STAFF);

    MapOfNoteLocs noteLocations;
    noteLocations[staff] = { this->GetDrawingLoc() };
    return noteLocations;
}

MapOfDotLocs Note::CalcDotLocations(int layerCount, bool primary) const
{
    const bool isUpwardDirection = (this->GetDrawingStemDir() == STEMDIRECTION_up) || (layerCount == 1);
    const bool shiftUpwards = (isUpwardDirection == primary);
    MapOfNoteLocs noteLocs = this->CalcNoteLocations();
    assert(noteLocs.size() == 1);

    MapOfDotLocs dotLocs;
    const Staff *staff = noteLocs.cbegin()->first;
    int loc = *noteLocs.cbegin()->second.cbegin();
    if (loc % 2 == 0) loc += (shiftUpwards ? 1 : -1);
    dotLocs[staff] = { loc };
    return dotLocs;
}

int Note::GenerateMIDI(FunctorParams *functorParams)
{
    GenerateMIDIParams *params = vrv_params_cast<GenerateMIDIParams *>(functorParams);
    assert(params);

    // Skip linked notes
    if (this->HasSameasLink()) {
        return FUNCTOR_SIBLINGS;
    }

    // Skip cue notes when midiNoCue is activated
    if (this->GetCue() == BOOLEAN_true && params->m_cueExclusion) {
        return FUNCTOR_SIBLINGS;
    }

    // If the note is a secondary tied note, then ignore it
    if (this->GetScoreTimeTiedDuration() < 0.0) {
        return FUNCTOR_SIBLINGS;
    }

    // Handle grace notes
    if (this->IsGraceNote()) {
        const int pitch = this->GetMIDIPitch(params->m_transSemi);

        double quarterDuration = 0.0;
        const data_DURATION dur = this->GetDur();
        if ((dur >= DURATION_long) && (dur <= DURATION_1024)) {
            quarterDuration = pow(2.0, (DURATION_4 - dur));
        }

        params->m_graceNotes.push_back({ { pitch }, quarterDuration });

        bool accented = (this->GetGrace() == GRACE_acc);
        GraceGrp *graceGrp = vrv_cast<GraceGrp *>(this->GetFirstAncestor(GRACEGRP));
        if (graceGrp && (graceGrp->GetGrace() == GRACE_acc)) accented = true;
        params->m_accentedGraceNote = accented;

        return FUNCTOR_SIBLINGS;
    }

    const int channel = params->m_midiChannel;
    int velocity = MIDI_VELOCITY;
    if (this->HasVel()) velocity = this->GetVel();

    double startTime = params->m_totalTime + this->GetScoreTimeOnset();
    const int tpq = params->m_midiFile->getTPQ();

    // Check if some grace notes must be performed
    if (!params->m_graceNotes.empty()) {
        this->GenerateGraceNoteMIDI(functorParams, startTime, tpq, channel, velocity);
        params->m_graceNotes.clear();
    }

    // Check if note is deferred
    if (params->m_deferredNotes.find(this) != params->m_deferredNotes.end()) {
        startTime += params->m_deferredNotes.at(this);
        params->m_deferredNotes.erase(this);
    }

    // Check if note was expanded into sequence of short notes due to trills/tremolandi
    // Play either the expanded note sequence or a single note
    if (params->m_expandedNotes.find(this) != params->m_expandedNotes.end()) {
        for (const auto &midiNote : params->m_expandedNotes[this]) {
            const double stopTime = startTime + midiNote.duration;

            params->m_midiFile->addNoteOn(params->m_midiTrack, startTime * tpq, channel, midiNote.pitch, velocity);
            params->m_midiFile->addNoteOff(params->m_midiTrack, stopTime * tpq, channel, midiNote.pitch);

            startTime = stopTime;
        }
    }
    else {
        const int pitch = this->GetMIDIPitch(params->m_transSemi);

        if (this->HasTabCourse() && this->GetTabCourse() >= 1) {
            // Tablature 'rule of holds'.  A note on a course is held until the next note
            // on that course is required, or until a default hold duration is reached.

            const int course = this->GetTabCourse();
            if (params->m_heldNotes.size() < static_cast<size_t>(course))
                params->m_heldNotes.resize(course); // make room

            // if a previously held note on this course is already sounding, end it now.
            if (params->m_heldNotes[course - 1].m_pitch > 0)
                params->m_heldNotes[course - 1].m_stopTime = startTime; // stop now

            // end all previously held notes that have reached their stoptime
            for (auto &held : params->m_heldNotes) {
                if (held.m_pitch > 0 && held.m_stopTime <= startTime) {
                    params->m_midiFile->addNoteOff(params->m_midiTrack, held.m_stopTime * tpq, channel, held.m_pitch);
                    held.m_pitch = 0;
                    held.m_stopTime = 0;
                }
            }

            // hold this note until the greater of its rhythm sign and the default duration.
            // TODO optimize the default hold duration
            const double defaultHoldTime = 4; // quarter notes
            params->m_heldNotes[course - 1].m_pitch = pitch;
            params->m_heldNotes[course - 1].m_stopTime = params->m_totalTime
                + std::max(defaultHoldTime, this->GetScoreTimeOffset() + this->GetScoreTimeTiedDuration());

            // start this note
            params->m_midiFile->addNoteOn(params->m_midiTrack, startTime * tpq, channel, pitch, velocity);
        }
        else {
            const double stopTime = params->m_totalTime + this->GetScoreTimeOffset() + this->GetScoreTimeTiedDuration();

            params->m_midiFile->addNoteOn(params->m_midiTrack, startTime * tpq, channel, pitch, velocity);
            params->m_midiFile->addNoteOff(params->m_midiTrack, stopTime * tpq, channel, pitch);
        }
    }

    // Store reference, i.e. for Nachschlag
    params->m_lastNote = this;

    return FUNCTOR_CONTINUE;
}

int Note::GenerateTimemap(FunctorParams *functorParams)
{
    GenerateTimemapParams *params = vrv_params_cast<GenerateTimemapParams *>(functorParams);
    assert(params);

    if (this->HasGrace()) return FUNCTOR_SIBLINGS;

    // Skip cue notes when midiNoCue is activated
    if (this->GetCue() == BOOLEAN_true && params->m_cueExclusion) {
        return FUNCTOR_SIBLINGS;
    }

    Note *note = vrv_cast<Note *>(this->ThisOrSameasLink());
    assert(note);

    params->m_timemap->AddEntry(note, params);

    return FUNCTOR_SIBLINGS;
}

int Note::Transpose(FunctorParams *functorParams)
{
    TransposeParams *params = vrv_params_cast<TransposeParams *>(functorParams);
    assert(params);

    if (!this->HasPname()) return FUNCTOR_SIBLINGS;

    TransPitch pitch = this->GetTransPitch();
    params->m_transposer->Transpose(pitch);

    const int staffN = this->GetAncestorStaff(RESOLVE_CROSS_STAFF)->GetN();
    const bool hasKeySig = ((params->m_keySigForStaffN.count(staffN) > 0) || (params->m_keySigForStaffN.count(-1) > 0));
    this->UpdateFromTransPitch(pitch, hasKeySig);

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv

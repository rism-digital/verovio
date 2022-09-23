/////////////////////////////////////////////////////////////////////////////
// Name:        stem.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "stem.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "btrem.h"
#include "chord.h"
#include "elementpart.h"
#include "functorparams.h"
#include "note.h"
#include "smufl.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Stem
//----------------------------------------------------------------------------

static const ClassRegistrar<Note> s_factory("stem", STEM);

Stem::Stem() : LayerElement(STEM, "stem-"), AttGraced(), AttStemVis(), AttVisibility()
{
    this->RegisterAttClass(ATT_GRACED);
    this->RegisterAttClass(ATT_STEMVIS);
    this->RegisterAttClass(ATT_VISIBILITY);

    this->Reset();
}

Stem::~Stem() {}

void Stem::Reset()
{
    LayerElement::Reset();
    this->ResetGraced();
    this->ResetStemVis();
    this->ResetVisibility();

    m_drawingStemDir = STEMDIRECTION_NONE;
    m_drawingStemLen = 0;
    m_drawingStemMod = STEMMODIFIER_NONE;
    m_drawingStemAdjust = 0;
    m_isVirtual = false;
    m_stemModRelY = 0;
}

bool Stem::IsSupportedChild(Object *child)
{
    if (child->Is(FLAG)) {
        assert(dynamic_cast<Flag *>(child));
    }
    else {
        return false;
    }
    return true;
}

void Stem::FillAttributes(const AttStems &attSource)
{
    if (attSource.HasStemDir()) {
        this->SetDir(attSource.GetStemDir());
    }
    if (attSource.HasStemLen()) {
        this->SetLen(attSource.GetStemLen());
    }
    if (attSource.HasStemPos()) {
        this->SetPos(attSource.GetStemPos());
    }
    if (attSource.HasStemMod()) {
        this->SetDrawingStemMod(attSource.GetStemMod());
    }
    if (attSource.HasStemVisible()) {
        this->SetVisible(attSource.GetStemVisible());
    }
}

int Stem::CompareToElementPosition(const Doc *doc, const LayerElement *otherElement, int margin) const
{
    const Staff *staff = this->GetAncestorStaff();

    // check if there is an overlap on the left or on the right and displace stem's parent correspondingly
    const int right = HorizontalLeftOverlap(otherElement, doc, margin, 0);
    const int left = HorizontalRightOverlap(otherElement, doc, margin, 0);
    if (!right || !left) return 0;

    int horizontalMargin = 2 * doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
    const Flag *currentFlag = vrv_cast<const Flag *>(this->FindDescendantByType(FLAG, 1));
    if (currentFlag && currentFlag->m_drawingNbFlags) {
        char32_t flagGlyph = currentFlag->GetFlagGlyph(STEMDIRECTION_down);
        const int flagWidth = doc->GetGlyphWidth(flagGlyph, staff->m_drawingStaffSize, this->GetDrawingCueSize());
        horizontalMargin += flagWidth;
    }

    if (right < left) {
        return right + horizontalMargin;
    }
    else {
        return (-horizontalMargin - left);
    }
}

void Stem::AdjustFlagPlacement(const Doc *doc, Flag *flag, int staffSize, int verticalCenter, int duration)
{
    assert(this->GetParent());
    assert(this->GetParent()->IsLayerElement());

    LayerElement *parent = vrv_cast<LayerElement *>(this->GetParent());
    if (!parent) return;

    const data_STEMDIRECTION stemDirection = this->GetDrawingStemDir();
    // For overlapping purposes we don't care for flags shorter than 16th since they grow in opposite direction
    char32_t flagGlyph = SMUFL_E242_flag16thUp;
    if (duration < DURATION_16) flagGlyph = flag->GetFlagGlyph(stemDirection);
    const int glyphHeight = doc->GetGlyphHeight(flagGlyph, staffSize, this->GetDrawingCueSize());

    // Make sure that flags don't overlap with notehead. Upward flags cannot overlap with noteheads so check
    // only downward ones
    const int adjustmentStep = doc->GetDrawingUnit(staffSize);
    if (stemDirection == STEMDIRECTION_down) {
        const int noteheadMargin = this->GetDrawingStemLen() - (glyphHeight + parent->GetDrawingRadius(doc));
        if ((duration > DURATION_16) && (noteheadMargin < 0)) {
            int offset = 0;
            if (noteheadMargin % adjustmentStep < -adjustmentStep / 3 * 2) offset = adjustmentStep / 2;
            const int heightToAdjust = (noteheadMargin / adjustmentStep) * adjustmentStep - offset;
            this->SetDrawingStemLen(this->GetDrawingStemLen() - heightToAdjust);
            flag->SetDrawingYRel(-this->GetDrawingStemLen());
        }
    }

    Note *note = NULL;
    if (parent->Is(NOTE)) {
        note = vrv_cast<Note *>(parent);
    }
    else if (parent->Is(CHORD)) {
        note = vrv_cast<Chord *>(parent)->GetTopNote();
    }
    int ledgerAbove = 0;
    int ledgerBelow = 0;
    if (!note || !note->HasLedgerLines(ledgerAbove, ledgerBelow)) return;
    if (((stemDirection == STEMDIRECTION_up) && !ledgerBelow)
        || ((stemDirection == STEMDIRECTION_down) && !ledgerAbove))
        return;

    // Make sure that flags don't overlap with first (top or bottom) ledger line (effectively avoiding all ledgers)
    const int directionBias = (stemDirection == STEMDIRECTION_down) ? -1 : 1;
    const int position = this->GetDrawingY() - this->GetDrawingStemLen() - directionBias * glyphHeight;
    const int ledgerPosition = verticalCenter - 6 * directionBias * adjustmentStep;
    const int displacementMargin = (position - ledgerPosition) * directionBias;

    if (displacementMargin < 0) {
        int offset = 0;
        if ((stemDirection == STEMDIRECTION_down) && (displacementMargin % adjustmentStep > -adjustmentStep / 3)) {
            offset = adjustmentStep / 2;
        }
        const int heightToAdjust = (displacementMargin / adjustmentStep - 1) * adjustmentStep * directionBias - offset;
        this->SetDrawingStemLen(this->GetDrawingStemLen() + heightToAdjust);
        flag->SetDrawingYRel(-this->GetDrawingStemLen());
    }
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Stem::AdjustSlashes(const Doc *doc, const Staff *staff, int flagOffset) const
{
    // if stem length is explicitly set - exit
    if (this->HasLen()) return 0;

    const int staffSize = staff->m_drawingStaffSize;
    const int unit = doc->GetDrawingUnit(staffSize);
    data_STEMMODIFIER stemMod = STEMMODIFIER_NONE;
    const BTrem *bTrem = vrv_cast<const BTrem *>(this->GetFirstAncestor(BTREM));
    if (bTrem) {
        stemMod = bTrem->GetDrawingStemMod();
    }
    else if (this->HasDrawingStemMod() && (this->GetDrawingStemMod() < 8)) {
        stemMod = this->GetDrawingStemMod();
    }
    if ((stemMod == STEMMODIFIER_NONE) || (stemMod == STEMMODIFIER_none)) return 0;

    const char32_t code = this->StemModToGlyph(stemMod);
    // if there is no glyph - do nothing
    if (!code) return 0;

    int lenAdjust = flagOffset;
    if (this->GetParent()->Is(CHORD)) {
        const Chord *chord = vrv_cast<const Chord *>(this->GetParent());
        lenAdjust += std::abs(chord->GetTopNote()->GetDrawingY() - chord->GetBottomNote()->GetDrawingY());
    }

    const int glyphHeight = doc->GetGlyphHeight(code, staffSize, false);
    const int actualLength = std::abs(this->GetDrawingStemLen()) - lenAdjust / unit * unit;
    const int diff = actualLength - std::abs(m_stemModRelY) - 0.5 * glyphHeight;
    const int halfUnit = 0.5 * unit;

    int adjust = 0;
    if ((diff < halfUnit) && (diff >= -halfUnit)) {
        adjust = halfUnit;
    }
    else if (diff < -halfUnit) {
        adjust = (std::abs(diff) / halfUnit + 1) * halfUnit;
        if (stemMod == STEMMODIFIER_6slash) {
            adjust += doc->GetGlyphHeight(SMUFL_E220_tremolo1, staffSize, false) / 4;
        }
    }
    return ((this->GetDrawingStemDir() == STEMDIRECTION_up) ? -adjust : adjust);
}

int Stem::CalcStem(FunctorParams *functorParams)
{
    CalcStemParams *params = vrv_params_cast<CalcStemParams *>(functorParams);
    assert(params);

    assert(params->m_staff);
    assert(params->m_layer);
    assert(params->m_interface);

    const int staffSize = params->m_staff->m_drawingStaffSize;
    const int stemShift = params->m_doc->GetDrawingStemWidth(staffSize) / 2;
    const bool drawingCueSize = this->GetDrawingCueSize();

    // For notes longer than half notes the stem is always 0
    if (params->m_dur < DUR_2) {
        this->SetDrawingXRel(0);
        this->SetDrawingYRel(0);
        this->SetDrawingStemLen(0);
        const int adjust = this->CalculateStemModAdjustment(params->m_doc, params->m_staff, 0);
        if (adjust) this->SetDrawingStemLen(this->GetDrawingStemLen() + adjust);
        return FUNCTOR_CONTINUE;
    }

    /************ Set the position, the length and adjust to the note head ************/

    const int unit = params->m_doc->GetDrawingUnit(staffSize);
    int baseStem = 0;
    // Use the given one if any
    if (this->HasLen()) {
        baseStem = this->GetLen() * -unit;
    }
    // Do not adjust the baseStem for stem sameas notes (its length is in m_chordStemLength)
    else if (!params->m_isStemSameasSecondary) {
        const int thirdUnit = unit / 3;
        const data_STEMDIRECTION stemDir = params->m_interface->GetDrawingStemDir();
        baseStem = -(params->m_interface->CalcStemLenInThirdUnits(params->m_staff, stemDir) * thirdUnit);
        if (drawingCueSize) baseStem = params->m_doc->GetCueSize(baseStem);
    }
    // Even if a stem length is given we add the length of the chord content (however only if not 0)
    // Also, the given stem length is understood as being measured from the center of the note.
    // This means that it will be adjusted according to the note head (see below
    if (!params->m_staff || !this->HasLen() || (this->GetLen() != 0)) {
        Point p;
        if (this->GetDrawingStemDir() == STEMDIRECTION_up) {
            if (this->GetPos() == STEMPOSITION_left) {
                p = params->m_interface->GetStemDownNW(params->m_doc, staffSize, drawingCueSize);
                p.x += stemShift;
            }
            else {
                p = params->m_interface->GetStemUpSE(params->m_doc, staffSize, drawingCueSize);
                p.x -= stemShift;
            }
            const int stemShotening = (params->m_isStemSameasSecondary) ? 0 : p.y;
            this->SetDrawingStemLen(baseStem + params->m_chordStemLength + stemShotening);
        }
        else {
            if (this->GetPos() == STEMPOSITION_right) {
                p = params->m_interface->GetStemUpSE(params->m_doc, staffSize, drawingCueSize);
                p.x -= stemShift;
            }
            else {
                p = params->m_interface->GetStemDownNW(params->m_doc, staffSize, drawingCueSize);
                p.x += stemShift;
            }
            const int stemShotening = (params->m_isStemSameasSecondary) ? 0 : p.y;
            this->SetDrawingStemLen(-(baseStem + params->m_chordStemLength - stemShotening));
        }
        this->SetDrawingYRel(this->GetDrawingYRel() + p.y);
        this->SetDrawingXRel(p.x);
    }

    /************ Set flag and slashes (if necessary) and adjust the length ************/
    int flagOffset = 0;
    Flag *flag = NULL;
    // There is never a flag with a duration longer than 8th notes
    if (params->m_dur > DUR_4) {
        flag = vrv_cast<Flag *>(this->GetFirst(FLAG));
        assert(flag);
        // There is never a flag with stem sameas notes
        if (params->m_isStemSameasSecondary) {
            flag->m_drawingNbFlags = 0;
        }
        else {
            flag->m_drawingNbFlags = params->m_dur - DUR_4;
            flagOffset = unit * (flag->m_drawingNbFlags + 1);
        }
    }

    // SMUFL flags cover some additional stem length from the 32th only
    if (flag) {
        flag->SetDrawingYRel(-this->GetDrawingStemLen());
    }

    // Do not adjust the length with stem sameas notes or if given in the encoding
    // however, the stem will be extend with the SMuFL extension from 32th - this can be improved
    if (params->m_isStemSameasSecondary || this->HasLen()) {
        if ((this->GetLen() == 0) && flag) flag->m_drawingNbFlags = 0;
        return FUNCTOR_CONTINUE;
    }
    if ((this->GetVisible() == BOOLEAN_false) && flag) {
        flag->m_drawingNbFlags = 0;
        return FUNCTOR_CONTINUE;
    }

    int flagHeight = 0;

    // SMUFL flags cover some additional stem length from the 32th only
    if (params->m_dur > DUR_16) {
        assert(flag);
        Point stemEnd;
        if (this->GetDrawingStemDir() == STEMDIRECTION_up) {
            stemEnd = flag->GetStemUpSE(params->m_doc, staffSize, drawingCueSize);
        }
        else {
            stemEnd = flag->GetStemDownNW(params->m_doc, staffSize, drawingCueSize);
        }
        // Trick for shortening the stem with DUR_8
        flagHeight = stemEnd.y;
    }

    int endY = this->GetDrawingY() - this->GetDrawingStemLen() + flagHeight;
    bool adjust = false;
    if ((this->GetDrawingStemDir() == STEMDIRECTION_up) && (endY < params->m_verticalCenter)) {
        adjust = true;
    }
    else if ((this->GetDrawingStemDir() == STEMDIRECTION_down) && (endY > params->m_verticalCenter)) {
        adjust = true;
    }

    // Do not adjust the length of grace notes - this is debatable and should probably become as styling option
    // However we still want flags from grace notes not to overlap with ledger lines
    if (adjust && !params->m_isGraceNote) {
        this->SetDrawingStemLen(this->GetDrawingStemLen() + (endY - params->m_verticalCenter));
        if (flag) flag->SetDrawingYRel(-this->GetDrawingStemLen());
    }

    if (!params->m_isGraceNote && !drawingCueSize && !params->m_isStemSameasSecondary) {
        const int adjust = this->CalculateStemModAdjustment(params->m_doc, params->m_staff, flagOffset);
        if (adjust) this->SetDrawingStemLen(this->GetDrawingStemLen() + adjust);
        if (flag) flag->SetDrawingYRel(-this->GetDrawingStemLen());
    }

    if (flag) AdjustFlagPlacement(params->m_doc, flag, staffSize, params->m_verticalCenter, params->m_dur);

    return FUNCTOR_CONTINUE;
}

void Stem::CalculateStemModRelY(const Doc *doc, const Staff *staff)
{
    const int sign = (this->GetDrawingStemDir() == STEMDIRECTION_up) ? 1 : -1;
    LayerElement *parent = vrv_cast<LayerElement *>(this->GetParent());
    // Get note
    Note *note = NULL;
    if (parent->Is(NOTE)) {
        note = vrv_cast<Note *>(parent);
    }
    else if (parent->Is(CHORD)) {
        note = (sign > 0) ? vrv_cast<Chord *>(parent)->GetTopNote() : vrv_cast<Chord *>(parent)->GetBottomNote();
    }
    if (!note || note->IsGraceNote() || note->GetDrawingCueSize()) return;

    // Get stem mod for the element
    data_STEMMODIFIER stemMod = STEMMODIFIER_NONE;
    BTrem *bTrem = vrv_cast<BTrem *>(this->GetFirstAncestor(BTREM));
    if (bTrem) {
        stemMod = bTrem->GetDrawingStemMod();
    }
    else if (this->HasDrawingStemMod() && (this->GetDrawingStemMod() < 8)) {
        stemMod = this->GetDrawingStemMod();
    }
    if ((stemMod == STEMMODIFIER_NONE) || (stemMod == STEMMODIFIER_none)) return;

    // calculate height offset for positioning of stem mod elements on the stem
    const char32_t code = this->StemModToGlyph(stemMod);
    if (!code) return;

    const int unit = doc->GetDrawingUnit(staff->m_drawingStaffSize);
    const int glyphHalfHeight = doc->GetGlyphHeight(code, staff->m_drawingStaffSize, false) / 2;
    const int noteLoc = note->GetDrawingLoc();
    int height = 2 * unit;
    switch (stemMod) {
        case STEMMODIFIER_1slash:
        case STEMMODIFIER_2slash:
        case STEMMODIFIER_3slash:
        case STEMMODIFIER_4slash:
        case STEMMODIFIER_5slash:
        case STEMMODIFIER_6slash: {
            if (noteLoc % 2 == 0) height += unit;
            height += glyphHalfHeight;
            if (stemMod == STEMMODIFIER_6slash)
                height += doc->GetGlyphHeight(SMUFL_E220_tremolo1, staff->m_drawingStaffSize, false) / 2;
            break;
        }
        case STEMMODIFIER_sprech:
        case STEMMODIFIER_z: {
            height += (noteLoc % 2) ? 3 * unit : 2 * unit;
            if (stemMod == STEMMODIFIER_sprech) height -= sign * glyphHalfHeight;
            break;
        }
        default: return;
    }

    // calculate adjust for the stem modifiers that overlap with ledger lines
    const int position = note->GetDrawingY() + sign * height;
    const int staffSize = staff->m_drawingStaffSize;
    const int doubleUnit = 2 * unit;
    const int margin = (sign > 0) ? staff->GetDrawingY() - doc->GetDrawingStaffSize(staffSize) : staff->GetDrawingY();
    const int ledgerLineDifference = margin - (position - sign * glyphHalfHeight);
    const int adjust = (sign * ledgerLineDifference > 0) ? (ledgerLineDifference / doubleUnit) * doubleUnit : 0;

    m_stemModRelY = sign * height + adjust;
}

int Stem::CalculateStemModAdjustment(const Doc *doc, const Staff *staff, int flagOffset)
{
    this->CalculateStemModRelY(doc, staff);
    return this->AdjustSlashes(doc, staff, flagOffset);
}

int Stem::ResetData(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetData(functorParams);

    m_drawingStemDir = STEMDIRECTION_NONE;
    m_drawingStemLen = 0;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

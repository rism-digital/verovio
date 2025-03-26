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
#include "doc.h"
#include "elementpart.h"
#include "functor.h"
#include "note.h"
#include "smufl.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Stem
//----------------------------------------------------------------------------

static const ClassRegistrar<Note> s_factory("stem", STEM);

Stem::Stem() : LayerElement(STEM), AttGraced(), AttStemVis(), AttVisibility()
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

bool Stem::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ FLAG };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else {
        return false;
    }
}

void Stem::FillAttributes(const AttStems &attSource)
{
    if (attSource.HasStemDir()) {
        this->SetDir(attSource.GetStemDir());
    }
    if (attSource.HasStemLen()) {
        data_MEASUREMENTSIGNED stemLen;
        stemLen.SetVu(attSource.GetStemLen());
        this->SetLen(stemLen);
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

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

FunctorCode Stem::Accept(Functor &functor)
{
    return functor.VisitStem(this);
}

FunctorCode Stem::Accept(ConstFunctor &functor) const
{
    return functor.VisitStem(this);
}

FunctorCode Stem::AcceptEnd(Functor &functor)
{
    return functor.VisitStemEnd(this);
}

FunctorCode Stem::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitStemEnd(this);
}

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
    else if (this->HasDrawingStemMod() && (this->GetDrawingStemMod() < STEMMODIFIER_MAX)) {
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
    int diff = 0;
    if ((stemMod == STEMMODIFIER_sprech) && (this->GetDrawingStemDir() == STEMDIRECTION_down)) {
        diff = std::abs(actualLength - std::abs(m_stemModRelY));
    }
    else {
        diff = actualLength - std::abs(m_stemModRelY) - 0.5 * glyphHeight;
    }
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
        Chord *chord = vrv_cast<Chord *>(parent);
        assert(chord);
        note = (sign > 0) ? chord->GetTopNote() : chord->GetBottomNote();
    }
    if (!note || note->IsGraceNote() || note->GetDrawingCueSize()) return;

    // Get stem mod for the element
    data_STEMMODIFIER stemMod = STEMMODIFIER_NONE;
    BTrem *bTrem = vrv_cast<BTrem *>(this->GetFirstAncestor(BTREM));
    if (bTrem) {
        stemMod = bTrem->GetDrawingStemMod();
    }
    else if (this->HasDrawingStemMod() && (this->GetDrawingStemMod() < STEMMODIFIER_MAX)) {
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
        case STEMMODIFIER_1slash: [[fallthrough]];
        case STEMMODIFIER_2slash: [[fallthrough]];
        case STEMMODIFIER_3slash: [[fallthrough]];
        case STEMMODIFIER_4slash: [[fallthrough]];
        case STEMMODIFIER_5slash: [[fallthrough]];
        case STEMMODIFIER_6slash: {
            if (noteLoc % 2 == 0) height += unit;
            height += glyphHalfHeight;
            if (stemMod == STEMMODIFIER_6slash)
                height += doc->GetGlyphHeight(SMUFL_E220_tremolo1, staff->m_drawingStaffSize, false) / 2;
            break;
        }
        case STEMMODIFIER_sprech: [[fallthrough]];
        case STEMMODIFIER_z: {
            height += unit;
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

} // namespace vrv

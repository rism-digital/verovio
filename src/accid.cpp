/////////////////////////////////////////////////////////////////////////////
// Name:        accid.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "accid.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "doc.h"
#include "functorparams.h"
#include "note.h"
#include "smufl.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Accid
//----------------------------------------------------------------------------

static const ClassRegistrar<Accid> s_factory("accid", ACCID);

Accid::Accid()
    : LayerElement(ACCID, "accid-")
    , PositionInterface()
    , AttAccidental()
    , AttAccidentalGestural()
    , AttAccidLog()
    , AttColor()
    , AttEnclosingChars()
    , AttExtSym()
{

    RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    RegisterAttClass(ATT_ACCIDENTAL);
    RegisterAttClass(ATT_ACCIDENTALGESTURAL);
    RegisterAttClass(ATT_ACCIDLOG);
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_ENCLOSINGCHARS);
    RegisterAttClass(ATT_EXTSYM);

    Reset();
}

Accid::~Accid() {}

void Accid::Reset()
{
    LayerElement::Reset();
    PositionInterface::Reset();
    ResetAccidental();
    ResetAccidentalGestural();
    ResetAccidLog();
    ResetColor();
    ResetEnclosingChars();
    ResetExtSym();

    m_drawingUnison = NULL;
}

std::wstring Accid::GetSymbolStr(const data_NOTATIONTYPE notationType) const
{
    if (!this->HasAccid()) return L"";

    wchar_t code = 0;

    // If there is glyph.num, prioritize it
    if (HasGlyphNum()) {
        code = GetGlyphNum();
        if (NULL == Resources::GetGlyph(code)) code = 0;
    }
    // If there is glyph.name (second priority)
    else if (HasGlyphName()) {
        wchar_t code = Resources::GetGlyphCode(GetGlyphName());
        if (NULL == Resources::GetGlyph(code)) code = 0;
    }

    if (!code) {
        switch (notationType) {
            case NOTATIONTYPE_mensural:
            case NOTATIONTYPE_mensural_black:
            case NOTATIONTYPE_mensural_white:
                switch (GetAccid()) {
                    case ACCIDENTAL_WRITTEN_s: code = SMUFL_E9E3_medRenSharpCroix; break;
                    case ACCIDENTAL_WRITTEN_f: code = SMUFL_E9E0_medRenFlatSoftB; break;
                    case ACCIDENTAL_WRITTEN_n: code = SMUFL_E9E2_medRenNatural; break;
                    // we do not want to ignore non-mensural accidentals
                    default: code = GetAccidGlyph(GetAccid()); break;
                }
                break;
            default: code = GetAccidGlyph(GetAccid()); break;
        }
    }

    std::wstring symbolStr;
    if (this->HasEnclose()) {
        if (this->GetEnclose() == ENCLOSURE_brack) {
            symbolStr.push_back(SMUFL_E26C_accidentalBracketLeft);
            symbolStr.push_back(code);
            symbolStr.push_back(SMUFL_E26D_accidentalBracketRight);
        }
        else {
            symbolStr.push_back(SMUFL_E26A_accidentalParensLeft);
            symbolStr.push_back(code);
            symbolStr.push_back(SMUFL_E26B_accidentalParensRight);
        }
    }
    else {
        symbolStr.push_back(code);
    }
    return symbolStr;
}

void Accid::AdjustToLedgerLines(Doc *doc, LayerElement *element, int staffSize)
{
    Staff *staff = element->GetAncestorStaff(RESOLVE_CROSS_STAFF);
    Chord *chord = vrv_cast<Chord *>(this->GetFirstAncestor(CHORD));

    if (element->Is(NOTE) && chord && chord->HasAdjacentNotesInStaff(staff)) {
        const int horizontalMargin = 4 * doc->GetDrawingStemWidth(staffSize);
        const int drawingUnit = doc->GetDrawingUnit(staffSize);
        const int staffTop = staff->GetDrawingY();
        const int staffBottom = staffTop - doc->GetDrawingStaffSize(staffSize);
        if (this->HorizontalContentOverlap(element, 0)) {
            if (((this->GetContentTop() > staffTop + 2 * drawingUnit) && (this->GetDrawingY() < element->GetDrawingY()))
                || ((this->GetContentBottom() < staffBottom - 2 * drawingUnit)
                    && (this->GetDrawingY() > element->GetDrawingY()))) {
                const int xRelShift = this->GetSelfRight() - element->GetSelfLeft() + horizontalMargin;
                if (xRelShift > 0) this->SetDrawingXRel(this->GetDrawingXRel() - xRelShift);
            }
        }
    }
}

void Accid::AdjustX(LayerElement *element, Doc *doc, int staffSize, std::vector<Accid *> &leftAccids,
    std::vector<Accid *> &adjustedAccids)
{
    assert(element);
    assert(doc);

    if (this == element) return;

    const int verticalMargin = 1 * doc->GetDrawingStemWidth(staffSize);
    int horizontalMargin = 2 * doc->GetDrawingStemWidth(staffSize);

    if (element->Is(NOTE)) horizontalMargin = 3 * doc->GetDrawingStemWidth(staffSize);

    if (!this->VerticalSelfOverlap(element, verticalMargin)) {
        this->AdjustToLedgerLines(doc, element, staffSize);
        return;
    }

    // Look for identical accidentals that needs to remain superimposed
    if (element->Is(ACCID) && (this->GetDrawingY() == element->GetDrawingY())) {
        Accid *accid = vrv_cast<Accid *>(element);
        assert(accid);
        if (this->GetSymbolStr(NOTATIONTYPE_NONE) == accid->GetSymbolStr(NOTATIONTYPE_NONE)) {
            // There is the same accidental, so we leave it a the same place
            // This should also work for the chords on multiple layers by setting unison accidental
            accid->SetDrawingUnisonAccid(this);
            return;
        }
    }

    if (element->Is(ACCID)) {
        Accid *accid = vrv_cast<Accid *>(element);
        if (!this->HorizontalLeftOverlap(element, doc, horizontalMargin, verticalMargin)) {
            // There is enough space on the right of the accidental, but maybe we will need to
            // adjust it again (see recursive call below), so keep the accidental that is on the left
            leftAccids.push_back(accid);
            return;
        }
        if (std::find(adjustedAccids.begin(), adjustedAccids.end(), accid) == adjustedAccids.end()) return;
    }

    int xRelShift = 0;
    if (element->Is(STEM)) {
        xRelShift = this->GetSelfRight() - element->GetSelfLeft() + horizontalMargin;
    }
    else {
        xRelShift = this->HorizontalRightOverlap(element, doc, horizontalMargin, verticalMargin);
    }

    // Move only to the left
    if (xRelShift > 0) {
        this->SetDrawingXRel(this->GetDrawingXRel() - xRelShift);
        if (std::find(adjustedAccids.begin(), adjustedAccids.end(), this) == adjustedAccids.end())
            adjustedAccids.push_back(this);
        // We have some accidentals on the left, check again with all of these
        if (!leftAccids.empty()) {
            std::vector<Accid *> leftAccidsSubset;
            std::vector<Accid *>::iterator iter;
            // Recursively adjust all accidental that are on the left because enough space was previously available
            for (iter = leftAccids.begin(); iter != leftAccids.end(); ++iter) {
                this->AdjustX(dynamic_cast<LayerElement *>(*iter), doc, staffSize, leftAccidsSubset, adjustedAccids);
            }
        }
    }
}

//----------------------------------------------------------------------------
// Static methods for Accid
//----------------------------------------------------------------------------

wchar_t Accid::GetAccidGlyph(data_ACCIDENTAL_WRITTEN accid)
{
    switch (accid) {
        case ACCIDENTAL_WRITTEN_s: return SMUFL_E262_accidentalSharp; break;
        case ACCIDENTAL_WRITTEN_f: return SMUFL_E260_accidentalFlat; break;
        case ACCIDENTAL_WRITTEN_ss: return SMUFL_E269_accidentalSharpSharp; break;
        case ACCIDENTAL_WRITTEN_x: return SMUFL_E263_accidentalDoubleSharp; break;
        case ACCIDENTAL_WRITTEN_ff: return SMUFL_E264_accidentalDoubleFlat; break;
        case ACCIDENTAL_WRITTEN_sx: return SMUFL_E265_accidentalTripleSharp; break; // Missing in SMuFL
        case ACCIDENTAL_WRITTEN_xs: return SMUFL_E265_accidentalTripleSharp; break;
        case ACCIDENTAL_WRITTEN_ts: return SMUFL_E265_accidentalTripleSharp; break; // Missing in SMuFL
        case ACCIDENTAL_WRITTEN_tf: return SMUFL_E266_accidentalTripleFlat; break;
        case ACCIDENTAL_WRITTEN_n: return SMUFL_E261_accidentalNatural; break;
        case ACCIDENTAL_WRITTEN_nf: return SMUFL_E267_accidentalNaturalFlat; break;
        case ACCIDENTAL_WRITTEN_ns: return SMUFL_E268_accidentalNaturalSharp; break;
        case ACCIDENTAL_WRITTEN_su: return SMUFL_E274_accidentalThreeQuarterTonesSharpArrowUp; break;
        case ACCIDENTAL_WRITTEN_sd: return SMUFL_E275_accidentalQuarterToneSharpArrowDown; break;
        case ACCIDENTAL_WRITTEN_fu: return SMUFL_E270_accidentalQuarterToneFlatArrowUp; break;
        case ACCIDENTAL_WRITTEN_fd: return SMUFL_E271_accidentalThreeQuarterTonesFlatArrowDown; break;
        case ACCIDENTAL_WRITTEN_nu: return SMUFL_E272_accidentalQuarterToneSharpNaturalArrowUp; break;
        case ACCIDENTAL_WRITTEN_nd: return SMUFL_E273_accidentalQuarterToneFlatNaturalArrowDown; break;
        case ACCIDENTAL_WRITTEN_1qf: return SMUFL_E280_accidentalQuarterToneFlatStein; break;
        case ACCIDENTAL_WRITTEN_3qf: return SMUFL_E281_accidentalThreeQuarterTonesFlatZimmermann; break;
        case ACCIDENTAL_WRITTEN_1qs: return SMUFL_E282_accidentalQuarterToneSharpStein; break;
        case ACCIDENTAL_WRITTEN_3qs: return SMUFL_E283_accidentalThreeQuarterTonesSharpStein; break;
        case ACCIDENTAL_WRITTEN_bms: return SMUFL_E447_accidentalBuyukMucennebSharp; break;
        case ACCIDENTAL_WRITTEN_kms: return SMUFL_E446_accidentalKucukMucennebSharp; break;
        case ACCIDENTAL_WRITTEN_bs: return SMUFL_E445_accidentalBakiyeSharp; break;
        case ACCIDENTAL_WRITTEN_ks: return SMUFL_E444_accidentalKomaSharp; break;
        case ACCIDENTAL_WRITTEN_kf: return SMUFL_E443_accidentalKomaFlat; break;
        case ACCIDENTAL_WRITTEN_bf: return SMUFL_E442_accidentalBakiyeFlat; break;
        case ACCIDENTAL_WRITTEN_kmf: return SMUFL_E441_accidentalKucukMucennebFlat; break;
        case ACCIDENTAL_WRITTEN_bmf: return SMUFL_E440_accidentalBuyukMucennebFlat; break;
        default: break;
    }
    return 0;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

int Accid::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetDrawing(functorParams);
    PositionInterface::InterfaceResetDrawing(functorParams, this);

    return FUNCTOR_CONTINUE;
}

int Accid::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    LayerElement::ResetHorizontalAlignment(functorParams);
    PositionInterface::InterfaceResetHorizontalAlignment(functorParams, this);

    m_isDrawingOctave = false;
    m_drawingOctave = NULL;
    m_drawingUnison = NULL;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

/////////////////////////////////////////////////////////////////////////////
// Name:        accid.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "accid.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "doc.h"
#include "functor.h"
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
    : LayerElement(ACCID)
    , PositionInterface()
    , AttAccidental()
    , AttAccidentalGes()
    , AttAccidLog()
    , AttColor()
    , AttEnclosingChars()
    , AttExtSymAuth()
    , AttExtSymNames()
    , AttPlacementOnStaff()
    , AttPlacementRelEvent()
{

    this->RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    this->RegisterAttClass(ATT_ACCIDENTAL);
    this->RegisterAttClass(ATT_ACCIDENTALGES);
    this->RegisterAttClass(ATT_ACCIDLOG);
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_ENCLOSINGCHARS);
    this->RegisterAttClass(ATT_EXTSYMAUTH);
    this->RegisterAttClass(ATT_EXTSYMNAMES);
    this->RegisterAttClass(ATT_PLACEMENTONSTAFF);
    this->RegisterAttClass(ATT_PLACEMENTRELEVENT);

    this->Reset();
}

Accid::~Accid() {}

void Accid::Reset()
{
    LayerElement::Reset();
    PositionInterface::Reset();
    this->ResetAccidental();
    this->ResetAccidentalGes();
    this->ResetAccidLog();
    this->ResetColor();
    this->ResetEnclosingChars();
    this->ResetExtSymAuth();
    this->ResetExtSymNames();
    this->ResetPlacementOnStaff();
    this->ResetPlacementRelEvent();

    m_drawingUnison = NULL;
    m_alignedWithSameLayer = false;
}

std::u32string Accid::GetSymbolStr(data_NOTATIONTYPE notationType) const
{
    return Accid::CreateSymbolStr(this->GetAccid(), this->GetEnclose(), notationType, this->GetDocResources(),
        this->GetGlyphNum(), this->GetGlyphName());
}

void Accid::AdjustToLedgerLines(const Doc *doc, LayerElement *element, int staffSize)
{
    Staff *staff = element->GetAncestorStaff(RESOLVE_CROSS_STAFF);
    Chord *chord = vrv_cast<Chord *>(this->GetFirstAncestor(CHORD));

    const int unit = doc->GetDrawingUnit(staffSize);
    const int rightMargin = doc->GetRightMargin(ACCID) * unit;
    if (element->Is(NOTE) && chord && chord->HasAdjacentNotesInStaff(staff)) {
        const int horizontalMargin = doc->GetOptions()->m_ledgerLineExtension.GetValue() * unit + 0.5 * rightMargin;
        const int staffTop = staff->GetDrawingY();
        const int staffBottom = staffTop - doc->GetDrawingStaffSize(staffSize);
        if (this->HorizontalContentOverlap(element, 0)) {
            if (((this->GetContentTop() > staffTop + 2 * unit) && (this->GetDrawingY() < element->GetDrawingY()))
                || ((this->GetContentBottom() < staffBottom - 2 * unit)
                    && (this->GetDrawingY() > element->GetDrawingY()))) {
                int right = this->GetSelfRight();
                // Special case: Reduce shift for flats intersecting only the first ledger line above the staff
                if ((this->GetAccid() == ACCIDENTAL_WRITTEN_f) || (this->GetAccid() == ACCIDENTAL_WRITTEN_ff)) {
                    if ((this->GetContentTop() > staffTop + 2 * unit)
                        && (this->GetContentTop() < staffTop + 4 * unit)) {
                        right = this->GetCutOutRight(doc->GetResources(), true);
                    }
                }
                const int xRelShift = right - element->GetSelfLeft() + horizontalMargin;
                if (xRelShift > 0) this->SetDrawingXRel(this->GetDrawingXRel() - xRelShift);
            }
        }
    }
}

void Accid::AdjustX(LayerElement *element, const Doc *doc, int staffSize, std::vector<Accid *> &leftAccids,
    std::set<Accid *> &adjustedAccids)
{
    assert(element);
    assert(doc);

    if (this == element) return;

    const int unit = doc->GetDrawingUnit(staffSize);
    int horizontalMargin = doc->GetRightMargin(ACCID) * unit;
    // Reduce spacing for successive accidentals
    if (element->Is(ACCID)) {
        horizontalMargin *= 0.66;
    }
    else if (element->Is(NOTE)) {
        Note *note = vrv_cast<Note *>(element);
        int ledgerAbove = 0;
        int ledgerBelow = 0;
        Staff *staff = note->GetAncestorStaff(RESOLVE_CROSS_STAFF);
        if (note->HasLedgerLines(ledgerAbove, ledgerBelow, staff)) {
            const int value = doc->GetOptions()->m_ledgerLineExtension.GetValue() * unit + 0.5 * horizontalMargin;
            horizontalMargin = std::max(horizontalMargin, value);
        }
    }
    const int verticalMargin = unit / 4;

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
        if (!adjustedAccids.contains(accid)) return;
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
        adjustedAccids.insert(this);
        // We have some accidentals on the left, check again with all of these
        if (!leftAccids.empty()) {
            std::vector<Accid *> leftAccidsSubset;
            // Recursively adjust all accidental that are on the left because enough space was previously available
            for (Accid *accid : leftAccids) {
                this->AdjustX(accid, doc, staffSize, leftAccidsSubset, adjustedAccids);
            }
        }
    }
}

//----------------------------------------------------------------------------
// Static methods for Accid
//----------------------------------------------------------------------------

char32_t Accid::GetAccidGlyph(data_ACCIDENTAL_WRITTEN accid)
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
        case ACCIDENTAL_WRITTEN_koron: return SMUFL_E460_accidentalKoron; break;
        case ACCIDENTAL_WRITTEN_sori: return SMUFL_E461_accidentalSori; break;
        default: break;
    }
    return 0;
}

std::u32string Accid::CreateSymbolStr(data_ACCIDENTAL_WRITTEN accid, data_ENCLOSURE enclosure,
    data_NOTATIONTYPE notationType, const Resources *resources, data_HEXNUM glyphNum, std::string glyphName)
{
    char32_t code = 0;

    if (resources) {
        // If there is glyph.num, prioritize it
        if (glyphNum != 0) {
            code = glyphNum;
            if (NULL == resources->GetGlyph(code)) code = 0;
        }
        // If there is glyph.name (second priority)
        else if (!glyphName.empty()) {
            code = resources->GetGlyphCode(glyphName);
            if (NULL == resources->GetGlyph(code)) code = 0;
        }
    }

    if (!code) {
        if (accid == ACCIDENTAL_WRITTEN_NONE) return U"";

        switch (notationType) {
            case NOTATIONTYPE_mensural:
            case NOTATIONTYPE_mensural_black:
            case NOTATIONTYPE_mensural_white:
                switch (accid) {
                    case ACCIDENTAL_WRITTEN_s: code = SMUFL_E9E3_medRenSharpCroix; break;
                    case ACCIDENTAL_WRITTEN_f: code = SMUFL_E9E0_medRenFlatSoftB; break;
                    case ACCIDENTAL_WRITTEN_n: code = SMUFL_E9E2_medRenNatural; break;
                    // we do not want to ignore non-mensural accidentals
                    default: code = Accid::GetAccidGlyph(accid); break;
                }
                break;
            default: code = Accid::GetAccidGlyph(accid); break;
        }
    }

    std::u32string symbolStr;
    switch (enclosure) {
        case ENCLOSURE_brack:
            symbolStr.push_back(SMUFL_E26C_accidentalBracketLeft);
            symbolStr.push_back(code);
            symbolStr.push_back(SMUFL_E26D_accidentalBracketRight);
            break;
        case ENCLOSURE_paren:
            symbolStr.push_back(SMUFL_E26A_accidentalParensLeft);
            symbolStr.push_back(code);
            symbolStr.push_back(SMUFL_E26B_accidentalParensRight);
            break;
        default: symbolStr.push_back(code); break;
    }
    return symbolStr;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

FunctorCode Accid::Accept(Functor &functor)
{
    return functor.VisitAccid(this);
}

FunctorCode Accid::Accept(ConstFunctor &functor) const
{
    return functor.VisitAccid(this);
}

FunctorCode Accid::AcceptEnd(Functor &functor)
{
    return functor.VisitAccidEnd(this);
}

FunctorCode Accid::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitAccidEnd(this);
}

//----------------------------------------------------------------------------
// AccidOctaveSort
//----------------------------------------------------------------------------

std::string AccidOctaveSort::GetOctaveID(const Accid *accid) const
{
    const Note *note = vrv_cast<const Note *>(accid->GetFirstAncestor(NOTE));
    assert(note);
    const Chord *chord = note->IsChordTone();

    std::string octaveID = chord ? chord->GetID() : note->GetID();
    octaveID += "-" + std::to_string(accid->GetAccid());
    octaveID += "-" + std::to_string(note->GetPname());

    return octaveID;
}

} // namespace vrv

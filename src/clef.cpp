/////////////////////////////////////////////////////////////////////////////
// Name:        clef.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "clef.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "doc.h"
#include "functorparams.h"
#include "layer.h"
#include "scoredefinterface.h"
#include "smufl.h"
#include "staff.h"

namespace vrv {

//----------------------------------------------------------------------------
// Clef
//----------------------------------------------------------------------------

static const ClassRegistrar<Clef> s_factory("clef", CLEF);

Clef::Clef()
    : LayerElement(CLEF, "clef-")
    , AttClefShape()
    , AttColor()
    , AttLineLoc()
    , AttOctaveDisplacement()
    , AttStaffIdent()
    , AttVisibility()
{
    this->RegisterAttClass(ATT_CLEFSHAPE);
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_ENCLOSINGCHARS);
    this->RegisterAttClass(ATT_EXTSYM);
    this->RegisterAttClass(ATT_LINELOC);
    this->RegisterAttClass(ATT_OCTAVEDISPLACEMENT);
    this->RegisterAttClass(ATT_OCTAVEDISPLACEMENT);
    this->RegisterAttClass(ATT_VISIBILITY);

    this->Reset();
}

Clef::~Clef() {}

void Clef::Reset()
{
    LayerElement::Reset();
    this->ResetClefShape();
    this->ResetColor();
    this->ResetEnclosingChars();
    this->ResetExtSym();
    this->ResetLineLoc();
    this->ResetOctaveDisplacement();
    this->ResetStaffIdent();
    this->ResetVisibility();
}

int Clef::GetClefLocOffset() const
{
    // Only resolve simple sameas links to avoid infinite recursion
    const Clef *sameas = dynamic_cast<const Clef *>(this->GetSameasLink());
    if (sameas && !sameas->HasSameasLink()) {
        return sameas->GetClefLocOffset();
    }

    int offset = 0;
    if (this->GetShape() == CLEFSHAPE_G) {
        offset = -4;
    }
    else if (this->GetShape() == CLEFSHAPE_GG) {
        offset = 3;
    }
    else if (this->GetShape() == CLEFSHAPE_F) {
        offset = 4;
    }

    offset += (this->GetLine() - 1) * 2;

    int disPlace = 0;
    if (this->GetDisPlace() == STAFFREL_basic_above)
        disPlace = -1;
    else if (this->GetDisPlace() == STAFFREL_basic_below)
        disPlace = 1;

    // ignore disPlace for gClef8vbOld
    if (this->GetShape() == CLEFSHAPE_GG) disPlace = 0;

    if ((disPlace != 0) && (this->GetDis() != OCTAVE_DIS_NONE)) offset += (disPlace * (this->GetDis() - 1));

    return offset;
}

//----------------------------------------------------------------------------
// Static methods for Clef
//----------------------------------------------------------------------------

char32_t Clef::GetClefGlyph(const data_NOTATIONTYPE notationtype) const
{
    const Resources *resources = this->GetDocResources();
    const bool clefChange = (this->GetAlignment() && (this->GetAlignment()->GetType() == ALIGNMENT_CLEF));
    if (!resources) return 0;

    // If there is glyph.num, prioritize it
    if (this->HasGlyphNum()) {
        char32_t code = this->GetGlyphNum();
        if (NULL != resources->GetGlyph(code)) return code;
    }
    // If there is glyph.name (second priority)
    else if (this->HasGlyphName()) {
        char32_t code = resources->GetGlyphCode(this->GetGlyphName());
        if (NULL != resources->GetGlyph(code)) return code;
    }

    switch (notationtype) {
        case NOTATIONTYPE_tab:
        case NOTATIONTYPE_tab_guitar: return SMUFL_E06D_6stringTabClef; break;
        case NOTATIONTYPE_neume:
            // neume clefs
            return (this->GetShape() == CLEFSHAPE_F) ? SMUFL_E902_chantFclef : SMUFL_E906_chantCclef;
            break;
        case NOTATIONTYPE_mensural:
        case NOTATIONTYPE_mensural_white:
            // mensural clefs
            switch (this->GetShape()) {
                case CLEFSHAPE_G: return SMUFL_E901_mensuralGclefPetrucci; break;
                case CLEFSHAPE_F: return SMUFL_E904_mensuralFclefPetrucci; break;
                case CLEFSHAPE_C:
                    switch (this->GetLine()) {
                        case 1: return SMUFL_E907_mensuralCclefPetrucciPosLowest; break;
                        case 2: return SMUFL_E908_mensuralCclefPetrucciPosLow; break;
                        case 3: return SMUFL_E909_mensuralCclefPetrucciPosMiddle; break;
                        case 4: return SMUFL_E90A_mensuralCclefPetrucciPosHigh; break;
                        case 5: return SMUFL_E90B_mensuralCclefPetrucciPosHighest; break;
                    }
                    [[fallthrough]];
                default: return SMUFL_E909_mensuralCclefPetrucciPosMiddle; break;
            }
        case NOTATIONTYPE_mensural_black:
            switch (this->GetShape()) {
                case CLEFSHAPE_C: return SMUFL_E906_chantCclef; break;
                case CLEFSHAPE_F: return SMUFL_E902_chantFclef; break;
                default:
                    // G clef doesn't exist in black notation, so should never get here, but just in case.
                    if (!this->GetDis()) return SMUFL_E901_mensuralGclefPetrucci;
            }
            [[fallthrough]];
        default:
            // cmn clefs
            switch (this->GetShape()) {
                case CLEFSHAPE_G:
                    switch (this->GetDis()) {
                        case OCTAVE_DIS_8:
                            return (this->GetDisPlace() == STAFFREL_basic_above) ? SMUFL_E053_gClef8va
                                                                                 : SMUFL_E052_gClef8vb;
                            break;
                        case OCTAVE_DIS_15:
                            return (this->GetDisPlace() == STAFFREL_basic_above) ? SMUFL_E053_gClef8va
                                                                                 : SMUFL_E051_gClef15mb;
                            break;
                        default: return (clefChange) ? SMUFL_E07A_gClefChange : SMUFL_E050_gClef; break;
                    }
                case CLEFSHAPE_GG: return SMUFL_E055_gClef8vbOld;
                case CLEFSHAPE_F:
                    switch (this->GetDis()) {
                        case OCTAVE_DIS_8:
                            return (this->GetDisPlace() == STAFFREL_basic_above) ? SMUFL_E065_fClef8va
                                                                                 : SMUFL_E064_fClef8vb;
                            break;
                        case OCTAVE_DIS_15:
                            return (this->GetDisPlace() == STAFFREL_basic_above) ? SMUFL_E066_fClef15ma
                                                                                 : SMUFL_E063_fClef15mb;
                            break;
                        default: return (clefChange) ? SMUFL_E07C_fClefChange : SMUFL_E062_fClef; break;
                    }
                case CLEFSHAPE_C:
                    switch (this->GetDis()) {
                        case OCTAVE_DIS_8: return SMUFL_E05D_cClef8vb; break;
                        default: return (clefChange) ? SMUFL_E07B_cClefChange : SMUFL_E05C_cClef; break;
                    }
                case CLEFSHAPE_perc: return SMUFL_E069_unpitchedPercussionClef1;
                default: break;
            }
    }

    return 0;
}

//----------------------------------------------------------------------------
// Clef functors methods
//----------------------------------------------------------------------------

int Clef::AdjustBeams(FunctorParams *functorParams)
{
    AdjustBeamParams *params = vrv_params_cast<AdjustBeamParams *>(functorParams);
    assert(params);
    if (!params->m_beam) return FUNCTOR_SIBLINGS;
    // ignore elements that start before/after the beam
    if (this->GetDrawingX() < params->m_x1) return FUNCTOR_CONTINUE;
    const int beamRight = params->m_x1 + (params->m_y2 - params->m_y1) / params->m_beamSlope;
    if (this->GetDrawingX() > beamRight) return FUNCTOR_CONTINUE;

    Staff *staff = this->GetAncestorStaff();
    // find number of beams at current position
    const int beams = vrv_cast<Beam *>(params->m_beam)->GetBeamPartDuration(this) - DUR_4;
    const int beamWidth = vrv_cast<Beam *>(params->m_beam)->m_beamWidth;
    // find beam Y positions that are relevant to current clef
    const int currentBeamYLeft = params->m_y1 + params->m_beamSlope * (this->GetContentLeft() - params->m_x1);
    const int currentBeamYRight = params->m_y1 + params->m_beamSlope * (this->GetContentRight() - params->m_x1);
    // get clef code and find its bounds on the staff (anchor point and top/bottom depending on the beam place)
    const char32_t clefCode = this->GetClefGlyph(staff->m_drawingNotationType);
    if (!clefCode) return FUNCTOR_SIBLINGS;

    const int clefPosition = staff->GetDrawingY()
        - params->m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * (staff->m_drawingLines - this->GetLine());
    const int clefBounds = clefPosition
        + ((params->m_directionBias > 0) ? params->m_doc->GetGlyphTop(clefCode, staff->m_drawingStaffSize, false)
                                         : params->m_doc->GetGlyphBottom(clefCode, staff->m_drawingStaffSize, false));
    // calculate margins for the clef
    const int leftMargin = params->m_directionBias * (currentBeamYLeft - clefBounds) - beams * beamWidth;
    const int rightMargin = params->m_directionBias * (currentBeamYRight - clefBounds) - beams * beamWidth;
    const int overlapMargin = std::min(leftMargin, rightMargin);
    if (overlapMargin >= 0) return FUNCTOR_CONTINUE;
    // calculate offset required for the beam
    const int unit = params->m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    const int unitChangeNumber = ((std::abs(overlapMargin) + unit / 6) / unit);
    if (unitChangeNumber > 0) {
        const int adjust = unitChangeNumber * unit * params->m_directionBias;
        if (std::abs(adjust) > std::abs(params->m_overlapMargin)) params->m_overlapMargin = adjust;
    }

    return FUNCTOR_CONTINUE;
}

int Clef::AdjustClefChanges(FunctorParams *functorParams)
{
    AdjustClefsParams *params = vrv_params_cast<AdjustClefsParams *>(functorParams);
    assert(params);

    if (this->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    assert(this->GetAlignment());
    if (this->GetAlignment()->GetType() != ALIGNMENT_CLEF) return FUNCTOR_CONTINUE;

    if (!this->HasContentBB()) return FUNCTOR_CONTINUE;

    assert(params->m_aligner);

    Staff *staff = this->GetAncestorStaff();

    // Create ad comparison object for each type / @n
    std::vector<int> ns;
    // -1 for barline attributes that need to be taken into account each time
    ns.push_back(BARLINE_REFERENCES);
    ns.push_back(m_crossStaff ? m_crossStaff->GetN() : staff->GetN());
    AttNIntegerAnyComparison matchStaff(ALIGNMENT_REFERENCE, ns);

    // Look if we have a grace aligner just after the clef.
    // Limitation: clef changes are always aligned before grace notes, even if appearing after in the encoding.
    // To overcome this limitation we will need to rethink alignment, or (better) use <graceGrp> and have the
    // <clef> within it at the right place. Then the Clef should use the grace aligner and not the measure aligner.
    GraceAligner *graceAligner = NULL;
    Alignment *nextAlignment = vrv_cast<Alignment *>(params->m_aligner->GetNext(this->GetAlignment()));
    if (nextAlignment && nextAlignment->GetType() == ALIGNMENT_GRACENOTE) {
        // If we have one, then check if we have one for our staff (or all staves with --grace-rhythm-align
        int graceAlignerId = params->m_doc->GetOptions()->m_graceRhythmAlign.GetValue() ? 0 : staff->GetN();
        if (nextAlignment->HasGraceAligner(graceAlignerId)) {
            graceAligner = nextAlignment->GetGraceAligner(graceAlignerId);
        }
    }

    // No grace aligner, look for the next alignment with something on that staff
    if (!graceAligner) {
        nextAlignment = NULL;
        // Look for the next reference - here we start with the next alignment, because otherwise it will find the
        // reference to the Clef in its own children
        Object *next = params->m_aligner->FindNextChild(&matchStaff, params->m_aligner->GetNext(this->GetAlignment()));
        if (next) {
            nextAlignment = vrv_cast<Alignment *>(next->GetParent());
            assert(nextAlignment);
        }
    }

    Alignment *previousAlignment = NULL;
    // Look for the previous reference on this staff (or a barline)
    Object *previous = params->m_aligner->FindPreviousChild(&matchStaff, this->GetAlignment());
    if (previous) {
        // We looked for the first alignment reference - we actually need the parent alignment
        previousAlignment = vrv_cast<Alignment *>(previous->GetParent());
        assert(previousAlignment);
    }

    // This should never happen because we always have at least barline alignments - even empty
    if (!previousAlignment || !nextAlignment) {
        LogDebug("No alignment found before and after the clef change");
        return FUNCTOR_CONTINUE;
    }

    // LayerElement::AdjustXPos can have spread the alignment apart.
    // We want them to point to the same position. Otherwise, adjusting proportionally
    // (below) will yield displacements.
    this->GetAlignment()->SetXRel(nextAlignment->GetXRel());

    int previousLeft, previousRight;
    previousAlignment->GetLeftRight(ns, previousLeft, previousRight);
    // This typically happens with invisible barlines. Just take the position of the alignment
    if (previousRight == VRV_UNSET) previousRight = previousAlignment->GetXRel();

    // Get the right position of the grace group or of the next element
    int nextLeft, nextRight;
    if (graceAligner) {
        nextLeft = graceAligner->GetGraceGroupLeft(staff->GetN());
    }
    else {
        nextAlignment->GetLeftRight(ns, nextLeft, nextRight);
    }
    // This typically happens with invisible barlines or with --grace-rhythm-align but no grace on that staff
    if (nextLeft == -VRV_UNSET) nextLeft = nextAlignment->GetXRel();

    const int unit = params->m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    const int selfRight = this->GetContentRight() + params->m_doc->GetRightMargin(this) * unit;
    // First move it to the left if necessary
    if (selfRight > nextLeft) {
        this->SetDrawingXRel(this->GetDrawingXRel() - selfRight + nextLeft);
    }
    // Then look if it overlaps on the right and make room if necessary
    const int selfLeft = this->GetContentLeft() - params->m_doc->GetLeftMargin(this) * unit;
    if (selfLeft < previousRight) {
        ArrayOfAdjustmentTuples boundaries{ std::make_tuple(
            previousAlignment, this->GetAlignment(), previousRight - selfLeft) };
        params->m_aligner->AdjustProportionally(boundaries);
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

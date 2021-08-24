/////////////////////////////////////////////////////////////////////////////
// Name:        clef.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "clef.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "doc.h"
#include "functorparams.h"
#include "scoredefinterface.h"
#include "staff.h"

namespace vrv {

//----------------------------------------------------------------------------
// Clef
//----------------------------------------------------------------------------

static const ClassRegistrar<Clef> s_factory("clef", CLEF);

Clef::Clef()
    : LayerElement(CLEF, "clef-"), AttClefShape(), AttColor(), AttLineLoc(), AttOctaveDisplacement(), AttVisibility()
{
    RegisterAttClass(ATT_CLEFSHAPE);
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_EXTSYM);
    RegisterAttClass(ATT_LINELOC);
    RegisterAttClass(ATT_OCTAVEDISPLACEMENT);
    RegisterAttClass(ATT_VISIBILITY);

    Reset();
}

Clef::~Clef() {}

void Clef::Reset()
{
    LayerElement::Reset();
    ResetClefShape();
    ResetColor();
    ResetExtSym();
    ResetLineLoc();
    ResetOctaveDisplacement();
    ResetVisibility();
}

int Clef::GetClefLocOffset() const
{
    if (this->HasSameasLink() && this->GetSameasLink()->Is(CLEF)) {
        Clef *sameas = vrv_cast<Clef *>(this->GetSameasLink());
        assert(sameas);
        return sameas->GetClefLocOffset();
    }

    int offset = 0;
    if (GetShape() == CLEFSHAPE_G) {
        offset = -4;
    }
    else if (GetShape() == CLEFSHAPE_GG) {
        offset = 3;
    }
    else if (GetShape() == CLEFSHAPE_F) {
        offset = 4;
    }

    offset += (GetLine() - 1) * 2;

    int disPlace = 0;
    if (GetDisPlace() == STAFFREL_basic_above)
        disPlace = -1;
    else if (GetDisPlace() == STAFFREL_basic_below)
        disPlace = 1;

    // ignore disPlace for gClef8vbOld
    if (GetShape() == CLEFSHAPE_GG) disPlace = 0;

    if ((disPlace != 0) && (GetDis() != OCTAVE_DIS_NONE)) offset += (disPlace * (GetDis() - 1));

    return offset;
}

//----------------------------------------------------------------------------
// Static methods for Clef
//----------------------------------------------------------------------------

wchar_t Clef::GetClefGlyph(data_NOTATIONTYPE notationtype) const
{
    // If there is glyph.num, prioritize it
    if (HasGlyphNum()) {
        wchar_t code = GetGlyphNum();
        if (NULL != Resources::GetGlyph(code)) return code;
    }
    // If there is glyph.name (second priority)
    else if (HasGlyphName()) {
        wchar_t code = Resources::GetGlyphCode(GetGlyphName());
        if (NULL != Resources::GetGlyph(code)) return code;
    }

    switch (notationtype) {
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
                        default: return SMUFL_E050_gClef; break;
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
                        default: return SMUFL_E062_fClef; break;
                    }
                case CLEFSHAPE_C:
                    switch (this->GetDis()) {
                        case OCTAVE_DIS_8: return SMUFL_E05D_cClef8vb; break;
                        default: return SMUFL_E05C_cClef; break;
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
    const std::map<data_CLEFSHAPE, std::pair<wchar_t, double>> topToMiddleProportions
        = { { CLEFSHAPE_G, { SMUFL_E050_gClef, 0.6 } }, { CLEFSHAPE_C, { SMUFL_E05C_cClef, 0.5 } },
              { CLEFSHAPE_F, { SMUFL_E062_fClef, 0.35 } } };

    AdjustBeamParams *params = vrv_params_cast<AdjustBeamParams *>(functorParams);
    assert(params);
    if (!params->m_beam) return FUNCTOR_SIBLINGS;

    Staff *staff = vrv_cast<Staff *>(GetFirstAncestor(STAFF));
    assert(staff);

    auto currentShapeIter = topToMiddleProportions.find(GetShape());
    if (currentShapeIter == topToMiddleProportions.end()) return FUNCTOR_CONTINUE;

    // const int directionBias = (vrv_cast<Beam *>(params->m_beam)->m_drawingPlace == BEAMPLACE_above) ? 1 : -1;
    const int proportion = (params->m_directionBias > 0) ? 0 : -1;

    // Y position differs for clef shapes, so we need to take into account only a part of the glyph height
    // Proportion of the glyph about Y point is defined in the topToMiddleProportions map and used when
    // left and right margins are calculated
    const int clefPosition = staff->GetDrawingY()
        - params->m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * (staff->m_drawingLines - GetLine());
    const int clefGlyphHeight
        = params->m_doc->GetGlyphHeight(currentShapeIter->second.first, staff->m_drawingStaffSize, true);
    const int beamWidth = params->m_doc->GetDrawingBeamWidth(staff->m_drawingStaffSize, false);
    const int leftMargin = (clefPosition + clefGlyphHeight * (proportion + currentShapeIter->second.second)
                               + (params->m_directionBias * beamWidth) - params->m_y1)
        * params->m_directionBias;
    const int rightMargin = (clefPosition + clefGlyphHeight * (proportion + currentShapeIter->second.second)
                                + (params->m_directionBias * beamWidth) - params->m_y2)
        * params->m_directionBias;

    // If both sides of beam overlap with Clef, we need to get smaller margin, i.e. the one that would make one side not
    // overlap anymore. For sloped beams this would generally mean that slope will avoid collision as well, for
    // non-sloped ones it doesn't matter since both ends are at the same Y position
    const bool bothSidesOverlap = ((leftMargin > params->m_overlapMargin) && (rightMargin > params->m_overlapMargin));
    const int overlapMargin = bothSidesOverlap ? std::min(leftMargin, rightMargin) : std::max(leftMargin, rightMargin);
    if ((overlapMargin >= params->m_overlapMargin)
        && ((overlapMargin >= beamWidth / 2) || (leftMargin == rightMargin))) {
        const int staffOffset = params->m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        params->m_overlapMargin = (overlapMargin / staffOffset + (leftMargin == rightMargin ? 1 : 2)) * staffOffset
            * params->m_directionBias;
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

    assert(params->m_aligner);

    Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff);

    // Create ad comparison object for each type / @n
    std::vector<int> ns;
    // -1 for barline attributes that need to be taken into account each time
    ns.push_back(BARLINE_REFERENCES);
    ns.push_back(staff->GetN());
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
        LogDebug("No aligment found before and after the clef change");
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

    int selfRight = this->GetContentRight() + params->m_doc->GetRightMargin(CLEF) * staff->m_drawingStaffSize;
    // First move it to the left if necessary
    if (selfRight > nextLeft) {
        this->SetDrawingXRel(this->GetDrawingXRel() - selfRight + nextLeft);
    }
    // Then look if it overlaps on the right and make room if necessary
    int selfLeft = this->GetContentLeft() - params->m_doc->GetLeftMargin(CLEF) * staff->m_drawingStaffSize;
    if (selfLeft < previousRight) {
        ArrayOfAdjustmentTuples boundaries{ std::make_tuple(
            previousAlignment, this->GetAlignment(), previousRight - selfLeft) };
        params->m_aligner->AdjustProportionally(boundaries);
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

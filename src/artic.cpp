/////////////////////////////////////////////////////////////////////////////
// Name:        artic.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "artic.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "doc.h"
#include "elementpart.h"
#include "floatingobject.h"
#include "functorparams.h"
#include "layer.h"
#include "smufl.h"
#include "staff.h"
#include "stem.h"
#include "vrv.h"

namespace vrv {

const std::vector<data_ARTICULATION> Artic::s_outStaffArtic
    = { ARTICULATION_acc, ARTICULATION_dnbow, ARTICULATION_marc, ARTICULATION_upbow, ARTICULATION_harm,
          ARTICULATION_snap, ARTICULATION_damp, ARTICULATION_lhpizz, ARTICULATION_open, ARTICULATION_stop };

const std::vector<data_ARTICULATION> Artic::s_aboveStaffArtic
    = { ARTICULATION_dnbow, ARTICULATION_marc, ARTICULATION_upbow, ARTICULATION_harm, ARTICULATION_snap,
          ARTICULATION_damp, ARTICULATION_lhpizz, ARTICULATION_open, ARTICULATION_stop };

//----------------------------------------------------------------------------
// Artic
//----------------------------------------------------------------------------

static const ClassRegistrar<Artic> s_factory("artic", ARTIC);

Artic::Artic()
    : LayerElement(ARTIC, "artic-")
    , AttArticulation()
    , AttArticulationGestural()
    , AttColor()
    , AttEnclosingChars()
    , AttExtSym()
    , AttPlacementRelEvent()
{
    this->RegisterAttClass(ATT_ARTICULATION);
    this->RegisterAttClass(ATT_ARTICULATIONGESTURAL);
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_ENCLOSINGCHARS);
    this->RegisterAttClass(ATT_EXTSYM);
    this->RegisterAttClass(ATT_PLACEMENTRELEVENT);

    this->Reset();
}

Artic::~Artic() {}

void Artic::Reset()
{
    LayerElement::Reset();
    this->ResetArticulation();
    this->ResetArticulationGestural();
    this->ResetColor();
    this->ResetEnclosingChars();
    this->ResetExtSym();
    this->ResetPlacementRelEvent();

    m_drawingPlace = STAFFREL_NONE;
}

bool Artic::IsInsideArtic(data_ARTICULATION artic) const
{
    // Always outside if enclosing brackets are used
    if ((this->GetEnclose() == ENCLOSURE_brack) || (this->GetEnclose() == ENCLOSURE_paren)) {
        return false;
    }

    const auto end = Artic::s_outStaffArtic.end();
    const auto it = std::find(Artic::s_outStaffArtic.begin(), end, artic);
    return (it == end);
}

bool Artic::IsInsideArtic() const
{
    return IsInsideArtic(this->GetArticFirst());
}

data_ARTICULATION Artic::GetArticFirst() const
{
    std::vector<data_ARTICULATION> articList = this->GetArtic();
    if (articList.empty()) return ARTICULATION_NONE;

    return articList.front();
}

void Artic::SplitMultival(Object *parent)
{
    assert(parent);

    std::vector<data_ARTICULATION> articList = this->GetArtic();
    if (articList.empty()) return;

    int idx = this->GetIdx() + 1;
    std::vector<data_ARTICULATION>::iterator iter;
    for (iter = articList.begin() + 1; iter != articList.end(); ++iter) {
        Artic *artic = new Artic();
        artic->SetArtic({ *iter });
        artic->AttColor::operator=(*this);
        artic->AttEnclosingChars::operator=(*this);
        artic->AttExtSym::operator=(*this);
        artic->AttPlacementRelEvent::operator=(*this);
        artic->SetParent(parent);
        parent->InsertChild(artic, idx);
        idx++;
    }

    // The original element only keep the first value
    this->SetArtic({ articList.at(0) });

    // Multiple valued attributes cannot be preserved as such
    if (this->IsAttribute()) {
        this->IsAttribute(false);
        LogMessage("Multiple valued attribute @artic on '%s' permanently converted to <artic> elements",
            parent->GetID().c_str());
    }
}

void Artic::GetAllArtics(bool direction, std::vector<Artic *> &artics)
{
    Object *parentNoteOrChord = this->GetFirstAncestor(CHORD);

    if (!parentNoteOrChord) parentNoteOrChord = this->GetFirstAncestor(NOTE);

    if (!parentNoteOrChord) return;

    Object *first = (direction == FORWARD) ? this : parentNoteOrChord->GetFirst();
    Object *last = (direction == BACKWARD) ? this : parentNoteOrChord->GetLast();
    ClassIdComparison matchType(ARTIC);
    ListOfObjects children;
    parentNoteOrChord->FindAllDescendantsBetween(&children, &matchType, first, last);
    for (auto &child : children) {
        if (child == this) continue;
        Artic *artic = vrv_cast<Artic *>(child);
        assert(artic);
        if (artic->GetDrawingPlace() == this->GetDrawingPlace()) artics.push_back(artic);
    }
}

bool Artic::AlwaysAbove() const
{
    auto end = Artic::s_aboveStaffArtic.end();
    auto i = std::find(Artic::s_aboveStaffArtic.begin(), end, this->GetArticFirst());
    return (i != end);
}

void Artic::AddSlurPositioner(FloatingCurvePositioner *positioner, bool start)
{
    if (start) {
        if (std::find(m_startSlurPositioners.begin(), m_startSlurPositioners.end(), positioner)
            == m_startSlurPositioners.end())
            m_startSlurPositioners.push_back(positioner);
    }
    else {
        if (std::find(m_endSlurPositioners.begin(), m_endSlurPositioners.end(), positioner)
            == m_endSlurPositioners.end())
            m_endSlurPositioners.push_back(positioner);
    }
}

char32_t Artic::GetArticGlyph(data_ARTICULATION artic, data_STAFFREL place) const
{
    const Resources *resources = this->GetDocResources();
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

    if (place == STAFFREL_above) {
        switch (artic) {
            case ARTICULATION_acc: return SMUFL_E4A0_articAccentAbove;
            case ARTICULATION_acc_soft: return SMUFL_ED40_articSoftAccentAbove;
            case ARTICULATION_stacc: return SMUFL_E4A2_articStaccatoAbove;
            case ARTICULATION_ten: return SMUFL_E4A4_articTenutoAbove;
            case ARTICULATION_stacciss: return SMUFL_E4A8_articStaccatissimoWedgeAbove;
            case ARTICULATION_marc: return SMUFL_E4AC_articMarcatoAbove;
            // case ARTICULATION_marc_stacc: return SMUFL_E4AE_articMarcatoStaccatoAbove;
            case ARTICULATION_spicc: return SMUFL_E4A6_articStaccatissimoAbove;
            // case ARTICULATION_doit;
            // case ARTICULATION_scoop;
            // case ARTICULATION_rip;
            // case ARTICULATION_plop;
            // case ARTICULATION_fall;
            // case ARTICULATION_longfall;
            // case ARTICULATION_bend;
            // case ARTICULATION_flip;
            // case ARTICULATION_smear;
            // case ARTICULATION_shake;
            case ARTICULATION_dnbow: return SMUFL_E610_stringsDownBow;
            case ARTICULATION_upbow: return SMUFL_E612_stringsUpBow;
            case ARTICULATION_harm: return SMUFL_E614_stringsHarmonic;
            case ARTICULATION_snap: return SMUFL_E631_pluckedSnapPizzicatoAbove;
            // case ARTICULATION_fingernail;
            // case ARTICULATION_ten_stacc: return SMUFL_E4B2_articTenutoStaccatoAbove;
            case ARTICULATION_damp: return SMUFL_E638_pluckedDamp;
            // case ARTICULATION_dampall;
            case ARTICULATION_open: return SMUFL_E5E7_brassMuteOpen;
            case ARTICULATION_stop: return SMUFL_E5E5_brassMuteClosed;
            // case ARTICULATION_dbltongue;
            // case ARTICULATION_trpltongue;
            // case ARTICULATION_heel;
            // case ARTICULATION_toe;
            // case ARTICULATION_tap;
            case ARTICULATION_lhpizz: return SMUFL_E633_pluckedLeftHandPizzicato;
            case ARTICULATION_dot: return SMUFL_E4A2_articStaccatoAbove;
            case ARTICULATION_stroke: return SMUFL_E4AA_articStaccatissimoStrokeAbove;
            default: return 0;
        }
    }
    else if (place == STAFFREL_below) {
        switch (artic) {
            case ARTICULATION_acc: return SMUFL_E4A1_articAccentBelow;
            case ARTICULATION_acc_soft: return SMUFL_ED41_articSoftAccentBelow;
            case ARTICULATION_stacc: return SMUFL_E4A3_articStaccatoBelow;
            case ARTICULATION_ten: return SMUFL_E4A5_articTenutoBelow;
            case ARTICULATION_stacciss: return SMUFL_E4A9_articStaccatissimoWedgeBelow;
            case ARTICULATION_marc: return SMUFL_E4AD_articMarcatoBelow;
            // Removed in MEI 4.0
            // case ARTICULATION_marc_stacc: return SMUFL_E4AF_articMarcatoStaccatoBelow;
            case ARTICULATION_spicc: return SMUFL_E4A7_articStaccatissimoBelow;
            //
            case ARTICULATION_dnbow: return SMUFL_E611_stringsDownBowTurned;
            case ARTICULATION_upbow: return SMUFL_E613_stringsUpBowTurned;
            case ARTICULATION_harm: return SMUFL_E614_stringsHarmonic;
            case ARTICULATION_snap: return SMUFL_E630_pluckedSnapPizzicatoBelow;
            case ARTICULATION_damp: return SMUFL_E638_pluckedDamp;
            // case ARTICULATION_dampall;
            case ARTICULATION_open: return SMUFL_E5E7_brassMuteOpen;
            case ARTICULATION_stop: return SMUFL_E5E5_brassMuteClosed;
            //
            // Removed in MEI 4.0
            // case ARTICULATION_ten_stacc: return SMUFL_E4B3_articTenutoStaccatoBelow;
            //
            case ARTICULATION_lhpizz: return SMUFL_E633_pluckedLeftHandPizzicato;
            case ARTICULATION_dot: return SMUFL_E4A3_articStaccatoBelow;
            case ARTICULATION_stroke: return SMUFL_E4AB_articStaccatissimoStrokeBelow;
            default: return 0;
        }
    }
    else
        return 0;
}

std::pair<char32_t, char32_t> Artic::GetEnclosingGlyphs() const
{
    std::pair<char32_t, char32_t> glyphs(0, 0);
    if (this->HasEnclose()) {
        switch (this->GetEnclose()) {
            case ENCLOSURE_brack:
                glyphs = { SMUFL_E26C_accidentalBracketLeft, SMUFL_E26D_accidentalBracketRight };
                break;
            case ENCLOSURE_paren: glyphs = { SMUFL_E26A_accidentalParensLeft, SMUFL_E26B_accidentalParensRight }; break;
            default: break;
        }
    }
    return glyphs;
}

//----------------------------------------------------------------------------
// Static methods for Artic
//----------------------------------------------------------------------------

bool Artic::VerticalCorr(char32_t code, data_STAFFREL place)
{
    if (place == STAFFREL_above)
        return false;
    else if (code == SMUFL_E611_stringsDownBowTurned)
        return true;
    else if (code == SMUFL_E613_stringsUpBowTurned)
        return true;
    else if (code == SMUFL_E630_pluckedSnapPizzicatoBelow)
        return true;
    else if (code == SMUFL_E614_stringsHarmonic)
        return true;
    else
        return false;
}

bool Artic::IsCentered(data_ARTICULATION artic)
{
    if (artic == ARTICULATION_stacc) return true;
    if (artic == ARTICULATION_ten) return true;
    return false;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

int Artic::ConvertMarkupArtic(FunctorParams *functorParams)
{
    ConvertMarkupArticParams *params = vrv_params_cast<ConvertMarkupArticParams *>(functorParams);
    assert(params);

    if (this->GetArtic().size() > 1)
        params->m_articPairsToConvert.emplace_back(std::make_pair(this->GetParent(), this));

    return FUNCTOR_CONTINUE;
}

int Artic::CalcArtic(FunctorParams *functorParams)
{
    CalcArticParams *params = vrv_params_cast<CalcArticParams *>(functorParams);
    assert(params);

    if (!params->m_parent) return FUNCTOR_CONTINUE;

    /************** placement **************/

    Layer *layer = vrv_cast<Layer *>(this->GetFirstAncestor(LAYER));
    assert(layer);

    if (params->m_parent->m_crossLayer) {
        layer = params->m_parent->m_crossLayer;
    }

    bool allowAbove = true;
    data_STEMDIRECTION layerStemDir;

    // for now we ignore within @place
    if (this->GetPlace() != STAFFREL_NONE) {
        m_drawingPlace = this->GetPlace();
        // if we have a place indication do not allow to be changed to above
        allowAbove = false;
    }
    else if ((layerStemDir = layer->GetDrawingStemDir(params->m_parent)) != STEMDIRECTION_NONE) {
        m_drawingPlace = (layerStemDir == STEMDIRECTION_up) ? STAFFREL_above : STAFFREL_below;
        // If we have more than one layer do not allow to be changed to above
        allowAbove = false;
    }
    else if (params->m_stemDir == STEMDIRECTION_up) {
        m_drawingPlace = STAFFREL_below;
    }
    else {
        m_drawingPlace = STAFFREL_above;
    }

    // Not sure what this is anymore...
    if (this->IsOutsideArtic()) {
        // If allowAbove is true it will place the above if the content requires so (even if place below if given)
        if (m_drawingPlace == STAFFREL_below && allowAbove && this->AlwaysAbove()) m_drawingPlace = STAFFREL_above;
    }

    /************** adjust the xRel position **************/

    this->SetDrawingXRel(CalculateHorizontalShift(params->m_doc, params->m_parent, params->m_stemDir));

    /************** set cross-staff / layer **************/

    // Exception for artic because they are relative to the staff - we set m_crossStaff and m_crossLayer
    if (this->GetDrawingPlace() == STAFFREL_above && params->m_crossStaffAbove) {
        this->m_crossStaff = params->m_staffAbove;
        this->m_crossLayer = params->m_layerAbove;
    }
    else if (this->GetDrawingPlace() == STAFFREL_below && params->m_crossStaffBelow) {
        this->m_crossStaff = params->m_staffBelow;
        this->m_crossLayer = params->m_layerBelow;
    }

    return FUNCTOR_CONTINUE;
}

int Artic::AdjustArtic(FunctorParams *functorParams)
{
    AdjustArticParams *params = vrv_params_cast<AdjustArticParams *>(functorParams);
    assert(params);

    if (!params->m_parent) return FUNCTOR_CONTINUE;

    int yIn, yOut, yRel;

    Staff *staff = this->GetAncestorStaff(RESOLVE_CROSS_STAFF);
    Beam *beam = dynamic_cast<Beam *>(this->GetFirstAncestor(BEAM));
    int staffYBottom = -params->m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize);

    Stem *stem = vrv_cast<Stem *>(params->m_parent->FindDescendantByType(STEM));
    Flag *flag = vrv_cast<Flag *>(params->m_parent->FindDescendantByType(FLAG));
    // Avoid in artic to be in legder lines
    if (this->GetDrawingPlace() == STAFFREL_above) {
        int yAboveStem
            = params->m_parent->GetDrawingTop(params->m_doc, staff->m_drawingStaffSize, false) - staff->GetDrawingY();
        if (flag && stem && (stem->GetDrawingStemDir() == STEMDIRECTION_up))
            yAboveStem += flag->GetStemUpSE(params->m_doc, staff->m_drawingStaffSize, false).y;
        yIn = std::max(yAboveStem, staffYBottom);
        yOut = std::max(yIn, 0);
    }
    else {
        int yBelowStem = params->m_parent->GetDrawingBottom(params->m_doc, staff->m_drawingStaffSize, false)
            - staff->GetDrawingY();
        if (flag && stem && (stem->GetDrawingStemDir() == STEMDIRECTION_down))
            yBelowStem += flag->GetStemDownNW(params->m_doc, staff->m_drawingStaffSize, false).y;
        yIn = std::min(yBelowStem, 0);
        if (beam && beam->m_crossStaffContent && beam->m_drawingPlace == BEAMPLACE_mixed) yIn -= beam->m_beamWidthBlack;
        yOut = std::min(yIn, staffYBottom);
    }

    yRel = this->IsInsideArtic() ? yIn : yOut;
    this->SetDrawingYRel(yRel);

    // Adjust according to the position of a previous artic
    Artic *previous = NULL;
    if (this->GetDrawingPlace() == STAFFREL_above && !params->m_articAbove.empty()) {
        previous = params->m_articAbove.back();
        int inTop = previous->GetContentTop();
        int outBottom = this->GetContentBottom();
        if (inTop > outBottom) {
            this->SetDrawingYRel(this->GetDrawingYRel() + inTop - outBottom);
        }
    }
    if (this->GetDrawingPlace() == STAFFREL_below && !params->m_articBelow.empty()) {
        previous = params->m_articBelow.back();
        int inBottom = previous->GetContentBottom();
        int outTop = this->GetContentTop();
        if (inBottom < outTop) {
            this->SetDrawingYRel(this->GetDrawingYRel() - outTop + inBottom);
        }
    }

    // Add spacing
    const int unit = params->m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    const int spacingTop = params->m_doc->GetTopMargin(ARTIC) * unit;
    const int spacingBottom = params->m_doc->GetBottomMargin(ARTIC) * unit;
    const int direction = (this->GetDrawingPlace() == STAFFREL_above) ? 1 : -1;
    int y = this->GetDrawingY();
    int yShift = 0;

    const int bottomMargin = staff->GetDrawingY() - params->m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize);
    if (this->IsInsideArtic()) {
        // If we are above the top of the  staff, just pile them up
        if ((this->GetDrawingPlace() == STAFFREL_above) && (y > staff->GetDrawingY())) {
            yShift += spacingBottom;
        }
        // If we are below the bottom, just pile the down
        else if ((this->GetDrawingPlace() == STAFFREL_below) && (y < bottomMargin)) {
            if (y > bottomMargin - unit) {
                yShift = (bottomMargin - unit) - y;
                if (std::abs(yShift) < spacingTop) yShift = -spacingTop;
            }
            else {
                yShift -= spacingTop;
            }
        }
        // Otherwise make it fit the staff space
        else {
            yShift = staff->GetNearestInterStaffPosition(y, params->m_doc, this->GetDrawingPlace()) - y;
            if (staff->IsOnStaffLine(y + yShift, params->m_doc)) yShift += unit * direction;
        }
    }
    // Artic part outside just need to be piled up or down
    else {
        const int spacing = (direction > 0) ? spacingBottom : spacingTop;
        yShift += spacing * direction;
    }
    this->SetDrawingYRel(this->GetDrawingYRel() + yShift);

    // Add it to the list of previous artics - actually keeping only the last one could be sufficient?
    if (this->GetDrawingPlace() == STAFFREL_above) {
        params->m_articAbove.push_back(this);
    }
    else {
        params->m_articBelow.push_back(this);
    }

    return FUNCTOR_SIBLINGS;
}

int Artic::AdjustArticWithSlurs(FunctorParams *functorParams)
{
    FunctorDocParams *params = vrv_params_cast<FunctorDocParams *>(functorParams);
    assert(params);

    if (m_startSlurPositioners.empty() && m_endSlurPositioners.empty()) return FUNCTOR_CONTINUE;

    std::vector<FloatingCurvePositioner *>::iterator iter;
    for (iter = m_endSlurPositioners.begin(); iter != m_endSlurPositioners.end(); ++iter) {
        // if (this->Encloses((*iter)->m_points[1])) this->SetColor("red");
        int shift = this->Intersects((*iter), CONTENT, params->m_doc->GetDrawingUnit(100));
        if (shift != 0) {
            this->SetDrawingYRel(this->GetDrawingYRel() + shift);
            // this->SetColor("red");
        }
    }

    for (iter = m_startSlurPositioners.begin(); iter != m_startSlurPositioners.end(); ++iter) {
        // if (this->Encloses((*iter)->m_points[1])) this->SetColor("red");
        int shift = this->Intersects((*iter), CONTENT, params->m_doc->GetDrawingUnit(100));
        if (shift != 0) {
            this->SetDrawingYRel(this->GetDrawingYRel() + shift);
            // this->SetColor("green");
        }
    }

    return FUNCTOR_SIBLINGS;
}

int Artic::ResetVerticalAlignment(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetVerticalAlignment(functorParams);

    m_startSlurPositioners.clear();
    m_endSlurPositioners.clear();

    return FUNCTOR_CONTINUE;
}

int Artic::ResetData(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetData(functorParams);

    m_drawingPlace = STAFFREL_NONE;

    return FUNCTOR_CONTINUE;
}

int Artic::CalculateHorizontalShift(const Doc *doc, const LayerElement *parent, data_STEMDIRECTION stemDir) const
{
    int shift = parent->GetDrawingRadius(doc);
    if ((parent->GetChildCount(ARTIC) > 1) || (doc->GetOptions()->m_staccatoCenter.GetValue())) {
        return shift;
    }
    data_ARTICULATION artic = this->GetArticFirst();
    switch (artic) {
        case ARTICULATION_stacc:
        case ARTICULATION_stacciss: {
            const Staff *staff = this->GetAncestorStaff();
            const int stemWidth = doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
            if ((stemDir == STEMDIRECTION_up) && (m_drawingPlace == STAFFREL_above)) {
                shift += shift - stemWidth / 2;
            }
            else if ((stemDir == STEMDIRECTION_down) && (m_drawingPlace == STAFFREL_below)) {
                shift = stemWidth / 2;
            }
            break;
        }
        default: {
            break;
        }
    }

    return shift;
}

} // namespace vrv

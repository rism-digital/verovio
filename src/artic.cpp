/////////////////////////////////////////////////////////////////////////////
// Name:        artic.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "artic.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "doc.h"
#include "floatingobject.h"
#include "functorparams.h"
#include "layer.h"
#include "smufl.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

const std::vector<data_ARTICULATION> Artic::s_outStaffArtic = { ARTICULATION_acc, ARTICULATION_dnbow, ARTICULATION_marc,
    ARTICULATION_upbow, ARTICULATION_harm, ARTICULATION_snap, ARTICULATION_damp };

const std::vector<data_ARTICULATION> Artic::s_aboveStaffArtic = { ARTICULATION_dnbow, ARTICULATION_marc,
    ARTICULATION_upbow, ARTICULATION_harm, ARTICULATION_snap, ARTICULATION_damp };

//----------------------------------------------------------------------------
// Artic
//----------------------------------------------------------------------------

static const ClassRegistrar<Artic> s_factory("artic", ARTIC);

Artic::Artic()
    : LayerElement(ARTIC, "artic-")
    , AttArticulation()
    , AttColor()
    , AttEnclosingChars()
    , AttExtSym()
    , AttPlacementRelEvent()
{
    RegisterAttClass(ATT_ARTICULATION);
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_ENCLOSINGCHARS);
    RegisterAttClass(ATT_EXTSYM);
    RegisterAttClass(ATT_PLACEMENTRELEVENT);

    Reset();
}

Artic::~Artic() {}

void Artic::Reset()
{
    LayerElement::Reset();
    ResetArticulation();
    ResetColor();
    ResetEnclosingChars();
    ResetExtSym();
    ResetPlacementRelEvent();

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
            parent->GetUuid().c_str());
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
    parentNoteOrChord->FindAllDescendantBetween(&children, &matchType, first, last);
    for (auto &child : children) {
        if (child == this) continue;
        Artic *artic = vrv_cast<Artic *>(child);
        assert(artic);
        if (artic->GetDrawingPlace() == this->GetDrawingPlace()) artics.push_back(artic);
    }
}

void Artic::SplitArtic(std::vector<data_ARTICULATION> *insideSlur, std::vector<data_ARTICULATION> *outsideSlur)
{
    assert(insideSlur);
    assert(outsideSlur);

    std::vector<data_ARTICULATION> articList = this->GetArtic();
    for (data_ARTICULATION artic : articList) {
        if (IsInsideArtic(artic)) {
            insideSlur->push_back(artic);
        }
        else {
            outsideSlur->push_back(artic);
        }
    }
}

bool Artic::AlwaysAbove()
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

wchar_t Artic::GetArticGlyph(data_ARTICULATION artic, data_STAFFREL place) const
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

    if (place == STAFFREL_above) {
        switch (artic) {
            case ARTICULATION_acc: return SMUFL_E4A0_articAccentAbove;
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
            // case ARTICULATION_open;
            // case ARTICULATION_stop;
            // case ARTICULATION_dbltongue;
            // case ARTICULATION_trpltongue;
            // case ARTICULATION_heel;
            // case ARTICULATION_toe;
            // case ARTICULATION_tap;
            // case ARTICULATION_lhpizz;
            // case ARTICULATION_dot;
            case ARTICULATION_stroke: return SMUFL_E4AA_articStaccatissimoStrokeAbove;
            default: return 0;
        }
    }
    else if (place == STAFFREL_below) {
        switch (artic) {
            case ARTICULATION_acc: return SMUFL_E4A1_articAccentBelow;
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
            //
            // Removed in MEI 4.0
            // case ARTICULATION_ten_stacc: return SMUFL_E4B3_articTenutoStaccatoBelow;
            //
            case ARTICULATION_stroke: return SMUFL_E4AB_articStaccatissimoStrokeBelow;
            default: return 0;
        }
    }
    else
        return 0;
}

wchar_t Artic::GetEnclosingGlyph(bool beforeArtic) const
{
    wchar_t glyph = 0;
    if (this->HasEnclose()) {
        switch (this->GetEnclose()) {
            case ENCLOSURE_brack:
                glyph = beforeArtic ? SMUFL_E26C_accidentalBracketLeft : SMUFL_E26D_accidentalBracketRight;
                break;
            case ENCLOSURE_paren:
                glyph = beforeArtic ? SMUFL_E26A_accidentalParensLeft : SMUFL_E26B_accidentalParensRight;
                break;
            default: break;
        }
    }
    return glyph;
}

//----------------------------------------------------------------------------
// Static methods for Artic
//----------------------------------------------------------------------------

bool Artic::VerticalCorr(wchar_t code, data_STAFFREL place)
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

    if (this->GetArtic().size() > 1) params->m_articPairsToConvert.emplace_back(std::make_pair(GetParent(), this));

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

    // Get the parent or cross-staff / layer

    Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff);

    if (m_crossStaff) {
        staff = m_crossStaff;
    }

    Beam *beam = dynamic_cast<Beam *>(GetFirstAncestor(BEAM));
    int staffYBottom = -params->m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize);
    // Avoid in artic to be in legder lines
    if (this->GetDrawingPlace() == STAFFREL_above) {
        yIn = std::max(
            params->m_parent->GetDrawingTop(params->m_doc, staff->m_drawingStaffSize, false) - staff->GetDrawingY(),
            staffYBottom);
        yOut = std::max(yIn, 0);
    }
    else {
        yIn = std::min(
            params->m_parent->GetDrawingBottom(params->m_doc, staff->m_drawingStaffSize, false) - staff->GetDrawingY(),
            0);
        if (beam && beam->m_crossStaffContent && beam->m_drawingPlace == BEAMPLACE_mixed) yIn -= beam->m_beamWidth;
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
    int spacingTop = params->m_doc->GetTopMargin(ARTIC) * params->m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    int spacingBottom
        = params->m_doc->GetBottomMargin(ARTIC) * params->m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    int y = this->GetDrawingY();
    int yShift = 0;
    int direction = (this->GetDrawingPlace() == STAFFREL_above) ? 1 : -1;

    if (this->IsInsideArtic()) {
        // If we are above the top of the  staff, just pile them up
        if ((this->GetDrawingPlace() == STAFFREL_above) && (y > staff->GetDrawingY())) {
            yShift += spacingBottom;
        }
        // If we are below the bottom, just pile the down
        else if ((this->GetDrawingPlace() == STAFFREL_below)
            && (y < staff->GetDrawingY() - params->m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize))) {
            yShift -= spacingTop;
        }
        // Otherwise make it fit the staff space
        else {
            yShift = staff->GetNearestInterStaffPosition(y, params->m_doc, this->GetDrawingPlace()) - y;
            if (staff->IsOnStaffLine(y + yShift, params->m_doc))
                yShift += params->m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * direction;
        }
    }
    // Artic part outside just need to be piled up or down
    else {
        int spacing = (direction > 0) ? spacingBottom : spacingTop;
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

int Artic::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetDrawing(functorParams);

    m_drawingPlace = STAFFREL_NONE;

    return FUNCTOR_CONTINUE;
}

int Artic::CalculateHorizontalShift(Doc *doc, LayerElement *parent, data_STEMDIRECTION stemDir) const
{
    int shift = parent->GetDrawingRadius(doc);
    if ((parent->GetChildCount(ARTIC) > 1) || (doc->GetOptions()->m_staccatoCenter.GetValue())) {
        return shift;
    }
    data_ARTICULATION artic = GetArticFirst();
    switch (artic) {
        case ARTICULATION_stacc:
        case ARTICULATION_stacciss: {
            Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
            assert(staff);
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

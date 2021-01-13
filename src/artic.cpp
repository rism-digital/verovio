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

std::vector<data_ARTICULATION> Artic::s_outStaffArtic
    = { ARTICULATION_acc, ARTICULATION_dnbow, ARTICULATION_marc, ARTICULATION_upbow, ARTICULATION_harm };

std::vector<data_ARTICULATION> Artic::s_aboveStaffArtic
    = { ARTICULATION_dnbow, ARTICULATION_marc, ARTICULATION_upbow, ARTICULATION_harm };

//----------------------------------------------------------------------------
// Artic
//----------------------------------------------------------------------------

Artic::Artic() : LayerElement("artic-"), AttArticulation(), AttColor(), AttPlacement()
{
    RegisterAttClass(ATT_ARTICULATION);
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_PLACEMENT);

    Reset();
}

Artic::~Artic() {}

void Artic::Reset()
{
    LayerElement::Reset();
    ResetArticulation();
    ResetColor();
    ResetPlacement();
}

bool Artic::IsSupportedChild(Object *child)
{
    if (child->Is(ARTIC_PART)) {
        assert(dynamic_cast<ArticPart *>(child));
    }
    else {
        return false;
    }
    return true;
}

void Artic::SplitArtic(std::vector<data_ARTICULATION> *insideSlur, std::vector<data_ARTICULATION> *outsideSlur)
{
    assert(insideSlur);
    assert(outsideSlur);

    std::vector<data_ARTICULATION>::iterator iter;
    auto end = Artic::s_outStaffArtic.end();
    std::vector<data_ARTICULATION> articList = this->GetArtic();

    for (iter = articList.begin(); iter != articList.end(); ++iter) {
        // return false if one cannot be rendered on the staff
        auto i = std::find(Artic::s_outStaffArtic.begin(), end, *iter);
        if (i != end)
            outsideSlur->push_back(*iter);
        else
            insideSlur->push_back(*iter);
    }
}

ArticPart *Artic::GetInsidePart()
{
    ArticPartTypeComparison articPartComparison(ARTIC_PART_INSIDE);
    return dynamic_cast<ArticPart *>(FindDescendantByComparison(&articPartComparison, 1));
}

ArticPart *Artic::GetOutsidePart()
{
    ArticPartTypeComparison articPartComparison(ARTIC_PART_OUTSIDE);
    return dynamic_cast<ArticPart *>(FindDescendantByComparison(&articPartComparison, 1));
}

wchar_t Artic::GetSmuflCode(data_ARTICULATION artic, const data_STAFFREL &place)
{
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
            // case ARTICULATION_snap;
            // case ARTICULATION_fingernail;
            // case ARTICULATION_ten_stacc: return SMUFL_E4B2_articTenutoStaccatoAbove;
            // case ARTICULATION_damp;
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
            // case ARTICULATION_stroke;
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
            //
            // Removed in MEI 4.0
            // case ARTICULATION_ten_stacc: return SMUFL_E4B3_articTenutoStaccatoBelow;
            //
            default: return 0;
        }
    }
    else
        return 0;
}

bool Artic::VerticalCorr(wchar_t code, const data_STAFFREL &place)
{
    if (place == STAFFREL_above)
        return false;
    else if (code == SMUFL_E611_stringsDownBowTurned)
        return true;
    else if (code == SMUFL_E613_stringsUpBowTurned)
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
// ArticPart
//----------------------------------------------------------------------------

ArticPart::ArticPart(ArticPartType type, Artic *artic)
    : LayerElement("artic-part-"), AttArticulation(), AttColor(), AttPlacement()
{
    assert(artic);

    RegisterAttClass(ATT_ARTICULATION);
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_PLACEMENT);

    m_type = type;

    Reset();

    // preserve the color from the parent artic
    this->SetColor(artic->GetColor());
}

ArticPart::~ArticPart() {}

void ArticPart::Reset()
{
    LayerElement::Reset();
    ResetArticulation();
    ResetColor();
    ResetPlacement();
}

bool ArticPart::AlwaysAbove()
{
    std::vector<data_ARTICULATION>::iterator iter;
    auto end = Artic::s_aboveStaffArtic.end();
    std::vector<data_ARTICULATION> articList = this->GetArtic();

    for (iter = articList.begin(); iter != articList.end(); ++iter) {
        // return false if one has always to be rendered above the staff
        auto i = std::find(Artic::s_aboveStaffArtic.begin(), end, *iter);
        if (i != end) {
            return true;
        }
    }
    return false;
}

void ArticPart::AddSlurPositioner(FloatingCurvePositioner *positioner, bool start)
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

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

int Artic::CalcArtic(FunctorParams *functorParams)
{
    FunctorDocParams *params = vrv_params_cast<FunctorDocParams *>(functorParams);
    assert(params);

    /************** Get the parent and the stem direction **************/

    LayerElement *parent = NULL;
    Note *parentNote = NULL;
    Chord *parentChord = dynamic_cast<Chord *>(this->GetFirstAncestor(CHORD, 2));
    data_STEMDIRECTION stemDir = STEMDIRECTION_NONE;
    data_STAFFREL place = STAFFREL_NONE;

    if (!parentChord) {
        parentNote = dynamic_cast<Note *>(this->GetFirstAncestor(NOTE));
        parent = parentNote;
    }
    else {
        parent = parentChord;
    }

    if (!parentChord && !parentNote) {
        // no parent chord or note, nothing we can do...
        return FUNCTOR_CONTINUE;
    }

    Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff);
    Layer *layer = vrv_cast<Layer *>(this->GetFirstAncestor(LAYER));
    assert(layer);
    if (parent && parent->m_crossStaff) layer = parent->m_crossLayer;

    stemDir = parentNote ? parentNote->GetDrawingStemDir() : parentChord->GetDrawingStemDir();

    /************** placement **************/

    bool allowAbove = true;
    data_STEMDIRECTION layerStemDir;

    // for now we ignore within @place
    if (this->GetPlace() != STAFFREL_NONE) {
        place = this->GetPlace();
        // If we have a place indication do not allow to be changed to above
        allowAbove = false;
    }
    else if ((layerStemDir = layer->GetDrawingStemDir(parent)) != STEMDIRECTION_NONE) {
        place = (layerStemDir == STEMDIRECTION_up) ? STAFFREL_above : STAFFREL_below;
        // If we have more than one layer do not allow to be changed to above
        allowAbove = false;
    }
    else if (stemDir == STEMDIRECTION_up)
        place = STAFFREL_below;
    else
        place = STAFFREL_above;

    /************** adjust the xRel position **************/

    int xShift = parent->GetDrawingRadius(params->m_doc);
    this->SetDrawingXRel(xShift);

    /************** set it to both the inside and outside part **************/

    ArticPart *insidePart = this->GetInsidePart();
    ArticPart *outsidePart = this->GetOutsidePart();

    if (insidePart) {
        insidePart->SetPlace(place);
    }

    if (outsidePart) {
        // If allowAbove is true it will place the above if the content requires so (even if place below if given)
        if (place == STAFFREL_below && allowAbove && outsidePart->AlwaysAbove()) place = STAFFREL_above;
        outsidePart->SetPlace(place);
    }

    /************** calculate the y position **************/

    Staff *staffAbove = NULL;
    Staff *staffBelow = NULL;
    Layer *crossLayer = NULL;

    // Cross-staff handling of articulation will need to be re-thought. We can look at assiging a cross-staff to the
    // appropriate ArticPart
    // (see below) - For chords, we need to distinguish cross-staff chords and cross-staff chord notes
    if (parent->m_crossStaff && parent->m_crossLayer) {
        staff = parent->m_crossStaff;
        staffAbove = staff;
        staffBelow = staff;
        crossLayer = parent->m_crossLayer;
    }
    else if (parentChord) {
        parentChord->GetCrossStaffExtremes(staffAbove, staffBelow);
    }

    int staffYBottom = -params->m_doc->GetDrawingStaffSize(staff->m_drawingStaffSize);
    // Avoid in artic to be in legder lines
    int yInAbove = std::max(
        parent->GetDrawingTop(params->m_doc, staff->m_drawingStaffSize, false) - staff->GetDrawingY(), staffYBottom);
    int yInBelow
        = std::min(parent->GetDrawingBottom(params->m_doc, staff->m_drawingStaffSize, false) - staff->GetDrawingY(), 0);
    int yOutAbove = std::max(yInAbove, 0);
    int yOutBelow = std::min(yInBelow, staffYBottom);

    // Does not work properly with chords, needs rethinking - It might be better to make artic or articPart relative to
    // notes
    // The problem is that in MEI artic are children of chord element and not of the notes
    if (insidePart) {
        if (insidePart->GetPlace() == STAFFREL_above) {
            insidePart->SetDrawingYRel(yInAbove);
            if (parent->m_crossStaff) {
                insidePart->m_crossStaff = staffAbove;
                insidePart->m_crossLayer = crossLayer;
            }
        }
        else {
            insidePart->SetDrawingYRel(yInBelow);
            if (parent->m_crossStaff) {
                insidePart->m_crossStaff = staffBelow;
                insidePart->m_crossLayer = crossLayer;
            }
        }
    }

    if (outsidePart) {
        if (outsidePart->GetPlace() == STAFFREL_above) {
            outsidePart->SetDrawingYRel(yOutAbove);
            if (parent->m_crossStaff) {
                outsidePart->m_crossStaff = staffAbove;
                outsidePart->m_crossLayer = crossLayer;
            }
        }
        else {
            outsidePart->SetDrawingYRel(yOutBelow);
            if (parent->m_crossStaff) {
                outsidePart->m_crossStaff = staffBelow;
                outsidePart->m_crossLayer = crossLayer;
            }
        }
    }

    // If we have both an inside and outside part we need to move the outside part away when they are both on the same
    // side
    if (insidePart && outsidePart) {

        int margin = params->m_doc->GetTopMargin(insidePart->GetClassId())
            * params->m_doc->GetDrawingUnit(staff->m_drawingStaffSize);

        if (insidePart->GetPlace() == outsidePart->GetPlace()) {
            if (insidePart->GetPlace() == STAFFREL_above) {
                int inTop = insidePart->GetContentTop();
                int outBottom = outsidePart->GetContentBottom();
                if (inTop > outBottom)
                    outsidePart->SetDrawingYRel(outsidePart->GetDrawingYRel() + inTop - outBottom + margin);
            }
            else {
                int inBottom = insidePart->GetContentBottom();
                int outTop = outsidePart->GetContentTop();
                if (inBottom < outTop)
                    outsidePart->SetDrawingYRel(outsidePart->GetDrawingYRel() + outTop - inBottom + margin);
            }
        }
    }

    return FUNCTOR_SIBLINGS;
}

int Artic::PrepareLayerElementParts(FunctorParams *functorParams)
{
    std::vector<data_ARTICULATION> insideSlur;
    std::vector<data_ARTICULATION> outsideSlur;

    this->SplitArtic(&insideSlur, &outsideSlur);

    if (insideSlur.size() > 0) {
        ArticPart *articPart = new ArticPart(ARTIC_PART_INSIDE, this);
        articPart->SetArtic(insideSlur);
        this->AddChild(articPart);
    }
    if (outsideSlur.size() > 0) {
        ArticPart *articPart = new ArticPart(ARTIC_PART_OUTSIDE, this);
        articPart->SetArtic(outsideSlur);
        this->AddChild(articPart);
    }

    /************ Prepare the drawing cue size ************/

    Functor prepareDrawingCueSize(&Object::PrepareDrawingCueSize);
    this->Process(&prepareDrawingCueSize, NULL);

    return FUNCTOR_CONTINUE;
}

int Artic::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetDrawing(functorParams);

    // Remove all ArticPart children
    ClearChildren();

    return FUNCTOR_CONTINUE;
}

int ArticPart::ResetVerticalAlignment(FunctorParams *functorParams)
{
    m_startSlurPositioners.clear();
    m_endSlurPositioners.clear();

    return FUNCTOR_CONTINUE;
}

int ArticPart::AdjustArticWithSlurs(FunctorParams *functorParams)
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

} // namespace vrv

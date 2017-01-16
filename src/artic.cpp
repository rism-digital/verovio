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

#include "attcomparison.h"
#include "doc.h"
#include "floatingobject.h"
#include "functorparams.h"
#include "smufl.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

std::vector<data_ARTICULATION> Artic::s_outStaffArtic = { ARTICULATION_acc, ARTICULATION_dnbow, ARTICULATION_marc,
    ARTICULATION_marc_stacc, ARTICULATION_upbow, ARTICULATION_harm };

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

Artic::~Artic()
{
}

void Artic::Reset()
{
    LayerElement::Reset();
    ResetArticulation();
    ResetColor();
    ResetPlacement();
}

void Artic::AddChild(Object *child)
{
    if (child->Is() == ARTIC_PART) {
        assert(dynamic_cast<ArticPart *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

void Artic::SplitArtic(std::vector<data_ARTICULATION> *insideSlur, std::vector<data_ARTICULATION> *outsideSlur)
{
    assert(insideSlur);
    assert(outsideSlur);

    std::vector<data_ARTICULATION>::iterator iter;
    auto end = Artic::s_outStaffArtic.end();
    std::vector<data_ARTICULATION> articList = this->GetArtic();

    for (iter = articList.begin(); iter != articList.end(); iter++) {
        // return false if one cannot be rendered on the staff
        auto i = std::find(Artic::s_outStaffArtic.begin(), end, *iter);
        if (i != end)
            outsideSlur->push_back(*iter);
        else
            insideSlur->push_back(*iter);
    }
}

void Artic::UpdateOutsidePartPosition(int yAbove, int yBelow, data_STAFFREL place, bool allowAbove)
{
    ArticPart *outsidePart = GetOutsidePart();
    if (!outsidePart) return;

    if (place == STAFFREL_below && allowAbove && outsidePart->AlwaysAbove()) place = STAFFREL_above;

    outsidePart->SetPlace(place);
    if (place == STAFFREL_above)
        outsidePart->SetDrawingY(yAbove);
    else
        outsidePart->SetDrawingY(yBelow);
}

void Artic::UpdateInsidePartPosition(int yAbove, int yBelow, data_STAFFREL place)
{
    ArticPart *insidePart = GetInsidePart();
    if (!insidePart) return;

    insidePart->SetPlace(place);
    if (place == STAFFREL_above)
        insidePart->SetDrawingY(yAbove);
    else
        insidePart->SetDrawingY(yBelow);
}

ArticPart *Artic::GetInsidePart()
{
    ArticPartTypeComparison articPartComparison(ARTIC_PART_INSIDE);
    return dynamic_cast<ArticPart *>(FindChildByAttComparison(&articPartComparison, 1));
}

ArticPart *Artic::GetOutsidePart()
{
    ArticPartTypeComparison articPartComparison(ARTIC_PART_OUTSIDE);
    return dynamic_cast<ArticPart *>(FindChildByAttComparison(&articPartComparison, 1));
}

wchar_t Artic::GetSmuflCode(data_ARTICULATION artic, data_STAFFREL place)
{
    if (place == STAFFREL_above) {
        switch (artic) {
            case ARTICULATION_acc: return SMUFL_E4A0_articAccentAbove;
            case ARTICULATION_stacc: return SMUFL_E4A2_articStaccatoAbove;
            case ARTICULATION_ten: return SMUFL_E4A4_articTenutoAbove;
            case ARTICULATION_stacciss: return SMUFL_E4A8_articStaccatissimoWedgeAbove;
            case ARTICULATION_marc: return SMUFL_E4AC_articMarcatoAbove;
            case ARTICULATION_marc_stacc: return SMUFL_E4AE_articMarcatoStaccatoAbove;
            case ARTICULATION_spicc:
                return SMUFL_E4A6_articStaccatissimoAbove;
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
            case ARTICULATION_harm:
                return SMUFL_E614_stringsHarmonic;
            // case ARTICULATION_snap;
            // case ARTICULATION_fingernail;
            case ARTICULATION_ten_stacc:
                return SMUFL_E4B2_articTenutoStaccatoAbove;
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
            default: return 0; break;
        }
    }
    else if (place == STAFFREL_below) {
        switch (artic) {
            case ARTICULATION_acc: return SMUFL_E4A1_articAccentBelow;
            case ARTICULATION_stacc: return SMUFL_E4A3_articStaccatoBelow;
            case ARTICULATION_ten: return SMUFL_E4A5_articTenutoBelow;
            case ARTICULATION_stacciss: return SMUFL_E4A9_articStaccatissimoWedgeBelow;
            case ARTICULATION_marc: return SMUFL_E4AD_articMarcatoBelow;
            case ARTICULATION_marc_stacc: return SMUFL_E4AF_articMarcatoStaccatoBelow;
            case ARTICULATION_spicc:
                return SMUFL_E4A7_articStaccatissimoBelow;
            //
            case ARTICULATION_dnbow: return SMUFL_E611_stringsDownBowTurned;
            case ARTICULATION_upbow: return SMUFL_E613_stringsUpBowTurned;
            case ARTICULATION_harm:
                return SMUFL_E614_stringsHarmonic;
            //
            case ARTICULATION_ten_stacc:
                return SMUFL_E4B3_articTenutoStaccatoBelow;
            //
            default: return 0; break;
        }
    }
    else
        return 0;
}

bool Artic::VerticalCorr(wchar_t code, data_STAFFREL place)
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

ArticPart::~ArticPart()
{
}

void ArticPart::Reset()
{
    LayerElement::Reset();
    ResetArticulation();
    ResetColor();
    ResetPlacement();

    m_drawingYRel = 0;
}

int ArticPart::GetDrawingY() const
{
    return BoundingBox::GetDrawingY() - this->GetDrawingYRel();
}

void ArticPart::SetDrawingYRel(int drawingYRel)
{
    if (GetPlace() == STAFFREL_above) {
        if (drawingYRel < m_drawingYRel) m_drawingYRel = drawingYRel;
    }
    else {
        if (drawingYRel > m_drawingYRel) m_drawingYRel = drawingYRel;
    }
};

bool ArticPart::AlwaysAbove()
{
    std::vector<data_ARTICULATION>::iterator iter;
    auto end = Artic::s_aboveStaffArtic.end();
    std::vector<data_ARTICULATION> articList = this->GetArtic();

    for (iter = articList.begin(); iter != articList.end(); iter++) {
        // return false if one has always to be rendered above the staff
        auto i = std::find(Artic::s_aboveStaffArtic.begin(), end, *iter);
        if (i != end) {
            return true;
        }
    }
    return false;
}

void ArticPart::AddSlurPositioner(FloatingPositioner *positioner, bool start)
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

int Artic::AdjustArtic(FunctorParams *functorParams)
{
    AdjustArticParams *params = dynamic_cast<AdjustArticParams *>(functorParams);
    assert(params);

    ArticPart *insidePart = this->GetInsidePart();
    ArticPart *outsidePart = this->GetOutsidePart();

    if (!outsidePart) return FUNCTOR_SIBLINGS;

    if (insidePart) {

        Staff *staff = dynamic_cast<Staff *>(this->GetFirstParent(STAFF));
        assert(staff);
        int margin = params->m_doc->GetTopMargin(insidePart->Is())
            * params->m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / PARAM_DENOMINATOR;

        if (insidePart->GetPlace() == outsidePart->GetPlace()) {
            if (insidePart->GetPlace() == STAFFREL_above) {
                int inTop = insidePart->GetContentTop();
                int outBottom = outsidePart->GetContentBottom();
                if (inTop > outBottom) outsidePart->SetDrawingYRel(outBottom - inTop - margin);
            }
            else {
                int inBottom = insidePart->GetContentBottom();
                int outTop = outsidePart->GetContentTop();
                if (inBottom < outTop) outsidePart->SetDrawingYRel(outTop - inBottom + margin);
            }
        }
    }

    return FUNCTOR_SIBLINGS;
}

int Artic::PrepareArtic(FunctorParams *functorParams)
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

    return FUNCTOR_CONTINUE;
};

int Artic::ResetDrawing(FunctorParams *functorParams)
{
    // Remove all ArticPart children
    ClearChildren();

    return FUNCTOR_CONTINUE;
};

int ArticPart::ResetVerticalAlignment(FunctorParams *functorParams)
{
    m_drawingYRel = 0;
    m_startSlurPositioners.clear();
    m_endSlurPositioners.clear();

    return FUNCTOR_CONTINUE;
}

int ArticPart::AdjustArticWithSlurs(FunctorParams *functorParams)
{
    AdjustArticWithSlursParams *params = dynamic_cast<AdjustArticWithSlursParams *>(functorParams);
    assert(params);

    if (m_startSlurPositioners.empty() && m_endSlurPositioners.empty()) return FUNCTOR_CONTINUE;

    std::vector<FloatingPositioner *>::iterator iter;
    for (iter = m_endSlurPositioners.begin(); iter != m_endSlurPositioners.end(); iter++) {
        // if (this->Encloses((*iter)->m_cuvrePoints[1])) this->SetColor("red");
        int shift = this->Intersects((*iter), params->m_doc->GetDrawingUnit(100));
        if (shift != 0) {
            this->SetDrawingYRel(this->GetDrawingYRel() - shift);
            // this->SetColor("red");
        }
    }

    for (iter = m_startSlurPositioners.begin(); iter != m_startSlurPositioners.end(); iter++) {
        // if (this->Encloses((*iter)->m_cuvrePoints[1])) this->SetColor("red");
        int shift = this->Intersects((*iter), params->m_doc->GetDrawingUnit(100));
        if (shift != 0) {
            this->SetDrawingYRel(this->GetDrawingYRel() - shift);
            // this->SetColor("green");
        }
    }

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv

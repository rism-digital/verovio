/////////////////////////////////////////////////////////////////////////////
// Name:        barline.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "divline.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "doc.h"
#include "functorparams.h"
#include "horizontalaligner.h"
#include "layer.h"
#include "measure.h"
#include "staff.h"
#include "system.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// DivLine
//----------------------------------------------------------------------------

DivLine::DivLine() : LayerElement("dline-"), AttDivLineLog(), AttColor(), AttVisibility()
{
    RegisterAttClass(ATT_DIVLINELOG);
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_VISIBILITY);

    Reset();
}

DivLine::~DivLine() {}

void DivLine::Reset()
{
    LayerElement::Reset();

    // Reset();
    ResetColor();
    ResetVisibility();
}

// bool DivLine::SetAlignment(Alignment *alignment)
// {
//     m_alignment = alignment;
//     return (m_alignment->AddLayerElementRef(this));
// }

std::wstring DivLine::GetSymbolStr() const
{
    if (!this->HasForm()) return L"";

    wchar_t symc = GetDivLineGlyph(this->GetForm());
    std::wstring symbolStr;

    // if (this->HasEnclose()) {
    //     if (this->GetEnclose() == ENCLOSURE_brack) {
    //         symbolStr.push_back(SMUFL_E26C_accidentalBracketLeft);
    //         symbolStr.push_back(symc);
    //         symbolStr.push_back(SMUFL_E26D_accidentalBracketRight);
    //     }
    //     else {
    //         symbolStr.push_back(SMUFL_E26A_accidentalParensLeft);
    //         symbolStr.push_back(symc);
    //         symbolStr.push_back(SMUFL_E26B_accidentalParensRight);
    //     }
    // }
    // else {
    //     symbolStr.push_back(symc);
    // }
    
    symbolStr.push_back(symc);

    return symbolStr;
}

wchar_t DivLine::GetDivLineGlyph(data_DIVLINE divLine)
{
    int symc = SMUFL_E8F5_chantDivisioMaxima;
    switch (divLine) {
        case DIVLINE_minima: symc = SMUFL_E8F3_chantDivisioMinima; break;
        case DIVLINE_maior: symc = SMUFL_E8F4_chantDivisioMaior; break;
        case DIVLINE_maxima: symc = SMUFL_E8F5_chantDivisioMaxima; break;
        case DIVLINE_finalis: symc = SMUFL_E8F6_chantDivisioFinalis; break;
        default: break;
    }
    return symc;
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------


// wchar_t DivLine::GetDivLineGlyph(data_DIVISIO divLine){
//     int symc = SMUFL_E8F5_chantDivisioMaxima;
// }
// bool DivLine::HasRepetitionDots() const
// {
//     if (GetForm() == BARRENDITION_rptstart || GetForm() == BARRENDITION_rptend || GetForm() == BARRENDITION_rptboth) {
//         return true;
//     }
//     return false;
// }

//----------------------------------------------------------------------------
// DivLineAttr
//----------------------------------------------------------------------------

// DivLineAttr::DivLineAttr() : DivLine()
// {
//     m_isLeft = false;
// }

DivLineAttr::~DivLineAttr() {}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------
/*
int DivLine::ConvertToCastOffMensural(FunctorParams *functorParams)
{
    ConvertToCastOffMensuralParams *params = dynamic_cast<ConvertToCastOffMensuralParams *>(functorParams);
    assert(params);

    assert(m_alignment);
    assert(params->m_targetSubSystem);
    assert(params->m_targetLayer);

    // If this is the last barline of the layer, we will just move it and do not create a new segment
    bool isLast = (params->m_contentLayer->GetLast() == this) ? true : false;
    Object *next = params->m_contentLayer->GetNext(this);
    bool nextIsBarline = (next && next->Is(BARLINE)) ? true : false;

    // See what if we create proper measures and what to do with the barLine
    bool convertToMeasured = params->m_doc->GetOptions()->m_mensuralToMeasure.GetValue();

    if (convertToMeasured) {
        // barLine object will be deleted
        params->m_targetMeasure->SetRight(this->GetForm());
    }
    else {
        this->MoveItselfTo(params->m_targetLayer);
    }

    // Now we can return if this the end barLine
    if (isLast || nextIsBarline) return FUNCTOR_SIBLINGS;

    for (auto const &staffN : params->m_staffNs) {
        // The barline is missing in at least one of the staves - do not break here
        if (!m_alignment->HasAlignmentReference(staffN)) {
            // LogDebug("BarLine not on all staves %d %s", params->m_targetStaff->GetN(), this->GetClassName().c_str());
            return FUNCTOR_SIBLINGS;
        }
    }

    // Make a segment break
    // First case: new need to add a new measure segment (e.g, first pass)
    if (params->m_targetSubSystem->GetChildCount() <= params->m_segmentIdx) {
        params->m_targetMeasure = new Measure(convertToMeasured);
        if (convertToMeasured) {
            params->m_targetMeasure->SetN(StringFormat("%d", params->m_segmentTotal + 1 + params->m_segmentIdx));
        }
        params->m_targetSubSystem->AddChild(params->m_targetMeasure);
        // Add a staff with same attribute as in the previous segment
        params->m_targetStaff = new Staff(*params->m_targetStaff);
        params->m_targetStaff->ClearChildren();
        params->m_targetStaff->CloneReset();
        params->m_targetMeasure->AddChild(params->m_targetStaff);
        // Add a layer also with the same attribute as in the previous segment
        params->m_targetLayer = new Layer(*params->m_targetLayer);
        params->m_targetLayer->ClearChildren();
        params->m_targetLayer->CloneReset();
        params->m_targetStaff->AddChild(params->m_targetLayer);
    }
    // Second case: retrieve the approrpiate segment
    else {
        params->m_targetMeasure = dynamic_cast<Measure *>(params->m_targetSubSystem->GetChild(params->m_segmentIdx));
        // It must be there
        assert(params->m_targetMeasure);

        // Look if we already have the staff (e.g., with more than one layer)
        AttNIntegerComparison comparisonStaffN(STAFF, params->m_targetStaff->GetN());
        Staff *staff = dynamic_cast<Staff *>(params->m_targetMeasure->FindDescendantByComparison(&comparisonStaffN));
        if (!staff) {
            staff = new Staff(*params->m_targetStaff);
            staff->ClearChildren();
            staff->CloneReset();
            params->m_targetMeasure->AddChild(staff);
        }
        params->m_targetStaff = staff;

        // Add a new layer as the new target
        params->m_targetLayer = new Layer(*params->m_targetLayer);
        params->m_targetLayer->ClearChildren();
        params->m_targetLayer->CloneReset();
        params->m_targetStaff->AddChild(params->m_targetLayer);
    }
    params->m_segmentIdx++;
    // LogDebug("BarLine staff %d - Idx %d", params->m_targetStaff->GetN(), params->m_segmentIdx);

    return FUNCTOR_SIBLINGS;
}
*/
} // namespace vrv

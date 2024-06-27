/////////////////////////////////////////////////////////////////////////////
// Name:        view_neume.cpp
// Author:      Andrew Tran, Juliette Regimbal
// Created:     2017
// Copyright (c) Author and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <cassert>
#include <math.h>

//----------------------------------------------------------------------------

#include "devicecontext.h"
#include "divline.h"
#include "doc.h"
#include "layer.h"
#include "layerelement.h"
#include "liquescent.h"
#include "mrpt.h"
#include "nc.h"
#include "neume.h"
#include "note.h"
#include "smufl.h"
#include "staff.h"
#include "syllable.h"
#include "vrv.h"
#include "zone.h"

namespace vrv {

struct NcDrawingParams {
    wchar_t fontNo = SMUFL_E990_chantPunctum;
    float xOffset = 0;
    float yOffset = 0;
};

void View::DrawSyllable(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(layer);
    assert(staff);
    assert(measure);

    Syllable *syllable = dynamic_cast<Syllable *>(element);
    assert(syllable);

    /******************************************************************/
    // Start the Beam graphic and draw the children

    dc->StartGraphic(element, "", element->GetID());

    /******************************************************************/
    // Draw the children

    this->DrawLayerChildren(dc, syllable, layer, staff, measure);

    dc->EndGraphic(element, this);
}

void View::DrawLiquescent(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(layer);
    assert(staff);
    assert(measure);

    NcDrawingParams params[3];

    dc->StartGraphic(element, "", element->GetID());

    Nc *nc = dynamic_cast<Nc *>(element->GetParent());

    if (nc->GetCurve() == curvatureDirection_CURVE_c) {
        params[0].fontNo = SMUFL_E9BE_chantConnectingLineAsc3rd;
        params[1].fontNo = SMUFL_E995_chantAuctumDesc;
        params[2].fontNo = SMUFL_E9BE_chantConnectingLineAsc3rd;
        params[2].xOffset = 0.8;
        params[0].yOffset = -1.5;
        params[2].yOffset = -1.75;
    }
    else if (nc->GetCurve() == curvatureDirection_CURVE_a) {
        params[0].fontNo = SMUFL_E9BE_chantConnectingLineAsc3rd;
        params[1].fontNo = SMUFL_E994_chantAuctumAsc;
        params[2].fontNo = SMUFL_E9BE_chantConnectingLineAsc3rd;
        params[2].xOffset = 0.8;
        params[0].yOffset = 0.5;
        params[2].yOffset = 0.75;
    }

    const int noteHeight
        = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / NOTE_HEIGHT_TO_STAFF_SIZE_RATIO);
    const int noteWidth
        = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / NOTE_WIDTH_TO_STAFF_SIZE_RATIO);

    int noteX = nc->GetDrawingX();
    int noteY = nc->GetDrawingY();

    if (staff->HasDrawingRotation()) {
        noteY -= staff->GetDrawingRotationOffsetFor(noteX);
    }

    for (int i = 0; i < static_cast<int>(sizeof(params)); ++i) {
        DrawSmuflCode(dc, noteX + params[i].xOffset * noteWidth, noteY + params[i].yOffset * noteHeight,
            params[i].fontNo, staff->m_drawingStaffSize, false, true);
    }

    dc->EndGraphic(element, this);
}

void View::DrawNc(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(layer);
    assert(staff);
    assert(measure);

    Nc *nc = dynamic_cast<Nc *>(element);
    assert(nc);

    if (m_options->m_neumeAsNote.GetValue()) {
        DrawNcAsNotehead(dc, nc, layer, staff, measure);
        return;
    }

    NcDrawingParams params;

    dc->StartGraphic(element, "", element->GetID());

    /******************************************************************/

    Neume *neume = vrv_cast<Neume *>(nc->GetFirstAncestor(NEUME));
    assert(neume);
    int position = neume->GetChildIndex(element);

    // Check if nc is part of a ligature or is an inclinatum
    if (nc->HasTilt() && nc->GetTilt() == COMPASSDIRECTION_se) {
        params.fontNo = SMUFL_E991_chantPunctumInclinatum;
    }
    else if (nc->GetLigated() == BOOLEAN_true) {
        int pitchDifference = 0;
        bool isFirst;
        int ligCount = neume->GetLigatureCount(position);

        if (ligCount % 2 == 0) {
            isFirst = false;
            Nc *lastNc = dynamic_cast<Nc *>(neume->GetChild(position > 0 ? position - 1 : 0));
            assert(lastNc);
            pitchDifference = nc->PitchDifferenceTo(lastNc);
            params.xOffset = -1;
            params.yOffset = -pitchDifference;
        }
        else {
            isFirst = true;
            Object *nextSibling = neume->GetChild(position + 1);
            if (nextSibling != NULL) {
                Nc *nextNc = dynamic_cast<Nc *>(nextSibling);
                assert(nextNc);
                pitchDifference = nextNc->PitchDifferenceTo(nc);
                params.yOffset = pitchDifference;
            }
        }

        // set the glyph
        switch (pitchDifference) {
            case -1: params.fontNo = isFirst ? SMUFL_E9B4_chantEntryLineAsc2nd : SMUFL_E9B9_chantLigaturaDesc2nd; break;
            case -2: params.fontNo = isFirst ? SMUFL_E9B5_chantEntryLineAsc3rd : SMUFL_E9BA_chantLigaturaDesc3rd; break;
            case -3: params.fontNo = isFirst ? SMUFL_E9B6_chantEntryLineAsc4th : SMUFL_E9BB_chantLigaturaDesc4th; break;
            case -4: params.fontNo = isFirst ? SMUFL_E9B7_chantEntryLineAsc5th : SMUFL_E9BC_chantLigaturaDesc5th; break;
            default: break;
        }
    }

    // If the nc is supposed to be a virga and currently is being rendered as a punctum
    // change it to a virga
    if (nc->GetTilt() == COMPASSDIRECTION_s && params.fontNo == SMUFL_E990_chantPunctum) {
        params.fontNo = SMUFL_E996_chantPunctumVirga;
    }

    else if (nc->GetTilt() == COMPASSDIRECTION_n && params.fontNo == SMUFL_E990_chantPunctum) {
        params.fontNo = SMUFL_E997_chantPunctumVirgaReversed;
    }

    const int noteHeight
        = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / NOTE_HEIGHT_TO_STAFF_SIZE_RATIO);
    const int noteWidth
        = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / NOTE_WIDTH_TO_STAFF_SIZE_RATIO);

    int noteX = nc->GetDrawingX();
    int noteY = nc->GetDrawingY();

    if (nc->HasFacs() && m_doc->IsNeumeLines()) {
        params.xOffset = 0;
    }
    // Not sure about this if - the nc pname and oct are going to be ignored
    else if (neume->HasFacs() && m_doc->IsNeumeLines()) {
        noteY = staff->GetDrawingY();
        noteX = neume->GetDrawingX() + position * noteWidth;
    }

    if (staff->HasDrawingRotation()) {
        noteY -= staff->GetDrawingRotationOffsetFor(noteX);
    }

    if (!nc->HasCurve()) {
        DrawSmuflCode(dc, noteX + params.xOffset * noteWidth, noteY + params.yOffset * noteHeight, params.fontNo,
            staff->m_drawingStaffSize, false, true);
    }

    // Draw the children
    this->DrawLayerChildren(dc, nc, layer, staff, measure);

    dc->EndGraphic(element, this);
}

void View::DrawNeume(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(layer);
    assert(staff);
    assert(measure);

    Neume *neume = dynamic_cast<Neume *>(element);
    assert(neume);

    /******************************************************************/
    // Start the Neume graphic and draw the children

    dc->StartGraphic(element, "", element->GetID());
    this->DrawLayerChildren(dc, neume, layer, staff, measure);

    if (m_options->m_neumeAsNote.GetValue()) {

        Nc *first = vrv_cast<Nc *>(neume->GetFirst(NC));
        Nc *last = vrv_cast<Nc *>(neume->GetLast(NC));

        if (first != last) {

            const int unit = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
            const int lineWidth = m_doc->GetOptions()->m_octaveLineThickness.GetValue() * unit;

            int x1 = first->GetDrawingX();
            int x2 = last->GetDrawingX();
            int y = staff->GetDrawingY();

            const int maxNcY = std::max(first->GetDrawingY(), last->GetDrawingY());
            y = std::max(y, maxNcY + unit);
            y += 2 * unit;

            x1 += lineWidth / 2;
            x2 += 2 * last->GetDrawingRadius(m_doc) - lineWidth / 2;

            dc->SetPen(m_currentColor, lineWidth, AxSOLID, 0, 0, AxCAP_BUTT, AxJOIN_MITER);

            dc->DrawLine(ToDeviceContextX(x1), ToDeviceContextY(y), ToDeviceContextX(x2), ToDeviceContextY(y));
            dc->DrawLine(ToDeviceContextX(x1), ToDeviceContextY(y + lineWidth / 2), ToDeviceContextX(x1),
                ToDeviceContextY(y - unit));
            dc->DrawLine(ToDeviceContextX(x2), ToDeviceContextY(y + lineWidth / 2), ToDeviceContextX(x2),
                ToDeviceContextY(y - unit));

            dc->ResetPen();
        }
    }

    dc->EndGraphic(element, this);
}

void View::DrawNcAsNotehead(DeviceContext *dc, Nc *nc, Layer *layer, Staff *staff, Measure *measure)
{
    /******************************************************************/
    // Start the Neume graphic and draw the children

    dc->StartGraphic(nc, "", nc->GetID());

    const int noteX = nc->GetDrawingX();
    const int noteY = nc->GetDrawingY();

    bool cueSize = false;
    if (nc->FindDescendantByType(LIQUESCENT)) {
        cueSize = true;
    }

    this->DrawSmuflCode(dc, noteX, noteY, SMUFL_E0A4_noteheadBlack, staff->m_drawingStaffSize, cueSize, true);

    dc->EndGraphic(nc, this);
}

void View::DrawDivLine(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    DivLine *divLine = dynamic_cast<DivLine *>(element);
    assert(divLine);

    dc->StartGraphic(element, "", element->GetID());

    int sym = 0;

    switch (divLine->GetForm()) {
        case divLineLog_FORM_minima: sym = SMUFL_E8F3_chantDivisioMinima; break;
        case divLineLog_FORM_maior: sym = SMUFL_E8F4_chantDivisioMaior; break;
        case divLineLog_FORM_maxima: sym = SMUFL_E8F5_chantDivisioMaxima; break;
        case divLineLog_FORM_finalis: sym = SMUFL_E8F6_chantDivisioFinalis; break;
        case divLineLog_FORM_caesura: sym = SMUFL_E8F8_chantCaesura; break;
        case divLineLog_FORM_virgula: sym = SMUFL_E8F7_chantVirgula; break;
        default: break;
    }

    int x, y;
    x = divLine->GetDrawingX();
    y = staff->GetDrawingY();

    y -= (m_doc->GetDrawingUnit(staff->m_drawingStaffSize)) * 3;

    if (staff->HasDrawingRotation()) {
        y -= staff->GetDrawingRotationOffsetFor(x);
    }

    DrawSmuflCode(dc, x, y, sym, staff->m_drawingStaffSize, false, true);

    dc->EndGraphic(element, this);
}

void View::DrawOriscus(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(layer);
    assert(staff);
    assert(measure);

    NcDrawingParams params[3];

    dc->StartGraphic(element, "", element->GetID());

    dc->EndGraphic(element, this);
}

void View::DrawQuilisma(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(layer);
    assert(staff);
    assert(measure);

    NcDrawingParams params[3];

    dc->StartGraphic(element, "", element->GetID());

    dc->EndGraphic(element, this);
}

} // namespace vrv

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

    struct drawingParams {
        wchar_t fontNo = SMUFL_E990_chantPunctum;
        wchar_t fontNoLiq[5] = {};
        double xOffset = 0;
        double yOffset = 0;
        double xOffsetLiq[5] = { 0, 0, 0, 0, 0 };
        double yOffsetLiq[5] = { 0, 0, 0, 0, 0 };
    };
    std::vector<drawingParams> params;
    params.push_back(drawingParams());

    dc->StartGraphic(element, "", element->GetID());

    /******************************************************************/

    // Intializing necessary variables
    Clef *clef = layer->GetClef(element);
    int staffSize = m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
    int staffLineNumber = staff->m_drawingLines;
    int clefLine = clef->GetLine();

    Neume *neume = vrv_cast<Neume *>(nc->GetFirstAncestor(NEUME));
    assert(neume);
    int position = neume->GetChildIndex(element);

    // Check if nc is part of a ligature or is an inclinatum
    if (nc->HasTilt() && nc->GetTilt() == COMPASSDIRECTION_se) {
        params.at(0).fontNo = SMUFL_E991_chantPunctumInclinatum;
    }
    else if (nc->GetLigated() == BOOLEAN_true) {
        int pitchDifference = 0;
        bool isFirst;
        // Check if this is the first or second part of a ligature
        // Object *nextSibling = neume->GetChild(position + 1);
        // if (nextSibling != NULL) {
        //     Nc *nextNc = dynamic_cast<Nc *>(nextSibling);
        //     assert(nextNc);
        //     if (nextNc->GetLigated() == BOOLEAN_true) { // first part of the ligature
        //         isFirst = true;
        //         pitchDifference = nextNc->PitchDifferenceTo(nc);
        //         params.at(0).yOffset = pitchDifference;
        //     }
        //     else {
        //         isFirst = false;
        //     }
        // }
        // else {
        //     isFirst = false;
        // }
        int ligCount = neume->GetLigatureCount(position);

        if (ligCount % 2 == 0) {
            isFirst = false;
            Nc *lastNc = dynamic_cast<Nc *>(neume->GetChild(position > 0 ? position - 1 : 0));
            assert(lastNc);
            pitchDifference = nc->PitchDifferenceTo(lastNc);
            params.at(0).xOffset = -1;
            params.at(0).yOffset = -pitchDifference;
        }
        else {
            isFirst = true;
            Object *nextSibling = neume->GetChild(position + 1);
            if (nextSibling != NULL) {
                Nc *nextNc = dynamic_cast<Nc *>(nextSibling);
                assert(nextNc);
                pitchDifference = nextNc->PitchDifferenceTo(nc);
                params.at(0).yOffset = pitchDifference;
            }
        }

        // if (!isFirst) { // still need to get pitchDifference
        //     Nc *lastnc = dynamic_cast<Nc *>(neume->GetChild(position > 0 ? position - 1 : 0));
        //     assert(lastnc);
        //     pitchDifference = nc->PitchDifferenceTo(lastnc);
        //     params.at(0).xOffset = -1;
        //     params.at(0).yOffset = -pitchDifference;
        // }

        // set the glyph
        switch (pitchDifference) {
            case -1:
                params.at(0).fontNo = isFirst ? SMUFL_E9B4_chantEntryLineAsc2nd : SMUFL_E9B9_chantLigaturaDesc2nd;
                break;
            case -2:
                params.at(0).fontNo = isFirst ? SMUFL_E9B5_chantEntryLineAsc3rd : SMUFL_E9BA_chantLigaturaDesc3rd;
                break;
            case -3:
                params.at(0).fontNo = isFirst ? SMUFL_E9B6_chantEntryLineAsc4th : SMUFL_E9BB_chantLigaturaDesc4th;
                break;
            case -4:
                params.at(0).fontNo = isFirst ? SMUFL_E9B7_chantEntryLineAsc5th : SMUFL_E9BC_chantLigaturaDesc5th;
                break;
            default: break;
        }
    }

    // If the nc is supposed to be a virga and currently is being rendered as a punctum
    // change it to a virga
    if (nc->GetTilt() == COMPASSDIRECTION_s && params.at(0).fontNo == SMUFL_E990_chantPunctum) {
        params.at(0).fontNo = SMUFL_E996_chantPunctumVirga;
    }

    else if (nc->GetTilt() == COMPASSDIRECTION_n && params.at(0).fontNo == SMUFL_E990_chantPunctum) {
        params.at(0).fontNo = SMUFL_E997_chantPunctumVirgaReversed;
    }

    else if (nc->GetCurve() == curvatureDirection_CURVE_c) {
        params.at(0).fontNoLiq[0] = SMUFL_E9BE_chantConnectingLineAsc3rd;
        params.at(0).fontNoLiq[1] = SMUFL_EB92_staffPosRaise3;
        params.at(0).fontNoLiq[2] = SMUFL_E995_chantAuctumDesc;
        params.at(0).fontNoLiq[3] = SMUFL_EB91_staffPosRaise2;
        params.at(0).fontNoLiq[4] = SMUFL_E9BE_chantConnectingLineAsc3rd;
        params.at(0).xOffsetLiq[4] = 0.8;
        params.at(0).yOffsetLiq[0] = -1.5;
        params.at(0).yOffsetLiq[4] = -1.75;
    }
    else if (nc->GetCurve() == curvatureDirection_CURVE_a) {
        params.at(0).fontNoLiq[0] = SMUFL_E9BE_chantConnectingLineAsc3rd;
        params.at(0).fontNoLiq[1] = SMUFL_EB98_staffPosLower1;
        params.at(0).fontNoLiq[2] = SMUFL_E994_chantAuctumAsc;
        params.at(0).fontNoLiq[3] = SMUFL_EB99_staffPosLower2;
        params.at(0).fontNoLiq[4] = SMUFL_E9BE_chantConnectingLineAsc3rd;
        params.at(0).xOffsetLiq[4] = 0.8;
        params.at(0).yOffsetLiq[0] = 0.5;
        params.at(0).yOffsetLiq[4] = 0.75;
    }

    const int noteHeight
        = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / NOTE_HEIGHT_TO_STAFF_SIZE_RATIO);
    const int noteWidth
        = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / NOTE_WIDTH_TO_STAFF_SIZE_RATIO);
    int noteY, noteX;
    int yValue;
    if (nc->HasFacs() && m_doc->IsFacs()) {
        noteY = ToLogicalY(staff->GetDrawingY());
        noteX = nc->GetDrawingX();
        params.at(0).xOffset = 0;
    }
    else if (neume->HasFacs() && m_doc->IsFacs()) {
        noteY = ToLogicalY(staff->GetDrawingY());
        noteX = neume->GetDrawingX() + position * noteWidth;
    }
    else {
        noteX = element->GetDrawingX();
        noteY = element->GetDrawingY();
    }
    // Calculating proper y offset based on pname, clef, staff, and staff rotate
    int clefYPosition = noteY - (staffSize * (staffLineNumber - clefLine));
    int pitchOffset = 0;

    // The default octave = 3, but the actual octave is calculated by
    // taking into account the displacement of the clef
    int clefOctave = 3;
    if (clef->GetDis() && clef->GetDisPlace()) {
        clefOctave += (clef->GetDisPlace() == STAFFREL_basic_above ? 1 : -1) * (clef->GetDis() / 7);
    }
    int octaveOffset = (nc->GetOct() - clefOctave) * ((staffSize / 2) * 7);
    int rotateOffset;
    if (m_doc->IsFacs() && (staff->GetDrawingRotate() != 0)) {
        double deg = staff->GetDrawingRotate();
        int xDiff = noteX - staff->GetDrawingX();
        rotateOffset = int(xDiff * tan(deg * M_PI / 180.0));
    }
    else {
        rotateOffset = 0;
    }

    if (nc->HasLoc()) {
        yValue = noteY + (nc->GetLoc() - 2 * (staffLineNumber - 1)) * (staffSize / 2);
    }
    else {
        if (clef->GetShape() == CLEFSHAPE_C) {
            pitchOffset = (nc->GetPname() - 1) * (staffSize / 2);
        }
        else if (clef->GetShape() == CLEFSHAPE_F) {
            pitchOffset = (nc->GetPname() - 4) * (staffSize / 2);
        }
        yValue = clefYPosition + pitchOffset + octaveOffset - rotateOffset;
    }

    for (auto it = params.begin(); it != params.end(); it++) {
        if (nc->GetCurve() == curvatureDirection_CURVE_a || nc->GetCurve() == curvatureDirection_CURVE_c) {
            for (int i = 0; i < static_cast<int>(sizeof(params.at(0).fontNoLiq)); i++) {
                DrawSmuflCode(dc, noteX + it->xOffsetLiq[i] * noteWidth, yValue + it->yOffsetLiq[i] * noteHeight,
                    it->fontNoLiq[i], staff->m_drawingStaffSize, false, true);
            }
        }
        else {
            DrawSmuflCode(dc, noteX + it->xOffset * noteWidth, yValue + it->yOffset * noteHeight, it->fontNo,
                staff->m_drawingStaffSize, false, true);
        }
    }

    // adjust facsimile values of element based on where it is rendered if necessary
    if (m_doc->IsFacs() && element->HasFacs()) {
        FacsimileInterface *fi = element->GetFacsimileInterface();
        fi->GetZone()->SetUlx(noteX);
        fi->GetZone()->SetUly(ToDeviceContextY(yValue));
        fi->GetZone()->SetLrx(noteX + noteWidth);
        fi->GetZone()->SetLry(ToDeviceContextY(yValue - noteHeight));
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

    // int x = divLine->GetDrawingX();
    // int y = divLine->GetDrawingY();

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
    if (m_doc->IsFacs() && (divLine->HasFacs())) {
        x = divLine->GetDrawingX();
        y = ToLogicalY(staff->GetDrawingY());
    }
    else {
        x = element->GetDrawingX();
        y = element->GetDrawingY();
        y -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    }

    y -= (m_doc->GetDrawingUnit(staff->m_drawingStaffSize)) * 3;

    int rotateOffset;
    if (m_doc->IsFacs() && (staff->GetDrawingRotate() != 0)) {
        double deg = staff->GetDrawingRotate();
        int xDiff = x - staff->GetDrawingX();
        rotateOffset = int(xDiff * tan(deg * M_PI / 180.0));
    }
    else {
        rotateOffset = 0;
    }

    y -= rotateOffset;

    DrawSmuflCode(dc, x, y, sym, staff->m_drawingStaffSize, false, true);

    dc->EndGraphic(element, this);
}
} // namespace vrv

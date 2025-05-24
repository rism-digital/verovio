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

void View::DrawLiquescent(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(layer);
    assert(staff);
    assert(measure);

    dc->StartGraphic(element, "", element->GetID());

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
        this->DrawNcAsNotehead(dc, nc, layer, staff, measure);
        return;
    }

    dc->StartGraphic(element, "", element->GetID());

    this->DrawNcGlyphs(dc, nc, staff);

    /******************************************************************/

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

            dc->SetPen(lineWidth, PEN_SOLID, 0, 0, LINECAP_BUTT, LINEJOIN_MITER);

            dc->DrawLine(this->ToDeviceContextX(x1), this->ToDeviceContextY(y), this->ToDeviceContextX(x2),
                this->ToDeviceContextY(y));
            dc->DrawLine(this->ToDeviceContextX(x1), this->ToDeviceContextY(y + lineWidth / 2),
                this->ToDeviceContextX(x1), this->ToDeviceContextY(y - unit));
            dc->DrawLine(this->ToDeviceContextX(x2), this->ToDeviceContextY(y + lineWidth / 2),
                this->ToDeviceContextX(x2), this->ToDeviceContextY(y - unit));

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

    this->DrawSmuflCode(dc, x, y, sym, staff->m_drawingStaffSize, false, true);

    dc->EndGraphic(element, this);
}

void View::DrawOriscus(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(layer);
    assert(staff);
    assert(measure);

    dc->StartGraphic(element, "", element->GetID());

    dc->EndGraphic(element, this);
}

void View::DrawQuilisma(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(layer);
    assert(staff);
    assert(measure);

    dc->StartGraphic(element, "", element->GetID());

    dc->EndGraphic(element, this);
}

void View::DrawNcGlyphs(DeviceContext *dc, Nc *nc, Staff *staff)
{
    assert(dc);
    assert(nc);
    assert(staff);

    int ncX = nc->GetDrawingX();
    int ncY = nc->GetDrawingY();

    if (staff->HasDrawingRotation()) {
        ncY -= staff->GetDrawingRotationOffsetFor(ncX);
    }

    for (auto &glyph : nc->m_drawingGlyphs) {
        this->DrawSmuflCode(
            dc, ncX + glyph.m_xOffset, ncY + glyph.m_yOffset, glyph.m_fontNo, staff->m_drawingStaffSize, false, true);
    }
}

} // namespace vrv

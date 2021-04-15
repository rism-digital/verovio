/////////////////////////////////////////////////////////////////////////////
// Name:        view_tab.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <iostream>
#include <math.h>

//----------------------------------------------------------------------------

#include "devicecontext.h"
#include "doc.h"
#include "note.h"
#include "options.h"
#include "rend.h"
#include "smufl.h"
#include "staff.h"
#include "system.h"
#include "tabgrp.h"
#include "tabrhythm.h"
#include "text.h"
#include "vrv.h"

namespace vrv {

void View::DrawTabGrp(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);

    TabGrp *tabGrp = dynamic_cast<TabGrp *>(element);
    assert(tabGrp);

    dc->StartGraphic(tabGrp, "", tabGrp->GetUuid());

    // Draw children (rhyhtm, notes)
    DrawLayerChildren(dc, tabGrp, layer, staff, measure);

    dc->EndGraphic(tabGrp, this);
}

void View::DrawTabNote(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);

    Note *note = dynamic_cast<Note *>(element);
    assert(note);

    // TabGrp *tabGrp = note->IsTabGrpNote();
    // assert(tabGrp);

    dc->StartGraphic(note, "", note->GetUuid());

    int x = element->GetDrawingX();
    int y = element->GetDrawingY();

    int glyphSize = staff->m_drawingStaffSize / TABLATURE_STAFF_RATIO;
    bool drawingCueSize = false;

    if (staff->m_drawingNotationType == NOTATIONTYPE_tab_guitar) {

        std::wstring fret = note->GetTabFretString(staff->m_drawingNotationType);

        FontInfo fretTxt;
        // global styling for fret is missing
        // if (!dc->UseGlobalStyling()) {
        fretTxt.SetFaceName("Times");
        fretTxt.SetWeight(FONTWEIGHT_bold);
        //}

        TextDrawingParams params;
        params.m_x = x;
        params.m_y = y;
        params.m_pointSize = m_doc->GetDrawingLyricFont(glyphSize)->GetPointSize() * 3 / 5;
        fretTxt.SetPointSize(params.m_pointSize);

        dc->SetBrush(m_currentColour, AxSOLID);
        dc->SetFont(&fretTxt);

        // TextExtend extend;
        // dc->GetTextExtent(fret, &extend, false);
        // params.m_x -= (extend.m_width / 2);

        params.m_x += (m_doc->GetTextGlyphWidth(L'0', &fretTxt, drawingCueSize));
        params.m_y -= (m_doc->GetTextGlyphHeight(L'0', &fretTxt, drawingCueSize) / 2);

        dc->StartText(ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y), HORIZONTALALIGNMENT_center);
        DrawTextString(dc, fret, params);
        dc->EndText();

        dc->ResetFont();
    }
    else {

        std::wstring fret = note->GetTabFretString(staff->m_drawingNotationType);
        wchar_t code = (staff->m_drawingNotationType == NOTATIONTYPE_tab_lute_french)
                ? SMUFL_EBC0_luteFrenchFretA
                : SMUFL_EBE0_luteItalianFret0;
        int radius = m_doc->GetGlyphWidth(SMUFL_E0A4_noteheadBlack, glyphSize, false);
        y -= (m_doc->GetGlyphHeight(code, glyphSize, drawingCueSize) / 2);
        x += radius - (m_doc->GetGlyphWidth(code, glyphSize, drawingCueSize) / 2);

        dc->SetFont(m_doc->GetDrawingSmuflFont(glyphSize, false));
        DrawSmuflString(dc, x, y, fret, HORIZONTALALIGNMENT_center, glyphSize);
        dc->ResetFont();
    }

    // Draw children (nothing yet)
    DrawLayerChildren(dc, note, layer, staff, measure);

    dc->EndGraphic(note, this);
}

void View::DrawTabRhythm(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);

    TabRhythm *tabRhythm = dynamic_cast<TabRhythm *>(element);
    assert(tabRhythm);

    TabGrp *tabGrp = dynamic_cast<TabGrp *>(tabRhythm->GetFirstAncestor(TABGRP));
    assert(tabGrp);

    dc->StartGraphic(tabRhythm, "", tabRhythm->GetUuid());

    int x = element->GetDrawingX();
    int y = element->GetDrawingY();
    y += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 1.5;
    int drawingDur = (tabGrp->GetDurGes() != DURATION_NONE) ? tabGrp->GetActualDurGes() : tabGrp->GetActualDur();
    int glyphSize = staff->m_drawingStaffSize / TABLATURE_STAFF_RATIO;
    int radius = m_doc->GetGlyphWidth(SMUFL_E0A4_noteheadBlack, glyphSize, false) / 2;
    x += radius;

    int symc = 0;
    switch (drawingDur) {
        case DUR_2: symc = SMUFL_EBA7_luteDurationWhole; break;
        case DUR_4: symc = SMUFL_EBA8_luteDurationHalf; break;
        case DUR_8: symc = SMUFL_EBA9_luteDurationQuarter; break;
        case DUR_16: symc = SMUFL_EBAA_luteDuration8th; break;
        case DUR_32: symc = SMUFL_EBAB_luteDuration16th; break;
        default: symc = SMUFL_EBA9_luteDurationQuarter;
    }

    DrawSmuflCode(dc, x, y, symc, glyphSize, true);

    int i;
    if (tabGrp->HasDots()) {
        y += m_doc->GetDrawingUnit(glyphSize) * 0.5;
        x += m_doc->GetDrawingUnit(glyphSize);
        for (i = 0; i < tabGrp->GetDots(); ++i) {
            DrawDot(dc, x, y, glyphSize / 2);
            // HARDCODED
            x += m_doc->GetDrawingUnit(glyphSize) * 0.75;
        }
    }

    // Draw children (nothing yet)
    DrawLayerChildren(dc, tabRhythm, layer, staff, measure);

    dc->EndGraphic(tabRhythm, this);
}

} // namespace vrv

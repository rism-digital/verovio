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
#include "tabdursym.h"
#include "tabgrp.h"
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

    int glyphSize = staff->GetDrawingStaffNotationSize();
    bool drawingCueSize = false;

    if (staff->m_drawingNotationType == NOTATIONTYPE_tab_guitar) {

        std::wstring fret = note->GetTabFretString(staff->m_drawingNotationType);

        FontInfo fretTxt;
        fretTxt.SetFaceName("Times");

        TextDrawingParams params;
        params.m_x = x;
        params.m_y = y;
        params.m_pointSize = m_doc->GetDrawingLyricFont(glyphSize)->GetPointSize() * 4 / 5;
        fretTxt.SetPointSize(params.m_pointSize);

        dc->SetBrush(m_currentColour, AxSOLID);
        dc->SetFont(&fretTxt);

        params.m_y -= (m_doc->GetTextGlyphHeight(L'0', &fretTxt, drawingCueSize) / 2);

        dc->StartText(ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y), HORIZONTALALIGNMENT_center);
        DrawTextString(dc, fret, params);
        dc->EndText();

        dc->ResetFont();
    }
    else {

        std::wstring fret = note->GetTabFretString(staff->m_drawingNotationType);
        // Center for italian tablature
        if (staff->IsTabLuteItalian()) {
            y -= (m_doc->GetGlyphHeight(SMUFL_EBE0_luteItalianFret0, glyphSize, drawingCueSize) / 2);
        }
        // Above the line for french tablature
        else if (staff->IsTabLuteFrench()) {
            y -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize)
                - m_doc->GetDrawingStaffLineWidth(staff->m_drawingStaffSize);
        }

        dc->SetFont(m_doc->GetDrawingSmuflFont(glyphSize, false));
        DrawSmuflString(dc, x, y, fret, HORIZONTALALIGNMENT_center, glyphSize);
        dc->ResetFont();
    }

    // Draw children (nothing yet)
    DrawLayerChildren(dc, note, layer, staff, measure);

    dc->EndGraphic(note, this);
}

void View::DrawTabDurSym(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);

    TabDurSym *tabDurSym = dynamic_cast<TabDurSym *>(element);
    assert(tabDurSym);

    TabGrp *tabGrp = dynamic_cast<TabGrp *>(tabDurSym->GetFirstAncestor(TABGRP));
    assert(tabGrp);

    dc->StartGraphic(tabDurSym, "", tabDurSym->GetUuid());

    int x = element->GetDrawingX();
    int y = element->GetDrawingY();

    int drawingDur = (tabGrp->GetDurGes() != DURATION_NONE) ? tabGrp->GetActualDurGes() : tabGrp->GetActualDur();
    int glyphSize = staff->GetDrawingStaffNotationSize();

    // We only need to draw the stems
    // Do we also need to draw the dots?
    if (tabGrp->IsInBeam()) {
        const int height = m_doc->GetGlyphHeight(SMUFL_EBA8_luteDurationHalf, glyphSize, true);
        DrawFilledRectangle(dc, x - m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) / 2, y,
            x + m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) / 2, y + height);
    }
    else {
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

        if (tabGrp->HasDots()) {
            y += m_doc->GetDrawingUnit(glyphSize) * 0.5;
            x += m_doc->GetDrawingUnit(glyphSize);
            for (int i = 0; i < tabGrp->GetDots(); ++i) {
                DrawDot(dc, x, y, glyphSize * 2 / 3);
                // HARDCODED
                x += m_doc->GetDrawingUnit(glyphSize) * 0.75;
            }
        }
    }

    // Draw children (nothing yet)
    DrawLayerChildren(dc, tabDurSym, layer, staff, measure);

    dc->EndGraphic(tabDurSym, this);
}

} // namespace vrv

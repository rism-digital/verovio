/////////////////////////////////////////////////////////////////////////////
// Name:        view_tab.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <cassert>
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
#include "stem.h"
#include "system.h"
#include "tabdursym.h"
#include "tabgrp.h"
#include "text.h"
#include "vrv.h"

namespace vrv {

void View::DrawTabClef(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    Clef *clef = vrv_cast<Clef *>(element);
    assert(clef);

    const int glyphSize = staff->GetDrawingStaffNotationSize();

    int x, y;
    y = staff->GetDrawingY();
    x = element->GetDrawingX();

    char32_t sym = clef->GetClefGlyph(staff->m_drawingNotationType);

    if (sym == 0) {
        clef->SetEmptyBB();
        return;
    }

    y -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * (staff->m_drawingLines - 1);

    dc->StartGraphic(element, "", element->GetID());

    this->DrawSmuflCode(dc, x, y, sym, glyphSize, false);

    // Possibly draw enclosing brackets
    this->DrawClefEnclosing(dc, clef, staff, sym, x, y);

    dc->EndGraphic(element, this);
}

void View::DrawTabGrp(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);

    TabGrp *tabGrp = dynamic_cast<TabGrp *>(element);
    assert(tabGrp);

    dc->StartGraphic(tabGrp, "", tabGrp->GetID());

    // Draw children (rhyhtm, notes)
    this->DrawLayerChildren(dc, tabGrp, layer, staff, measure);

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

    dc->StartGraphic(note, "", note->GetID());

    int x = element->GetDrawingX();
    int y = element->GetDrawingY();

    int glyphSize = staff->GetDrawingStaffNotationSize();
    bool drawingCueSize = false;

    if (staff->m_drawingNotationType == NOTATIONTYPE_tab_guitar) {

        std::u32string fret = note->GetTabFretString(staff->m_drawingNotationType);

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
        this->DrawTextString(dc, fret, params);
        dc->EndText();

        dc->ResetFont();
    }
    else {

        std::u32string fret = note->GetTabFretString(staff->m_drawingNotationType);
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
        this->DrawSmuflString(dc, x, y, fret, HORIZONTALALIGNMENT_center, glyphSize);
        dc->ResetFont();
    }

    // Draw children (nothing yet)
    this->DrawLayerChildren(dc, note, layer, staff, measure);

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

    dc->StartGraphic(tabDurSym, "", tabDurSym->GetID());

    int x = element->GetDrawingX();
    int y = element->GetDrawingY();

    const int glyphSize = staff->GetDrawingStaffNotationSize();
    const int drawingDur = (tabGrp->GetDurGes() != DURATION_NONE) ? tabGrp->GetActualDurGes() : tabGrp->GetActualDur();

    // For beam and guitar notation, stem are drawn through the child Stem
    if (!tabGrp->IsInBeam() && !staff->IsTabGuitar()) {
        int symc = 0;
        switch (drawingDur) {
                // TODO SMUFL_EBA6_luteDurationDoubleWhole is defined by SMUFL but not yet implemented in Verovio
                /* case DUR_1: symc = SMUFL_EBA6_luteDurationDoubleWhole; break; // 1 back flag */
            case DUR_2: symc = SMUFL_EBA7_luteDurationWhole; break; // 0 flags
            case DUR_4: symc = SMUFL_EBA8_luteDurationHalf; break; // 1 flag
            case DUR_8: symc = SMUFL_EBA9_luteDurationQuarter; break; // 2 flags
            case DUR_16: symc = SMUFL_EBAA_luteDuration8th; break; // 3 flags
            case DUR_32: symc = SMUFL_EBAB_luteDuration16th; break; // 4 flags
            case DUR_64: symc = SMUFL_EBAC_luteDuration32nd; break; // 5 flags
            default: symc = SMUFL_EBA9_luteDurationQuarter; // 2 flags
        }

        this->DrawSmuflCode(dc, x, y, symc, glyphSize, true);
    }

    if (tabGrp->HasDots()) {
        const int stemDirFactor = (tabDurSym->GetDrawingStemDir() == STEMDIRECTION_down) ? -1 : 1;
        if (tabDurSym->GetDrawingStem()) {
            y = tabDurSym->GetDrawingStem()->GetDrawingY();
        }

        int dotSize = 0;

        if (tabGrp->IsInBeam() || staff->IsTabGuitar()) {
            y += m_doc->GetDrawingUnit(glyphSize) * 0.5 * stemDirFactor;
            x += m_doc->GetDrawingUnit(glyphSize);
            dotSize = glyphSize * 2 / 3;
        }
        else {
            // Vertical: the more flags the lower the dots
            const int durfactor = DUR_64 - std::min(std::max(drawingDur, DUR_2), DUR_64) + 1;
            static_assert(DUR_64 - DUR_2 + 1 == 6);
            static_assert(DUR_64 - DUR_64 + 1 == 1);

            y += m_doc->GetDrawingUnit(glyphSize) * stemDirFactor * durfactor * 2 / 5;

            // Horizontal: allow for font width
            x += m_doc->GetGlyphWidth(SMUFL_EBA9_luteDurationQuarter, glyphSize, false) / 2;
            dotSize = glyphSize * 9 / 10;
        }

        for (int i = 0; i < tabGrp->GetDots(); ++i) {
            this->DrawDot(dc, x, y, dotSize);
            // HARDCODED
            x += m_doc->GetDrawingUnit(glyphSize) * 0.75;
        }
    }

    // Draw children (stems)
    this->DrawLayerChildren(dc, tabDurSym, layer, staff, measure);

    dc->EndGraphic(tabDurSym, this);
}

} // namespace vrv

/////////////////////////////////////////////////////////////////////////////
// Name:        view_tab.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <cmath>
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
#include "staffdef.h"
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

    // Draw children (rhythm, notes)
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

    int x = element->GetDrawingX();
    int y = element->GetDrawingY();

    int glyphSize = staff->GetDrawingStaffNotationSize();
    bool drawingCueSize = false;
    int overline = 0;
    int strike = 0;
    int underline = 0;

    if (staff->m_drawingNotationType == NOTATIONTYPE_tab_guitar) {

        std::u32string fret = note->GetTabFretString(staff->m_drawingNotationType, overline, strike, underline);

        FontInfo fretTxt;
        if (!dc->UseGlobalStyling()) {
            fretTxt.SetFaceName(m_doc->GetResources().GetTextFont());
        }

        TextDrawingParams params;
        params.m_x = x;
        params.m_y = y;
        params.m_pointSize = m_doc->GetDrawingLyricFont(glyphSize)->GetPointSize() * 4 / 5;
        fretTxt.SetPointSize(params.m_pointSize);

        dc->SetFont(&fretTxt);

        params.m_y -= (m_doc->GetTextGlyphHeight(L'0', &fretTxt, drawingCueSize) / 2);

        dc->StartText(
            this->ToDeviceContextX(params.m_x), this->ToDeviceContextY(params.m_y), HORIZONTALALIGNMENT_center);
        this->DrawTextString(dc, fret, params);
        dc->EndText();

        dc->ResetFont();
    }
    else {

        std::u32string fret = note->GetTabFretString(staff->m_drawingNotationType, overline, strike, underline);
        // Center for italian tablature
        if (staff->IsTabLuteItalian()) {
            y -= (m_doc->GetGlyphHeight(SMUFL_EBE0_luteItalianFret0, glyphSize, drawingCueSize) / 2);
        }
        // Above the line for french tablature
        else if (staff->IsTabLuteFrench()) {
            y -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize)
                - m_doc->GetDrawingStaffLineWidth(staff->m_drawingStaffSize);
        }
        // Center for German tablature
        else if (staff->IsTabLuteGerman()) {
            y -= m_doc->GetGlyphHeight(SMUFL_EC17_luteGermanAUpper, glyphSize, drawingCueSize) / 2;
        }

        dc->SetFont(m_doc->GetDrawingSmuflFont(glyphSize, false));
        this->DrawSmuflString(dc, x, y, fret, HORIZONTALALIGNMENT_center, glyphSize);

        // Add overlines, strikethoughs and underlines if required
        if ((overline > 0 || strike > 0 || underline > 0) && !fret.empty()) {
            const int lineThickness
                = m_options->m_lyricLineThickness.GetValue() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
            const int widthFront = m_doc->GetGlyphWidth(fret.front(), glyphSize, drawingCueSize);
            const int widthBack = m_doc->GetGlyphWidth(fret.back(), glyphSize, drawingCueSize);
            TextExtend extend;
            dc->GetSmuflTextExtent(fret, &extend);

            // TODO These fiddle factors seem necessary to get the lines balanced on either side
            // of the fret string.  Can we do better?
            const int x1
                = x - (fret.size() == 1 ? widthFront * 7 / 10 : widthFront * 12 / 10); // extend on the left hand side
            const int x2 = x + extend.m_width - widthBack * 1 / 10; // trim right hand overhang on last character

            dc->SetPen(lineThickness, PEN_SOLID);

            // overlines
            int y1 = y + extend.m_ascent + lineThickness;

            for (int i = 0; i < overline; ++i) {
                dc->DrawLine(this->ToDeviceContextX(x1), this->ToDeviceContextY(y1), this->ToDeviceContextX(x2),
                    this->ToDeviceContextY(y1));
                y1 += 2 * lineThickness;
            }

            // strikethroughs
            y1 = y + extend.m_ascent / 2 - (strike - 1) * lineThickness;

            for (int i = 0; i < strike; ++i) {
                dc->DrawLine(this->ToDeviceContextX(x1), this->ToDeviceContextY(y1), this->ToDeviceContextX(x2),
                    this->ToDeviceContextY(y1));
                y1 += 2 * lineThickness;
            }

            // underlines
            y1 = y - extend.m_descent - lineThickness;

            for (int i = 0; i < underline; ++i) {
                dc->DrawLine(this->ToDeviceContextX(x1), this->ToDeviceContextY(y1), this->ToDeviceContextX(x2),
                    this->ToDeviceContextY(y1));
                y1 -= 2 * lineThickness;
            }

            dc->ResetPen();
        }
        dc->ResetFont();
    }

    // Draw children (nothing yet)
    this->DrawLayerChildren(dc, note, layer, staff, measure);
}

void View::DrawTabDurSym(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);

    TabDurSym *tabDurSym = dynamic_cast<TabDurSym *>(element);
    assert(tabDurSym);

    TabGrp *tabGrp = vrv_cast<TabGrp *>(tabDurSym->GetFirstAncestor(TABGRP));
    assert(tabGrp);

    dc->StartGraphic(tabDurSym, "", tabDurSym->GetID());

    // adjust vertical position for tabDurSym@tab.line, tabDurSym@vo and tablature type
    // tabDurSym@tab.line takes priority over tabDurSym@vo
    if (!tabGrp->IsInBeam() && !staff->IsTabGuitar()) {
        if (tabDurSym->HasTabLine()) {
            const int yAdjust = (tabDurSym->GetTabLine() - staff->m_drawingLines) * 2;
            tabDurSym->SetDrawingYRel(yAdjust * m_doc->GetDrawingUnit(staff->m_drawingStaffSize));
        }
        else {
            int yAdjust = 1; // margin between staff line and rhythm sign, in half lines

            // position rhythm sign according to tablature type
            if (staff->IsTabLuteFrench() || staff->IsTabLuteGerman()) {
                yAdjust = 2;
            }
            else if (staff->IsTabLuteItalian() && staff->m_drawingLines >= 6) {
                yAdjust = 3; //  allow for >= 7 course Italian tablature
            }

            // adjust for tabDurSym@vo
            if (tabDurSym->HasVo() && tabDurSym->GetVo().GetType() == MEASUREMENTTYPE_vu) {
                yAdjust += std::round(tabDurSym->GetVo().GetVu());
            }

            tabDurSym->SetDrawingYRel(yAdjust * m_doc->GetDrawingUnit(staff->m_drawingStaffSize));
        }
    }

    int x = element->GetDrawingX();
    int y = element->GetDrawingY();

    const int glyphSize = staff->GetDrawingStaffNotationSize();

    const int drawingDur = (tabGrp->GetDurGes() != DURATION_NONE) ? tabGrp->GetActualDurGes() : tabGrp->GetActualDur();

    // For beam and guitar notation, stem are drawn through the child Stem
    if (!tabGrp->IsInBeam() && !staff->IsTabGuitar()) {
        int symc = 0;
        switch (drawingDur) {
            case DURATION_1: symc = SMUFL_EBA6_luteDurationDoubleWhole; break; // 1 back flag */
            case DURATION_2: symc = SMUFL_EBA7_luteDurationWhole; break; // 0 flags
            case DURATION_4: symc = SMUFL_EBA8_luteDurationHalf; break; // 1 flag
            case DURATION_8: symc = SMUFL_EBA9_luteDurationQuarter; break; // 2 flags
            case DURATION_16: symc = SMUFL_EBAA_luteDuration8th; break; // 3 flags
            case DURATION_32: symc = SMUFL_EBAB_luteDuration16th; break; // 4 flags
            case DURATION_64: symc = SMUFL_EBAC_luteDuration32nd; break; // 5 flags
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
            int durOffset = (drawingDur > DURATION_2) ? drawingDur : DURATION_2;
            durOffset = (durOffset < DURATION_64) ? durOffset : DURATION_64;
            const int durfactor = DURATION_64 - durOffset + 1;
            static_assert(DURATION_64 - DURATION_2 + 1 == 6);
            static_assert(DURATION_64 - DURATION_64 + 1 == 1);

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

    // Draw children (stems) for beam or guitar notation
    if (tabGrp->IsInBeam() || staff->IsTabGuitar()) {
        this->DrawLayerChildren(dc, tabDurSym, layer, staff, measure);
    }

    dc->EndGraphic(tabDurSym, this);
}

} // namespace vrv

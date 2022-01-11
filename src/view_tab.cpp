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
        wchar_t code = (staff->m_drawingNotationType == NOTATIONTYPE_tab_lute_french) ? SMUFL_EBC0_luteFrenchFretA
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
    y += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 1.5;
    int drawingDur = (tabGrp->GetDurGes() != DURATION_NONE) ? tabGrp->GetActualDurGes() : tabGrp->GetActualDur();
    int glyphSize = staff->m_drawingStaffSize / TABLATURE_STAFF_RATIO;

    // We only need to draw the stems
    // Do we also need to draw the dots?
    if (tabGrp->IsInBeam()) {
        const int height = m_doc->GetGlyphHeight(SMUFL_EBA8_luteDurationHalf, glyphSize, true);
        DrawFilledRectangle(dc, x - m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) / 2, y,
            x + m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) / 2, y + height);
    }
    else {
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

        if (tabGrp->HasDots()) {
            y += m_doc->GetDrawingUnit(glyphSize) * 0.5;
            x += m_doc->GetDrawingUnit(glyphSize);
            for (int i = 0; i < tabGrp->GetDots(); ++i) {
                DrawDot(dc, x, y, glyphSize / 2);
                // HARDCODED
                x += m_doc->GetDrawingUnit(glyphSize) * 0.75;
            }
        }
    }

    // Draw children (nothing yet)
    DrawLayerChildren(dc, tabDurSym, layer, staff, measure);

    dc->EndGraphic(tabDurSym, this);
}

void View::DrawTabBeamSegment(DeviceContext *dc, BeamSegment *beamSegment, BeamDrawingInterface *beamInterface,
    Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(beamSegment);
    assert(beamInterface);
    assert(layer);
    assert(staff);
    assert(measure);

    // temporary coordinates
    int x1, x2, y1, y2;

    // temporary variables
    int shiftY;

    // loops
    int i;

    const ArrayOfBeamElementCoords *beamElementCoords = beamSegment->GetElementCoordRefs();

    int elementCount = (int)beamElementCoords->size();
    int last = elementCount - 1;

    /******************************************************************/
    // Draw the beam full bar

    // Adjust the x position of the first and last element for taking into account the stem width
    beamElementCoords->at(0)->m_x -= (m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize)) / 2;
    beamElementCoords->at(last)->m_x += (m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize)) / 2;

    // Shift direction
    shiftY = (beamInterface->m_drawingPlace == BEAMPLACE_below) ? 1.0 : -1.0;

    int glyphSize = staff->m_drawingStaffSize / TABLATURE_STAFF_RATIO;
    int height = m_doc->GetGlyphHeight(SMUFL_EBA8_luteDurationHalf, glyphSize, true);
    int y = staff->GetDrawingY() + height;
    y += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 1.5;

    x1 = beamElementCoords->at(0)->m_x;
    x2 = beamElementCoords->at(last)->m_x;

    // For acc and rit beam (see AttBeamingVis set
    // s_y = 0 and s_y2 = 0 respectively

    const int polygonHeight = beamInterface->m_beamWidthBlack * shiftY / 2;
    DrawObliquePolygon(dc, x1, y, x2, y, polygonHeight);

    /******************************************************************/
    // Draw the beam for partial bars (if any)

    // Map the indexes of the tabGrp since we need to ignore rests when drawing partials
    // However, exception for the first and last element of a beam
    std::vector<int> noteIndexes;
    for (i = 0; i < elementCount; ++i) {
        if (beamElementCoords->at(i)->m_element->Is(REST))
            if (i > 0 && i < elementCount - 1) continue;
        noteIndexes.push_back(i);
    }
    int noteCount = (int)noteIndexes.size();

    if (noteCount > 0) {
        int testDur = DUR_16;
        int barY = beamInterface->m_beamWidth / 2;

        if (beamInterface->m_drawingPlace == BEAMPLACE_above) {
            barY = -barY;
        }

        int fractBeamWidth = m_doc->GetGlyphWidth(SMUFL_E0A4_noteheadBlack, glyphSize, beamInterface->m_cueSize);

        // loop
        while (testDur <= beamInterface->m_shortestDur) {
            // true at the beginning of a beam or after a breakSec
            bool start = true;

            int idx = 0;

            // all but the last one
            for (i = 0; i < noteCount - 1; ++i) {
                idx = noteIndexes.at(i);
                int nextIdx = noteIndexes.at(i + 1);

                bool breakSec = ((beamElementCoords->at(idx)->m_breaksec)
                    && (testDur - DUR_8 >= beamElementCoords->at(idx)->m_breaksec));
                beamElementCoords->at(idx)->m_partialFlags[testDur - DUR_8] = PARTIAL_NONE;
                // partial is needed
                if (beamElementCoords->at(idx)->m_dur >= (char)testDur) {
                    // and for the next one too, but no break - through
                    if ((beamElementCoords->at(nextIdx)->m_dur >= (char)testDur) && !breakSec) {
                        beamElementCoords->at(idx)->m_partialFlags[testDur - DUR_8] = PARTIAL_THROUGH;
                    }
                    // not needed for the next one or break
                    else {
                        // we are starting a beam or after a beam break - put it right
                        if (start) {
                            if ((idx != 0) && (beamElementCoords->at(idx - 1)->m_element->Is(REST))) {
                                beamElementCoords->at(idx)->m_partialFlags[testDur - DUR_8] = PARTIAL_LEFT;
                            }
                            else {
                                beamElementCoords->at(idx)->m_partialFlags[testDur - DUR_8] = PARTIAL_RIGHT;
                            }
                        }
                        // or the previous one had no partial
                        else if (beamElementCoords->at(noteIndexes.at(i - 1))->m_dur < (char)testDur) {
                            // if we are at the full bar level, put it left
                            if (testDur == DUR_16) {
                                beamElementCoords->at(idx)->m_partialFlags[testDur - DUR_8] = PARTIAL_LEFT;
                            }
                            // if the previous level underneath was a partial through, put it left
                            else if (beamElementCoords->at(noteIndexes.at(i - 1))->m_partialFlags[testDur - 1 - DUR_8]
                                == PARTIAL_THROUGH) {
                                beamElementCoords->at(idx)->m_partialFlags[testDur - DUR_8] = PARTIAL_LEFT;
                            }
                            // if the level underneath was not left (right or through), put it right
                            else if (beamElementCoords->at(idx)->m_partialFlags[testDur - 1 - DUR_8] != PARTIAL_LEFT) {
                                beamElementCoords->at(idx)->m_partialFlags[testDur - DUR_8] = PARTIAL_RIGHT;
                            }
                            // it was put left before, put it left
                            else {
                                beamElementCoords->at(idx)->m_partialFlags[testDur - DUR_8] = PARTIAL_LEFT;
                            }
                        }
                    }
                }
                // we are not in a group
                start = breakSec;
            }
            // last one
            idx = (int)noteIndexes.back();
            beamElementCoords->at(idx)->m_partialFlags[testDur - DUR_8] = PARTIAL_NONE;
            // partial is needed
            if ((beamElementCoords->at(idx)->m_dur >= (char)testDur)) {
                // and the previous one had no partial - put it left
                if ((noteCount == 1) || (beamElementCoords->at(noteIndexes.at(i - 1))->m_dur < (char)testDur)
                    || start) {
                    beamElementCoords->at(idx)->m_partialFlags[testDur - DUR_8] = PARTIAL_LEFT;
                }
            }

            barY = beamInterface->m_beamWidth * (testDur - DUR_8) / 2;

            // draw them
            for (i = 0; i < noteCount; ++i) {
                int barYPos = 0;
                idx = noteIndexes.at(i);
                if (BEAMPLACE_mixed == beamInterface->m_drawingPlace) {
                    int elemIndex = idx;
                    if (BEAMPLACE_NONE == beamElementCoords->at(idx)->m_partialFlagPlace) {
                        if ((0 == i) || ((noteCount - 1) == i)
                            || (beamElementCoords->at(noteIndexes.at(i - 1))->m_partialFlagPlace
                                != beamElementCoords->at(noteIndexes.at(i + 1))->m_partialFlagPlace)) {
                            continue;
                        }
                        elemIndex = i - 1;
                    }
                    barYPos
                        = barY * ((BEAMPLACE_above == beamElementCoords->at(elemIndex)->m_partialFlagPlace) ? 1 : -1);
                }
                else {
                    barYPos = shiftY * barY;
                }
                if (beamElementCoords->at(idx)->m_partialFlags[testDur - DUR_8] == PARTIAL_THROUGH) {
                    // through should never be set on the last one
                    assert(i < noteCount - 1);
                    if (i >= noteCount - 1) continue; // assert for debug and skip otherwise
                    y1 = y + barYPos;
                    y2 = y + barYPos;
                    DrawObliquePolygon(dc, beamElementCoords->at(idx)->m_x, y1,
                        beamElementCoords->at(noteIndexes.at(i + 1))->m_x, y2, polygonHeight);
                }
                else if (beamElementCoords->at(idx)->m_partialFlags[testDur - DUR_8] == PARTIAL_RIGHT) {
                    y1 = y + barYPos;
                    int x2 = beamElementCoords->at(idx)->m_x + fractBeamWidth;
                    y2 = y + barYPos + beamSegment->m_beamSlope * (x2 - beamElementCoords->at(0)->m_x);
                    DrawObliquePolygon(dc, beamElementCoords->at(idx)->m_x, y1, x2, y2, polygonHeight);
                }
                else if (beamElementCoords->at(idx)->m_partialFlags[testDur - DUR_8] == PARTIAL_LEFT) {
                    y2 = y + barYPos;
                    int x1 = beamElementCoords->at(idx)->m_x - fractBeamWidth;
                    y1 = y + barYPos + beamSegment->m_beamSlope * (x1 - beamElementCoords->at(0)->m_x);
                    DrawObliquePolygon(dc, x1, y1, beamElementCoords->at(idx)->m_x, y2, polygonHeight);
                }
            }

            testDur += 1;

        } // end of while
    } // end of drawing partial bars
}

} // namespace vrv

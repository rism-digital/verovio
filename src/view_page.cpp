/////////////////////////////////////////////////////////////////////////////
// Name:        view_page.cpp
// Author:      Laurent Pugin and Chris Niven
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "annot.h"
#include "attcomparison.h"
#include "beam.h"
#include "clef.h"
#include "controlelement.h"
#include "devicecontext.h"
#include "doc.h"
#include "editorial.h"
#include "ending.h"
#include "fb.h"
#include "fig.h"
#include "functorparams.h"
#include "keysig.h"
#include "label.h"
#include "labelabbr.h"
#include "layer.h"
#include "measure.h"
#include "mensur.h"
#include "metersig.h"
#include "mnum.h"
#include "note.h"
#include "options.h"
#include "page.h"
#include "smufl.h"
#include "staff.h"
#include "staffdef.h"
#include "staffgrp.h"
#include "syl.h"
#include "system.h"
#include "text.h"
#include "tuplet.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// View - Page
//----------------------------------------------------------------------------

void View::DrawCurrentPage(DeviceContext *dc, bool background)
{
    assert(dc);
    assert(m_doc);

    m_currentPage = m_doc->SetDrawingPage(m_pageIdx);

    int i;

    // Keep the width of the initial scoreDef
    SetScoreDefDrawingWidth(dc, &m_currentPage->m_drawingScoreDef);

    // Set the current score def to the page one
    // The page one has previously been set by Object::SetCurrentScoreDef
    m_drawingScoreDef = m_currentPage->m_drawingScoreDef;

    if (background) dc->DrawRectangle(0, 0, m_doc->m_drawingPageWidth, m_doc->m_drawingPageHeight);

    dc->DrawBackgroundImage();

    Point origin = dc->GetLogicalOrigin();
    dc->SetLogicalOrigin(origin.x - m_doc->m_drawingPageMarginLeft, origin.y - m_doc->m_drawingPageMarginTop);

    dc->StartPage();

    for (i = 0; i < m_currentPage->GetSystemCount(); ++i) {
        // cast to System check in DrawSystem
        System *system = dynamic_cast<System *>(m_currentPage->GetChild(i));
        DrawSystem(dc, system);
    }

    DrawRunningElements(dc, m_currentPage);

    dc->EndPage();
}

double View::GetPPUFactor() const
{
    if (!m_currentPage) return 1.0;

    return m_currentPage->GetPPUFactor();
}

void View::SetScoreDefDrawingWidth(DeviceContext *dc, ScoreDef *scoreDef)
{
    assert(dc);
    assert(scoreDef);

    char numAlteration = 0;

    // key signature of the scoreDef
    if (scoreDef->HasKeySigInfo()) {
        KeySig *keySig = scoreDef->GetKeySigCopy();
        assert(keySig);
        numAlteration = (keySig->GetAlterationNumber() > numAlteration) ? keySig->GetAlterationNumber() : numAlteration;
        delete keySig;
    }

    // longest key signature of the staffDefs
    const ListOfObjects *scoreDefList = scoreDef->GetList(scoreDef); // make sure it's initialized
    for (ListOfObjects::const_iterator it = scoreDefList->begin(); it != scoreDefList->end(); ++it) {
        StaffDef *staffDef = dynamic_cast<StaffDef *>(*it);
        assert(staffDef);
        if (!staffDef->HasKeySigInfo()) continue;
        KeySig *keySig = staffDef->GetKeySigCopy();
        assert(keySig);
        numAlteration = (keySig->GetAlterationNumber() > numAlteration) ? keySig->GetAlterationNumber() : numAlteration;
        delete keySig;
    }

    int width = 0;
    // G-clef as default width
    width += m_doc->GetLeftMargin(CLEF) + m_doc->GetGlyphWidth(SMUFL_E050_gClef, 100, false)
        + m_doc->GetRightMargin(CLEF);
    if (numAlteration > 0) {
        width += m_doc->GetLeftMargin(KEYSIG)
            + m_doc->GetGlyphWidth(SMUFL_E262_accidentalSharp, 100, false) * TEMP_KEYSIG_STEP
            + m_doc->GetRightMargin(KEYSIG);
    }

    scoreDef->SetDrawingWidth(width);
}

//----------------------------------------------------------------------------
// View - System
//----------------------------------------------------------------------------

void View::DrawSystem(DeviceContext *dc, System *system)
{
    assert(dc);
    assert(system);

    dc->StartGraphic(system, "", system->GetUuid());

    // first we need to clear the drawing list of postponed elements
    system->ResetDrawingList();

    Measure *systemStart = dynamic_cast<Measure *>(system->FindChildByType(MEASURE));
    if (systemStart) {
        DrawScoreDef(dc, system->GetDrawingScoreDef(), systemStart, system->GetDrawingX(), NULL);
    }

    DrawSystemChildren(dc, system, system);

    DrawSystemList(dc, system, SYL);
    DrawSystemList(dc, system, HAIRPIN);
    DrawSystemList(dc, system, TRILL);
    DrawSystemList(dc, system, OCTAVE);
    DrawSystemList(dc, system, TIE);
    DrawSystemList(dc, system, SLUR);
    DrawSystemList(dc, system, ENDING);

    dc->EndGraphic(system, this);
}

void View::DrawSystemList(DeviceContext *dc, System *system, const ClassId classId)
{
    assert(dc);
    assert(system);

    ListOfObjects *drawingList = system->GetDrawingList();
    ListOfObjects::iterator iter;

    for (iter = drawingList->begin(); iter != drawingList->end(); ++iter) {
        if ((*iter)->Is(classId) && (classId == HAIRPIN)) {
            DrawTimeSpanningElement(dc, *iter, system);
        }
        if ((*iter)->Is(classId) && (classId == OCTAVE)) {
            DrawTimeSpanningElement(dc, *iter, system);
        }
        if ((*iter)->Is(classId) && (classId == SYL)) {
            DrawTimeSpanningElement(dc, *iter, system);
        }
        if ((*iter)->Is(classId) && (classId == TIE)) {
            DrawTimeSpanningElement(dc, *iter, system);
        }
        if ((*iter)->Is(classId) && (classId == TRILL)) {
            DrawTimeSpanningElement(dc, *iter, system);
        }
        if ((*iter)->Is(classId) && (classId == SLUR)) {
            DrawTimeSpanningElement(dc, *iter, system);
        }
        if ((*iter)->Is(classId) && (classId == ENDING)) {
            // cast to Ending check in DrawEnding
            DrawEnding(dc, dynamic_cast<Ending *>(*iter), system);
        }
    }
}

void View::DrawScoreDef(
    DeviceContext *dc, ScoreDef *scoreDef, Measure *measure, int x, BarLine *barLine, bool isLastMeasure)
{
    assert(dc);
    assert(scoreDef);
    // we need at least one measure to be able to draw the groups - we need access to the staff elements,
    assert(measure);

    StaffGrp *staffGrp = dynamic_cast<StaffGrp *>(scoreDef->FindChildByType(STAFFGRP));
    if (!staffGrp) {
        return;
    }

    if (barLine == NULL) {
        // Draw the first staffGrp and from there its children recursively
        DrawStaffGrp(dc, measure, staffGrp, x, true, !scoreDef->DrawLabels());

        DrawStaffDefLabels(dc, measure, scoreDef, !scoreDef->DrawLabels());
        // if this was true (non-abbreviated labels), set it to false for next one
        // scoreDef->SetDrawLabels(false);
    }
    else {
        dc->StartGraphic(barLine, "", barLine->GetUuid());
        DrawBarLines(dc, measure, staffGrp, barLine, isLastMeasure);
        dc->EndGraphic(barLine, this);
    }

    return;
}

void View::DrawStaffGrp(
    DeviceContext *dc, Measure *measure, StaffGrp *staffGrp, int x, bool topStaffGrp, bool abbreviations)
{
    assert(dc);
    assert(measure);
    assert(staffGrp);

    TextExtend extend;

    const ListOfObjects *staffDefs = staffGrp->GetList(staffGrp);
    if (staffDefs->empty()) {
        return;
    }

    // Get the first and last staffDef of the staffGrp
    StaffDef *firstDef = dynamic_cast<StaffDef *>(staffDefs->front());
    StaffDef *lastDef = dynamic_cast<StaffDef *>(staffDefs->back());

    if (!firstDef || !lastDef) {
        LogDebug("Could not get staffDef while drawing staffGrp - DrawStaffGrp");
        return;
    }

    // Get the corresponding staff looking at the previous (or first) measure
    AttNIntegerComparison comparisonFirst(STAFF, firstDef->GetN());
    Staff *first = dynamic_cast<Staff *>(measure->FindChildByComparison(&comparisonFirst, 1));
    AttNIntegerComparison comparisonLast(STAFF, lastDef->GetN());
    Staff *last = dynamic_cast<Staff *>(measure->FindChildByComparison(&comparisonLast, 1));

    if (!first || !last) {
        LogDebug(
            "Could not get staff (%d; %d) while drawing staffGrp - DrawStaffGrp", firstDef->GetN(), lastDef->GetN());
        return;
    }

    int yTop = first->GetDrawingY();
    // for the bottom position we need to take into account the number of lines and the staff size
    int yBottom
        = last->GetDrawingY() - (lastDef->GetLines() - 1) * m_doc->GetDrawingDoubleUnit(last->m_drawingStaffSize);
    int barLineWidth = m_doc->GetDrawingBarLineWidth(100);

    // adjust the top and bottom according to staffline width
    x += barLineWidth / 2;
    // yTop += m_doc->GetDrawingStaffLineWidth(100) / 1;
    // yBottom -= m_doc->GetDrawingStaffLineWidth(100) / 4;

    Label *label = dynamic_cast<Label *>(staffGrp->FindChildByType(LABEL, 1));
    LabelAbbr *labelAbbr = dynamic_cast<LabelAbbr *>(staffGrp->FindChildByType(LABELABBR, 1));
    Object *graphic = label;

    std::wstring labelAbbrStr = (labelAbbr) ? labelAbbr->GetText(labelAbbr) : L"";
    std::wstring labelStr = (label) ? label->GetText(label) : L"";

    if (abbreviations) {
        labelStr = labelAbbrStr;
        graphic = labelAbbr;
    }

    if (graphic && (labelStr.length() != 0)) {
        // HARDCODED
        int space = 4 * m_doc->GetDrawingBeamWidth(100, false);
        int xLabel = x - space;
        int yLabel = yBottom - (yBottom - yTop) / 2 - m_doc->GetDrawingUnit(100);

        dc->SetBrush(m_currentColour, AxSOLID);

        FontInfo grpTxt;
        if (!dc->UseGlobalStyling()) {
            grpTxt.SetFaceName("Times");
        }

        TextDrawingParams params;
        params.m_x = xLabel;
        params.m_y = yLabel;
        params.m_pointSize = m_doc->GetDrawingLyricFont(100)->GetPointSize();

        grpTxt.SetPointSize(params.m_pointSize);
        dc->SetFont(&grpTxt);

        dc->GetTextExtent(labelStr, &extend, true);

        dc->StartGraphic(graphic, "", graphic->GetUuid());

        dc->StartText(ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y), HORIZONTALALIGNMENT_right);
        DrawTextChildren(dc, graphic, params);
        dc->EndText();

        dc->EndGraphic(graphic, this);

        // keep the widest width for the system
        System *system = dynamic_cast<System *>(measure->GetFirstParent(SYSTEM));
        if (system) {
            system->SetDrawingLabelsWidth(extend.m_width + space);
        }

        // also store in the system the maximum width with abbreviations
        if (system && !abbreviations && (labelAbbrStr.length() > 0)) {
            dc->GetTextExtent(labelAbbrStr, &extend, true);
            system->SetDrawingAbbrLabelsWidth(extend.m_width + space);
        }

        dc->ResetFont();
        dc->ResetBrush();
    }

    // actually draw the line, the brace or the bracket
    if (topStaffGrp && ((firstDef != lastDef) || (staffGrp->GetSymbol() != staffGroupingSym_SYMBOL_NONE))) {
        DrawVerticalLine(dc, yTop, yBottom, x, barLineWidth);
    }
    // this will need to be changed with the next version of MEI will line means additional thick line
    if (staffGrp->GetSymbol() == staffGroupingSym_SYMBOL_line) {
        DrawVerticalLine(dc, yTop, yBottom, x, barLineWidth);
    }
    else if (staffGrp->GetSymbol() == staffGroupingSym_SYMBOL_brace) {
        DrawBrace(dc, x, yTop, yBottom, last->m_drawingStaffSize);
    }
    else if (staffGrp->GetSymbol() == staffGroupingSym_SYMBOL_bracket) {
        DrawBracket(dc, x, yTop, yBottom, last->m_drawingStaffSize);
        x -= 2 * m_doc->GetDrawingBeamWidth(100, false) - m_doc->GetDrawingBeamWhiteWidth(100, false);
    }

    // recursively draw the children
    int i;
    StaffGrp *childStaffGrp = NULL;
    for (i = 0; i < staffGrp->GetChildCount(); ++i) {
        childStaffGrp = dynamic_cast<StaffGrp *>(staffGrp->GetChild(i));
        if (childStaffGrp) {
            DrawStaffGrp(dc, measure, childStaffGrp, x, false, abbreviations);
        }
    }
}

void View::DrawStaffDefLabels(DeviceContext *dc, Measure *measure, ScoreDef *scoreDef, bool abbreviations)
{
    assert(dc);
    assert(measure);
    assert(scoreDef);

    TextExtend extend;

    const ListOfObjects *scoreDefChildren = scoreDef->GetList(scoreDef);
    ListOfObjects::const_iterator iter = scoreDefChildren->begin();
    while (iter != scoreDefChildren->end()) {
        StaffDef *staffDef = dynamic_cast<StaffDef *>(*iter);

        if (!staffDef) {
            LogDebug("Should be staffDef in View::DrawStaffDefLabels");
            ++iter;
            continue;
        }

        AttNIntegerComparison comparison(STAFF, staffDef->GetN());
        Staff *staff = dynamic_cast<Staff *>(measure->FindChildByComparison(&comparison, 1));
        System *system = dynamic_cast<System *>(measure->GetFirstParent(SYSTEM));

        if (!staff || !system) {
            LogDebug("Staff or System missing in View::DrawStaffDefLabels");
            ++iter;
            continue;
        }

        Label *label = dynamic_cast<Label *>(staffDef->FindChildByType(LABEL, 1));
        LabelAbbr *labelAbbr = dynamic_cast<LabelAbbr *>(staffDef->FindChildByType(LABELABBR, 1));
        Object *graphic = label;

        std::wstring labelStr = (label) ? label->GetText(label) : L"";
        std::wstring labelAbbrStr = (labelAbbr) ? labelAbbr->GetText(labelAbbr) : L"";

        if (abbreviations) {
            labelStr = labelAbbrStr;
            graphic = labelAbbr;
        }

        if (!graphic || (labelStr.length() == 0)) {
            ++iter;
            continue;
        }

        // HARDCODED
        int space = 3 * m_doc->GetDrawingBeamWidth(100, false);
        int x = system->GetDrawingX() - space;
        int y = staff->GetDrawingY()
            - (staffDef->GetLines() * m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 2);

        FontInfo labelTxt;
        if (!dc->UseGlobalStyling()) {
            labelTxt.SetFaceName("Times");
        }

        TextDrawingParams params;
        params.m_x = x;
        params.m_y = y;
        params.m_pointSize = m_doc->GetDrawingLyricFont(staff->m_drawingStaffSize)->GetPointSize();

        labelTxt.SetPointSize(params.m_pointSize);

        dc->SetBrush(m_currentColour, AxSOLID);
        dc->SetFont(&labelTxt);

        dc->GetTextExtent(labelStr, &extend, true);

        dc->StartGraphic(graphic, "", graphic->GetUuid());

        dc->StartText(ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y), HORIZONTALALIGNMENT_right);
        DrawTextChildren(dc, graphic, params);
        dc->EndText();

        dc->EndGraphic(graphic, this);

        // keep the widest width for the system
        system->SetDrawingLabelsWidth(extend.m_width + space);
        // also store in the system the maximum width with abbreviations for justification
        if (!abbreviations && (labelAbbrStr.length() > 0)) {
            dc->GetTextExtent(labelAbbrStr, &extend, true);
            system->SetDrawingAbbrLabelsWidth(extend.m_width + space);
        }

        dc->ResetFont();
        dc->ResetBrush();

        ++iter;
    }
}

void View::DrawBracket(DeviceContext *dc, int x, int y1, int y2, int staffSize)
{
    assert(dc);

    int x1, x2;

    x2 = x - m_doc->GetDrawingBeamWidth(100, false);
    x1 = x2 - m_doc->GetDrawingBeamWidth(100, false);

    DrawSmuflCode(dc, x1, y1, SMUFL_E003_bracketTop, staffSize, false);
    DrawSmuflCode(dc, x1, y2, SMUFL_E004_bracketBottom, staffSize, false);

    // adjust to top and bottom position so we make sure there is no white space between
    // the glyphs and the line
    y1 += m_doc->GetDrawingStemWidth(100);
    y2 -= m_doc->GetDrawingStemWidth(100);
    DrawFilledRectangle(dc, x1, y1, x2, y2);

    return;
}

void View::DrawBrace(DeviceContext *dc, int x, int y1, int y2, int staffSize)
{
    assert(dc);

    // int new_coords[2][6];
    Point points[4];
    Point bez1[4];
    Point bez2[4];

    int penWidth = m_doc->GetDrawingStemWidth(100);
    y1 -= penWidth;
    y2 += penWidth;
    BoundingBox::Swap(y1, y2);

    int ymed, xdec, fact;

    x -= m_doc->GetDrawingBeamWhiteWidth(staffSize, false); // distance entre barre et debut accolade

    ymed = (y1 + y2) / 2;
    fact = m_doc->GetDrawingBeamWhiteWidth(staffSize, false) + m_doc->GetDrawingStemWidth(100);
    xdec = ToDeviceContextX(fact);

    points[0].x = ToDeviceContextX(x);
    points[0].y = ToDeviceContextY(y1);
    points[1].x = ToDeviceContextX(x - m_doc->GetDrawingDoubleUnit(staffSize) * 2);
    points[1].y = points[0].y - ToDeviceContextX(m_doc->GetDrawingDoubleUnit(staffSize) * 3);
    points[3].x = ToDeviceContextX(x - m_doc->GetDrawingDoubleUnit(staffSize));
    points[3].y = ToDeviceContextY(ymed);
    points[2].x = ToDeviceContextX(x + m_doc->GetDrawingUnit(staffSize));
    points[2].y = points[3].y + ToDeviceContextX(m_doc->GetDrawingDoubleUnit(staffSize));

    bez1[0] = points[0];
    bez1[1] = points[1];
    bez1[2] = points[2];
    bez1[3] = points[3];

    points[1].x += xdec;
    points[2].x += xdec;

    bez2[0] = points[0];
    bez2[1] = points[1];
    bez2[2] = points[2];
    bez2[3] = points[3];

    dc->SetPen(m_currentColour, std::max(1, penWidth), AxSOLID);
    dc->SetBrush(m_currentColour, AxSOLID);

    dc->DrawComplexBezierPath(bez1, bez2);

    // on produit l'image reflet vers le bas: 0 est identique
    points[0].y = ToDeviceContextY(y2);
    points[1].y = points[0].y + ToDeviceContextX(m_doc->GetDrawingDoubleUnit(staffSize) * 3);
    points[3].y = ToDeviceContextY(ymed);
    points[2].y = points[3].y - ToDeviceContextX(m_doc->GetDrawingDoubleUnit(staffSize));

    bez1[0] = points[0];
    bez1[1] = points[1];
    bez1[2] = points[2];
    bez1[3] = points[3];

    points[1].x -= xdec;
    points[2].x -= xdec;

    bez2[0] = points[0];
    bez2[1] = points[1];
    bez2[2] = points[2];
    bez2[3] = points[3];

    dc->DrawComplexBezierPath(bez1, bez2);

    dc->ResetPen();
    dc->ResetBrush();

    return;
}

void View::DrawBarLines(DeviceContext *dc, Measure *measure, StaffGrp *staffGrp, BarLine *barLine, bool isLastMeasure)
{
    assert(dc);
    assert(measure);
    assert(staffGrp);
    assert(barLine);

    if (staffGrp->GetBarthru() != BOOLEAN_true) {
        // recursively draw the children (staffDef or staffGrp) - we assume @barthru is false by default
        int i;
        StaffGrp *childStaffGrp = NULL;
        StaffDef *childStaffDef = NULL;
        for (i = 0; i < staffGrp->GetChildCount(); ++i) {
            childStaffGrp = dynamic_cast<StaffGrp *>(staffGrp->GetChild(i));
            childStaffDef = dynamic_cast<StaffDef *>(staffGrp->GetChild(i));
            if (childStaffGrp) {
                DrawBarLines(dc, measure, childStaffGrp, barLine, isLastMeasure);
            }
            else if (childStaffDef) {
                AttNIntegerComparison comparison(STAFF, childStaffDef->GetN());
                Staff *staff = dynamic_cast<Staff *>(measure->FindChildByComparison(&comparison, 1));
                if (!staff) {
                    LogDebug("Could not get staff (%d) while drawing staffGrp - DrawBarLines", childStaffDef->GetN());
                    continue;
                }
                int yTop = staff->GetDrawingY();
                // for the bottom position we need to take into account the number of lines and the staff size
                int yBottom = staff->GetDrawingY()
                    - (childStaffDef->GetLines() - 1) * m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
                DrawBarLine(dc, yTop, yBottom, barLine);
                if (barLine->HasRepetitionDots()) {
                    DrawBarLineDots(dc, childStaffDef, staff, barLine);
                }
            }
        }
    }
    else {
        const ListOfObjects *staffDefs = staffGrp->GetList(staffGrp);
        if (staffDefs->empty()) {
            return;
        }

        // Get the first and last staffDef of the staffGrp
        StaffDef *firstDef = dynamic_cast<StaffDef *>(staffDefs->front());
        StaffDef *lastDef = dynamic_cast<StaffDef *>(staffDefs->back());

        if (!firstDef || !lastDef) {
            LogDebug("Could not get staffDef while drawing staffGrp - DrawStaffGrp");
            return;
        }

        // Get the corresponding staff looking at the previous (or first) measure
        AttNIntegerComparison comparisonFirst(STAFF, firstDef->GetN());
        Staff *first = dynamic_cast<Staff *>(measure->FindChildByComparison(&comparisonFirst, 1));
        AttNIntegerComparison comparisonLast(STAFF, lastDef->GetN());
        Staff *last = dynamic_cast<Staff *>(measure->FindChildByComparison(&comparisonLast, 1));

        if (!first || !last) {
            LogDebug("Could not get staff (%d; %d) while drawing staffGrp - DrawStaffGrp", firstDef->GetN(),
                lastDef->GetN());
            return;
        }

        int yTop = first->GetDrawingY();
        // for the bottom position we need to take into account the number of lines and the staff size
        int yBottom
            = last->GetDrawingY() - (lastDef->GetLines() - 1) * m_doc->GetDrawingDoubleUnit(last->m_drawingStaffSize);

        // erase intersections only if we have more than one staff
        bool eraseIntersections = (first != last) ? true : false;
        // do not erase intersections with right barline of the last measure of the system
        if (isLastMeasure && barLine->Is(BARLINE_ATTR_RIGHT)) {
            eraseIntersections = false;
        }
        DrawBarLine(dc, yTop, yBottom, barLine, eraseIntersections);

        // Now we have a barthru barLine, but we have dots so we still need to go through each staff
        if (barLine->HasRepetitionDots()) {
            StaffDef *childStaffDef = NULL;
            const ListOfObjects *childList = staffGrp->GetList(staffGrp); // make sure it's initialized
            for (ListOfObjects::const_reverse_iterator it = childList->rbegin(); it != childList->rend(); ++it) {
                childStaffDef = dynamic_cast<StaffDef *>((*it));
                if (childStaffDef) {
                    AttNIntegerComparison comparison(STAFF, childStaffDef->GetN());
                    Staff *staff = dynamic_cast<Staff *>(measure->FindChildByComparison(&comparison, 1));
                    if (!staff) {
                        LogDebug(
                            "Could not get staff (%d) while drawing staffGrp - DrawBarLines", childStaffDef->GetN());
                        continue;
                    }
                    DrawBarLineDots(dc, childStaffDef, staff, barLine);
                }
            }
        }
    }
}

void View::DrawBarLine(DeviceContext *dc, int yTop, int yBottom, BarLine *barLine, bool eraseIntersections)
{
    assert(dc);
    assert(barLine);

    int x = barLine->GetDrawingX();
    int barLineWidth = m_doc->GetDrawingBarLineWidth(100);
    int barLineThickWidth = m_doc->GetDrawingBeamWidth(100, false);
    int x1 = x - m_doc->GetDrawingBeamWidth(100, false) - barLineWidth;
    int x2 = x + m_doc->GetDrawingBeamWidth(100, false) + barLineWidth;
    // optimized for five line staves
    int dashLength = m_doc->GetDrawingUnit(100) * 8 / 13;

    SegmentedLine line(yTop, yBottom);
    // We do not need to do this during layout calculation
    if (eraseIntersections && !dc->Is(BBOX_DEVICE_CONTEXT)) {
        System *system = dynamic_cast<System *>(barLine->GetFirstParent(SYSTEM));
        if (system) {
            int minX = x1 - barLineWidth / 2;
            int maxX = x1 + barLineWidth / 2;
            if ((barLine->GetForm() == BARRENDITION_rptend) || (barLine->GetForm() == BARRENDITION_end)) {
                maxX = x + barLineThickWidth / 2;
            }
            else if (barLine->GetForm() == BARRENDITION_rptboth) {
                maxX = x2 + barLineWidth / 2;
            }
            else if (barLine->GetForm() == BARRENDITION_rptstart) {
                minX = x - barLineThickWidth / 2;
                maxX = x2 + barLineWidth / 2;
            }
            else if ((barLine->GetForm() == BARRENDITION_dbl) || (barLine->GetForm() == BARRENDITION_dbldashed)) {
                minX = x - barLineWidth / 2;
                maxX = x2 + barLineWidth / 2;
            }
            Object lines;
            lines.SetParent(system);
            lines.UpdateContentBBoxX(minX, maxX);
            lines.UpdateContentBBoxY(yTop, yBottom);
            int margin = m_doc->GetDrawingUnit(100) / 2;
            system->m_systemAligner.FindAllIntersectionPoints(line, lines, { DIR, DYNAM, TEMPO }, margin);
        }
    }

    if (barLine->GetForm() == BARRENDITION_single) {
        DrawVerticalSegmentedLine(dc, x, line, barLineWidth);
    }
    else if (barLine->GetForm() == BARRENDITION_dashed) {
        DrawVerticalSegmentedLine(dc, x, line, barLineWidth, dashLength);
    }
    else if (barLine->GetForm() == BARRENDITION_rptend) {
        DrawVerticalSegmentedLine(dc, x1, line, barLineWidth);
        DrawVerticalSegmentedLine(dc, x, line, barLineThickWidth);
    }
    else if (barLine->GetForm() == BARRENDITION_rptboth) {
        DrawVerticalSegmentedLine(dc, x1, line, barLineWidth);
        DrawVerticalSegmentedLine(dc, x, line, barLineThickWidth);
        DrawVerticalSegmentedLine(dc, x2, line, barLineWidth);
    }
    else if (barLine->GetForm() == BARRENDITION_rptstart) {
        DrawVerticalSegmentedLine(dc, x, line, barLineThickWidth);
        DrawVerticalSegmentedLine(dc, x2, line, barLineWidth);
    }
    else if (barLine->GetForm() == BARRENDITION_invis) {
        barLine->SetEmptyBB();
    }
    else if (barLine->GetForm() == BARRENDITION_end) {
        DrawVerticalSegmentedLine(dc, x1, line, barLineWidth);
        DrawVerticalSegmentedLine(dc, x, line, barLineThickWidth);
    }
    else if (barLine->GetForm() == BARRENDITION_dbl) {
        // Narrow the bars a little bit - should be centered?
        x2 -= barLineWidth;
        DrawVerticalSegmentedLine(dc, x, line, barLineWidth);
        DrawVerticalSegmentedLine(dc, x2, line, barLineWidth);
    }
    else if (barLine->GetForm() == BARRENDITION_dbldashed) {
        x2 -= barLineWidth;
        DrawVerticalSegmentedLine(dc, x, line, barLineWidth, dashLength);
        DrawVerticalSegmentedLine(dc, x2, line, barLineWidth, dashLength);
    }
    else {
        // Use solid barline as fallback
        LogWarning("%s bar lines not supported", barLine->AttBarLineLog::BarrenditionToStr(barLine->GetForm()).c_str());
        DrawVerticalSegmentedLine(dc, x, line, barLineWidth);
    }
}

void View::DrawBarLineDots(DeviceContext *dc, StaffDef *staffDef, Staff *staff, BarLine *barLine)
{
    assert(dc);
    assert(staffDef);
    assert(staff);
    assert(barLine);

    int x = barLine->GetDrawingX();
    int x1 = x - 2 * m_doc->GetDrawingBeamWidth(100, false) - m_doc->GetDrawingBarLineWidth(staff->m_drawingStaffSize);
    int x2 = x + 2 * m_doc->GetDrawingBeamWidth(100, false) + m_doc->GetDrawingBarLineWidth(staff->m_drawingStaffSize);

    int yBottom = staff->GetDrawingY() - staffDef->GetLines() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    int yTop = yBottom + m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);

    if ((barLine->GetForm() == BARRENDITION_rptstart) || (barLine->GetForm() == BARRENDITION_rptboth)) {
        DrawDot(dc, x2, yBottom, staff->m_drawingStaffSize);
        DrawDot(dc, x2, yTop, staff->m_drawingStaffSize);
    }
    if ((barLine->GetForm() == BARRENDITION_rptend) || (barLine->GetForm() == BARRENDITION_rptboth)) {
        DrawDot(dc, x1, yBottom, staff->m_drawingStaffSize);
        DrawDot(dc, x1, yTop, staff->m_drawingStaffSize);
    }

    return;
}

//----------------------------------------------------------------------------
// View - Measure
//----------------------------------------------------------------------------

void View::DrawMeasure(DeviceContext *dc, Measure *measure, System *system)
{
    assert(dc);
    assert(measure);
    assert(system);

    // This is a special case where we do not draw (SVG, Bounding boxes, etc.) the measure for unmeasured music
    if (measure->IsMeasuredMusic()) {
        dc->StartGraphic(measure, "", measure->GetUuid());
    }

    if (m_drawingScoreDef.GetMnumVisible() != BOOLEAN_false) {
        MNum *mnum = dynamic_cast<MNum *>(measure->FindChildByType(MNUM));
        if (mnum) {
            // this should be an option
            Measure *systemStart = dynamic_cast<Measure *>(system->FindChildByType(MEASURE));
            if (measure == systemStart || !mnum->IsGenerated()) {
                // Draw measure numbers > 1
                if ((measure->GetN() != "0") && (measure->GetN()) != "1") {
                    DrawMNum(dc, mnum, measure);
                }
            }
        }
    }

    DrawMeasureChildren(dc, measure, measure, system);

    // Draw the barlines only with measured music
    if (measure->IsMeasuredMusic()) {
        if (measure->GetDrawingLeftBarLine() != BARRENDITION_NONE) {
            DrawScoreDef(
                dc, &m_drawingScoreDef, measure, measure->GetLeftBarLine()->GetDrawingX(), measure->GetLeftBarLine());
        }
        if (measure->GetDrawingRightBarLine() != BARRENDITION_NONE) {
            bool isLast = (measure == system->FindChildByType(MEASURE, 1, BACKWARD)) ? true : false;
            DrawScoreDef(dc, &m_drawingScoreDef, measure, measure->GetRightBarLine()->GetDrawingX(),
                measure->GetRightBarLine(), isLast);
        }
    }

    if (measure->IsMeasuredMusic()) {
        dc->EndGraphic(measure, this);
    }

    if (measure->GetDrawingEnding()) {
        system->AddToDrawingList(measure->GetDrawingEnding());
    }
}

void View::DrawMNum(DeviceContext *dc, MNum *mnum, Measure *measure)
{
    assert(dc);
    assert(measure);
    assert(mnum);

    Staff *staff = dynamic_cast<Staff *>(measure->FindChildByType(STAFF));
    if (staff) {

        dc->StartGraphic(mnum, "", mnum->GetUuid());

        FontInfo mnumTxt;
        if (!dc->UseGlobalStyling()) {
            mnumTxt.SetFaceName("Times");
            mnumTxt.SetStyle(FONTSTYLE_italic);
        }

        TextDrawingParams params;

        data_HORIZONTALALIGNMENT alignment = mnum->GetChildRendAlignment();
        // mNum are center aligned by default
        if (alignment == HORIZONTALALIGNMENT_NONE) alignment = HORIZONTALALIGNMENT_center;

        // HARDCODED
        // we set mNum to a fixed height above the system and make it a bit smaller than other text
        params.m_x = staff->GetDrawingX();
        params.m_y = staff->GetDrawingY() + 1.5 * m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
        params.m_pointSize = m_doc->GetDrawingLyricFont(staff->m_drawingStaffSize)->GetPointSize() * 4 / 5;

        mnumTxt.SetPointSize(params.m_pointSize);

        dc->SetBrush(m_currentColour, AxSOLID);
        dc->SetFont(&mnumTxt);

        dc->StartText(ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y), alignment);
        DrawTextChildren(dc, mnum, params);
        dc->EndText();

        dc->ResetFont();

        dc->EndGraphic(mnum, this);
    }
}

//----------------------------------------------------------------------------
// View - Staff
//----------------------------------------------------------------------------

void View::DrawStaff(DeviceContext *dc, Staff *staff, Measure *measure, System *system)
{
    assert(dc);
    assert(staff);
    assert(measure);
    assert(system);

    dc->StartGraphic(staff, "", staff->GetUuid());

    DrawStaffLines(dc, staff, measure, system);

    DrawStaffDef(dc, staff, measure);

    if (staff->GetLedgerLinesAbove()) {
        DrawLedgerLines(dc, staff, staff->GetLedgerLinesAbove(), false, false);
    }
    if (staff->GetLedgerLinesBelow()) {
        DrawLedgerLines(dc, staff, staff->GetLedgerLinesBelow(), true, false);
    }
    if (staff->GetLedgerLinesAboveCue()) {
        DrawLedgerLines(dc, staff, staff->GetLedgerLinesAboveCue(), false, true);
    }
    if (staff->GetLedgerLinesBelowCue()) {
        DrawLedgerLines(dc, staff, staff->GetLedgerLinesBelowCue(), true, true);
    }

    DrawStaffChildren(dc, staff, staff, measure);

    DrawStaffDefCautionary(dc, staff, measure);

    std::vector<Object *>::iterator iter;
    for (iter = staff->m_timeSpanningElements.begin(); iter != staff->m_timeSpanningElements.end(); ++iter) {
        system->AddToDrawingList(*iter);
    }

    dc->EndGraphic(staff, this);
}

void View::DrawStaffLines(DeviceContext *dc, Staff *staff, Measure *measure, System *system)
{
    assert(dc);
    assert(staff);
    assert(measure);
    assert(system);

    int j, x1, x2, y;

    y = staff->GetDrawingY();

    x1 = measure->GetDrawingX();
    x2 = x1 + measure->GetWidth();

    int lineWidth = m_doc->GetDrawingStaffLineWidth(staff->m_drawingStaffSize);
    dc->SetPen(m_currentColour, ToDeviceContextX(lineWidth), AxSOLID);
    dc->SetBrush(m_currentColour, AxSOLID);

    for (j = 0; j < staff->m_drawingLines; ++j) {
        dc->DrawLine(ToDeviceContextX(x1), ToDeviceContextY(y), ToDeviceContextX(x2), ToDeviceContextY(y));
        // For drawing rectangles instead of lines
        y -= m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
    }

    dc->ResetPen();
    dc->ResetBrush();

    return;
}

void View::DrawLedgerLines(DeviceContext *dc, Staff *staff, ArrayOfLedgerLines *lines, bool below, bool cueSize)
{
    assert(dc);
    assert(staff);
    assert(lines);

    std::string gClass = "above";
    int y = staff->GetDrawingY();
    int x = staff->GetDrawingX();
    int ySpace = m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);

    if (below) {
        gClass = "below";
        y -= ySpace * (staff->m_drawingLines - 1);
        ySpace = -ySpace;
    }
    y += ySpace;

    if (cueSize) {
        gClass += " cue";
    }

    dc->StartCustomGraphic("ledgerLines", gClass);

    // HARDCODED
    int lineWidth = m_doc->GetDrawingStaffLineWidth(staff->m_drawingStaffSize) * 1.75;
    if (cueSize) lineWidth = m_doc->GetDrawingStaffLineWidth(staff->m_drawingStaffSize) * 1.25;

    dc->SetPen(m_currentColour, ToDeviceContextX(lineWidth), AxSOLID);
    dc->SetBrush(m_currentColour, AxSOLID);

    ArrayOfLedgerLines::iterator iter;
    std::list<std::pair<int, int> >::iterator iterDashes;

    // First add the dash
    for (iter = lines->begin(); iter != lines->end(); ++iter) {
        for (iterDashes = (*iter).m_dashes.begin(); iterDashes != (*iter).m_dashes.end(); ++iterDashes) {
            dc->DrawLine(ToDeviceContextX(x + iterDashes->first), ToDeviceContextY(y),
                ToDeviceContextX(x + iterDashes->second), ToDeviceContextY(y));
        }
        y += ySpace;
    }

    dc->ResetPen();
    dc->ResetBrush();

    dc->EndCustomGraphic();

    return;
}

void View::DrawStaffDef(DeviceContext *dc, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(staff);
    assert(measure);

    // StaffDef information is always in the first layer
    Layer *layer = dynamic_cast<Layer *>(staff->FindChildByType(LAYER));
    if (!layer || !layer->HasStaffDef()) return;

    // StaffDef staffDef;
    // dc->StartGraphic(&staffDef, "", staffDef.GetUuid());

    // draw the scoreDef if required
    if (layer->GetStaffDefClef()) {
        DrawLayerElement(dc, layer->GetStaffDefClef(), layer, staff, measure);
    }
    if (layer->GetStaffDefKeySig()) {
        DrawLayerElement(dc, layer->GetStaffDefKeySig(), layer, staff, measure);
    }
    if (layer->GetStaffDefMensur()) {
        DrawLayerElement(dc, layer->GetStaffDefMensur(), layer, staff, measure);
    }
    if (layer->GetStaffDefMeterSig()) {
        DrawLayerElement(dc, layer->GetStaffDefMeterSig(), layer, staff, measure);
    }

    // dc->EndGraphic(&staffDef, this);
}

void View::DrawStaffDefCautionary(DeviceContext *dc, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(staff);
    assert(measure);

    // StaffDef cautionary information is always in the first layer
    Layer *layer = dynamic_cast<Layer *>(staff->FindChildByType(LAYER));
    if (!layer || !layer->HasCautionStaffDef()) return;

    // StaffDef staffDef;
    // dc->StartGraphic(&staffDef, "cautionary", staffDef.GetUuid());

    // draw the scoreDef if required
    if (layer->GetCautionStaffDefClef()) {
        DrawLayerElement(dc, layer->GetCautionStaffDefClef(), layer, staff, measure);
    }
    if (layer->GetCautionStaffDefKeySig()) {
        DrawLayerElement(dc, layer->GetCautionStaffDefKeySig(), layer, staff, measure);
    }
    if (layer->GetCautionStaffDefMensur()) {
        DrawLayerElement(dc, layer->GetCautionStaffDefMensur(), layer, staff, measure);
    }
    if (layer->GetCautionStaffDefMeterSig()) {
        DrawLayerElement(dc, layer->GetCautionStaffDefMeterSig(), layer, staff, measure);
    }

    // dc->EndGraphic(&staffDef, this);
}

//----------------------------------------------------------------------------
// View - Layer
//----------------------------------------------------------------------------

// a partir d'un y, trouve la hauteur d'une note exprimee en code touche et
// octave. Retourne code clavier, et situe l'octave.

int View::CalculatePitchCode(Layer *layer, int y_n, int x_pos, int *octave)
{
    assert(layer);
    assert(octave);

    Staff *parentStaff = dynamic_cast<Staff *>(layer->GetFirstParent(STAFF));
    assert(parentStaff); // Pointer to parent has to be a staff

    static int touches[]
        = { PITCHNAME_c, PITCHNAME_d, PITCHNAME_e, PITCHNAME_f, PITCHNAME_g, PITCHNAME_a, PITCHNAME_b };
    int y_dec, yb, plafond;
    int degres, octaves, position, code;

    int staffSize = parentStaff->m_drawingStaffSize;
    // calculer position du do central en fonction clef
    // y_n += (int) m_doc->GetDrawingUnit(staffSize) / 4;
    yb = parentStaff->GetDrawingY() - m_doc->GetDrawingStaffSize(staffSize); // UT1 default

    plafond = yb + 8 * m_doc->GetDrawingOctaveSize(staffSize);
    if (y_n > plafond) y_n = plafond;

    LayerElement *previous = NULL;
    LayerElement *pelement = layer->GetAtPos(x_pos);
    if ((previous = layer->GetPrevious(pelement))) pelement = previous;

    Clef *clef = layer->GetClef(pelement);
    if (clef) {
        yb += (clef->GetClefLocOffset()) * m_doc->GetDrawingUnit(staffSize); // UT1 reel
    }
    yb -= 4 * m_doc->GetDrawingOctaveSize(staffSize); // UT, note la plus grave

    y_dec = y_n - yb; // decalage par rapport a UT le plus grave

    if (y_dec < 0) y_dec = 0;

    degres = y_dec / m_doc->GetDrawingUnit(staffSize); // ecart en degres (PITCHNAME_c..PITCHNAME_b) par rapport a UT1
    octaves = degres / 7;
    position = degres % 7;

    code = touches[position];
    *octave = octaves /*- OCTBIT*/; // LP remove OCTBIT : oct 0 ‡ 7

    return (code);
}

void View::DrawLayer(DeviceContext *dc, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(layer);
    assert(staff);
    assert(measure);

    // first we need to clear the drawing list of postponed elements
    layer->ResetDrawingList();

    // Now start to draw the layer content

    dc->StartGraphic(layer, "", layer->GetUuid());

    DrawLayerChildren(dc, layer, layer, staff, measure);

    dc->EndGraphic(layer, this);

    // first draw the postponed tuplets
    DrawLayerList(dc, layer, staff, measure, TUPLET);
}

void View::DrawLayerList(DeviceContext *dc, Layer *layer, Staff *staff, Measure *measure, const ClassId classId)
{
    assert(dc);
    assert(layer);
    assert(staff);
    assert(measure);

    ListOfObjects *drawingList = layer->GetDrawingList();
    ListOfObjects::iterator iter;

    for (iter = drawingList->begin(); iter != drawingList->end(); ++iter) {
        if ((*iter)->Is(classId) && (classId == TUPLET)) {
            Tuplet *tuplet = dynamic_cast<Tuplet *>((*iter));
            assert(tuplet);
            dc->ResumeGraphic(tuplet, tuplet->GetUuid());
            DrawTupletPostponed(dc, tuplet, layer, staff);
            dc->EndResumedGraphic(tuplet, this);
        }
        else {
            // This should never happen
            LogError("Element '%s' in the layer list cannot be drawn", (*iter)->GetClassName().c_str());
        }
    }
}

//----------------------------------------------------------------------------
// View - Children
//----------------------------------------------------------------------------

void View::DrawSystemChildren(DeviceContext *dc, Object *parent, System *system)
{
    assert(dc);
    assert(parent);
    assert(system);

    Object *current;
    for (current = parent->GetFirst(); current; current = parent->GetNext()) {
        if (current->Is(MEASURE)) {
            // cast to Measure check in DrawMeasure
            DrawMeasure(dc, dynamic_cast<Measure *>(current), system);
        }
        // scoreDef are not drawn directly, but anything else should not be possible
        else if (current->Is(SCOREDEF)) {
            // nothing to do, then
            ScoreDef *scoreDef = dynamic_cast<ScoreDef *>(current);
            assert(scoreDef);
            SetScoreDefDrawingWidth(dc, scoreDef);
        }
        else if (current->IsSystemElement()) {
            // cast to EditorialElement check in DrawSystemEditorial element
            DrawSystemElement(dc, dynamic_cast<SystemElement *>(current), system);
        }
        else if (current->IsEditorialElement()) {
            // cast to EditorialElement check in DrawSystemEditorial element
            DrawSystemEditorialElement(dc, dynamic_cast<EditorialElement *>(current), system);
        }
        else {
            assert(false);
        }
    }
}

void View::DrawMeasureChildren(DeviceContext *dc, Object *parent, Measure *measure, System *system)
{
    assert(dc);
    assert(parent);
    assert(measure);
    assert(system);

    Object *current;
    for (current = parent->GetFirst(); current; current = parent->GetNext()) {
        if (current->Is(STAFF)) {
            // cast to Staff check in DrawStaff
            DrawStaff(dc, dynamic_cast<Staff *>(current), measure, system);
        }
        else if (current->IsControlElement()) {
            // cast to ControlElement check in DrawControlElement
            DrawControlElement(dc, dynamic_cast<ControlElement *>(current), measure, system);
        }
        else if (current->IsEditorialElement()) {
            // cast to EditorialElement check in DrawMeasureEditorialElement
            DrawMeasureEditorialElement(dc, dynamic_cast<EditorialElement *>(current), measure, system);
        }
        else {
            LogDebug("Current is %s", current->GetClassName().c_str());
            assert(false);
        }
    }
}

void View::DrawStaffChildren(DeviceContext *dc, Object *parent, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(parent);
    assert(staff);
    assert(measure);

    Object *current;
    for (current = parent->GetFirst(); current; current = parent->GetNext()) {
        if (current->Is(LAYER)) {
            // cast to Layer check in DrawLayer
            DrawLayer(dc, dynamic_cast<Layer *>(current), staff, measure);
        }
        else if (current->IsEditorialElement()) {
            // cast to EditorialElement check in DrawStaffEditorialElement
            DrawStaffEditorialElement(dc, dynamic_cast<EditorialElement *>(current), staff, measure);
        }
        else {
            assert(false);
        }
    }
}

void View::DrawLayerChildren(DeviceContext *dc, Object *parent, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(parent);
    assert(layer);
    assert(staff);
    assert(measure);

    Object *current;
    for (current = parent->GetFirst(); current; current = parent->GetNext()) {
        if (current->IsLayerElement()) {
            DrawLayerElement(dc, dynamic_cast<LayerElement *>(current), layer, staff, measure);
        }
        else if (current->IsEditorialElement()) {
            // cast to EditorialElement check in DrawLayerEditorialElement
            DrawLayerEditorialElement(dc, dynamic_cast<EditorialElement *>(current), layer, staff, measure);
        }
        else {
            assert(false);
        }
    }
}

void View::DrawTextChildren(DeviceContext *dc, Object *parent, TextDrawingParams &params)
{
    assert(dc);
    assert(parent);

    Object *current;
    for (current = parent->GetFirst(); current; current = parent->GetNext()) {
        if (current->IsTextElement()) {
            DrawTextElement(dc, dynamic_cast<TextElement *>(current), params);
        }
        else if (current->IsEditorialElement()) {
            // cast to EditorialElement check in DrawTextEditorialElement
            DrawTextEditorialElement(dc, dynamic_cast<EditorialElement *>(current), params);
        }
        else {
            assert(false);
        }
    }
}

void View::DrawFbChildren(DeviceContext *dc, Object *parent, TextDrawingParams &params)
{
    assert(dc);
    assert(parent);

    Object *current;
    for (current = parent->GetFirst(); current; current = parent->GetNext()) {
        if (current->IsTextElement()) {
            DrawTextElement(dc, dynamic_cast<TextElement *>(current), params);
        }
        else if (current->IsEditorialElement()) {
            // cast to EditorialElement check in DrawLayerEditorialElement
            DrawFbEditorialElement(dc, dynamic_cast<EditorialElement *>(current), params);
        }
        else {
            assert(false);
        }
    }
}

void View::DrawRunningChildren(DeviceContext *dc, Object *parent, TextDrawingParams &params)
{
    assert(dc);
    assert(parent);

    Object *current;
    for (current = parent->GetFirst(); current; current = parent->GetNext()) {
        if (current->Is(FIG)) {
            DrawFig(dc, dynamic_cast<Fig *>(current), params);
        }
        else if (current->IsTextElement()) {
            // We are now reaching a text element - start set only here because we can have a figure
            TextDrawingParams paramsChild = params;
            dc->StartText(ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y), HORIZONTALALIGNMENT_left);
            DrawTextElement(dc, dynamic_cast<TextElement *>(current), paramsChild);
            dc->EndText();
        }
        else if (current->IsEditorialElement()) {
            // cast to EditorialElement check in DrawLayerEditorialElement
            DrawRunningEditorialElement(dc, dynamic_cast<EditorialElement *>(current), params);
        }
        else {
            assert(false);
        }
    }
}

//----------------------------------------------------------------------------
// View - Editorial
//----------------------------------------------------------------------------

void View::DrawSystemEditorialElement(DeviceContext *dc, EditorialElement *element, System *system)
{
    assert(element);
    if (element->Is(ANNOT)) {
        DrawAnnot(dc, element);
        return;
    }
    if (element->Is(APP))
        assert((dynamic_cast<App *>(element))->GetLevel() == EDITORIAL_TOPLEVEL);
    else if (element->Is(CHOICE))
        assert((dynamic_cast<Choice *>(element))->GetLevel() == EDITORIAL_TOPLEVEL);

    std::string boundaryStart;
    if (element->IsBoundaryElement()) boundaryStart = "boundaryStart";

    dc->StartGraphic(element, boundaryStart, element->GetUuid());
    // EditorialElements at the system level that are visible have no children
    // if (element->m_visibility == Visible) {
    //    DrawSystemChildren(dc, element, system);
    //}
    dc->EndGraphic(element, this);
}

void View::DrawMeasureEditorialElement(DeviceContext *dc, EditorialElement *element, Measure *measure, System *system)
{
    assert(element);
    if (element->Is(ANNOT)) {
        DrawAnnot(dc, element);
        return;
    }
    if (element->Is(APP))
        assert((dynamic_cast<App *>(element))->GetLevel() == EDITORIAL_MEASURE);
    else if (element->Is(CHOICE))
        assert((dynamic_cast<Choice *>(element))->GetLevel() == EDITORIAL_MEASURE);

    dc->StartGraphic(element, "", element->GetUuid());
    if (element->m_visibility == Visible) {
        DrawMeasureChildren(dc, element, measure, system);
    }
    dc->EndGraphic(element, this);
}

void View::DrawStaffEditorialElement(DeviceContext *dc, EditorialElement *element, Staff *staff, Measure *measure)
{
    assert(element);
    if (element->Is(ANNOT)) {
        DrawAnnot(dc, element);
        return;
    }
    if (element->Is(APP))
        assert((dynamic_cast<App *>(element))->GetLevel() == EDITORIAL_STAFF);
    else if (element->Is(CHOICE))
        assert((dynamic_cast<Choice *>(element))->GetLevel() == EDITORIAL_STAFF);

    dc->StartGraphic(element, "", element->GetUuid());
    if (element->m_visibility == Visible) {
        DrawStaffChildren(dc, element, staff, measure);
    }
    dc->EndGraphic(element, this);
}

void View::DrawLayerEditorialElement(
    DeviceContext *dc, EditorialElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(element);
    if (element->Is(ANNOT)) {
        DrawAnnot(dc, element);
        return;
    }
    if (element->Is(APP))
        assert((dynamic_cast<App *>(element))->GetLevel() == EDITORIAL_LAYER);
    else if (element->Is(CHOICE))
        assert((dynamic_cast<Choice *>(element))->GetLevel() == EDITORIAL_LAYER);

    dc->StartGraphic(element, "", element->GetUuid());
    if (element->m_visibility == Visible) {
        DrawLayerChildren(dc, element, layer, staff, measure);
    }
    dc->EndGraphic(element, this);
}

void View::DrawTextEditorialElement(DeviceContext *dc, EditorialElement *element, TextDrawingParams &params)
{
    assert(element);
    if (element->Is(ANNOT)) {
        DrawAnnot(dc, element, true);
        return;
    }
    if (element->Is(APP))
        assert((dynamic_cast<App *>(element))->GetLevel() == EDITORIAL_TEXT);
    else if (element->Is(CHOICE))
        assert((dynamic_cast<Choice *>(element))->GetLevel() == EDITORIAL_TEXT);

    dc->StartTextGraphic(element, "", element->GetUuid());
    if (element->m_visibility == Visible) {
        DrawTextChildren(dc, element, params);
    }
    dc->EndTextGraphic(element, this);
}

void View::DrawFbEditorialElement(DeviceContext *dc, EditorialElement *element, TextDrawingParams &params)
{
    assert(element);
    if (element->Is(ANNOT)) {
        DrawAnnot(dc, element, true);
        return;
    }
    if (element->Is(APP))
        assert((dynamic_cast<App *>(element))->GetLevel() == EDITORIAL_FB);
    else if (element->Is(CHOICE))
        assert((dynamic_cast<Choice *>(element))->GetLevel() == EDITORIAL_FB);

    dc->StartTextGraphic(element, "", element->GetUuid());
    if (element->m_visibility == Visible) {
        DrawFbChildren(dc, element, params);
    }
    dc->EndTextGraphic(element, this);
}

void View::DrawRunningEditorialElement(DeviceContext *dc, EditorialElement *element, TextDrawingParams &params)
{
    assert(element);
    if (element->Is(ANNOT)) {
        DrawAnnot(dc, element, true);
        return;
    }
    if (element->Is(APP))
        assert((dynamic_cast<App *>(element))->GetLevel() == EDITORIAL_RUNNING);
    else if (element->Is(CHOICE))
        assert((dynamic_cast<Choice *>(element))->GetLevel() == EDITORIAL_RUNNING);

    dc->StartGraphic(element, "", element->GetUuid());
    if (element->m_visibility == Visible) {
        DrawRunningChildren(dc, element, params);
    }
    dc->EndGraphic(element, this);
}

void View::DrawAnnot(DeviceContext *dc, EditorialElement *element, bool isTextElement)
{
    assert(element);

    if (isTextElement) {
        dc->StartTextGraphic(element, "", element->GetUuid());
    }
    else {
        dc->StartGraphic(element, "", element->GetUuid());
    }

    Annot *annot = dynamic_cast<Annot *>(element);
    assert(annot);
    dc->AddDescription(UTF16to8(annot->GetText(annot)));

    if (isTextElement) {
        dc->EndTextGraphic(element, this);
    }
    else {
        dc->EndGraphic(element, this);
    }
}

} // namespace vrv

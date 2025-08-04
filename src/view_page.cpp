/////////////////////////////////////////////////////////////////////////////
// Name:        view_page.cpp
// Author:      Laurent Pugin and Chris Niven
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <math.h>
#include <sstream>

//----------------------------------------------------------------------------

#include "annot.h"
#include "app.h"
#include "beam.h"
#include "beamspan.h"
#include "choice.h"
#include "clef.h"
#include "comparison.h"
#include "controlelement.h"
#include "devicecontext.h"
#include "div.h"
#include "doc.h"
#include "editorial.h"
#include "ending.h"
#include "f.h"
#include "fb.h"
#include "fig.h"
#include "glyph.h"
#include "keysig.h"
#include "label.h"
#include "labelabbr.h"
#include "layer.h"
#include "layerdef.h"
#include "measure.h"
#include "mensur.h"
#include "metersig.h"
#include "mnum.h"
#include "note.h"
#include "options.h"
#include "page.h"
#include "pageelement.h"
#include "pagemilestone.h"
#include "reh.h"
#include "smufl.h"
#include "staff.h"
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
    assert(m_currentPage);

    // Ensure that resources are set
    const bool dcHasResources = dc->HasResources();
    if (!dcHasResources) dc->SetResources(&m_doc->GetResources());

    // Keep the width of the initial scoreDef
    this->SetScoreDefDrawingWidth(dc, &m_currentPage->m_drawingScoreDef);

    // Set the current score def to the page one
    // The page one has previously been set by the ScoreDefSetCurrentFunctor
    m_drawingScoreDef = m_currentPage->m_drawingScoreDef;

    if ((m_doc->GetAdjustedDrawingPageHeight() > dc->GetHeight()) && m_options->m_shrinkToFit.GetValue()) {
        dc->SetContentHeight(m_doc->GetAdjustedDrawingPageHeight());
    }
    else {
        dc->SetContentHeight(dc->GetHeight());
    }

    // if (background) dc->DrawRectangle(0, 0, m_doc->m_drawingPageWidth, m_doc->m_drawingPageHeight);
    dc->DrawBackgroundImage();

    Point origin = dc->GetLogicalOrigin();
    dc->SetLogicalOrigin(origin.x - m_doc->m_drawingPageMarginLeft, origin.y - m_doc->m_drawingPageMarginTop);

    dc->StartPage();

    for (Object *child : m_currentPage->GetChildren()) {
        if (child->IsPageElement()) {
            // cast to PageElement check in DrawSystemEditorial element
            this->DrawPageElement(dc, dynamic_cast<PageElement *>(child));
        }
        else if (child->Is(SYSTEM)) {
            System *system = dynamic_cast<System *>(child);
            this->DrawSystem(dc, system);
        }
        else {
            assert(false);
        }
    }

    this->DrawRunningElements(dc, m_currentPage);

    dc->EndPage();

    if (!dcHasResources) dc->ResetResources();
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
        KeySig *keySig = scoreDef->GetKeySig();
        assert(keySig);
        numAlteration = (keySig->GetAccidCount() > numAlteration) ? keySig->GetAccidCount() : numAlteration;
    }

    // longest key signature of the staffDefs
    const ListOfObjects &childList = scoreDef->GetList(); // make sure it's initialized
    for (Object *child : childList) {
        StaffDef *staffDef = vrv_cast<StaffDef *>(child);
        assert(staffDef);
        if (!staffDef->HasKeySigInfo()) continue;
        KeySig *keySig = staffDef->GetKeySig();
        assert(keySig);
        numAlteration = (keySig->GetAccidCount() > numAlteration) ? keySig->GetAccidCount() : numAlteration;
    }

    const int unit = m_doc->GetDrawingUnit(100);
    int width = 0;
    // G-clef as default width
    width += m_doc->GetGlyphWidth(SMUFL_E050_gClef, 100, false)
        + (m_doc->GetLeftMargin(CLEF) + m_doc->GetRightMargin(CLEF)) * unit;
    if (numAlteration > 0) {
        width += m_doc->GetGlyphWidth(SMUFL_E262_accidentalSharp, 100, false) * TEMP_KEYSIG_STEP
            + (m_doc->GetLeftMargin(KEYSIG) + m_doc->GetRightMargin(KEYSIG)) * unit;
    }

    scoreDef->SetDrawingWidth(width);
}

void View::DrawPageElement(DeviceContext *dc, PageElement *element)
{
    assert(dc);
    assert(element);

    if (element->Is(PAGE_MILESTONE_END)) {
        PageMilestoneEnd *elementEnd = vrv_cast<PageMilestoneEnd *>(element);
        assert(elementEnd);
        assert(elementEnd->GetStart());
        dc->StartGraphic(element, elementEnd->GetStart()->GetID(), element->GetID());
        dc->EndGraphic(element, this);
    }
    else if (element->Is(MDIV)) {
        // When the mdiv is not visible, then there is no start / end element
        std::string elementStart = (element->IsMilestoneElement()) ? "pageMilestone" : "";
        dc->StartGraphic(element, elementStart, element->GetID());
        dc->EndGraphic(element, this);
    }
    else if (element->Is(SCORE)) {
        dc->StartGraphic(element, "pageMilestone", element->GetID());
        dc->EndGraphic(element, this);
    }
}

//----------------------------------------------------------------------------
// View - System
//----------------------------------------------------------------------------

void View::DrawSystem(DeviceContext *dc, System *system)
{
    assert(dc);
    assert(system);

    dc->StartGraphic(system, "", system->GetID());

    Measure *firstMeasure = vrv_cast<Measure *>(system->FindDescendantByType(MEASURE, 1));

    this->DrawSystemDivider(dc, system, firstMeasure);

    // first we need to clear the drawing list of postponed elements
    system->ResetDrawingList();

    if (firstMeasure) {
        this->DrawScoreDef(dc, system->GetDrawingScoreDef(), firstMeasure, system->GetDrawingX(), NULL);
    }

    this->DrawSystemChildren(dc, system, system);

    this->DrawSystemList(dc, system, SYL);
    this->DrawSystemList(dc, system, ANNOTSCORE);
    this->DrawSystemList(dc, system, BEAMSPAN);
    this->DrawSystemList(dc, system, BRACKETSPAN);
    this->DrawSystemList(dc, system, DYNAM);
    this->DrawSystemList(dc, system, DIR);
    this->DrawSystemList(dc, system, GLISS);
    this->DrawSystemList(dc, system, HAIRPIN);
    this->DrawSystemList(dc, system, TRILL);
    this->DrawSystemList(dc, system, FIGURE);
    this->DrawSystemList(dc, system, LV);
    this->DrawSystemList(dc, system, PHRASE);
    this->DrawSystemList(dc, system, OCTAVE);
    this->DrawSystemList(dc, system, ORNAM);
    this->DrawSystemList(dc, system, PEDAL);
    this->DrawSystemList(dc, system, PITCHINFLECTION);
    this->DrawSystemList(dc, system, TEMPO);
    this->DrawSystemList(dc, system, TIE);
    this->DrawSystemList(dc, system, SLUR);
    this->DrawSystemList(dc, system, ENDING);

    dc->EndGraphic(system, this);
}

void View::DrawSystemList(DeviceContext *dc, System *system, const ClassId classId)
{
    assert(dc);
    assert(system);

    ArrayOfObjects *drawingList = system->GetDrawingList();

    for (Object *object : *drawingList) {
        static const std::set<ClassId> timeSpanningClasses = { ANNOTSCORE, BEAMSPAN, BRACKETSPAN, DIR, DYNAM, FIGURE,
            GLISS, HAIRPIN, LV, OCTAVE, ORNAM, PEDAL, PHRASE, PITCHINFLECTION, SLUR, SYL, TEMPO, TIE, TRILL };
        if (object->Is(classId) && timeSpanningClasses.contains(classId)) {
            this->DrawTimeSpanningElement(dc, object, system);
        }
        if (object->Is(classId) && (classId == ENDING)) {
            // cast to Ending check in DrawEnding
            this->DrawEnding(dc, dynamic_cast<Ending *>(object), system);
        }
    }
}

void View::DrawScoreDef(DeviceContext *dc, ScoreDef *scoreDef, Measure *measure, int x, BarLine *barLine,
    bool isLastMeasure, bool isLastSystem)
{
    assert(dc);
    assert(scoreDef);
    // we need at least one measure to be able to draw the groups - we need access to the staff elements,
    assert(measure);

    StaffGrp *staffGrp = vrv_cast<StaffGrp *>(scoreDef->FindDescendantByType(STAFFGRP));
    if (!staffGrp) {
        return;
    }

    if (barLine == NULL) {
        // Draw the first staffGrp and from there its children recursively
        this->DrawStaffGrp(dc, measure, staffGrp, x, true, !scoreDef->DrawLabels());
    }
    else {
        dc->StartGraphic(barLine, "", barLine->GetID());
        int yBottomPrevious = VRV_UNSET;
        this->DrawBarLines(dc, measure, staffGrp, barLine, isLastMeasure, isLastSystem, yBottomPrevious);
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

    if (staffGrp->GetDrawingVisibility() == OPTIMIZATION_HIDDEN) {
        return;
    }

    auto [firstDef, lastDef] = staffGrp->GetFirstLastStaffDef();

    // Get the first and last staffDef of the staffGrp
    if (!firstDef || !lastDef) {
        LogDebug("Could not get staffDef while drawing staffGrp - DrawStaffGrp");
        return;
    }

    // Get the corresponding staff looking at the previous (or first) measure
    AttNIntegerComparison comparisonFirst(STAFF, firstDef->GetN());
    Staff *first = vrv_cast<Staff *>(measure->FindDescendantByComparison(&comparisonFirst, 1));
    AttNIntegerComparison comparisonLast(STAFF, lastDef->GetN());
    Staff *last = vrv_cast<Staff *>(measure->FindDescendantByComparison(&comparisonLast, 1));

    if (!first || !last) {
        LogDebug(
            "Could not get staff (%d; %d) while drawing staffGrp - DrawStaffGrp", firstDef->GetN(), lastDef->GetN());
        return;
    }

    const int staffSize = staffGrp->GetMaxStaffSize();
    int yTop = first->GetDrawingY();
    // for the bottom position we need to take into account the number of lines and the staff size
    int yBottom
        = last->GetDrawingY() - (lastDef->GetLines() - 1) * m_doc->GetDrawingDoubleUnit(last->m_drawingStaffSize);
    // adjust to single line staves
    if (firstDef->GetLines() <= 1) yTop += m_doc->GetDrawingDoubleUnit(last->m_drawingStaffSize);
    if (lastDef->GetLines() <= 1) yBottom -= m_doc->GetDrawingDoubleUnit(last->m_drawingStaffSize);

    // draw the system start bar line
    ScoreDef *scoreDef = vrv_cast<ScoreDef *>(staffGrp->GetFirstAncestor(SCOREDEF));
    if (topStaffGrp) {
        if (scoreDef && scoreDef->HasSystemStartLine()) {
            const int barLineWidth = m_doc->GetDrawingBarLineWidth(staffSize);
            this->DrawVerticalLine(dc, yTop, yBottom, x + barLineWidth / 2, barLineWidth);
        }
    }

    // draw the group symbol
    const int staffGrpX = x;
    this->DrawGrpSym(dc, measure, staffGrp, x);
    const int grpSymSpace = staffGrpX - x;

    // recursively draw the children
    StaffGrp *childStaffGrp = NULL;
    for (int i = 0; i < staffGrp->GetChildCount(); ++i) {
        childStaffGrp = dynamic_cast<StaffGrp *>(staffGrp->GetChild(i));
        if (childStaffGrp) {
            this->DrawStaffGrp(dc, measure, childStaffGrp, x, false, abbreviations);
        }
    }

    // DrawStaffGrpLabel
    const int space = m_doc->GetDrawingDoubleUnit(staffGrp->GetMaxStaffSize());
    const int xLabel = x - space;
    const int yLabel = yBottom - (yBottom - yTop) / 2 - m_doc->GetDrawingUnit(100);
    this->DrawLabels(dc, scoreDef, staffGrp, xLabel, yLabel, abbreviations, 100, 2 * space + grpSymSpace);

    this->DrawStaffDefLabels(dc, measure, staffGrp, x, abbreviations);
}

void View::DrawStaffDefLabels(DeviceContext *dc, Measure *measure, StaffGrp *staffGrp, int x, bool abbreviations)
{
    assert(dc);
    assert(measure);
    assert(staffGrp);

    StaffDef *staffDef = NULL;
    for (int i = 0; i < staffGrp->GetChildCount(); ++i) {
        staffDef = dynamic_cast<StaffDef *>(staffGrp->GetChild(i));

        if (!staffDef) {
            continue;
        }

        AttNIntegerComparison comparison(STAFF, staffDef->GetN());
        Staff *staff = vrv_cast<Staff *>(measure->FindDescendantByComparison(&comparison, 1));
        ScoreDef *scoreDef = vrv_cast<ScoreDef *>(staffGrp->GetFirstAncestor(SCOREDEF));

        if (!staff || !scoreDef) {
            LogDebug("Staff or ScoreDef missing in View::DrawStaffDefLabels");
            continue;
        }

        if (!staff->DrawingIsVisible()) {
            continue;
        }

        // HARDCODED
        const int doubleUnit = m_doc->GetDrawingDoubleUnit(staffGrp->GetMaxStaffSize());
        const int space = doubleUnit;
        const int y = staff->GetDrawingY() - (staffDef->GetLines() * doubleUnit / 2);

        const int staffSize = staff->GetDrawingStaffNotationSize();
        int adjust = 0;
        if (staffDef->HasLayerDefWithLabel()) adjust = 3 * doubleUnit;
        this->DrawLabels(
            dc, scoreDef, staffDef, x - doubleUnit - adjust, y, abbreviations, staffSize, 2 * space + adjust);

        this->DrawLayerDefLabels(dc, scoreDef, staff, staffDef, x, abbreviations);
    }
}

void View::DrawGrpSym(DeviceContext *dc, Measure *measure, StaffGrp *staffGrp, int &x)
{
    // draw the group symbol
    GrpSym *groupSymbol = vrv_cast<GrpSym *>(staffGrp->GetGroupSymbol());
    if (!groupSymbol) return;

    // Get the corresponding staff looking at the previous (or first) measure
    AttNIntegerComparison comparisonFirst(STAFF, groupSymbol->GetStartDef()->GetN());
    Staff *first = vrv_cast<Staff *>(measure->FindDescendantByComparison(&comparisonFirst, 1));
    AttNIntegerComparison comparisonLast(STAFF, groupSymbol->GetEndDef()->GetN());
    Staff *last = vrv_cast<Staff *>(measure->FindDescendantByComparison(&comparisonLast, 1));

    if (!first || !last) {
        LogDebug("Could not get staff (%d; %d) while drawing staffGrp - DrawStaffGrp",
            groupSymbol->GetStartDef()->GetN(), groupSymbol->GetEndDef()->GetN());
        return;
    }

    dc->StartGraphic(groupSymbol, "", groupSymbol->GetID());

    const int staffSize = staffGrp->GetMaxStaffSize();
    int yTop = first->GetDrawingY();
    // for the bottom position we need to take into account the number of lines and the staff size
    int yBottom = last->GetDrawingY()
        - (groupSymbol->GetEndDef()->GetLines() - 1) * m_doc->GetDrawingDoubleUnit(last->m_drawingStaffSize);
    // for the bottom position we need to take into account the number of lines and the staff size
    if (groupSymbol->GetStartDef()->GetLines() <= 1) yTop += m_doc->GetDrawingDoubleUnit(last->m_drawingStaffSize);
    if (groupSymbol->GetEndDef()->GetLines() <= 1) yBottom -= m_doc->GetDrawingDoubleUnit(last->m_drawingStaffSize);

    switch (groupSymbol->GetSymbol()) {
        case staffGroupingSym_SYMBOL_line: {
            const int lineWidth = m_doc->GetDrawingUnit(staffSize) * m_options->m_bracketThickness.GetValue();
            const int yOffset = m_doc->GetDrawingUnit(staffSize) * m_options->m_staffLineWidth.GetValue() / 2;
            this->DrawVerticalLine(dc, yTop + yOffset, yBottom - yOffset, x - 1.5 * lineWidth, lineWidth);
            x -= 2 * lineWidth;
            break;
        }
        case staffGroupingSym_SYMBOL_brace: {
            this->DrawBrace(dc, x, yTop, yBottom, staffSize);
            x -= 2.5 * m_doc->GetDrawingUnit(staffSize);
            break;
        }
        case staffGroupingSym_SYMBOL_bracket: {
            this->DrawBracket(dc, x, yTop, yBottom, staffSize);
            x -= m_doc->GetDrawingUnit(staffSize) * (1.0 + m_options->m_bracketThickness.GetValue());
            break;
        }
        case staffGroupingSym_SYMBOL_bracketsq: {
            this->DrawBracketSq(dc, x, yTop, yBottom, staffSize);
            x -= m_doc->GetDrawingUnit(staffSize);
            break;
        }
        default: break;
    }

    dc->EndGraphic(groupSymbol, this);
}

void View::DrawLayerDefLabels(
    DeviceContext *dc, ScoreDef *scoreDef, Staff *staff, StaffDef *staffDef, int x, bool abbreviations)
{
    assert(dc);
    assert(staff);
    assert(staffDef);

    // constants
    const int space = m_doc->GetDrawingDoubleUnit(scoreDef->GetMaxStaffSize());
    const int yCenter
        = staff->GetDrawingY() - (staffDef->GetLines() * m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 2);
    const int staffSize = staff->GetDrawingStaffNotationSize();
    const int pointSize = m_doc->GetDrawingLyricFont(staffSize)->GetPointSize();
    const int layerDefCount = staffDef->GetChildCount(LAYERDEF);
    const int requiredSpace = pointSize * layerDefCount;

    int initialY = yCenter + (requiredSpace - pointSize) / 2;
    for (int i = 0; i < layerDefCount; ++i) {
        LayerDef *layerDef = vrv_cast<LayerDef *>(staffDef->GetChild(i, LAYERDEF));
        if (!layerDef) continue;

        this->DrawLabels(dc, scoreDef, layerDef, x - space, initialY, abbreviations, staffSize, space);
        initialY -= pointSize;
    }
}

void View::DrawLabels(
    DeviceContext *dc, ScoreDef *scoreDef, Object *object, int x, int y, bool abbreviations, int staffSize, int space)
{
    assert(dc);
    assert(scoreDef);
    assert(object->Is({ LAYERDEF, STAFFDEF, STAFFGRP }));

    Label *label = vrv_cast<Label *>(object->FindDescendantByType(LABEL, 1));
    LabelAbbr *labelAbbr = vrv_cast<LabelAbbr *>(object->FindDescendantByType(LABELABBR, 1));
    Object *graphic = label;

    std::u32string labelStr = label ? label->GetText() : U"";
    std::u32string labelAbbrStr = labelAbbr ? labelAbbr->GetText() : U"";

    if (abbreviations) {
        labelStr = labelAbbrStr;
        graphic = labelAbbr;
    }

    if (!graphic || (labelStr.length() == 0)) {
        return;
    }

    FontInfo labelTxt;
    if (!dc->UseGlobalStyling()) {
        labelTxt.SetFaceName(m_doc->GetResources().GetTextFont());
    }
    labelTxt.SetPointSize(m_doc->GetDrawingLyricFont(staffSize)->GetPointSize());

    int lineCount = graphic->GetChildCount(LB) + 1;
    if (lineCount > 1) {
        y += (m_doc->GetTextLineHeight(&labelTxt, false) * (lineCount - 1) / 2);
    }

    TextDrawingParams params;
    params.m_x = x;
    params.m_y = y;
    params.m_pointSize = labelTxt.GetPointSize();

    dc->SetFont(&labelTxt);

    dc->StartGraphic(graphic, "", graphic->GetID());

    dc->StartText(this->ToDeviceContextX(params.m_x), this->ToDeviceContextY(params.m_y), HORIZONTALALIGNMENT_right);
    this->DrawTextChildren(dc, graphic, params);
    dc->EndText();

    dc->EndGraphic(graphic, this);

    // keep the widest width for the system - careful: this can be the label OR labelAbbr
    scoreDef->SetDrawingLabelsWidth(graphic->GetContentX2() - graphic->GetContentX1() + space);
    // also store in the system the maximum width with abbreviations for justification
    if (labelAbbr && !abbreviations && (labelAbbrStr.length() > 0)) {
        TextExtend extend;
        std::vector<std::u32string> lines;
        labelAbbr->GetTextLines(lines);
        int maxLength = 0;
        for (std::u32string &line : lines) {
            dc->GetTextExtent(line, &extend, true);
            maxLength = (extend.m_width > maxLength) ? extend.m_width : maxLength;
        }
        System *system = vrv_cast<System *>(scoreDef->GetFirstAncestor(SYSTEM));
        assert(system);
        system->SetDrawingAbbrLabelsWidth(maxLength + space);
    }

    dc->ResetFont();
}

void View::DrawBracket(DeviceContext *dc, int x, int y1, int y2, int staffSize)
{
    assert(dc);

    const int offset = m_doc->GetDrawingStaffLineWidth(staffSize) / 2;
    const int basicDist = m_doc->GetDrawingUnit(staffSize);

    const int bracketThickness = m_doc->GetDrawingUnit(staffSize) * m_options->m_bracketThickness.GetValue();

    const int x2 = x - basicDist;
    const int x1 = x2 - bracketThickness;

    this->DrawSmuflCode(dc, x1, y1 + offset + bracketThickness / 2, SMUFL_E003_bracketTop, staffSize, false);
    this->DrawSmuflCode(dc, x1, y2 - offset - bracketThickness / 2, SMUFL_E004_bracketBottom, staffSize, false);

    this->DrawFilledRectangle(
        dc, x1, y1 + 2 * offset + bracketThickness / 2, x2, y2 - 2 * offset - bracketThickness / 2);
}

void View::DrawBracketSq(DeviceContext *dc, int x, int y1, int y2, int staffSize)
{
    assert(dc);

    const int y = std::min(y1, y2);
    const int height = std::abs(y2 - y1);
    const int horizontalThickness = m_doc->GetDrawingStaffLineWidth(staffSize);
    const int verticalThickness = m_doc->GetDrawingUnit(staffSize) * m_options->m_subBracketThickness.GetValue();
    const int width = m_doc->GetDrawingUnit(staffSize);

    this->DrawSquareBracket(dc, true, x - width, y, height, width, horizontalThickness, verticalThickness);
}

void View::DrawBrace(DeviceContext *dc, int x, int y1, int y2, int staffSize)
{
    assert(dc);

    const int basicDist = m_doc->GetDrawingUnit(staffSize);

    x -= basicDist;

    if (m_doc->GetOptions()->m_useBraceGlyph.GetValue()) {
        FontInfo *font = m_doc->GetDrawingSmuflFont(staffSize, false);
        const int width = m_doc->GetGlyphWidth(SMUFL_E000_brace, staffSize, false);
        const int height = 8 * m_doc->GetDrawingUnit(staffSize);
        const float scale = static_cast<float>(y1 - y2) / height;
        // We want the brace width always to be 2 units
        const int braceWidth = m_doc->GetDrawingDoubleUnit(staffSize);
        x -= braceWidth;
        const float currentWidthToHeightRatio = font->GetWidthToHeightRatio();
        const float widthAfterScalling = width * scale;
        font->SetWidthToHeightRatio(static_cast<float>(braceWidth) / widthAfterScalling);
        this->DrawSmuflCode(dc, x, y2, SMUFL_E000_brace, staffSize * scale, false);
        font->SetWidthToHeightRatio(currentWidthToHeightRatio);
        return;
    }
    // int new_coords[2][6];
    Point points[4];
    Point bez1[4];
    Point bez2[4];

    const int penWidth = m_doc->GetDrawingStemWidth(staffSize);
    y1 -= penWidth;
    y2 += penWidth;
    x += penWidth;
    std::swap(y1, y2);

    const int fact = m_doc->GetDrawingBeamWhiteWidth(staffSize, false) + m_doc->GetDrawingStemWidth(staffSize);
    const int xdec = this->ToDeviceContextX(fact);
    const int ymed = (y1 + y2) / 2;

    points[0].x = this->ToDeviceContextX(x);
    points[0].y = this->ToDeviceContextY(y1);
    points[1].x = this->ToDeviceContextX(x - m_doc->GetDrawingDoubleUnit(staffSize) * 2);
    points[1].y = points[0].y - this->ToDeviceContextX(m_doc->GetDrawingDoubleUnit(staffSize) * 3);
    points[3].x = this->ToDeviceContextX(x - m_doc->GetDrawingDoubleUnit(staffSize));
    points[3].y = this->ToDeviceContextY(ymed);
    points[2].x = this->ToDeviceContextX(x + m_doc->GetDrawingUnit(staffSize));
    points[2].y = points[3].y + this->ToDeviceContextX(m_doc->GetDrawingDoubleUnit(staffSize));

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

    dc->SetPen(std::max(1, penWidth), PEN_SOLID);

    dc->DrawCubicBezierPathFilled(bez1, bez2);

    // on produit l'image reflet vers le bas: 0 est identique
    points[0].y = this->ToDeviceContextY(y2);
    points[1].y = points[0].y + this->ToDeviceContextX(m_doc->GetDrawingDoubleUnit(staffSize) * 3);
    points[3].y = this->ToDeviceContextY(ymed);
    points[2].y = points[3].y - this->ToDeviceContextX(m_doc->GetDrawingDoubleUnit(staffSize));

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

    dc->DrawCubicBezierPathFilled(bez1, bez2);

    dc->ResetPen();

    return;
}

void View::DrawBarLines(DeviceContext *dc, Measure *measure, StaffGrp *staffGrp, BarLine *barLine, bool isLastMeasure,
    bool isLastSystem, int &yBottomPrevious)
{
    assert(dc);
    assert(measure);
    assert(staffGrp);
    assert(barLine);

    if (staffGrp->GetDrawingVisibility() == OPTIMIZATION_HIDDEN) {
        return;
    }

    const bool barlineThrough = barLine->IsDrawnThrough(staffGrp);

    for (int i = 0; i < staffGrp->GetChildCount(); ++i) {
        Object *child = staffGrp->GetChild(i);

        // Recursive call for staff group
        if (child->Is(STAFFGRP)) {
            StaffGrp *childStaffGrp = vrv_cast<StaffGrp *>(child);
            this->DrawBarLines(dc, measure, childStaffGrp, barLine, isLastMeasure, isLastSystem, yBottomPrevious);
            if (!barlineThrough) yBottomPrevious = VRV_UNSET;
            continue;
        }

        // Determine the staff def
        if (!child->Is(STAFFDEF)) continue;
        StaffDef *staffDef = vrv_cast<StaffDef *>(child);
        assert(staffDef);
        if (staffDef->GetDrawingVisibility() == OPTIMIZATION_HIDDEN) {
            continue;
        }

        // Determine the barline form
        data_BARRENDITION form = barLine->GetForm();
        if (!barlineThrough && (measure->HasInvisibleStaffBarlines())) {
            data_BARRENDITION barlineRend = (barLine->GetPosition() == BarLinePosition::Right)
                ? measure->GetDrawingRightBarLineByStaffN(staffDef->GetN())
                : measure->GetDrawingLeftBarLineByStaffN(staffDef->GetN());
            if (barlineRend != BARRENDITION_NONE) form = barlineRend;
        }
        if (form == BARRENDITION_NONE) {
            yBottomPrevious = VRV_UNSET;
            continue;
        }

        // Determine the method
        const auto [hasMethod, method] = barLine->GetMethodFromContext(staffDef);
        const bool methodMensur = hasMethod && (method == BARMETHOD_mensur);
        const bool methodTakt = hasMethod && (method == BARMETHOD_takt);

        // Get the corresponding staff
        AttNIntegerComparison comparison(STAFF, staffDef->GetN());
        Staff *staff = vrv_cast<Staff *>(measure->FindDescendantByComparison(&comparison, 1));
        if (!staff) {
            LogDebug("Could not get staff (%d) while drawing staffGrp - DrawBarLines", staffDef->GetN());
            yBottomPrevious = VRV_UNSET;
            continue;
        }
        if (!barlineThrough && (staff->GetVisible() == BOOLEAN_false)) {
            yBottomPrevious = VRV_UNSET;
            continue;
        }
        const int unit = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);

        // For the bottom position we need to take into account the number of lines and the staff size
        const int yStaffTop = staff->GetDrawingY();
        const int yStaffBottom = yStaffTop - 2 * (staffDef->GetLines() - 1) * unit;
        int yBottom = yStaffBottom;
        int yLength = yStaffTop - yStaffBottom;

        // Adjust start and length
        if (!methodMensur && !methodTakt) {
            const auto [hasPlace, place] = barLine->GetPlaceFromContext(staffDef);
            if (hasPlace) {
                // bar.place counts upwards (note order).
                yBottom += place * unit;
            }
            else if (staffDef->GetLines() <= 1) {
                // Make sure barlines are visible with a single line
                yBottom -= 2 * unit;
            }

            const auto [hasLength, length] = barLine->GetLengthFromContext(staffDef);
            if (hasLength) {
                yLength = length * unit;
            }
            else if (staffDef->GetLines() <= 1) {
                yLength = 4 * unit;
            }
        }
        int yTop = yBottom + yLength;

        // Shift the taktstrich outwards?
        const int yTaktstrichShift = methodMensur ? unit : 0;

        // Determine which parts to draw
        bool drawInsideStaff = !methodMensur && !methodTakt;
        bool drawOutsideStaff = !methodTakt && barlineThrough;
        bool drawTaktstrichAbove = (methodMensur && !barlineThrough) || methodTakt;
        bool drawTaktstrichBelow = methodMensur && !barlineThrough;
        if ((isLastMeasure && isLastSystem) || barLine->HasRepetitionDots()) {
            drawInsideStaff = true;
            drawTaktstrichAbove = false;
            drawTaktstrichBelow = false;
        }

        // Now draw the barline part inside the staff
        if (drawInsideStaff) {
            this->DrawBarLine(dc, yTop, yBottom, barLine, form);
            if (barLine->HasRepetitionDots()) {
                this->DrawBarLineDots(dc, staff, barLine);
            }
        }

        // ... and the barline part outside the staff
        if (drawOutsideStaff && (yBottomPrevious != VRV_UNSET)) {
            // Do not erase intersections with right barline of the last measure of the system
            const bool eraseIntersections = !isLastMeasure || (barLine->GetPosition() != BarLinePosition::Right);

            this->DrawBarLine(dc, yBottomPrevious, yTop, barLine, form, true, eraseIntersections);
        }
        yBottomPrevious = drawOutsideStaff ? yBottom : VRV_UNSET;

        // Draw the taktstrich above the staff
        if (drawTaktstrichAbove) {
            const int yTaktstrichCenter = yStaffTop + yTaktstrichShift;
            this->DrawBarLine(dc, yTaktstrichCenter + unit, yTaktstrichCenter - unit, barLine, form);
        }

        // Draw the taktstrich below the staff
        if (drawTaktstrichBelow) {
            const int yTaktstrichCenter = yStaffBottom - yTaktstrichShift;
            this->DrawBarLine(dc, yTaktstrichCenter + unit, yTaktstrichCenter - unit, barLine, form);
        }
    }
}

void View::DrawBarLine(DeviceContext *dc, int yTop, int yBottom, BarLine *barLine, data_BARRENDITION form,
    bool inStaffSpace, bool eraseIntersections)
{
    assert(dc);
    assert(barLine);

    Staff *staff = barLine->GetAncestorStaff(ANCESTOR_ONLY, false);
    const int staffSize = (staff) ? staff->m_drawingStaffSize : 100;
    const int unit = m_doc->GetDrawingUnit(staffSize);

    const int x = barLine->GetDrawingX();
    const int barLineWidth = m_doc->GetDrawingBarLineWidth(staffSize);
    const int barLineThickWidth = unit * m_options->m_thickBarlineThickness.GetValue();
    const int barLineSeparation = unit * m_options->m_barLineSeparation.GetValue();
    const int barLinesSum = barLineThickWidth + barLineWidth;
    int x2 = x + barLineSeparation;

    const int dashLength = unit * m_options->m_dashedBarLineDashLength.GetValue();
    const int gapLength = unit * m_options->m_dashedBarLineGapLength.GetValue();
    if (inStaffSpace && ((form == BARRENDITION_dashed) || (form == BARRENDITION_dbldashed))) {
        // Dashed lines in staff space should start with a gap
        yTop -= dashLength;
        yBottom += dashLength;
    }
    const int serpentWidth = m_doc->GetGlyphWidth(SMUFL_E04A_segnoSerpent1, staffSize, false);

    SegmentedLine line(yTop, yBottom);
    // We do not need to do this during layout calculation
    if (eraseIntersections && !dc->Is(BBOX_DEVICE_CONTEXT)) {
        System *system = vrv_cast<System *>(barLine->GetFirstAncestor(SYSTEM));
        if (system) {
            int minX = x - barLineWidth / 2;
            int maxX = x + barLineWidth / 2;
            if ((form == BARRENDITION_rptend) || (form == BARRENDITION_end)) {
                maxX = x2 + barLinesSum / 2;
            }
            else if (form == BARRENDITION_heavy) {
                minX = x - barLineThickWidth / 2;
                maxX = x + barLineThickWidth / 2;
            }
            else if (form == BARRENDITION_rptboth) {
                maxX = x + barLinesSum + barLineSeparation * 2;
            }
            else if (form == BARRENDITION_rptstart) {
                minX = x - barLineThickWidth / 2;
                maxX = x2 + barLinesSum / 2;
            }
            else if ((form == BARRENDITION_dbl) || (form == BARRENDITION_dbldashed)
                || (form == BARRENDITION_dbldotted)) {
                maxX = x2 + barLineWidth / 2;
            }
            else if (form == BARRENDITION_dblheavy) {
                minX = x - barLineThickWidth / 2;
                maxX = x2 + barLineThickWidth / 2;
            }
            Object lines;
            lines.SetParent(system);
            lines.UpdateContentBBoxX(minX, maxX);
            lines.UpdateContentBBoxY(yTop, yBottom);
            const int margin = unit / 2;
            system->m_systemAligner.FindAllIntersectionPoints(line, lines, { CPMARK, DIR, DYNAM, TEMPO }, margin);
        }
    }

    switch (form) {
        case BARRENDITION_NONE: //
            [[fallthrough]];
        case BARRENDITION_single: //
            this->DrawVerticalSegmentedLine(dc, x, line, barLineWidth);
            break;
        case BARRENDITION_dashed: //
            this->DrawVerticalSegmentedLine(dc, x, line, barLineWidth, dashLength, gapLength);
            break;
        case BARRENDITION_dotted: //
            this->DrawVerticalDots(dc, x, line, barLineWidth, 2 * unit);
            break;
        case BARRENDITION_heavy: //
            this->DrawVerticalSegmentedLine(dc, x, line, barLineThickWidth);
            break;
        case BARRENDITION_rptend:
            this->DrawVerticalSegmentedLine(dc, x, line, barLineWidth);
            this->DrawVerticalSegmentedLine(dc, x2 + barLinesSum / 2, line, barLineThickWidth);
            break;
        case BARRENDITION_rptboth:
            x2 = x + barLinesSum + barLineSeparation * 2;
            this->DrawVerticalSegmentedLine(dc, x, line, barLineWidth);
            this->DrawVerticalSegmentedLine(dc, (x + x2) / 2, line, barLineThickWidth);
            this->DrawVerticalSegmentedLine(dc, x2, line, barLineWidth);
            break;
        case BARRENDITION_rptstart:
            this->DrawVerticalSegmentedLine(dc, x, line, barLineThickWidth);
            this->DrawVerticalSegmentedLine(dc, x2 + barLinesSum / 2, line, barLineWidth);
            break;
        case BARRENDITION_invis: //
            barLine->SetEmptyBB();
            break;
        case BARRENDITION_end:
            this->DrawVerticalSegmentedLine(dc, x, line, barLineWidth);
            this->DrawVerticalSegmentedLine(dc, x2 + barLinesSum / 2, line, barLineThickWidth);
            break;
        case BARRENDITION_dbl:
            this->DrawVerticalSegmentedLine(dc, x, line, barLineWidth);
            this->DrawVerticalSegmentedLine(dc, x2 + barLineWidth, line, barLineWidth);
            break;
        case BARRENDITION_dblheavy:
            this->DrawVerticalSegmentedLine(dc, x, line, barLineThickWidth);
            this->DrawVerticalSegmentedLine(dc, x2 + barLineThickWidth, line, barLineThickWidth);
            break;
        case BARRENDITION_dblsegno:
            this->DrawVerticalSegmentedLine(dc, x, line, barLineWidth);
            this->DrawVerticalSegmentedLine(dc, x2 + barLineWidth, line, barLineWidth);
            this->DrawSmuflCode(dc, (x + (barLineSeparation + barLineWidth - serpentWidth) / 2), yBottom,
                SMUFL_E04A_segnoSerpent1, staffSize, false);
            break;
        case BARRENDITION_dbldashed:
            this->DrawVerticalSegmentedLine(dc, x, line, barLineWidth, dashLength, gapLength);
            this->DrawVerticalSegmentedLine(dc, x2 + barLineWidth, line, barLineWidth, dashLength, gapLength);
            break;
        case BARRENDITION_dbldotted:
            this->DrawVerticalDots(dc, x, line, barLineWidth, 2 * unit);
            this->DrawVerticalDots(dc, x2 + barLineWidth, line, barLineWidth, 2 * unit);
            break;
        default:
            // Use solid barline as fallback
            LogWarning(
                "%s bar lines not supported", barLine->AttBarLineLog::BarrenditionToStr(barLine->GetForm()).c_str());
            this->DrawVerticalSegmentedLine(dc, x, line, barLineWidth);
            break;
    }
}

void View::DrawBarLineDots(DeviceContext *dc, Staff *staff, BarLine *barLine)
{
    assert(dc);
    assert(staff);
    assert(barLine);

    const int x = barLine->GetDrawingX();
    const int dotSeparation = m_doc->GetDrawingUnit(100) * m_options->m_repeatBarLineDotSeparation.GetValue();
    const int barLineWidth = m_doc->GetDrawingUnit(100) * m_options->m_barLineWidth.GetValue();
    const int thickBarLineWidth = m_doc->GetDrawingUnit(100) * m_options->m_thickBarlineThickness.GetValue();
    const int barLineSeparation = m_doc->GetDrawingUnit(100) * m_options->m_barLineSeparation.GetValue();
    const int xShift = thickBarLineWidth + dotSeparation + barLineSeparation + barLineWidth;
    const int staffSize = staff->m_drawingStaffSize;
    const int dotWidth = m_doc->GetGlyphWidth(SMUFL_E044_repeatDot, staffSize, false);

    const int x1 = x - barLineWidth / 2 - (dotSeparation + dotWidth);
    const int x2 = x + xShift;

    const int numDots = 3 - staff->m_drawingLines % 2; // odd => 2 dots, even => 3 dots
    const int yInc = m_doc->GetDrawingDoubleUnit(staffSize); // vertical distance between dots
    const int yBottom = staff->GetDrawingY() - (staff->m_drawingLines + numDots % 2) * m_doc->GetDrawingUnit(staffSize);
    const int yTop = yBottom + (numDots - 1) * yInc;

    if (barLine->GetForm() == BARRENDITION_rptstart) {
        for (int y = yTop; y >= yBottom; y -= yInc) {
            this->DrawSmuflCode(dc, x2 - thickBarLineWidth / 2, y, SMUFL_E044_repeatDot, staffSize, false);
        }
    }
    if (barLine->GetForm() == BARRENDITION_rptboth) {
        for (int y = yTop; y >= yBottom; y -= yInc) {
            this->DrawSmuflCode(
                dc, x2 + barLineSeparation + barLineWidth / 2, y, SMUFL_E044_repeatDot, staffSize, false);
        }
    }
    if ((barLine->GetForm() == BARRENDITION_rptend) || (barLine->GetForm() == BARRENDITION_rptboth)) {
        for (int y = yTop; y >= yBottom; y -= yInc) {
            this->DrawSmuflCode(dc, x1, y, SMUFL_E044_repeatDot, staffSize, false);
        }
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
        dc->StartGraphic(measure, "", measure->GetID());
    }

    if (m_drawingScoreDef.GetMnumVisible() != BOOLEAN_false) {
        MNum *mnum = vrv_cast<MNum *>(measure->FindDescendantByType(MNUM));
        Reh *reh = vrv_cast<Reh *>(measure->FindDescendantByType(REH));
        const bool hasRehearsal = reh
            && ((reh->HasTstamp() && (reh->GetTstamp() == 0.0))
                || (reh->GetStart()->Is(BARLINE)
                    && vrv_cast<BarLine *>(reh->GetStart())->GetPosition() == BarLinePosition::Left));
        if (mnum && !hasRehearsal) {
            // this should be an option
            Measure *systemStart = vrv_cast<Measure *>(system->FindDescendantByType(MEASURE));

            // Draw non-generated measure numbers
            // If mnumInterval is 0, draw system starting measure numbers > 1,
            // otherwise, draw every (mnumInterval)th measure number.
            int mnumInterval = m_options->m_mnumInterval.GetValue();
            if ((mnumInterval == 0 && measure == systemStart && measure->GetN() != "0" && measure->GetN() != "1")
                || !mnum->IsGenerated()
                || (mnumInterval >= 1 && (std::atoi(measure->GetN().c_str()) % mnumInterval == 0))) {
                int symbolOffset = m_doc->GetDrawingUnit(100);
                ScoreDef *scoreDef = system->GetDrawingScoreDef();
                GrpSym *groupSymbol = vrv_cast<GrpSym *>(scoreDef->FindDescendantByType(GRPSYM));
                if (groupSymbol && (groupSymbol->GetSymbol() == staffGroupingSym_SYMBOL_bracket)) {
                    symbolOffset
                        += m_doc->GetGlyphHeight(SMUFL_E003_bracketTop, 100, false) + m_doc->GetDrawingUnit(100) / 6;
                }
                // hardcoded offset for the mNum based on the lyric font size
                const int yOffset = m_doc->GetDrawingLyricFont(60)->GetPointSize();
                this->DrawMNum(dc, mnum, measure, system, std::max(symbolOffset, yOffset));
            }
        }
    }

    this->DrawMeasureChildren(dc, measure, measure, system);

    // Draw the barlines only with measured music
    if (measure->IsMeasuredMusic()) {
        System *system = vrv_cast<System *>(measure->GetFirstAncestor(SYSTEM));
        assert(system);
        if ((measure->GetDrawingLeftBarLine() != BARRENDITION_NONE) || measure->HasInvisibleStaffBarlines()) {
            this->DrawScoreDef(dc, system->GetDrawingScoreDef(), measure, measure->GetLeftBarLine()->GetDrawingX(),
                measure->GetLeftBarLine());
        }
        if ((measure->GetDrawingRightBarLine() != BARRENDITION_NONE) || measure->HasInvisibleStaffBarlines()) {
            this->DrawScoreDef(dc, system->GetDrawingScoreDef(), measure, measure->GetRightBarLine()->GetDrawingX(),
                measure->GetRightBarLine(), measure->IsLastInSystem(), system->IsLastOfMdiv());
        }
    }

    if (measure->IsMeasuredMusic()) {
        dc->EndGraphic(measure, this);
    }

    if (measure->GetDrawingEnding()) {
        system->AddToDrawingList(measure->GetDrawingEnding());
    }

    /*
    //Debug code for displaying aligner positions
    for (auto &child : measure->m_measureAligner.GetChildren()) {
        Alignment *alignment = vrv_cast<Alignment *>(child);
        int x = alignment->GetXRel() + measure->GetDrawingX();
        int y = system->GetDrawingY() - m_doc->GetDrawingStaffSize(100);
        this->DrawVerticalLine(dc, y, y + m_doc->GetDrawingUnit(100), x, 20);
    }
    */
}

void View::DrawMeterSigGrp(DeviceContext *dc, Layer *layer, Staff *staff)
{
    assert(dc);
    assert(layer);
    assert(staff);

    MeterSigGrp *meterSigGrp = layer->GetStaffDefMeterSigGrp();
    ListOfObjects childList = meterSigGrp->GetList();

    // Ignore invisible meter signatures and those without count
    childList.erase(std::remove_if(childList.begin(), childList.end(),
                        [](Object *object) {
                            MeterSig *meterSig = vrv_cast<MeterSig *>(object);
                            assert(meterSig);
                            return ((meterSig->GetVisible() == BOOLEAN_false) || !meterSig->HasCount());
                        }),
        childList.end());

    const int glyphSize = staff->GetDrawingStaffNotationSize();

    const int unit = m_doc->GetDrawingUnit(glyphSize);
    int offset = 0;
    dc->StartGraphic(meterSigGrp, "", meterSigGrp->GetID());
    // Draw meterSigGrp by alternating meterSig and plus sign (when required)
    for (auto iter = childList.begin(); iter != childList.end(); ++iter) {
        MeterSig *meterSig = vrv_cast<MeterSig *>(*iter);
        assert(meterSig);
        this->DrawMeterSig(dc, meterSig, staff, offset);

        const int y = staff->GetDrawingY() - unit * (staff->m_drawingLines - 1);
        const int x = meterSig->GetDrawingX() + offset;
        const int width = meterSig->GetContentRight() - meterSig->GetContentLeft();
        if ((meterSigGrp->GetFunc() == meterSigGrpLog_FUNC_mixed) && (iter != std::prev(childList.end()))) {
            // draw plus sign here
            const int plusX = x + width + unit / 2;
            this->DrawSmuflCode(dc, plusX, y, SMUFL_E08C_timeSigPlus, glyphSize, false);
            offset += width + unit + m_doc->GetGlyphWidth(SMUFL_E08C_timeSigPlus, glyphSize, false);
        }
        else {
            offset += width + unit;
        }
    }

    dc->EndGraphic(meterSigGrp, this);
}

void View::DrawMNum(DeviceContext *dc, MNum *mnum, Measure *measure, System *system, int yOffset)
{
    assert(dc);
    assert(measure);
    assert(mnum);

    Staff *staff = system->GetTopVisibleStaff();
    if (staff) {
        // Only one FloatingPositioner on the top (visible) staff
        if (!system->SetCurrentFloatingPositioner(staff->GetN(), mnum, staff, staff)) {
            return;
        }

        dc->StartGraphic(mnum, "", mnum->GetID());

        FontInfo mnumTxt;
        if (!dc->UseGlobalStyling()) {
            mnumTxt.SetFaceName(m_doc->GetResources().GetTextFont());
            mnumTxt.SetStyle(FONTSTYLE_italic);
        }

        TextDrawingParams params;

        data_HORIZONTALALIGNMENT alignment = mnum->GetChildRendAlignment();
        // mNum are center aligned by default
        if (alignment == HORIZONTALALIGNMENT_NONE) alignment = HORIZONTALALIGNMENT_center;

        // HARDCODED
        // we set mNum to a fixed height above the system and make it a bit smaller than other text
        params.m_x = staff->GetDrawingX();
        params.m_y = staff->GetDrawingY() + yOffset;
        if (mnum->HasFontsize()) {
            data_FONTSIZE *fs = mnum->GetFontsizeAlternate();
            if (fs->GetType() == FONTSIZE_fontSizeNumeric) {
                mnumTxt.SetPointSize(fs->GetFontSizeNumeric());
            }
            else if (fs->GetType() == FONTSIZE_term) {
                const int percent = fs->GetPercentForTerm();
                mnumTxt.SetPointSize(m_doc->GetDrawingLyricFont(percent)->GetPointSize());
            }
            else if (fs->GetType() == FONTSIZE_percent) {
                mnumTxt.SetPointSize(m_doc->GetDrawingLyricFont(fs->GetPercent())->GetPointSize());
            }
        }
        else {
            mnumTxt.SetPointSize(m_doc->GetDrawingLyricFont(80)->GetPointSize());
        }

        dc->SetFont(&mnumTxt);

        dc->StartText(this->ToDeviceContextX(params.m_x), this->ToDeviceContextY(params.m_y), alignment);
        this->DrawTextChildren(dc, mnum, params);
        dc->EndText();

        dc->ResetFont();

        this->DrawTextEnclosure(dc, params, staff->m_drawingStaffSize);

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

    assert(system->GetDrawingScoreDef());
    StaffDef *staffDef = system->GetDrawingScoreDef()->GetStaffDef(staff->GetN());
    if (staffDef && (staffDef->GetDrawingVisibility() == OPTIMIZATION_HIDDEN)) {
        return;
    }

    dc->StartGraphic(staff, "", staff->GetID());

    if (m_doc->IsFacs()) {
        staff->SetFromFacsimile(m_doc);
    }

    this->DrawStaffLines(dc, staff, staffDef, measure, system);

    if (staffDef && (m_doc->GetType() != Facs)) {
        this->DrawStaffDef(dc, staff, measure);
    }

    if (!staff->GetLedgerLinesAbove().empty()) {
        this->DrawLedgerLines(dc, staff, staff->GetLedgerLinesAbove(), false, false);
    }
    if (!staff->GetLedgerLinesBelow().empty()) {
        this->DrawLedgerLines(dc, staff, staff->GetLedgerLinesBelow(), true, false);
    }
    if (!staff->GetLedgerLinesAboveCue().empty()) {
        this->DrawLedgerLines(dc, staff, staff->GetLedgerLinesAboveCue(), false, true);
    }
    if (!staff->GetLedgerLinesBelowCue().empty()) {
        this->DrawLedgerLines(dc, staff, staff->GetLedgerLinesBelowCue(), true, true);
    }

    this->DrawStaffChildren(dc, staff, staff, measure);

    this->DrawStaffDefCautionary(dc, staff, measure);

    for (Object *spanningElement : staff->m_timeSpanningElements) {
        system->AddToDrawingListIfNecessary(spanningElement);
    }

    dc->EndGraphic(staff, this);
}

void View::DrawStaffLines(DeviceContext *dc, Staff *staff, StaffDef *staffDef, Measure *measure, System *system)
{
    assert(dc);
    assert(staff);
    assert(staffDef);
    assert(measure);
    assert(system);

    // If German lute tablature the default is @lines.visible="false", but setting @lines.visible="true"
    // will draw the staff lines.
    bool gltLines = (staff->IsTabLuteGerman() && staffDef->GetLinesVisible() != BOOLEAN_true);
    // For anything other than German lute tablature the default is @lines.visible="true"
    bool visibleLines = (staffDef->GetLinesVisible() != BOOLEAN_false);

    // Nothing to do if both are false
    if (!gltLines && !visibleLines) return;

    int j, x1, x2, y1, y2;

    x1 = measure->GetDrawingX();
    x2 = x1 + measure->GetWidth();
    y1 = staff->GetDrawingY();
    if (!staff->HasDrawingRotation()) {
        y2 = y1;
    }
    else {
        y2 = y1 - measure->GetWidth() * tan(staff->GetDrawingRotation() * M_PI / 180.0);
    }

    const int lineWidth = m_doc->GetDrawingStaffLineWidth(staff->m_drawingStaffSize);
    dc->SetPen(this->ToDeviceContextX(lineWidth), PEN_SOLID);

    // If German lute tablature the default is @lines.visible="false", but setting @lines.visible="true"
    // will draw the staff lines.
    // For anything other than German lute tablature the default is @lines.visible="true"
    if (gltLines) {
        // German tablature has no staff, just a single base line
        // But internally we maintain the fiction of an invisible staff as a coordinate system
        SegmentedLine line(x1, x2);
        // Issue #3589 move base line slightly further down and reduce thickness
        y1 -= (m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * staff->m_drawingLines) * 11 / 10;
        this->DrawHorizontalSegmentedLine(dc, y1, line, lineWidth / 2);
    }
    // Normal staff lines
    else {
        // draw staff lines
        for (j = 0; j < staff->m_drawingLines; ++j) {
            // Skewed lines - with Facs (neumes) only for now
            if (y1 != y2) {
                dc->DrawLine(this->ToDeviceContextX(x1), this->ToDeviceContextY(y1), this->ToDeviceContextX(x2),
                    this->ToDeviceContextY(y2));
                // For drawing rectangles instead of lines
                y1 -= m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
                y2 -= m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
            }
            else {
                const bool isFrenchOrGermanOrItalianTablature
                    = (staff->IsTabLuteFrench() || staff->IsTabLuteGerman() || staff->IsTabLuteItalian());
                SegmentedLine line(x1, x2);
                // We do not need to do this during layout calculation - and only with guitar tablature but not for
                // French, German or Italian lute tablature
                if (!dc->Is(BBOX_DEVICE_CONTEXT) && staff->IsTablature() && !isFrenchOrGermanOrItalianTablature) {
                    Object fullLine;
                    fullLine.SetParent(system);
                    fullLine.UpdateContentBBoxY(y1 + (lineWidth / 2), y1 - (lineWidth / 2));
                    fullLine.UpdateContentBBoxX(x1, x2);
                    int margin = m_doc->GetDrawingUnit(100) / 2;
                    ListOfObjects notes = staff->FindAllDescendantsByType(NOTE, false);
                    for (Object *note : notes) {
                        if (note->VerticalContentOverlap(&fullLine, margin / 2)) {
                            line.AddGap(note->GetContentLeft() - margin, note->GetContentRight() + margin);
                        }
                    }
                }
                this->DrawHorizontalSegmentedLine(dc, y1, line, lineWidth);
                y1 -= m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
                y2 = y1;
            }
        }
    }

    dc->ResetPen();

    return;
}

void View::DrawLedgerLines(DeviceContext *dc, Staff *staff, const ArrayOfLedgerLines &lines, bool below, bool cueSize)
{
    assert(dc);
    assert(staff);

    std::string gClass = "above";
    int y = staff->GetDrawingY();
    int x = staff->GetDrawingX();
    int ySpace = m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);

    if (below) {
        gClass = "below";
        ySpace *= -1;
        y += ySpace * (staff->m_drawingLines - 1);
    }
    y += ySpace;

    if (cueSize) {
        gClass += " cue";
    }

    dc->StartCustomGraphic("ledgerLines", gClass);

    int lineWidth
        = m_doc->GetOptions()->m_ledgerLineThickness.GetValue() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    if (cueSize) lineWidth *= m_doc->GetOptions()->m_graceFactor.GetValue();

    dc->SetPen(this->ToDeviceContextX(lineWidth), PEN_SOLID);

    bool svgHtml5 = (m_doc->GetOptions()->m_svgHtml5.GetValue());

    for (const LedgerLine &line : lines) {
        for (const LedgerLine::Dash &dash : line.m_dashes) {
            if (svgHtml5) {
                // Add the custom graphic only with html5
                dc->StartCustomGraphic("lineDash");
                std::string events = ConcatenateIDs(dash.m_events);
                dc->SetCustomGraphicAttributes("related", events);
            }

            dc->DrawLine(this->ToDeviceContextX(x + dash.m_x1), this->ToDeviceContextY(y),
                this->ToDeviceContextX(x + dash.m_x2), this->ToDeviceContextY(y));

            if (svgHtml5) dc->EndCustomGraphic();
        }
        y += ySpace;
    }

    dc->ResetPen();

    dc->EndCustomGraphic();
}

void View::DrawStaffDef(DeviceContext *dc, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(staff);
    assert(measure);

    // StaffDef information is always in the first layer
    Layer *layer = vrv_cast<Layer *>(staff->FindDescendantByType(LAYER));
    if (!layer || !layer->HasStaffDef()) return;

    // StaffDef staffDef;
    // dc->StartGraphic(&staffDef, "", staffDef.GetID());

    // draw the scoreDef if required
    if (layer->GetStaffDefClef()) {
        this->DrawLayerElement(dc, layer->GetStaffDefClef(), layer, staff, measure);
    }
    if (layer->GetStaffDefKeySig()) {
        this->DrawLayerElement(dc, layer->GetStaffDefKeySig(), layer, staff, measure);
    }
    if (layer->GetStaffDefMensur()) {
        this->DrawLayerElement(dc, layer->GetStaffDefMensur(), layer, staff, measure);
    }
    if (layer->GetStaffDefMeterSigGrp()) {
        this->DrawMeterSigGrp(dc, layer, staff);
    }
    else if (layer->GetStaffDefMeterSig()) {
        this->DrawLayerElement(dc, layer->GetStaffDefMeterSig(), layer, staff, measure);
    }

    // dc->EndGraphic(&staffDef, this);
}

void View::DrawStaffDefCautionary(DeviceContext *dc, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(staff);
    assert(measure);

    // StaffDef cautionary information is always in the first layer
    Layer *layer = vrv_cast<Layer *>(staff->FindDescendantByType(LAYER));
    if (!layer || !layer->HasCautionStaffDef()) return;

    // StaffDef staffDef;
    // dc->StartGraphic(&staffDef, "cautionary", staffDef.GetID());

    // draw the scoreDef if required
    if (layer->GetCautionStaffDefClef()) {
        this->DrawLayerElement(dc, layer->GetCautionStaffDefClef(), layer, staff, measure);
    }
    if (layer->GetCautionStaffDefKeySig()) {
        this->DrawLayerElement(dc, layer->GetCautionStaffDefKeySig(), layer, staff, measure);
    }
    if (layer->GetCautionStaffDefMensur()) {
        this->DrawLayerElement(dc, layer->GetCautionStaffDefMensur(), layer, staff, measure);
    }
    if (layer->GetCautionStaffDefMeterSig()) {
        this->DrawLayerElement(dc, layer->GetCautionStaffDefMeterSig(), layer, staff, measure);
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

    Staff *parentStaff = vrv_cast<Staff *>(layer->GetFirstAncestor(STAFF));
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

    dc->StartGraphic(layer, "", layer->GetID());

    this->DrawLayerChildren(dc, layer, layer, staff, measure);

    dc->EndGraphic(layer, this);

    // first draw the postponed tuplets
    this->DrawLayerList(dc, layer, staff, measure, TUPLET_BRACKET);
    this->DrawLayerList(dc, layer, staff, measure, TUPLET_NUM);
}

void View::DrawLayerList(DeviceContext *dc, Layer *layer, Staff *staff, Measure *measure, const ClassId classId)
{
    assert(dc);
    assert(layer);
    assert(staff);
    assert(measure);

    ArrayOfObjects *drawingList = layer->GetDrawingList();

    for (Object *object : *drawingList) {
        if (object->Is(classId) && (classId == TUPLET_BRACKET)) {
            this->DrawTupletBracket(dc, dynamic_cast<LayerElement *>(object), layer, staff, measure);
        }
        if (object->Is(classId) && (classId == TUPLET_NUM)) {
            this->DrawTupletNum(dc, dynamic_cast<LayerElement *>(object), layer, staff, measure);
        }
    }
}

void View::DrawSystemDivider(DeviceContext *dc, System *system, Measure *firstMeasure)
{
    assert(dc);
    assert(system);

    // Draw system divider (from the second one) if scoreDef is optimized
    if (!firstMeasure || (m_options->m_systemDivider.GetValue() == SYSTEMDIVIDER_none)) return;
    // No system divider if we are on the first system of a page or of an mdiv
    if (system->IsFirstInPage() || system->IsFirstOfMdiv()) return;

    // initialize to zero, first measure is not supposed to have system divider
    int previousSystemBottomMarginY = 0;
    Object *currentPage = system->GetFirstAncestor(PAGE);
    if (currentPage) {
        Object *previousSystem = currentPage->GetPrevious(system);
        if (previousSystem) {
            Measure *previousSystemMeasure = vrv_cast<Measure *>(previousSystem->FindDescendantByType(MEASURE, 1));
            if (previousSystemMeasure) {
                Staff *bottomStaff = previousSystemMeasure->GetBottomVisibleStaff();
                // set Y position to that of lowest (bottom) staff, substact space taken by staff lines and
                // substract offset of the system divider symbol itself (added to y2 and y4)
                if (bottomStaff) {
                    previousSystemBottomMarginY = bottomStaff->GetDrawingY()
                        - (bottomStaff->m_drawingLines - 1)
                            * m_doc->GetDrawingDoubleUnit(bottomStaff->m_drawingStaffSize)
                        - m_doc->GetDrawingUnit(100) * 5;
                }
            }
        }
    }

    if ((system->IsDrawingOptimized() || (m_options->m_systemDivider.GetValue() > SYSTEMDIVIDER_auto))) {
        int y = system->GetDrawingY();
        Staff *staff = system->GetTopVisibleStaff();
        if (staff) {
            // Place it in the middle of current and previous systems - in very tight layout this can collision with
            // the staff above. To be improved
            y = (staff->GetDrawingY() + previousSystemBottomMarginY) / 2;
        }
        int x1 = system->GetDrawingX() - m_doc->GetDrawingUnit(100) * 3;
        int x2 = system->GetDrawingX() + m_doc->GetDrawingUnit(100) * 3;
        int y1 = y - m_doc->GetDrawingUnit(100) * 1;
        int y2 = y + m_doc->GetDrawingUnit(100) * 3;
        int y3 = y1 + m_doc->GetDrawingUnit(100) * 2;
        int y4 = y2 + m_doc->GetDrawingUnit(100) * 2;
        // left and left-right
        dc->StartCustomGraphic("systemDivider");

        this->DrawObliquePolygon(dc, x1, y1, x2, y2, m_doc->GetDrawingUnit(100) * 1.5);
        this->DrawObliquePolygon(dc, x1, y3, x2, y4, m_doc->GetDrawingUnit(100) * 1.5);
        if (m_options->m_systemDivider.GetValue() == SYSTEMDIVIDER_left_right) {
            // Right divider is not taken into account in the layout calculation and can collision with the music
            // content
            Measure *lastMeasure = vrv_cast<Measure *>(system->FindDescendantByType(MEASURE, 1, BACKWARD));
            assert(lastMeasure);
            int x4 = lastMeasure->GetDrawingX() + lastMeasure->GetRightBarLineRight();
            int x3 = x4 - m_doc->GetDrawingUnit(100) * 6;
            this->DrawObliquePolygon(dc, x3, y1, x4, y2, m_doc->GetDrawingUnit(100) * 1.5);
            this->DrawObliquePolygon(dc, x3, y3, x4, y4, m_doc->GetDrawingUnit(100) * 1.5);
        }

        dc->EndCustomGraphic();
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

    for (Object *current : parent->GetChildren()) {
        if (current->Is(MEASURE)) {
            // cast to Measure check in DrawMeasure
            this->DrawMeasure(dc, vrv_cast<Measure *>(current), system);
        }
        // scoreDef are not drawn directly, but anything else should not be possible
        else if (current->Is(SCOREDEF)) {
            // nothing to do, then
            ScoreDef *scoreDef = vrv_cast<ScoreDef *>(current);
            assert(scoreDef);

            Measure *nextMeasure = vrv_cast<Measure *>(system->GetNext(scoreDef, MEASURE));
            if (nextMeasure && scoreDef->DrawLabels()) {
                this->DrawScoreDef(dc, scoreDef, nextMeasure, nextMeasure->GetDrawingX());
            }

            this->SetScoreDefDrawingWidth(dc, scoreDef);
        }
        else if (current->IsSystemElement()) {
            // cast to SystemElement check in DrawSystemEditorial element
            this->DrawSystemElement(dc, dynamic_cast<SystemElement *>(current), system);
        }
        else if (current->Is(DIV)) {
            // cast to Div check in DrawDiv element
            this->DrawDiv(dc, dynamic_cast<Div *>(current), system);
        }
        else if (current->IsEditorialElement()) {
            // cast to EditorialElement check in DrawSystemEditorial element
            this->DrawSystemEditorialElement(dc, dynamic_cast<EditorialElement *>(current), system);
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

    ListOfObjects objects = parent->FindAllDescendantsByType(BEAMSPAN, false);
    for (Object *element : objects) {
        BeamSpan *beamSpan = vrv_cast<BeamSpan *>(element);
        BeamSpanSegment *segment = beamSpan->GetSegmentForSystem(system);
        if (segment) {
            segment->CalcBeam(segment->GetLayer(), segment->GetStaff(), m_doc, beamSpan, beamSpan->m_drawingPlace);
        }
    }

    for (Object *current : parent->GetChildren()) {
        if (current->Is(STAFF)) {
            // cast to Staff check in DrawStaff
            this->DrawStaff(dc, vrv_cast<Staff *>(current), measure, system);
        }
        else if (current->IsControlElement()) {
            // cast to ControlElement check in DrawControlElement
            this->DrawControlElement(dc, dynamic_cast<ControlElement *>(current), measure, system);
        }
        else if (current->IsEditorialElement()) {
            // cast to EditorialElement check in DrawMeasureEditorialElement
            this->DrawMeasureEditorialElement(dc, dynamic_cast<EditorialElement *>(current), measure, system);
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

    for (Object *current : parent->GetChildren()) {
        if (current->Is(LAYER)) {
            // cast to Layer check in DrawLayer
            this->DrawLayer(dc, vrv_cast<Layer *>(current), staff, measure);
        }
        else if (current->IsEditorialElement()) {
            // cast to EditorialElement check in DrawStaffEditorialElement
            this->DrawStaffEditorialElement(dc, dynamic_cast<EditorialElement *>(current), staff, measure);
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

    for (Object *current : parent->GetChildren()) {
        if (current->IsLayerElement()) {
            this->DrawLayerElement(dc, dynamic_cast<LayerElement *>(current), layer, staff, measure);
        }
        else if (current->IsEditorialElement()) {
            // cast to EditorialElement check in DrawLayerEditorialElement
            this->DrawLayerEditorialElement(dc, dynamic_cast<EditorialElement *>(current), layer, staff, measure);
        }
        else if (!current->Is({ LABEL, LABELABBR })) {
            assert(false);
        }
    }
}

void View::DrawTextChildren(DeviceContext *dc, Object *parent, TextDrawingParams &params)
{
    assert(dc);
    assert(parent);

    // For ControlElement, we need to set the positioner empty bounding box if no text
    if (parent->IsControlElement()) {
        if (!parent->GetChildCount() || !parent->HasNonEditorialContent()) {
            ControlElement *controlElement = vrv_cast<ControlElement *>(parent);
            assert(controlElement);
            FloatingPositioner *positioner = controlElement->GetCurrentFloatingPositioner();
            // With MNum drawn from DrawMeasure there will be no positioner
            if (positioner) positioner->SetEmptyBB();
        }
    }

    for (Object *current : parent->GetChildren()) {
        if (current->IsTextElement()) {
            this->DrawTextElement(dc, dynamic_cast<TextElement *>(current), params);
        }
        else if (current->IsEditorialElement()) {
            // cast to EditorialElement check in DrawTextEditorialElement
            this->DrawTextEditorialElement(dc, dynamic_cast<EditorialElement *>(current), params);
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

    for (Object *current : parent->GetChildren()) {
        if (current->IsTextElement()) {
            this->DrawTextElement(dc, dynamic_cast<TextElement *>(current), params);
        }
        else if (current->IsEditorialElement()) {
            // cast to EditorialElement check in DrawLayerEditorialElement
            this->DrawFbEditorialElement(dc, dynamic_cast<EditorialElement *>(current), params);
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

    for (Object *current : parent->GetChildren()) {
        if (current->Is(FIG)) {
            this->DrawFig(dc, dynamic_cast<Fig *>(current), params);
        }
        else if (current->IsTextElement()) {
            // We are now reaching a text element - start set only here because we can have a figure
            TextDrawingParams paramsChild = params;
            dc->StartText(
                this->ToDeviceContextX(params.m_x), this->ToDeviceContextY(params.m_y), HORIZONTALALIGNMENT_left);
            this->DrawTextElement(dc, dynamic_cast<TextElement *>(current), paramsChild);
            dc->EndText();
        }
        else if (current->IsEditorialElement()) {
            // cast to EditorialElement check in DrawLayerEditorialElement
            this->DrawRunningEditorialElement(dc, dynamic_cast<EditorialElement *>(current), params);
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
        this->DrawAnnot(dc, element);
        return;
    }
    if (element->Is(APP)) {
        assert(dynamic_cast<App *>(element));
        EditorialLevel level = dynamic_cast<App *>(element)->GetLevel();
        if ((level != EDITORIAL_SCORE) && (level != EDITORIAL_TOPLEVEL)) return;
    }
    else if (element->Is(CHOICE)) {
        assert(dynamic_cast<Choice *>(element));
        EditorialLevel level = dynamic_cast<Choice *>(element)->GetLevel();
        if ((level != EDITORIAL_SCORE) && (level != EDITORIAL_TOPLEVEL)) return;
    }
    std::string elementStart;
    if (element->IsMilestoneElement()) elementStart = "systemElementStart";

    dc->StartGraphic(element, elementStart, element->GetID());
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
        this->DrawAnnot(dc, element);
        return;
    }
    if (element->Is(APP)) {
        assert(dynamic_cast<App *>(element) && (dynamic_cast<App *>(element)->GetLevel() == EDITORIAL_MEASURE));
    }
    else if (element->Is(CHOICE)) {
        assert(dynamic_cast<Choice *>(element) && (dynamic_cast<Choice *>(element)->GetLevel() == EDITORIAL_MEASURE));
    }

    dc->StartGraphic(element, "", element->GetID());
    if (!element->IsHidden()) {
        this->DrawMeasureChildren(dc, element, measure, system);
    }
    dc->EndGraphic(element, this);
}

void View::DrawStaffEditorialElement(DeviceContext *dc, EditorialElement *element, Staff *staff, Measure *measure)
{
    assert(element);
    if (element->Is(ANNOT)) {
        this->DrawAnnot(dc, element);
        return;
    }
    if (element->Is(APP)) {
        assert(dynamic_cast<App *>(element) && (dynamic_cast<App *>(element)->GetLevel() == EDITORIAL_STAFF));
    }
    else if (element->Is(CHOICE)) {
        assert(dynamic_cast<Choice *>(element) && (dynamic_cast<Choice *>(element)->GetLevel() == EDITORIAL_STAFF));
    }

    dc->StartGraphic(element, "", element->GetID());
    if (!element->IsHidden()) {
        this->DrawStaffChildren(dc, element, staff, measure);
    }
    dc->EndGraphic(element, this);
}

void View::DrawLayerEditorialElement(
    DeviceContext *dc, EditorialElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(element);
    if (element->Is(ANNOT)) {
        this->DrawAnnot(dc, element);
        return;
    }
    if (element->Is(APP)) {
        assert(dynamic_cast<App *>(element) && (dynamic_cast<App *>(element)->GetLevel() == EDITORIAL_LAYER));
    }
    else if (element->Is(CHOICE)) {
        assert(dynamic_cast<Choice *>(element) && (dynamic_cast<Choice *>(element)->GetLevel() == EDITORIAL_LAYER));
    }

    dc->StartGraphic(element, "", element->GetID());
    if (!element->IsHidden()) {
        this->DrawLayerChildren(dc, element, layer, staff, measure);
    }
    dc->EndGraphic(element, this);
}

void View::DrawTextEditorialElement(DeviceContext *dc, EditorialElement *element, TextDrawingParams &params)
{
    assert(element);
    if (element->Is(ANNOT)) {
        this->DrawAnnot(dc, element, true);
        return;
    }
    if (element->Is(APP)) {
        assert(dynamic_cast<App *>(element) && (dynamic_cast<App *>(element)->GetLevel() == EDITORIAL_TEXT));
    }
    else if (element->Is(CHOICE)) {
        assert(dynamic_cast<Choice *>(element) && (dynamic_cast<Choice *>(element)->GetLevel() == EDITORIAL_TEXT));
    }

    dc->StartTextGraphic(element, "", element->GetID());
    if (!element->IsHidden()) {
        this->DrawTextChildren(dc, element, params);
    }
    dc->EndTextGraphic(element, this);
}

void View::DrawFbEditorialElement(DeviceContext *dc, EditorialElement *element, TextDrawingParams &params)
{
    assert(element);
    if (element->Is(ANNOT)) {
        this->DrawAnnot(dc, element, true);
        return;
    }
    if (element->Is(APP)) {
        assert(dynamic_cast<App *>(element) && (dynamic_cast<App *>(element)->GetLevel() == EDITORIAL_FB));
    }
    else if (element->Is(CHOICE)) {
        assert(dynamic_cast<Choice *>(element) && (dynamic_cast<Choice *>(element)->GetLevel() == EDITORIAL_FB));
    }

    dc->StartTextGraphic(element, "", element->GetID());
    if (!element->IsHidden()) {
        this->DrawFbChildren(dc, element, params);
    }
    dc->EndTextGraphic(element, this);
}

void View::DrawRunningEditorialElement(DeviceContext *dc, EditorialElement *element, TextDrawingParams &params)
{
    assert(element);
    if (element->Is(ANNOT)) {
        this->DrawAnnot(dc, element, true);
        return;
    }
    if (element->Is(APP)) {
        assert(dynamic_cast<App *>(element) && (dynamic_cast<App *>(element)->GetLevel() == EDITORIAL_RUNNING));
    }
    else if (element->Is(CHOICE)) {
        assert(dynamic_cast<Choice *>(element) && (dynamic_cast<Choice *>(element)->GetLevel() == EDITORIAL_RUNNING));
    }

    dc->StartGraphic(element, "", element->GetID());
    if (!element->IsHidden()) {
        this->DrawRunningChildren(dc, element, params);
    }
    dc->EndGraphic(element, this);
}

void View::DrawAnnot(DeviceContext *dc, EditorialElement *element, bool isTextElement)
{
    assert(element);

    if (isTextElement) {
        dc->StartTextGraphic(element, "", element->GetID());
    }
    else {
        dc->StartGraphic(element, "", element->GetID());
    }

    Annot *annot = vrv_cast<Annot *>(element);
    assert(annot);
    dc->AddDescription(UTF32to8(annot->GetText()));

    if (isTextElement) {
        dc->EndTextGraphic(element, this);
    }
    else {
        dc->EndGraphic(element, this);
    }
}

} // namespace vrv

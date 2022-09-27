/////////////////////////////////////////////////////////////////////////////
// Name:        view_element.cpp
// Author:      Laurent Pugin and Chris Niven
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "view.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <math.h>

//----------------------------------------------------------------------------

#include "accid.h"
#include "artic.h"
#include "beam.h"
#include "beatrpt.h"
#include "btrem.h"
#include "chord.h"
#include "clef.h"
#include "custos.h"
#include "devicecontext.h"
#include "dot.h"
#include "dynam.h"
#include "elementpart.h"
#include "f.h"
#include "ftrem.h"
#include "functorparams.h"
#include "halfmrpt.h"
#include "keyaccid.h"
#include "label.h"
#include "labelabbr.h"
#include "layer.h"
#include "measure.h"
#include "mensur.h"
#include "mrest.h"
#include "mrpt.h"
#include "mrpt2.h"
#include "mspace.h"
#include "multirest.h"
#include "multirpt.h"
#include "note.h"
#include "options.h"
#include "rest.h"
#include "smufl.h"
#include "staff.h"
#include "stem.h"
#include "syl.h"
#include "system.h"
#include "tie.h"
#include "tuplet.h"
#include "verse.h"
#include "vrv.h"
#include "zone.h"

namespace vrv {

void View::DrawLayerElement(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    if (element->HasSameas()) {
        dc->StartGraphic(element, "", element->GetID());
        element->SetEmptyBB();
        dc->EndGraphic(element, this);
        return;
    }

    int previousColor = m_currentColour;

    if (element == m_currentElement) {
        m_currentColour = AxRED;
    }
    else {
        m_currentColour = AxNONE;
    }

    if (element->Is(ACCID)) {
        this->DrawAccid(dc, element, layer, staff, measure);
    }
    else if (element->Is(ARTIC)) {
        this->DrawArtic(dc, element, layer, staff, measure);
    }
    else if (element->Is(BARLINE)) {
        this->DrawBarLine(dc, element, layer, staff, measure);
    }
    else if (element->Is(BEAM)) {
        this->DrawBeam(dc, element, layer, staff, measure);
    }
    else if (element->Is(BEATRPT)) {
        this->DrawBeatRpt(dc, element, layer, staff, measure);
    }
    else if (element->Is(BTREM)) {
        this->DrawBTrem(dc, element, layer, staff, measure);
    }
    else if (element->Is(CHORD)) {
        this->DrawDurationElement(dc, element, layer, staff, measure);
    }
    else if (element->Is(CLEF)) {
        this->DrawClef(dc, element, layer, staff, measure);
    }
    else if (element->Is(CUSTOS)) {
        this->DrawCustos(dc, element, layer, staff, measure);
    }
    else if (element->Is(DOT)) {
        this->DrawDot(dc, element, layer, staff, measure);
    }
    else if (element->Is(DOTS)) {
        this->DrawDots(dc, element, layer, staff, measure);
    }
    else if (element->Is(FTREM)) {
        this->DrawFTrem(dc, element, layer, staff, measure);
    }
    else if (element->Is(FLAG)) {
        this->DrawFlag(dc, element, layer, staff, measure);
    }
    else if (element->Is(GRACEGRP)) {
        this->DrawGraceGrp(dc, element, layer, staff, measure);
    }
    else if (element->Is(HALFMRPT)) {
        this->DrawHalfmRpt(dc, element, layer, staff, measure);
    }
    else if (element->Is(KEYSIG)) {
        this->DrawKeySig(dc, element, layer, staff, measure);
    }
    else if (element->Is(LIGATURE)) {
        this->DrawLigature(dc, element, layer, staff, measure);
    }
    else if (element->Is(MENSUR)) {
        this->DrawMensur(dc, element, layer, staff, measure);
    }
    else if (element->Is(METERSIG)) {
        this->DrawMeterSig(dc, element, layer, staff, measure);
    }
    else if (element->Is(MREST)) {
        this->DrawMRest(dc, element, layer, staff, measure);
    }
    else if (element->Is(MRPT)) {
        this->DrawMRpt(dc, element, layer, staff, measure);
    }
    else if (element->Is(MRPT2)) {
        this->DrawMRpt2(dc, element, layer, staff, measure);
    }
    else if (element->Is(MSPACE)) {
        this->DrawMSpace(dc, element, layer, staff, measure);
    }
    else if (element->Is(MULTIREST)) {
        this->DrawMultiRest(dc, element, layer, staff, measure);
    }
    else if (element->Is(MULTIRPT)) {
        this->DrawMultiRpt(dc, element, layer, staff, measure);
    }
    else if (element->Is(NC)) {
        this->DrawNc(dc, element, layer, staff, measure);
    }
    else if (element->Is(NOTE)) {
        this->DrawDurationElement(dc, element, layer, staff, measure);
    }
    else if (element->Is(NEUME)) {
        this->DrawNeume(dc, element, layer, staff, measure);
    }
    else if (element->Is(PLICA)) {
        this->DrawPlica(dc, element, layer, staff, measure);
    }
    else if (element->Is(PROPORT)) {
        this->DrawProport(dc, element, layer, staff, measure);
    }
    else if (element->Is(REST)) {
        this->DrawDurationElement(dc, element, layer, staff, measure);
    }
    else if (element->Is(SPACE)) {
        this->DrawSpace(dc, element, layer, staff, measure);
    }
    else if (element->Is(STEM)) {
        this->DrawStem(dc, element, layer, staff, measure);
    }
    else if (element->Is(SYL)) {
        this->DrawSyl(dc, element, layer, staff, measure);
    }
    else if (element->Is(SYLLABLE)) {
        this->DrawSyllable(dc, element, layer, staff, measure);
    }
    else if (element->Is(TABDURSYM)) {
        this->DrawTabDurSym(dc, element, layer, staff, measure);
    }
    else if (element->Is(TABGRP)) {
        this->DrawTabGrp(dc, element, layer, staff, measure);
    }
    else if (element->Is(TUPLET)) {
        this->DrawTuplet(dc, element, layer, staff, measure);
    }
    else if (element->Is(TUPLET_BRACKET)) {
        dc->StartGraphic(element, "", element->GetID());
        dc->EndGraphic(element, this);
        layer->AddToDrawingList(element);
    }
    else if (element->Is(TUPLET_NUM)) {
        dc->StartGraphic(element, "", element->GetID());
        dc->EndGraphic(element, this);
        layer->AddToDrawingList(element);
    }
    else if (element->Is(VERSE)) {
        this->DrawVerse(dc, element, layer, staff, measure);
    }
    else {
        // This should never happen
        LogError("Element '%s' cannot be drawn", element->GetClassName().c_str());
    }

    m_currentColour = previousColor;
}

//----------------------------------------------------------------------------
// View - LayerElement
//----------------------------------------------------------------------------

void View::DrawAccid(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    Accid *accid = vrv_cast<Accid *>(element);
    assert(accid);

    // This can happen with accid within note with only accid.ges
    // We still create an graphic in the output
    if (!accid->HasAccid() || staff->IsTablature()) {
        dc->StartGraphic(element, "", element->GetID());
        accid->SetEmptyBB();
        dc->EndGraphic(element, this);
        return;
    }

    dc->StartGraphic(element, "", element->GetID());

    const data_NOTATIONTYPE notationType = staff->m_drawingNotationType;
    std::u32string accidStr = accid->GetSymbolStr(notationType);

    int x = accid->GetDrawingX();
    int y = accid->GetDrawingY();

    if (accid->HasPlace() || (accid->GetFunc() == accidLog_FUNC_edit)) {
        const int unit = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        const int staffTop = staff->GetDrawingY();
        const int staffBottom = staffTop - (staff->m_drawingLines - 1) * unit * 2;

        // look at the note position and adjust it if necessary
        Note *note = dynamic_cast<Note *>(accid->GetFirstAncestor(NOTE, MAX_ACCID_DEPTH));
        if (note) {
            const int drawingDur = note->GetDrawingDur();
            int noteTop = note->GetDrawingTop(m_doc, staff->m_drawingStaffSize);
            int noteBottom = note->GetDrawingBottom(m_doc, staff->m_drawingStaffSize);
            bool onStaff = (accid->GetOnstaff() == BOOLEAN_true);

            // Adjust position to mensural stems
            if (note->IsMensuralDur()) {
                if (accid->GetFunc() != accidLog_FUNC_edit) onStaff = (accid->GetOnstaff() != BOOLEAN_false);
                const int verticalCenter = staffTop - (staff->m_drawingLines - 1) * unit;
                const data_STEMDIRECTION stemDir = this->GetMensuralStemDir(layer, note, verticalCenter);
                if ((drawingDur > DUR_1) || (drawingDur < DUR_BR)) {
                    if (stemDir == STEMDIRECTION_up) {
                        noteTop = note->GetDrawingY() + unit * STANDARD_STEMLENGTH;
                        noteBottom -= unit;
                    }
                    else {
                        noteBottom = note->GetDrawingY() - unit * STANDARD_STEMLENGTH;
                    }
                }
            }
            if (accid->GetPlace() == STAFFREL_below) {
                y = ((noteBottom <= staffBottom) || onStaff) ? noteBottom : staffBottom;
            }
            else {
                y = ((noteTop >= staffTop) || onStaff) ? noteTop : staffTop;
            }
            // Increase the x position of the accid
            x += note->GetDrawingRadius(m_doc);
        }
        TextExtend extend;
        dc->SetFont(m_doc->GetDrawingSmuflFont(staff->m_drawingStaffSize, accid->GetDrawingCueSize()));
        dc->GetSmuflTextExtent(accid->GetSymbolStr(notationType), &extend);
        dc->ResetFont();
        y = (accid->GetPlace() == STAFFREL_below) ? y - extend.m_ascent - unit : y + extend.m_descent + unit;
    }

    this->DrawSmuflString(
        dc, x, y, accidStr, HORIZONTALALIGNMENT_center, staff->m_drawingStaffSize, accid->GetDrawingCueSize(), true);

    dc->EndGraphic(element, this);
}

void View::DrawArtic(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    Artic *artic = vrv_cast<Artic *>(element);
    assert(artic);

    /************** draw the artic **************/

    int x = artic->GetDrawingX();
    int y = artic->GetDrawingY();

    const bool drawingCueSize = artic->GetDrawingCueSize();

    dc->SetFont(m_doc->GetDrawingSmuflFont(staff->m_drawingStaffSize, drawingCueSize));

    const data_ARTICULATION articValue = artic->GetArticFirst();
    const data_STAFFREL place = artic->GetDrawingPlace();

    const char32_t code = artic->GetArticGlyph(articValue, place);
    char32_t enclosingFront, enclosingBack;
    std::tie(enclosingFront, enclosingBack) = artic->GetEnclosingGlyphs();

    // Skip it if we do not have it in the font (for now - we should log / document this somewhere)
    if (code == 0) {
        artic->SetEmptyBB();
        dc->ResetFont();
        return;
    }

    // The correction for centering the glyph
    const int xCorr = m_doc->GetGlyphWidth(code, staff->m_drawingStaffSize, drawingCueSize) / 2;

    const int glyphHeight = m_doc->GetGlyphHeight(code, staff->m_drawingStaffSize, drawingCueSize);

    // The height by which enclosing brackets exceed the artic symbol
    int exceedingHeight = 0;
    for (const char32_t symbol : { enclosingFront, enclosingBack }) {
        if (symbol == 0) continue;
        const int symbolHeight = m_doc->GetGlyphHeight(symbol, staff->m_drawingStaffSize, drawingCueSize);
        exceedingHeight = std::max(exceedingHeight, symbolHeight - glyphHeight);
    }

    // Center the glyph if necessary
    int yCorr = 0;
    if (Artic::IsCentered(articValue) && !enclosingFront && !enclosingBack) {
        y += (place == STAFFREL_above) ? -(glyphHeight / 2) : (glyphHeight / 2);
    }
    else {
        y += (place == STAFFREL_above) ? (exceedingHeight / 2) : -(exceedingHeight / 2);
        // @glyph.num/name are (usually?) aligned for placement above and needs to be shifted when below
        if ((artic->HasGlyphNum() || artic->HasGlyphName()) && (place == STAFFREL_below)) {
            yCorr += glyphHeight;
        }
    }

    // The relative vertical displacement of enclosing brackets
    int yCorrEncl = (place == STAFFREL_above) ? -(glyphHeight / 2) : (glyphHeight / 2);

    // Adjust the baseline for glyph above the baseline in SMuFL
    if (Artic::VerticalCorr(code, place)) {
        y -= glyphHeight;
        yCorrEncl = -glyphHeight / 2;
    }

    // Draw glyph including possible enclosing brackets
    dc->StartGraphic(element, "", element->GetID());

    if (enclosingFront) {
        int xCorrEncl = std::max(xCorr, m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2 / 3);
        xCorrEncl += m_doc->GetGlyphWidth(enclosingFront, staff->m_drawingStaffSize, drawingCueSize);
        this->DrawSmuflCode(
            dc, x - xCorrEncl, y - yCorrEncl, enclosingFront, staff->m_drawingStaffSize, drawingCueSize);
    }

    this->DrawSmuflCode(dc, x - xCorr, y - yCorr, code, staff->m_drawingStaffSize, drawingCueSize);

    if (enclosingBack) {
        const int xCorrEncl = std::max(xCorr, m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 2 / 3);
        this->DrawSmuflCode(dc, x + xCorrEncl, y - yCorrEncl, enclosingBack, staff->m_drawingStaffSize, drawingCueSize);
    }

    dc->EndGraphic(element, this);

    dc->ResetFont();
}

void View::DrawBarLine(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    BarLine *barLine = vrv_cast<BarLine *>(element);
    assert(barLine);

    if (barLine->GetForm() == BARRENDITION_invis) {
        barLine->SetEmptyBB();
        return;
    }

    dc->StartGraphic(element, "", element->GetID());

    const int yTop = staff->GetDrawingY();
    const int yBottom = yTop - (staff->m_drawingLines - 1) * m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);

    const int offset = (yTop == yBottom) ? m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) : 0;

    this->DrawBarLine(dc, yTop + offset, yBottom - offset, barLine, barLine->GetForm());
    if (barLine->HasRepetitionDots()) {
        this->DrawBarLineDots(dc, staff, barLine);
    }

    dc->EndGraphic(element, this);
}

void View::DrawBeatRpt(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    BeatRpt *beatRpt = vrv_cast<BeatRpt *>(element);
    assert(beatRpt);

    dc->StartGraphic(element, "", element->GetID());

    const int staffSize = staff->m_drawingStaffSize;
    const int xSymbol = element->GetDrawingX();
    const int ySymbol = element->GetDrawingY() - (staff->m_drawingLines - 1) * m_doc->GetDrawingUnit(staffSize);

    if (beatRpt->GetSlash() == BEATRPT_REND_mixed) {
        this->DrawSmuflCode(dc, xSymbol, ySymbol, SMUFL_E501_repeat2Bars, staffSize, false);
    }
    else {
        char32_t slash = SMUFL_E504_repeatBarSlash;
        const int slashNum = beatRpt->HasSlash() ? beatRpt->GetSlash() : 1;
        const int halfWidth = m_doc->GetGlyphWidth(slash, staffSize, false) / 2;
        for (int i = 0; i < slashNum; ++i) {
            this->DrawSmuflCode(dc, xSymbol + i * halfWidth, ySymbol, slash, staffSize, false);
        }
    }

    dc->EndGraphic(element, this);
}

void View::DrawBTrem(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    BTrem *bTrem = vrv_cast<BTrem *>(element);
    assert(bTrem);

    const int staffSize = staff->m_drawingStaffSize;
    int xOffset = 0;
    int yTop = staff->GetDrawingY();
    int yBottom = yTop - (staff->m_drawingLines - 1) * m_doc->GetDrawingDoubleUnit(staffSize);

    // Get the chord or note child
    Object *bTremElement = bTrem->FindDescendantByType(CHORD);
    if (!bTremElement) bTremElement = bTrem->FindDescendantByType(NOTE);
    if (!bTremElement) {
        bTrem->SetEmptyBB();
        return;
    }

    dc->StartGraphic(element, "", element->GetID());

    this->DrawLayerChildren(dc, bTrem, layer, staff, measure);

    if (bTremElement->Is(CHORD)) {
        Chord *childChord = vrv_cast<Chord *>(bTremElement);
        xOffset = childChord->GetDrawingRadius(m_doc);
        yTop
            = (childChord->GetDrawingTop(m_doc, staffSize) > yTop) ? childChord->GetDrawingTop(m_doc, staffSize) : yTop;
        yBottom = (childChord->GetDrawingBottom(m_doc, staffSize) < yBottom)
            ? childChord->GetDrawingBottom(m_doc, staffSize)
            : yBottom;
    }
    else if (bTremElement->Is(NOTE)) {
        Note *childNote = vrv_cast<Note *>(bTremElement);
        if (childNote->HasStemSameasNote() && childNote->GetStemSameasRole() == SAMEAS_SECONDARY) {
            bTrem->SetEmptyBB();
            dc->EndGraphic(element, this);
            return;
        }
        xOffset = childNote->GetDrawingRadius(m_doc);
        yTop = (childNote->GetDrawingTop(m_doc, staffSize) > yTop) ? childNote->GetDrawingTop(m_doc, staffSize) : yTop;
        yBottom = (childNote->GetDrawingBottom(m_doc, staffSize) < yBottom)
            ? childNote->GetDrawingBottom(m_doc, staffSize)
            : yBottom;
    }

    this->DrawStemMod(dc, element, staff);

    // draw the (tuplet) number
    if (bTrem->HasNum() && (bTrem->GetNumVisible() != BOOLEAN_false)) {
        dc->SetFont(m_doc->GetDrawingSmuflFont(staff->m_drawingStaffSize, false));
        // calculate the extend of the number
        TextExtend extend;
        const std::u32string figures = this->IntToTupletFigures(bTrem->GetNum());
        dc->GetSmuflTextExtent(figures, &extend);
        int yNum = yTop + m_doc->GetDrawingUnit(staffSize);
        if (bTrem->GetNumPlace() == STAFFREL_basic_below) {
            yNum = yBottom - m_doc->GetDrawingUnit(staffSize) - extend.m_height;
        }
        dc->DrawMusicText(
            figures, ToDeviceContextX(element->GetDrawingX() + xOffset - extend.m_width / 2), ToDeviceContextY(yNum));
        dc->ResetFont();
    }

    dc->EndGraphic(element, this);
}

void View::DrawChord(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    Chord *chord = vrv_cast<Chord *>(element);
    assert(chord);

    if (chord->m_crossStaff) staff = chord->m_crossStaff;

    chord->ResetDrawingList();

    /************ Draw children (notes, accidentals, etc) ************/

    this->DrawLayerChildren(dc, chord, layer, staff, measure);
}

void View::DrawClef(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    Clef *clef = vrv_cast<Clef *>(element);
    assert(clef);

    if (clef->m_crossStaff) staff = clef->m_crossStaff;

    // hidden clef
    if (clef->GetVisible() == BOOLEAN_false) {
        dc->StartGraphic(element, "", element->GetID());
        clef->SetEmptyBB();
        dc->EndGraphic(element, this);
        return;
    }

    if (staff->IsTablature()) {
        this->DrawTabClef(dc, element, layer, staff, measure);
        return;
    }

    int x, y;
    if (m_doc->GetType() == Facs && clef->HasFacs()) {
        y = ToLogicalY(staff->GetDrawingY());
        x = clef->GetDrawingX();
    }
    else {
        y = staff->GetDrawingY();
        x = element->GetDrawingX();
    }

    char32_t sym = clef->GetClefGlyph(staff->m_drawingNotationType);

    if (sym == 0) {
        clef->SetEmptyBB();
        return;
    }

    if (clef->HasLine()) {
        y -= m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * (staff->m_drawingLines - clef->GetLine());
        if ((m_doc->GetType() == Facs) && (staff->GetDrawingRotate() != 0)) {
            double deg = staff->GetDrawingRotate();
            int xDiff = x - staff->GetDrawingX();
            y -= int(xDiff * tan(deg * M_PI / 180.0));
        }
    }
    else if (clef->GetShape() == CLEFSHAPE_perc) {
        y -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * (staff->m_drawingLines - 1);
    }
    else {
        // we have no information where to put it
        return;
    }

    dc->StartGraphic(element, "", element->GetID());

    this->DrawSmuflCode(dc, x, y, sym, staff->m_drawingStaffSize, false);

    if ((m_doc->GetType() == Facs) && element->HasFacs()) {
        const int noteHeight = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 2);
        const int noteWidth = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 1.4);

        FacsimileInterface *fi = element->GetFacsimileInterface();
        fi->GetZone()->SetUlx(x);
        fi->GetZone()->SetUly(ToDeviceContextY(y));
        fi->GetZone()->SetLrx(x + noteWidth);
        fi->GetZone()->SetLry(ToDeviceContextY(y - noteHeight));
    }

    // Possibly draw enclosing brackets
    this->DrawClefEnclosing(dc, clef, staff, sym, x, y);

    dc->EndGraphic(element, this);
}

void View::DrawClefEnclosing(DeviceContext *dc, Clef *clef, Staff *staff, char32_t glyph, int x, int y)
{
    if ((clef->GetEnclose() == ENCLOSURE_brack) || (clef->GetEnclose() == ENCLOSURE_box)) {
        const int unit = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        const int glyphSize = staff->GetDrawingStaffNotationSize();
        x += m_doc->GetGlyphLeft(glyph, glyphSize, false);
        y += m_doc->GetGlyphBottom(glyph, glyphSize, false);
        const int height = m_doc->GetGlyphHeight(glyph, glyphSize, false);
        const int width = m_doc->GetGlyphWidth(glyph, glyphSize, false);
        const int offset = unit * 3 / 4;
        // We use overlapping brackets to draw boxes :)
        const int bracketWidth = (clef->GetEnclose() == ENCLOSURE_brack) ? unit : (width + offset);
        const int verticalThickness = m_doc->GetDrawingStemWidth(glyphSize);
        const int horizontalThickness = ((clef->GetEnclose() == ENCLOSURE_brack) ? 2 : 1) * verticalThickness;

        this->DrawEnclosingBrackets(
            dc, x, y, height, width, offset, bracketWidth, horizontalThickness, verticalThickness);
    }
    else if (clef->HasEnclose() && (clef->GetEnclose() != ENCLOSURE_none)) {
        LogWarning("Only drawing of enclosing brackets and boxes is supported for clef.");
    }
}

void View::DrawCustos(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    Custos *custos = vrv_cast<Custos *>(element);
    assert(custos);

    dc->StartGraphic(element, "", element->GetID());

    // Select glyph to use for this custos
    const int sym = custos->GetCustosGlyph(staff->m_drawingNotationType);

    int x, y;
    if (custos->HasFacs() && m_doc->GetType() == Facs) {
        x = custos->GetDrawingX();
        // Recalculate y from pitch to prevent visual/meaning mismatch
        Clef *clef = layer->GetClef(element);
        y = ToLogicalY(staff->GetDrawingY());
        PitchInterface pi;
        // Neume notation uses C3 for C clef rather than C4.
        // Take this into account when determining location.
        // However this doesn't affect the value for F clef.
        pi.SetPname(PITCHNAME_c);
        if ((staff->m_drawingNotationType == NOTATIONTYPE_neume) && (clef->GetShape() == CLEFSHAPE_C)) {
            pi.SetOct(3);
        }
        else {
            pi.SetOct(4);
        }
        // Convert from lines & spaces from bottom to from top
        int CClefOffsetFromTop = ((staff->m_drawingLines - 1) * 2) - clef->GetClefLocOffset();
        int custosOffsetFromTop = CClefOffsetFromTop + pi.PitchDifferenceTo(custos);
        y -= custosOffsetFromTop * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    }
    else {
        x = element->GetDrawingX();
        y = element->GetDrawingY();
        // Because SMuFL does not have the origin correpsonding to the pitch as for notes, we need to correct it.
        // This will remain approximate
        y -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    }

    if ((m_doc->GetType() == Facs) && (staff->GetDrawingRotate() != 0)) {
        double deg = staff->GetDrawingRotate();
        int xDiff = x - staff->GetDrawingX();
        y -= int(xDiff * tan(deg * M_PI / 180.0));
    }

    this->DrawSmuflCode(dc, x, y, sym, staff->m_drawingStaffSize, false, true);

    if ((m_doc->GetType() == Facs) && element->HasFacs()) {
        const int noteHeight = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 2);
        const int noteWidth = (int)(m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) / 1.4);

        FacsimileInterface *fi = element->GetFacsimileInterface();
        fi->GetZone()->SetUlx(x);
        fi->GetZone()->SetUly(ToDeviceContextY(y));
        fi->GetZone()->SetLrx(x + noteWidth);
        fi->GetZone()->SetLry(ToDeviceContextY(y - noteHeight));
    }

    /************ Draw children (accidentals, etc) ************/
    // Drawing the children should be done before ending the graphic. Otherwise the SVG tree will not match the MEI one
    this->DrawLayerChildren(dc, custos, layer, staff, measure);

    dc->EndGraphic(element, this);
}

void View::DrawDot(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    Dot *dot = vrv_cast<Dot *>(element);
    assert(dot);

    dc->StartGraphic(element, "", element->GetID());

    if (dot->m_drawingPreviousElement && dot->m_drawingPreviousElement->IsInLigature()) {
        this->DrawDotInLigature(dc, element, layer, staff, measure);
    }
    else {
        int x = element->GetDrawingX();
        int y = element->GetDrawingY();

        if (m_doc->GetType() != Transcription) {
            // Use the note to which the points to for position
            if (dot->m_drawingPreviousElement && !dot->m_drawingNextElement) {
                x += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 7 / 2;
                y = dot->m_drawingPreviousElement->GetDrawingY();
            }
            if (dot->m_drawingPreviousElement && dot->m_drawingNextElement) {
                x += ((dot->m_drawingNextElement->GetDrawingX() - dot->m_drawingPreviousElement->GetDrawingX()) / 2);
                x += dot->m_drawingPreviousElement->GetDrawingRadius(m_doc);
                y = dot->m_drawingPreviousElement->GetDrawingY();
            }
        }

        this->DrawDotsPart(dc, x, y, 1, staff);
    }

    dc->EndGraphic(element, this);
}

void View::DrawDots(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    Dots *dots = vrv_cast<Dots *>(element);
    assert(dots);

    dc->StartGraphic(element, "", element->GetID());

    for (const auto &mapEntry : dots->GetMapOfDotLocs()) {
        const Staff *dotStaff = (mapEntry.first) ? mapEntry.first : staff;
        int y = dotStaff->GetDrawingY()
            - m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * (dotStaff->m_drawingLines - 1);
        int x = dots->GetDrawingX() + m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        for (int loc : mapEntry.second) {
            this->DrawDotsPart(dc, x, y + loc * m_doc->GetDrawingUnit(staff->m_drawingStaffSize), dots->GetDots(),
                dotStaff, element->GetDrawingCueSize());
        }
    }

    dc->EndGraphic(element, this);
}

void View::DrawDurationElement(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    if (dynamic_cast<Chord *>(element)) {
        dc->StartGraphic(element, "", element->GetID());
        this->DrawChord(dc, element, layer, staff, measure);
        dc->EndGraphic(element, this);
    }
    else if (dynamic_cast<Note *>(element)) {
        dc->StartGraphic(element, "", element->GetID());
        this->DrawNote(dc, element, layer, staff, measure);
        dc->EndGraphic(element, this);
    }
    else if (dynamic_cast<Rest *>(element)) {
        dc->StartGraphic(element, "", element->GetID());
        this->DrawRest(dc, element, layer, staff, measure);
        dc->EndGraphic(element, this);
    }
}

void View::DrawFlag(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    Flag *flag = vrv_cast<Flag *>(element);
    assert(flag);

    Stem *stem = vrv_cast<Stem *>(flag->GetFirstAncestor(STEM));
    assert(stem);

    int x = flag->GetDrawingX() - m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) / 2;
    int y = flag->GetDrawingY();

    dc->StartGraphic(element, "", element->GetID());

    char32_t code = flag->GetFlagGlyph(stem->GetDrawingStemDir());
    this->DrawSmuflCode(dc, x, y, code, staff->GetDrawingStaffNotationSize(), flag->GetDrawingCueSize());

    dc->EndGraphic(element, this);
}

void View::DrawGraceGrp(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    dc->StartGraphic(element, "", element->GetID());

    // basically nothing to do here
    this->DrawLayerChildren(dc, element, layer, staff, measure);

    dc->EndGraphic(element, this);
}

void View::DrawHalfmRpt(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    HalfmRpt *halfmRpt = vrv_cast<HalfmRpt *>(element);
    assert(halfmRpt);

    int x = halfmRpt->GetDrawingX();
    x += m_doc->GetGlyphWidth(SMUFL_E500_repeat1Bar, staff->m_drawingStaffSize, false) / 2;

    dc->StartGraphic(element, "", element->GetID());

    this->DrawMRptPart(dc, x, SMUFL_E500_repeat1Bar, 0, false, staff);

    dc->EndGraphic(element, this);
}

void View::DrawKeySig(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    KeySig *keySig = vrv_cast<KeySig *>(element);
    assert(keySig);

    int x, y;

    Clef *clef = layer->GetClef(element);
    if (!clef) {
        keySig->SetEmptyBB();
        return;
    }

    // hidden key signature
    if (keySig->GetVisible() == BOOLEAN_false) {
        dc->StartGraphic(element, "", element->GetID());
        keySig->SetEmptyBB();
        dc->EndGraphic(element, this);
        return;
    }

    // C major (0) key sig and no cancellation
    else if ((keySig->GetAccidCount() == 0) && (keySig->m_drawingCancelAccidCount == 0)) {
        keySig->SetEmptyBB();
        return;
    }

    // C major (0) key sig and system scoreDef - cancellation (if any) is done at the end of the previous system
    else if ((keySig->GetScoreDefRole() == SCOREDEF_SYSTEM) && (keySig->GetAccidCount() == 0)) {
        keySig->SetEmptyBB();
        return;
    }

    x = element->GetDrawingX();
    // HARDCODED
    int naturalGlyphWidth = m_doc->GetGlyphWidth(SMUFL_E261_accidentalNatural, staff->m_drawingStaffSize, false);
    int step = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    int naturalStep = step * TEMP_KEYSIG_NATURAL_STEP;
    step *= TEMP_KEYSIG_STEP;

    int clefLocOffset = layer->GetClefLocOffset(element);

    dc->StartGraphic(element, "", element->GetID());

    // Show cancellation if showchange is true (false by default) or if C major
    if ((keySig->GetScoreDefRole() != SCOREDEF_SYSTEM)
        && ((keySig->GetSigShowchange() == BOOLEAN_true) || (keySig->GetAccidCount() == 0))) {
        if (keySig->m_skipCancellation) {
            LogWarning("Cautionary accidentals are skipped if the new or previous KeySig contains KeyAccid children.");
        }
        else {
            const int beginCancel
                = (keySig->GetAccidType() == keySig->m_drawingCancelAccidType) ? keySig->GetAccidCount() : 0;
            for (int i = beginCancel; i < keySig->m_drawingCancelAccidCount; ++i) {
                data_PITCHNAME pitch = KeySig::GetAccidPnameAt(keySig->m_drawingCancelAccidType, i);
                const int loc = PitchInterface::CalcLoc(
                    pitch, KeySig::GetOctave(keySig->m_drawingCancelAccidType, pitch, clef), clefLocOffset);
                y = staff->GetDrawingY() + staff->CalcPitchPosYRel(m_doc, loc);

                dc->StartCustomGraphic("keyAccid");

                this->DrawSmuflCode(dc, x, y, SMUFL_E261_accidentalNatural, staff->m_drawingStaffSize, false);

                dc->EndCustomGraphic();

                x += naturalGlyphWidth + naturalStep;
                if ((keySig->GetAccidCount() > 0) && (i + 1 == keySig->m_drawingCancelAccidCount)) {
                    // Add some extra space after last natural
                    x += step;
                }
            }
        }
    }

    dc->SetFont(m_doc->GetDrawingSmuflFont(staff->m_drawingStaffSize, false));

    ListOfObjects childList = keySig->GetList(keySig);
    for (Object *child : childList) {
        KeyAccid *keyAccid = vrv_cast<KeyAccid *>(child);
        assert(keyAccid);
        this->DrawKeyAccid(dc, keyAccid, staff, clef, clefLocOffset, x);
        x += step;
    }

    dc->ResetFont();

    dc->EndGraphic(element, this);
}

void View::DrawMeterSig(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    MeterSig *meterSig = vrv_cast<MeterSig *>(element);
    assert(meterSig);

    // hidden time signature
    if (meterSig->GetForm() == METERFORM_invis) {
        dc->StartGraphic(element, "", element->GetID());
        meterSig->SetEmptyBB();
        dc->EndGraphic(element, this);
        return;
    }

    this->DrawMeterSig(dc, meterSig, staff, 0);
}

void View::DrawKeyAccid(DeviceContext *dc, KeyAccid *keyAccid, Staff *staff, Clef *clef, int clefLocOffset, int &x)
{
    const std::u32string symbolStr = keyAccid->GetSymbolStr(staff->m_drawingNotationType);
    const int loc = keyAccid->CalcStaffLoc(clef, clefLocOffset);
    const int y = staff->GetDrawingY() + staff->CalcPitchPosYRel(m_doc, loc);

    dc->StartCustomGraphic("keyAccid", "", keyAccid->GetID());

    this->DrawSmuflString(dc, x, y, symbolStr, HORIZONTALALIGNMENT_left, staff->m_drawingStaffSize, false);

    dc->EndCustomGraphic();

    TextExtend extend;
    dc->GetSmuflTextExtent(symbolStr, &extend);
    x += extend.m_width;
}

void View::DrawMeterSig(DeviceContext *dc, MeterSig *meterSig, Staff *staff, int horizOffset)
{
    const bool hasSmallEnclosing = (meterSig->HasSym() || (meterSig->GetForm() == METERFORM_num));
    char32_t enclosingFront, enclosingBack;
    std::tie(enclosingFront, enclosingBack) = meterSig->GetEnclosingGlyphs(hasSmallEnclosing);

    dc->StartGraphic(meterSig, "", meterSig->GetID());

    int y = staff->GetDrawingY() - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * (staff->m_drawingLines - 1);
    int x = meterSig->GetDrawingX() + horizOffset;

    const int glyphSize = staff->GetDrawingStaffNotationSize();

    if (enclosingFront) {
        this->DrawSmuflCode(dc, x, y, enclosingFront, glyphSize, false);
        x += m_doc->GetGlyphWidth(enclosingFront, glyphSize, false);
    }

    if (meterSig->HasSym()) {
        const char32_t code = meterSig->GetSymbolGlyph();
        this->DrawSmuflCode(dc, x, y, code, glyphSize, false);
        x += m_doc->GetGlyphWidth(code, glyphSize, false);
    }
    else if (meterSig->GetForm() == METERFORM_num) {
        x += this->DrawMeterSigFigures(dc, x, y, meterSig, 0, staff);
    }
    else if (meterSig->HasCount()) {
        x += this->DrawMeterSigFigures(dc, x, y, meterSig, meterSig->GetUnit(), staff);
    }

    if (enclosingBack) {
        this->DrawSmuflCode(dc, x, y, enclosingBack, glyphSize, false);
    }

    dc->EndGraphic(meterSig, this);
}

void View::DrawMRest(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    MRest *mRest = vrv_cast<MRest *>(element);
    assert(mRest);

    dc->StartGraphic(element, "", element->GetID());

    mRest->CenterDrawingX();

    const bool drawingCueSize = mRest->GetDrawingCueSize();
    int x = mRest->GetDrawingX();
    int y = (measure->m_measureAligner.GetMaxTime() >= (DUR_MAX * 2))
        ? element->GetDrawingY() - m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize)
        : element->GetDrawingY();
    char32_t rest
        = (measure->m_measureAligner.GetMaxTime() >= (DUR_MAX * 2)) ? SMUFL_E4E2_restDoubleWhole : SMUFL_E4E3_restWhole;

    x -= m_doc->GetGlyphWidth(rest, staff->m_drawingStaffSize, drawingCueSize) / 2;

    this->DrawSmuflCode(dc, x, y, rest, staff->m_drawingStaffSize, drawingCueSize);

    // single legder line for whole rest glyphs
    if ((measure->m_measureAligner.GetMaxTime() < (DUR_MAX * 2))
        && (y > staff->GetDrawingY()
            || y < staff->GetDrawingY()
                    - (staff->m_drawingLines - 1) * m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize))) {
        const int width = m_doc->GetGlyphWidth(rest, staff->m_drawingStaffSize, drawingCueSize);
        int ledgerLineThickness
            = m_doc->GetOptions()->m_ledgerLineThickness.GetValue() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        int ledgerLineExtension
            = m_doc->GetOptions()->m_ledgerLineExtension.GetValue() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        if (drawingCueSize) {
            ledgerLineThickness *= m_doc->GetOptions()->m_graceFactor.GetValue();
            ledgerLineExtension *= m_doc->GetOptions()->m_graceFactor.GetValue();
        }

        dc->StartCustomGraphic("ledgerLines");
        this->DrawHorizontalLine(dc, x - ledgerLineExtension, x + width + ledgerLineExtension, y, ledgerLineThickness);
        dc->EndCustomGraphic();
    }

    dc->EndGraphic(element, this);
}

void View::DrawMRpt(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    MRpt *mRpt = vrv_cast<MRpt *>(element);
    assert(mRpt);

    mRpt->CenterDrawingX();

    dc->StartGraphic(element, "", element->GetID());

    this->DrawMRptPart(dc, element->GetDrawingX(), SMUFL_E500_repeat1Bar, 0, false, staff);

    // draw the measure count
    const int mRptNum = mRpt->HasNum() ? mRpt->GetNum() : mRpt->m_drawingMeasureCount;
    if ((mRptNum > 0) && (mRpt->GetNumVisible() != BOOLEAN_false)) {
        dc->SetFont(m_doc->GetDrawingSmuflFont(staff->m_drawingStaffSize, false));
        // calculate the extend of the number
        TextExtend extend;
        const std::u32string figures = this->IntToTupletFigures(mRptNum);
        dc->GetSmuflTextExtent(figures, &extend);
        const int staffSize = staff->m_drawingStaffSize;
        const int staffHeight = (staff->m_drawingLines - 1) * m_doc->GetDrawingDoubleUnit(staffSize);
        const int offset = std::max(m_doc->GetGlyphHeight(SMUFL_E500_repeat1Bar, staffSize, false) - staffHeight, 0);
        int yNum = staff->GetDrawingY() + m_doc->GetDrawingUnit(staffSize) + offset / 2;
        if (mRpt->GetNumPlace() == STAFFREL_basic_below) {
            yNum -= staff->m_drawingLines * m_doc->GetDrawingDoubleUnit(staffSize) + extend.m_height + offset;
        }
        dc->DrawMusicText(
            figures, ToDeviceContextX(element->GetDrawingX() - extend.m_width / 2), ToDeviceContextY(yNum));
        dc->ResetFont();
    }

    dc->EndGraphic(element, this);
}

void View::DrawMRpt2(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    MRpt2 *mRpt2 = vrv_cast<MRpt2 *>(element);
    assert(mRpt2);

    mRpt2->CenterDrawingX();

    dc->StartGraphic(element, "", element->GetID());

    this->DrawMRptPart(dc, element->GetDrawingX(), SMUFL_E501_repeat2Bars, 2, true, staff);

    dc->EndGraphic(element, this);
}

void View::DrawMSpace(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    // MSpace *mSpace = vrv_cast<MSpace *>(element);
    // assert(mSpace);

    dc->StartGraphic(element, "", element->GetID());
    // nothing to draw here
    dc->EndGraphic(element, this);
}

void View::DrawMultiRest(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    MultiRest *multiRest = vrv_cast<MultiRest *>(element);
    assert(multiRest);

    multiRest->CenterDrawingX();

    dc->StartGraphic(element, "", element->GetID());

    int measureWidth = measure->GetInnerWidth();
    int xCentered = multiRest->GetDrawingX();
    // in case there is CLEF element in the same measure to the right of the mRest, we need to adjust width and starting
    // postion of it to make sure that there is no overlap
    if (layer->GetLast() != element) {
        Object *object = layer->GetNext(element);
        if (object && object->Is(CLEF)) {
            const int rightMargin = xCentered + measureWidth / 2;
            const int widthAdjust = rightMargin - object->GetDrawingX();
            measureWidth -= widthAdjust;
            xCentered -= widthAdjust / 2;
        }
    }

    // We do not support more than three chars
    const int num = multiRest->HasNum() ? std::min(multiRest->GetNum(), 999) : 1;

    const int multiRestThickness
        = m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * m_doc->GetOptions()->m_multiRestThickness.GetValue();
    // Position centered in staff
    int y2 = staff->GetDrawingY() - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * (staff->m_drawingLines - 1)
        - multiRestThickness / 2;
    if (multiRest->HasLoc()) {
        y2 -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * (staff->m_drawingLines - 1 - multiRest->GetLoc());
    }
    int y1 = y2 + multiRestThickness;

    if (multiRest->UseBlockStyle(m_doc)) {
        // This is 1/2 the length of the black rectangle
        int width = measureWidth - 2 * m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
        if (multiRest->HasWidth()) {
            const int fixedWidth = multiRest->AttWidth::GetWidth() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
            width = (width > fixedWidth) ? fixedWidth : width;
        }
        if (width > m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) * 4) {
            // xCentered is the central point, calculate x1 and x2
            const int x1 = xCentered - width / 2;
            const int x2 = xCentered + width / 2;

            // bounding box is not relevant for the multi-rest rectangle
            dc->DeactivateGraphicX();

            // Draw the base rect
            this->DrawFilledRectangle(dc, x1, y1, x2, y2);

            // Draw two lines at beginning and end
            int border = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
            this->DrawFilledRectangle(
                dc, x1, y1 + border, x1 + m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) * 2, y2 - border);
            this->DrawFilledRectangle(
                dc, x2 - m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) * 2, y1 + border, x2, y2 - border);

            dc->ReactivateGraphic();
        }
    }
    else {
        if (staff->m_drawingLines % 2 != 0) {
            y2 += m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
            y1 += m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        }

        const int lgWidth = m_doc->GetGlyphWidth(SMUFL_E4E1_restLonga, staff->m_drawingStaffSize, false);
        const int brWidth = m_doc->GetGlyphWidth(SMUFL_E4E2_restDoubleWhole, staff->m_drawingStaffSize, false);
        const int sbWidth = m_doc->GetGlyphWidth(SMUFL_E4E3_restWhole, staff->m_drawingStaffSize, false);

        int width = (num / 4) * (lgWidth + m_doc->GetDrawingUnit(staff->m_drawingStaffSize));
        width += ((num % 4) / 2) * (brWidth + m_doc->GetDrawingUnit(staff->m_drawingStaffSize));
        width = (num % 2) ? width + sbWidth : width - m_doc->GetDrawingUnit(staff->m_drawingStaffSize);

        int x1 = xCentered - width / 2;

        int count = num;
        while ((count / 4)) {
            this->DrawSmuflCode(dc, x1, y2, SMUFL_E4E1_restLonga, staff->m_drawingStaffSize, false);
            x1 += lgWidth + m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
            count -= 4;
        }
        while ((count / 2)) {
            this->DrawSmuflCode(dc, x1, y2, SMUFL_E4E2_restDoubleWhole, staff->m_drawingStaffSize, false);
            x1 += brWidth + m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
            count -= 2;
        }
        if (count) this->DrawSmuflCode(dc, x1, y1, SMUFL_E4E3_restWhole, staff->m_drawingStaffSize, false);
    }

    // Draw the number
    if (multiRest->GetNumVisible() != BOOLEAN_false) {
        dc->SetFont(m_doc->GetDrawingSmuflFont(staff->m_drawingStaffSize, false));

        const int staffHeight = (staff->m_drawingLines - 1) * m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
        const int offset = 3 * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);

        const int y = (multiRest->GetNumPlace() == STAFFREL_basic_below)
            ? std::min(staff->GetDrawingY() - staffHeight, y2) - offset
            : std::max(staff->GetDrawingY(), y1) + offset;

        this->DrawSmuflString(
            dc, xCentered, y, this->IntToTimeSigFigures(num), HORIZONTALALIGNMENT_center, staff->m_drawingStaffSize);
        dc->ResetFont();
    }

    dc->EndGraphic(element, this);
}

void View::DrawMultiRpt(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    MultiRpt *multiRpt = vrv_cast<MultiRpt *>(element);
    assert(multiRpt);

    multiRpt->CenterDrawingX();

    dc->StartGraphic(element, "", element->GetID());

    this->DrawMRptPart(dc, element->GetDrawingX(), SMUFL_E501_repeat2Bars, multiRpt->GetNum(), true, staff);

    dc->EndGraphic(element, this);

    return;
}

void View::DrawNote(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    Note *note = vrv_cast<Note *>(element);
    assert(note);

    if (note->IsMensuralDur()) {
        this->DrawMensuralNote(dc, element, layer, staff, measure);
        return;
    }
    if (note->IsTabGrpNote()) {
        this->DrawTabNote(dc, element, layer, staff, measure);
        return;
    }

    if (note->m_crossStaff) staff = note->m_crossStaff;

    bool drawingCueSize = note->GetDrawingCueSize();
    int noteY = element->GetDrawingY();
    int noteX = element->GetDrawingX();

    if (note->HasStemSameasNote() && note->GetFlippedNotehead()) {
        int xShift = note->GetDrawingRadius(m_doc) * 2 - m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
        xShift *= (note->GetDrawingStemDir() == STEMDIRECTION_up) ? -1 : 1;
        noteX -= xShift;
    }

    if (!(note->GetHeadVisible() == BOOLEAN_false)) {
        /************** Noteheads: **************/
        int drawingDur = note->GetDrawingDur();
        if (drawingDur == DUR_NONE) {
            if (note->IsInBeam() && !dc->Is(BBOX_DEVICE_CONTEXT)) {
                LogWarning("Missing duration for note '%s' in beam", note->GetID().c_str());
            }
            drawingDur = DUR_4;
        }
        drawingDur = ((note->GetColored() == BOOLEAN_true) && drawingDur > DUR_1) ? (drawingDur + 1) : drawingDur;

        if (drawingDur < DUR_BR) {
            this->DrawMaximaToBrevis(dc, noteY, element, layer, staff);
        }
        else {
            // Whole notes
            char32_t fontNo;
            if (note->GetColored() == BOOLEAN_true) {
                fontNo = (drawingDur == DUR_1) ? SMUFL_E0FA_noteheadWholeFilled : SMUFL_E0A3_noteheadHalf;
            }
            else {
                fontNo = note->GetNoteheadGlyph(drawingDur);
            }

            dc->StartCustomGraphic("notehead");

            this->DrawSmuflCode(dc, noteX, noteY, fontNo, staff->m_drawingStaffSize, drawingCueSize, true);

            // handle notehead enclosure
            if (note->HasHeadMod()) {
                switch (note->GetHeadMod()) {
                    case NOTEHEADMODIFIER_paren: {
                        this->DrawSmuflCode(dc, noteX - note->GetDrawingRadius(m_doc), noteY,
                            SMUFL_E26A_accidentalParensLeft, staff->m_drawingStaffSize, drawingCueSize, true);
                        this->DrawSmuflCode(dc, noteX + note->GetDrawingRadius(m_doc) * 2, noteY,
                            SMUFL_E26B_accidentalParensRight, staff->m_drawingStaffSize, drawingCueSize, true);
                        break;
                    }
                    case NOTEHEADMODIFIER_slash:
                    case NOTEHEADMODIFIER_backslash:
                    case NOTEHEADMODIFIER_vline:
                    case NOTEHEADMODIFIER_hline: {
                        // TODO: Handle other headmodifiers whenever they become available
                        // char32_t glyphCode = note->GetNoteheadModifierGlyph();
                        // int offset = (m_doc->GetGlyphWidth - note->GetDrawingRadius(m_doc) * 2) / 2;
                        // DrawSmuflCode(dc, noteX - offset, noteY, glyphCode, staff->m_drawingStaffSize,
                        // drawingCueSize, true);
                        break;
                    }
                    default: break;
                }
            }

            dc->EndCustomGraphic();
        }
    }

    /************ Draw children (accidentals, etc) ************/

    this->DrawLayerChildren(dc, note, layer, staff, measure);
}

void View::DrawRest(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    Rest *rest = vrv_cast<Rest *>(element);
    assert(rest);

    if (rest->m_crossStaff) staff = rest->m_crossStaff;

    const bool drawingCueSize = rest->GetDrawingCueSize();
    int drawingDur = rest->GetActualDur();
    if (drawingDur == DUR_NONE) {
        if (!dc->Is(BBOX_DEVICE_CONTEXT)) {
            LogWarning("Missing duration for rest '%s'", rest->GetID().c_str());
        }
        drawingDur = DUR_4;
    }
    const char32_t drawingGlyph = rest->GetRestGlyph(drawingDur);

    const int x = element->GetDrawingX();
    const int y = element->GetDrawingY();

    this->DrawSmuflCode(dc, x, y, drawingGlyph, staff->m_drawingStaffSize, drawingCueSize);

    if ((drawingDur == DUR_1 || drawingDur == DUR_2 || drawingDur == DUR_BR)) {
        const int width = m_doc->GetGlyphWidth(drawingGlyph, staff->m_drawingStaffSize, drawingCueSize);
        int ledgerLineThickness
            = m_doc->GetOptions()->m_ledgerLineThickness.GetValue() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        int ledgerLineExtension
            = m_doc->GetOptions()->m_ledgerLineExtension.GetValue() * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        if (drawingCueSize) {
            ledgerLineThickness *= m_doc->GetOptions()->m_graceFactor.GetValue();
            ledgerLineExtension *= m_doc->GetOptions()->m_graceFactor.GetValue();
        }
        const int topMargin = staff->GetDrawingY();
        const int bottomMargin = staff->GetDrawingY()
            - (staff->m_drawingLines - 1) * m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);

        dc->StartCustomGraphic("ledgerLines");
        // single legder line for half and whole rests
        if ((drawingDur == DUR_1 || drawingDur == DUR_2) && (y > topMargin || y < bottomMargin)) {
            dc->DeactivateGraphicX();
            this->DrawHorizontalLine(
                dc, x - ledgerLineExtension, x + width + ledgerLineExtension, y, ledgerLineThickness);
            dc->ReactivateGraphic();
        }
        // double ledger line for breve rests
        else if (drawingDur == DUR_BR && (y >= topMargin || y <= bottomMargin)) {
            const int height = m_doc->GetGlyphHeight(drawingGlyph, staff->m_drawingStaffSize, drawingCueSize);
            dc->DeactivateGraphicX();
            if (y != topMargin) {
                this->DrawHorizontalLine(
                    dc, x - ledgerLineExtension, x + width + ledgerLineExtension, y, ledgerLineThickness);
            }
            if (y != bottomMargin - height) {
                this->DrawHorizontalLine(
                    dc, x - ledgerLineExtension, x + width + ledgerLineExtension, y + height, ledgerLineThickness);
            }
            dc->ReactivateGraphic();
        }
        dc->EndCustomGraphic();
    }

    /************ Draw children (dots) ************/
    this->DrawLayerChildren(dc, rest, layer, staff, measure);
}

void View::DrawSpace(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    dc->StartGraphic(element, "", element->GetID());
    dc->DrawPlaceholder(ToDeviceContextX(element->GetDrawingX()), ToDeviceContextY(element->GetDrawingY()));
    dc->EndGraphic(element, this);
}

void View::DrawStem(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    Stem *stem = vrv_cast<Stem *>(element);
    assert(stem);

    // We check if this belongs to a mensural note
    Note *parent = vrv_cast<Note *>(stem->GetFirstAncestor(NOTE));
    if (parent && parent->IsMensuralDur()) {
        if (((parent->GetDrawingDur() > DUR_1) || ((parent->GetStemDir() != STEMDIRECTION_NONE)))
            && stem->GetVisible() != BOOLEAN_false) {
            /************** Stem/notehead direction: **************/
            const int staffCenter
                = staff->GetDrawingY() - m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * (staff->m_drawingLines - 1);
            const data_STEMDIRECTION stemDir
                = (stem->HasDir()) ? stem->GetDir() : this->GetMensuralStemDir(layer, parent, staffCenter);
            /************** Draw stem: **************/
            dc->StartGraphic(element, "", element->GetID());
            this->DrawMensuralStem(dc, parent, staff, stemDir, parent->GetDrawingX(), parent->GetDrawingY());
            dc->EndGraphic(element, this);
        }

        return;
    }

    // Do not draw virtual (e.g., whole note) stems
    if (stem->IsVirtual()) return;

    dc->StartGraphic(element, "", element->GetID());

    this->DrawFilledRectangle(dc, stem->GetDrawingX() - m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) / 2,
        stem->GetDrawingY(), stem->GetDrawingX() + m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) / 2,
        stem->GetDrawingY() - (stem->GetDrawingStemLen() + stem->GetDrawingStemAdjust()));

    this->DrawStemMod(dc, element, staff);

    this->DrawLayerChildren(dc, stem, layer, staff, measure);

    /************ Draw slash ************/

    if ((stem->GetGrace() == GRACE_unacc) && !stem->IsInBeam()) {
        this->DrawAcciaccaturaSlash(dc, stem, staff);
    }

    dc->EndGraphic(element, this);
}

void View::DrawStemMod(DeviceContext *dc, LayerElement *element, Staff *staff)
{
    // bTrem should already draw stem mod, so avoid doing it for second time from stem
    if (element->GetFirstAncestor(BTREM)) return;

    LayerElement *childElement = NULL;
    // For bTrem, get first CHORD or NOTE, if there's no CHORD
    if (element->Is(BTREM)) {
        childElement = vrv_cast<LayerElement *>(element->FindDescendantByType(CHORD));
        if (!childElement) childElement = vrv_cast<LayerElement *>(element->FindDescendantByType(NOTE));
    }
    // For stem just get parent element
    else if (element->Is(STEM)) {
        childElement = vrv_cast<LayerElement *>(element->GetParent());
    }
    else {
        // should not happen, draw nothing
        LogWarning("Drawing stem mod supported only for elements of <stem> or <bTrem> type.");
        return;
    }
    // If we have no element at this point - exit without drawing anything
    if (!childElement) return;

    // Get stem related values (direction and coordinates)
    data_STEMDIRECTION stemDir = STEMDIRECTION_NONE;
    int stemRelY = 0;
    int stemX = 0;
    StemmedDrawingInterface *stem = childElement->GetStemmedDrawingInterface();
    if (stem) {
        stemDir = stem->GetDrawingStemDir();
        stemRelY = stem->GetDrawingStemModRelY();
        stemX = stem->GetDrawingStemStart(childElement).x;
    }

    // Get note
    Note *note = NULL;
    if (childElement->Is(NOTE)) {
        note = vrv_cast<Note *>(childElement);
    }
    else if (childElement->Is(CHORD)) {
        note = (stemDir == STEMDIRECTION_up) ? vrv_cast<Chord *>(childElement)->GetTopNote()
                                             : vrv_cast<Chord *>(childElement)->GetBottomNote();
    }
    if (!note || note->IsGraceNote() || note->GetDrawingCueSize()) return;

    // Get duration for the element
    DurationInterface *duration = childElement->GetDurationInterface();
    const int drawingDur = duration ? duration->GetActualDur() : 0;

    data_STEMMODIFIER stemMod = element->GetDrawingStemMod();
    if ((stemMod == STEMMODIFIER_NONE) || (stemMod == STEMMODIFIER_none)) return;
    const char32_t code = element->StemModToGlyph(stemMod);
    if (!code) return;

    // calculate position for the stem mod
    const int y = note->GetDrawingY() + stemRelY;
    const int x = (drawingDur <= DUR_1) ? childElement->GetDrawingX() + childElement->GetDrawingRadius(m_doc) : stemX;

    if ((code != SMUFL_E645_vocalSprechgesang) || !element->Is(BTREM)) {
        int adjust = 0;
        if (stemMod == STEMMODIFIER_6slash) {
            const int unit = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
            const int sign = (stemDir == STEMDIRECTION_up) ? 1 : -1;
            const int slash1height = m_doc->GetGlyphHeight(SMUFL_E220_tremolo1, staff->m_drawingStaffSize, false);
            const int slash6height = m_doc->GetGlyphHeight(code, staff->m_drawingStaffSize, false);
            // we need to take half (0.5) of the height difference between to glyphs for the the initial position and
            // add a quarter (0.25) of the glyph to account for the height of slash side and white space between
            // slashes. This results in 0.75 adjustment of the height
            adjust = -sign * unit;
            const int slash1adjust = sign * 0.75 * (slash6height - slash1height) + adjust;
            this->DrawSmuflCode(dc, x, y + slash1adjust, SMUFL_E220_tremolo1, staff->m_drawingStaffSize, false);
        }
        this->DrawSmuflCode(dc, x, y + adjust, code, staff->m_drawingStaffSize, false);
    }
}

void View::DrawSyl(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    Syl *syl = vrv_cast<Syl *>(element);
    assert(syl);

    bool isNeume = (staff->m_drawingNotationType == NOTATIONTYPE_neume);

    if (!syl->GetStart() && !isNeume) {
        LogWarning("Parent note for <syl> was not found");
        return;
    }

    syl->SetDrawingYRel(this->GetSylYRel(syl->m_drawingVerse, staff));

    dc->StartGraphic(syl, "", syl->GetID());
    dc->DeactivateGraphicY();

    dc->SetBrush(m_currentColour, AxSOLID);

    FontInfo currentFont = *m_doc->GetDrawingLyricFont(staff->m_drawingStaffSize);
    if (syl->HasFontweight()) {
        currentFont.SetWeight(syl->GetFontweight());
    }
    if (syl->HasFontstyle()) {
        currentFont.SetStyle(syl->GetFontstyle());
    }
    if (syl->GetStart()->GetDrawingCueSize()) {
        currentFont.SetPointSize(m_doc->GetCueSize(currentFont.GetPointSize()));
    }
    dc->SetFont(&currentFont);

    TextDrawingParams params;
    params.m_x = syl->GetDrawingX();
    params.m_y = syl->GetDrawingY();
    if (m_doc->GetType() == Facs) {
        params.m_width = syl->GetDrawingWidth();
        params.m_height = syl->GetDrawingHeight();
    }
    assert(dc->GetFont());
    params.m_pointSize = dc->GetFont()->GetPointSize();

    dc->StartText(ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y));
    this->DrawTextChildren(dc, syl, params);

    if (syl->GetCon() == sylLog_CON_b) {
        dc->ReactivateGraphic();
        dc->DeactivateGraphic();
        FontInfo vrvTxt;
        vrvTxt.SetPointSize(dc->GetFont()->GetPointSize() * m_doc->GetMusicToLyricFontSizeRatio());
        vrvTxt.SetFaceName(m_doc->GetOptions()->m_font.GetValue());
        vrvTxt.SetSmuflFont(true);
        dc->SetFont(&vrvTxt);
        std::u32string str;
        str.push_back(SMUFL_E551_lyricsElision);
        dc->DrawText(UTF32to8(str), str);
        dc->ResetFont();
        dc->ReactivateGraphic();
        dc->DeactivateGraphicY();
    }

    dc->EndText();

    dc->ResetFont();
    dc->ResetBrush();

    if (syl->GetStart() && syl->GetEnd()) {
        System *currentSystem = dynamic_cast<System *>(measure->GetFirstAncestor(SYSTEM));
        // Postpone the drawing of the syl to the end of the system; this will call DrawSylConnector
        // that will look if the last note is in the same system (or not) and draw the connectors accordingly
        if (currentSystem) {
            currentSystem->AddToDrawingList(syl);
        }
    }

    dc->ReactivateGraphic();
    dc->EndGraphic(syl, this);
}

void View::DrawVerse(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure)
{
    assert(dc);
    assert(element);
    assert(layer);
    assert(staff);
    assert(measure);

    Verse *verse = vrv_cast<Verse *>(element);
    assert(verse);

    Label *label = dynamic_cast<Label *>(verse->FindDescendantByType(LABEL, 1));
    LabelAbbr *labelAbbr = verse->GetDrawingLabelAbbr();

    if (label || labelAbbr) {

        Object *graphic = NULL;
        if (label) {
            graphic = label;
        }
        else {
            graphic = labelAbbr;
        }

        LayerElement *layerElement
            = vrv_cast<LayerElement *>(element->GetFirstAncestorInRange(LAYER_ELEMENT, LAYER_ELEMENT_max));

        FontInfo labelTxt;
        if (!dc->UseGlobalStyling()) {
            labelTxt.SetFaceName("Times");
        }
        int pointSize = m_doc->GetDrawingLyricFont(staff->m_drawingStaffSize)->GetPointSize();
        if (layerElement && layerElement->GetDrawingCueSize()) {
            pointSize = m_doc->GetCueSize(pointSize);
        }
        labelTxt.SetPointSize(pointSize);

        TextDrawingParams params;
        params.m_x = verse->GetDrawingX() - m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        params.m_y = staff->GetDrawingY() + this->GetSylYRel(std::max(1, verse->GetN()), staff);
        params.m_pointSize = labelTxt.GetPointSize();

        dc->SetBrush(m_currentColour, AxSOLID);
        dc->SetFont(&labelTxt);

        dc->StartGraphic(graphic, "", graphic->GetID());

        dc->StartText(ToDeviceContextX(params.m_x), ToDeviceContextY(params.m_y), HORIZONTALALIGNMENT_right);
        this->DrawTextChildren(dc, graphic, params);
        dc->EndText();

        dc->EndGraphic(graphic, this);

        dc->ResetFont();
        dc->ResetBrush();
    }

    dc->StartGraphic(verse, "", verse->GetID());

    this->DrawLayerChildren(dc, verse, layer, staff, measure);

    dc->EndGraphic(verse, this);
}

//----------------------------------------------------------------------------
// Partial drawing methods
//----------------------------------------------------------------------------

void View::DrawAcciaccaturaSlash(DeviceContext *dc, Stem *stem, Staff *staff)
{
    assert(dc);
    assert(stem);
    assert(staff);

    dc->SetPen(AxNONE, m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize) * 1.2, AxSOLID);
    dc->SetBrush(AxNONE, AxSOLID);

    int positionShift = m_doc->GetCueSize(m_doc->GetDrawingUnit(staff->m_drawingStaffSize));
    int positionShiftX1 = positionShift;
    int positionShiftY1 = positionShift * -4;
    int positionShiftX2 = positionShift * 2;
    int positionShiftY2 = positionShift * -1;

    const data_STEMDIRECTION stemDir = stem->GetDrawingStemDir();
    int y = stem->GetDrawingY() - stem->GetDrawingStemLen();
    Flag *flag = dynamic_cast<Flag *>(stem->GetFirst(FLAG));
    if (flag) {
        const char32_t glyph = flag->GetFlagGlyph(stemDir);
        if (glyph) {
            const int slashAdjust = (stemDir == STEMDIRECTION_up)
                ? m_doc->GetGlyphTop(glyph, staff->m_drawingStaffSize, true)
                : m_doc->GetGlyphBottom(glyph, staff->m_drawingStaffSize, true);
            y += slashAdjust;
        }
    }
    if ((stemDir == STEMDIRECTION_down) && (!flag || (flag->GetFlagGlyph(stemDir) == SMUFL_E241_flag8thDown))) {
        y -= m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 3;
    }

    Point startPoint(stem->GetDrawingX(), y);

    // HARDCODED
    if (stemDir == STEMDIRECTION_up) {
        dc->DrawLine(ToDeviceContextX(startPoint.x - positionShiftX1), ToDeviceContextY(startPoint.y + positionShiftY1),
            ToDeviceContextX(startPoint.x + positionShiftX2), ToDeviceContextY(startPoint.y + positionShiftY2));
    }
    else {
        dc->DrawLine(ToDeviceContextX(startPoint.x - positionShiftX1), ToDeviceContextY(startPoint.y - positionShiftY1),
            ToDeviceContextX(startPoint.x + positionShiftX2), ToDeviceContextY(startPoint.y - positionShiftY2));
    }

    dc->ResetPen();
    dc->ResetBrush();
}

void View::DrawDotsPart(DeviceContext *dc, int x, int y, unsigned char dots, const Staff *staff, bool dimin)
{
    const int unit = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    if (staff->IsOnStaffLine(y, m_doc)) {
        y += unit;
    }
    const double distance = dimin ? m_doc->GetOptions()->m_graceFactor.GetValue() : 1.0;
    for (int i = 0; i < dots; ++i) {
        if (staff->IsMensural()) {
            this->DrawDiamond(dc, x - unit / 2, y, unit, unit, true, 0);
        }
        else {
            this->DrawDot(dc, x, y, staff->m_drawingStaffSize, dimin);
        }
        // HARDCODED
        x += m_doc->GetDrawingUnit(staff->m_drawingStaffSize) * 1.5 * distance;
    }
}

int View::DrawMeterSigFigures(DeviceContext *dc, int x, int y, MeterSig *meterSig, int den, Staff *staff)
{
    assert(dc);
    assert(staff);

    const auto [numSummands, numSign] = meterSig->GetCount();
    std::u32string timeSigCombNumerator, timeSigCombDenominator;
    for (int summand : numSummands) {
        if (!timeSigCombNumerator.empty()) {
            switch (numSign) {
                case MeterCountSign::Slash: timeSigCombNumerator += SMUFL_E08E_timeSigFractionalSlash; break;
                case MeterCountSign::Minus: timeSigCombNumerator += SMUFL_E090_timeSigMinus; break;
                case MeterCountSign::Asterisk: timeSigCombNumerator += SMUFL_E091_timeSigMultiply; break;
                case MeterCountSign::Plus: timeSigCombNumerator += SMUFL_E08D_timeSigPlusSmall; break;
                case MeterCountSign::None:
                default: break;
            }
        }
        timeSigCombNumerator += IntToTimeSigFigures(summand);
    }
    if (den) timeSigCombDenominator = this->IntToTimeSigFigures(den);

    const int glyphSize = staff->GetDrawingStaffNotationSize();

    dc->SetFont(m_doc->GetDrawingSmuflFont(glyphSize, false));

    std::u32string widthText = (timeSigCombNumerator.length() > timeSigCombDenominator.length())
        ? timeSigCombNumerator
        : timeSigCombDenominator;

    TextExtend extend;
    dc->GetSmuflTextExtent(widthText, &extend);
    const int width = extend.m_width;
    x += width / 2;

    if (den) {
        int yNum = y + m_doc->GetDrawingDoubleUnit(glyphSize);
        int yDen = y - m_doc->GetDrawingDoubleUnit(glyphSize);
        // In case when one of the handwritten fonts is used, we need to make sure that meterSig is displayed properly
        // for it, based on the height of corresponding glyphs
        FontInfo *fontInfo = dc->GetFont();
        std::vector<std::string> handwrittenFonts = m_doc->GetOptions()->m_handwrittenFont.GetValue();
        if (const auto it = std::find(handwrittenFonts.begin(), handwrittenFonts.end(), fontInfo->GetFaceName());
            it != handwrittenFonts.end()) {
            TextExtend numExtend;
            dc->GetSmuflTextExtent(timeSigCombNumerator, &numExtend);
            yNum = y + numExtend.m_height / 2;

            TextExtend denExtend;
            dc->GetSmuflTextExtent(timeSigCombDenominator, &denExtend);
            yDen = y - denExtend.m_height / 2;
        }
        this->DrawSmuflString(dc, x, yNum, timeSigCombNumerator, HORIZONTALALIGNMENT_center, glyphSize);
        this->DrawSmuflString(dc, x, yDen, timeSigCombDenominator, HORIZONTALALIGNMENT_center, glyphSize);
    }
    else {
        this->DrawSmuflString(dc, x, y, timeSigCombNumerator, HORIZONTALALIGNMENT_center, glyphSize);
    }

    dc->ResetFont();

    return width;
}

void View::DrawMRptPart(DeviceContext *dc, int xCentered, char32_t rptGlyph, int num, bool line, Staff *staff)
{
    const int staffSize = staff->m_drawingStaffSize;
    const int y = staff->GetDrawingY();
    const int xSymbol = xCentered - m_doc->GetGlyphWidth(rptGlyph, staffSize, false) / 2;
    const int ySymbol = y - (staff->m_drawingLines - 1) * m_doc->GetDrawingUnit(staffSize);

    this->DrawSmuflCode(dc, xSymbol, ySymbol, rptGlyph, staffSize, false);

    if (line) {
        const int yBottom = y - (staff->m_drawingLines - 1) * m_doc->GetDrawingDoubleUnit(staffSize);
        const int offset = (y == ySymbol) ? m_doc->GetDrawingDoubleUnit(staffSize) : 0;
        this->DrawVerticalLine(dc, y + offset, yBottom - offset, xCentered, m_doc->GetDrawingBarLineWidth(staffSize));
    }

    if (num > 0) {
        dc->SetFont(m_doc->GetDrawingSmuflFont(staffSize, false));
        // calculate the width of the figures
        TextExtend extend;
        const std::u32string figures = this->IntToTimeSigFigures(num);
        dc->GetSmuflTextExtent(figures, &extend);
        const int symHeight = m_doc->GetGlyphHeight(rptGlyph, staffSize, false);
        const int yNum = (y > ySymbol + symHeight / 2)
            ? staff->GetDrawingY() + m_doc->GetDrawingUnit(staffSize) + extend.m_height / 2
            : ySymbol + 3 * m_doc->GetDrawingUnit(staffSize) + extend.m_height / 2;
        dc->DrawMusicText(figures, ToDeviceContextX(xCentered - extend.m_width / 2), ToDeviceContextY(yNum));
        dc->ResetFont();
    }
}

//----------------------------------------------------------------------------
// Calculation or preparation methods
///----------------------------------------------------------------------------

int View::GetFYRel(F *f, Staff *staff)
{
    assert(f && staff);

    int y = staff->GetDrawingY();

    StaffAlignment *alignment = staff->GetAlignment();
    // Something must be seriously wrong...
    if (!alignment) return y;

    y -= (alignment->GetStaffHeight() + alignment->GetOverflowBelow());

    FloatingPositioner *positioner = alignment->FindFirstFloatingPositioner(HARM);
    // There is no other harm, we use the bottom line.
    if (!positioner) return y;

    y = positioner->GetDrawingY();

    Object *fb = f->GetFirstAncestor(FB);
    assert(fb);
    int line = fb->GetDescendantIndex(f, FIGURE, UNLIMITED_DEPTH);

    if (line > 0) {
        FontInfo *fFont = m_doc->GetDrawingLyricFont(staff->m_drawingStaffSize);
        int lineHeight = m_doc->GetTextLineHeight(fFont, false);
        y -= (line * lineHeight);
    }

    return y;
}

int View::GetSylYRel(int verseN, Staff *staff)
{
    assert(staff);

    const bool verseCollapse = m_options->m_lyricVerseCollapse.GetValue();
    int y = 0;
    StaffAlignment *alignment = staff->GetAlignment();
    if (alignment) {
        FontInfo *lyricFont = m_doc->GetDrawingLyricFont(staff->m_drawingStaffSize);
        int descender = -m_doc->GetTextGlyphDescender(L'q', lyricFont, false);
        int height = m_doc->GetTextGlyphHeight(L'I', lyricFont, false);
        int margin = m_doc->GetBottomMargin(SYL) * m_doc->GetDrawingUnit(staff->m_drawingStaffSize);

        y = -alignment->GetStaffHeight() - alignment->GetOverflowBelow()
            + alignment->GetVersePosition(verseN, verseCollapse) * (height + descender + margin) + (descender);
    }
    return y;
}

} // namespace vrv

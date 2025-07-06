/////////////////////////////////////////////////////////////////////////////
// Name:        calcstemfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "calcstemfunctor.h"

//----------------------------------------------------------------------------

#include "beamspan.h"
#include "doc.h"
#include "elementpart.h"
#include "ftrem.h"
#include "layer.h"
#include "staff.h"
#include "stem.h"
#include "tabdursym.h"
#include "tabgrp.h"
#include "vrv.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// CalcStemFunctor
//----------------------------------------------------------------------------

CalcStemFunctor::CalcStemFunctor(Doc *doc) : DocFunctor(doc)
{
    m_chordStemLength = 0;
    m_verticalCenter = 0;
    m_dur = DURATION_1;
    m_isGraceNote = false;
    m_isStemSameasSecondary = false;
    m_tabGrpWithNoNote = false;
    m_staff = NULL;
    m_layer = NULL;
    m_interface = NULL;
}

FunctorCode CalcStemFunctor::VisitBeam(Beam *beam)
{
    const ListOfObjects &beamChildren = beam->GetList();

    // Should we assert this at the beginning?
    if (beamChildren.empty()) {
        return FUNCTOR_CONTINUE;
    }

    Layer *layer = vrv_cast<Layer *>(beam->GetFirstAncestor(LAYER));
    assert(layer);
    Staff *staff = vrv_cast<Staff *>(layer->GetFirstAncestor(STAFF));
    assert(staff);

    if (!beam->HasCoords()) {
        beam->InitCoords(beamChildren, staff, beam->GetPlace());
        const bool isCue = ((beam->GetCue() == BOOLEAN_true) || beam->GetFirstAncestor(GRACEGRP));
        beam->InitGraceStemDir(beam->GetFirstAncestor(GRACEGRP));
        beam->InitCue(isCue);
    }

    if (beam->IsTabBeam()) return FUNCTOR_CONTINUE;

    BeamSegment &segment = beam->m_beamSegment;
    segment.InitCoordRefs(beam->GetElementCoords());

    data_BEAMPLACE initialPlace = beam->GetPlace();
    if (beam->HasStemSameasBeam()) segment.InitSameasRoles(beam->GetStemSameasBeam(), initialPlace);

    segment.CalcBeam(layer, staff, m_doc, beam, initialPlace);

    if (beam->HasStemSameasBeam()) segment.CalcNoteHeadShiftForStemSameas(beam->GetStemSameasBeam(), initialPlace);

    return FUNCTOR_CONTINUE;
}

FunctorCode CalcStemFunctor::VisitBeamSpan(BeamSpan *beamSpan)
{
    if (!beamSpan->GetStart() || !beamSpan->GetEnd() || beamSpan->GetBeamedElements().empty()) return FUNCTOR_CONTINUE;

    Layer *layer = vrv_cast<Layer *>(beamSpan->GetStart()->GetFirstAncestor(LAYER));
    Staff *staff = vrv_cast<Staff *>(beamSpan->GetStart()->GetFirstAncestor(STAFF));
    Measure *measure = vrv_cast<Measure *>(beamSpan->GetStart()->GetFirstAncestor(MEASURE));

    beamSpan->InitCoords(beamSpan->GetBeamedElements(), staff, beamSpan->GetPlace());

    BeamSpanSegment *firstSegment = beamSpan->GetSegment(0);
    firstSegment->SetMeasure(measure);
    firstSegment->SetStaff(staff);
    firstSegment->SetLayer(layer);
    ArrayOfBeamElementCoords coord(beamSpan->m_beamElementCoords.begin(), beamSpan->m_beamElementCoords.end());
    firstSegment->SetBeginCoord(coord.front());
    firstSegment->SetEndCoord(coord.back());
    firstSegment->InitCoordRefs(&coord);
    firstSegment->CalcBeam(layer, staff, m_doc, beamSpan, beamSpan->GetPlace());

    return FUNCTOR_CONTINUE;
}

FunctorCode CalcStemFunctor::VisitChord(Chord *chord)
{
    // Set them to NULL in any case
    m_interface = NULL;

    // Stems have been calculated previously in beam or fTrem - siblings because flags do not need to
    // be processed either
    if (chord->IsInBeam() || chord->GetAncestorFTrem()) {
        return FUNCTOR_SIBLINGS;
    }

    // if the chord isn't visible, carry on
    if (!chord->IsVisible() || (chord->GetStemVisible() == BOOLEAN_false)) {
        return FUNCTOR_SIBLINGS;
    }

    Stem *stem = chord->GetDrawingStem();
    assert(stem);
    Staff *staff = chord->GetAncestorStaff();
    Layer *layer = vrv_cast<Layer *>(chord->GetFirstAncestor(LAYER));
    assert(layer);

    if (chord->m_crossStaff) {
        staff = chord->m_crossStaff;
        layer = chord->m_crossLayer;
    }

    // Cache to avoid further lookup
    m_staff = staff;
    m_layer = layer;
    m_interface = chord;
    m_dur = chord->GetNoteOrChordDur(chord);
    m_isGraceNote = chord->IsGraceNote();
    m_isStemSameasSecondary = false;

    /************ Set the direction ************/

    int yMax, yMin;
    chord->GetYExtremes(yMax, yMin);
    m_chordStemLength = yMin - yMax;

    int staffY = staff->GetDrawingY();
    int staffSize = staff->m_drawingStaffSize;
    m_verticalCenter = staffY - m_doc->GetDrawingDoubleUnit(staffSize) * 2;

    data_STEMDIRECTION layerStemDir;
    data_STEMDIRECTION stemDir = STEMDIRECTION_NONE;

    if (stem->HasDir()) {
        stemDir = stem->GetDir();
    }
    else if ((layerStemDir = layer->GetDrawingStemDir(chord)) != STEMDIRECTION_NONE) {
        stemDir = layerStemDir;
    }
    else {
        stemDir = this->CalcStemDirection(chord, m_verticalCenter);
    }

    chord->SetDrawingStemDir(stemDir);

    // Position the stem to the bottom note when up
    if (stemDir == STEMDIRECTION_up) {
        stem->SetDrawingYRel(yMin - chord->GetDrawingY());
    }
    // And to the top note when down
    else {
        stem->SetDrawingYRel(yMax - chord->GetDrawingY());
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode CalcStemFunctor::VisitFTrem(FTrem *fTrem)
{
    const ListOfObjects &fTremChildren = fTrem->GetList();

    // Should we assert this at the beginning?
    if (fTremChildren.empty()) {
        return FUNCTOR_CONTINUE;
    }

    Layer *layer = vrv_cast<Layer *>(fTrem->GetFirstAncestor(LAYER));
    assert(layer);
    Staff *staff = vrv_cast<Staff *>(layer->GetFirstAncestor(STAFF));
    assert(staff);

    if (!fTrem->HasCoords()) {
        fTrem->InitCoords(fTremChildren, staff, BEAMPLACE_NONE);
        fTrem->InitCue(false);
    }

    if (fTrem->GetElementCoords()->size() != 2) {
        LogError("Stem calculation: <fTrem> element has invalid number of descendants.");
        return FUNCTOR_CONTINUE;
    }

    BeamSegment &segment = fTrem->m_beamSegment;

    segment.InitCoordRefs(fTrem->GetElementCoords());
    segment.CalcBeam(layer, staff, m_doc, fTrem);

    return FUNCTOR_CONTINUE;
}

FunctorCode CalcStemFunctor::VisitNote(Note *note)
{
    if (!note->IsVisible() || (note->GetStemVisible() == BOOLEAN_false)) {
        return FUNCTOR_SIBLINGS;
    }

    // Stems have been calculated previously in Beam or fTrem - siblings because flags do not need to
    // be processed either
    if (note->IsInBeam() || note->GetAncestorFTrem()) {
        return FUNCTOR_SIBLINGS;
    }

    // We do not need to calc stems for mensural notes
    // We have no stem with tab because it belongs to tabDurSym in this case
    if (note->IsMensuralDur() || note->IsTabGrpNote()) {
        return FUNCTOR_SIBLINGS;
    }

    if (note->IsChordTone()) {
        assert(m_interface);
        return FUNCTOR_CONTINUE;
    }

    // This now should be NULL and the chord stem length will be 0
    m_interface = NULL;
    m_chordStemLength = 0;

    Stem *stem = note->GetDrawingStem();
    assert(stem);
    Staff *staff = note->GetAncestorStaff();
    Layer *layer = vrv_cast<Layer *>(note->GetFirstAncestor(LAYER));
    assert(layer);

    if (note->m_crossStaff) {
        staff = note->m_crossStaff;
        layer = note->m_crossLayer;
    }

    // Cache to avoid further lookup
    m_staff = staff;
    m_layer = layer;
    m_interface = note;
    m_dur = note->GetActualDur();
    m_isGraceNote = note->IsGraceNote();
    m_isStemSameasSecondary = false;

    const int staffSize = staff->m_drawingStaffSize;

    m_verticalCenter = staff->GetDrawingY() - m_doc->GetDrawingUnit(staffSize) * (staff->m_drawingLines - 1);

    /************ Set the direction ************/

    data_STEMDIRECTION layerStemDir;
    data_STEMDIRECTION stemDir = STEMDIRECTION_NONE;

    if (note->HasStemSameasNote()) {
        stemDir = note->CalcStemDirForSameasNote(m_verticalCenter);
    }
    else if (stem->HasDir()) {
        stemDir = stem->GetDir();
    }
    else if (note->IsGraceNote()) {
        stemDir = STEMDIRECTION_up;
    }
    else if ((layerStemDir = layer->GetDrawingStemDir(note)) != STEMDIRECTION_NONE) {
        stemDir = layerStemDir;
    }
    else {
        stemDir = (note->GetDrawingY() >= m_verticalCenter) ? STEMDIRECTION_down : STEMDIRECTION_up;
    }

    note->SetDrawingStemDir(stemDir);

    // Make sure the relative position of the stem is the same
    stem->SetDrawingYRel(0);

    // Use m_chordStemLength for the length of the stem between the notes
    // The value of m_stemSameasRole is set by Note::CalcStemDirForSameasNote
    if (note->HasStemSameasNote() && (note->GetStemSameasRole() == SAMEAS_SECONDARY)) {
        m_chordStemLength = -std::abs(note->GetDrawingY() - note->GetStemSameasNote()->GetDrawingY());
        m_isStemSameasSecondary = true;
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode CalcStemFunctor::VisitStaff(Staff *staff)
{
    ListOfObjects layers = staff->FindAllDescendantsByType(LAYER, false);
    if (layers.empty()) {
        return FUNCTOR_CONTINUE;
    }

    // Not more than one layer - drawing stem dir remains unset unless there is cross-staff content
    if (layers.size() < 2) {
        Layer *layer = vrv_cast<Layer *>(layers.front());
        assert(layer);
        if (layer->HasCrossStaffFromBelow()) {
            layer->SetDrawingStemDir(STEMDIRECTION_up);
        }
        else if (layer->HasCrossStaffFromAbove()) {
            layer->SetDrawingStemDir(STEMDIRECTION_down);
        }
        return FUNCTOR_CONTINUE;
    }

    // Detecting empty layers (empty layers can also have @sameas) which have to be ignored for stem direction
    IsEmptyComparison isEmptyElement(LAYER);
    ListOfObjects emptyLayers;
    staff->FindAllDescendantsByComparison(&emptyLayers, &isEmptyElement);

    // We have only one layer (or less) with content - drawing stem dir remains unset
    if ((layers.size() < 3) && (emptyLayers.size() > 0)) {
        return FUNCTOR_CONTINUE;
    }

    if (!emptyLayers.empty()) {
        ListOfObjects nonEmptyLayers;
        // no need to sort since it is already sorted
        std::set_difference(layers.begin(), layers.end(), emptyLayers.begin(), emptyLayers.end(),
            std::inserter(nonEmptyLayers, nonEmptyLayers.begin()));
        layers = nonEmptyLayers;
    }

    for (auto &object : layers) {
        // Alter stem direction between even and odd numbered layers
        Layer *layer = vrv_cast<Layer *>(object);
        layer->SetDrawingStemDir(layer->GetN() % 2 ? STEMDIRECTION_up : STEMDIRECTION_down);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode CalcStemFunctor::VisitStem(Stem *stem)
{
    assert(m_staff);
    assert(m_layer);
    assert(m_interface);

    const int staffSize = m_staff->m_drawingStaffSize;
    const int stemShift = m_doc->GetDrawingStemWidth(staffSize) / 2;
    const bool drawingCueSize = stem->GetDrawingCueSize();

    // For notes longer than half notes the stem is always 0
    if (m_dur < DURATION_2) {
        stem->SetDrawingXRel(0);
        stem->SetDrawingYRel(0);
        stem->SetDrawingStemLen(0);
        const int adjust = stem->CalculateStemModAdjustment(m_doc, m_staff, 0);
        if (adjust) stem->SetDrawingStemLen(stem->GetDrawingStemLen() + adjust);
        return FUNCTOR_CONTINUE;
    }

    /************ Set the position, the length and adjust to the note head ************/

    const int unit = m_doc->GetDrawingUnit(staffSize);
    int baseStem = 0;
    // Use the given one if any
    if (stem->HasLen() && stem->GetLen().GetType() == MEASUREMENTTYPE_vu) {
        baseStem = stem->GetLen().GetVu() * -unit;
    }
    // Do not adjust the baseStem for stem sameas notes (its length is in m_chordStemLength)
    else if (!m_isStemSameasSecondary) {
        const int thirdUnit = unit / 3;
        const data_STEMDIRECTION stemDir = m_interface->GetDrawingStemDir();
        baseStem = -(m_interface->CalcStemLenInThirdUnits(m_staff, stemDir) * thirdUnit);
        if (drawingCueSize) baseStem = m_doc->GetCueSize(baseStem);
    }
    // Even if a stem length is given we add the length of the chord content (however only if not 0)
    // Also, the given stem length is understood as being measured from the center of the note.
    // This means that it will be adjusted according to the note head
    if (!m_staff || !stem->HasLen() || (stem->GetLen().GetVu() != 0)) {
        Point p;
        if (stem->GetDrawingStemDir() == STEMDIRECTION_up) {
            if (stem->GetPos() == STEMPOSITION_left) {
                p = m_interface->GetStemDownNW(m_doc, staffSize, drawingCueSize);
                p.x += stemShift;
            }
            else {
                p = m_interface->GetStemUpSE(m_doc, staffSize, drawingCueSize);
                p.x -= stemShift;
            }
            const int stemShortening = m_isStemSameasSecondary ? 0 : p.y;
            stem->SetDrawingStemLen(baseStem + m_chordStemLength + stemShortening);
        }
        else {
            if (stem->GetPos() == STEMPOSITION_right) {
                p = m_interface->GetStemUpSE(m_doc, staffSize, drawingCueSize);
                p.x -= stemShift;
            }
            else {
                p = m_interface->GetStemDownNW(m_doc, staffSize, drawingCueSize);
                p.x += stemShift;
            }
            const int stemShortening = m_isStemSameasSecondary ? 0 : p.y;
            stem->SetDrawingStemLen(-(baseStem + m_chordStemLength - stemShortening));
        }
        stem->SetDrawingYRel(stem->GetDrawingYRel() + p.y);
        stem->SetDrawingXRel(p.x);
    }

    /************ Set flag and slashes (if necessary) and adjust the length ************/
    int flagOffset = 0;
    Flag *flag = NULL;
    // There is never a flag with a duration longer than 8th notes
    if (m_dur > DURATION_4) {
        flag = vrv_cast<Flag *>(stem->GetFirst(FLAG));
        assert(flag);
        // There is never a flag with stem sameas notes
        if (m_isStemSameasSecondary) {
            flag->m_drawingNbFlags = 0;
        }
        else {
            flag->m_drawingNbFlags = m_dur - DURATION_4;
            flagOffset = unit * (flag->m_drawingNbFlags + 1);
        }
    }

    // SMUFL flags cover some additional stem length from the 32th only
    if (flag) {
        flag->SetDrawingYRel(-stem->GetDrawingStemLen());
    }

    // Do not adjust the length with stem sameas notes or if given in the encoding
    // however, the stem will be extended with the SMuFL extension from 32th - this can be improved
    if (m_isStemSameasSecondary || stem->HasLen()) {
        if ((stem->GetLen().GetVu() == 0) && flag) flag->m_drawingNbFlags = 0;
        return FUNCTOR_CONTINUE;
    }
    if ((stem->GetVisible() == BOOLEAN_false) && flag) {
        flag->m_drawingNbFlags = 0;
        return FUNCTOR_CONTINUE;
    }

    int flagHeight = 0;

    // SMUFL flags cover some additional stem length from the 32th only
    if (m_dur > DURATION_16) {
        assert(flag);
        Point stemEnd;
        // Commented since this is crashing - needs investigating
        if (stem->GetDrawingStemDir() == STEMDIRECTION_up) {
            // stemEnd = flag->GetStemUpSE(m_doc, staffSize, drawingCueSize);
        }
        else {
            // stemEnd = flag->GetStemDownNW(m_doc, staffSize, drawingCueSize);
        }
        // Trick for shortening the stem with DURATION_8
        flagHeight = stemEnd.y;
    }

    int endY = stem->GetDrawingY() - stem->GetDrawingStemLen() + flagHeight;
    bool adjust = false;
    if ((stem->GetDrawingStemDir() == STEMDIRECTION_up) && (endY < m_verticalCenter)) {
        adjust = true;
    }
    else if ((stem->GetDrawingStemDir() == STEMDIRECTION_down) && (endY > m_verticalCenter)) {
        adjust = true;
    }

    // Do not adjust the length of grace notes - this is debatable and should probably become a styling option
    // However we still want flags from grace notes not to overlap with ledger lines
    if (adjust && !m_isGraceNote) {
        stem->SetDrawingStemLen(stem->GetDrawingStemLen() + (endY - m_verticalCenter));
        if (flag) flag->SetDrawingYRel(-stem->GetDrawingStemLen());
    }

    if (!m_isGraceNote && !drawingCueSize && !m_isStemSameasSecondary) {
        const int adjust = stem->CalculateStemModAdjustment(m_doc, m_staff, flagOffset);
        if (adjust) stem->SetDrawingStemLen(stem->GetDrawingStemLen() + adjust);
        if (flag) flag->SetDrawingYRel(-stem->GetDrawingStemLen());
    }

    if (flag) this->AdjustFlagPlacement(m_doc, stem, flag, staffSize, m_verticalCenter, m_dur);

    return FUNCTOR_CONTINUE;
}

FunctorCode CalcStemFunctor::VisitTabDurSym(TabDurSym *tabDurSym)
{
    // Stems have been calculated previously in Beam
    if (tabDurSym->IsInBeam()) {
        return FUNCTOR_SIBLINGS;
    }

    // This now should be NULL and the chord stem length will be 0
    m_interface = NULL;
    m_chordStemLength = 0;

    Stem *stem = tabDurSym->GetDrawingStem();
    assert(stem);

    // Do not draw virtual (e.g., whole note) stems
    if (m_dur < DURATION_2 || m_tabGrpWithNoNote) {
        stem->IsVirtual(true);
        return FUNCTOR_SIBLINGS;
    }

    // Cache to avoid further lookup
    m_staff = tabDurSym->GetAncestorStaff();
    assert(m_staff);
    m_layer = vrv_cast<Layer *>(tabDurSym->GetFirstAncestor(LAYER));
    assert(m_layer);
    m_interface = tabDurSym;
    // Grace and stem sameas not supported in tablature
    m_isGraceNote = false;
    m_isStemSameasSecondary = false;

    int staffSize = m_staff->m_drawingStaffSize;

    /************ Set the direction ************/

    data_STEMDIRECTION layerStemDir;
    // Up by default with tablature
    data_STEMDIRECTION stemDir = STEMDIRECTION_up;

    if (stem->HasDir()) {
        stemDir = stem->GetDir();
    }
    else if ((layerStemDir = m_layer->GetDrawingStemDir()) != STEMDIRECTION_NONE) {
        stemDir = layerStemDir;
    }

    tabDurSym->SetDrawingStemDir(stemDir);

    int stemDirFactor = -1;
    if (stemDir == STEMDIRECTION_down) {
        tabDurSym->AdjustDrawingYRel(m_staff, m_doc);
        stemDirFactor = 1;
    }

    if (m_staff->IsTabWithStemsOutside()) {
        // Make sure the relative position of the stem is the same
        stem->SetDrawingYRel(0);
    }
    else {
        // Otherwise attach it to the closest note
        TabGrp *tabGrp = vrv_cast<TabGrp *>(tabDurSym->GetFirstAncestor(TABGRP));
        assert(tabGrp);
        Note *note = (stemDir == STEMDIRECTION_down) ? tabGrp->GetBottomNote() : tabGrp->GetTopNote();
        int yRel = note ? note->GetDrawingYRel() : 0;
        // Because the tabDurSym is relative to the top or bottom staff line, remove its relative value
        yRel -= tabDurSym->GetDrawingYRel();
        // Remove a unit for the stem not to go to the center of the note
        yRel -= m_doc->GetDrawingUnit(m_staff->m_drawingStaffSize) * stemDirFactor;
        stem->SetDrawingYRel(yRel);
    }

    int stemSize = tabDurSym->CalcStemLenInThirdUnits(m_staff, stemDir) * m_doc->GetDrawingUnit(staffSize);
    stemSize /= (3 * stemDirFactor);

    if (m_dur == DURATION_2) {
        // Stems for half notes twice shorter
        stemSize /= 2;
    }

    stem->SetDrawingStemLen(stemSize);

    // Flag currently used only for guitar tablature because it is included in the glyphs for lute tab
    if (m_staff->IsTabGuitar()) {
        Flag *flag = vrv_cast<Flag *>(stem->GetFirst(FLAG));
        if (flag) {
            flag->m_drawingNbFlags = m_dur - DURATION_4;
            flag->SetDrawingYRel(-stemSize);
        }
    }

    // Do not call VisitStem with TabDurSym because everything is done here
    return FUNCTOR_SIBLINGS;
}

FunctorCode CalcStemFunctor::VisitTabGrp(TabGrp *tabGrp)
{
    m_dur = tabGrp->GetActualDur();
    m_tabGrpWithNoNote = !tabGrp->FindDescendantByType(NOTE);

    return FUNCTOR_CONTINUE;
}

data_STEMDIRECTION CalcStemFunctor::CalcStemDirection(const Chord *chord, int verticalCenter) const
{
    const ListOfConstObjects &childList = chord->GetList();
    ListOfConstObjects topNotes, bottomNotes;

    // split notes into two vectors - notes above vertical center and below
    std::partition_copy(childList.begin(), childList.end(), std::back_inserter(topNotes),
        std::back_inserter(bottomNotes),
        [verticalCenter](const Object *note) { return note->GetDrawingY() > verticalCenter; });

    auto bottomIter = bottomNotes.begin();
    auto topIter = topNotes.rbegin();
    for (; bottomIter != bottomNotes.end() && topIter != topNotes.rend(); ++bottomIter, ++topIter) {
        const int bottomY = (*bottomIter)->GetDrawingY();
        const int topY = (*topIter)->GetDrawingY();
        const int middlePoint = (topY + bottomY) / 2;

        // if notes are equidistant - proceed to the next pair of notes
        if (middlePoint == verticalCenter) {
            continue;
        }
        // otherwise return corresponding stem direction
        else if (middlePoint > verticalCenter) {
            return STEMDIRECTION_down;
        }
        else {
            return STEMDIRECTION_up;
        }
    }

    // if there are still unprocessed notes left on the bottom that are not on the center - stem direction should be up
    if ((bottomIter != bottomNotes.end()) && ((*bottomIter)->GetDrawingY() != verticalCenter)) {
        return STEMDIRECTION_up;
    }
    // otherwise place it down
    return STEMDIRECTION_down;
}

void CalcStemFunctor::AdjustFlagPlacement(
    const Doc *doc, Stem *stem, Flag *flag, int staffSize, int verticalCenter, data_DURATION duration) const
{
    assert(stem->GetParent());
    assert(stem->GetParent()->IsLayerElement());

    LayerElement *parent = vrv_cast<LayerElement *>(stem->GetParent());
    if (!parent) return;

    const data_STEMDIRECTION stemDirection = stem->GetDrawingStemDir();
    // For overlapping purposes we don't care for flags shorter than 16th since they grow in opposite direction
    char32_t flagGlyph = SMUFL_E242_flag16thUp;
    if (duration < DURATION_16) flagGlyph = flag->GetFlagGlyph(stemDirection);
    const int glyphHeight = doc->GetGlyphHeight(flagGlyph, staffSize, stem->GetDrawingCueSize());

    // Make sure that flags don't overlap with notehead. Upward flags cannot overlap with noteheads so check
    // only downward ones
    const int adjustmentStep = doc->GetDrawingUnit(staffSize);
    if (stemDirection == STEMDIRECTION_down) {
        const int noteheadMargin = stem->GetDrawingStemLen() - (glyphHeight + parent->GetDrawingRadius(doc));
        if ((duration > DURATION_16) && (noteheadMargin < 0)) {
            int offset = 0;
            if (noteheadMargin % adjustmentStep < -adjustmentStep / 3 * 2) offset = adjustmentStep / 2;
            const int heightToAdjust = (noteheadMargin / adjustmentStep) * adjustmentStep - offset;
            stem->SetDrawingStemLen(stem->GetDrawingStemLen() - heightToAdjust);
            flag->SetDrawingYRel(-stem->GetDrawingStemLen());
        }
    }

    Note *note = NULL;
    if (parent->Is(NOTE)) {
        note = vrv_cast<Note *>(parent);
    }
    else if (parent->Is(CHORD)) {
        Chord *chord = vrv_cast<Chord *>(parent);
        assert(chord);
        note = chord->GetTopNote();
    }
    assert(note);
    int ledgerAbove = 0;
    int ledgerBelow = 0;

    Staff *staff = note->GetAncestorStaff(RESOLVE_CROSS_STAFF);
    if (!note || !note->HasLedgerLines(ledgerAbove, ledgerBelow, staff)) return;
    if (((stemDirection == STEMDIRECTION_up) && !ledgerBelow)
        || ((stemDirection == STEMDIRECTION_down) && !ledgerAbove))
        return;

    // Make sure that flags don't overlap with first (top or bottom) ledger line (effectively avoiding all ledgers)
    const int directionBias = (stemDirection == STEMDIRECTION_down) ? -1 : 1;
    const int position = stem->GetDrawingY() - stem->GetDrawingStemLen() - directionBias * glyphHeight;
    const int ledgerPosition = verticalCenter - 6 * directionBias * adjustmentStep;
    const int displacementMargin = (position - ledgerPosition) * directionBias;

    if (displacementMargin < 0) {
        int offset = 0;
        if ((stemDirection == STEMDIRECTION_down) && (displacementMargin % adjustmentStep > -adjustmentStep / 3)) {
            offset = adjustmentStep / 2;
        }
        const int heightToAdjust = (displacementMargin / adjustmentStep - 1) * adjustmentStep * directionBias - offset;
        stem->SetDrawingStemLen(stem->GetDrawingStemLen() + heightToAdjust);
        flag->SetDrawingYRel(-stem->GetDrawingStemLen());
    }
}

} // namespace vrv

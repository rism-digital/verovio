/////////////////////////////////////////////////////////////////////////////
// Name:        alignfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "alignfunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "dot.h"
#include "layer.h"
#include "ligature.h"
#include "rest.h"
#include "section.h"
#include "staff.h"
#include "system.h"
#include "tabgrp.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AlignHorizontallyFunctor
//----------------------------------------------------------------------------

AlignHorizontallyFunctor::AlignHorizontallyFunctor(Doc *doc) : DocFunctor(doc)
{
    m_measureAligner = NULL;
    m_time = 0.0;
    m_currentMensur = NULL;
    m_currentMeterSig = NULL;
    m_notationType = NOTATIONTYPE_cmn;
    m_scoreDefRole = SCOREDEF_NONE;
    m_isFirstMeasure = false;
    m_hasMultipleLayer = false;
}

FunctorCode AlignHorizontallyFunctor::VisitLayer(Layer *layer)
{
    m_currentMensur = layer->GetCurrentMensur();
    m_currentMeterSig = layer->GetCurrentMeterSig();

    // We are starting a new layer, reset the time;
    // We set it to -1.0 for the scoreDef attributes since they have to be aligned before any timestamp event (-1.0)
    m_time = DUR_MAX * -1.0;

    m_scoreDefRole = m_isFirstMeasure ? SCOREDEF_SYSTEM : SCOREDEF_INTERMEDIATE;

    if (layer->GetStaffDefClef()) {
        if (layer->GetStaffDefClef()->GetVisible() != BOOLEAN_false) {
            this->VisitClef(layer->GetStaffDefClef());
        }
    }
    if (layer->GetStaffDefKeySig()) {
        if (layer->GetStaffDefKeySig()->GetVisible() != BOOLEAN_false) {
            this->VisitKeySig(layer->GetStaffDefKeySig());
        }
    }
    if (layer->GetStaffDefMensur()) {
        this->VisitMensur(layer->GetStaffDefMensur());
    }
    if (layer->GetStaffDefMeterSigGrp()) {
        layer->GetStaffDefMeterSigGrp()->Process(*this);
        // Inner process may stop, still continue the outer process
        this->ResetCode();
    }
    else if (layer->GetStaffDefMeterSig()) {
        if (layer->GetStaffDefMeterSig()->GetForm() != METERFORM_invis) {
            this->VisitMeterSig(layer->GetStaffDefMeterSig());
        }
    }

    m_scoreDefRole = SCOREDEF_NONE;

    // Now we have to set it to 0.0 since we will start aligning musical content
    m_time = 0.0;

    return FUNCTOR_CONTINUE;
}

FunctorCode AlignHorizontallyFunctor::VisitLayerEnd(Layer *layer)
{
    m_scoreDefRole = SCOREDEF_CAUTIONARY;
    m_time = m_measureAligner->GetMaxTime();

    if (layer->GetCautionStaffDefClef()) {
        this->VisitClef(layer->GetCautionStaffDefClef());
    }
    if (layer->GetCautionStaffDefKeySig()) {
        this->VisitKeySig(layer->GetCautionStaffDefKeySig());
    }
    if (layer->GetCautionStaffDefMensur()) {
        this->VisitMensur(layer->GetCautionStaffDefMensur());
    }
    if (layer->GetCautionStaffDefMeterSig()) {
        this->VisitMeterSig(layer->GetCautionStaffDefMeterSig());
    }

    m_scoreDefRole = SCOREDEF_NONE;

    Staff *staff = vrv_cast<Staff *>(layer->GetFirstAncestor(STAFF));
    assert(staff);
    int graceAlignerId = m_doc->GetOptions()->m_graceRhythmAlign.GetValue() ? 0 : staff->GetN();

    for (int i = 0; i < m_measureAligner->GetChildCount(); ++i) {
        Alignment *alignment = vrv_cast<Alignment *>(m_measureAligner->GetChild(i));
        assert(alignment);
        if (alignment->HasGraceAligner(graceAlignerId)) {
            alignment->GetGraceAligner(graceAlignerId)->AlignStack();
        }
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode AlignHorizontallyFunctor::VisitLayerElement(LayerElement *layerElement)
{
    assert(!layerElement->GetAlignment());

    if (layerElement->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    layerElement->SetScoreDefRole(m_scoreDefRole);

    AlignmentType type = ALIGNMENT_DEFAULT;

    Chord *chordParent = vrv_cast<Chord *>(layerElement->GetFirstAncestor(CHORD, MAX_CHORD_DEPTH));
    Ligature *ligatureParent = vrv_cast<Ligature *>(layerElement->GetFirstAncestor(LIGATURE, MAX_LIGATURE_DEPTH));
    Note *noteParent = vrv_cast<Note *>(layerElement->GetFirstAncestor(NOTE, MAX_NOTE_DEPTH));
    Rest *restParent = vrv_cast<Rest *>(layerElement->GetFirstAncestor(REST, MAX_NOTE_DEPTH));
    TabGrp *tabGrpParent = vrv_cast<TabGrp *>(layerElement->GetFirstAncestor(TABGRP, MAX_TABGRP_DEPTH));
    const bool ligatureAsBracket = m_doc->GetOptions()->m_ligatureAsBracket.GetValue();

    if (chordParent) {
        layerElement->SetAlignment(chordParent->GetAlignment());
    }
    else if (noteParent) {
        layerElement->SetAlignment(noteParent->GetAlignment());
    }
    else if (restParent) {
        layerElement->SetAlignment(restParent->GetAlignment());
    }
    else if (tabGrpParent) {
        layerElement->SetAlignment(tabGrpParent->GetAlignment());
    }
    else if (layerElement->Is({ DOTS, FLAG, STEM })) {
        assert(false);
    }
    else if (ligatureParent && layerElement->Is(NOTE) && !ligatureAsBracket) {
        // Ligature notes are all aligned with the first note
        Note *note = vrv_cast<Note *>(layerElement);
        assert(note);
        Note *firstNote = dynamic_cast<Note *>(ligatureParent->GetListFront(ligatureParent));
        if (firstNote && (firstNote != note)) {
            Alignment *alignment = firstNote->GetAlignment();
            layerElement->SetAlignment(alignment);
            alignment->AddLayerElementRef(layerElement);
            double duration
                = layerElement->GetAlignmentDuration(m_currentMensur, m_currentMeterSig, true, m_notationType);
            m_time += duration;
            return FUNCTOR_CONTINUE;
        }
    }
    // We do not align these (container). Any other?
    else if (layerElement->Is({ BEAM, LIGATURE, FTREM, TUPLET })) {
        double duration
            = layerElement->GetSameAsContentAlignmentDuration(m_currentMensur, m_currentMeterSig, true, m_notationType);
        m_time += duration;
        return FUNCTOR_CONTINUE;
    }
    else if (layerElement->Is(BARLINE)) {
        type = ALIGNMENT_BARLINE;
    }
    else if (layerElement->Is(CLEF)) {
        if ((layerElement->GetScoreDefRole() == SCOREDEF_SYSTEM)
            || (layerElement->GetScoreDefRole() == SCOREDEF_INTERMEDIATE))
            type = ALIGNMENT_SCOREDEF_CLEF;
        else if (layerElement->GetScoreDefRole() == SCOREDEF_CAUTIONARY)
            type = ALIGNMENT_SCOREDEF_CAUTION_CLEF;
        else {
            type = ALIGNMENT_CLEF;
        }
    }
    else if (layerElement->Is(KEYSIG)) {
        if ((layerElement->GetScoreDefRole() == SCOREDEF_SYSTEM)
            || (layerElement->GetScoreDefRole() == SCOREDEF_INTERMEDIATE))
            type = ALIGNMENT_SCOREDEF_KEYSIG;
        else if (layerElement->GetScoreDefRole() == SCOREDEF_CAUTIONARY)
            type = ALIGNMENT_SCOREDEF_CAUTION_KEYSIG;
        else {
            // type = ALIGNMENT_KEYSIG;
            // We force this because they should appear only at the beginning of a measure and should be non-justifiable
            // We also need it because the PAE importer creates keySig (and not staffDef @key.sig)
            type = ALIGNMENT_SCOREDEF_KEYSIG;
        }
    }
    else if (layerElement->Is(MENSUR)) {
        if ((layerElement->GetScoreDefRole() == SCOREDEF_SYSTEM)
            || (layerElement->GetScoreDefRole() == SCOREDEF_INTERMEDIATE))
            type = ALIGNMENT_SCOREDEF_MENSUR;
        else if (layerElement->GetScoreDefRole() == SCOREDEF_CAUTIONARY)
            type = ALIGNMENT_SCOREDEF_CAUTION_MENSUR;
        else {
            // replace the current mensur
            m_currentMensur = vrv_cast<Mensur *>(layerElement);
            assert(m_currentMensur);
            type = ALIGNMENT_MENSUR;
        }
    }
    else if (layerElement->Is(METERSIG)) {
        if ((layerElement->GetScoreDefRole() == SCOREDEF_SYSTEM)
            || (layerElement->GetScoreDefRole() == SCOREDEF_INTERMEDIATE))
            type = ALIGNMENT_SCOREDEF_METERSIG;
        else if (layerElement->GetScoreDefRole() == SCOREDEF_CAUTIONARY)
            type = ALIGNMENT_SCOREDEF_CAUTION_METERSIG;
        else if (layerElement->GetParent() && layerElement->GetParent()->Is(METERSIGGRP))
            type = ALIGNMENT_SCOREDEF_METERSIG;
        else {
            // replace the current meter signature
            m_currentMeterSig = vrv_cast<MeterSig *>(layerElement);
            assert(m_currentMeterSig);
            // type = ALIGNMENT_METERSIG
            // We force this because they should appear only at the beginning of a measure and should be non-justifiable
            // We also need it because the PAE importer creates meterSig (and not staffDef @meter)
            type = ALIGNMENT_SCOREDEF_METERSIG;
        }
    }
    else if (layerElement->Is({ MULTIREST, MREST, MRPT })) {
        type = ALIGNMENT_FULLMEASURE;
    }
    else if (layerElement->Is({ MRPT2, MULTIRPT })) {
        type = ALIGNMENT_FULLMEASURE2;
    }
    else if (layerElement->Is(DOT)) {
        Dot *dot = vrv_cast<Dot *>(layerElement);
        assert(dot);
        if (dot->m_drawingPreviousElement) {
            layerElement->SetAlignment(dot->m_drawingPreviousElement->GetAlignment());
        }
        else {
            // Create an alignment only if the dot has no resolved preceeding note
            type = ALIGNMENT_DOT;
        }
    }
    else if (layerElement->Is(ACCID)) {
        // accid within note was already taken into account by noteParent
        type = ALIGNMENT_ACCID;
    }
    else if (layerElement->Is(ARTIC)) {
        // Refer to the note parent
        Note *note = vrv_cast<Note *>(layerElement->GetFirstAncestor(NOTE));
        assert(note);
        layerElement->SetAlignment(note->GetAlignment());
    }
    else if (layerElement->Is(SYL)) {
        Staff *staff = layerElement->GetAncestorStaff();
        if (staff->m_drawingNotationType == NOTATIONTYPE_neume) {
            type = ALIGNMENT_DEFAULT;
        }
        else {
            Note *note = vrv_cast<Note *>(layerElement->GetFirstAncestor(NOTE));
            assert(note);
            layerElement->SetAlignment(note->GetAlignment());
        }
    }
    else if (layerElement->Is(VERSE)) {
        // Idem
        Note *note = vrv_cast<Note *>(layerElement->GetFirstAncestor(NOTE));
        assert(note);
        layerElement->SetAlignment(note->GetAlignment());
    }
    else if (layerElement->Is(GRACEGRP)) {
        return FUNCTOR_CONTINUE;
    }
    else if (layerElement->IsGraceNote()) {
        type = ALIGNMENT_GRACENOTE;
    }

    double duration = 0.0;
    // We have already an alignment with grace note children - skip this
    if (!layerElement->GetAlignment()) {
        // get the duration of the event
        duration = layerElement->GetAlignmentDuration(m_currentMensur, m_currentMeterSig, true, m_notationType);

        // For timestamp, what we get from GetAlignmentDuration is actually the position of the timestamp
        // So use it as current time - we can do this because the timestamp loop is redirected from the measure
        // The time will be reset to 0.0 when starting a new layer anyway
        if (layerElement->Is(TIMESTAMP_ATTR)) {
            m_time = duration;
        }
        else {
            m_measureAligner->SetMaxTime(m_time + duration);
        }

        layerElement->SetAlignment(m_measureAligner->GetAlignmentAtTime(m_time, type));
        assert(layerElement->GetAlignment());
    }

    Alignment *alignment = layerElement->GetAlignment();
    if (alignment->GetType() != ALIGNMENT_GRACENOTE) {
        if (alignment->AddLayerElementRef(layerElement)) m_hasMultipleLayer = true;
    }
    // For grace note aligner do not add them to the reference list because they will be processed by their original
    // hierarchy from the GraceAligner
    else {
        assert(layerElement->IsGraceNote());
        if (layerElement->Is(CHORD) || (layerElement->Is(NOTE) && !chordParent)) {
            Staff *staff = layerElement->GetAncestorStaff();
            int graceAlignerId = m_doc->GetOptions()->m_graceRhythmAlign.GetValue() ? 0 : staff->GetN();
            GraceAligner *graceAligner = alignment->GetGraceAligner(graceAlignerId);
            // We know that this is a note or a chord - we stack them and they will be added at the end of the layer
            // This will also see it for all their children
            graceAligner->StackGraceElement(layerElement);
        }
    }

    // LogDebug("Element %f %s", m_time, layerElement->GetClassName().c_str());

    if (!layerElement->Is(TIMESTAMP_ATTR)) {
        // increase the time position, but only when not a timestamp (it would actually do nothing)
        m_time += duration;
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode AlignHorizontallyFunctor::VisitMeasure(Measure *measure)
{
    // clear the content of the measureAligner
    MeasureAligner &measureAligner = measure->m_measureAligner;
    measureAligner.Reset();

    // point to it
    m_measureAligner = &measureAligner;
    m_hasMultipleLayer = false;

    if (measure->GetLeftBarLine()->SetAlignment(measureAligner.GetLeftBarLineAlignment())) m_hasMultipleLayer = true;
    if (measure->GetRightBarLine()->SetAlignment(measureAligner.GetRightBarLineAlignment())) m_hasMultipleLayer = true;

    assert(m_measureAligner);

    return FUNCTOR_CONTINUE;
}

FunctorCode AlignHorizontallyFunctor::VisitMeasureEnd(Measure *measure)
{
    int meterUnit = m_currentMeterSig ? m_currentMeterSig->GetUnit() : 4;
    measure->m_measureAligner.SetInitialTstamp(meterUnit);

    // We also need to align the timestamps - we do it at the end since we need the *meterSig to be initialized by a
    // Layer. Obviously this will not work with different time signature. However, I am not sure how this would work
    // in MEI anyway.
    measure->m_timestampAligner.Process(*this);

    // Next scoreDef will be INTERMEDIATE_SCOREDEF (See VisitLayer)
    m_isFirstMeasure = false;

    if (m_hasMultipleLayer) measure->HasAlignmentRefWithMultipleLayers(true);

    return FUNCTOR_CONTINUE;
}

FunctorCode AlignHorizontallyFunctor::VisitMeterSigGrp(MeterSigGrp *meterSigGrp)
{
    return meterSigGrp->IsScoreDefElement() ? FUNCTOR_STOP : FUNCTOR_CONTINUE;
}

FunctorCode AlignHorizontallyFunctor::VisitStaff(Staff *staff)
{
    StaffDef *drawingStaffDef = staff->m_drawingStaffDef;
    assert(drawingStaffDef);

    if (drawingStaffDef->HasNotationtype()) {
        m_notationType = drawingStaffDef->GetNotationtype();
    }
    else {
        m_notationType = NOTATIONTYPE_cmn;
    }
    Measure *parentMeasure = vrv_cast<Measure *>(staff->GetFirstAncestor(MEASURE));
    if (parentMeasure) drawingStaffDef->AlternateCurrentMeterSig(parentMeasure);

    return FUNCTOR_CONTINUE;
}

FunctorCode AlignHorizontallyFunctor::VisitSystem(System *system)
{
    // since we are starting a new system its first scoreDef will need to be a SYSTEM_SCOREDEF
    m_isFirstMeasure = true;

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// AlignMeasuresFunctor
//----------------------------------------------------------------------------

AlignMeasuresFunctor::AlignMeasuresFunctor(Doc *doc) : DocFunctor(doc)
{
    m_shift = 0;
    m_justifiableWidth = 0;
    m_applySectionRestartShift = false;
    m_storeCastOffSystemWidths = false;
}

FunctorCode AlignMeasuresFunctor::VisitMeasure(Measure *measure)
{
    if (m_applySectionRestartShift) {
        m_shift += measure->GetSectionRestartShift(m_doc);
        m_applySectionRestartShift = false;
    }

    measure->SetDrawingXRel(m_shift);

    m_shift += measure->GetWidth();
    m_justifiableWidth += measure->GetRightBarLineXRel() - measure->GetLeftBarLineXRel();

    return FUNCTOR_SIBLINGS;
}

FunctorCode AlignMeasuresFunctor::VisitScoreDef(ScoreDef *scoreDef)
{
    m_shift += scoreDef->GetDrawingLabelsWidth();

    if (m_applySectionRestartShift) {
        ClassIdsComparison comparison({ LABEL, LABELABBR });
        if (scoreDef->FindDescendantByComparison(&comparison)) {
            m_applySectionRestartShift = false;
        }
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode AlignMeasuresFunctor::VisitSection(Section *section)
{
    if (section->GetRestart() == BOOLEAN_true) {
        m_applySectionRestartShift = true;
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode AlignMeasuresFunctor::VisitSystem(System *system)
{
    system->SetDrawingXRel(system->m_systemLeftMar + system->GetDrawingLabelsWidth());
    m_shift = 0;
    m_justifiableWidth = 0;

    return FUNCTOR_CONTINUE;
}

FunctorCode AlignMeasuresFunctor::VisitSystemEnd(System *system)
{
    if (m_storeCastOffSystemWidths) {
        system->m_castOffTotalWidth = m_shift + system->GetDrawingLabelsWidth();
        system->m_castOffJustifiableWidth = m_justifiableWidth;
    }
    else {
        system->m_drawingTotalWidth = m_shift + system->GetDrawingLabelsWidth();
        system->m_drawingJustifiableWidth = m_justifiableWidth;
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

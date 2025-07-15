/////////////////////////////////////////////////////////////////////////////
// Name:        alignfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "alignfunctor.h"

//----------------------------------------------------------------------------

#include "div.h"
#include "doc.h"
#include "dot.h"
#include "fig.h"
#include "layer.h"
#include "ligature.h"
#include "nc.h"
#include "neume.h"
#include "page.h"
#include "proport.h"
#include "rend.h"
#include "rest.h"
#include "runningelement.h"
#include "section.h"
#include "staff.h"
#include "svg.h"
#include "syllable.h"
#include "system.h"
#include "tabgrp.h"
#include "verse.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AlignHorizontallyFunctor
//----------------------------------------------------------------------------

AlignHorizontallyFunctor::AlignHorizontallyFunctor(Doc *doc) : DocFunctor(doc)
{
    static const std::map<int, data_DURATION> durationEq{
        { DURATION_EQ_brevis, DURATION_brevis }, //
        { DURATION_EQ_semibrevis, DURATION_semibrevis }, //
        { DURATION_EQ_minima, DURATION_minima }, //
    };

    m_measureAligner = NULL;
    m_time = 0;
    m_notationType = NOTATIONTYPE_cmn;
    m_scoreDefRole = SCOREDEF_NONE;
    m_isFirstMeasure = false;
    m_hasMultipleLayer = false;
    m_currentParams.equivalence = durationEq.at(m_doc->GetOptions()->m_durationEquivalence.GetValue());
}

FunctorCode AlignHorizontallyFunctor::VisitLayer(Layer *layer)
{
    m_currentParams.mensur = layer->GetCurrentMensur();
    m_currentParams.meterSig = layer->GetCurrentMeterSig();
    m_currentParams.proport = layer->GetCurrentProport();

    // We are starting a new layer, reset the time;
    // We set it to -1.0 for the scoreDef attributes since they have to be aligned before any timestamp event (-1.0)
    m_time = -1;

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
        if (layer->GetStaffDefMeterSig()->GetVisible() != BOOLEAN_false) {
            this->VisitMeterSig(layer->GetStaffDefMeterSig());
        }
    }

    m_scoreDefRole = SCOREDEF_NONE;

    // Now we have to set it to 0.0 since we will start aligning musical content
    m_time = 0;

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
    const bool neumeAsNote = m_doc->GetOptions()->m_neumeAsNote.GetValue();

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
        Note *firstNote = dynamic_cast<Note *>(ligatureParent->GetListFront());
        if (firstNote && (firstNote != note)) {
            Alignment *alignment = firstNote->GetAlignment();
            layerElement->SetAlignment(alignment);
            alignment->AddLayerElementRef(layerElement);
            Fraction duration = layerElement->GetAlignmentDuration(m_currentParams, true, m_notationType);
            m_time = m_time + duration;
            return FUNCTOR_CONTINUE;
        }
    }
    // A ligature gets a default alignment in order to allow mensural cast-off
    else if (layerElement->Is(LIGATURE)) {
        // Nothing to do
    }
    // We do not align these (container). Any other?
    else if (layerElement->Is({ BEAM, FTREM, TUPLET })) {
        Fraction duration = layerElement->GetSameAsContentAlignmentDuration(m_currentParams, true, m_notationType);
        m_time = m_time + duration;
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
            type = ALIGNMENT_KEYSIG;
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
            m_currentParams.mensur = vrv_cast<Mensur *>(layerElement);
            assert(m_currentParams.mensur);
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
            m_currentParams.meterSig = vrv_cast<MeterSig *>(layerElement);
            assert(m_currentParams.meterSig);
            // type = ALIGNMENT_METERSIG
            // We force this because they should appear only at the beginning of a measure and should be non-justifiable
            // We also need it because the PAE importer creates meterSig (and not staffDef @meter)
            type = ALIGNMENT_SCOREDEF_METERSIG;
        }
    }
    else if (layerElement->Is(PROPORT)) {
        if (layerElement->GetType() == "cmme_tempo_change") return FUNCTOR_SIBLINGS;
        // replace the current proport
        const Proport *previous = (m_currentParams.proport) ? (m_currentParams.proport) : NULL;
        m_currentParams.proport = vrv_cast<Proport *>(layerElement);
        assert(m_currentParams.proport);
        if (previous) {
            m_currentParams.proport->Cumulate(previous);
        }
        type = ALIGNMENT_PROPORT;
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
    else if (layerElement->Is(CUSTOS)) {
        type = ALIGNMENT_CUSTOS;
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
        Note *note = vrv_cast<Note *>(layerElement->GetFirstAncestor(NOTE));
        if (note) {
            layerElement->SetAlignment(note->GetAlignment());
        }
        else {
            Syllable *syllable = vrv_cast<Syllable *>(layerElement->GetFirstAncestor(SYLLABLE));
            if (syllable) layerElement->SetAlignment(syllable->GetAlignment());
        }
        // Else add a default
    }
    else if (layerElement->Is(VERSE)) {
        // Idem
        Note *note = vrv_cast<Note *>(layerElement->GetFirstAncestor(NOTE));
        assert(note);
        layerElement->SetAlignment(note->GetAlignment());
    }
    else if (layerElement->Is(NC)) {
        // Align with the neume
        if (!neumeAsNote) {
            Neume *neume = vrv_cast<Neume *>(layerElement->GetFirstAncestor(NEUME));
            assert(neume);
            layerElement->SetAlignment(neume->GetAlignment());
        }
        // Otherwise each nc has its own aligner
    }
    else if (layerElement->Is(NEUME)) {
        // Align with the syllable
        if (neumeAsNote) {
            Syllable *syllable = vrv_cast<Syllable *>(layerElement->GetFirstAncestor(SYLLABLE));
            assert(syllable);
            layerElement->SetAlignment(syllable->GetAlignment());
            return FUNCTOR_CONTINUE;
        }
        // Otherwise each neume has its own aligner
    }
    else if (layerElement->Is(GRACEGRP)) {
        return FUNCTOR_CONTINUE;
    }
    else if (layerElement->IsGraceNote()) {
        type = ALIGNMENT_GRACENOTE;
    }

    Fraction duration;
    // We have already an alignment with grace note children - skip this
    if (!layerElement->GetAlignment()) {
        // get the duration of the event
        duration = layerElement->GetAlignmentDuration(m_currentParams, true, m_notationType);

        // For timestamp, what we get from GetAlignmentDuration is actually the position of the timestamp
        // So use it as current time - we can do this because the timestamp loop is redirected from the measure
        // The time will be reset to 0.0 when starting a new layer anyway
        if (layerElement->Is(TIMESTAMP_ATTR)) {
            m_time = duration;
            // When a tstamp is pointing to the end of a measure, then use the right barline alignment
            if (m_time == this->m_measureAligner->GetRightAlignment()->GetTime()) {
                type = ALIGNMENT_MEASURE_RIGHT_BARLINE;
            }
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
        m_time = m_time + duration;
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
    m_currentParams.metcon = (measure->GetMetcon() != BOOLEAN_false);

    if (measure->GetLeftBarLine()->SetAlignment(measureAligner.GetLeftBarLineAlignment())) m_hasMultipleLayer = true;
    if (measure->GetRightBarLine()->SetAlignment(measureAligner.GetRightBarLineAlignment())) m_hasMultipleLayer = true;

    assert(m_measureAligner);

    return FUNCTOR_CONTINUE;
}

FunctorCode AlignHorizontallyFunctor::VisitMeasureEnd(Measure *measure)
{
    data_DURATION meterUnit = (m_currentParams.meterSig && m_currentParams.meterSig->HasUnit())
        ? m_currentParams.meterSig->GetUnitAsDur()
        : DURATION_4;
    measure->m_measureAligner.SetInitialTstamp(meterUnit);

    // We also need to align the timestamps - we do it at the end since we need the *meterSig to be initialized by a
    // Layer. Obviously this will not work with different time signature. However, I am not sure how this would work
    // in MEI anyway.
    measure->m_timestampAligner.Process(*this);

    // Next scoreDef will be INTERMEDIATE_SCOREDEF (See VisitLayer)
    m_isFirstMeasure = false;

    if (m_hasMultipleLayer) measure->HasAlignmentRefWithMultipleLayers(true);

    // measure->m_measureAligner.LogDebugTree(3);

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

FunctorCode AlignMeasuresFunctor::VisitDiv(Div *div)
{
    if (div->GetDrawingInline()) div->SetDrawingXRel(m_shift);

    m_shift += div->GetContentWidth();

    return FUNCTOR_SIBLINGS;
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

//----------------------------------------------------------------------------
// AlignVerticallyFunctor
//----------------------------------------------------------------------------

AlignVerticallyFunctor::AlignVerticallyFunctor(Doc *doc) : DocFunctor(doc)
{
    m_systemAligner = NULL;
    m_staffIdx = 0;
    m_staffN = 0;
    m_cumulatedShift = 0;
    m_justificationSum = 0;
    m_pageWidth = 0;
}

FunctorCode AlignVerticallyFunctor::VisitDiv(Div *div)
{
    m_systemAligner->GetBottomAlignment()->SetYRel(-div->GetTotalHeight(m_doc));

    m_pageWidth = div->GetTotalWidth(m_doc);

    return FUNCTOR_CONTINUE;
}

FunctorCode AlignVerticallyFunctor::VisitFig(Fig *fig)
{
    Svg *svg = vrv_cast<Svg *>(fig->FindDescendantByType(SVG));
    int width = svg ? svg->GetWidth() : 0;

    if (fig->GetHalign() == HORIZONTALALIGNMENT_right) {
        fig->SetDrawingXRel(m_pageWidth - width);
    }
    else if (fig->GetHalign() == HORIZONTALALIGNMENT_center) {
        fig->SetDrawingXRel((m_pageWidth - width) / 2);
    }

    return FUNCTOR_SIBLINGS;
}

FunctorCode AlignVerticallyFunctor::VisitMeasure(Measure *measure)
{
    // we also need to reset the staff index
    m_staffIdx = 0;

    return FUNCTOR_CONTINUE;
}

FunctorCode AlignVerticallyFunctor::VisitPageEnd(Page *page)
{
    m_cumulatedShift = 0;

    // Also align the header and footer
    RunningElement *header = page->GetHeader();
    if (header) {
        header->SetDrawingPage(page);
        header->SetDrawingYRel(0);
        header->Process(*this);
    }
    RunningElement *footer = page->GetFooter();
    if (footer) {
        footer->SetDrawingPage(page);
        footer->SetDrawingYRel(0);
        footer->Process(*this);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode AlignVerticallyFunctor::VisitRend(Rend *rend)
{
    if (!rend->GetFirstAncestorInRange(TEXT_LAYOUT_ELEMENT, TEXT_LAYOUT_ELEMENT_max)) return FUNCTOR_SIBLINGS;

    if (rend->GetHalign()) {
        switch (rend->GetHalign()) {
            case HORIZONTALALIGNMENT_right: rend->SetDrawingXRel(m_pageWidth); break;
            case HORIZONTALALIGNMENT_center: rend->SetDrawingXRel(m_pageWidth / 2); break;
            default: break;
        }
    }

    return FUNCTOR_SIBLINGS;
}

FunctorCode AlignVerticallyFunctor::VisitRunningElement(RunningElement *runningElement)
{
    this->VisitTextLayoutElement(runningElement);

    m_pageWidth = runningElement->GetTotalWidth(m_doc);

    return FUNCTOR_CONTINUE;
}

FunctorCode AlignVerticallyFunctor::VisitStaff(Staff *staff)
{
    if (!staff->DrawingIsVisible()) {
        return FUNCTOR_SIBLINGS;
    }

    m_staffN = staff->GetN();

    // this gets (or creates) the staff alignment
    StaffAlignment *alignment = m_systemAligner->GetStaffAlignment(m_staffIdx, staff, m_doc);
    assert(alignment);
    staff->SetAlignment(alignment);

    std::vector<Object *>::const_iterator verseIterator = std::find_if(
        staff->m_timeSpanningElements.begin(), staff->m_timeSpanningElements.end(), ObjectComparison(VERSE));
    if (verseIterator != staff->m_timeSpanningElements.end()) {
        Verse *verse = vrv_cast<Verse *>(*verseIterator);
        assert(verse);
        alignment->AddVerseN(verse->GetN(), verse->GetPlace());
    }

    // add verse number to alignment in case there are spanning SYL elements but there is no verse number already - this
    // generally happens with verses spanning over several systems which results in invalid placement of connector lines
    std::vector<Object *>::const_iterator sylIterator = std::find_if(
        staff->m_timeSpanningElements.begin(), staff->m_timeSpanningElements.end(), ObjectComparison(SYL));
    if (sylIterator != staff->m_timeSpanningElements.end()) {
        Verse *verse = vrv_cast<Verse *>((*sylIterator)->GetFirstAncestor(VERSE));
        if (verse) {
            const int verseNumber = verse->GetN();
            const data_STAFFREL versePlace = verse->GetPlace();
            const bool verseCollapse = m_doc->GetOptions()->m_lyricVerseCollapse.GetValue();
            if ((versePlace == STAFFREL_above) && !alignment->GetVersePositionAbove(verseNumber, verseCollapse)) {
                alignment->AddVerseN(verseNumber, verse->GetPlace());
            }
            if ((versePlace != STAFFREL_above) && !alignment->GetVersePositionBelow(verseNumber, verseCollapse)) {
                alignment->AddVerseN(verseNumber, verse->GetPlace());
            }
        }
    }

    // for next staff
    ++m_staffIdx;

    return FUNCTOR_CONTINUE;
}

FunctorCode AlignVerticallyFunctor::VisitStaffAlignmentEnd(StaffAlignment *staffAlignment)
{
    m_cumulatedShift += staffAlignment->GetMinimumSpacing(m_doc);

    staffAlignment->SetYRel(-m_cumulatedShift);

    m_cumulatedShift += staffAlignment->GetStaffHeight();
    ++m_staffIdx;

    return FUNCTOR_CONTINUE;
}

FunctorCode AlignVerticallyFunctor::VisitSyllable(Syllable *syllable)
{
    if (!syllable->FindDescendantByType(SYL)) return FUNCTOR_CONTINUE;

    StaffAlignment *alignment = m_systemAligner->GetStaffAlignmentForStaffN(m_staffN);
    if (!alignment) return FUNCTOR_CONTINUE;
    // Current limitation of only one syl (verse n) by syllable
    alignment->AddVerseN(1, STAFFREL_below);

    return FUNCTOR_CONTINUE;
}

FunctorCode AlignVerticallyFunctor::VisitSystem(System *system)
{
    m_systemAligner = &system->m_systemAligner;

    return FUNCTOR_CONTINUE;
}

FunctorCode AlignVerticallyFunctor::VisitSystemEnd(System *system)
{
    m_cumulatedShift = 0;
    m_staffIdx = 0;

    // StaffAlignment are added following the staff element in the measures
    // We can now reorder them according to the scoreDef order
    if (system->GetDrawingScoreDef()) {
        system->m_systemAligner.ReorderBy(system->GetDrawingScoreDef()->GetStaffNs());
    }

    system->m_systemAligner.Process(*this);

    return FUNCTOR_SIBLINGS;
}

FunctorCode AlignVerticallyFunctor::VisitVerse(Verse *verse)
{
    StaffAlignment *alignment = m_systemAligner->GetStaffAlignmentForStaffN(m_staffN);

    if (!alignment) return FUNCTOR_CONTINUE;

    // Add the number count
    alignment->AddVerseN(verse->GetN(), verse->GetPlace());

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// AlignSystemsFunctor
//----------------------------------------------------------------------------

AlignSystemsFunctor::AlignSystemsFunctor(Doc *doc) : DocFunctor(doc)
{
    m_shift = 0;
    m_systemSpacing = 0;
    m_prevBottomOverflow = 0;
    m_prevBottomClefOverflow = 0;
    m_justificationSum = 0.0;
}

FunctorCode AlignSystemsFunctor::VisitPage(Page *page)
{
    m_justificationSum = 0;

    RunningElement *header = page->GetHeader();
    if (header) {
        header->SetDrawingYRel(m_shift);
        const int headerHeight = header->GetTotalHeight(m_doc);
        if (headerHeight > 0) {
            m_shift -= headerHeight;
        }
    }
    return FUNCTOR_CONTINUE;
}

FunctorCode AlignSystemsFunctor::VisitPageEnd(Page *page)
{
    page->m_drawingJustifiableHeight = m_shift;
    page->m_justificationSum = m_justificationSum;

    RunningElement *footer = page->GetFooter();
    if (footer) {
        page->m_drawingJustifiableHeight -= footer->GetTotalHeight(m_doc);

        // Move it up below the last system
        if (m_doc->GetOptions()->m_adjustPageHeight.GetValue()) {
            if (page->GetChildCount()) {
                System *last = vrv_cast<System *>(page->GetLast(SYSTEM));
                assert(last);
                const int unit = m_doc->GetDrawingUnit(100);
                const int topMargin = m_doc->GetOptions()->m_topMarginPgFooter.GetValue() * unit;
                footer->SetDrawingYRel(last->GetDrawingYRel() - last->GetHeight() - topMargin);
            }
        }
        else {
            footer->SetDrawingYRel(footer->GetContentHeight());
        }
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode AlignSystemsFunctor::VisitSystem(System *system)
{
    SystemAligner &systemAligner = system->m_systemAligner;
    assert(systemAligner.GetBottomAlignment());

    // No spacing for the first system
    if (!system->IsFirstInPage()) {
        // const int contentOverflow = m_prevBottomOverflow + systemAligner.GetOverflowAbove(m_doc);
        // const int clefOverflow = m_prevBottomClefOverflow + systemAligner.GetOverflowAbove(m_doc, true);
        const int unit = m_doc->GetDrawingUnit(100);
        m_shift -= std::max(m_systemSpacing, 2 * unit);
    }

    system->SetDrawingYRel(m_shift);

    m_shift += systemAligner.GetBottomAlignment()->GetYRel();

    m_justificationSum += systemAligner.GetJustificationSum(m_doc);
    if (system->IsFirstInPage()) {
        // remove extra system justification factor to get exactly (systemsCount-1)*justificationSystem
        m_justificationSum -= m_doc->GetOptions()->m_justificationSystem.GetValue();
    }

    // These are currently not used
    m_prevBottomOverflow = systemAligner.GetOverflowBelow(m_doc);
    m_prevBottomClefOverflow = systemAligner.GetOverflowBelow(m_doc, true);

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv

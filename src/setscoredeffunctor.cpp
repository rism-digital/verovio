/////////////////////////////////////////////////////////////////////////////
// Name:        setscoredeffunctor.cpp
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "setscoredeffunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "layer.h"
#include "page.h"
#include "pages.h"
#include "score.h"
#include "staff.h"
#include "system.h"
#include "tuning.h"
#include "vrv.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// ReplaceDrawingValuesInStaffDefFunctor
//----------------------------------------------------------------------------

ReplaceDrawingValuesInStaffDefFunctor::ReplaceDrawingValuesInStaffDefFunctor(const Clef *clef, const KeySig *keySig,
    const Mensur *mensur, const MeterSig *meterSig, const MeterSigGrp *meterSigGrp)
    : Functor()
{
    m_clef = clef;
    m_keySig = keySig;
    m_mensur = mensur;
    m_meterSig = meterSig;
    m_meterSigGrp = meterSigGrp;
}

FunctorCode ReplaceDrawingValuesInStaffDefFunctor::VisitStaffDef(StaffDef *staffDef)
{
    if (m_clef) {
        staffDef->SetCurrentClef(m_clef);
    }
    if (m_keySig) {
        staffDef->SetCurrentKeySig(m_keySig);
    }
    if (m_mensur) {
        staffDef->SetCurrentMensur(m_mensur);
    }
    if (m_meterSig) {
        staffDef->SetCurrentMeterSig(m_meterSig);
    }
    if (m_meterSigGrp) {
        staffDef->SetCurrentMeterSigGrp(m_meterSigGrp);
    }

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// ScoreDefSetCurrentPageFunctor
//----------------------------------------------------------------------------

ScoreDefSetCurrentPageFunctor::ScoreDefSetCurrentPageFunctor(Doc *doc) : DocFunctor(doc) {}

FunctorCode ScoreDefSetCurrentPageFunctor::VisitPageEnd(Page *page)
{
    const Object *firstSystem = page->GetFirst(SYSTEM);
    const Object *reference = firstSystem ? firstSystem : page;
    page->m_score = m_doc->GetCorrespondingScore(reference, m_scores);

    const Object *lastSystem = page->GetLast(SYSTEM);
    reference = lastSystem ? lastSystem : page;
    page->m_scoreEnd = m_doc->GetCorrespondingScore(reference, m_scores);

    return FUNCTOR_CONTINUE;
}

FunctorCode ScoreDefSetCurrentPageFunctor::VisitScore(Score *score)
{
    m_scores.push_back(score);

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// ScoreDefSetCurrentFunctor
//----------------------------------------------------------------------------

ScoreDefSetCurrentFunctor::ScoreDefSetCurrentFunctor(Doc *doc) : DocFunctor(doc)
{
    m_currentScore = NULL;
    m_currentScoreDef = NULL;
    m_currentStaffDef = NULL;
    m_upcomingScoreDef.Reset();
    m_previousMeasure = NULL;
    m_currentSystem = NULL;
    m_drawLabels = false;
    m_restart = false;
    m_hasMeasure = false;
}

FunctorCode ScoreDefSetCurrentFunctor::VisitClef(Clef *clef)
{
    LayerElement *elementOrLink = clef->ThisOrSameasLink();
    if (!elementOrLink || !elementOrLink->Is(CLEF)) return FUNCTOR_CONTINUE;
    clef = vrv_cast<Clef *>(elementOrLink);
    if (clef->IsScoreDefElement()) {
        return FUNCTOR_CONTINUE;
    }
    assert(m_currentStaffDef);
    const int n = clef->m_crossStaff ? clef->m_crossStaff->GetN() : m_currentStaffDef->GetN();
    StaffDef *upcomingStaffDef = m_upcomingScoreDef.GetStaffDef(n);
    assert(upcomingStaffDef);
    upcomingStaffDef->SetCurrentClef(clef);
    m_upcomingScoreDef.m_setAsDrawing = true;
    return FUNCTOR_CONTINUE;
}

FunctorCode ScoreDefSetCurrentFunctor::VisitKeySig(KeySig *keySig)
{
    if (keySig->IsScoreDefElement()) {
        return FUNCTOR_CONTINUE;
    }
    assert(m_currentStaffDef);
    StaffDef *upcomingStaffDef = m_upcomingScoreDef.GetStaffDef(m_currentStaffDef->GetN());
    assert(upcomingStaffDef);
    upcomingStaffDef->SetCurrentKeySig(keySig);
    m_upcomingScoreDef.m_setAsDrawing = true;
    return FUNCTOR_CONTINUE;
}

FunctorCode ScoreDefSetCurrentFunctor::VisitLayer(Layer *layer)
{
    if (m_doc->GetType() != Transcription) layer->SetDrawingStaffDefValues(m_currentStaffDef);
    return FUNCTOR_CONTINUE;
}

FunctorCode ScoreDefSetCurrentFunctor::VisitMeasure(Measure *measure)
{
    // If we have a restart scoreDef before, for redrawing of everything on the measure
    if (m_restart) {
        m_upcomingScoreDef.SetRedrawFlags(StaffDefRedrawFlags::REDRAW_ALL);
    }

    int drawingFlags = 0;
    // This is the first measure of the system - more to do...
    if (m_currentSystem) {
        drawingFlags |= Measure::BarlineDrawingFlags::SYSTEM_BREAK;
        // We had a scoreDef so we need to put cautionary values
        // This will also happen with clef in the last measure - however, the cautionary functor will not do
        // anything then
        // The cautionary scoreDef for restart is already done when hitting the scoreDef
        if (m_upcomingScoreDef.m_setAsDrawing && m_previousMeasure && !m_restart) {
            ScoreDef cautionaryScoreDef = m_upcomingScoreDef;
            SetCautionaryScoreDefFunctor setCautionaryScoreDef(&cautionaryScoreDef);
            m_previousMeasure->Process(setCautionaryScoreDef);
        }
        // Set the flags we want to have. This also sets m_setAsDrawing to true so the next measure will keep it
        m_upcomingScoreDef.SetRedrawFlags(StaffDefRedrawFlags::REDRAW_CLEF | StaffDefRedrawFlags::REDRAW_KEYSIG);
        // Set it to the current system (used e.g. for endings)
        m_currentSystem->SetDrawingScoreDef(&m_upcomingScoreDef);
        m_currentSystem->GetDrawingScoreDef()->SetDrawLabels(m_drawLabels);
        m_currentSystem = NULL;
        m_drawLabels = false;
    }
    if (m_upcomingScoreDef.m_setAsDrawing) {
        measure->SetDrawingScoreDef(&m_upcomingScoreDef);
        m_currentScoreDef = measure->GetDrawingScoreDef();
        m_upcomingScoreDef.SetRedrawFlags(StaffDefRedrawFlags::FORCE_REDRAW);
        m_upcomingScoreDef.m_setAsDrawing = false;
    }
    m_drawLabels = false;

    // set other flags based on score def change
    if (m_upcomingScoreDef.m_insertScoreDef) {
        drawingFlags |= Measure::BarlineDrawingFlags::SCORE_DEF_INSERT;
        m_upcomingScoreDef.m_insertScoreDef = false;
    }

    // check if we need to draw barlines for current/previous measures (in cases when all staves are invisible in
    // them)
    ListOfObjects currentObjects, previousObjects;
    AttVisibilityComparison comparison(STAFF, BOOLEAN_false);
    measure->FindAllDescendantsByComparison(&currentObjects, &comparison);
    if ((int)currentObjects.size() == measure->GetChildCount(STAFF)) {
        drawingFlags |= Measure::BarlineDrawingFlags::INVISIBLE_MEASURE_CURRENT;
    }
    if (m_previousMeasure) {
        m_previousMeasure->FindAllDescendantsByComparison(&previousObjects, &comparison);
        if ((int)previousObjects.size() == m_previousMeasure->GetChildCount(STAFF))
            drawingFlags |= Measure::BarlineDrawingFlags::INVISIBLE_MEASURE_PREVIOUS;
    }

    measure->SetInvisibleStaffBarlines(m_previousMeasure, currentObjects, previousObjects, drawingFlags);
    measure->SetDrawingBarLines(m_previousMeasure, drawingFlags);

    m_previousMeasure = measure;
    m_restart = false;
    m_hasMeasure = true;

    return FUNCTOR_CONTINUE;
}

FunctorCode ScoreDefSetCurrentFunctor::VisitMensur(Mensur *mensur)
{
    if (mensur->IsScoreDefElement()) {
        return FUNCTOR_CONTINUE;
    }
    assert(m_currentStaffDef);
    StaffDef *upcomingStaffDef = m_upcomingScoreDef.GetStaffDef(m_currentStaffDef->GetN());
    assert(upcomingStaffDef);
    upcomingStaffDef->SetCurrentMensur(mensur);
    m_upcomingScoreDef.m_setAsDrawing = true;
    return FUNCTOR_CONTINUE;
}

FunctorCode ScoreDefSetCurrentFunctor::VisitPage(Page *page)
{
    // This will be reached before we reach the beginning of a first Score.
    // However, page->m_score has already been set by ScoreDefSetCurrentPageFunctor
    // This must be the first page or a new score is starting on this page
    assert(page->m_score);
    assert(page->m_score->GetScoreDef());
    if (!m_currentScore || (m_currentScore != page->m_score)) {
        m_upcomingScoreDef = *page->m_score->GetScoreDef();
        m_upcomingScoreDef.Process(*this);
    }
    page->m_drawingScoreDef = m_upcomingScoreDef;
    return FUNCTOR_CONTINUE;
}

FunctorCode ScoreDefSetCurrentFunctor::VisitProport(Proport *proport)
{
    if (proport->GetType() == "cmme_tempo_change") return FUNCTOR_SIBLINGS;
    assert(m_currentStaffDef);
    StaffDef *upcomingStaffDef = m_upcomingScoreDef.GetStaffDef(m_currentStaffDef->GetN());
    assert(upcomingStaffDef);
    upcomingStaffDef->SetCurrentProport(proport);
    return FUNCTOR_CONTINUE;
}

FunctorCode ScoreDefSetCurrentFunctor::VisitScore(Score *score)
{
    m_currentScore = score;
    m_upcomingScoreDef = *score->GetScoreDef();
    m_upcomingScoreDef.Process(*this);
    // Trigger the redraw of everything
    m_upcomingScoreDef.SetRedrawFlags(StaffDefRedrawFlags::REDRAW_ALL);
    m_drawLabels = true;
    m_currentScoreDef = NULL;
    m_currentStaffDef = NULL;
    m_previousMeasure = NULL;
    m_currentSystem = NULL;
    m_restart = false;
    m_hasMeasure = false;
    return FUNCTOR_CONTINUE;
}

FunctorCode ScoreDefSetCurrentFunctor::VisitScoreDef(ScoreDef *scoreDef)
{
    // Replace the current scoreDef with the new one, including its content (staffDef) - this also sets
    // m_setAsDrawing to true so it will then be taken into account at the next measure
    if (scoreDef->HasClefInfo(UNLIMITED_DEPTH) || scoreDef->HasKeySigInfo(UNLIMITED_DEPTH)
        || scoreDef->HasMensurInfo(UNLIMITED_DEPTH) || scoreDef->HasMeterSigGrpInfo(UNLIMITED_DEPTH)
        || scoreDef->HasMeterSigInfo(UNLIMITED_DEPTH)) {
        m_upcomingScoreDef.ReplaceDrawingValues(scoreDef);
        m_upcomingScoreDef.m_insertScoreDef = true;
    }
    if (scoreDef->IsSectionRestart()) {
        m_drawLabels = true;
        m_restart = true;
        // Redraw the labels only if we already have a measure in the system. Otherwise this will be
        // done through the system scoreDef
        scoreDef->SetDrawLabels(m_hasMeasure);
        // If we have a previous measure, we need to set the cautionary scoreDef independently from the
        // presence of a system break
        if (m_previousMeasure) {
            ScoreDef cautionaryScoreDef = m_upcomingScoreDef;
            SetCautionaryScoreDefFunctor setCautionaryScoreDef(&cautionaryScoreDef);
            m_previousMeasure->Process(setCautionaryScoreDef);
        }
    }
    return FUNCTOR_CONTINUE;
}

FunctorCode ScoreDefSetCurrentFunctor::VisitStaff(Staff *staff)
{
    m_currentStaffDef = m_currentScoreDef->GetStaffDef(staff->GetN());
    assert(staff->m_drawingStaffDef == NULL);
    staff->m_drawingStaffDef = m_currentStaffDef;
    assert(staff->m_drawingTuning == NULL);
    staff->m_drawingTuning = vrv_cast<Tuning *>(m_currentStaffDef->FindDescendantByType(TUNING));
    staff->m_drawingLines = m_currentStaffDef->GetLines();
    staff->m_drawingNotationType = m_currentStaffDef->GetNotationtype();
    staff->m_drawingStaffSize = 100;
    if (m_currentStaffDef->HasScale()) {
        staff->m_drawingStaffSize = m_currentStaffDef->GetScale();
    }
    if (staff->IsTablature()) {
        staff->m_drawingStaffSize *= TABLATURE_STAFF_RATIO;
    }
    if (MeterSigGrp *metersiggrp = m_currentStaffDef->GetCurrentMeterSigGrp();
        metersiggrp->GetFunc() == meterSigGrpLog_FUNC_alternating) {
        Measure *parentMeasure = vrv_cast<Measure *>(staff->GetFirstAncestor(MEASURE));
        if (parentMeasure) metersiggrp->AddAlternatingMeasureToVector(parentMeasure);
    }
    return FUNCTOR_CONTINUE;
}

FunctorCode ScoreDefSetCurrentFunctor::VisitStaffDef(StaffDef *staffDef)
{
    m_upcomingScoreDef.ReplaceDrawingValues(staffDef);
    return FUNCTOR_CONTINUE;
}

FunctorCode ScoreDefSetCurrentFunctor::VisitStaffGrp(StaffGrp *staffGrp)
{
    // For now replace labels only if we have a section@restart
    if (m_restart) {
        m_upcomingScoreDef.ReplaceDrawingLabels(staffGrp);
    }
    return FUNCTOR_CONTINUE;
}

FunctorCode ScoreDefSetCurrentFunctor::VisitSystem(System *system)
{
    // This is the only thing we do for now - we need to wait until we reach the first measure
    m_currentSystem = system;
    m_hasMeasure = false;
    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// ScoreDefOptimizeFunctor
//----------------------------------------------------------------------------

ScoreDefOptimizeFunctor::ScoreDefOptimizeFunctor(Doc *doc) : DocFunctor(doc)
{
    m_currentScoreDef = NULL;
    m_encoded = false;
    m_firstScoreDef = true;
    m_hasFermata = false;
    m_hasTempo = false;
}

FunctorCode ScoreDefOptimizeFunctor::VisitMeasure(Measure *measure)
{
    if (!m_doc->GetOptions()->m_condenseTempoPages.GetValue()) {
        return FUNCTOR_CONTINUE;
    }

    m_hasFermata = (measure->FindDescendantByType(FERMATA));
    m_hasTempo = (measure->FindDescendantByType(TEMPO));

    return FUNCTOR_CONTINUE;
}

FunctorCode ScoreDefOptimizeFunctor::VisitScore(Score *score)
{
    m_currentScoreDef = NULL;
    m_encoded = false;
    m_firstScoreDef = true;
    m_hasFermata = false;
    m_hasTempo = false;

    return FUNCTOR_CONTINUE;
}

FunctorCode ScoreDefOptimizeFunctor::VisitStaff(Staff *staff)
{
    assert(m_currentScoreDef);
    StaffDef *staffDef = m_currentScoreDef->GetStaffDef(staff->GetN());

    if (!staffDef) {
        LogDebug("Could not find staffDef for staff (%d) when optimizing scoreDef", staff->GetN());
        return FUNCTOR_SIBLINGS;
    }

    // Always show staves with a clef change
    if (staff->FindDescendantByType(CLEF)) {
        staffDef->SetDrawingVisibility(OPTIMIZATION_SHOW);
    }

    // Always show all staves when there is a fermata or a tempo
    // (without checking if the fermata is actually on that staff)
    if (m_hasFermata || m_hasTempo) {
        staffDef->SetDrawingVisibility(OPTIMIZATION_SHOW);
    }

    if (staffDef->GetDrawingVisibility() == OPTIMIZATION_SHOW) {
        return FUNCTOR_SIBLINGS;
    }

    staffDef->SetDrawingVisibility(OPTIMIZATION_HIDDEN);

    // Ignore layers that are empty (or with @sameas)
    ListOfObjects layers;
    IsEmptyComparison matchTypeLayer(LAYER);
    matchTypeLayer.ReverseComparison();
    staff->FindAllDescendantsByComparison(&layers, &matchTypeLayer);

    Object *note = staff->FindDescendantByType(NOTE);

    // Show the staff only if there are any notes
    if (note) {
        staffDef->SetDrawingVisibility(OPTIMIZATION_SHOW);
    }

    return FUNCTOR_SIBLINGS;
}

FunctorCode ScoreDefOptimizeFunctor::VisitStaffGrpEnd(StaffGrp *staffGrp)
{
    staffGrp->SetDrawingVisibility(OPTIMIZATION_HIDDEN);

    const Object *instrDef = staffGrp->FindDescendantByType(INSTRDEF, 1);
    if (instrDef) {
        VisibleStaffDefOrGrpObject visibleStaves;
        const Object *firstVisible = staffGrp->FindDescendantByComparison(&visibleStaves, 1);
        if (firstVisible) {
            staffGrp->SetEverythingVisible();
        }

        return FUNCTOR_CONTINUE;
    }

    for (auto child : staffGrp->GetChildren()) {
        if (child->Is(STAFFDEF)) {
            StaffDef *staffDef = vrv_cast<StaffDef *>(child);
            assert(staffDef);
            if (staffDef->GetDrawingVisibility() != OPTIMIZATION_HIDDEN) {
                staffGrp->SetDrawingVisibility(OPTIMIZATION_SHOW);
                break;
            }
        }
        else if (child->Is(STAFFGRP)) {
            StaffGrp *staffGrpChild = vrv_cast<StaffGrp *>(child);
            assert(staffGrpChild);
            if (staffGrpChild->GetDrawingVisibility() != OPTIMIZATION_HIDDEN) {
                staffGrp->SetDrawingVisibility(OPTIMIZATION_SHOW);
                break;
            }
        }
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode ScoreDefOptimizeFunctor::VisitSystem(System *system)
{
    system->IsDrawingOptimized(true);

    if (m_firstScoreDef) {
        m_firstScoreDef = false;
        if (!m_doc->GetOptions()->m_condenseFirstPage.GetValue()) {
            return FUNCTOR_SIBLINGS;
        }
    }

    if (system->IsLastOfMdiv()) {
        if (m_doc->GetOptions()->m_condenseNotLastSystem.GetValue()) {
            return FUNCTOR_SIBLINGS;
        }
    }

    m_currentScoreDef = system->GetDrawingScoreDef();
    assert(m_currentScoreDef);

    return FUNCTOR_CONTINUE;
}

FunctorCode ScoreDefOptimizeFunctor::VisitSystemEnd(System *system)
{
    m_currentScoreDef->Process(*this);
    system->m_systemAligner.SetSpacing(m_currentScoreDef);

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// SetCautionaryScoreDefFunctor
//----------------------------------------------------------------------------

SetCautionaryScoreDefFunctor::SetCautionaryScoreDefFunctor(ScoreDef *currentScoreDef) : Functor()
{
    m_currentScoreDef = currentScoreDef;
    m_currentStaffDef = NULL;
}

FunctorCode SetCautionaryScoreDefFunctor::VisitLayer(Layer *layer)
{
    layer->SetDrawingCautionValues(m_currentStaffDef);
    return FUNCTOR_SIBLINGS;
}

FunctorCode SetCautionaryScoreDefFunctor::VisitStaff(Staff *staff)
{
    assert(m_currentScoreDef);
    m_currentStaffDef = m_currentScoreDef->GetStaffDef(staff->GetN());
    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// ScoreDefSetGrpSymFunctor
//----------------------------------------------------------------------------

ScoreDefSetGrpSymFunctor::ScoreDefSetGrpSymFunctor() : Functor() {}

FunctorCode ScoreDefSetGrpSymFunctor::VisitGrpSym(GrpSym *grpSym)
{
    // For the grpSym that is encoded in the scope of the staffGrp just get first and last staffDefs and set then as
    // starting and ending points
    if (grpSym->GetParent()->Is(STAFFGRP)) {
        StaffGrp *staffGrp = vrv_cast<StaffGrp *>(grpSym->GetParent());
        assert(staffGrp);
        auto [firstDef, lastDef] = staffGrp->GetFirstLastStaffDef();
        if (firstDef && lastDef) {
            grpSym->SetStartDef(firstDef);
            grpSym->SetEndDef(lastDef);
            staffGrp->SetGroupSymbol(grpSym);
        }
    }
    // For the grpSym that is encoded in the scope of the scoreDef we need to find corresponding staffDefs with matching
    // @startid and @endid. We also need to make sure that @level attribute is adhered to, hence we limit search depth.
    // Finally, we need to make sure that both starting and ending elements have the same parent (since we cannot draw
    // cross-group grpSym)
    else if (grpSym->GetParent()->Is(SCOREDEF)) {
        ScoreDef *scoreDef = vrv_cast<ScoreDef *>(grpSym->GetParent());
        assert(scoreDef);

        const std::string startId = ExtractIDFragment(grpSym->GetStartid());
        const std::string endId = ExtractIDFragment(grpSym->GetEndid());
        const int level = grpSym->GetLevel();

        IDComparison compare(STAFFDEF, startId);
        StaffDef *start = vrv_cast<StaffDef *>(scoreDef->FindDescendantByComparison(&compare, level));
        compare.SetID(endId);
        StaffDef *end = vrv_cast<StaffDef *>(scoreDef->FindDescendantByComparison(&compare, level));

        if (!start || !end) {
            LogWarning("Could not find startid/endid on level %d for '%s'", level, grpSym->GetID().c_str());
            return FUNCTOR_CONTINUE;
        }

        if (start->GetParent() != end->GetParent()) {
            LogWarning("'%s' has mismatching parents for startid:%s and endid:%s", grpSym->GetID().c_str(),
                startId.c_str(), endId.c_str());
            return FUNCTOR_CONTINUE;
        }

        grpSym->SetStartDef(start);
        grpSym->SetEndDef(end);
        // dynamic_cast because we never check parent type
        StaffGrp *staffGrp = dynamic_cast<StaffGrp *>(start->GetParent());
        assert(staffGrp);
        staffGrp->SetGroupSymbol(grpSym);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode ScoreDefSetGrpSymFunctor::VisitSystem(System *system)
{
    ScoreDef *drawingScoreDef = system->GetDrawingScoreDef();
    if (drawingScoreDef) drawingScoreDef->Process(*this);

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// ScoreDefUnsetCurrentFunctor
//----------------------------------------------------------------------------

ScoreDefUnsetCurrentFunctor::ScoreDefUnsetCurrentFunctor() : Functor() {}

FunctorCode ScoreDefUnsetCurrentFunctor::VisitAlignmentReference(AlignmentReference *alignmentReference)
{
    Alignment *alignment = vrv_cast<Alignment *>(alignmentReference->GetParent());
    assert(alignment);

    switch (alignment->GetType()) {
        case ALIGNMENT_SCOREDEF_CLEF:
        case ALIGNMENT_SCOREDEF_KEYSIG:
        case ALIGNMENT_SCOREDEF_MENSUR:
        case ALIGNMENT_SCOREDEF_METERSIG:
        case ALIGNMENT_SCOREDEF_CAUTION_CLEF:
        case ALIGNMENT_SCOREDEF_CAUTION_KEYSIG:
        case ALIGNMENT_SCOREDEF_CAUTION_MENSUR:
        case ALIGNMENT_SCOREDEF_CAUTION_METERSIG: alignmentReference->ClearChildren(); break;
        default: break;
    }

    return FUNCTOR_SIBLINGS;
}

FunctorCode ScoreDefUnsetCurrentFunctor::VisitKeySig(KeySig *keySig)
{
    keySig->ResetDrawingClef();

    return FUNCTOR_CONTINUE;
}

FunctorCode ScoreDefUnsetCurrentFunctor::VisitLayer(Layer *layer)
{
    layer->ResetStaffDefObjects();

    return FUNCTOR_CONTINUE;
}

FunctorCode ScoreDefUnsetCurrentFunctor::VisitMeasure(Measure *measure)
{
    measure->ResetDrawingScoreDef();

    // We also need to remove scoreDef elements in the AlignmentReference objects
    measure->m_measureAligner.Process(*this);

    return FUNCTOR_CONTINUE;
}

FunctorCode ScoreDefUnsetCurrentFunctor::VisitPage(Page *page)
{
    page->m_score = NULL;
    page->m_scoreEnd = NULL;

    return FUNCTOR_CONTINUE;
}

FunctorCode ScoreDefUnsetCurrentFunctor::VisitStaff(Staff *staff)
{
    staff->m_drawingStaffDef = NULL;
    staff->m_drawingTuning = NULL;

    return FUNCTOR_CONTINUE;
}

FunctorCode ScoreDefUnsetCurrentFunctor::VisitSystem(System *system)
{
    system->ResetDrawingScoreDef();
    system->IsDrawingOptimized(false);

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// SetStaffDefRedrawFlagsFunctor
//----------------------------------------------------------------------------

SetStaffDefRedrawFlagsFunctor::SetStaffDefRedrawFlagsFunctor(int redrawFlags) : Functor()
{
    m_redrawFlags = redrawFlags;
}

FunctorCode SetStaffDefRedrawFlagsFunctor::VisitStaffDef(StaffDef *staffDef)
{
    const bool forceRedraw = m_redrawFlags & StaffDefRedrawFlags::FORCE_REDRAW;
    const bool redrawClef = m_redrawFlags & StaffDefRedrawFlags::REDRAW_CLEF;
    if (redrawClef || forceRedraw) {
        staffDef->SetDrawClef(redrawClef);
    }
    const bool redrawKeySig = m_redrawFlags & StaffDefRedrawFlags::REDRAW_KEYSIG;
    if (redrawKeySig || forceRedraw) {
        staffDef->SetDrawKeySig(redrawKeySig);
    }
    const bool redrawMensur = m_redrawFlags & StaffDefRedrawFlags::REDRAW_MENSUR;
    if (redrawMensur || forceRedraw) {
        staffDef->SetDrawMensur(redrawMensur);
    }
    const bool redrawMeterSig = m_redrawFlags & StaffDefRedrawFlags::REDRAW_METERSIG;
    if (redrawMeterSig || forceRedraw) {
        staffDef->SetDrawMeterSig(redrawMeterSig);
    }
    const bool redrawMeterSigGrp = m_redrawFlags & StaffDefRedrawFlags::REDRAW_METERSIGGRP;
    if (redrawMeterSigGrp || forceRedraw) {
        staffDef->SetDrawMeterSigGrp(redrawMeterSigGrp);
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

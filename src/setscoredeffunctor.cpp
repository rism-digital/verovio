/////////////////////////////////////////////////////////////////////////////
// Name:        setscoredeffunctor.cpp
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "setscoredeffunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "functorparams.h"
#include "layer.h"
#include "page.h"
#include "score.h"
#include "staff.h"
#include "system.h"
#include "tuning.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// ReplaceDrawingValuesInStaffDefFunctor
//----------------------------------------------------------------------------

ReplaceDrawingValuesInStaffDefFunctor::ReplaceDrawingValuesInStaffDefFunctor(const Clef *clef, const KeySig *keySig,
    const Mensur *mensur, const MeterSig *meterSig, const MeterSigGrp *meterSigGrp)
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
    if (!page->m_score) {
        page->m_score = m_doc->GetCurrentScore();
    }
    else {
        page->m_scoreEnd = m_doc->GetCurrentScore();
    }

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

FunctorCode ScoreDefSetCurrentFunctor::VisitObject(Object *object)
{
    if (object->Is({ DOC, MDIV, PAGES })) return FUNCTOR_CONTINUE;

    // starting a new page
    if (object->Is(PAGE)) {
        Page *page = vrv_cast<Page *>(object);
        assert(page);
        // This will be reached before we reach the beginning of a first Score.
        // However, page->m_score has already been set by ScoreDefSetCurrentPageFunctor
        // This must be the first page or a new score is starting on this page
        assert(page->m_score);
        if (!m_currentScore || (m_currentScore != page->m_score)) {
            m_upcomingScoreDef = *page->m_score->GetScoreDef();
            m_upcomingScoreDef.Process(*this);
        }
        page->m_drawingScoreDef = m_upcomingScoreDef;
        return FUNCTOR_CONTINUE;
    }

    // starting a new score
    if (object->Is(SCORE)) {
        Score *score = vrv_cast<Score *>(object);
        assert(score);
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

    // starting a new system
    if (object->Is(SYSTEM)) {
        System *system = vrv_cast<System *>(object);
        assert(system);
        // This is the only thing we do for now - we need to wait until we reach the first measure
        m_currentSystem = system;
        m_hasMeasure = false;
        return FUNCTOR_CONTINUE;
    }

    // starting a new measure
    if (object->Is(MEASURE)) {
        // If we have a restart scoreDef before, for redrawing of everything on the measure
        if (m_restart) {
            m_upcomingScoreDef.SetRedrawFlags(StaffDefRedrawFlags::REDRAW_ALL);
        }

        Measure *measure = vrv_cast<Measure *>(object);
        assert(measure);
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
                SetCautionaryScoreDefParams setCautionaryScoreDefParams(&cautionaryScoreDef);
                Functor setCautionaryScoreDef(&Object::SetCautionaryScoreDef);
                m_previousMeasure->Process(&setCautionaryScoreDef, &setCautionaryScoreDefParams);
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

    // starting a new scoreDef
    if (object->Is(SCOREDEF)) {
        ScoreDef *scoreDef = vrv_cast<ScoreDef *>(object);
        assert(scoreDef);
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
                SetCautionaryScoreDefParams setCautionaryScoreDefParams(&cautionaryScoreDef);
                Functor setCautionaryScoreDef(&Object::SetCautionaryScoreDef);
                m_previousMeasure->Process(&setCautionaryScoreDef, &setCautionaryScoreDefParams);
            }
        }
    }

    // starting a new staffGrp
    if (object->Is(STAFFGRP)) {
        StaffGrp *staffGrp = vrv_cast<StaffGrp *>(object);
        assert(staffGrp);
        // For now replace labels only if we have a section@restart
        if (m_restart) {
            m_upcomingScoreDef.ReplaceDrawingLabels(staffGrp);
        }
    }

    // starting a new staffDef
    if (object->Is(STAFFDEF)) {
        StaffDef *staffDef = vrv_cast<StaffDef *>(object);
        assert(staffDef);
        m_upcomingScoreDef.ReplaceDrawingValues(staffDef);
    }

    // starting a new staff
    if (object->Is(STAFF)) {
        Staff *staff = vrv_cast<Staff *>(object);
        assert(staff);
        m_currentStaffDef = m_currentScoreDef->GetStaffDef(staff->GetN());
        assert(staff->m_drawingStaffDef == NULL);
        staff->m_drawingStaffDef = m_currentStaffDef;
        assert(staff->m_drawingTuning == NULL);
        staff->m_drawingTuning = dynamic_cast<Tuning *>(m_currentStaffDef->FindDescendantByType(TUNING));
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

    // starting a new layer
    if (object->Is(LAYER)) {
        Layer *layer = vrv_cast<Layer *>(object);
        assert(layer);
        if (m_doc->GetType() != Transcription) layer->SetDrawingStaffDefValues(m_currentStaffDef);
        return FUNCTOR_CONTINUE;
    }

    // starting a new clef
    if (object->Is(CLEF)) {
        LayerElement *element = vrv_cast<LayerElement *>(object);
        assert(element);
        LayerElement *elementOrLink = element->ThisOrSameasLink();
        if (!elementOrLink || !elementOrLink->Is(CLEF)) return FUNCTOR_CONTINUE;
        Clef *clef = vrv_cast<Clef *>(elementOrLink);
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

    // starting a new keysig
    if (object->Is(KEYSIG)) {
        KeySig *keySig = vrv_cast<KeySig *>(object);
        assert(keySig);
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

    // starting a new mensur
    if (object->Is(MENSUR)) {
        Mensur *mensur = vrv_cast<Mensur *>(object);
        assert(mensur);
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

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

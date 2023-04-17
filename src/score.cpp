/////////////////////////////////////////////////////////////////////////////
// Name:        score.cpp
// Author:      Laurent Pugin
// Created:     29/08/2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "score.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "doc.h"
#include "editorial.h"
#include "ending.h"
#include "functor.h"
#include "functorparams.h"
#include "page.h"
#include "pages.h"
#include "pb.h"
#include "runningelement.h"
#include "sb.h"
#include "section.h"
#include "symboldef.h"
#include "system.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Score
//----------------------------------------------------------------------------

static const ClassRegistrar<Score> s_factory("score", SCORE);

Score::Score() : PageElement(SCORE, "score-"), PageMilestoneInterface(), AttLabelled(), AttNNumberLike()
{
    this->RegisterAttClass(ATT_LABELLED);
    this->RegisterAttClass(ATT_NNUMBERLIKE);

    this->Reset();
}

Score::~Score() {}

void Score::Reset()
{
    Object::Reset();
    this->ResetLabelled();
    this->ResetNNumberLike();

    m_scoreDef.Reset();

    m_drawingPgHeadHeight = 0;
    m_drawingPgFootHeight = 0;
    m_drawingPgHead2Height = 0;
    m_drawingPgFoot2Height = 0;
}

bool Score::IsSupportedChild(Object *child)
{
    if (child->Is(SCOREDEF)) {
        assert(dynamic_cast<ScoreDef *>(child));
    }
    else if (child->Is(SB)) {
        assert(dynamic_cast<Sb *>(child));
    }
    else if (child->Is(SECTION)) {
        assert(dynamic_cast<Section *>(child));
    }
    else if (child->Is(ENDING)) {
        assert(dynamic_cast<Ending *>(child));
    }
    else if (child->Is(PB)) {
        assert(dynamic_cast<Pb *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

void Score::SetAsCurrent()
{
    Doc *doc = vrv_cast<Doc *>(this->GetFirstAncestor(DOC));
    // The doc can be NULL when doing the castoff and the pages are no attached to the doc
    // If such cases, it will not matter not to have the current scoreDef in the doc
    if (doc) {
        doc->SetCurrentScore(this);
    }
}

void Score::CalcRunningElementHeight(Doc *doc)
{
    assert(doc);

    Pages *pages = doc->GetPages();
    assert(pages);
    assert(pages->GetChildCount() == 0);

    Page *page1 = new Page();
    page1->m_score = this;
    page1->m_scoreEnd = this;
    pages->AddChild(page1);
    doc->SetDrawingPage(0);
    page1->LayOutVertically();

    RunningElement *page1Header = page1->GetHeader();
    RunningElement *page1Footer = page1->GetFooter();

    m_drawingPgHeadHeight = (page1Header) ? page1Header->GetTotalHeight(doc) : 0;
    m_drawingPgFootHeight = (page1Footer) ? page1Footer->GetTotalHeight(doc) : 0;

    Page *page2 = new Page();
    page2->m_score = this;
    page2->m_scoreEnd = this;
    pages->AddChild(page2);
    doc->SetDrawingPage(1);
    page2->LayOutVertically();

    RunningElement *page2Header = page2->GetHeader();
    RunningElement *page2Footer = page2->GetFooter();

    m_drawingPgHead2Height = (page2Header) ? page2Header->GetTotalHeight(doc) : 0;
    m_drawingPgFoot2Height = (page2Footer) ? page2Footer->GetTotalHeight(doc) : 0;

    pages->DeleteChild(page1);
    pages->DeleteChild(page2);

    doc->ResetDataPage();
}

bool Score::ScoreDefNeedsOptimization(int optionCondense) const
{
    if (optionCondense == CONDENSE_none) return false;
    // optimize scores only if encoded
    bool optimize = (m_scoreDef.HasOptimize() && m_scoreDef.GetOptimize() == BOOLEAN_true);
    // if nothing specified, do not if there is only one grpSym
    if ((optionCondense == CONDENSE_auto) && !m_scoreDef.HasOptimize()) {
        ListOfConstObjects symbols = m_scoreDef.FindAllDescendantsByType(GRPSYM);
        optimize = (symbols.size() > 1);
    }

    return optimize;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

FunctorCode Score::Accept(MutableFunctor &functor)
{
    return functor.VisitScore(this);
}

FunctorCode Score::Accept(ConstFunctor &functor) const
{
    return functor.VisitScore(this);
}

FunctorCode Score::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitScoreEnd(this);
}

FunctorCode Score::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitScoreEnd(this);
}

int Score::ConvertMarkupScoreDef(FunctorParams *functorParams)
{
    ConvertMarkupScoreDefParams *params = vrv_params_cast<ConvertMarkupScoreDefParams *>(functorParams);
    assert(params);

    // Evaluate functor on scoreDef
    this->GetScoreDef()->Process(params->m_functor, params, params->m_functorEnd);

    return FUNCTOR_CONTINUE;
}

int Score::ConvertToPageBased(FunctorParams *functorParams)
{
    ConvertToPageBasedParams *params = vrv_params_cast<ConvertToPageBasedParams *>(functorParams);
    assert(params);

    assert(!params->m_currentSystem);
    this->MoveItselfTo(params->m_page);
    System *system = new System();
    params->m_currentSystem = system;
    params->m_page->AddChild(system);

    return FUNCTOR_CONTINUE;
}

int Score::ConvertToPageBasedEnd(FunctorParams *functorParams)
{
    ConvertToPageBasedParams *params = vrv_params_cast<ConvertToPageBasedParams *>(functorParams);
    assert(params);

    ConvertToPageBasedMilestone(this, params->m_page);
    params->m_currentSystem = NULL;

    return FUNCTOR_CONTINUE;
}

int Score::Transpose(FunctorParams *functorParams)
{
    TransposeParams *params = vrv_params_cast<TransposeParams *>(functorParams);
    assert(params);

    ScoreDef *scoreDef = this->GetScoreDef();
    Transposer *transposer = params->m_transposer;
    const std::string &transposition = params->m_transposition;

    // Check whether we transpose to sounding pitch
    if (params->m_transposeToSoundingPitch) {
        // Evaluate functor on scoreDef
        scoreDef->Process(params->m_functor, params, params->m_functorEnd);
        return FUNCTOR_CONTINUE;
    }

    // Check whether we are in the selected mdiv
    if (!params->m_selectedMdivID.empty()
        && (std::find(params->m_currentMdivIDs.begin(), params->m_currentMdivIDs.end(), params->m_selectedMdivID)
            == params->m_currentMdivIDs.end())) {
        return FUNCTOR_CONTINUE;
    }

    if (transposer->IsValidIntervalName(transposition)) {
        transposer->SetTransposition(transposition);
    }
    else if (transposer->IsValidKeyTonic(transposition)) {
        // Find the starting key tonic of the data to use in calculating the tranposition interval:
        // Set transposition by key tonic.
        // Detect the current key from the keysignature.
        KeySig *keySig = vrv_cast<KeySig *>(scoreDef->FindDescendantByType(KEYSIG));
        // If there is no keysignature, assume it is C.
        TransPitch currentKey = TransPitch(0, 0, 0);
        if (keySig && keySig->HasPname()) {
            currentKey = TransPitch(keySig->GetPname(), ACCIDENTAL_GESTURAL_NONE, keySig->GetAccid(), 0);
        }
        else if (keySig) {
            // No tonic pitch in key signature, so infer from key signature.
            int fifthsInt = keySig->GetFifthsInt();
            // Check the keySig@mode is present (currently assuming major):
            currentKey = transposer->CircleOfFifthsToMajorTonic(fifthsInt);
            // need to add a dummy "0" key signature in score (staffDefs of staffDef).
        }
        transposer->SetTransposition(currentKey, transposition);
    }
    else if (transposer->IsValidSemitones(transposition)) {
        KeySig *keySig = vrv_cast<KeySig *>(scoreDef->FindDescendantByType(KEYSIG));
        int fifths = 0;
        if (keySig) {
            fifths = keySig->GetFifthsInt();
        }
        else {
            LogWarning("No key signature in data, assuming no key signature with no sharps/flats.");
            // need to add a dummy "0" key signature in score (staffDefs of staffDef).
        }
        transposer->SetTransposition(fifths, transposition);
    }
    else {
        LogWarning("Transposition is invalid: %s", transposition.c_str());
        // there is no transposition that can be done so do not try
        // to transpose any further (if continuing in this function,
        // there will not be an error, just that the transposition
        // will be at the unison, so no notes should change.
        return FUNCTOR_STOP;
    }

    // Evaluate functor on scoreDef
    scoreDef->Process(params->m_functor, params, params->m_functorEnd);

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

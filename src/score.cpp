/////////////////////////////////////////////////////////////////////////////
// Name:        score.cpp
// Author:      Laurent Pugin
// Created:     29/08/2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "score.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "doc.h"
#include "editorial.h"
#include "ending.h"
#include "functorparams.h"
#include "page.h"
#include "pages.h"
#include "pb.h"
#include "runningelement.h"
#include "sb.h"
#include "scoredef.h"
#include "section.h"
#include "system.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Score
//----------------------------------------------------------------------------

static const ClassRegistrar<Score> s_factory("score", SCORE);

Score::Score() : PageElement("score-"), PageElementStartInterface(), AttLabelled(), AttNNumberLike()
{
    RegisterAttClass(ATT_LABELLED);
    RegisterAttClass(ATT_NNUMBERLIKE);

    Reset();
}

Score::~Score() {}

void Score::Reset()
{
    Object::Reset();
    ResetLabelled();
    ResetNNumberLike();

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

    m_drawingPgHeadHeight = (page1Header) ? page1Header->GetTotalHeight() : 0;
    m_drawingPgFootHeight = (page1Footer) ? page1Footer->GetTotalHeight() : 0;

    Page *page2 = new Page();
    page2->m_score = this;
    page2->m_scoreEnd = this;
    pages->AddChild(page2);
    doc->SetDrawingPage(1);
    page2->LayOutVertically();

    RunningElement *page2Header = page2->GetHeader();
    RunningElement *page2Footer = page2->GetFooter();

    m_drawingPgHead2Height = (page2Header) ? page2Header->GetTotalHeight() : 0;
    m_drawingPgFoot2Height = (page2Footer) ? page2Footer->GetTotalHeight() : 0;

    pages->DeleteChild(page1);
    pages->DeleteChild(page2);

    doc->ResetDrawingPage();
}

bool Score::ScoreDefNeedsOptimization(int optionCondense)
{
    if (optionCondense == CONDENSE_none) return false;
    // optimize scores only if encoded
    bool optimize = (m_scoreDef.HasOptimize() && m_scoreDef.GetOptimize() == BOOLEAN_true);
    // if nothing specified, do not if there is only one grpSym
    if ((optionCondense == CONDENSE_auto) && !m_scoreDef.HasOptimize()) {
        ListOfObjects symbols;
        ClassIdComparison matchClassId(GRPSYM);
        m_scoreDef.FindAllDescendantByComparison(&symbols, &matchClassId);
        optimize = (symbols.size() > 1);
    }

    return optimize;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

int Score::AdjustDots(FunctorParams *functorParams)
{
    AdjustDotsParams *params = vrv_params_cast<AdjustDotsParams *>(functorParams);
    assert(params);

    params->m_staffNs = params->m_doc->GetCurrentScoreDef()->GetStaffNs();

    return FUNCTOR_CONTINUE;
}

int Score::AdjustLayers(FunctorParams *functorParams)
{
    AdjustLayersParams *params = vrv_params_cast<AdjustLayersParams *>(functorParams);
    assert(params);

    params->m_staffNs = params->m_doc->GetCurrentScoreDef()->GetStaffNs();

    return FUNCTOR_CONTINUE;
}

int Score::AdjustXPos(FunctorParams *functorParams)
{
    AdjustXPosParams *params = vrv_params_cast<AdjustXPosParams *>(functorParams);
    assert(params);

    params->m_staffNs = params->m_doc->GetCurrentScoreDef()->GetStaffNs();

    return FUNCTOR_CONTINUE;
}

int Score::AdjustGraceXPos(FunctorParams *functorParams)
{
    AdjustGraceXPosParams *params = vrv_params_cast<AdjustGraceXPosParams *>(functorParams);
    assert(params);

    params->m_staffNs = params->m_doc->GetCurrentScoreDef()->GetStaffNs();

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

    ConvertToPageBasedBoundary(this, params->m_page);
    params->m_currentSystem = NULL;

    return FUNCTOR_CONTINUE;
}

int Score::CastOffPages(FunctorParams *functorParams)
{
    CastOffPagesParams *params = vrv_params_cast<CastOffPagesParams *>(functorParams);
    assert(params);

    PageElement::CastOffPages(functorParams);

    params->m_pgHeadHeight = m_drawingPgHeadHeight;
    params->m_pgFootHeight = m_drawingPgFootHeight;
    params->m_pgHead2Height = m_drawingPgHead2Height;
    params->m_pgFoot2Height = m_drawingPgFoot2Height;

    return FUNCTOR_CONTINUE;
}

int Score::UnCastOff(FunctorParams *functorParams)
{
    UnCastOffParams *params = vrv_params_cast<UnCastOffParams *>(functorParams);
    assert(params);

    PageElement::UnCastOff(functorParams);

    assert(!params->m_currentSystem);
    System *system = new System();
    params->m_currentSystem = system;
    params->m_page->AddChild(system);

    return FUNCTOR_CONTINUE;
}

int Score::ScoreDefOptimize(FunctorParams *functorParams)
{
    ScoreDefOptimizeParams *params = vrv_params_cast<ScoreDefOptimizeParams *>(functorParams);
    assert(params);

    params->m_currentScoreDef = NULL;
    params->m_encoded = false;
    params->m_firstScoreDef = true;
    params->m_hasFermata = false;
    params->m_hasTempo = false;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

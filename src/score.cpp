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

FunctorCode Score::Accept(Functor &functor)
{
    return functor.VisitScore(this);
}

FunctorCode Score::Accept(ConstFunctor &functor) const
{
    return functor.VisitScore(this);
}

FunctorCode Score::AcceptEnd(Functor &functor)
{
    return functor.VisitScoreEnd(this);
}

FunctorCode Score::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitScoreEnd(this);
}

} // namespace vrv

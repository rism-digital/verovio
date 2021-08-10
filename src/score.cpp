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

#include "editorial.h"
#include "ending.h"
#include "functorparams.h"
#include "page.h"
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
}

bool Score::IsSupportedChild(Object *child)
{
    if (child->Is(SCOREDEF)) {
        assert(dynamic_cast<ScoreDef *>(child));
    }
    else if (child->Is(SECTION)) {
        assert(dynamic_cast<Section *>(child));
    }
    else if (child->Is(ENDING)) {
        assert(dynamic_cast<Ending *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

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

} // namespace vrv

/////////////////////////////////////////////////////////////////////////////
// Name:        pages.cpp
// Author:      Laurent Pugin
// Created:     2018/02/15
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "pages.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "page.h"
#include "score.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Pages
//----------------------------------------------------------------------------

Pages::Pages() : Object("pages-"), AttLabelled(), AttNNumberLike()
{
    RegisterAttClass(ATT_LABELLED);
    RegisterAttClass(ATT_NNUMBERLIKE);

    Reset();
}

Pages::~Pages() {}

void Pages::Reset()
{
    Object::Reset();
    ResetLabelled();
    ResetNNumberLike();
}

bool Pages::IsSupportedChild(Object *child)
{
    if (child->Is(PAGE)) {
        assert(dynamic_cast<Page *>(child));
    }
    else if (child->Is(SCOREDEF)) {
        assert(dynamic_cast<ScoreDef *>(child));
    }
    else {
        return false;
    }
    return true;
}

void Pages::ConvertFrom(Score *score)
{
    score->SwapUuid(this);
    this->AttLabelled::operator=(*score);
    this->AttNNumberLike::operator=(*score);
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

} // namespace vrv

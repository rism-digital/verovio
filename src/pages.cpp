/////////////////////////////////////////////////////////////////////////////
// Name:        pages.cpp
// Author:      Laurent Pugin
// Created:     2018/02/15
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "pages.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "page.h"
#include "score.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Pages
//----------------------------------------------------------------------------

Pages::Pages() : Object(PAGES, "pages-"), AttLabelled(), AttNNumberLike()
{
    this->RegisterAttClass(ATT_LABELLED);
    this->RegisterAttClass(ATT_NNUMBERLIKE);

    this->Reset();
}

Pages::~Pages() {}

void Pages::Reset()
{
    Object::Reset();
    this->ResetLabelled();
    this->ResetNNumberLike();
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
    score->SwapID(this);
    this->AttLabelled::operator=(*score);
    this->AttNNumberLike::operator=(*score);
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

} // namespace vrv

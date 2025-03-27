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

#include "functor.h"
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

bool Pages::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ PAGE, SCOREDEF };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else {
        return false;
    }
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

FunctorCode Pages::Accept(Functor &functor)
{
    return functor.VisitPages(this);
}

FunctorCode Pages::Accept(ConstFunctor &functor) const
{
    return functor.VisitPages(this);
}

FunctorCode Pages::AcceptEnd(Functor &functor)
{
    return functor.VisitPagesEnd(this);
}

FunctorCode Pages::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitPagesEnd(this);
}

} // namespace vrv

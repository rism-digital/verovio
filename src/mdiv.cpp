/////////////////////////////////////////////////////////////////////////////
// Name:        mdiv.cpp
// Author:      Laurent Pugin
// Created:     2018/02/15
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "mdiv.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "functor.h"
#include "iomei.h"
#include "page.h"
#include "pages.h"
#include "score.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Mdiv
//----------------------------------------------------------------------------

static const ClassRegistrar<Mdiv> s_factory("mdiv", MDIV);

Mdiv::Mdiv() : PageElement(MDIV, "mdiv-"), PageMilestoneInterface(), AttLabelled(), AttNNumberLike()
{
    this->RegisterAttClass(ATT_LABELLED);
    this->RegisterAttClass(ATT_NNUMBERLIKE);

    this->Reset();
}

Mdiv::~Mdiv() {}

void Mdiv::Reset()
{
    Object::Reset();
    this->ResetLabelled();
    this->ResetNNumberLike();

    m_visibility = Hidden;
}

bool Mdiv::IsSupportedChild(Object *child)
{
    if (child->Is(MDIV)) {
        assert(dynamic_cast<Mdiv *>(child));
    }
    else if (child->Is(SCORE)) {
        assert(dynamic_cast<Score *>(child));
    }
    else {
        return false;
    }
    return true;
}

void Mdiv::MakeVisible()
{
    m_visibility = Visible;
    if (this->GetParent() && this->GetParent()->Is(MDIV)) {
        Mdiv *parent = vrv_cast<Mdiv *>(this->GetParent());
        assert(parent);
        parent->MakeVisible();
    }
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

FunctorCode Mdiv::Accept(Functor &functor)
{
    return functor.VisitMdiv(this);
}

FunctorCode Mdiv::Accept(ConstFunctor &functor) const
{
    return functor.VisitMdiv(this);
}

FunctorCode Mdiv::AcceptEnd(Functor &functor)
{
    return functor.VisitMdivEnd(this);
}

FunctorCode Mdiv::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitMdivEnd(this);
}

} // namespace vrv

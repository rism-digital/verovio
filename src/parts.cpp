/////////////////////////////////////////////////////////////////////////////
// Name:        parts.cpp
// Author:      Laurent Pugin
// Created:     2018/02/15
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "parts.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "functor.h"
#include "iomei.h"
#include "page.h"
#include "pages.h"
#include "scoreorpartelement.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Parts
//----------------------------------------------------------------------------

static const ClassRegistrar<Parts> s_factory("parts", PARTS);

Parts::Parts() : PageElement(PARTS), PageMilestoneInterface(), AttLabelled(), AttNNumberLike()
{
    this->RegisterAttClass(ATT_LABELLED);
    this->RegisterAttClass(ATT_NNUMBERLIKE);

    this->Reset();
}

Parts::~Parts() {}

void Parts::Reset()
{
    Object::Reset();
    PageMilestoneInterface::Reset();
    this->ResetLabelled();
    this->ResetNNumberLike();
}

bool Parts::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ PART };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else {
        return false;
    }
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

FunctorCode Parts::Accept(Functor &functor)
{
    return functor.VisitParts(this);
}

FunctorCode Parts::Accept(ConstFunctor &functor) const
{
    return functor.VisitParts(this);
}

FunctorCode Parts::AcceptEnd(Functor &functor)
{
    return functor.VisitPartsEnd(this);
}

FunctorCode Parts::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitPartsEnd(this);
}

} // namespace vrv

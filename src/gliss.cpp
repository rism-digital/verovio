/////////////////////////////////////////////////////////////////////////////
// Name:        gliss.cpp
// Author:      Klaus Rettinghaus
// Created:     2020
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "gliss.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "devicecontext.h"
#include "doc.h"
#include "functor.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Gliss
//----------------------------------------------------------------------------

static const ClassRegistrar<Gliss> s_factory("gliss", GLISS);

Gliss::Gliss() : ControlElement(GLISS), TimeSpanningInterface(), AttLineRend(), AttLineRendBase(), AttNNumberLike()
{
    this->RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    this->RegisterAttClass(ATT_LINEREND);
    this->RegisterAttClass(ATT_LINERENDBASE);
    this->RegisterAttClass(ATT_NNUMBERLIKE);

    this->Reset();
}

Gliss::~Gliss() {}

void Gliss::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    this->ResetLineRend();
    this->ResetLineRendBase();
    this->ResetNNumberLike();
}

//----------------------------------------------------------------------------
// Gliss functor methods
//----------------------------------------------------------------------------

FunctorCode Gliss::Accept(Functor &functor)
{
    return functor.VisitGliss(this);
}

FunctorCode Gliss::Accept(ConstFunctor &functor) const
{
    return functor.VisitGliss(this);
}

FunctorCode Gliss::AcceptEnd(Functor &functor)
{
    return functor.VisitGlissEnd(this);
}

FunctorCode Gliss::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitGlissEnd(this);
}

} // namespace vrv

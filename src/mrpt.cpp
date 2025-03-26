/////////////////////////////////////////////////////////////////////////////
// Name:        mrpt.cpp
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "mrpt.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <math.h>

//----------------------------------------------------------------------------

#include "chord.h"
#include "editorial.h"
#include "functor.h"
#include "layer.h"
#include "note.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// MRpt
//----------------------------------------------------------------------------

static const ClassRegistrar<MRpt> s_factory("mRpt", MRPT);

MRpt::MRpt() : LayerElement(MRPT), AttColor(), AttNumbered(), AttNumberPlacement()
{
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_NUMBERED);
    this->RegisterAttClass(ATT_NUMBERPLACEMENT);

    this->Reset();
}

MRpt::~MRpt() {}

void MRpt::Reset()
{
    LayerElement::Reset();
    this->ResetColor();
    this->ResetNumbered();
    this->ResetNumberPlacement();

    m_drawingMeasureCount = 0;
}

//----------------------------------------------------------------------------
// MRpt functor methods
//----------------------------------------------------------------------------

FunctorCode MRpt::Accept(Functor &functor)
{
    return functor.VisitMRpt(this);
}

FunctorCode MRpt::Accept(ConstFunctor &functor) const
{
    return functor.VisitMRpt(this);
}

FunctorCode MRpt::AcceptEnd(Functor &functor)
{
    return functor.VisitMRptEnd(this);
}

FunctorCode MRpt::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitMRptEnd(this);
}

} // namespace vrv

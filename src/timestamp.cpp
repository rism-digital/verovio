/////////////////////////////////////////////////////////////////////////////
// Name:        timestamp.cpp
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "timestamp.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// TimestampAttr
//----------------------------------------------------------------------------

TimestampAttr::TimestampAttr() : LayerElement(TIMESTAMP_ATTR)
{
    this->Reset();
}

TimestampAttr::~TimestampAttr() {}

void TimestampAttr::Reset()
{
    LayerElement::Reset();

    m_actualDurPos = 0.0;
}

Fraction TimestampAttr::GetTimestampAttrAlignmentDuration(data_DURATION meterUnit) const
{
    Fraction duration(meterUnit);
    return (duration * Fraction(m_actualDurPos * DUR_MAX, DUR_MAX));
}

FunctorCode TimestampAttr::Accept(Functor &functor)
{
    return functor.VisitTimestamp(this);
}

FunctorCode TimestampAttr::Accept(ConstFunctor &functor) const
{
    return functor.VisitTimestamp(this);
}

FunctorCode TimestampAttr::AcceptEnd(Functor &functor)
{
    return functor.VisitTimestampEnd(this);
}

FunctorCode TimestampAttr::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitTimestampEnd(this);
}

} // namespace vrv

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

TimestampAttr::TimestampAttr() : LayerElement(TIMESTAMP_ATTR, "tstp-")
{
    this->Reset();
}

TimestampAttr::~TimestampAttr() {}

void TimestampAttr::Reset()
{
    LayerElement::Reset();

    m_actualDurPos = 0.0;
}

double TimestampAttr::GetTimestampAttrAlignmentDuration(int meterUnit) const
{
    return DUR_MAX / meterUnit * m_actualDurPos;
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

/////////////////////////////////////////////////////////////////////////////
// Name:        lv.cpp
// Author:      Andriy Makarchuk
// Created:     01/07/2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "lv.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Lv
//----------------------------------------------------------------------------

static const ClassRegistrar<Lv> s_factory("lv", LV);

Lv::Lv() : Tie("lv-")
{
    Reset();
}

Lv::~Lv() {}

void Lv::Reset()
{
    Tie::Reset();
}

bool Lv::CalculatePosition(Doc *doc, Staff *staff, int x1, int x2, int spanningType, Point bezier[4])
{
    if (spanningType != SPANNING_START_END) {
        //  this makes no sense
        LogWarning("Lv across systems is not supported. Use <tie> instead.");
        return false;
    }

    LayerElement *start = GetStart();
    LayerElement *end = GetEnd();
    if (start->GetFirstAncestor(MEASURE) != end->GetFirstAncestor(MEASURE)) {
        //  this makes no sense
        LogWarning("Lv across measures is not supported. Use <tie> instead.");
        return false;
    }

    return Tie::CalculatePosition(doc, staff, x1, x2, spanningType, bezier);
}

//----------------------------------------------------------------------------
// Lv functor methods
//----------------------------------------------------------------------------

} // namespace vrv

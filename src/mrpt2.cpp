/////////////////////////////////////////////////////////////////////////////
// Name:        mrpt2.cpp
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "mrpt2.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "chord.h"
#include "editorial.h"
#include "functorparams.h"
#include "layer.h"
#include "note.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// MRpt2
//----------------------------------------------------------------------------

static const ClassRegistrar<MRpt2> s_factory("mRpt2", MRPT2);

MRpt2::MRpt2() : LayerElement(MRPT2, "mrpt2-"), AttColor()
{
    RegisterAttClass(ATT_COLOR);

    Reset();
}

MRpt2::~MRpt2() {}

void MRpt2::Reset()
{
    LayerElement::Reset();
    ResetColor();
}

} // namespace vrv

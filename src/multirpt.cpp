/////////////////////////////////////////////////////////////////////////////
// Name:        multirpt.cpp
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "multirpt.h"

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
// MultiRpt
//----------------------------------------------------------------------------

static const ClassRegistrar<MultiRpt> s_factory("multiRpt", MULTIRPT);

MultiRpt::MultiRpt() : LayerElement(MULTIRPT, "multirpt-"), AttNumbered()
{
    RegisterAttClass(ATT_NUMBERED);
    Reset();
}

MultiRpt::~MultiRpt() {}

void MultiRpt::Reset()
{
    LayerElement::Reset();
    ResetNumbered();
}

} // namespace vrv

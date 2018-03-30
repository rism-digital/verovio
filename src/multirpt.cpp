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

MultiRpt::MultiRpt() : LayerElement("multirpt-"), AttNumbered()
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

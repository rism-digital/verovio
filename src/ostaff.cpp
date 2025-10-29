/////////////////////////////////////////////////////////////////////////////
// Name:        ostaff.cpp
// Author:      Klaus Rettinghaus
// Created:     2025
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "ostaff.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <iterator>
#include <vector>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "doc.h"
#include "editorial.h"
#include "functor.h"
#include "hairpin.h"
#include "keysig.h"
#include "layer.h"
#include "measure.h"
#include "note.h"
#include "page.h"
#include "staffdef.h"
#include "syl.h"
#include "system.h"
#include "timeinterface.h"
#include "tuning.h"
#include "verse.h"
#include "vrv.h"
#include "zone.h"

namespace vrv {

//----------------------------------------------------------------------------
// OStaff
//----------------------------------------------------------------------------

static const ClassRegistrar<OStaff> s_factory("ostaff", OSTAFF);

OStaff::OStaff(int n) : Staff(OSTAFF)
{
    this->Reset();
}

OStaff::~OStaff() {}

void OStaff::Reset()
{
    Staff::Reset();
}

} // namespace vrv

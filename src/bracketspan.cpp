/////////////////////////////////////////////////////////////////////////////
// Name:        bracketspan.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "bracketspan.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "devicecontext.h"
#include "doc.h"
#include "functorparams.h"
#include "verticalaligner.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// BracketSpan
//----------------------------------------------------------------------------

BracketSpan::BracketSpan()
    : ControlElement("bspan-")
    , TimeSpanningInterface()
    , AttBracketSpanLog()
    , AttColor()
    , AttPlacement()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_BRACKETSPANLOG);
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_PLACEMENT);

    Reset();
}

BracketSpan::~BracketSpan() {}

void BracketSpan::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    ResetBracketSpanLog();
    ResetColor();
    ResetPlacement();
}

//----------------------------------------------------------------------------
// BracketSpan functor methods
//----------------------------------------------------------------------------

} // namespace vrv

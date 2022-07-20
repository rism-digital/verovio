/////////////////////////////////////////////////////////////////////////////
// Name:        bracketspan.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "bracketspan.h"

//----------------------------------------------------------------------------

#include <cassert>

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

static const ClassRegistrar<BracketSpan> s_factory("bracketSpan", BRACKETSPAN);

BracketSpan::BracketSpan()
    : ControlElement(BRACKETSPAN, "bspan-")
    , TimeSpanningInterface()
    , AttBracketSpanLog()
    , AttColor()
    , AttLineRend()
    , AttLineRendBase()
{
    this->RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    this->RegisterAttClass(ATT_BRACKETSPANLOG);
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_LINEREND);
    this->RegisterAttClass(ATT_LINERENDBASE);

    this->Reset();
}

BracketSpan::~BracketSpan() {}

void BracketSpan::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    this->ResetBracketSpanLog();
    this->ResetColor();
    this->ResetLineRend();
    this->ResetLineRendBase();
}

//----------------------------------------------------------------------------
// BracketSpan functor methods
//----------------------------------------------------------------------------

} // namespace vrv

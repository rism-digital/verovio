/////////////////////////////////////////////////////////////////////////////
// Name:        trill.cpp
// Author:      Klaus Rettinghaus
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "trill.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "verticalaligner.h"

namespace vrv {

//----------------------------------------------------------------------------
// Trill
//----------------------------------------------------------------------------

Trill::Trill()
    : ControlElement("trill-")
    , TimeSpanningInterface()
    , AttColor()
    , AttExtender()
    , AttLineRend()
    , AttNNumberLike()
    , AttOrnamentAccid()
    , AttPlacement()
    , AttExtSym()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_EXTENDER);
    RegisterAttClass(ATT_LINEREND);
    RegisterAttClass(ATT_NNUMBERLIKE);
    RegisterAttClass(ATT_ORNAMENTACCID);
    RegisterAttClass(ATT_PLACEMENT);
    RegisterAttClass(ATT_EXTSYM);

    Reset();
}

Trill::~Trill() {}

void Trill::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    ResetColor();
    ResetExtender();
    ResetLineRend();
    ResetNNumberLike();
    ResetOrnamentAccid();
    ResetPlacement();
    ResetExtSym();
}

//----------------------------------------------------------------------------
// Trill functor methods
//----------------------------------------------------------------------------

} // namespace vrv

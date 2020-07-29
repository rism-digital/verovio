/////////////////////////////////////////////////////////////////////////////
// Name:        turn.cpp
// Author:      Klaus Rettinghaus
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "turn.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "smufl.h"
#include "verticalaligner.h"

namespace vrv {

//----------------------------------------------------------------------------
// Turn
//----------------------------------------------------------------------------

Turn::Turn()
    : ControlElement("turn-")
    , TimePointInterface()
    , AttColor()
    , AttOrnamentAccid()
    , AttPlacement()
    , AttTurnLog()
    , AttExtSym()
{
    RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_ORNAMENTACCID);
    RegisterAttClass(ATT_PLACEMENT);
    RegisterAttClass(ATT_TURNLOG);
    RegisterAttClass(ATT_EXTSYM);

    Reset();
}

Turn::~Turn() {}

void Turn::Reset()
{
    ControlElement::Reset();
    TimePointInterface::Reset();
    ResetColor();
    ResetOrnamentAccid();
    ResetPlacement();
    ResetTurnLog();
    ResetExtSym();
}

wchar_t Turn::GetTurnGlyph() const
{
    return (GetForm() == turnLog_FORM_lower)? SMUFL_E568_ornamentTurnInverted : SMUFL_E567_ornamentTurn;
}

//----------------------------------------------------------------------------
// Turn functor methods
//----------------------------------------------------------------------------

} // namespace vrv

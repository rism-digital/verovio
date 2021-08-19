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

#include "functorparams.h"
#include "layerelement.h"
#include "smufl.h"
#include "verticalaligner.h"

namespace vrv {

//----------------------------------------------------------------------------
// Turn
//----------------------------------------------------------------------------

static const ClassRegistrar<Turn> s_factory("turn", TURN);

Turn::Turn()
    : ControlElement(TURN, "turn-")
    , TimePointInterface()
    , AttColor()
    , AttExtSym()
    , AttOrnamentAccid()
    , AttPlacementRelStaff()
    , AttTurnLog()
{
    RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_EXTSYM);
    RegisterAttClass(ATT_ORNAMENTACCID);
    RegisterAttClass(ATT_PLACEMENTRELSTAFF);
    RegisterAttClass(ATT_TURNLOG);

    Reset();
}

Turn::~Turn() {}

void Turn::Reset()
{
    ControlElement::Reset();
    TimePointInterface::Reset();
    ResetColor();
    ResetExtSym();
    ResetOrnamentAccid();
    ResetPlacementRelStaff();
    ResetTurnLog();

    m_drawingEndElement = NULL;
}

wchar_t Turn::GetTurnGlyph() const
{
    // If there is glyph.num, prioritize it
    if (HasGlyphNum()) {
        wchar_t code = GetGlyphNum();
        if (NULL != Resources::GetGlyph(code)) return code;
    }
    // If there is glyph.name (second priority)
    else if (HasGlyphName()) {
        wchar_t code = Resources::GetGlyphCode(GetGlyphName());
        if (NULL != Resources::GetGlyph(code)) return code;
    }

    return (GetForm() == turnLog_FORM_lower) ? SMUFL_E568_ornamentTurnInverted : SMUFL_E567_ornamentTurn;
}

//----------------------------------------------------------------------------
// Turn functor methods
//----------------------------------------------------------------------------

int Turn::PrepareDelayedTurns(FunctorParams *functorParams)
{
    PrepareDelayedTurnsParams *params = vrv_params_cast<PrepareDelayedTurnsParams *>(functorParams);
    assert(params);

    // We already initialized the params->m_delayedTurns map
    if (!params->m_initMap) return FUNCTOR_CONTINUE;

    // Map only delayed turns
    if (this->GetDelayed() != BOOLEAN_true) return FUNCTOR_CONTINUE;

    // Map only delayed turn pointing to a LayerElement (i.e., not using @tstamp)
    if (this->GetStart() && !this->GetStart()->Is(TIMESTAMP_ATTR)) {
        params->m_delayedTurns[this->GetStart()] = this;
    }

    return FUNCTOR_CONTINUE;
}

int Turn::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    ControlElement::ResetDrawing(functorParams);

    m_drawingEndElement = NULL;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

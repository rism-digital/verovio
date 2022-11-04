/////////////////////////////////////////////////////////////////////////////
// Name:        turn.cpp
// Author:      Klaus Rettinghaus
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "turn.h"

//----------------------------------------------------------------------------

#include <cassert>

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
    this->RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_EXTSYM);
    this->RegisterAttClass(ATT_ORNAMENTACCID);
    this->RegisterAttClass(ATT_PLACEMENTRELSTAFF);
    this->RegisterAttClass(ATT_TURNLOG);

    this->Reset();
}

Turn::~Turn() {}

void Turn::Reset()
{
    ControlElement::Reset();
    TimePointInterface::Reset();
    this->ResetColor();
    this->ResetExtSym();
    this->ResetOrnamentAccid();
    this->ResetPlacementRelStaff();
    this->ResetTurnLog();

    m_drawingEndElement = NULL;
}

char32_t Turn::GetTurnGlyph() const
{
    const Resources *resources = this->GetDocResources();
    if (!resources) return 0;

    // If there is glyph.num, prioritize it
    if (this->HasGlyphNum()) {
        char32_t code = this->GetGlyphNum();
        if (NULL != resources->GetGlyph(code)) return code;
    }
    // If there is glyph.name (second priority)
    else if (this->HasGlyphName()) {
        char32_t code = resources->GetGlyphCode(this->GetGlyphName());
        if (NULL != resources->GetGlyph(code)) return code;
    }

    return (this->GetForm() == turnLog_FORM_lower) ? SMUFL_E568_ornamentTurnInverted : SMUFL_E567_ornamentTurn;
}

int Turn::GetTurnHeight(const Doc *doc, int staffSize) const
{
    assert(doc);

    char32_t originalGlyph = this->GetTurnGlyph();
    char32_t referenceGlyph;
    switch (originalGlyph) {
        case SMUFL_E569_ornamentTurnSlash: referenceGlyph = SMUFL_E567_ornamentTurn; break;
        case SMUFL_E56D_ornamentMordent: referenceGlyph = SMUFL_E56C_ornamentShortTrill; break;
        default: referenceGlyph = originalGlyph;
    }
    return doc->GetGlyphHeight(referenceGlyph, staffSize, false);
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

int Turn::ResetData(FunctorParams *functorParams)
{
    // Call parent one too
    ControlElement::ResetData(functorParams);

    m_drawingEndElement = NULL;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

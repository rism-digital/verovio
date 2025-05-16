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

#include "doc.h"
#include "functor.h"
#include "layerelement.h"
#include "resources.h"
#include "smufl.h"
#include "verticalaligner.h"

namespace vrv {

//----------------------------------------------------------------------------
// Turn
//----------------------------------------------------------------------------

static const ClassRegistrar<Turn> s_factory("turn", TURN);

Turn::Turn()
    : ControlElement(TURN)
    , TimePointInterface()
    , AttExtSymAuth()
    , AttExtSymNames()
    , AttOrnamentAccid()
    , AttPlacementRelStaff()
    , AttTurnLog()
{
    this->RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    this->RegisterAttClass(ATT_EXTSYMAUTH);
    this->RegisterAttClass(ATT_EXTSYMNAMES);
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
    this->ResetExtSymAuth();
    this->ResetExtSymNames();
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

FunctorCode Turn::Accept(Functor &functor)
{
    return functor.VisitTurn(this);
}

FunctorCode Turn::Accept(ConstFunctor &functor) const
{
    return functor.VisitTurn(this);
}

FunctorCode Turn::AcceptEnd(Functor &functor)
{
    return functor.VisitTurnEnd(this);
}

FunctorCode Turn::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitTurnEnd(this);
}

} // namespace vrv

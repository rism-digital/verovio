/////////////////////////////////////////////////////////////////////////////
// Name:        pedal.cpp
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "pedal.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "doc.h"
#include "functor.h"
#include "horizontalaligner.h"
#include "layerelement.h"
#include "resources.h"
#include "smufl.h"
#include "system.h"
#include "vrv.h"

//----------------------------------------------------------------------------

#include "MidiFile.h"

namespace vrv {

//----------------------------------------------------------------------------
// Pedal
//----------------------------------------------------------------------------

static const ClassRegistrar<Pedal> s_factory("pedal", PEDAL);

Pedal::Pedal()
    : ControlElement(PEDAL)
    , TimeSpanningInterface()
    , AttExtSymAuth()
    , AttExtSymNames()
    , AttPedalLog()
    , AttPedalVis()
    , AttPlacementRelStaff()
    , AttVerticalGroup()
{
    this->RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    this->RegisterAttClass(ATT_EXTSYMAUTH);
    this->RegisterAttClass(ATT_EXTSYMNAMES);
    this->RegisterAttClass(ATT_PEDALLOG);
    this->RegisterAttClass(ATT_PEDALVIS);
    this->RegisterAttClass(ATT_PLACEMENTRELSTAFF);
    this->RegisterAttClass(ATT_VERTICALGROUP);

    this->Reset();
}

Pedal::~Pedal() {}

void Pedal::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    this->ResetExtSymAuth();
    this->ResetExtSymNames();
    this->ResetPedalLog();
    this->ResetPedalVis();
    this->ResetPlacementRelStaff();
    this->ResetVerticalGroup();

    m_endsWithBounce = false;
}

char32_t Pedal::GetPedalGlyph() const
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

    return (this->GetFunc() == "sostenuto") ? SMUFL_E659_keyboardPedalSost : SMUFL_E650_keyboardPedalPed;
}

data_PEDALSTYLE Pedal::GetPedalForm(const Doc *doc, const System *system) const
{
    data_PEDALSTYLE style = static_cast<data_PEDALSTYLE>(doc->GetOptions()->m_pedalStyle.GetValue());
    if (style != PEDALSTYLE_NONE) {
        return style;
    }
    else if (this->HasForm()) {
        style = this->GetForm();
    }
    else if (const ScoreDef *scoreDef = system->GetDrawingScoreDef(); scoreDef && scoreDef->HasPedalStyle()) {
        style = scoreDef->GetPedalStyle();
    }

    return style;
}

//----------------------------------------------------------------------------
// Pedal functor methods
//----------------------------------------------------------------------------

FunctorCode Pedal::Accept(Functor &functor)
{
    return functor.VisitPedal(this);
}

FunctorCode Pedal::Accept(ConstFunctor &functor) const
{
    return functor.VisitPedal(this);
}

FunctorCode Pedal::AcceptEnd(Functor &functor)
{
    return functor.VisitPedalEnd(this);
}

FunctorCode Pedal::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitPedalEnd(this);
}

} // namespace vrv

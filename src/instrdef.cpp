/////////////////////////////////////////////////////////////////////////////
// Name:        instrdef.cpp
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "instrdef.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "scoredef.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// InstrDef
//----------------------------------------------------------------------------

static const ClassRegistrar<InstrDef> s_factory("instrDef", INSTRDEF);

InstrDef::InstrDef()
    : Object(INSTRDEF, "instrdef-"), AttChannelized(), AttLabelled(), AttMidiInstrument(), AttNNumberLike()
{
    this->RegisterAttClass(ATT_CHANNELIZED);
    this->RegisterAttClass(ATT_LABELLED);
    this->RegisterAttClass(ATT_MIDIINSTRUMENT);
    this->RegisterAttClass(ATT_NNUMBERLIKE);

    this->Reset();
}

InstrDef::~InstrDef() {}

void InstrDef::Reset()
{
    Object::Reset();
    this->ResetChannelized();
    this->ResetLabelled();
    this->ResetMidiInstrument();
    this->ResetNNumberLike();
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

} // namespace vrv

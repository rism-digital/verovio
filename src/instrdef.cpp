/////////////////////////////////////////////////////////////////////////////
// Name:        instrdef.cpp
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "instrdef.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "scoredef.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// InstrDef
//----------------------------------------------------------------------------

InstrDef::InstrDef() : Object("instrdef-"), AttChannelized(), AttLabelled(), AttMidiInstrument(), AttNNumberLike()
{
    RegisterAttClass(ATT_CHANNELIZED);
    RegisterAttClass(ATT_LABELLED);
    RegisterAttClass(ATT_MIDIINSTRUMENT);
    RegisterAttClass(ATT_NNUMBERLIKE);

    Reset();
}

InstrDef::~InstrDef() {}

void InstrDef::Reset()
{
    Object::Reset();
    ResetChannelized();
    ResetLabelled();
    ResetMidiInstrument();
    ResetNNumberLike();
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

} // namespace vrv

/////////////////////////////////////////////////////////////////////////////
// Name:        syllable.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "syllable.h"
#include "scoredefinterface.h"

namespace vrv {

int Syllable::s_num = 3;
int Syllable::s_numBase = 2;

//----------------------------------------------------------------------------
// Syllable
//----------------------------------------------------------------------------

Syllable::Syllable()
    : LayerElement("syllable-")
    , AttColor()
    , AttDurationRatio()
    , AttRelativesize()
    , AttSlashcount()
{
    Init();
}

void Syllable::Init()
{
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_DURATIONRATIO);
    RegisterAttClass(ATT_RELATIVESIZE);
    RegisterAttClass(ATT_SLASHCOUNT);

    Reset();
}

Syllable::~Syllable()
{
}

void Syllable::Reset()
{
    LayerElement::Reset();
    ResetColor();
    ResetDurationRatio();
    ResetRelativesize();
    ResetSlashcount();
}

} // namespace vrv

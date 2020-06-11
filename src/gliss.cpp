/////////////////////////////////////////////////////////////////////////////
// Name:        gliss.cpp
// Author:      Klaus Rettinghaus
// Created:     2020
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "gliss.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "devicecontext.h"
#include "doc.h"
#include "functorparams.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Gliss
//----------------------------------------------------------------------------

Gliss::Gliss()
    : ControlElement("gliss-"), TimeSpanningInterface(), AttColor(), AttLineRend(), AttLineRendBase(), AttNNumberLike()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_LINEREND);
    RegisterAttClass(ATT_LINERENDBASE);
    RegisterAttClass(ATT_NNUMBERLIKE);

    Reset();
}

Gliss::~Gliss() {}

void Gliss::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    ResetColor();
    ResetLineRend();
    ResetLineRendBase();
    ResetNNumberLike();
}

//----------------------------------------------------------------------------
// Gliss functor methods
//----------------------------------------------------------------------------

} // namespace vrv

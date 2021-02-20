/////////////////////////////////////////////////////////////////////////////
// Name:        btrem.cpp
// Author:      Klaus Rettinghaus
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "btrem.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "chord.h"
#include "editorial.h"
#include "layer.h"
#include "note.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// BTrem
//----------------------------------------------------------------------------

BTrem::BTrem() : LayerElement("btrem-"), AttBTremLog(), AttTremMeasured()
{
    RegisterAttClass(ATT_BTREMLOG);
    RegisterAttClass(ATT_TREMMEASURED);

    Reset();
}

BTrem::~BTrem() {}

void BTrem::Reset()
{
    LayerElement::Reset();
    ResetBTremLog();
    ResetTremMeasured();
}

bool BTrem::IsSupportedChild(Object *child)
{
    if (child->Is(CHORD)) {
        assert(dynamic_cast<Chord *>(child));
    }
    else if (child->Is(CLEF)) {
        assert(dynamic_cast<Clef *>(child));
    }
    else if (child->Is(NOTE)) {
        assert(dynamic_cast<Note *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

} // namespace vrv

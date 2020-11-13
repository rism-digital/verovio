/////////////////////////////////////////////////////////////////////////////
// Name:        textdirinterface.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "textdirinterface.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// TextDirInterface
//----------------------------------------------------------------------------

TextDirInterface::TextDirInterface() : Interface(), AttPlacement()
{
    RegisterInterfaceAttClass(ATT_PLACEMENT);

    Reset();
}

TextDirInterface::~TextDirInterface() {}

void TextDirInterface::Reset()
{
    ResetPlacement();
}

int TextDirInterface::GetNumberOfLines(Object *object)
{
    assert(object);

    ListOfObjects lbs;
    ClassIdComparison matches(LB);
    object->FindAllDescendantByComparison(&lbs, &matches);
    return ((int)lbs.size() + 1);
}

} // namespace vrv

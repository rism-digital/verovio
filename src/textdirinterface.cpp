/////////////////////////////////////////////////////////////////////////////
// Name:        textdirinterface.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "textdirinterface.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// TextDirInterface
//----------------------------------------------------------------------------

TextDirInterface::TextDirInterface() : Interface(), AttPlacementRelStaff()
{
    this->RegisterInterfaceAttClass(ATT_PLACEMENTRELSTAFF);

    this->Reset();
}

TextDirInterface::~TextDirInterface() {}

void TextDirInterface::Reset()
{
    this->ResetPlacementRelStaff();
}

int TextDirInterface::GetNumberOfLines(const Object *object) const
{
    assert(object);

    return object->GetDescendantCount(LB) + 1;
}

} // namespace vrv

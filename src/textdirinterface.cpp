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

bool TextDirInterface::AreChildrenAlignedTo(const Object *object, data_HORIZONTALALIGNMENT alignment) const
{
    ArrayOfConstObjects children = object->GetChildren();
    bool hasHalign = std::any_of(children.begin(), children.end(), [&alignment](const Object *child) {
        const AttHorizontalAlign *hAlign = dynamic_cast<const AttHorizontalAlign *>(child);
        return (hAlign && (hAlign->GetHalign() == alignment));
    });
    return hasHalign;
}

} // namespace vrv

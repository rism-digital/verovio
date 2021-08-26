/////////////////////////////////////////////////////////////////////////////
// Name:        multirest.cpp
// Author:      Rodolfo Zitellini
// Created:     2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "multirest.h"

//----------------------------------------------------------------------------

#include "doc.h"

namespace vrv {

//----------------------------------------------------------------------------
// MultiRest
//----------------------------------------------------------------------------

static const ClassRegistrar<MultiRest> s_factory("multiRest", MULTIREST);

MultiRest::MultiRest()
    : LayerElement(MULTIREST, "multirest-")
    , PositionInterface()
    , AttColor()
    , AttMultiRestVis()
    , AttNumbered()
    , AttWidth()
{
    RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_MULTIRESTVIS);
    RegisterAttClass(ATT_NUMBERED);
    RegisterAttClass(ATT_WIDTH);
    Reset();
}

MultiRest::~MultiRest() {}

void MultiRest::Reset()
{
    LayerElement::Reset();
    PositionInterface::Reset();
    ResetColor();
    ResetMultiRestVis();
    ResetNumbered();
    ResetWidth();
}

bool MultiRest::UseBlockStyle(Doc *doc) const
{
    bool useBlock = false;
    switch (doc->GetOptions()->m_multiRestStyle.GetValue()) {
        case MULTIRESTSTYLE_auto:
            if (GetNum() > 15) {
                useBlock = true;
            }
            else if (GetNum() > 4) {
                useBlock = (GetBlock() != BOOLEAN_false);
            }
            else {
                useBlock = (GetBlock() == BOOLEAN_true);
            }
            break;
        case MULTIRESTSTYLE_default: useBlock = (GetNum() > 4); break;
        case MULTIRESTSTYLE_block: useBlock = (GetNum() > 1); break;
        case MULTIRESTSTYLE_symbols: useBlock = (GetNum() > 30); break;
        default: // should not arrive here
            break;
    }
    return useBlock;
}

} // namespace vrv

/////////////////////////////////////////////////////////////////////////////
// Name:        multirest.cpp
// Author:      Rodolfo Zitellini
// Created:     2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "multirest.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// MultiRest
//----------------------------------------------------------------------------

static const ClassRegistrar<MultiRest> s_factory("multiRest", MULTIREST);

MultiRest::MultiRest()
    : LayerElement(MULTIREST)
    , PositionInterface()
    , AttColor()
    , AttMultiRestVis()
    , AttNumbered()
    , AttNumberPlacement()
    , AttWidth()
{
    this->RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_MULTIRESTVIS);
    this->RegisterAttClass(ATT_NUMBERED);
    this->RegisterAttClass(ATT_NUMBERPLACEMENT);
    this->RegisterAttClass(ATT_WIDTH);
    this->Reset();
}

MultiRest::~MultiRest() {}

void MultiRest::Reset()
{
    LayerElement::Reset();
    PositionInterface::Reset();
    this->ResetColor();
    this->ResetMultiRestVis();
    this->ResetNumbered();
    this->ResetNumberPlacement();
    this->ResetWidth();
}

bool MultiRest::UseBlockStyle(const Doc *doc) const
{
    bool useBlock = false;
    switch (doc->GetOptions()->m_multiRestStyle.GetValue()) {
        case MULTIRESTSTYLE_auto:
            if (this->GetNum() > 15) {
                useBlock = true;
            }
            else if (this->GetNum() > 4) {
                useBlock = (this->GetBlock() != BOOLEAN_false);
            }
            else {
                useBlock = (this->GetBlock() == BOOLEAN_true);
            }
            break;
        case MULTIRESTSTYLE_default: useBlock = (this->GetNum() > 4); break;
        case MULTIRESTSTYLE_block: useBlock = (this->GetNum() > 1); break;
        case MULTIRESTSTYLE_symbols: useBlock = (this->GetNum() > 30); break;
        default: // should not arrive here
            break;
    }
    return useBlock;
}

FunctorCode MultiRest::Accept(Functor &functor)
{
    return functor.VisitMultiRest(this);
}

FunctorCode MultiRest::Accept(ConstFunctor &functor) const
{
    return functor.VisitMultiRest(this);
}

FunctorCode MultiRest::AcceptEnd(Functor &functor)
{
    return functor.VisitMultiRestEnd(this);
}

FunctorCode MultiRest::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitMultiRestEnd(this);
}

} // namespace vrv

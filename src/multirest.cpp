/////////////////////////////////////////////////////////////////////////////
// Name:        multirest.cpp
// Author:      Rodolfo Zitellini
// Created:     2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "multirest.h"

namespace vrv {

//----------------------------------------------------------------------------
// MultiRest
//----------------------------------------------------------------------------

MultiRest::MultiRest():
    LayerElement("multirest-"),
    AttNumbered()
{
    Reset();
}

MultiRest::MultiRest(int number):
    LayerElement("multirest-")
{
    Reset();
}


MultiRest::~MultiRest()
{
}

void MultiRest::Reset()
{
    LayerElement::Reset();
    ResetNumbered();
}

bool MultiRest::operator==( Object& other )
{
    MultiRest *otherM = dynamic_cast<MultiRest*>( &other );
    if ( !otherM ) {
        return false;
    }
    if ( this->GetNum() != otherM->GetNum() ) {
        return false;
    }
    return true;
}

} // namespace vrv
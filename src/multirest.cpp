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
    LayerElement("multirest-")
{
    m_number = 0;
}

MultiRest::MultiRest(int number):
    LayerElement("multirest-")
{
    m_number = number;
}


MultiRest::~MultiRest()
{
}


bool MultiRest::operator==( Object& other )
{
    MultiRest *otherM = dynamic_cast<MultiRest*>( &other );
    if ( !otherM ) {
        return false;
    }
    if ( this->m_number != otherM->m_number ) {
        return false;
    }
    return true;
}

} // namespace vrv
/////////////////////////////////////////////////////////////////////////////
// Name:        mrpt.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "mrpt.h"

namespace vrv {

//----------------------------------------------------------------------------
// MRpt
//----------------------------------------------------------------------------

MRpt::MRpt( ):
    LayerElement("mrpt-")
{
    Reset();
}

MRpt::~MRpt()
{
}
    
void MRpt::Reset()
{
    LayerElement::Reset();
}

} // namespace vrv

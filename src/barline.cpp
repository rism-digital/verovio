/////////////////////////////////////////////////////////////////////////////
// Name:        barline.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "barline.h"

namespace vrv {

//----------------------------------------------------------------------------
// BarLine
//----------------------------------------------------------------------------

BarLine::BarLine():
	LayerElement("bline-"),
    AttBarLineLog()
{
    RegisterAttClass(ATT_BARLINELOG);
    Reset();
}

BarLine::~BarLine()
{
}
    
void BarLine::Reset()
{
    LayerElement::Reset();
    ResetBarLineLog();
}

bool BarLine::HasRepetitionDots()
{
    if (GetForm() == BARRENDITION_rptstart || GetForm() == BARRENDITION_rptend || GetForm() == BARRENDITION_rptboth) {
        return true;
    }
    return false;
}
    
//----------------------------------------------------------------------------
// BarLineAttr
//----------------------------------------------------------------------------

BarLineAttr::BarLineAttr():
    BarLine()
{
}

BarLineAttr::~BarLineAttr()
{
}

} // namespace vrv
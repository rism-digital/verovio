/////////////////////////////////////////////////////////////////////////////
// Name:        barline.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "barline.h"

namespace vrv {

//----------------------------------------------------------------------------
// Barline
//----------------------------------------------------------------------------

Barline::Barline():
	LayerElement("bline-"),
    AttBarLineLog()
{
    RegisterAttClass(ATT_BARLINELOG);
    Reset();
}

Barline::~Barline()
{
}
    
void Barline::Reset()
{
    LayerElement::Reset();
    ResetBarLineLog();
}

bool Barline::HasRepetitionDots()
{
    if (GetRend() == BARRENDITION_rptstart || GetRend() == BARRENDITION_rptend || GetRend() == BARRENDITION_rptboth) {
        return true;
    }
    return false;
}
    
//----------------------------------------------------------------------------
// BarlineAttr
//----------------------------------------------------------------------------

BarlineAttr::BarlineAttr():
    Barline()
{
}

BarlineAttr::~BarlineAttr()
{
}

} // namespace vrv
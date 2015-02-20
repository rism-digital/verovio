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
    Reset();
    m_partialBarline = false; // this was used unitialized
    m_onStaffOnly = false; // is this good?
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

} // namespace vrv
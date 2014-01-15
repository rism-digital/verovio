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
	LayerElement("bline-")
{
    m_barlineType = BARLINE_SINGLE;
    m_partialBarline = false; // this was used unitialized
    m_onStaffOnly = false; // is this good?
}

Barline::~Barline()
{
}

bool Barline::operator==( Object& other )
{
    Barline *otherBarline = dynamic_cast<Barline*>( &other );
    if ( !otherBarline ) {
        return false;
    }
    return true;
}

bool Barline::HasRepetitionDots()
{
    if (m_barlineType == BARLINE_RPTSTART || m_barlineType == BARLINE_RPTEND || m_barlineType == BARLINE_RPTBOTH) {
        return true;
    }
    return false;
}

} // namespace vrv
/////////////////////////////////////////////////////////////////////////////
// Name:        style.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "style.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// Style
//----------------------------------------------------------------------------

Style::Style()
{
    
    m_unit = DEFAULT_UNIT * DEFINITON_FACTOR;
    m_landscape = false;
    m_staffLineWidth = (unsigned char)(2.0 * DEFINITON_FACTOR);
    m_stemWidth = (unsigned char)(2.0 * DEFINITON_FACTOR);
    m_barlineWidth = (unsigned char)(3.0 * DEFINITON_FACTOR);
    m_beamMaxSlope = 30;
    m_beamMinSlope = 10;
    
    // originally in WG Parameters2
    m_smallStaffNum = 3;
    m_smallStaffDen = 4;
    m_graceNum = 3;
    m_graceDen = 4;
    
    m_pageHeight = DEFAULT_PAGE_HEIGHT * DEFINITON_FACTOR;
    m_pageWidth = DEFAULT_PAGE_WIDTH * DEFINITON_FACTOR;
    m_pageRightMar = DEFAULT_PAGE_RIGHT_MAR * DEFINITON_FACTOR;;
    m_pageLeftMar = DEFAULT_PAGE_LEFT_MAR * DEFINITON_FACTOR;
    m_pageTopMar = DEFAULT_PAGE_TOP_MAR * DEFINITON_FACTOR;
    m_spacingStaff = DEFAULT_SPACING_STAFF;
    m_spacingSystem = DEFAULT_SPACING_SYSTEM;
}

Style::~Style()
{
}

} // namespace vrv
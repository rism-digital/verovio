/////////////////////////////////////////////////////////////////////////////
// Name:        style.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "style.h"

//----------------------------------------------------------------------------

#include "vrvdef.h"

namespace vrv {

//----------------------------------------------------------------------------
// Style
//----------------------------------------------------------------------------

Style::Style()
{
    
    m_unit = DEFAULT_UNIT * DEFINITON_FACTOR;
    m_landscape = false;
    m_barlineWidth = (unsigned short)(DEFAULT_BARLINE_WITDH * DEFINITON_FACTOR);
    m_staffLineWidth = (unsigned short)(DEFAULT_STAFFLINE_WITDH * DEFINITON_FACTOR);
    m_stemWidth = (unsigned short)(DEFAULT_STEM_WITDH * DEFINITON_FACTOR);
    
    /**
     * Wolfgang legacy code used for initialiazing Doc::m_drawingBeamMin/MaxSlope
     * and used in View::DrawBeamPostponed
     */
    m_beamMaxSlope = 10;
    m_beamMinSlope = 0;
    
    /**
     * We should probably keep graceNum and Den for cue sized notes.
     * smallStaffNum and Den should be replaced for a more MEI way (percentage?)
     */
    m_smallStaffNum = 4;
    m_smallStaffDen = 5;
    m_graceNum = 3;
    m_graceDen = 4;
    
    m_pageHeight = DEFAULT_PAGE_HEIGHT * DEFINITON_FACTOR;
    m_pageWidth = DEFAULT_PAGE_WIDTH * DEFINITON_FACTOR;
    m_pageRightMar = DEFAULT_PAGE_RIGHT_MAR * DEFINITON_FACTOR;;
    m_pageLeftMar = DEFAULT_PAGE_LEFT_MAR * DEFINITON_FACTOR;
    m_pageTopMar = DEFAULT_PAGE_TOP_MAR * DEFINITON_FACTOR;
    m_spacingStaff = DEFAULT_SPACING_STAFF;
    m_spacingSystem = DEFAULT_SPACING_SYSTEM;
    
    m_lyricSize = DEFAULT_LYRIC_SIZE * PARAM_DENOMINATOR;
    
    m_minMeasureWidth = (short)(DEFAULT_MEASURE_WIDTH * PARAM_DENOMINATOR);
}

Style::~Style()
{
}

} // namespace vrv
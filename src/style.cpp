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
    m_barLineWidth = (unsigned short)(DEFAULT_BARLINE_WITDH * DEFINITON_FACTOR);
    m_staffLineWidth = (unsigned short)(DEFAULT_STAFFLINE_WITDH * DEFINITON_FACTOR);
    m_stemWidth = (unsigned short)(DEFAULT_STEM_WITDH * DEFINITON_FACTOR);
    
    /**
     * Wolfgang legacy code used for initialiazing Doc::m_drawingBeamMin/MaxSlope
     * and used in View::DrawBeamPostponed
     */
    m_beamMaxSlope = 10;
    m_beamMinSlope = 0;
    
    /**
     * graceNum and Den for cue sized notes.
     */
    m_graceNum = 5;
    m_graceDen = 7;
    
    m_pageHeight = DEFAULT_PAGE_HEIGHT * DEFINITON_FACTOR;
    m_pageWidth = DEFAULT_PAGE_WIDTH * DEFINITON_FACTOR;
    m_pageRightMar = DEFAULT_PAGE_RIGHT_MAR * DEFINITON_FACTOR;;
    m_pageLeftMar = DEFAULT_PAGE_LEFT_MAR * DEFINITON_FACTOR;
    m_pageTopMar = DEFAULT_PAGE_TOP_MAR * DEFINITON_FACTOR;
    m_spacingStaff = DEFAULT_SPACING_STAFF;
    m_spacingSystem = DEFAULT_SPACING_SYSTEM;
    
    m_lyricSize = DEFAULT_LYRIC_SIZE * PARAM_DENOMINATOR;
    
    m_minMeasureWidth = (short)(DEFAULT_MEASURE_WIDTH * PARAM_DENOMINATOR);
    
    /** The layout right margin by element */
    m_rightMarginBarline = DEFAULT_RIGHT_MARGIN_BARLINE * DEFINITON_FACTOR;
    m_rightMarginBarlineAttr = DEFAULT_RIGHT_MARGIN_BARLINE_ATTR * DEFINITON_FACTOR;
    m_rightMarginClef = DEFAULT_RIGHT_MARGIN_CLEF * DEFINITON_FACTOR;
    m_rightMarginKeySig = DEFAULT_RIGHT_MARGIN_KEYSIG * DEFINITON_FACTOR;
    m_rightMarginMensur = DEFAULT_RIGHT_MARGIN_MENSUR * DEFINITON_FACTOR;
    m_rightMarginMeterSig = DEFAULT_RIGHT_MARGIN_METERSIG * DEFINITON_FACTOR;
    m_rightMarginMRest = DEFAULT_RIGHT_MARGIN_MREST * DEFINITON_FACTOR;
    m_rightMarginMultiRest = DEFAULT_RIGHT_MARGIN_MULTIREST * DEFINITON_FACTOR;
    /** The default right margin */
    m_rightMarginDefault = DEFAULT_RIGHT_MARGIN_DEFAULT * DEFINITON_FACTOR;
    
    /** The layout left margin by element */
    m_leftMarginBarline = DEFAULT_LEFT_MARGIN_BARLINE * DEFINITON_FACTOR;
    m_leftMarginBarlineAttr = DEFAULT_LEFT_MARGIN_BARLINE_ATTR * DEFINITON_FACTOR;
    m_leftMarginChord = DEFAULT_LEFT_MARGIN_CHORD * DEFINITON_FACTOR;
    m_leftMarginClef = DEFAULT_LEFT_MARGIN_CLEF * DEFINITON_FACTOR;
    m_leftMarginMRest = DEFAULT_LEFT_MARGIN_MREST * DEFINITON_FACTOR;
    m_leftMarginNote = DEFAULT_LEFT_MARGIN_NOTE * DEFINITON_FACTOR;
    /** The default left margin */
    m_leftMarginDefault = DEFAULT_LEFT_MARGIN_DEFAULT * DEFINITON_FACTOR;
}

Style::~Style()
{
}

} // namespace vrv
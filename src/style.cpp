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

    m_unit = DEFAULT_UNIT * DEFINITION_FACTOR;
    m_landscape = false;
    m_barLineWidth = (unsigned short)(DEFAULT_BARLINE_WITDH * DEFINITION_FACTOR);
    m_staffLineWidth = (unsigned short)(DEFAULT_STAFFLINE_WITDH * DEFINITION_FACTOR);
    m_stemWidth = (unsigned short)(DEFAULT_STEM_WITDH * DEFINITION_FACTOR);

    /**
     * Used for initializing Doc::m_drawingBeamMin/MaxSlope and used in View::DrawBeamPostponed
     */
    m_beamMaxSlope = 10;
    m_beamMinSlope = 0;

    /**
     * graceNum and Den for cue-sized notes.
     */
    m_graceNum = 5;
    m_graceDen = 7;

    m_pageHeight = DEFAULT_PAGE_HEIGHT * DEFINITION_FACTOR;
    m_pageWidth = DEFAULT_PAGE_WIDTH * DEFINITION_FACTOR;
    m_pageRightMar = DEFAULT_PAGE_RIGHT_MAR * DEFINITION_FACTOR;

    m_pageLeftMar = DEFAULT_PAGE_LEFT_MAR * DEFINITION_FACTOR;
    m_pageTopMar = DEFAULT_PAGE_TOP_MAR * DEFINITION_FACTOR;
    m_spacingStaff = DEFAULT_SPACING_STAFF;
    m_spacingSystem = DEFAULT_SPACING_SYSTEM;

    m_lyricSize = DEFAULT_LYRIC_SIZE * PARAM_DENOMINATOR;
    m_hairpinSize = DEFAULT_HAIRPIN_SIZE * PARAM_DENOMINATOR;

    m_minMeasureWidth = (short)(DEFAULT_MEASURE_WIDTH * PARAM_DENOMINATOR);

    /** ties and slurs */
    m_tieThickness = DEFAULT_TIE_THICKNESS * PARAM_DENOMINATOR;
    m_minSlurHeight = DEFAULT_MIN_SLUR_HEIGHT * PARAM_DENOMINATOR;
    m_maxSlurHeight = DEFAULT_MAX_SLUR_HEIGHT * PARAM_DENOMINATOR;

    m_slurThickness = DEFAULT_SLUR_THICKNESS * PARAM_DENOMINATOR;

    /** The left spacing */
    m_leftPosition = DEFAULT_LEFT_POSITION * DEFINITION_FACTOR;

    /** The layout left margin by element */
    m_leftMarginAccid = DEFAULT_LEFT_MARGIN_ACCID * DEFINITION_FACTOR;
    m_leftMarginBarLine = DEFAULT_LEFT_MARGIN_BARLINE * DEFINITION_FACTOR;
    m_leftMarginBarLineAttrLeft = DEFAULT_LEFT_MARGIN_BARLINE_ATTR_LEFT * DEFINITION_FACTOR;
    m_leftMarginBarLineAttrRight = DEFAULT_LEFT_MARGIN_BARLINE_ATTR_RIGHT * DEFINITION_FACTOR;
    m_leftMarginBeatRpt = DEFAULT_LEFT_MARGIN_BEATRPT * DEFINITION_FACTOR;
    m_leftMarginChord = DEFAULT_LEFT_MARGIN_CHORD * DEFINITION_FACTOR;
    m_leftMarginClef = DEFAULT_LEFT_MARGIN_CLEF * DEFINITION_FACTOR;
    m_leftMarginKeySig = DEFAULT_LEFT_MARGIN_KEYSIG * DEFINITION_FACTOR;
    m_leftMarginMensur = DEFAULT_LEFT_MARGIN_MENSUR * DEFINITION_FACTOR;
    m_leftMarginMeterSig = DEFAULT_LEFT_MARGIN_METERSIG * DEFINITION_FACTOR;
    m_leftMarginMRest = DEFAULT_LEFT_MARGIN_MREST * DEFINITION_FACTOR;
    m_leftMarginMRpt2 = DEFAULT_LEFT_MARGIN_MRPT2 * DEFINITION_FACTOR;
    m_leftMarginMultiRest = DEFAULT_LEFT_MARGIN_MULTIREST * DEFINITION_FACTOR;
    m_leftMarginMultiRpt = DEFAULT_LEFT_MARGIN_MULTIRPT * DEFINITION_FACTOR;
    m_leftMarginNote = DEFAULT_LEFT_MARGIN_NOTE * DEFINITION_FACTOR;
    m_leftMarginRest = DEFAULT_LEFT_MARGIN_REST * DEFINITION_FACTOR;
    /** The default left margin */
    m_leftMarginDefault = DEFAULT_LEFT_MARGIN_DEFAULT * DEFINITION_FACTOR;

    /** The layout right margin by element */
    m_rightMarginAccid = DEFAULT_RIGHT_MARGIN_ACCID * DEFINITION_FACTOR;
    m_rightMarginBarLine = DEFAULT_RIGHT_MARGIN_BARLINE * DEFINITION_FACTOR;
    m_rightMarginBarLineAttrLeft = DEFAULT_RIGHT_MARGIN_BARLINE_ATTR_LEFT * DEFINITION_FACTOR;
    m_rightMarginBarLineAttrRight = DEFAULT_RIGHT_MARGIN_BARLINE_ATTR_RIGHT * DEFINITION_FACTOR;
    m_rightMarginBeatRpt = DEFAULT_RIGHT_MARGIN_BEATRPT * DEFINITION_FACTOR;
    m_rightMarginChord = DEFAULT_RIGHT_MARGIN_CHORD * DEFINITION_FACTOR;
    m_rightMarginClef = DEFAULT_RIGHT_MARGIN_CLEF * DEFINITION_FACTOR;
    m_rightMarginKeySig = DEFAULT_RIGHT_MARGIN_KEYSIG * DEFINITION_FACTOR;
    m_rightMarginMensur = DEFAULT_RIGHT_MARGIN_MENSUR * DEFINITION_FACTOR;
    m_rightMarginMeterSig = DEFAULT_RIGHT_MARGIN_METERSIG * DEFINITION_FACTOR;
    m_rightMarginMRest = DEFAULT_RIGHT_MARGIN_MREST * DEFINITION_FACTOR;
    m_rightMarginMRpt2 = DEFAULT_RIGHT_MARGIN_MRPT2 * DEFINITION_FACTOR;
    m_rightMarginMultiRest = DEFAULT_RIGHT_MARGIN_MULTIREST * DEFINITION_FACTOR;
    m_rightMarginMultiRpt = DEFAULT_RIGHT_MARGIN_MULTIRPT * DEFINITION_FACTOR;
    m_rightMarginNote = DEFAULT_RIGHT_MARGIN_NOTE * DEFINITION_FACTOR;
    m_rightMarginRest = DEFAULT_RIGHT_MARGIN_REST * DEFINITION_FACTOR;
    /** The default right margin */
    m_rightMarginDefault = DEFAULT_RIGHT_MARGIN_DEFAULT * DEFINITION_FACTOR;

    /** The default right margin */
    m_bottomMarginDefault = DEFAULT_BOTTOM_MARGIN_DEFAULT * DEFINITION_FACTOR;

    /** The default right margin */
    m_topMarginDefault = DEFAULT_TOP_MARGIN_DEFAULT * DEFINITION_FACTOR;
}

Style::~Style()
{
}

} // namespace vrv
/////////////////////////////////////////////////////////////////////////////
// Name:        musleipzigbbox.cpp
// Author:      Rodolfo Zitellini
// Created:     30/07/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "leipzigbbox.h"

//----------------------------------------------------------------------------

#include "vrvdef.h"

namespace vrv {

bool LeipzigBBox::m_initialized =  false;
/*
LeipzigBBox::BoundingBox* LeipzigBBox::m_bBox = LeipzigBBox::InitializeStatic();
*/


LeipzigBBox::BoundingBox LeipzigBBox::m_bBox[LEIPZIG_GLYPHS];

// Statically initialize the struct
void LeipzigBBox::InitializeStatic() {
        
    /*
     * This list is used for getting the bounding box of the Leipzig glyphs.
     * The values were obtained with ./varia/svg/split.xsl and boundingbox.svg.
     * It should not be modified by hand.
     */
    m_bBox[LEIPZIG_BBOX_ORN_MORDENT].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_ORN_MORDENT].m_y = -2.0;
    m_bBox[LEIPZIG_BBOX_ORN_MORDENT].m_width = 605.0;
    m_bBox[LEIPZIG_BBOX_ORN_MORDENT].m_height = 242.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_0].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_0].m_y = 0.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_0].m_width = 427.7;
    m_bBox[LEIPZIG_BBOX_FIGURE_0].m_height = 516.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_1].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_1].m_y = 0.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_1].m_width = 296.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_1].m_height = 496.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_2].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_2].m_y = -15.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_2].m_width = 402.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_2].m_height = 513.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_3].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_3].m_y = 0.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_3].m_width = 383.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_3].m_height = 515.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_4].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_4].m_y = 0.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_4].m_width = 387.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_4].m_height = 494.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_5].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_5].m_y = 0.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_5].m_width = 357.7;
    m_bBox[LEIPZIG_BBOX_FIGURE_5].m_height = 494.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_6].m_x = -1.3;
    m_bBox[LEIPZIG_BBOX_FIGURE_6].m_y = 0.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_6].m_width = 395.3;
    m_bBox[LEIPZIG_BBOX_FIGURE_6].m_height = 516.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_7].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_7].m_y = 0.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_7].m_width = 404.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_7].m_height = 511.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_8].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_8].m_y = 0.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_8].m_width = 386.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_8].m_height = 515.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_9].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_9].m_y = 0.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_9].m_width = 397.0;
    m_bBox[LEIPZIG_BBOX_FIGURE_9].m_height = 513.0;
    m_bBox[LEIPZIG_BBOX_FERMATA_UP].m_x = -376.0;
    m_bBox[LEIPZIG_BBOX_FERMATA_UP].m_y = -57.0;
    m_bBox[LEIPZIG_BBOX_FERMATA_UP].m_width = 753.0;
    m_bBox[LEIPZIG_BBOX_FERMATA_UP].m_height = 405.0;
    m_bBox[LEIPZIG_BBOX_FERMATA_DOWN].m_x = -376.0;
    m_bBox[LEIPZIG_BBOX_FERMATA_DOWN].m_y = -348.0;
    m_bBox[LEIPZIG_BBOX_FERMATA_DOWN].m_width = 753.0;
    m_bBox[LEIPZIG_BBOX_FERMATA_DOWN].m_height = 405.0;
    m_bBox[LEIPZIG_BBOX_ORN_TRILL].m_x = -216.0;
    m_bBox[LEIPZIG_BBOX_ORN_TRILL].m_y = -161.0;
    m_bBox[LEIPZIG_BBOX_ORN_TRILL].m_width = 432.0;
    m_bBox[LEIPZIG_BBOX_ORN_TRILL].m_height = 364.0;
    m_bBox[LEIPZIG_BBOX_METER_SYMB_2_CUT].m_x = -2.0;
    m_bBox[LEIPZIG_BBOX_METER_SYMB_2_CUT].m_y = -69.0;
    m_bBox[LEIPZIG_BBOX_METER_SYMB_2_CUT].m_width = 360.0;
    m_bBox[LEIPZIG_BBOX_METER_SYMB_2_CUT].m_height = 633.0;
    m_bBox[LEIPZIG_BBOX_METER_SYMB_CUT].m_x = 1.0;
    m_bBox[LEIPZIG_BBOX_METER_SYMB_CUT].m_y = -69.0;
    m_bBox[LEIPZIG_BBOX_METER_SYMB_CUT].m_width = 425.0;
    m_bBox[LEIPZIG_BBOX_METER_SYMB_CUT].m_height = 649.0;
    m_bBox[LEIPZIG_BBOX_HEAD_WHOLE].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_HEAD_WHOLE].m_y = -133.0;
    m_bBox[LEIPZIG_BBOX_HEAD_WHOLE].m_width = 405.0;
    m_bBox[LEIPZIG_BBOX_HEAD_WHOLE].m_height = 266.0;
    m_bBox[LEIPZIG_BBOX_HEAD_WHOLE_FILL].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_HEAD_WHOLE_FILL].m_y = -133.0;
    m_bBox[LEIPZIG_BBOX_HEAD_WHOLE_FILL].m_width = 405.0;
    m_bBox[LEIPZIG_BBOX_HEAD_WHOLE_FILL].m_height = 266.0;
    m_bBox[LEIPZIG_BBOX_HEAD_HALF].m_x = -15.0;
    m_bBox[LEIPZIG_BBOX_HEAD_HALF].m_y = -135.0;
    m_bBox[LEIPZIG_BBOX_HEAD_HALF].m_width = 314.0;
    m_bBox[LEIPZIG_BBOX_HEAD_HALF].m_height = 270.0;
    m_bBox[LEIPZIG_BBOX_HEAD_QUARTER].m_x = -15.0;
    m_bBox[LEIPZIG_BBOX_HEAD_QUARTER].m_y = -135.0;
    m_bBox[LEIPZIG_BBOX_HEAD_QUARTER].m_width = 314.0;
    m_bBox[LEIPZIG_BBOX_HEAD_QUARTER].m_height = 270.0;
    m_bBox[LEIPZIG_BBOX_SLASH_UP].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_SLASH_UP].m_y = -597.0;
    m_bBox[LEIPZIG_BBOX_SLASH_UP].m_width = 276.0;
    m_bBox[LEIPZIG_BBOX_SLASH_UP].m_height = 577.0;
    m_bBox[LEIPZIG_BBOX_SLASH_DOWN].m_x = 164.0;
    m_bBox[LEIPZIG_BBOX_SLASH_DOWN].m_y = 20.0;
    m_bBox[LEIPZIG_BBOX_SLASH_DOWN].m_width = 276.0;
    m_bBox[LEIPZIG_BBOX_SLASH_DOWN].m_height = 577.0;
    m_bBox[LEIPZIG_BBOX_CLEF_G].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_CLEF_G].m_y = -655.0;
    m_bBox[LEIPZIG_BBOX_CLEF_G].m_width = 679.0;
    m_bBox[LEIPZIG_BBOX_CLEF_G].m_height = 1809.0;
    m_bBox[LEIPZIG_BBOX_CLEF_F].m_x = 4.0;
    m_bBox[LEIPZIG_BBOX_CLEF_F].m_y = -566.0;
    m_bBox[LEIPZIG_BBOX_CLEF_F].m_width = 694.0;
    m_bBox[LEIPZIG_BBOX_CLEF_F].m_height = 832.0;
    m_bBox[LEIPZIG_BBOX_CLEF_C].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_CLEF_C].m_y = -536.0;
    m_bBox[LEIPZIG_BBOX_CLEF_C].m_width = 659.0;
    m_bBox[LEIPZIG_BBOX_CLEF_C].m_height = 1082.0;
    m_bBox[LEIPZIG_BBOX_CLEF_G8].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_CLEF_G8].m_y = -844.0;
    m_bBox[LEIPZIG_BBOX_CLEF_G8].m_width = 679.0;
    m_bBox[LEIPZIG_BBOX_CLEF_G8].m_height = 1998.0;
    m_bBox[LEIPZIG_BBOX_ALT_SHARP].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_ALT_SHARP].m_y = -364.0;
    m_bBox[LEIPZIG_BBOX_ALT_SHARP].m_width = 197.0;
    m_bBox[LEIPZIG_BBOX_ALT_SHARP].m_height = 746.0;
    m_bBox[LEIPZIG_BBOX_ALT_NATURAL].m_x = 20.0;
    m_bBox[LEIPZIG_BBOX_ALT_NATURAL].m_y = -377.0;
    m_bBox[LEIPZIG_BBOX_ALT_NATURAL].m_width = 157.0;
    m_bBox[LEIPZIG_BBOX_ALT_NATURAL].m_height = 754.0;
    m_bBox[LEIPZIG_BBOX_ALT_FLAT].m_x = 22.0;
    m_bBox[LEIPZIG_BBOX_ALT_FLAT].m_y = -184.0;
    m_bBox[LEIPZIG_BBOX_ALT_FLAT].m_width = 198.0;
    m_bBox[LEIPZIG_BBOX_ALT_FLAT].m_height = 678.0;
    m_bBox[LEIPZIG_BBOX_ALT_DOUBLE_SHARP].m_x = -20.0;
    m_bBox[LEIPZIG_BBOX_ALT_DOUBLE_SHARP].m_y = -114.0;
    m_bBox[LEIPZIG_BBOX_ALT_DOUBLE_SHARP].m_width = 271.0;
    m_bBox[LEIPZIG_BBOX_ALT_DOUBLE_SHARP].m_height = 228.0;
    m_bBox[LEIPZIG_BBOX_REST_4].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_REST_4].m_y = -146.0;
    m_bBox[LEIPZIG_BBOX_REST_4].m_width = 324.0;
    m_bBox[LEIPZIG_BBOX_REST_4].m_height = 808.0;
    m_bBox[LEIPZIG_BBOX_REST_8].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_REST_8].m_y = 0.0;
    m_bBox[LEIPZIG_BBOX_REST_8].m_width = 290.0;
    m_bBox[LEIPZIG_BBOX_REST_8].m_height = 455.0;
    m_bBox[LEIPZIG_BBOX_REST_16].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_REST_16].m_y = -249.0;
    m_bBox[LEIPZIG_BBOX_REST_16].m_width = 341.0;
    m_bBox[LEIPZIG_BBOX_REST_16].m_height = 702.0;
    m_bBox[LEIPZIG_BBOX_REST_32].m_x = 1.0;
    m_bBox[LEIPZIG_BBOX_REST_32].m_y = -248.0;
    m_bBox[LEIPZIG_BBOX_REST_32].m_width = 419.0;
    m_bBox[LEIPZIG_BBOX_REST_32].m_height = 946.0;
    m_bBox[LEIPZIG_BBOX_REST_64].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_REST_64].m_y = -248.0;
    m_bBox[LEIPZIG_BBOX_REST_64].m_width = 503.0;
    m_bBox[LEIPZIG_BBOX_REST_64].m_height = 1191.0;
    m_bBox[LEIPZIG_BBOX_REST_128].m_x = -1.0;
    m_bBox[LEIPZIG_BBOX_REST_128].m_y = -510.0;
    m_bBox[LEIPZIG_BBOX_REST_128].m_width = 602.0;
    m_bBox[LEIPZIG_BBOX_REST_128].m_height = 1485.0;
    m_bBox[LEIPZIG_BBOX_HEAD_WHOLE_DIAMOND].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_HEAD_WHOLE_DIAMOND].m_y = -211.0;
    m_bBox[LEIPZIG_BBOX_HEAD_WHOLE_DIAMOND].m_width = 359.0;
    m_bBox[LEIPZIG_BBOX_HEAD_WHOLE_DIAMOND].m_height = 445.0;
    m_bBox[LEIPZIG_BBOX_HEAD_WHOLE_FILLDIAMOND].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_HEAD_WHOLE_FILLDIAMOND].m_y = -211.0;
    m_bBox[LEIPZIG_BBOX_HEAD_WHOLE_FILLDIAMOND].m_width = 359.0;
    m_bBox[LEIPZIG_BBOX_HEAD_WHOLE_FILLDIAMOND].m_height = 445.0;
    m_bBox[LEIPZIG_BBOX_HEAD_HALF_DIAMOND].m_x = -34.0;
    m_bBox[LEIPZIG_BBOX_HEAD_HALF_DIAMOND].m_y = -211.0;
    m_bBox[LEIPZIG_BBOX_HEAD_HALF_DIAMOND].m_width = 358.0;
    m_bBox[LEIPZIG_BBOX_HEAD_HALF_DIAMOND].m_height = 445.0;
    m_bBox[LEIPZIG_BBOX_HEAD_QUARTER_FILLDIAMOND].m_x = -34.0;
    m_bBox[LEIPZIG_BBOX_HEAD_QUARTER_FILLDIAMOND].m_y = -211.0;
    m_bBox[LEIPZIG_BBOX_HEAD_QUARTER_FILLDIAMOND].m_width = 358.0;
    m_bBox[LEIPZIG_BBOX_HEAD_QUARTER_FILLDIAMOND].m_height = 445.0;
    m_bBox[LEIPZIG_BBOX_SLASH_UP_MENSURAL].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_SLASH_UP_MENSURAL].m_y = -237.0;
    m_bBox[LEIPZIG_BBOX_SLASH_UP_MENSURAL].m_width = 162.0;
    m_bBox[LEIPZIG_BBOX_SLASH_UP_MENSURAL].m_height = 237.0;
    m_bBox[LEIPZIG_BBOX_SLASH_DOWN_MENSURAL].m_x = 2.0;
    m_bBox[LEIPZIG_BBOX_SLASH_DOWN_MENSURAL].m_y = 6.0;
    m_bBox[LEIPZIG_BBOX_SLASH_DOWN_MENSURAL].m_width = 162.0;
    m_bBox[LEIPZIG_BBOX_SLASH_DOWN_MENSURAL].m_height = 237.0;
    m_bBox[LEIPZIG_BBOX_CLEF_G_MENSURAL].m_x = 145.3;
    m_bBox[LEIPZIG_BBOX_CLEF_G_MENSURAL].m_y = -180.7;
    m_bBox[LEIPZIG_BBOX_CLEF_G_MENSURAL].m_width = 461.7;
    m_bBox[LEIPZIG_BBOX_CLEF_G_MENSURAL].m_height = 936.3;
    m_bBox[LEIPZIG_BBOX_CLEF_F_MENSURAL].m_x = 109.0;
    m_bBox[LEIPZIG_BBOX_CLEF_F_MENSURAL].m_y = -753.0;
    m_bBox[LEIPZIG_BBOX_CLEF_F_MENSURAL].m_width = 286.0;
    m_bBox[LEIPZIG_BBOX_CLEF_F_MENSURAL].m_height = 1108.0;
    m_bBox[LEIPZIG_BBOX_CLEF_C_MENSURAL].m_x = 154.0;
    m_bBox[LEIPZIG_BBOX_CLEF_C_MENSURAL].m_y = -602.0;
    m_bBox[LEIPZIG_BBOX_CLEF_C_MENSURAL].m_width = 349.0;
    m_bBox[LEIPZIG_BBOX_CLEF_C_MENSURAL].m_height = 1197.0;
    m_bBox[LEIPZIG_BBOX_CLEF_G_CHIAVETTE].m_x = 13.0;
    m_bBox[LEIPZIG_BBOX_CLEF_G_CHIAVETTE].m_y = -273.0;
    m_bBox[LEIPZIG_BBOX_CLEF_G_CHIAVETTE].m_width = 619.0;
    m_bBox[LEIPZIG_BBOX_CLEF_G_CHIAVETTE].m_height = 1031.0;
    m_bBox[LEIPZIG_BBOX_ALT_SHARP_MENSURAL].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_ALT_SHARP_MENSURAL].m_y = -130.0;
    m_bBox[LEIPZIG_BBOX_ALT_SHARP_MENSURAL].m_width = 240.0;
    m_bBox[LEIPZIG_BBOX_ALT_SHARP_MENSURAL].m_height = 260.0;
    m_bBox[LEIPZIG_BBOX_ALT_NATURAL_MENSURAL].m_x = 30.0;
    m_bBox[LEIPZIG_BBOX_ALT_NATURAL_MENSURAL].m_y = -334.0;
    m_bBox[LEIPZIG_BBOX_ALT_NATURAL_MENSURAL].m_width = 134.0;
    m_bBox[LEIPZIG_BBOX_ALT_NATURAL_MENSURAL].m_height = 713.0;
    m_bBox[LEIPZIG_BBOX_ALT_FLAT_MENSURAL].m_x = 30.0;
    m_bBox[LEIPZIG_BBOX_ALT_FLAT_MENSURAL].m_y = -112.0;
    m_bBox[LEIPZIG_BBOX_ALT_FLAT_MENSURAL].m_width = 177.0;
    m_bBox[LEIPZIG_BBOX_ALT_FLAT_MENSURAL].m_height = 641.0;
    m_bBox[LEIPZIG_BBOX_ALT_DOUBLE_SHARP_MENSURAL].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_ALT_DOUBLE_SHARP_MENSURAL].m_y = -114.0;
    m_bBox[LEIPZIG_BBOX_ALT_DOUBLE_SHARP_MENSURAL].m_width = 271.0;
    m_bBox[LEIPZIG_BBOX_ALT_DOUBLE_SHARP_MENSURAL].m_height = 228.0;
    m_bBox[LEIPZIG_BBOX_REST_4_MENSURAL].m_x = 80.0;
    m_bBox[LEIPZIG_BBOX_REST_4_MENSURAL].m_y = 0.0;
    m_bBox[LEIPZIG_BBOX_REST_4_MENSURAL].m_width = 170.0;
    m_bBox[LEIPZIG_BBOX_REST_4_MENSURAL].m_height = 170.0;
    m_bBox[LEIPZIG_BBOX_REST_8_MENSURAL].m_x = 80.0;
    m_bBox[LEIPZIG_BBOX_REST_8_MENSURAL].m_y = 0.0;
    m_bBox[LEIPZIG_BBOX_REST_8_MENSURAL].m_width = 170.0;
    m_bBox[LEIPZIG_BBOX_REST_8_MENSURAL].m_height = 170.0;
    m_bBox[LEIPZIG_BBOX_REST_16_MENSURAL].m_x = 80.0;
    m_bBox[LEIPZIG_BBOX_REST_16_MENSURAL].m_y = 0.0;
    m_bBox[LEIPZIG_BBOX_REST_16_MENSURAL].m_width = 162.0;
    m_bBox[LEIPZIG_BBOX_REST_16_MENSURAL].m_height = 262.0;
    m_bBox[LEIPZIG_BBOX_REST_32_MENSURAL].m_x = 80.0;
    m_bBox[LEIPZIG_BBOX_REST_32_MENSURAL].m_y = 0.0;
    m_bBox[LEIPZIG_BBOX_REST_32_MENSURAL].m_width = 162.0;
    m_bBox[LEIPZIG_BBOX_REST_32_MENSURAL].m_height = 362.0;
    m_bBox[LEIPZIG_BBOX_REST_64_MENSURAL].m_x = 80.0;
    m_bBox[LEIPZIG_BBOX_REST_64_MENSURAL].m_y = 0.0;
    m_bBox[LEIPZIG_BBOX_REST_64_MENSURAL].m_width = 162.0;
    m_bBox[LEIPZIG_BBOX_REST_64_MENSURAL].m_height = 461.0;
    m_bBox[LEIPZIG_BBOX_REST_128_MENSURAL].m_x = 80.0;
    m_bBox[LEIPZIG_BBOX_REST_128_MENSURAL].m_y = 0.0;
    m_bBox[LEIPZIG_BBOX_REST_128_MENSURAL].m_width = 162.0;
    m_bBox[LEIPZIG_BBOX_REST_128_MENSURAL].m_height = 558.0;
    m_bBox[LEIPZIG_BBOX_METER_SYMB_3_CUT].m_x = 1.0;
    m_bBox[LEIPZIG_BBOX_METER_SYMB_3_CUT].m_y = -61.0;
    m_bBox[LEIPZIG_BBOX_METER_SYMB_3_CUT].m_width = 336.0;
    m_bBox[LEIPZIG_BBOX_METER_SYMB_3_CUT].m_height = 634.0;
    m_bBox[LEIPZIG_BBOX_METER_SYMB_COMMON].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_METER_SYMB_COMMON].m_y = 0.0;
    m_bBox[LEIPZIG_BBOX_METER_SYMB_COMMON].m_width = 427.0;
    m_bBox[LEIPZIG_BBOX_METER_SYMB_COMMON].m_height = 516.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_8].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_8].m_y = -171.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_8].m_width = 274.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_8].m_height = 326.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_1].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_1].m_y = -156.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_1].m_width = 207.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_1].m_height = 311.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_6].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_6].m_y = -171.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_6].m_width = 214.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_6].m_height = 326.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_0].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_0].m_y = -171.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_0].m_width = 243.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_0].m_height = 326.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_2].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_2].m_y = -171.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_2].m_width = 260.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_2].m_height = 326.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_4].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_4].m_y = -156.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_4].m_width = 259.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_4].m_height = 311.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_5].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_5].m_y = -171.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_5].m_width = 240.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_5].m_height = 326.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_3].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_3].m_y = -171.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_3].m_width = 255.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_3].m_height = 326.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_7].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_7].m_y = -156.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_7].m_width = 243.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_7].m_height = 311.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_9].m_x = 0.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_9].m_y = -172.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_9].m_width = 225.0;
    m_bBox[LEIPZIG_BBOX_OBLIQUE_FIGURE_9].m_height = 327.0;
    /* end of the generated data */
    
    m_initialized = true;
    
    return;
};

void LeipzigBBox::GetCharBounds(const unsigned char c, int *x, int *y, int *w, int *h)
{
    unsigned int glyph;
    
    if(!m_initialized)
        InitializeStatic();
    
    switch (c) {
            /* figures */
        case 48: glyph = LEIPZIG_BBOX_FIGURE_0; break;
        case 49: glyph = LEIPZIG_BBOX_FIGURE_1; break;
        case 50: glyph = LEIPZIG_BBOX_FIGURE_2; break;
        case 51: glyph = LEIPZIG_BBOX_FIGURE_3; break;
        case 52: glyph = LEIPZIG_BBOX_FIGURE_4; break;
        case 53: glyph = LEIPZIG_BBOX_FIGURE_5; break;
        case 54: glyph = LEIPZIG_BBOX_FIGURE_6; break;
        case 55: glyph = LEIPZIG_BBOX_FIGURE_7; break;
        case 56: glyph = LEIPZIG_BBOX_FIGURE_8; break;
        case 57: glyph = LEIPZIG_BBOX_FIGURE_9; break;
            /* oblique figures */
        case 0x82: glyph = LEIPZIG_BBOX_OBLIQUE_FIGURE_0; break;
        case 0x83: glyph = LEIPZIG_BBOX_OBLIQUE_FIGURE_1; break;
        case 0x84: glyph = LEIPZIG_BBOX_OBLIQUE_FIGURE_2; break;
        case 0x85: glyph = LEIPZIG_BBOX_OBLIQUE_FIGURE_3; break;
        case 0x86: glyph = LEIPZIG_BBOX_OBLIQUE_FIGURE_4; break;
        case 0x87: glyph = LEIPZIG_BBOX_OBLIQUE_FIGURE_5; break;
        case 0x88: glyph = LEIPZIG_BBOX_OBLIQUE_FIGURE_6; break;
        case 0x89: glyph = LEIPZIG_BBOX_OBLIQUE_FIGURE_7; break;
        case 0x8A: glyph = LEIPZIG_BBOX_OBLIQUE_FIGURE_8; break;
        case 0x8B: glyph = LEIPZIG_BBOX_OBLIQUE_FIGURE_9; break;
            /* fermata */
        case 0x3F: glyph = LEIPZIG_BBOX_FERMATA_UP; break;
        case 0x40: glyph = LEIPZIG_BBOX_FERMATA_DOWN; break;        
            /* clef */
        case LEIPZIG_CLEF_G: glyph = LEIPZIG_BBOX_CLEF_G; break;
        case LEIPZIG_CLEF_F: glyph = LEIPZIG_BBOX_CLEF_F; break;
        case LEIPZIG_CLEF_C: glyph = LEIPZIG_BBOX_CLEF_C; break;
        case LEIPZIG_CLEF_8va: glyph = LEIPZIG_BBOX_CLEF_G8; break;
        case LEIPZIG_CLEF_G + LEIPZIG_OFFSET_MENSURAL: glyph = LEIPZIG_BBOX_CLEF_G_MENSURAL; break;
        case LEIPZIG_CLEF_F + LEIPZIG_OFFSET_MENSURAL: glyph = LEIPZIG_BBOX_CLEF_F_MENSURAL; break;
        case LEIPZIG_CLEF_C + LEIPZIG_OFFSET_MENSURAL: glyph = LEIPZIG_BBOX_CLEF_C_MENSURAL; break;
        case LEIPZIG_CLEF_8va + LEIPZIG_OFFSET_MENSURAL: glyph = LEIPZIG_BBOX_CLEF_G_MENSURAL; break; // ??
            /* meter */
        case LEIPZIG_METER_SYMB_2_CUT: glyph = LEIPZIG_BBOX_METER_SYMB_2_CUT; break;
        case LEIPZIG_METER_SYMB_3_CUT: glyph = LEIPZIG_BBOX_METER_SYMB_3_CUT; break;
        case LEIPZIG_METER_SYMB_CUT: glyph = LEIPZIG_BBOX_METER_SYMB_CUT; break;
        case LEIPZIG_METER_SYMB_COMMON: glyph = LEIPZIG_BBOX_METER_SYMB_COMMON; break;        
            /* alterations */
        case LEIPZIG_ACCID_SHARP: glyph = LEIPZIG_BBOX_ALT_SHARP; break;
        case LEIPZIG_ACCID_NATURAL: glyph = LEIPZIG_BBOX_ALT_NATURAL; break;
        case LEIPZIG_ACCID_FLAT: glyph = LEIPZIG_BBOX_ALT_FLAT; break;
        case LEIPZIG_ACCID_DOUBLE_SHARP: glyph = LEIPZIG_BBOX_ALT_DOUBLE_SHARP; break;
        case LEIPZIG_ACCID_SHARP + LEIPZIG_OFFSET_MENSURAL: glyph = LEIPZIG_BBOX_ALT_SHARP_MENSURAL; break;
        case LEIPZIG_ACCID_NATURAL + LEIPZIG_OFFSET_MENSURAL: glyph = LEIPZIG_BBOX_ALT_NATURAL_MENSURAL; break;
        case LEIPZIG_ACCID_FLAT + LEIPZIG_OFFSET_MENSURAL: glyph = LEIPZIG_BBOX_ALT_FLAT_MENSURAL; break;
        case LEIPZIG_ACCID_DOUBLE_SHARP + LEIPZIG_OFFSET_MENSURAL: glyph = LEIPZIG_BBOX_ALT_DOUBLE_SHARP_MENSURAL; break;
            /* rests */
        case LEIPZIG_REST_QUARTER: glyph = LEIPZIG_BBOX_REST_4; break;
        case LEIPZIG_REST_QUARTER + 1: glyph = LEIPZIG_BBOX_REST_8; break;
        case LEIPZIG_REST_QUARTER + 2: glyph = LEIPZIG_BBOX_REST_16; break;
        case LEIPZIG_REST_QUARTER + 3: glyph = LEIPZIG_BBOX_REST_32; break;
        case LEIPZIG_REST_QUARTER + 4: glyph = LEIPZIG_BBOX_REST_64; break;
        case LEIPZIG_REST_QUARTER + 5: glyph = LEIPZIG_BBOX_REST_128; break;
        case LEIPZIG_REST_QUARTER + LEIPZIG_OFFSET_MENSURAL: glyph = LEIPZIG_BBOX_REST_4_MENSURAL; break;
        case LEIPZIG_REST_QUARTER + 1 + LEIPZIG_OFFSET_MENSURAL: glyph = LEIPZIG_BBOX_REST_8_MENSURAL; break;
        case LEIPZIG_REST_QUARTER + 2 + LEIPZIG_OFFSET_MENSURAL: glyph = LEIPZIG_BBOX_REST_16_MENSURAL; break;
        case LEIPZIG_REST_QUARTER + 3 + LEIPZIG_OFFSET_MENSURAL: glyph = LEIPZIG_BBOX_REST_32_MENSURAL; break;
        case LEIPZIG_REST_QUARTER + 4 + LEIPZIG_OFFSET_MENSURAL: glyph = LEIPZIG_BBOX_REST_64_MENSURAL; break;
        case LEIPZIG_REST_QUARTER + 5 + LEIPZIG_OFFSET_MENSURAL: glyph = LEIPZIG_BBOX_REST_128_MENSURAL; break;
            /* note heads */
        case LEIPZIG_HEAD_WHOLE: glyph = LEIPZIG_BBOX_HEAD_WHOLE; break;
        case LEIPZIG_HEAD_WHOLE_FILLED: glyph = LEIPZIG_BBOX_HEAD_WHOLE_FILL; break;
        case LEIPZIG_HEAD_HALF: glyph = LEIPZIG_BBOX_HEAD_HALF; break;
        case LEIPZIG_HEAD_QUARTER: glyph = LEIPZIG_BBOX_HEAD_QUARTER; break;
        case LEIPZIG_HEAD_WHOLE + LEIPZIG_OFFSET_MENSURAL: glyph = LEIPZIG_BBOX_HEAD_WHOLE_DIAMOND; break;
        case LEIPZIG_HEAD_WHOLE_FILLED + LEIPZIG_OFFSET_MENSURAL: glyph = LEIPZIG_BBOX_HEAD_WHOLE_FILLDIAMOND; break;
        case LEIPZIG_HEAD_HALF + LEIPZIG_OFFSET_MENSURAL: glyph = LEIPZIG_BBOX_HEAD_HALF_DIAMOND; break;
        case LEIPZIG_HEAD_QUARTER + LEIPZIG_OFFSET_MENSURAL: glyph = LEIPZIG_BBOX_HEAD_QUARTER_FILLDIAMOND; break;
            /* slashes */
        case LEIPZIG_STEM_FLAG_UP: glyph = LEIPZIG_BBOX_SLASH_UP; break;
        case LEIPZIG_STEM_FLAG_DOWN: glyph = LEIPZIG_BBOX_SLASH_DOWN; break;
        case LEIPZIG_STEM_FLAG_UP + LEIPZIG_OFFSET_MENSURAL: glyph = LEIPZIG_BBOX_SLASH_UP_MENSURAL; break;
        case LEIPZIG_STEM_FLAG_DOWN + LEIPZIG_OFFSET_MENSURAL: glyph = LEIPZIG_BBOX_SLASH_DOWN_MENSURAL; break;
            /* ornaments */
        case 35: glyph = LEIPZIG_BBOX_ORN_MORDENT; break;
        case LEIPZIG_EMB_TRILL: glyph = LEIPZIG_BBOX_ORN_TRILL; break;
            /* todo */
        default: glyph = LEIPZIG_BBOX_ALT_DOUBLE_SHARP; // ??
    }
    
    *w = m_bBox[glyph].m_width;
    *h = m_bBox[glyph].m_height;
    *x = m_bBox[glyph].m_x;
    *y = m_bBox[glyph].m_y;
     
    
    //bBox = 0;
}

} // namespace vrv
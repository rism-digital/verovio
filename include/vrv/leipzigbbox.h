/////////////////////////////////////////////////////////////////////////////
// Name:        leipzigbbox.h
// Author:      Rodolfo Zitellini
// Created:     30/07/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_LEIPZIGBBOX_H__
#define __VRV_LEIPZIGBBOX_H__

#include "object.h"

namespace vrv {

/*
 * This list is used for getting the bounding box of the Leipzig glyphs.
 * The values were obtained with ./varia/svg/split.xsl and boundingbox.svg.
 * It should not be modified by hand.
 */
#define LEIPZIG_BBOX_ORN_MORDENT 0
#define LEIPZIG_BBOX_FIGURE_0 1
#define LEIPZIG_BBOX_FIGURE_1 2
#define LEIPZIG_BBOX_FIGURE_2 3
#define LEIPZIG_BBOX_FIGURE_3 4
#define LEIPZIG_BBOX_FIGURE_4 5
#define LEIPZIG_BBOX_FIGURE_5 6
#define LEIPZIG_BBOX_FIGURE_6 7
#define LEIPZIG_BBOX_FIGURE_7 8
#define LEIPZIG_BBOX_FIGURE_8 9
#define LEIPZIG_BBOX_FIGURE_9 10
#define LEIPZIG_BBOX_FERMATA_UP 11
#define LEIPZIG_BBOX_FERMATA_DOWN 12
#define LEIPZIG_BBOX_ORN_TRILL 13
#define LEIPZIG_BBOX_METER_SYMB_2_CUT 14
#define LEIPZIG_BBOX_METER_SYMB_CUT 15
#define LEIPZIG_BBOX_HEAD_WHOLE 16
#define LEIPZIG_BBOX_HEAD_WHOLE_FILL 17
#define LEIPZIG_BBOX_HEAD_HALF 18
#define LEIPZIG_BBOX_HEAD_QUARTER 19
#define LEIPZIG_BBOX_SLASH_UP 20
#define LEIPZIG_BBOX_SLASH_DOWN 21
#define LEIPZIG_BBOX_CLEF_G 22
#define LEIPZIG_BBOX_CLEF_F 23
#define LEIPZIG_BBOX_CLEF_C 24
#define LEIPZIG_BBOX_CLEF_G8 25
#define LEIPZIG_BBOX_ALT_SHARP 26
#define LEIPZIG_BBOX_ALT_NATURAL 27
#define LEIPZIG_BBOX_ALT_FLAT 28
#define LEIPZIG_BBOX_ALT_DOUBLE_SHARP 29
#define LEIPZIG_BBOX_REST_4 30
#define LEIPZIG_BBOX_REST_8 31
#define LEIPZIG_BBOX_REST_16 32
#define LEIPZIG_BBOX_REST_32 33
#define LEIPZIG_BBOX_REST_64 34
#define LEIPZIG_BBOX_REST_128 35
#define LEIPZIG_BBOX_HEAD_WHOLE_DIAMOND 36
#define LEIPZIG_BBOX_HEAD_WHOLE_FILLDIAMOND 37
#define LEIPZIG_BBOX_HEAD_HALF_DIAMOND 38
#define LEIPZIG_BBOX_HEAD_QUARTER_FILLDIAMOND 39
#define LEIPZIG_BBOX_SLASH_UP_MENSURAL 40
#define LEIPZIG_BBOX_SLASH_DOWN_MENSURAL 41
#define LEIPZIG_BBOX_CLEF_G_MENSURAL 42
#define LEIPZIG_BBOX_CLEF_F_MENSURAL 43
#define LEIPZIG_BBOX_CLEF_C_MENSURAL 44
#define LEIPZIG_BBOX_CLEF_G_CHIAVETTE 45
#define LEIPZIG_BBOX_ALT_SHARP_MENSURAL 46
#define LEIPZIG_BBOX_ALT_NATURAL_MENSURAL 47
#define LEIPZIG_BBOX_ALT_FLAT_MENSURAL 48
#define LEIPZIG_BBOX_ALT_DOUBLE_SHARP_MENSURAL 49
#define LEIPZIG_BBOX_REST_4_MENSURAL 50
#define LEIPZIG_BBOX_REST_8_MENSURAL 51
#define LEIPZIG_BBOX_REST_16_MENSURAL 52
#define LEIPZIG_BBOX_REST_32_MENSURAL 53
#define LEIPZIG_BBOX_REST_64_MENSURAL 54
#define LEIPZIG_BBOX_REST_128_MENSURAL 55
#define LEIPZIG_BBOX_METER_SYMB_3_CUT 56
#define LEIPZIG_BBOX_METER_SYMB_COMMON 57
#define LEIPZIG_BBOX_OBLIQUE_FIGURE_8 58
#define LEIPZIG_BBOX_OBLIQUE_FIGURE_1 59
#define LEIPZIG_BBOX_OBLIQUE_FIGURE_6 60
#define LEIPZIG_BBOX_OBLIQUE_FIGURE_0 61
#define LEIPZIG_BBOX_OBLIQUE_FIGURE_2 62
#define LEIPZIG_BBOX_OBLIQUE_FIGURE_4 63
#define LEIPZIG_BBOX_OBLIQUE_FIGURE_5 64
#define LEIPZIG_BBOX_OBLIQUE_FIGURE_3 65
#define LEIPZIG_BBOX_OBLIQUE_FIGURE_7 66
#define LEIPZIG_BBOX_OBLIQUE_FIGURE_9 67
#define LEIPZIG_GLYPHS 68
/* end of the generated data */


/**
 * This class is used for getting the bounding box of the Leipzig glyphs.
 * The values (above) are used obtained with the ./varia/svg/split.xsl.
 */
class LeipzigBBox
{
public:
    
    // Get the bounds of a char in the array
    static void GetCharBounds(const unsigned char c, int *x, int *y, int *w, int *h);
    
    
private:
    typedef struct _BoundingBox
    {
        double m_x;
        double m_y;
        double m_width;
        double m_height;
    } BoundingBox;
    
    static void InitializeStatic();
  
public:

    
private:
    
    static BoundingBox m_bBox[LEIPZIG_GLYPHS];
    
    static bool m_initialized;
};


} // namespace vrv
    
#endif

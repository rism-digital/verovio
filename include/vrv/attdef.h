/////////////////////////////////////////////////////////////////////////////
// Name:        attdef
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __ATT_DEF_H__
#define __ATT_DEF_H__

#include <string>
#include <utility>
#include <vector>

//----------------------------------------------------------------------------

namespace vrv {
    
typedef std::vector<std::pair<std::string, std::string> >  ArrayOfStrAttr;
    
#define VRV_UNSET -0x7FFFFFFF
    
    
//----------------------------------------------------------------------------
// Durations
//----------------------------------------------------------------------------
 
/**
 * These duration values are used for internal calculation and differ from the 
 * MEI data.DURATION types (see below)
 */
#define DUR_MX -1  // maxima
#define DUR_LG 0  // longa
#define DUR_BR 1  // brevis
#define DUR_1 2   // whole note
#define DUR_2 3   // ...
#define DUR_4 4
#define DUR_8 5
#define DUR_16 6
#define DUR_32 7
#define DUR_64 8
#define DUR_128 9
#define DUR_256 10  // this is it for now
// used for alignement
#define DUR_MAX 1024
// mensural duration
#define DUR_MENSURAL_OFFSET (2 * DUR_MAX)
#define DUR_MENSURAL_MASK (2 * DUR_MAX -1)
// used for mensural alignment
#define DUR_MENSURAL_REF 1728
    
//----------------------------------------------------------------------------
// MEI data defines
//----------------------------------------------------------------------------
 
/**
 * MEI data.ACCIDENTAL.EXPLICIT
 */
enum data_ACCIDENTAL_EXPLICIT {
    ACCIDENTAL_EXPLICIT_NONE = 0,
    ACCIDENTAL_EXPLICIT_s,
    ACCIDENTAL_EXPLICIT_f,
    ACCIDENTAL_EXPLICIT_ss,
    ACCIDENTAL_EXPLICIT_x,
    ACCIDENTAL_EXPLICIT_ff,
    ACCIDENTAL_EXPLICIT_xs,
    ACCIDENTAL_EXPLICIT_ts,
    ACCIDENTAL_EXPLICIT_tf,
    ACCIDENTAL_EXPLICIT_n,
    ACCIDENTAL_EXPLICIT_nf,
    ACCIDENTAL_EXPLICIT_ns,
    ACCIDENTAL_EXPLICIT_su,
    ACCIDENTAL_EXPLICIT_sd,
    ACCIDENTAL_EXPLICIT_fu,
    ACCIDENTAL_EXPLICIT_fd,
    ACCIDENTAL_EXPLICIT_nu,
    ACCIDENTAL_EXPLICIT_nd
};
    
/**
 * MEI data.ACCIDENTAL.IMPLICIT
 */
enum data_ACCIDENTAL_IMPLICIT  {
    ACCIDENTAL_IMPLICIT_NONE = 0,
    ACCIDENTAL_IMPLICIT_s,
    ACCIDENTAL_IMPLICIT_f,
    ACCIDENTAL_IMPLICIT_ss,
    ACCIDENTAL_IMPLICIT_ff,
    ACCIDENTAL_IMPLICIT_n,
    ACCIDENTAL_IMPLICIT_su,
    ACCIDENTAL_IMPLICIT_sd,
    ACCIDENTAL_IMPLICIT_fu,
    ACCIDENTAL_IMPLICIT_fd
};

/**
 * MEI data_BARRENDITION
 */
enum data_BARRENDITION {
    BARRENDITION_NONE = 0,
    BARRENDITION_dashed,
    BARRENDITION_dotted,
    BARRENDITION_dbl,
    BARRENDITION_dbldashed,
    BARRENDITION_dbldotted,
    BARRENDITION_end,
    BARRENDITION_invis,
    BARRENDITION_rptstart,
    BARRENDITION_rptboth,
    BARRENDITION_rptend,
    BARRENDITION_single
};
    
/**
 * MEI data.BOOEAN
 */
enum data_BOOLEAN {
    BOOLEAN_NONE = 0,
    BOOLEAN_true,
    BOOLEAN_false
};
    
/**
 * MEI data.CLEFSHAPE
 */
enum data_CLEFSHAPE {
    CLEFSHAPE_NONE = 0,
    CLEFSHAPE_G,
    CLEFSHAPE_GG,
    CLEFSHAPE_F,
    CLEFSHAPE_C,
    CLEFSHAPE_perc,
    CLEFSHAPE_TAB
};

/**
 * MEI values for @con (no datatype in MEI)
 */
enum data_CON {
    CON_NONE = 0,
    CON_s,
    CON_d,
    CON_u,
    CON_t,
    CON_c,
    CON_v,
    CON_i,
    CON_b
};

/**
 * MEI data.DURATION
 */
enum data_DURATION {
    DURATION_NONE = VRV_UNSET,
    DURATION_long = DUR_LG,
    DURATION_breve,
    DURATION_1,
    DURATION_2,
    DURATION_4,
    DURATION_8,
    DURATION_16,
    DURATION_32,
    DURATION_64,
    DURATION_128,
    DURATION_256,
    DURATION_512,
    DURATION_1024,
    DURATION_2048,
    DURATION_maxima = DUR_MX,
    DURATION_longa = DUR_MENSURAL_OFFSET + DUR_LG,
    DURATION_brevis,
    DURATION_semibrevis,
    DURATION_minima,
    DURATION_semiminima,
    DURATION_fusa,
    DURATION_semifusa
};
    
/**
 * MEI data.FONTSTYLE
 */
enum data_FONTSTYLE {
    FONTSTYLE_NONE = 0,
    FONTSTYLE_italic,
    FONTSTYLE_normal,
    FONTSTYLE_oblique
};
    
/**
 * MEI data.FONTWEIGHT
 */
enum data_FONTWEIGHT {
    FONTWEIGHT_NONE = 0,
    FONTWEIGHT_bold,
    FONTWEIGHT_normal
};
    
/**
 * MEI data.KEYSIGNATURE
 * The maximum is 255 (unsigned char)
 * Order for 7f to 7s should not be changes. This is a special case since we use abs()
 * to get the number of flats or sharps
 */
enum data_KEYSIGNATURE {
    KEYSIGNATURE_NONE = 0,
    KEYSIGNATURE_7f,
    KEYSIGNATURE_6f,
    KEYSIGNATURE_5f,
    KEYSIGNATURE_4f,
    KEYSIGNATURE_3f,
    KEYSIGNATURE_2f,
    KEYSIGNATURE_1f,
    KEYSIGNATURE_0,
    KEYSIGNATURE_1s,
    KEYSIGNATURE_2s,
    KEYSIGNATURE_3s,
    KEYSIGNATURE_4s,
    KEYSIGNATURE_5s,
    KEYSIGNATURE_6s,
    KEYSIGNATURE_7s,
    KEYSIGNATURE_mixed
};

/**
 * MEI data.LAYERSCHEME
 */
enum data_LAYERSCHEME {
    LAYERSCHEME_NONE = 0,
    LAYERSCHEME_1,
    LAYERSCHEME_2o,
    LAYERSCHEME_2f,
    LAYERSCHEME_3o,
    LAYERSCHEME_3f
};

/**
 * MEI values for @lig (no datatype in MEI)
 */
enum data_LIGATURE {
    LIGATURE_NONE = 0,
    LIGATURE_recta,
    LIGATURE_obliqua
};
    
/**
 * MEI data.METERSIGN
 */
enum data_METERSIGN {
    METERSIGN_NONE = 0,
    METERSIGN_common,
    METERSIGN_cut
};

/**
 * MEI data.MENSURATIONSIGN
 */
enum data_MENSURATIONSIGN {
    MENSURATIONSIGN_NONE = 0,
    MENSURATIONSIGN_C,
    MENSURATIONSIGN_O
};

/**
 * MEI data.MODE
 */
enum data_MODE {
    MODE_NONE = 0,
    MODE_major,
    MODE_minor,
    MODE_dorian,
    MODE_phrygian,
    MODE_lydian,
    MODE_mixolydian,
    MODE_aeolian,
    MODE_locrian
};

/**
 * MEI data.MODUSMAIOR
 * NONE is -3 for perfect value (abs) by default
 */
enum data_MODUSMAIOR {
    MODUSMAIOR_NONE = -3,
    MODUSMAIOR_2 = 2,
    MODUSMAIOR_3
};
    
/**
 * MEI data.MODUSMINOR
 * NONE is -3 for perfect value (abs) by default
 */
enum data_MODUSMINOR {
    MODUSMINOR_NONE = -3,
    MODUSMINOR_2 = 2,
    MODUSMINOR_3
};
    
/**
 * MEI data.OCTAVE.DIS
 */
enum data_OCTAVE_DIS {
    OCTAVE_DIS_NONE = 0,
    OCTAVE_DIS_8 = 8,
    OCTAVE_DIS_15 = 15,
    OCTAVE_DIS_22 = 22
};
    
/**
 * MEI data.PITCHNAME
 */
enum data_PITCHNAME {
    PITCHNAME_NONE = 0,
    PITCHNAME_c,
    PITCHNAME_d,
    PITCHNAME_e,
    PITCHNAME_f,
    PITCHNAME_g,
    PITCHNAME_a,
    PITCHNAME_b,
};
    
/**
 * MEI data.PROLATIO
 * NONE is -3 for perfect value (abs) by default
 */
enum data_PROLATIO {
    PROLATIO_NONE = -3,
    PROLATIO_2 = 2,
    PROLATIO_3
};

/**
 * MEI data.ORIENTATION
 */
enum data_ORIENTATION {
    ORIENTATION_NONE = 0,
    ORIENTATION_reversed,
    ORIENTATION_90CW,
    ORIENTATION_90CCW
};
    
/**
 * MEI data.PLACE
 */
enum data_PLACE {
    PLACE_NONE = 0,
    PLACE_above,
    PLACE_below
};

/**
 * MEI data.STEMDIRECTION
 */
enum data_STEMDIRECTION {
    STEMDIRECTION_NONE = 0,
    STEMDIRECTION_up,
    STEMDIRECTION_down
};

/**
 * MEI data.STEMPOSITION
 */
enum data_STEMPOSITION {
    STEMPOSITION_NONE = 0,
    STEMPOSITION_left,
    STEMPOSITION_right,
    STEMPOSITION_center
};
    
/**
 * MEI data.TEMPUS
 * NONE is -3 for perfect value (abs) by default
 */
enum data_TEMPUS {
    TEMPUS_NONE = -3,
    TEMPUS_2 = 2,
    TEMPUS_3
};
    
/**
 * MEI data.TIE
 */
enum data_TIE {
    TIE_NONE = 0,
    TIE_i,
    TIE_m,
    TIE_t
};
        
/**
 * MEI values for @wordpos (no datatype in MEI)
 */
enum data_WORDPOS {
    WORDPOS_NONE = 0,
    WORDPOS_i,
    WORDPOS_m,
    WORDPOS_t
};
    
typedef std::pair<int, double> typedef_TSTAMP2;
    
} // namespace vrv

#endif // __ATT_DEF_H__

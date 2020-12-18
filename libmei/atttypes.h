/////////////////////////////////////////////////////////////////////////////
// Authors:     Laurent Pugin and Rodolfo Zitellini
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
//
// Code generated using a modified version of libmei
// by Andrew Hankinson, Alastair Porter, and Others
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// NOTE: this file was generated with the Verovio libmei version and
// should not be edited because changes will be lost.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ATT_TYPES_H__
#define __VRV_ATT_TYPES_H__

//----------------------------------------------------------------------------

namespace vrv {

/**
 * MEI data.ACCIDENTAL.GESTURAL
 */
enum data_ACCIDENTAL_GESTURAL {
    ACCIDENTAL_GESTURAL_NONE = 0,
    ACCIDENTAL_GESTURAL_s,
    ACCIDENTAL_GESTURAL_f,
    ACCIDENTAL_GESTURAL_ss,
    ACCIDENTAL_GESTURAL_ff,
    ACCIDENTAL_GESTURAL_n,
    ACCIDENTAL_GESTURAL_su,
    ACCIDENTAL_GESTURAL_sd,
    ACCIDENTAL_GESTURAL_fu,
    ACCIDENTAL_GESTURAL_fd,
    ACCIDENTAL_GESTURAL_MAX
};

/**
 * MEI data.ACCIDENTAL.WRITTEN
 */
enum data_ACCIDENTAL_WRITTEN {
    ACCIDENTAL_WRITTEN_NONE = 0,
    ACCIDENTAL_WRITTEN_s,
    ACCIDENTAL_WRITTEN_f,
    ACCIDENTAL_WRITTEN_ss,
    ACCIDENTAL_WRITTEN_x,
    ACCIDENTAL_WRITTEN_ff,
    ACCIDENTAL_WRITTEN_xs,
    ACCIDENTAL_WRITTEN_sx,
    ACCIDENTAL_WRITTEN_ts,
    ACCIDENTAL_WRITTEN_tf,
    ACCIDENTAL_WRITTEN_n,
    ACCIDENTAL_WRITTEN_nf,
    ACCIDENTAL_WRITTEN_ns,
    ACCIDENTAL_WRITTEN_su,
    ACCIDENTAL_WRITTEN_sd,
    ACCIDENTAL_WRITTEN_fu,
    ACCIDENTAL_WRITTEN_fd,
    ACCIDENTAL_WRITTEN_nu,
    ACCIDENTAL_WRITTEN_nd,
    ACCIDENTAL_WRITTEN_1qf,
    ACCIDENTAL_WRITTEN_3qf,
    ACCIDENTAL_WRITTEN_1qs,
    ACCIDENTAL_WRITTEN_3qs,
    ACCIDENTAL_WRITTEN_bms,
    ACCIDENTAL_WRITTEN_kms,
    ACCIDENTAL_WRITTEN_bs,
    ACCIDENTAL_WRITTEN_ks,
    ACCIDENTAL_WRITTEN_kf,
    ACCIDENTAL_WRITTEN_bf,
    ACCIDENTAL_WRITTEN_kmf,
    ACCIDENTAL_WRITTEN_bmf,
    ACCIDENTAL_WRITTEN_MAX
};

/**
 * MEI data.ACCIDENTAL.WRITTEN.aeu
 */
enum data_ACCIDENTAL_WRITTEN_aeu {
    ACCIDENTAL_WRITTEN_aeu_NONE = 0,
    ACCIDENTAL_WRITTEN_aeu_bms,
    ACCIDENTAL_WRITTEN_aeu_kms,
    ACCIDENTAL_WRITTEN_aeu_bs,
    ACCIDENTAL_WRITTEN_aeu_ks,
    ACCIDENTAL_WRITTEN_aeu_kf,
    ACCIDENTAL_WRITTEN_aeu_bf,
    ACCIDENTAL_WRITTEN_aeu_kmf,
    ACCIDENTAL_WRITTEN_aeu_bmf,
    ACCIDENTAL_WRITTEN_aeu_MAX
};

/**
 * MEI data.ACCIDENTAL.WRITTEN.basic
 */
enum data_ACCIDENTAL_WRITTEN_basic {
    ACCIDENTAL_WRITTEN_basic_NONE = 0,
    ACCIDENTAL_WRITTEN_basic_s,
    ACCIDENTAL_WRITTEN_basic_f,
    ACCIDENTAL_WRITTEN_basic_ss,
    ACCIDENTAL_WRITTEN_basic_x,
    ACCIDENTAL_WRITTEN_basic_ff,
    ACCIDENTAL_WRITTEN_basic_xs,
    ACCIDENTAL_WRITTEN_basic_sx,
    ACCIDENTAL_WRITTEN_basic_ts,
    ACCIDENTAL_WRITTEN_basic_tf,
    ACCIDENTAL_WRITTEN_basic_n,
    ACCIDENTAL_WRITTEN_basic_nf,
    ACCIDENTAL_WRITTEN_basic_ns,
    ACCIDENTAL_WRITTEN_basic_MAX
};

/**
 * MEI data.ACCIDENTAL.WRITTEN.extended
 */
enum data_ACCIDENTAL_WRITTEN_extended {
    ACCIDENTAL_WRITTEN_extended_NONE = 0,
    ACCIDENTAL_WRITTEN_extended_su,
    ACCIDENTAL_WRITTEN_extended_sd,
    ACCIDENTAL_WRITTEN_extended_fu,
    ACCIDENTAL_WRITTEN_extended_fd,
    ACCIDENTAL_WRITTEN_extended_nu,
    ACCIDENTAL_WRITTEN_extended_nd,
    ACCIDENTAL_WRITTEN_extended_1qf,
    ACCIDENTAL_WRITTEN_extended_3qf,
    ACCIDENTAL_WRITTEN_extended_1qs,
    ACCIDENTAL_WRITTEN_extended_3qs,
    ACCIDENTAL_WRITTEN_extended_MAX
};

/**
 * MEI data.ARTICULATION
 */
enum data_ARTICULATION {
    ARTICULATION_NONE = 0,
    ARTICULATION_acc,
    ARTICULATION_stacc,
    ARTICULATION_ten,
    ARTICULATION_stacciss,
    ARTICULATION_marc,
    ARTICULATION_spicc,
    ARTICULATION_doit,
    ARTICULATION_scoop,
    ARTICULATION_rip,
    ARTICULATION_plop,
    ARTICULATION_fall,
    ARTICULATION_longfall,
    ARTICULATION_bend,
    ARTICULATION_flip,
    ARTICULATION_smear,
    ARTICULATION_shake,
    ARTICULATION_dnbow,
    ARTICULATION_upbow,
    ARTICULATION_harm,
    ARTICULATION_snap,
    ARTICULATION_fingernail,
    ARTICULATION_damp,
    ARTICULATION_dampall,
    ARTICULATION_open,
    ARTICULATION_stop,
    ARTICULATION_dbltongue,
    ARTICULATION_trpltongue,
    ARTICULATION_heel,
    ARTICULATION_toe,
    ARTICULATION_tap,
    ARTICULATION_lhpizz,
    ARTICULATION_dot,
    ARTICULATION_stroke,
    ARTICULATION_MAX
};

/**
 * MEI data.BARMETHOD
 */
enum data_BARMETHOD {
    BARMETHOD_NONE = 0,
    BARMETHOD_mensur,
    BARMETHOD_staff,
    BARMETHOD_takt,
    BARMETHOD_MAX
};

/**
 * MEI data.BARRENDITION
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
    BARRENDITION_single,
    BARRENDITION_MAX
};

/**
 * MEI data.BEAMPLACE
 */
enum data_BEAMPLACE {
    BEAMPLACE_NONE = 0,
    BEAMPLACE_above,
    BEAMPLACE_below,
    BEAMPLACE_mixed,
    BEAMPLACE_MAX
};

/**
 * MEI data.BETYPE
 */
enum data_BETYPE {
    BETYPE_NONE = 0,
    BETYPE_byte,
    BETYPE_smil,
    BETYPE_midi,
    BETYPE_mmc,
    BETYPE_mtc,
    BETYPE_smpte_25,
    BETYPE_smpte_24,
    BETYPE_smpte_df30,
    BETYPE_smpte_ndf30,
    BETYPE_smpte_df29_97,
    BETYPE_smpte_ndf29_97,
    BETYPE_tcf,
    BETYPE_time,
    BETYPE_MAX
};

/**
 * MEI data.BOOLEAN
 */
enum data_BOOLEAN {
    BOOLEAN_NONE = 0,
    BOOLEAN_true,
    BOOLEAN_false,
    BOOLEAN_MAX
};

/**
 * MEI data.CERTAINTY
 */
enum data_CERTAINTY {
    CERTAINTY_NONE = 0,
    CERTAINTY_high,
    CERTAINTY_medium,
    CERTAINTY_low,
    CERTAINTY_unknown,
    CERTAINTY_MAX
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
    CLEFSHAPE_TAB,
    CLEFSHAPE_MAX
};

/**
 * MEI data.CLUSTER
 */
enum data_CLUSTER {
    CLUSTER_NONE = 0,
    CLUSTER_white,
    CLUSTER_black,
    CLUSTER_chromatic,
    CLUSTER_MAX
};

/**
 * MEI data.COLORNAMES
 */
enum data_COLORNAMES {
    COLORNAMES_NONE = 0,
    COLORNAMES_aliceblue,
    COLORNAMES_antiquewhite,
    COLORNAMES_aqua,
    COLORNAMES_aquamarine,
    COLORNAMES_azure,
    COLORNAMES_beige,
    COLORNAMES_bisque,
    COLORNAMES_black,
    COLORNAMES_blanchedalmond,
    COLORNAMES_blue,
    COLORNAMES_blueviolet,
    COLORNAMES_brown,
    COLORNAMES_burlywood,
    COLORNAMES_cadetblue,
    COLORNAMES_chartreuse,
    COLORNAMES_chocolate,
    COLORNAMES_coral,
    COLORNAMES_cornflowerblue,
    COLORNAMES_cornsilk,
    COLORNAMES_crimson,
    COLORNAMES_cyan,
    COLORNAMES_darkblue,
    COLORNAMES_darkcyan,
    COLORNAMES_darkgoldenrod,
    COLORNAMES_darkgray,
    COLORNAMES_darkgreen,
    COLORNAMES_darkgrey,
    COLORNAMES_darkkhaki,
    COLORNAMES_darkmagenta,
    COLORNAMES_darkolivegreen,
    COLORNAMES_darkorange,
    COLORNAMES_darkorchid,
    COLORNAMES_darkred,
    COLORNAMES_darksalmon,
    COLORNAMES_darkseagreen,
    COLORNAMES_darkslateblue,
    COLORNAMES_darkslategray,
    COLORNAMES_darkslategrey,
    COLORNAMES_darkturquoise,
    COLORNAMES_darkviolet,
    COLORNAMES_deeppink,
    COLORNAMES_deepskyblue,
    COLORNAMES_dimgray,
    COLORNAMES_dimgrey,
    COLORNAMES_dodgerblue,
    COLORNAMES_firebrick,
    COLORNAMES_floralwhite,
    COLORNAMES_forestgreen,
    COLORNAMES_fuchsia,
    COLORNAMES_gainsboro,
    COLORNAMES_ghostwhite,
    COLORNAMES_gold,
    COLORNAMES_goldenrod,
    COLORNAMES_gray,
    COLORNAMES_green,
    COLORNAMES_greenyellow,
    COLORNAMES_grey,
    COLORNAMES_honeydew,
    COLORNAMES_hotpink,
    COLORNAMES_indianred,
    COLORNAMES_indigo,
    COLORNAMES_ivory,
    COLORNAMES_khaki,
    COLORNAMES_lavender,
    COLORNAMES_lavenderblush,
    COLORNAMES_lawngreen,
    COLORNAMES_lemonchiffon,
    COLORNAMES_lightblue,
    COLORNAMES_lightcoral,
    COLORNAMES_lightcyan,
    COLORNAMES_lightgoldenrodyellow,
    COLORNAMES_lightgray,
    COLORNAMES_lightgreen,
    COLORNAMES_lightgrey,
    COLORNAMES_lightpink,
    COLORNAMES_lightsalmon,
    COLORNAMES_lightseagreen,
    COLORNAMES_lightskyblue,
    COLORNAMES_lightslategray,
    COLORNAMES_lightslategrey,
    COLORNAMES_lightsteelblue,
    COLORNAMES_lightyellow,
    COLORNAMES_lime,
    COLORNAMES_limegreen,
    COLORNAMES_linen,
    COLORNAMES_magenta,
    COLORNAMES_maroon,
    COLORNAMES_mediumaquamarine,
    COLORNAMES_mediumblue,
    COLORNAMES_mediumorchid,
    COLORNAMES_mediumpurple,
    COLORNAMES_mediumseagreen,
    COLORNAMES_mediumslateblue,
    COLORNAMES_mediumspringgreen,
    COLORNAMES_mediumturquoise,
    COLORNAMES_mediumvioletred,
    COLORNAMES_midnightblue,
    COLORNAMES_mintcream,
    COLORNAMES_mistyrose,
    COLORNAMES_moccasin,
    COLORNAMES_navajowhite,
    COLORNAMES_navy,
    COLORNAMES_oldlace,
    COLORNAMES_olive,
    COLORNAMES_olivedrab,
    COLORNAMES_orange,
    COLORNAMES_orangered,
    COLORNAMES_orchid,
    COLORNAMES_palegoldenrod,
    COLORNAMES_palegreen,
    COLORNAMES_paleturquoise,
    COLORNAMES_palevioletred,
    COLORNAMES_papayawhip,
    COLORNAMES_peachpuff,
    COLORNAMES_peru,
    COLORNAMES_pink,
    COLORNAMES_plum,
    COLORNAMES_powderblue,
    COLORNAMES_purple,
    COLORNAMES_rebeccapurple,
    COLORNAMES_red,
    COLORNAMES_rosybrown,
    COLORNAMES_royalblue,
    COLORNAMES_saddlebrown,
    COLORNAMES_salmon,
    COLORNAMES_sandybrown,
    COLORNAMES_seagreen,
    COLORNAMES_seashell,
    COLORNAMES_sienna,
    COLORNAMES_silver,
    COLORNAMES_skyblue,
    COLORNAMES_slateblue,
    COLORNAMES_slategray,
    COLORNAMES_slategrey,
    COLORNAMES_snow,
    COLORNAMES_springgreen,
    COLORNAMES_steelblue,
    COLORNAMES_tan,
    COLORNAMES_teal,
    COLORNAMES_thistle,
    COLORNAMES_tomato,
    COLORNAMES_turquoise,
    COLORNAMES_violet,
    COLORNAMES_wheat,
    COLORNAMES_white,
    COLORNAMES_whitesmoke,
    COLORNAMES_yellow,
    COLORNAMES_yellowgreen,
    COLORNAMES_MAX
};

/**
 * MEI data.COMPASSDIRECTION
 */
enum data_COMPASSDIRECTION {
    COMPASSDIRECTION_NONE = 0,
    COMPASSDIRECTION_n,
    COMPASSDIRECTION_e,
    COMPASSDIRECTION_s,
    COMPASSDIRECTION_w,
    COMPASSDIRECTION_ne,
    COMPASSDIRECTION_nw,
    COMPASSDIRECTION_se,
    COMPASSDIRECTION_sw,
    COMPASSDIRECTION_MAX
};

/**
 * MEI data.COMPASSDIRECTION.basic
 */
enum data_COMPASSDIRECTION_basic {
    COMPASSDIRECTION_basic_NONE = 0,
    COMPASSDIRECTION_basic_n,
    COMPASSDIRECTION_basic_e,
    COMPASSDIRECTION_basic_s,
    COMPASSDIRECTION_basic_w,
    COMPASSDIRECTION_basic_MAX
};

/**
 * MEI data.COMPASSDIRECTION.extended
 */
enum data_COMPASSDIRECTION_extended {
    COMPASSDIRECTION_extended_NONE = 0,
    COMPASSDIRECTION_extended_ne,
    COMPASSDIRECTION_extended_nw,
    COMPASSDIRECTION_extended_se,
    COMPASSDIRECTION_extended_sw,
    COMPASSDIRECTION_extended_MAX
};

/**
 * MEI data.DIVISIO
 */
enum data_DIVISIO {
    DIVISIO_NONE = 0,
    DIVISIO_ternaria,
    DIVISIO_quaternaria,
    DIVISIO_senariaimperf,
    DIVISIO_senariaperf,
    DIVISIO_octonaria,
    DIVISIO_novenaria,
    DIVISIO_duodenaria,
    DIVISIO_MAX
};

/**
 * MEI data.DURATIONRESTS
 */
enum data_DURATIONRESTS {
    DURATIONRESTS_NONE = 0,
    DURATIONRESTS_long,
    DURATIONRESTS_breve,
    DURATIONRESTS_1,
    DURATIONRESTS_2,
    DURATIONRESTS_4,
    DURATIONRESTS_8,
    DURATIONRESTS_16,
    DURATIONRESTS_32,
    DURATIONRESTS_64,
    DURATIONRESTS_128,
    DURATIONRESTS_256,
    DURATIONRESTS_512,
    DURATIONRESTS_1024,
    DURATIONRESTS_2048,
    DURATIONRESTS_MAX
};

/**
 * MEI data.DURATIONRESTS.mensural
 */
enum data_DURATIONRESTS_mensural {
    DURATIONRESTS_mensural_NONE = 0,
    DURATIONRESTS_mensural_2B,
    DURATIONRESTS_mensural_3B,
    DURATIONRESTS_mensural_maxima,
    DURATIONRESTS_mensural_longa,
    DURATIONRESTS_mensural_brevis,
    DURATIONRESTS_mensural_semibrevis,
    DURATIONRESTS_mensural_minima,
    DURATIONRESTS_mensural_semiminima,
    DURATIONRESTS_mensural_fusa,
    DURATIONRESTS_mensural_semifusa,
    DURATIONRESTS_mensural_MAX
};

/**
 * MEI data.DURQUALITY.mensural
 */
enum data_DURQUALITY_mensural {
    DURQUALITY_mensural_NONE = 0,
    DURQUALITY_mensural_perfecta,
    DURQUALITY_mensural_imperfecta,
    DURQUALITY_mensural_altera,
    DURQUALITY_mensural_minor,
    DURQUALITY_mensural_maior,
    DURQUALITY_mensural_duplex,
    DURQUALITY_mensural_MAX
};

/**
 * MEI data.ENCLOSURE
 */
enum data_ENCLOSURE {
    ENCLOSURE_NONE = 0,
    ENCLOSURE_paren,
    ENCLOSURE_brack,
    ENCLOSURE_MAX
};

/**
 * MEI data.EVENTREL
 */
enum data_EVENTREL {
    EVENTREL_NONE = 0,
    EVENTREL_above,
    EVENTREL_below,
    EVENTREL_left,
    EVENTREL_right,
    EVENTREL_above_left,
    EVENTREL_above_right,
    EVENTREL_below_left,
    EVENTREL_below_right,
    EVENTREL_MAX
};

/**
 * MEI data.EVENTREL.basic
 */
enum data_EVENTREL_basic {
    EVENTREL_basic_NONE = 0,
    EVENTREL_basic_above,
    EVENTREL_basic_below,
    EVENTREL_basic_left,
    EVENTREL_basic_right,
    EVENTREL_basic_MAX
};

/**
 * MEI data.EVENTREL.extended
 */
enum data_EVENTREL_extended {
    EVENTREL_extended_NONE = 0,
    EVENTREL_extended_above_left,
    EVENTREL_extended_above_right,
    EVENTREL_extended_below_left,
    EVENTREL_extended_below_right,
    EVENTREL_extended_MAX
};

/**
 * MEI data.FILL
 */
enum data_FILL {
    FILL_NONE = 0,
    FILL_void,
    FILL_solid,
    FILL_top,
    FILL_bottom,
    FILL_left,
    FILL_right,
    FILL_MAX
};

/**
 * MEI data.FLAGFORM.mensural
 */
enum data_FLAGFORM_mensural {
    FLAGFORM_mensural_NONE = 0,
    FLAGFORM_mensural_straight,
    FLAGFORM_mensural_angled,
    FLAGFORM_mensural_curled,
    FLAGFORM_mensural_flared,
    FLAGFORM_mensural_extended,
    FLAGFORM_mensural_hooked,
    FLAGFORM_mensural_MAX
};

/**
 * MEI data.FLAGPOS.mensural
 */
enum data_FLAGPOS_mensural {
    FLAGPOS_mensural_NONE = 0,
    FLAGPOS_mensural_left,
    FLAGPOS_mensural_right,
    FLAGPOS_mensural_center,
    FLAGPOS_mensural_MAX
};

/**
 * MEI data.FONTSIZETERM
 */
enum data_FONTSIZETERM {
    FONTSIZETERM_NONE = 0,
    FONTSIZETERM_xx_small,
    FONTSIZETERM_x_small,
    FONTSIZETERM_small,
    FONTSIZETERM_normal,
    FONTSIZETERM_large,
    FONTSIZETERM_x_large,
    FONTSIZETERM_xx_large,
    FONTSIZETERM_smaller,
    FONTSIZETERM_larger,
    FONTSIZETERM_MAX
};

/**
 * MEI data.FONTSTYLE
 */
enum data_FONTSTYLE {
    FONTSTYLE_NONE = 0,
    FONTSTYLE_italic,
    FONTSTYLE_normal,
    FONTSTYLE_oblique,
    FONTSTYLE_MAX
};

/**
 * MEI data.FONTWEIGHT
 */
enum data_FONTWEIGHT {
    FONTWEIGHT_NONE = 0,
    FONTWEIGHT_bold,
    FONTWEIGHT_normal,
    FONTWEIGHT_MAX
};

/**
 * MEI data.FRBRRELATIONSHIP
 */
enum data_FRBRRELATIONSHIP {
    FRBRRELATIONSHIP_NONE = 0,
    FRBRRELATIONSHIP_hasAbridgement,
    FRBRRELATIONSHIP_isAbridgementOf,
    FRBRRELATIONSHIP_hasAdaptation,
    FRBRRELATIONSHIP_isAdaptationOf,
    FRBRRELATIONSHIP_hasAlternate,
    FRBRRELATIONSHIP_isAlternateOf,
    FRBRRELATIONSHIP_hasArrangement,
    FRBRRELATIONSHIP_isArrangementOf,
    FRBRRELATIONSHIP_hasComplement,
    FRBRRELATIONSHIP_isComplementOf,
    FRBRRELATIONSHIP_hasEmbodiment,
    FRBRRELATIONSHIP_isEmbodimentOf,
    FRBRRELATIONSHIP_hasExemplar,
    FRBRRELATIONSHIP_isExemplarOf,
    FRBRRELATIONSHIP_hasImitation,
    FRBRRELATIONSHIP_isImitationOf,
    FRBRRELATIONSHIP_hasPart,
    FRBRRELATIONSHIP_isPartOf,
    FRBRRELATIONSHIP_hasRealization,
    FRBRRELATIONSHIP_isRealizationOf,
    FRBRRELATIONSHIP_hasReconfiguration,
    FRBRRELATIONSHIP_isReconfigurationOf,
    FRBRRELATIONSHIP_hasReproduction,
    FRBRRELATIONSHIP_isReproductionOf,
    FRBRRELATIONSHIP_hasRevision,
    FRBRRELATIONSHIP_isRevisionOf,
    FRBRRELATIONSHIP_hasSuccessor,
    FRBRRELATIONSHIP_isSuccessorOf,
    FRBRRELATIONSHIP_hasSummarization,
    FRBRRELATIONSHIP_isSummarizationOf,
    FRBRRELATIONSHIP_hasSupplement,
    FRBRRELATIONSHIP_isSupplementOf,
    FRBRRELATIONSHIP_hasTransformation,
    FRBRRELATIONSHIP_isTransformationOf,
    FRBRRELATIONSHIP_hasTranslation,
    FRBRRELATIONSHIP_isTranslationOf,
    FRBRRELATIONSHIP_MAX
};

/**
 * MEI data.GLISSANDO
 */
enum data_GLISSANDO {
    GLISSANDO_NONE = 0,
    GLISSANDO_i,
    GLISSANDO_m,
    GLISSANDO_t,
    GLISSANDO_MAX
};

/**
 * MEI data.GRACE
 */
enum data_GRACE {
    GRACE_NONE = 0,
    GRACE_acc,
    GRACE_unacc,
    GRACE_unknown,
    GRACE_MAX
};

/**
 * MEI data.HEADSHAPE
 */
enum data_HEADSHAPE {
    HEADSHAPE_NONE = 0,
    HEADSHAPE_quarter,
    HEADSHAPE_half,
    HEADSHAPE_whole,
    HEADSHAPE_backslash,
    HEADSHAPE_circle,
    HEADSHAPE_plus,
    HEADSHAPE_diamond,
    HEADSHAPE_isotriangle,
    HEADSHAPE_oval,
    HEADSHAPE_piewedge,
    HEADSHAPE_rectangle,
    HEADSHAPE_rtriangle,
    HEADSHAPE_semicircle,
    HEADSHAPE_slash,
    HEADSHAPE_square,
    HEADSHAPE_x,
    HEADSHAPE_MAX
};

/**
 * MEI data.HEADSHAPE.list
 */
enum data_HEADSHAPE_list {
    HEADSHAPE_list_NONE = 0,
    HEADSHAPE_list_quarter,
    HEADSHAPE_list_half,
    HEADSHAPE_list_whole,
    HEADSHAPE_list_backslash,
    HEADSHAPE_list_circle,
    HEADSHAPE_list_plus,
    HEADSHAPE_list_diamond,
    HEADSHAPE_list_isotriangle,
    HEADSHAPE_list_oval,
    HEADSHAPE_list_piewedge,
    HEADSHAPE_list_rectangle,
    HEADSHAPE_list_rtriangle,
    HEADSHAPE_list_semicircle,
    HEADSHAPE_list_slash,
    HEADSHAPE_list_square,
    HEADSHAPE_list_x,
    HEADSHAPE_list_MAX
};

/**
 * MEI data.HORIZONTALALIGNMENT
 */
enum data_HORIZONTALALIGNMENT {
    HORIZONTALALIGNMENT_NONE = 0,
    HORIZONTALALIGNMENT_left,
    HORIZONTALALIGNMENT_right,
    HORIZONTALALIGNMENT_center,
    HORIZONTALALIGNMENT_justify,
    HORIZONTALALIGNMENT_MAX
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
    LAYERSCHEME_3f,
    LAYERSCHEME_MAX
};

/**
 * MEI data.LIGATUREFORM
 */
enum data_LIGATUREFORM {
    LIGATUREFORM_NONE = 0,
    LIGATUREFORM_recta,
    LIGATUREFORM_obliqua,
    LIGATUREFORM_MAX
};

/**
 * MEI data.LINEFORM
 */
enum data_LINEFORM {
    LINEFORM_NONE = 0,
    LINEFORM_dashed,
    LINEFORM_dotted,
    LINEFORM_solid,
    LINEFORM_wavy,
    LINEFORM_MAX
};

/**
 * MEI data.LINESTARTENDSYMBOL
 */
enum data_LINESTARTENDSYMBOL {
    LINESTARTENDSYMBOL_NONE = 0,
    LINESTARTENDSYMBOL_angledown,
    LINESTARTENDSYMBOL_angleup,
    LINESTARTENDSYMBOL_angleright,
    LINESTARTENDSYMBOL_angleleft,
    LINESTARTENDSYMBOL_arrow,
    LINESTARTENDSYMBOL_arrowopen,
    LINESTARTENDSYMBOL_arrowwhite,
    LINESTARTENDSYMBOL_harpoonleft,
    LINESTARTENDSYMBOL_harpoonright,
    LINESTARTENDSYMBOL_H,
    LINESTARTENDSYMBOL_N,
    LINESTARTENDSYMBOL_Th,
    LINESTARTENDSYMBOL_ThRetro,
    LINESTARTENDSYMBOL_ThRetroInv,
    LINESTARTENDSYMBOL_ThInv,
    LINESTARTENDSYMBOL_T,
    LINESTARTENDSYMBOL_TInv,
    LINESTARTENDSYMBOL_CH,
    LINESTARTENDSYMBOL_RH,
    LINESTARTENDSYMBOL_none,
    LINESTARTENDSYMBOL_MAX
};

/**
 * MEI data.LINEWIDTHTERM
 */
enum data_LINEWIDTHTERM {
    LINEWIDTHTERM_NONE = 0,
    LINEWIDTHTERM_narrow,
    LINEWIDTHTERM_medium,
    LINEWIDTHTERM_wide,
    LINEWIDTHTERM_MAX
};

/**
 * MEI data.MELODICFUNCTION
 */
enum data_MELODICFUNCTION {
    MELODICFUNCTION_NONE = 0,
    MELODICFUNCTION_aln,
    MELODICFUNCTION_ant,
    MELODICFUNCTION_app,
    MELODICFUNCTION_apt,
    MELODICFUNCTION_arp,
    MELODICFUNCTION_arp7,
    MELODICFUNCTION_aun,
    MELODICFUNCTION_chg,
    MELODICFUNCTION_cln,
    MELODICFUNCTION_ct,
    MELODICFUNCTION_ct7,
    MELODICFUNCTION_cun,
    MELODICFUNCTION_cup,
    MELODICFUNCTION_et,
    MELODICFUNCTION_ln,
    MELODICFUNCTION_ped,
    MELODICFUNCTION_rep,
    MELODICFUNCTION_ret,
    MELODICFUNCTION_23ret,
    MELODICFUNCTION_78ret,
    MELODICFUNCTION_sus,
    MELODICFUNCTION_43sus,
    MELODICFUNCTION_98sus,
    MELODICFUNCTION_76sus,
    MELODICFUNCTION_un,
    MELODICFUNCTION_un7,
    MELODICFUNCTION_upt,
    MELODICFUNCTION_upt7,
    MELODICFUNCTION_MAX
};

/**
 * MEI data.MENSURATIONSIGN
 */
enum data_MENSURATIONSIGN {
    MENSURATIONSIGN_NONE = 0,
    MENSURATIONSIGN_C,
    MENSURATIONSIGN_O,
    MENSURATIONSIGN_t,
    MENSURATIONSIGN_q,
    MENSURATIONSIGN_si,
    MENSURATIONSIGN_i,
    MENSURATIONSIGN_sg,
    MENSURATIONSIGN_g,
    MENSURATIONSIGN_sp,
    MENSURATIONSIGN_p,
    MENSURATIONSIGN_sy,
    MENSURATIONSIGN_y,
    MENSURATIONSIGN_n,
    MENSURATIONSIGN_oc,
    MENSURATIONSIGN_d,
    MENSURATIONSIGN_MAX
};

/**
 * MEI data.METERFORM
 */
enum data_METERFORM {
    METERFORM_NONE = 0,
    METERFORM_num,
    METERFORM_denomsym,
    METERFORM_norm,
    METERFORM_invis,
    METERFORM_MAX
};

/**
 * MEI data.METERSIGN
 */
enum data_METERSIGN {
    METERSIGN_NONE = 0,
    METERSIGN_common,
    METERSIGN_cut,
    METERSIGN_MAX
};

/**
 * MEI data.MIDINAMES
 */
enum data_MIDINAMES {
    MIDINAMES_NONE = 0,
    MIDINAMES_Acoustic_Grand_Piano,
    MIDINAMES_Bright_Acoustic_Piano,
    MIDINAMES_Electric_Grand_Piano,
    MIDINAMES_Honky_tonk_Piano,
    MIDINAMES_Electric_Piano_1,
    MIDINAMES_Electric_Piano_2,
    MIDINAMES_Harpsichord,
    MIDINAMES_Clavi,
    MIDINAMES_Celesta,
    MIDINAMES_Glockenspiel,
    MIDINAMES_Music_Box,
    MIDINAMES_Vibraphone,
    MIDINAMES_Marimba,
    MIDINAMES_Xylophone,
    MIDINAMES_Tubular_Bells,
    MIDINAMES_Dulcimer,
    MIDINAMES_Drawbar_Organ,
    MIDINAMES_Percussive_Organ,
    MIDINAMES_Rock_Organ,
    MIDINAMES_Church_Organ,
    MIDINAMES_Reed_Organ,
    MIDINAMES_Accordion,
    MIDINAMES_Harmonica,
    MIDINAMES_Tango_Accordion,
    MIDINAMES_Acoustic_Guitar_nylon,
    MIDINAMES_Acoustic_Guitar_steel,
    MIDINAMES_Electric_Guitar_jazz,
    MIDINAMES_Electric_Guitar_clean,
    MIDINAMES_Electric_Guitar_muted,
    MIDINAMES_Overdriven_Guitar,
    MIDINAMES_Distortion_Guitar,
    MIDINAMES_Guitar_harmonics,
    MIDINAMES_Acoustic_Bass,
    MIDINAMES_Electric_Bass_finger,
    MIDINAMES_Electric_Bass_pick,
    MIDINAMES_Fretless_Bass,
    MIDINAMES_Slap_Bass_1,
    MIDINAMES_Slap_Bass_2,
    MIDINAMES_Synth_Bass_1,
    MIDINAMES_Synth_Bass_2,
    MIDINAMES_Violin,
    MIDINAMES_Viola,
    MIDINAMES_Cello,
    MIDINAMES_Contrabass,
    MIDINAMES_Tremolo_Strings,
    MIDINAMES_Pizzicato_Strings,
    MIDINAMES_Orchestral_Harp,
    MIDINAMES_Timpani,
    MIDINAMES_String_Ensemble_1,
    MIDINAMES_String_Ensemble_2,
    MIDINAMES_SynthStrings_1,
    MIDINAMES_SynthStrings_2,
    MIDINAMES_Choir_Aahs,
    MIDINAMES_Voice_Oohs,
    MIDINAMES_Synth_Voice,
    MIDINAMES_Orchestra_Hit,
    MIDINAMES_Trumpet,
    MIDINAMES_Trombone,
    MIDINAMES_Tuba,
    MIDINAMES_Muted_Trumpet,
    MIDINAMES_French_Horn,
    MIDINAMES_Brass_Section,
    MIDINAMES_SynthBrass_1,
    MIDINAMES_SynthBrass_2,
    MIDINAMES_Soprano_Sax,
    MIDINAMES_Alto_Sax,
    MIDINAMES_Tenor_Sax,
    MIDINAMES_Baritone_Sax,
    MIDINAMES_Oboe,
    MIDINAMES_English_Horn,
    MIDINAMES_Bassoon,
    MIDINAMES_Clarinet,
    MIDINAMES_Piccolo,
    MIDINAMES_Flute,
    MIDINAMES_Recorder,
    MIDINAMES_Pan_Flute,
    MIDINAMES_Blown_Bottle,
    MIDINAMES_Shakuhachi,
    MIDINAMES_Whistle,
    MIDINAMES_Ocarina,
    MIDINAMES_Lead_1_square,
    MIDINAMES_Lead_2_sawtooth,
    MIDINAMES_Lead_3_calliope,
    MIDINAMES_Lead_4_chiff,
    MIDINAMES_Lead_5_charang,
    MIDINAMES_Lead_6_voice,
    MIDINAMES_Lead_7_fifths,
    MIDINAMES_Lead_8_bass_and_lead,
    MIDINAMES_Pad_1_new_age,
    MIDINAMES_Pad_2_warm,
    MIDINAMES_Pad_3_polysynth,
    MIDINAMES_Pad_4_choir,
    MIDINAMES_Pad_5_bowed,
    MIDINAMES_Pad_6_metallic,
    MIDINAMES_Pad_7_halo,
    MIDINAMES_Pad_8_sweep,
    MIDINAMES_FX_1_rain,
    MIDINAMES_FX_2_soundtrack,
    MIDINAMES_FX_3_crystal,
    MIDINAMES_FX_4_atmosphere,
    MIDINAMES_FX_5_brightness,
    MIDINAMES_FX_6_goblins,
    MIDINAMES_FX_7_echoes,
    MIDINAMES_FX_8_sci_fi,
    MIDINAMES_Sitar,
    MIDINAMES_Banjo,
    MIDINAMES_Shamisen,
    MIDINAMES_Koto,
    MIDINAMES_Kalimba,
    MIDINAMES_Bagpipe,
    MIDINAMES_Fiddle,
    MIDINAMES_Shanai,
    MIDINAMES_Tinkle_Bell,
    MIDINAMES_Agogo,
    MIDINAMES_Steel_Drums,
    MIDINAMES_Woodblock,
    MIDINAMES_Taiko_Drum,
    MIDINAMES_Melodic_Tom,
    MIDINAMES_Synth_Drum,
    MIDINAMES_Reverse_Cymbal,
    MIDINAMES_Guitar_Fret_Noise,
    MIDINAMES_Breath_Noise,
    MIDINAMES_Seashore,
    MIDINAMES_Bird_Tweet,
    MIDINAMES_Telephone_Ring,
    MIDINAMES_Helicopter,
    MIDINAMES_Applause,
    MIDINAMES_Gunshot,
    MIDINAMES_Acoustic_Bass_Drum,
    MIDINAMES_Bass_Drum_1,
    MIDINAMES_Side_Stick,
    MIDINAMES_Acoustic_Snare,
    MIDINAMES_Hand_Clap,
    MIDINAMES_Electric_Snare,
    MIDINAMES_Low_Floor_Tom,
    MIDINAMES_Closed_Hi_Hat,
    MIDINAMES_High_Floor_Tom,
    MIDINAMES_Pedal_Hi_Hat,
    MIDINAMES_Low_Tom,
    MIDINAMES_Open_Hi_Hat,
    MIDINAMES_Low_Mid_Tom,
    MIDINAMES_Hi_Mid_Tom,
    MIDINAMES_Crash_Cymbal_1,
    MIDINAMES_High_Tom,
    MIDINAMES_Ride_Cymbal_1,
    MIDINAMES_Chinese_Cymbal,
    MIDINAMES_Ride_Bell,
    MIDINAMES_Tambourine,
    MIDINAMES_Splash_Cymbal,
    MIDINAMES_Cowbell,
    MIDINAMES_Crash_Cymbal_2,
    MIDINAMES_Vibraslap,
    MIDINAMES_Ride_Cymbal_2,
    MIDINAMES_Hi_Bongo,
    MIDINAMES_Low_Bongo,
    MIDINAMES_Mute_Hi_Conga,
    MIDINAMES_Open_Hi_Conga,
    MIDINAMES_Low_Conga,
    MIDINAMES_High_Timbale,
    MIDINAMES_Low_Timbale,
    MIDINAMES_High_Agogo,
    MIDINAMES_Low_Agogo,
    MIDINAMES_Cabasa,
    MIDINAMES_Maracas,
    MIDINAMES_Short_Whistle,
    MIDINAMES_Long_Whistle,
    MIDINAMES_Short_Guiro,
    MIDINAMES_Long_Guiro,
    MIDINAMES_Claves,
    MIDINAMES_Hi_Wood_Block,
    MIDINAMES_Low_Wood_Block,
    MIDINAMES_Mute_Cuica,
    MIDINAMES_Open_Cuica,
    MIDINAMES_Mute_Triangle,
    MIDINAMES_Open_Triangle,
    MIDINAMES_MAX
};

/**
 * MEI data.MODE
 */
enum data_MODE {
    MODE_NONE = 0,
    MODE_major,
    MODE_minor,
    MODE_dorian,
    MODE_hypodorian,
    MODE_phrygian,
    MODE_hypophrygian,
    MODE_lydian,
    MODE_hypolydian,
    MODE_mixolydian,
    MODE_hypomixolydian,
    MODE_peregrinus,
    MODE_ionian,
    MODE_hypoionian,
    MODE_aeolian,
    MODE_hypoaeolian,
    MODE_locrian,
    MODE_hypolocrian,
    MODE_MAX
};

/**
 * MEI data.MODE.cmn
 */
enum data_MODE_cmn {
    MODE_cmn_NONE = 0,
    MODE_cmn_major,
    MODE_cmn_minor,
    MODE_cmn_MAX
};

/**
 * MEI data.MODE.extended
 */
enum data_MODE_extended {
    MODE_extended_NONE = 0,
    MODE_extended_ionian,
    MODE_extended_hypoionian,
    MODE_extended_aeolian,
    MODE_extended_hypoaeolian,
    MODE_extended_locrian,
    MODE_extended_hypolocrian,
    MODE_extended_MAX
};

/**
 * MEI data.MODE.gregorian
 */
enum data_MODE_gregorian {
    MODE_gregorian_NONE = 0,
    MODE_gregorian_dorian,
    MODE_gregorian_hypodorian,
    MODE_gregorian_phrygian,
    MODE_gregorian_hypophrygian,
    MODE_gregorian_lydian,
    MODE_gregorian_hypolydian,
    MODE_gregorian_mixolydian,
    MODE_gregorian_hypomixolydian,
    MODE_gregorian_peregrinus,
    MODE_gregorian_MAX
};

/**
 * MEI data.MODSRELATIONSHIP
 */
enum data_MODSRELATIONSHIP {
    MODSRELATIONSHIP_NONE = 0,
    MODSRELATIONSHIP_preceding,
    MODSRELATIONSHIP_succeeding,
    MODSRELATIONSHIP_original,
    MODSRELATIONSHIP_host,
    MODSRELATIONSHIP_constituent,
    MODSRELATIONSHIP_otherVersion,
    MODSRELATIONSHIP_otherFormat,
    MODSRELATIONSHIP_isReferencedBy,
    MODSRELATIONSHIP_references,
    MODSRELATIONSHIP_MAX
};

/**
 * MEI data.MULTIBREVERESTS.mensural
 */
enum data_MULTIBREVERESTS_mensural {
    MULTIBREVERESTS_mensural_NONE = 0,
    MULTIBREVERESTS_mensural_2B,
    MULTIBREVERESTS_mensural_3B,
    MULTIBREVERESTS_mensural_MAX
};

/**
 * MEI data.NONSTAFFPLACE
 */
enum data_NONSTAFFPLACE {
    NONSTAFFPLACE_NONE = 0,
    NONSTAFFPLACE_botmar,
    NONSTAFFPLACE_topmar,
    NONSTAFFPLACE_leftmar,
    NONSTAFFPLACE_rightmar,
    NONSTAFFPLACE_facing,
    NONSTAFFPLACE_overleaf,
    NONSTAFFPLACE_end,
    NONSTAFFPLACE_inter,
    NONSTAFFPLACE_intra,
    NONSTAFFPLACE_super,
    NONSTAFFPLACE_sub,
    NONSTAFFPLACE_inspace,
    NONSTAFFPLACE_superimposed,
    NONSTAFFPLACE_MAX
};

/**
 * MEI data.NOTATIONTYPE
 */
enum data_NOTATIONTYPE {
    NOTATIONTYPE_NONE = 0,
    NOTATIONTYPE_cmn,
    NOTATIONTYPE_mensural,
    NOTATIONTYPE_mensural_black,
    NOTATIONTYPE_mensural_white,
    NOTATIONTYPE_neume,
    NOTATIONTYPE_tab,
    NOTATIONTYPE_MAX
};

/**
 * MEI data.NOTEHEADMODIFIER
 */
enum data_NOTEHEADMODIFIER {
    NOTEHEADMODIFIER_NONE = 0,
    NOTEHEADMODIFIER_slash,
    NOTEHEADMODIFIER_backslash,
    NOTEHEADMODIFIER_vline,
    NOTEHEADMODIFIER_hline,
    NOTEHEADMODIFIER_centerdot,
    NOTEHEADMODIFIER_paren,
    NOTEHEADMODIFIER_brack,
    NOTEHEADMODIFIER_box,
    NOTEHEADMODIFIER_circle,
    NOTEHEADMODIFIER_dblwhole,
    NOTEHEADMODIFIER_MAX
};

/**
 * MEI data.NOTEHEADMODIFIER.list
 */
enum data_NOTEHEADMODIFIER_list {
    NOTEHEADMODIFIER_list_NONE = 0,
    NOTEHEADMODIFIER_list_slash,
    NOTEHEADMODIFIER_list_backslash,
    NOTEHEADMODIFIER_list_vline,
    NOTEHEADMODIFIER_list_hline,
    NOTEHEADMODIFIER_list_centerdot,
    NOTEHEADMODIFIER_list_paren,
    NOTEHEADMODIFIER_list_brack,
    NOTEHEADMODIFIER_list_box,
    NOTEHEADMODIFIER_list_circle,
    NOTEHEADMODIFIER_list_dblwhole,
    NOTEHEADMODIFIER_list_MAX
};

/**
 * MEI data.OTHERSTAFF
 */
enum data_OTHERSTAFF {
    OTHERSTAFF_NONE = 0,
    OTHERSTAFF_above,
    OTHERSTAFF_below,
    OTHERSTAFF_MAX
};

/**
 * MEI data.RELATIONSHIP
 */
enum data_RELATIONSHIP {
    RELATIONSHIP_NONE = 0,
    RELATIONSHIP_hasAbridgement,
    RELATIONSHIP_isAbridgementOf,
    RELATIONSHIP_hasAdaptation,
    RELATIONSHIP_isAdaptationOf,
    RELATIONSHIP_hasAlternate,
    RELATIONSHIP_isAlternateOf,
    RELATIONSHIP_hasArrangement,
    RELATIONSHIP_isArrangementOf,
    RELATIONSHIP_hasComplement,
    RELATIONSHIP_isComplementOf,
    RELATIONSHIP_hasEmbodiment,
    RELATIONSHIP_isEmbodimentOf,
    RELATIONSHIP_hasExemplar,
    RELATIONSHIP_isExemplarOf,
    RELATIONSHIP_hasImitation,
    RELATIONSHIP_isImitationOf,
    RELATIONSHIP_hasPart,
    RELATIONSHIP_isPartOf,
    RELATIONSHIP_hasRealization,
    RELATIONSHIP_isRealizationOf,
    RELATIONSHIP_hasReconfiguration,
    RELATIONSHIP_isReconfigurationOf,
    RELATIONSHIP_hasReproduction,
    RELATIONSHIP_isReproductionOf,
    RELATIONSHIP_hasRevision,
    RELATIONSHIP_isRevisionOf,
    RELATIONSHIP_hasSuccessor,
    RELATIONSHIP_isSuccessorOf,
    RELATIONSHIP_hasSummarization,
    RELATIONSHIP_isSummarizationOf,
    RELATIONSHIP_hasSupplement,
    RELATIONSHIP_isSupplementOf,
    RELATIONSHIP_hasTransformation,
    RELATIONSHIP_isTransformationOf,
    RELATIONSHIP_hasTranslation,
    RELATIONSHIP_isTranslationOf,
    RELATIONSHIP_preceding,
    RELATIONSHIP_succeeding,
    RELATIONSHIP_original,
    RELATIONSHIP_host,
    RELATIONSHIP_constituent,
    RELATIONSHIP_otherVersion,
    RELATIONSHIP_otherFormat,
    RELATIONSHIP_isReferencedBy,
    RELATIONSHIP_references,
    RELATIONSHIP_MAX
};

/**
 * MEI data.ROTATION
 */
enum data_ROTATION {
    ROTATION_NONE = 0,
    ROTATION_none,
    ROTATION_down,
    ROTATION_left,
    ROTATION_ne,
    ROTATION_nw,
    ROTATION_se,
    ROTATION_sw,
    ROTATION_MAX
};

/**
 * MEI data.ROTATIONDIRECTION
 */
enum data_ROTATIONDIRECTION {
    ROTATIONDIRECTION_NONE = 0,
    ROTATIONDIRECTION_none,
    ROTATIONDIRECTION_down,
    ROTATIONDIRECTION_left,
    ROTATIONDIRECTION_ne,
    ROTATIONDIRECTION_nw,
    ROTATIONDIRECTION_se,
    ROTATIONDIRECTION_sw,
    ROTATIONDIRECTION_MAX
};

/**
 * MEI data.STAFFITEM
 */
enum data_STAFFITEM {
    STAFFITEM_NONE = 0,
    STAFFITEM_accid,
    STAFFITEM_annot,
    STAFFITEM_artic,
    STAFFITEM_dir,
    STAFFITEM_dynam,
    STAFFITEM_harm,
    STAFFITEM_ornam,
    STAFFITEM_sp,
    STAFFITEM_stageDir,
    STAFFITEM_tempo,
    STAFFITEM_beam,
    STAFFITEM_bend,
    STAFFITEM_bracketSpan,
    STAFFITEM_breath,
    STAFFITEM_cpMark,
    STAFFITEM_fermata,
    STAFFITEM_fing,
    STAFFITEM_hairpin,
    STAFFITEM_harpPedal,
    STAFFITEM_lv,
    STAFFITEM_mordent,
    STAFFITEM_octave,
    STAFFITEM_pedal,
    STAFFITEM_reh,
    STAFFITEM_tie,
    STAFFITEM_trill,
    STAFFITEM_tuplet,
    STAFFITEM_turn,
    STAFFITEM_ligature,
    STAFFITEM_MAX
};

/**
 * MEI data.STAFFITEM.basic
 */
enum data_STAFFITEM_basic {
    STAFFITEM_basic_NONE = 0,
    STAFFITEM_basic_accid,
    STAFFITEM_basic_annot,
    STAFFITEM_basic_artic,
    STAFFITEM_basic_dir,
    STAFFITEM_basic_dynam,
    STAFFITEM_basic_harm,
    STAFFITEM_basic_ornam,
    STAFFITEM_basic_sp,
    STAFFITEM_basic_stageDir,
    STAFFITEM_basic_tempo,
    STAFFITEM_basic_MAX
};

/**
 * MEI data.STAFFITEM.cmn
 */
enum data_STAFFITEM_cmn {
    STAFFITEM_cmn_NONE = 0,
    STAFFITEM_cmn_beam,
    STAFFITEM_cmn_bend,
    STAFFITEM_cmn_bracketSpan,
    STAFFITEM_cmn_breath,
    STAFFITEM_cmn_cpMark,
    STAFFITEM_cmn_fermata,
    STAFFITEM_cmn_fing,
    STAFFITEM_cmn_hairpin,
    STAFFITEM_cmn_harpPedal,
    STAFFITEM_cmn_lv,
    STAFFITEM_cmn_mordent,
    STAFFITEM_cmn_octave,
    STAFFITEM_cmn_pedal,
    STAFFITEM_cmn_reh,
    STAFFITEM_cmn_tie,
    STAFFITEM_cmn_trill,
    STAFFITEM_cmn_tuplet,
    STAFFITEM_cmn_turn,
    STAFFITEM_cmn_MAX
};

/**
 * MEI data.STAFFITEM.mensural
 */
enum data_STAFFITEM_mensural {
    STAFFITEM_mensural_NONE = 0,
    STAFFITEM_mensural_ligature,
    STAFFITEM_mensural_MAX
};

/**
 * MEI data.STAFFREL
 */
enum data_STAFFREL {
    STAFFREL_NONE = 0,
    STAFFREL_above,
    STAFFREL_below,
    STAFFREL_between,
    STAFFREL_within,
    STAFFREL_MAX
};

/**
 * MEI data.STAFFREL.basic
 */
enum data_STAFFREL_basic {
    STAFFREL_basic_NONE = 0,
    STAFFREL_basic_above,
    STAFFREL_basic_below,
    STAFFREL_basic_MAX
};

/**
 * MEI data.STAFFREL.extended
 */
enum data_STAFFREL_extended {
    STAFFREL_extended_NONE = 0,
    STAFFREL_extended_between,
    STAFFREL_extended_within,
    STAFFREL_extended_MAX
};

/**
 * MEI data.STEMDIRECTION
 */
enum data_STEMDIRECTION {
    STEMDIRECTION_NONE = 0,
    STEMDIRECTION_up,
    STEMDIRECTION_down,
    STEMDIRECTION_left,
    STEMDIRECTION_right,
    STEMDIRECTION_ne,
    STEMDIRECTION_se,
    STEMDIRECTION_nw,
    STEMDIRECTION_sw,
    STEMDIRECTION_MAX
};

/**
 * MEI data.STEMDIRECTION.basic
 */
enum data_STEMDIRECTION_basic {
    STEMDIRECTION_basic_NONE = 0,
    STEMDIRECTION_basic_up,
    STEMDIRECTION_basic_down,
    STEMDIRECTION_basic_MAX
};

/**
 * MEI data.STEMDIRECTION.extended
 */
enum data_STEMDIRECTION_extended {
    STEMDIRECTION_extended_NONE = 0,
    STEMDIRECTION_extended_left,
    STEMDIRECTION_extended_right,
    STEMDIRECTION_extended_ne,
    STEMDIRECTION_extended_se,
    STEMDIRECTION_extended_nw,
    STEMDIRECTION_extended_sw,
    STEMDIRECTION_extended_MAX
};

/**
 * MEI data.STEMFORM.mensural
 */
enum data_STEMFORM_mensural {
    STEMFORM_mensural_NONE = 0,
    STEMFORM_mensural_circle,
    STEMFORM_mensural_oblique,
    STEMFORM_mensural_swallowtail,
    STEMFORM_mensural_virgula,
    STEMFORM_mensural_MAX
};

/**
 * MEI data.STEMMODIFIER
 */
enum data_STEMMODIFIER {
    STEMMODIFIER_NONE = 0,
    STEMMODIFIER_none,
    STEMMODIFIER_1slash,
    STEMMODIFIER_2slash,
    STEMMODIFIER_3slash,
    STEMMODIFIER_4slash,
    STEMMODIFIER_5slash,
    STEMMODIFIER_6slash,
    STEMMODIFIER_sprech,
    STEMMODIFIER_z,
    STEMMODIFIER_MAX
};

/**
 * MEI data.STEMPOSITION
 */
enum data_STEMPOSITION {
    STEMPOSITION_NONE = 0,
    STEMPOSITION_left,
    STEMPOSITION_right,
    STEMPOSITION_center,
    STEMPOSITION_MAX
};

/**
 * MEI data.TEMPERAMENT
 */
enum data_TEMPERAMENT {
    TEMPERAMENT_NONE = 0,
    TEMPERAMENT_equal,
    TEMPERAMENT_just,
    TEMPERAMENT_mean,
    TEMPERAMENT_pythagorean,
    TEMPERAMENT_MAX
};

/**
 * MEI data.TEXTRENDITION
 */
enum data_TEXTRENDITION {
    TEXTRENDITION_NONE = 0,
    TEXTRENDITION_quote,
    TEXTRENDITION_quotedbl,
    TEXTRENDITION_italic,
    TEXTRENDITION_oblique,
    TEXTRENDITION_smcaps,
    TEXTRENDITION_bold,
    TEXTRENDITION_bolder,
    TEXTRENDITION_lighter,
    TEXTRENDITION_box,
    TEXTRENDITION_circle,
    TEXTRENDITION_dbox,
    TEXTRENDITION_tbox,
    TEXTRENDITION_bslash,
    TEXTRENDITION_fslash,
    TEXTRENDITION_line_through,
    TEXTRENDITION_none,
    TEXTRENDITION_overline,
    TEXTRENDITION_overstrike,
    TEXTRENDITION_strike,
    TEXTRENDITION_sub,
    TEXTRENDITION_sup,
    TEXTRENDITION_superimpose,
    TEXTRENDITION_underline,
    TEXTRENDITION_x_through,
    TEXTRENDITION_ltr,
    TEXTRENDITION_rtl,
    TEXTRENDITION_lro,
    TEXTRENDITION_rlo,
    TEXTRENDITION_MAX
};

/**
 * MEI data.TEXTRENDITIONLIST
 */
enum data_TEXTRENDITIONLIST {
    TEXTRENDITIONLIST_NONE = 0,
    TEXTRENDITIONLIST_quote,
    TEXTRENDITIONLIST_quotedbl,
    TEXTRENDITIONLIST_italic,
    TEXTRENDITIONLIST_oblique,
    TEXTRENDITIONLIST_smcaps,
    TEXTRENDITIONLIST_bold,
    TEXTRENDITIONLIST_bolder,
    TEXTRENDITIONLIST_lighter,
    TEXTRENDITIONLIST_box,
    TEXTRENDITIONLIST_circle,
    TEXTRENDITIONLIST_dbox,
    TEXTRENDITIONLIST_tbox,
    TEXTRENDITIONLIST_bslash,
    TEXTRENDITIONLIST_fslash,
    TEXTRENDITIONLIST_line_through,
    TEXTRENDITIONLIST_none,
    TEXTRENDITIONLIST_overline,
    TEXTRENDITIONLIST_overstrike,
    TEXTRENDITIONLIST_strike,
    TEXTRENDITIONLIST_sub,
    TEXTRENDITIONLIST_sup,
    TEXTRENDITIONLIST_superimpose,
    TEXTRENDITIONLIST_underline,
    TEXTRENDITIONLIST_x_through,
    TEXTRENDITIONLIST_ltr,
    TEXTRENDITIONLIST_rtl,
    TEXTRENDITIONLIST_lro,
    TEXTRENDITIONLIST_rlo,
    TEXTRENDITIONLIST_MAX
};

/**
 * MEI data.VERTICALALIGNMENT
 */
enum data_VERTICALALIGNMENT {
    VERTICALALIGNMENT_NONE = 0,
    VERTICALALIGNMENT_top,
    VERTICALALIGNMENT_middle,
    VERTICALALIGNMENT_bottom,
    VERTICALALIGNMENT_baseline,
    VERTICALALIGNMENT_MAX
};

/**
 * MEI att.accid.log@func
 */
enum accidLog_FUNC {
    accidLog_FUNC_NONE = 0,
    accidLog_FUNC_caution,
    accidLog_FUNC_edit,
    accidLog_FUNC_MAX
};

/**
 * MEI att.anchoredText.log@func
 */
enum anchoredTextLog_FUNC {
    anchoredTextLog_FUNC_NONE = 0,
    anchoredTextLog_FUNC_unknown,
    anchoredTextLog_FUNC_MAX
};

/**
 * MEI att.arpeg.log@order
 */
enum arpegLog_ORDER {
    arpegLog_ORDER_NONE = 0,
    arpegLog_ORDER_up,
    arpegLog_ORDER_down,
    arpegLog_ORDER_nonarp,
    arpegLog_ORDER_MAX
};

/**
 * MEI att.audience@audience
 */
enum audience_AUDIENCE {
    audience_AUDIENCE_NONE = 0,
    audience_AUDIENCE_private,
    audience_AUDIENCE_public,
    audience_AUDIENCE_MAX
};

/**
 * MEI att.bTrem.log@form
 */
enum bTremLog_FORM {
    bTremLog_FORM_NONE = 0,
    bTremLog_FORM_meas,
    bTremLog_FORM_unmeas,
    bTremLog_FORM_MAX
};

/**
 * MEI att.beamRend@form
 */
enum beamRend_FORM {
    beamRend_FORM_NONE = 0,
    beamRend_FORM_acc,
    beamRend_FORM_mixed,
    beamRend_FORM_rit,
    beamRend_FORM_norm,
    beamRend_FORM_MAX
};

/**
 * MEI att.beaming.vis@beam.rend
 */
enum beamingVis_BEAMREND {
    beamingVis_BEAMREND_NONE = 0,
    beamingVis_BEAMREND_acc,
    beamingVis_BEAMREND_rit,
    beamingVis_BEAMREND_norm,
    beamingVis_BEAMREND_MAX
};

/**
 * MEI att.bracketSpan.log@func
 */
enum bracketSpanLog_FUNC {
    bracketSpanLog_FUNC_NONE = 0,
    bracketSpanLog_FUNC_coloration,
    bracketSpanLog_FUNC_cross_rhythm,
    bracketSpanLog_FUNC_ligature,
    bracketSpanLog_FUNC_MAX
};

/**
 * MEI att.curvature@curvedir
 */
enum curvature_CURVEDIR {
    curvature_CURVEDIR_NONE = 0,
    curvature_CURVEDIR_above,
    curvature_CURVEDIR_below,
    curvature_CURVEDIR_mixed,
    curvature_CURVEDIR_MAX
};

/**
 * MEI att.curve.log@func
 */
enum curveLog_FUNC {
    curveLog_FUNC_NONE = 0,
    curveLog_FUNC_unknown,
    curveLog_FUNC_MAX
};

/**
 * MEI att.cutout@cutout
 */
enum cutout_CUTOUT {
    cutout_CUTOUT_NONE = 0,
    cutout_CUTOUT_cutout,
    cutout_CUTOUT_MAX
};

/**
 * MEI att.dot.log@form
 */
enum dotLog_FORM {
    dotLog_FORM_NONE = 0,
    dotLog_FORM_aug,
    dotLog_FORM_div,
    dotLog_FORM_MAX
};

/**
 * MEI att.endings@ending.rend
 */
enum endings_ENDINGREND {
    endings_ENDINGREND_NONE = 0,
    endings_ENDINGREND_top,
    endings_ENDINGREND_barred,
    endings_ENDINGREND_grouped,
    endings_ENDINGREND_MAX
};

/**
 * MEI att.episema.vis@form
 */
enum episemaVis_FORM {
    episemaVis_FORM_NONE = 0,
    episemaVis_FORM_h,
    episemaVis_FORM_v,
    episemaVis_FORM_MAX
};

/**
 * MEI att.evidence@evidence
 */
enum evidence_EVIDENCE {
    evidence_EVIDENCE_NONE = 0,
    evidence_EVIDENCE_internal,
    evidence_EVIDENCE_external,
    evidence_EVIDENCE_conjecture,
    evidence_EVIDENCE_MAX
};

/**
 * MEI att.extSym@glyph.auth
 */
enum extSym_GLYPHAUTH {
    extSym_GLYPHAUTH_NONE = 0,
    extSym_GLYPHAUTH_smufl,
    extSym_GLYPHAUTH_MAX
};

/**
 * MEI att.fTrem.log@form
 */
enum fTremLog_FORM {
    fTremLog_FORM_NONE = 0,
    fTremLog_FORM_meas,
    fTremLog_FORM_unmeas,
    fTremLog_FORM_MAX
};

/**
 * MEI att.fermata.vis@form
 */
enum fermataVis_FORM {
    fermataVis_FORM_NONE = 0,
    fermataVis_FORM_inv,
    fermataVis_FORM_norm,
    fermataVis_FORM_MAX
};

/**
 * MEI att.fermata.vis@shape
 */
enum fermataVis_SHAPE {
    fermataVis_SHAPE_NONE = 0,
    fermataVis_SHAPE_curved,
    fermataVis_SHAPE_square,
    fermataVis_SHAPE_angular,
    fermataVis_SHAPE_MAX
};

/**
 * MEI att.fingGrp.log@form
 */
enum fingGrpLog_FORM {
    fingGrpLog_FORM_NONE = 0,
    fingGrpLog_FORM_alter,
    fingGrpLog_FORM_combi,
    fingGrpLog_FORM_subst,
    fingGrpLog_FORM_MAX
};

/**
 * MEI att.fingGrp.vis@orient
 */
enum fingGrpVis_ORIENT {
    fingGrpVis_ORIENT_NONE = 0,
    fingGrpVis_ORIENT_horiz,
    fingGrpVis_ORIENT_vert,
    fingGrpVis_ORIENT_MAX
};

/**
 * MEI att.graceGrp.log@attach
 */
enum graceGrpLog_ATTACH {
    graceGrpLog_ATTACH_NONE = 0,
    graceGrpLog_ATTACH_pre,
    graceGrpLog_ATTACH_post,
    graceGrpLog_ATTACH_unknown,
    graceGrpLog_ATTACH_MAX
};

/**
 * MEI att.hairpin.log@form
 */
enum hairpinLog_FORM {
    hairpinLog_FORM_NONE = 0,
    hairpinLog_FORM_cres,
    hairpinLog_FORM_dim,
    hairpinLog_FORM_MAX
};

/**
 * MEI att.harm.anl@form
 */
enum harmAnl_FORM {
    harmAnl_FORM_NONE = 0,
    harmAnl_FORM_explicit,
    harmAnl_FORM_implied,
    harmAnl_FORM_MAX
};

/**
 * MEI att.harm.vis@rendgrid
 */
enum harmVis_RENDGRID {
    harmVis_RENDGRID_NONE = 0,
    harmVis_RENDGRID_grid,
    harmVis_RENDGRID_gridtext,
    harmVis_RENDGRID_text,
    harmVis_RENDGRID_MAX
};

/**
 * MEI att.harpPedal.log@a
 */
enum harpPedalLog_A {
    harpPedalLog_A_NONE = 0,
    harpPedalLog_A_f,
    harpPedalLog_A_n,
    harpPedalLog_A_s,
    harpPedalLog_A_MAX
};

/**
 * MEI att.harpPedal.log@b
 */
enum harpPedalLog_B {
    harpPedalLog_B_NONE = 0,
    harpPedalLog_B_f,
    harpPedalLog_B_n,
    harpPedalLog_B_s,
    harpPedalLog_B_MAX
};

/**
 * MEI att.harpPedal.log@c
 */
enum harpPedalLog_C {
    harpPedalLog_C_NONE = 0,
    harpPedalLog_C_f,
    harpPedalLog_C_n,
    harpPedalLog_C_s,
    harpPedalLog_C_MAX
};

/**
 * MEI att.harpPedal.log@d
 */
enum harpPedalLog_D {
    harpPedalLog_D_NONE = 0,
    harpPedalLog_D_f,
    harpPedalLog_D_n,
    harpPedalLog_D_s,
    harpPedalLog_D_MAX
};

/**
 * MEI att.harpPedal.log@e
 */
enum harpPedalLog_E {
    harpPedalLog_E_NONE = 0,
    harpPedalLog_E_f,
    harpPedalLog_E_n,
    harpPedalLog_E_s,
    harpPedalLog_E_MAX
};

/**
 * MEI att.harpPedal.log@f
 */
enum harpPedalLog_F {
    harpPedalLog_F_NONE = 0,
    harpPedalLog_F_f,
    harpPedalLog_F_n,
    harpPedalLog_F_s,
    harpPedalLog_F_MAX
};

/**
 * MEI att.harpPedal.log@g
 */
enum harpPedalLog_G {
    harpPedalLog_G_NONE = 0,
    harpPedalLog_G_f,
    harpPedalLog_G_n,
    harpPedalLog_G_s,
    harpPedalLog_G_MAX
};

/**
 * MEI att.line.log@func
 */
enum lineLog_FUNC {
    lineLog_FUNC_NONE = 0,
    lineLog_FUNC_coloration,
    lineLog_FUNC_ligature,
    lineLog_FUNC_unknown,
    lineLog_FUNC_MAX
};

/**
 * MEI att.liquescent.vis@curve
 */
enum liquescentVis_CURVE {
    liquescentVis_CURVE_NONE = 0,
    liquescentVis_CURVE_a,
    liquescentVis_CURVE_c,
    liquescentVis_CURVE_MAX
};

/**
 * MEI att.measurement@unit
 */
enum measurement_UNIT {
    measurement_UNIT_NONE = 0,
    measurement_UNIT_byte,
    measurement_UNIT_char,
    measurement_UNIT_cm,
    measurement_UNIT_deg,
    measurement_UNIT_in,
    measurement_UNIT_issue,
    measurement_UNIT_ft,
    measurement_UNIT_m,
    measurement_UNIT_mm,
    measurement_UNIT_page,
    measurement_UNIT_pc,
    measurement_UNIT_pt,
    measurement_UNIT_px,
    measurement_UNIT_rad,
    measurement_UNIT_record,
    measurement_UNIT_vol,
    measurement_UNIT_vu,
    measurement_UNIT_MAX
};

/**
 * MEI att.meiVersion@meiversion
 */
enum meiVersion_MEIVERSION {
    meiVersion_MEIVERSION_NONE = 0,
    meiVersion_MEIVERSION_4_0_0,
    meiVersion_MEIVERSION_4_0_1,
    meiVersion_MEIVERSION_MAX
};

/**
 * MEI att.mensur.vis@form
 */
enum mensurVis_FORM {
    mensurVis_FORM_NONE = 0,
    mensurVis_FORM_horizontal,
    mensurVis_FORM_vertical,
    mensurVis_FORM_MAX
};

/**
 * MEI att.mensural.vis@mensur.form
 */
enum mensuralVis_MENSURFORM {
    mensuralVis_MENSURFORM_NONE = 0,
    mensuralVis_MENSURFORM_horizontal,
    mensuralVis_MENSURFORM_vertical,
    mensuralVis_MENSURFORM_MAX
};

/**
 * MEI att.meterConformance@metcon
 */
enum meterConformance_METCON {
    meterConformance_METCON_NONE = 0,
    meterConformance_METCON_c,
    meterConformance_METCON_i,
    meterConformance_METCON_o,
    meterConformance_METCON_MAX
};

/**
 * MEI att.meterSigGrp.log@func
 */
enum meterSigGrpLog_FUNC {
    meterSigGrpLog_FUNC_NONE = 0,
    meterSigGrpLog_FUNC_alternating,
    meterSigGrpLog_FUNC_interchanging,
    meterSigGrpLog_FUNC_mixed,
    meterSigGrpLog_FUNC_MAX
};

/**
 * MEI att.mordent.log@form
 */
enum mordentLog_FORM {
    mordentLog_FORM_NONE = 0,
    mordentLog_FORM_lower,
    mordentLog_FORM_upper,
    mordentLog_FORM_MAX
};

/**
 * MEI att.ncForm@con
 */
enum ncForm_CON {
    ncForm_CON_NONE = 0,
    ncForm_CON_g,
    ncForm_CON_l,
    ncForm_CON_e,
    ncForm_CON_MAX
};

/**
 * MEI att.ncForm@curve
 */
enum ncForm_CURVE {
    ncForm_CURVE_NONE = 0,
    ncForm_CURVE_a,
    ncForm_CURVE_c,
    ncForm_CURVE_MAX
};

/**
 * MEI att.ncForm@rellen
 */
enum ncForm_RELLEN {
    ncForm_RELLEN_NONE = 0,
    ncForm_RELLEN_l,
    ncForm_RELLEN_s,
    ncForm_RELLEN_MAX
};

/**
 * MEI att.note.ges@extremis
 */
enum noteGes_EXTREMIS {
    noteGes_EXTREMIS_NONE = 0,
    noteGes_EXTREMIS_highest,
    noteGes_EXTREMIS_lowest,
    noteGes_EXTREMIS_MAX
};

/**
 * MEI att.noteHeads@head.auth
 */
enum noteHeads_HEADAUTH {
    noteHeads_HEADAUTH_NONE = 0,
    noteHeads_HEADAUTH_smufl,
    noteHeads_HEADAUTH_MAX
};

/**
 * MEI att.octave.log@coll
 */
enum octaveLog_COLL {
    octaveLog_COLL_NONE = 0,
    octaveLog_COLL_coll,
    octaveLog_COLL_MAX
};

/**
 * MEI att.pb.vis@folium
 */
enum pbVis_FOLIUM {
    pbVis_FOLIUM_NONE = 0,
    pbVis_FOLIUM_verso,
    pbVis_FOLIUM_recto,
    pbVis_FOLIUM_MAX
};

/**
 * MEI att.pedal.log@dir
 */
enum pedalLog_DIR {
    pedalLog_DIR_NONE = 0,
    pedalLog_DIR_down,
    pedalLog_DIR_up,
    pedalLog_DIR_half,
    pedalLog_DIR_bounce,
    pedalLog_DIR_MAX
};

/**
 * MEI att.pedal.log@func
 */
enum pedalLog_FUNC {
    pedalLog_FUNC_NONE = 0,
    pedalLog_FUNC_sustain,
    pedalLog_FUNC_soft,
    pedalLog_FUNC_sostenuto,
    pedalLog_FUNC_silent,
    pedalLog_FUNC_MAX
};

/**
 * MEI att.pedal.vis@form
 */
enum pedalVis_FORM {
    pedalVis_FORM_NONE = 0,
    pedalVis_FORM_line,
    pedalVis_FORM_pedstar,
    pedalVis_FORM_altpedstar,
    pedalVis_FORM_MAX
};

/**
 * MEI att.pianoPedals@pedal.style
 */
enum pianoPedals_PEDALSTYLE {
    pianoPedals_PEDALSTYLE_NONE = 0,
    pianoPedals_PEDALSTYLE_line,
    pianoPedals_PEDALSTYLE_pedstar,
    pianoPedals_PEDALSTYLE_altpedstar,
    pianoPedals_PEDALSTYLE_MAX
};

/**
 * MEI att.pointing@xlink:actuate
 */
enum pointing_XLINKACTUATE {
    pointing_XLINKACTUATE_NONE = 0,
    pointing_XLINKACTUATE_onLoad,
    pointing_XLINKACTUATE_onRequest,
    pointing_XLINKACTUATE_none,
    pointing_XLINKACTUATE_other,
    pointing_XLINKACTUATE_MAX
};

/**
 * MEI att.pointing@xlink:show
 */
enum pointing_XLINKSHOW {
    pointing_XLINKSHOW_NONE = 0,
    pointing_XLINKSHOW_new,
    pointing_XLINKSHOW_replace,
    pointing_XLINKSHOW_embed,
    pointing_XLINKSHOW_none,
    pointing_XLINKSHOW_other,
    pointing_XLINKSHOW_MAX
};

/**
 * MEI att.recordType@recordtype
 */
enum recordType_RECORDTYPE {
    recordType_RECORDTYPE_NONE = 0,
    recordType_RECORDTYPE_a,
    recordType_RECORDTYPE_c,
    recordType_RECORDTYPE_d,
    recordType_RECORDTYPE_e,
    recordType_RECORDTYPE_f,
    recordType_RECORDTYPE_g,
    recordType_RECORDTYPE_i,
    recordType_RECORDTYPE_j,
    recordType_RECORDTYPE_k,
    recordType_RECORDTYPE_m,
    recordType_RECORDTYPE_o,
    recordType_RECORDTYPE_p,
    recordType_RECORDTYPE_r,
    recordType_RECORDTYPE_t,
    recordType_RECORDTYPE_MAX
};

/**
 * MEI att.regularMethod@method
 */
enum regularMethod_METHOD {
    regularMethod_METHOD_NONE = 0,
    regularMethod_METHOD_silent,
    regularMethod_METHOD_tags,
    regularMethod_METHOD_MAX
};

/**
 * MEI att.rehearsal@reh.enclose
 */
enum rehearsal_REHENCLOSE {
    rehearsal_REHENCLOSE_NONE = 0,
    rehearsal_REHENCLOSE_box,
    rehearsal_REHENCLOSE_circle,
    rehearsal_REHENCLOSE_none,
    rehearsal_REHENCLOSE_MAX
};

/**
 * MEI att.sb.vis@form
 */
enum sbVis_FORM {
    sbVis_FORM_NONE = 0,
    sbVis_FORM_hash,
    sbVis_FORM_MAX
};

/**
 * MEI att.staffGroupingSym@symbol
 */
enum staffGroupingSym_SYMBOL {
    staffGroupingSym_SYMBOL_NONE = 0,
    staffGroupingSym_SYMBOL_brace,
    staffGroupingSym_SYMBOL_bracket,
    staffGroupingSym_SYMBOL_bracketsq,
    staffGroupingSym_SYMBOL_line,
    staffGroupingSym_SYMBOL_none,
    staffGroupingSym_SYMBOL_MAX
};

/**
 * MEI att.syl.log@con
 */
enum sylLog_CON {
    sylLog_CON_NONE = 0,
    sylLog_CON_s,
    sylLog_CON_d,
    sylLog_CON_u,
    sylLog_CON_t,
    sylLog_CON_c,
    sylLog_CON_v,
    sylLog_CON_i,
    sylLog_CON_b,
    sylLog_CON_MAX
};

/**
 * MEI att.syl.log@wordpos
 */
enum sylLog_WORDPOS {
    sylLog_WORDPOS_NONE = 0,
    sylLog_WORDPOS_i,
    sylLog_WORDPOS_m,
    sylLog_WORDPOS_s,
    sylLog_WORDPOS_t,
    sylLog_WORDPOS_MAX
};

/**
 * MEI att.targetEval@evaluate
 */
enum targetEval_EVALUATE {
    targetEval_EVALUATE_NONE = 0,
    targetEval_EVALUATE_all,
    targetEval_EVALUATE_one,
    targetEval_EVALUATE_none,
    targetEval_EVALUATE_MAX
};

/**
 * MEI att.tempo.log@func
 */
enum tempoLog_FUNC {
    tempoLog_FUNC_NONE = 0,
    tempoLog_FUNC_continuous,
    tempoLog_FUNC_instantaneous,
    tempoLog_FUNC_metricmod,
    tempoLog_FUNC_precedente,
    tempoLog_FUNC_MAX
};

/**
 * MEI att.tuplet.vis@num.format
 */
enum tupletVis_NUMFORMAT {
    tupletVis_NUMFORMAT_NONE = 0,
    tupletVis_NUMFORMAT_count,
    tupletVis_NUMFORMAT_ratio,
    tupletVis_NUMFORMAT_MAX
};

/**
 * MEI att.turn.log@form
 */
enum turnLog_FORM {
    turnLog_FORM_NONE = 0,
    turnLog_FORM_lower,
    turnLog_FORM_upper,
    turnLog_FORM_MAX
};

/**
 * MEI att.voltaGroupingSym@voltasym
 */
enum voltaGroupingSym_VOLTASYM {
    voltaGroupingSym_VOLTASYM_NONE = 0,
    voltaGroupingSym_VOLTASYM_brace,
    voltaGroupingSym_VOLTASYM_bracket,
    voltaGroupingSym_VOLTASYM_bracketsq,
    voltaGroupingSym_VOLTASYM_line,
    voltaGroupingSym_VOLTASYM_none,
    voltaGroupingSym_VOLTASYM_MAX
};

/**
 * MEI att.whitespace@xml:space
 */
enum whitespace_XMLSPACE {
    whitespace_XMLSPACE_NONE = 0,
    whitespace_XMLSPACE_default,
    whitespace_XMLSPACE_preserve,
    whitespace_XMLSPACE_MAX
};


} // vrv namespace

#endif // __VRV_ATT_TYPES_H__

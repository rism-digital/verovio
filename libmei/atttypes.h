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
    ARTICULATION_marc_stacc,
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
    ARTICULATION_ten_stacc,
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
};

/**
 * MEI data.BARPLACE
 */
enum data_BARPLACE {
    BARPLACE_NONE = 0,
    BARPLACE_mensur,
    BARPLACE_staff,
    BARPLACE_takt,
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
};

/**
 * MEI data.BEAMPLACE
 */
enum data_BEAMPLACE {
    BEAMPLACE_NONE = 0,
    BEAMPLACE_above,
    BEAMPLACE_below,
    BEAMPLACE_mixed,
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
};

/**
 * MEI data.BOOLEAN
 */
enum data_BOOLEAN {
    BOOLEAN_NONE = 0,
    BOOLEAN_true,
    BOOLEAN_false,
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
};

/**
 * MEI data.ENCLOSURE
 */
enum data_ENCLOSURE {
    ENCLOSURE_NONE = 0,
    ENCLOSURE_paren,
    ENCLOSURE_brack,
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
    FONTSIZETERM_cue,
};

/**
 * MEI data.FONTSTYLE
 */
enum data_FONTSTYLE {
    FONTSTYLE_NONE = 0,
    FONTSTYLE_italic,
    FONTSTYLE_normal,
    FONTSTYLE_oblique,
};

/**
 * MEI data.FONTWEIGHT
 */
enum data_FONTWEIGHT {
    FONTWEIGHT_NONE = 0,
    FONTWEIGHT_bold,
    FONTWEIGHT_normal,
};

/**
 * MEI data.GLISSANDO
 */
enum data_GLISSANDO {
    GLISSANDO_NONE = 0,
    GLISSANDO_i,
    GLISSANDO_m,
    GLISSANDO_t,
};

/**
 * MEI data.GRACE
 */
enum data_GRACE {
    GRACE_NONE = 0,
    GRACE_acc,
    GRACE_unacc,
    GRACE_unknown,
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
};

/**
 * MEI data.LIGATUREFORM
 */
enum data_LIGATUREFORM {
    LIGATUREFORM_NONE = 0,
    LIGATUREFORM_recta,
    LIGATUREFORM_obliqua,
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
    LINESTARTENDSYMBOL_none,
};

/**
 * MEI data.LINEWIDTHTERM
 */
enum data_LINEWIDTHTERM {
    LINEWIDTHTERM_NONE = 0,
    LINEWIDTHTERM_narrow,
    LINEWIDTHTERM_medium,
    LINEWIDTHTERM_wide,
};

/**
 * MEI data.MENSURATIONSIGN
 */
enum data_MENSURATIONSIGN {
    MENSURATIONSIGN_NONE = 0,
    MENSURATIONSIGN_C,
    MENSURATIONSIGN_O,
};

/**
 * MEI data.METERSIGN
 */
enum data_METERSIGN {
    METERSIGN_NONE = 0,
    METERSIGN_common,
    METERSIGN_cut,
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
    MODE_locrian,
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
};

/**
 * MEI data.OTHERSTAFF
 */
enum data_OTHERSTAFF {
    OTHERSTAFF_NONE = 0,
    OTHERSTAFF_above,
    OTHERSTAFF_below,
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
};

/**
 * MEI data.STAFFITEM.cmn
 */
enum data_STAFFITEM_cmn {
    STAFFITEM_cmn_NONE = 0,
    STAFFITEM_cmn_beam,
    STAFFITEM_cmn_bend,
    STAFFITEM_cmn_breath,
    STAFFITEM_cmn_cpMark,
    STAFFITEM_cmn_fermata,
    STAFFITEM_cmn_fing,
    STAFFITEM_cmn_hairpin,
    STAFFITEM_cmn_harpPedal,
    STAFFITEM_cmn_ligatureSpan,
    STAFFITEM_cmn_lv,
    STAFFITEM_cmn_mordent,
    STAFFITEM_cmn_octave,
    STAFFITEM_cmn_pedal,
    STAFFITEM_cmn_reh,
    STAFFITEM_cmn_tie,
    STAFFITEM_cmn_trill,
    STAFFITEM_cmn_tuplet,
    STAFFITEM_cmn_turn,
};

/**
 * MEI data.STAFFITEM.mensural
 */
enum data_STAFFITEM_mensural {
    STAFFITEM_mensural_NONE = 0,
    STAFFITEM_mensural_ligature,
};

/**
 * MEI data.STAFFREL.basic
 */
enum data_STAFFREL_basic {
    STAFFREL_basic_NONE = 0,
    STAFFREL_basic_above,
    STAFFREL_basic_below,
};

/**
 * MEI data.STAFFREL.extended
 */
enum data_STAFFREL_extended {
    STAFFREL_extended_NONE = 0,
    STAFFREL_extended_between,
    STAFFREL_extended_within,
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
};

/**
 * MEI data.STEMPOSITION
 */
enum data_STEMPOSITION {
    STEMPOSITION_NONE = 0,
    STEMPOSITION_left,
    STEMPOSITION_right,
    STEMPOSITION_center,
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
};

/**
 * MEI att.accid.log@func
 */
enum accidLog_FUNC {
    accidLog_FUNC_NONE = 0,
    accidLog_FUNC_caution,
    accidLog_FUNC_edit,
};

/**
 * MEI att.arpeg.log@order
 */
enum arpegLog_ORDER {
    arpegLog_ORDER_NONE = 0,
    arpegLog_ORDER_up,
    arpegLog_ORDER_down,
    arpegLog_ORDER_nonarp,
};

/**
 * MEI att.audience@audience
 */
enum audience_AUDIENCE {
    audience_AUDIENCE_NONE = 0,
    audience_AUDIENCE_private,
    audience_AUDIENCE_public,
};

/**
 * MEI att.bTrem.log@form
 */
enum bTremLog_FORM {
    bTremLog_FORM_NONE = 0,
    bTremLog_FORM_meas,
    bTremLog_FORM_unmeas,
};

/**
 * MEI att.curvature@curvedir
 */
enum curvature_CURVEDIR {
    curvature_CURVEDIR_NONE = 0,
    curvature_CURVEDIR_above,
    curvature_CURVEDIR_below,
    curvature_CURVEDIR_mixed,
};

/**
 * MEI att.dot.log@form
 */
enum dotLog_FORM {
    dotLog_FORM_NONE = 0,
    dotLog_FORM_aug,
    dotLog_FORM_div,
};

/**
 * MEI att.fTrem.log@form
 */
enum fTremLog_FORM {
    fTremLog_FORM_NONE = 0,
    fTremLog_FORM_meas,
    fTremLog_FORM_unmeas,
};

/**
 * MEI att.graceGrp.log@attach
 */
enum graceGrpLog_ATTACH {
    graceGrpLog_ATTACH_NONE = 0,
    graceGrpLog_ATTACH_pre,
    graceGrpLog_ATTACH_post,
    graceGrpLog_ATTACH_unknown,
};

/**
 * MEI att.hairpin.log@form
 */
enum hairpinLog_FORM {
    hairpinLog_FORM_NONE = 0,
    hairpinLog_FORM_cres,
    hairpinLog_FORM_dim,
};

/**
 * MEI att.harpPedal.log@a
 */
enum harpPedalLog_A {
    harpPedalLog_A_NONE = 0,
    harpPedalLog_A_f,
    harpPedalLog_A_n,
    harpPedalLog_A_s,
};

/**
 * MEI att.harpPedal.log@b
 */
enum harpPedalLog_B {
    harpPedalLog_B_NONE = 0,
    harpPedalLog_B_f,
    harpPedalLog_B_n,
    harpPedalLog_B_s,
};

/**
 * MEI att.harpPedal.log@c
 */
enum harpPedalLog_C {
    harpPedalLog_C_NONE = 0,
    harpPedalLog_C_f,
    harpPedalLog_C_n,
    harpPedalLog_C_s,
};

/**
 * MEI att.harpPedal.log@d
 */
enum harpPedalLog_D {
    harpPedalLog_D_NONE = 0,
    harpPedalLog_D_f,
    harpPedalLog_D_n,
    harpPedalLog_D_s,
};

/**
 * MEI att.harpPedal.log@e
 */
enum harpPedalLog_E {
    harpPedalLog_E_NONE = 0,
    harpPedalLog_E_f,
    harpPedalLog_E_n,
    harpPedalLog_E_s,
};

/**
 * MEI att.harpPedal.log@f
 */
enum harpPedalLog_F {
    harpPedalLog_F_NONE = 0,
    harpPedalLog_F_f,
    harpPedalLog_F_n,
    harpPedalLog_F_s,
};

/**
 * MEI att.harpPedal.log@g
 */
enum harpPedalLog_G {
    harpPedalLog_G_NONE = 0,
    harpPedalLog_G_f,
    harpPedalLog_G_n,
    harpPedalLog_G_s,
};

/**
 * MEI att.meiVersion@meiversion
 */
enum meiVersion_MEIVERSION {
    meiVersion_MEIVERSION_NONE = 0,
    meiVersion_MEIVERSION_4_0_0,
};

/**
 * MEI att.meterConformance@metcon
 */
enum meterConformance_METCON {
    meterConformance_METCON_NONE = 0,
    meterConformance_METCON_c,
    meterConformance_METCON_i,
    meterConformance_METCON_o,
};

/**
 * MEI att.meterSigGrp.log@func
 */
enum meterSigGrpLog_FUNC {
    meterSigGrpLog_FUNC_NONE = 0,
    meterSigGrpLog_FUNC_alternating,
    meterSigGrpLog_FUNC_interchanging,
    meterSigGrpLog_FUNC_mixed,
};

/**
 * MEI att.mordent.log@form
 */
enum mordentLog_FORM {
    mordentLog_FORM_NONE = 0,
    mordentLog_FORM_lower,
    mordentLog_FORM_upper,
};

/**
 * MEI att.octave.log@coll
 */
enum octaveLog_COLL {
    octaveLog_COLL_NONE = 0,
    octaveLog_COLL_coll,
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
};

/**
 * MEI att.regularMethod@method
 */
enum regularMethod_METHOD {
    regularMethod_METHOD_NONE = 0,
    regularMethod_METHOD_silent,
    regularMethod_METHOD_tags,
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
};

/**
 * MEI att.syl.log@wordpos
 */
enum sylLog_WORDPOS {
    sylLog_WORDPOS_NONE = 0,
    sylLog_WORDPOS_i,
    sylLog_WORDPOS_m,
    sylLog_WORDPOS_t,
};

/**
 * MEI att.targetEval@evaluate
 */
enum targetEval_EVALUATE {
    targetEval_EVALUATE_NONE = 0,
    targetEval_EVALUATE_all,
    targetEval_EVALUATE_one,
    targetEval_EVALUATE_none,
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
};

/**
 * MEI att.turn.log@form
 */
enum turnLog_FORM {
    turnLog_FORM_NONE = 0,
    turnLog_FORM_lower,
    turnLog_FORM_upper,
};

/**
 * MEI att.whitespace@xml:space
 */
enum whitespace_XMLSPACE {
    whitespace_XMLSPACE_NONE = 0,
    whitespace_XMLSPACE_default,
    whitespace_XMLSPACE_preserve,
};



} // vrv namespace

#endif // __VRV_ATT_TYPES_H__

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
    ACCIDENTAL_EXPLICIT_sx,
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
    ACCIDENTAL_EXPLICIT_nd,
    ACCIDENTAL_EXPLICIT_1qf,
    ACCIDENTAL_EXPLICIT_3qf,
    ACCIDENTAL_EXPLICIT_1qs,
    ACCIDENTAL_EXPLICIT_3qs,
};

/**
 * MEI data.ACCIDENTAL.IMPLICIT
 */
enum data_ACCIDENTAL_IMPLICIT {
    ACCIDENTAL_IMPLICIT_NONE = 0,
    ACCIDENTAL_IMPLICIT_s,
    ACCIDENTAL_IMPLICIT_f,
    ACCIDENTAL_IMPLICIT_ss,
    ACCIDENTAL_IMPLICIT_ff,
    ACCIDENTAL_IMPLICIT_n,
    ACCIDENTAL_IMPLICIT_su,
    ACCIDENTAL_IMPLICIT_sd,
    ACCIDENTAL_IMPLICIT_fu,
    ACCIDENTAL_IMPLICIT_fd,
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
 * MEI data.CLUSTER
 */
enum data_CLUSTER {
    CLUSTER_NONE = 0,
    CLUSTER_white,
    CLUSTER_black,
    CLUSTER_chromatic,
};

/**
 * MEI data.COLORNAMES
 */
enum data_COLORNAMES {
    COLORNAMES_NONE = 0,
    COLORNAMES_aqua,
    COLORNAMES_black,
    COLORNAMES_blue,
    COLORNAMES_fuchsia,
    COLORNAMES_gray,
    COLORNAMES_green,
    COLORNAMES_lime,
    COLORNAMES_maroon,
    COLORNAMES_navy,
    COLORNAMES_olive,
    COLORNAMES_purple,
    COLORNAMES_red,
    COLORNAMES_silver,
    COLORNAMES_teal,
    COLORNAMES_white,
    COLORNAMES_yellow,
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
    FONTSIZETERM_medium,
    FONTSIZETERM_large,
    FONTSIZETERM_x_large,
    FONTSIZETERM_xx_large,
    FONTSIZETERM_smaller,
    FONTSIZETERM_larger,
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
 * MEI data.INEUMEFORM
 */
enum data_INEUMEFORM {
    INEUMEFORM_NONE = 0,
    INEUMEFORM_liquescent1,
    INEUMEFORM_liquescent2,
    INEUMEFORM_tied,
    INEUMEFORM_tiedliquescent1,
    INEUMEFORM_tiedliquescent2,
};

/**
 * MEI data.INEUMENAME
 */
enum data_INEUMENAME {
    INEUMENAME_NONE = 0,
    INEUMENAME_pessubpunctis,
    INEUMENAME_climacus,
    INEUMENAME_scandicus,
    INEUMENAME_bistropha,
    INEUMENAME_tristropha,
    INEUMENAME_pressusminor,
    INEUMENAME_pressusmaior,
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
 * MEI data.PLACE
 */
enum data_PLACE {
    PLACE_NONE = 0,
    PLACE_above,
    PLACE_below,
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
 * MEI data.SIZE
 */
enum data_SIZE {
    SIZE_NONE = 0,
    SIZE_normal,
    SIZE_cue,
};

/**
 * MEI data.STAFFREL
 */
enum data_STAFFREL {
    STAFFREL_NONE = 0,
    STAFFREL_above,
    STAFFREL_below,
    STAFFREL_within,
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
 * MEI data.TEMPERAMENT
 */
enum data_TEMPERAMENT {
    TEMPERAMENT_NONE = 0,
    TEMPERAMENT_equal,
    TEMPERAMENT_just,
    TEMPERAMENT_mean,
    TEMPERAMENT_pythagorean,
};

/**
 * MEI data.TEXTRENDITIONLIST
 */
enum data_TEXTRENDITIONLIST {
    TEXTRENDITIONLIST_NONE = 0,
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
    TEXTRENDITIONLIST_underline,
    TEXTRENDITIONLIST_ltr,
    TEXTRENDITIONLIST_rtl,
    TEXTRENDITIONLIST_lro,
    TEXTRENDITIONLIST_rlo,
};

/**
 * MEI data.UNEUMEFORM
 */
enum data_UNEUMEFORM {
    UNEUMEFORM_NONE = 0,
    UNEUMEFORM_liquescent1,
    UNEUMEFORM_liquescent2,
    UNEUMEFORM_liquescent3,
    UNEUMEFORM_quilismatic,
    UNEUMEFORM_rectangular,
    UNEUMEFORM_rhombic,
    UNEUMEFORM_tied,
};

/**
 * MEI data.UNEUMENAME
 */
enum data_UNEUMENAME {
    UNEUMENAME_NONE = 0,
    UNEUMENAME_punctum,
    UNEUMENAME_virga,
    UNEUMENAME_pes,
    UNEUMENAME_clivis,
    UNEUMENAME_torculus,
    UNEUMENAME_torculusresupinus,
    UNEUMENAME_porrectus,
    UNEUMENAME_porrectusflexus,
    UNEUMENAME_apostropha,
    UNEUMENAME_oriscus,
    UNEUMENAME_pressusmaior,
    UNEUMENAME_pressusminor,
    UNEUMENAME_virgastrata,
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
 * MEI att.bTrem.log@form
 */
enum bTremLog_FORM {
    bTremLog_FORM_NONE = 0,
    bTremLog_FORM_meas,
    bTremLog_FORM_unmeas,
};

/**
 * MEI att.beaming.vis@beam.rend
 */
enum beamingVis_BEAMREND {
    beamingVis_BEAMREND_NONE = 0,
    beamingVis_BEAMREND_acc,
    beamingVis_BEAMREND_rit,
    beamingVis_BEAMREND_norm,
};

/**
 * MEI att.beamrend@form
 */
enum beamrend_FORM {
    beamrend_FORM_NONE = 0,
    beamrend_FORM_acc,
    beamrend_FORM_mixed,
    beamrend_FORM_rit,
    beamrend_FORM_norm,
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
 * MEI att.cutout@cutout
 */
enum cutout_CUTOUT {
    cutout_CUTOUT_NONE = 0,
    cutout_CUTOUT_cutout,
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
 * MEI att.endings@ending.rend
 */
enum endings_ENDINGREND {
    endings_ENDINGREND_NONE = 0,
    endings_ENDINGREND_top,
    endings_ENDINGREND_barred,
    endings_ENDINGREND_grouped,
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
 * MEI att.fermata.vis@form
 */
enum fermataVis_FORM {
    fermataVis_FORM_NONE = 0,
    fermataVis_FORM_inv,
    fermataVis_FORM_norm,
};

/**
 * MEI att.fermata.vis@shape
 */
enum fermataVis_SHAPE {
    fermataVis_SHAPE_NONE = 0,
    fermataVis_SHAPE_curved,
    fermataVis_SHAPE_square,
    fermataVis_SHAPE_angular,
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
 * MEI att.harm.vis@rendgrid
 */
enum harmVis_RENDGRID {
    harmVis_RENDGRID_NONE = 0,
    harmVis_RENDGRID_grid,
    harmVis_RENDGRID_gridtext,
    harmVis_RENDGRID_text,
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
 * MEI att.meiversion@meiversion
 */
enum meiversion_MEIVERSION {
    meiversion_MEIVERSION_NONE = 0,
    meiversion_MEIVERSION_3_0_0,
};

/**
 * MEI att.mensur.vis@form
 */
enum mensurVis_FORM {
    mensurVis_FORM_NONE = 0,
    mensurVis_FORM_horizontal,
    mensurVis_FORM_vertical,
};

/**
 * MEI att.mensural.vis@mensur.form
 */
enum mensuralVis_MENSURFORM {
    mensuralVis_MENSURFORM_NONE = 0,
    mensuralVis_MENSURFORM_horizontal,
    mensuralVis_MENSURFORM_vertical,
};

/**
 * MEI att.meterSig.vis@form
 */
enum meterSigVis_FORM {
    meterSigVis_FORM_NONE = 0,
    meterSigVis_FORM_num,
    meterSigVis_FORM_denomsym,
    meterSigVis_FORM_norm,
    meterSigVis_FORM_invis,
};

/**
 * MEI att.meterSigDefault.vis@meter.rend
 */
enum meterSigDefaultVis_METERREND {
    meterSigDefaultVis_METERREND_NONE = 0,
    meterSigDefaultVis_METERREND_num,
    meterSigDefaultVis_METERREND_denomsym,
    meterSigDefaultVis_METERREND_norm,
    meterSigDefaultVis_METERREND_invis,
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
 * MEI att.meterconformance@metcon
 */
enum meterconformance_METCON {
    meterconformance_METCON_NONE = 0,
    meterconformance_METCON_c,
    meterconformance_METCON_i,
    meterconformance_METCON_o,
};

/**
 * MEI att.mordent.log@form
 */
enum mordentLog_FORM {
    mordentLog_FORM_NONE = 0,
    mordentLog_FORM_inv,
    mordentLog_FORM_norm,
};

/**
 * MEI att.note.log.mensural@lig
 */
enum noteLogMensural_LIG {
    noteLogMensural_LIG_NONE = 0,
    noteLogMensural_LIG_recta,
    noteLogMensural_LIG_obliqua,
};

/**
 * MEI att.octave.log@coll
 */
enum octaveLog_COLL {
    octaveLog_COLL_NONE = 0,
    octaveLog_COLL_coll,
};

/**
 * MEI att.pb.vis@folium
 */
enum pbVis_FOLIUM {
    pbVis_FOLIUM_NONE = 0,
    pbVis_FOLIUM_verso,
    pbVis_FOLIUM_recto,
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
 * MEI att.pedal.vis@form
 */
enum pedalVis_FORM {
    pedalVis_FORM_NONE = 0,
    pedalVis_FORM_line,
    pedalVis_FORM_pedstar,
    pedalVis_FORM_altpedstar,
};

/**
 * MEI att.pianopedals@pedal.style
 */
enum pianopedals_PEDALSTYLE {
    pianopedals_PEDALSTYLE_NONE = 0,
    pianopedals_PEDALSTYLE_line,
    pianopedals_PEDALSTYLE_pedstar,
    pianopedals_PEDALSTYLE_altpedstar,
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
 * MEI att.regularmethod@method
 */
enum regularmethod_METHOD {
    regularmethod_METHOD_NONE = 0,
    regularmethod_METHOD_silent,
    regularmethod_METHOD_tags,
};

/**
 * MEI att.rehearsal@reh.enclose
 */
enum rehearsal_REHENCLOSE {
    rehearsal_REHENCLOSE_NONE = 0,
    rehearsal_REHENCLOSE_box,
    rehearsal_REHENCLOSE_circle,
    rehearsal_REHENCLOSE_none,
};

/**
 * MEI att.sb.vis@form
 */
enum sbVis_FORM {
    sbVis_FORM_NONE = 0,
    sbVis_FORM_hash,
};

/**
 * MEI att.staffgroupingsym@symbol
 */
enum staffgroupingsym_SYMBOL {
    staffgroupingsym_SYMBOL_NONE = 0,
    staffgroupingsym_SYMBOL_brace,
    staffgroupingsym_SYMBOL_bracket,
    staffgroupingsym_SYMBOL_bracketsq,
    staffgroupingsym_SYMBOL_line,
    staffgroupingsym_SYMBOL_none,
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
 * MEI att.targeteval@evaluate
 */
enum targeteval_EVALUATE {
    targeteval_EVALUATE_NONE = 0,
    targeteval_EVALUATE_all,
    targeteval_EVALUATE_one,
    targeteval_EVALUATE_none,
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
 * MEI att.tuplet.vis@num.format
 */
enum tupletVis_NUMFORMAT {
    tupletVis_NUMFORMAT_NONE = 0,
    tupletVis_NUMFORMAT_count,
    tupletVis_NUMFORMAT_ratio,
};

/**
 * MEI att.turn.log@form
 */
enum turnLog_FORM {
    turnLog_FORM_NONE = 0,
    turnLog_FORM_inv,
    turnLog_FORM_norm,
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

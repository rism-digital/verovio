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

#include "attconverter.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// AttConverter
//----------------------------------------------------------------------------

std::string AttConverter::AccidentalWrittenToStr(data_ACCIDENTAL_WRITTEN data) const
{
    std::string value;
    switch (data) {
        case ACCIDENTAL_WRITTEN_s: value = "s"; break;
        case ACCIDENTAL_WRITTEN_f: value = "f"; break;
        case ACCIDENTAL_WRITTEN_ss: value = "ss"; break;
        case ACCIDENTAL_WRITTEN_x: value = "x"; break;
        case ACCIDENTAL_WRITTEN_ff: value = "ff"; break;
        case ACCIDENTAL_WRITTEN_xs: value = "xs"; break;
        case ACCIDENTAL_WRITTEN_sx: value = "sx"; break;
        case ACCIDENTAL_WRITTEN_ts: value = "ts"; break;
        case ACCIDENTAL_WRITTEN_tf: value = "tf"; break;
        case ACCIDENTAL_WRITTEN_n: value = "n"; break;
        case ACCIDENTAL_WRITTEN_nf: value = "nf"; break;
        case ACCIDENTAL_WRITTEN_ns: value = "ns"; break;
        case ACCIDENTAL_WRITTEN_su: value = "su"; break;
        case ACCIDENTAL_WRITTEN_sd: value = "sd"; break;
        case ACCIDENTAL_WRITTEN_fu: value = "fu"; break;
        case ACCIDENTAL_WRITTEN_fd: value = "fd"; break;
        case ACCIDENTAL_WRITTEN_nu: value = "nu"; break;
        case ACCIDENTAL_WRITTEN_nd: value = "nd"; break;
        case ACCIDENTAL_WRITTEN_1qf: value = "1qf"; break;
        case ACCIDENTAL_WRITTEN_3qf: value = "3qf"; break;
        case ACCIDENTAL_WRITTEN_1qs: value = "1qs"; break;
        case ACCIDENTAL_WRITTEN_3qs: value = "3qs"; break;
        default:
            LogWarning("Unknown value '%d' for data.ACCIDENTAL.WRITTEN", data);
            value = "";
            break;
    }
    return value;
}

data_ACCIDENTAL_WRITTEN AttConverter::StrToAccidentalWritten(std::string value) const
{
    if (value == "s") return ACCIDENTAL_WRITTEN_s;
    if (value == "f") return ACCIDENTAL_WRITTEN_f;
    if (value == "ss") return ACCIDENTAL_WRITTEN_ss;
    if (value == "x") return ACCIDENTAL_WRITTEN_x;
    if (value == "ff") return ACCIDENTAL_WRITTEN_ff;
    if (value == "xs") return ACCIDENTAL_WRITTEN_xs;
    if (value == "sx") return ACCIDENTAL_WRITTEN_sx;
    if (value == "ts") return ACCIDENTAL_WRITTEN_ts;
    if (value == "tf") return ACCIDENTAL_WRITTEN_tf;
    if (value == "n") return ACCIDENTAL_WRITTEN_n;
    if (value == "nf") return ACCIDENTAL_WRITTEN_nf;
    if (value == "ns") return ACCIDENTAL_WRITTEN_ns;
    if (value == "su") return ACCIDENTAL_WRITTEN_su;
    if (value == "sd") return ACCIDENTAL_WRITTEN_sd;
    if (value == "fu") return ACCIDENTAL_WRITTEN_fu;
    if (value == "fd") return ACCIDENTAL_WRITTEN_fd;
    if (value == "nu") return ACCIDENTAL_WRITTEN_nu;
    if (value == "nd") return ACCIDENTAL_WRITTEN_nd;
    if (value == "1qf") return ACCIDENTAL_WRITTEN_1qf;
    if (value == "3qf") return ACCIDENTAL_WRITTEN_3qf;
    if (value == "1qs") return ACCIDENTAL_WRITTEN_1qs;
    if (value == "3qs") return ACCIDENTAL_WRITTEN_3qs;
    LogWarning("Unsupported value '%s' for data.ACCIDENTAL.WRITTEN", value.c_str());
    return ACCIDENTAL_WRITTEN_NONE;
}

std::string AttConverter::ArticulationToStr(data_ARTICULATION data) const
{
    std::string value;
    switch (data) {
        case ARTICULATION_acc: value = "acc"; break;
        case ARTICULATION_stacc: value = "stacc"; break;
        case ARTICULATION_ten: value = "ten"; break;
        case ARTICULATION_stacciss: value = "stacciss"; break;
        case ARTICULATION_marc: value = "marc"; break;
        case ARTICULATION_marc_stacc: value = "marc-stacc"; break;
        case ARTICULATION_spicc: value = "spicc"; break;
        case ARTICULATION_doit: value = "doit"; break;
        case ARTICULATION_scoop: value = "scoop"; break;
        case ARTICULATION_rip: value = "rip"; break;
        case ARTICULATION_plop: value = "plop"; break;
        case ARTICULATION_fall: value = "fall"; break;
        case ARTICULATION_longfall: value = "longfall"; break;
        case ARTICULATION_bend: value = "bend"; break;
        case ARTICULATION_flip: value = "flip"; break;
        case ARTICULATION_smear: value = "smear"; break;
        case ARTICULATION_shake: value = "shake"; break;
        case ARTICULATION_dnbow: value = "dnbow"; break;
        case ARTICULATION_upbow: value = "upbow"; break;
        case ARTICULATION_harm: value = "harm"; break;
        case ARTICULATION_snap: value = "snap"; break;
        case ARTICULATION_fingernail: value = "fingernail"; break;
        case ARTICULATION_ten_stacc: value = "ten-stacc"; break;
        case ARTICULATION_damp: value = "damp"; break;
        case ARTICULATION_dampall: value = "dampall"; break;
        case ARTICULATION_open: value = "open"; break;
        case ARTICULATION_stop: value = "stop"; break;
        case ARTICULATION_dbltongue: value = "dbltongue"; break;
        case ARTICULATION_trpltongue: value = "trpltongue"; break;
        case ARTICULATION_heel: value = "heel"; break;
        case ARTICULATION_toe: value = "toe"; break;
        case ARTICULATION_tap: value = "tap"; break;
        case ARTICULATION_lhpizz: value = "lhpizz"; break;
        case ARTICULATION_dot: value = "dot"; break;
        case ARTICULATION_stroke: value = "stroke"; break;
        default:
            LogWarning("Unknown value '%d' for data.ARTICULATION", data);
            value = "";
            break;
    }
    return value;
}

data_ARTICULATION AttConverter::StrToArticulation(std::string value) const
{
    if (value == "acc") return ARTICULATION_acc;
    if (value == "stacc") return ARTICULATION_stacc;
    if (value == "ten") return ARTICULATION_ten;
    if (value == "stacciss") return ARTICULATION_stacciss;
    if (value == "marc") return ARTICULATION_marc;
    if (value == "marc-stacc") return ARTICULATION_marc_stacc;
    if (value == "spicc") return ARTICULATION_spicc;
    if (value == "doit") return ARTICULATION_doit;
    if (value == "scoop") return ARTICULATION_scoop;
    if (value == "rip") return ARTICULATION_rip;
    if (value == "plop") return ARTICULATION_plop;
    if (value == "fall") return ARTICULATION_fall;
    if (value == "longfall") return ARTICULATION_longfall;
    if (value == "bend") return ARTICULATION_bend;
    if (value == "flip") return ARTICULATION_flip;
    if (value == "smear") return ARTICULATION_smear;
    if (value == "shake") return ARTICULATION_shake;
    if (value == "dnbow") return ARTICULATION_dnbow;
    if (value == "upbow") return ARTICULATION_upbow;
    if (value == "harm") return ARTICULATION_harm;
    if (value == "snap") return ARTICULATION_snap;
    if (value == "fingernail") return ARTICULATION_fingernail;
    if (value == "ten-stacc") return ARTICULATION_ten_stacc;
    if (value == "damp") return ARTICULATION_damp;
    if (value == "dampall") return ARTICULATION_dampall;
    if (value == "open") return ARTICULATION_open;
    if (value == "stop") return ARTICULATION_stop;
    if (value == "dbltongue") return ARTICULATION_dbltongue;
    if (value == "trpltongue") return ARTICULATION_trpltongue;
    if (value == "heel") return ARTICULATION_heel;
    if (value == "toe") return ARTICULATION_toe;
    if (value == "tap") return ARTICULATION_tap;
    if (value == "lhpizz") return ARTICULATION_lhpizz;
    if (value == "dot") return ARTICULATION_dot;
    if (value == "stroke") return ARTICULATION_stroke;
    LogWarning("Unsupported value '%s' for data.ARTICULATION", value.c_str());
    return ARTICULATION_NONE;
}

std::string AttConverter::BarplaceToStr(data_BARPLACE data) const
{
    std::string value;
    switch (data) {
        case BARPLACE_mensur: value = "mensur"; break;
        case BARPLACE_staff: value = "staff"; break;
        case BARPLACE_takt: value = "takt"; break;
        default:
            LogWarning("Unknown value '%d' for data.BARPLACE", data);
            value = "";
            break;
    }
    return value;
}

data_BARPLACE AttConverter::StrToBarplace(std::string value) const
{
    if (value == "mensur") return BARPLACE_mensur;
    if (value == "staff") return BARPLACE_staff;
    if (value == "takt") return BARPLACE_takt;
    LogWarning("Unsupported value '%s' for data.BARPLACE", value.c_str());
    return BARPLACE_NONE;
}

std::string AttConverter::BarrenditionToStr(data_BARRENDITION data) const
{
    std::string value;
    switch (data) {
        case BARRENDITION_dashed: value = "dashed"; break;
        case BARRENDITION_dotted: value = "dotted"; break;
        case BARRENDITION_dbl: value = "dbl"; break;
        case BARRENDITION_dbldashed: value = "dbldashed"; break;
        case BARRENDITION_dbldotted: value = "dbldotted"; break;
        case BARRENDITION_end: value = "end"; break;
        case BARRENDITION_invis: value = "invis"; break;
        case BARRENDITION_rptstart: value = "rptstart"; break;
        case BARRENDITION_rptboth: value = "rptboth"; break;
        case BARRENDITION_rptend: value = "rptend"; break;
        case BARRENDITION_single: value = "single"; break;
        default:
            LogWarning("Unknown value '%d' for data.BARRENDITION", data);
            value = "";
            break;
    }
    return value;
}

data_BARRENDITION AttConverter::StrToBarrendition(std::string value) const
{
    if (value == "dashed") return BARRENDITION_dashed;
    if (value == "dotted") return BARRENDITION_dotted;
    if (value == "dbl") return BARRENDITION_dbl;
    if (value == "dbldashed") return BARRENDITION_dbldashed;
    if (value == "dbldotted") return BARRENDITION_dbldotted;
    if (value == "end") return BARRENDITION_end;
    if (value == "invis") return BARRENDITION_invis;
    if (value == "rptstart") return BARRENDITION_rptstart;
    if (value == "rptboth") return BARRENDITION_rptboth;
    if (value == "rptend") return BARRENDITION_rptend;
    if (value == "single") return BARRENDITION_single;
    LogWarning("Unsupported value '%s' for data.BARRENDITION", value.c_str());
    return BARRENDITION_NONE;
}

std::string AttConverter::BeamplaceToStr(data_BEAMPLACE data) const
{
    std::string value;
    switch (data) {
        case BEAMPLACE_above: value = "above"; break;
        case BEAMPLACE_below: value = "below"; break;
        case BEAMPLACE_mixed: value = "mixed"; break;
        default:
            LogWarning("Unknown value '%d' for data.BEAMPLACE", data);
            value = "";
            break;
    }
    return value;
}

data_BEAMPLACE AttConverter::StrToBeamplace(std::string value) const
{
    if (value == "above") return BEAMPLACE_above;
    if (value == "below") return BEAMPLACE_below;
    if (value == "mixed") return BEAMPLACE_mixed;
    LogWarning("Unsupported value '%s' for data.BEAMPLACE", value.c_str());
    return BEAMPLACE_NONE;
}

std::string AttConverter::BetypeToStr(data_BETYPE data) const
{
    std::string value;
    switch (data) {
        case BETYPE_byte: value = "byte"; break;
        case BETYPE_smil: value = "smil"; break;
        case BETYPE_midi: value = "midi"; break;
        case BETYPE_mmc: value = "mmc"; break;
        case BETYPE_mtc: value = "mtc"; break;
        case BETYPE_smpte_25: value = "smpte-25"; break;
        case BETYPE_smpte_24: value = "smpte-24"; break;
        case BETYPE_smpte_df30: value = "smpte-df30"; break;
        case BETYPE_smpte_ndf30: value = "smpte-ndf30"; break;
        case BETYPE_smpte_df29_97: value = "smpte-df29.97"; break;
        case BETYPE_smpte_ndf29_97: value = "smpte-ndf29.97"; break;
        case BETYPE_tcf: value = "tcf"; break;
        case BETYPE_time: value = "time"; break;
        default:
            LogWarning("Unknown value '%d' for data.BETYPE", data);
            value = "";
            break;
    }
    return value;
}

data_BETYPE AttConverter::StrToBetype(std::string value) const
{
    if (value == "byte") return BETYPE_byte;
    if (value == "smil") return BETYPE_smil;
    if (value == "midi") return BETYPE_midi;
    if (value == "mmc") return BETYPE_mmc;
    if (value == "mtc") return BETYPE_mtc;
    if (value == "smpte-25") return BETYPE_smpte_25;
    if (value == "smpte-24") return BETYPE_smpte_24;
    if (value == "smpte-df30") return BETYPE_smpte_df30;
    if (value == "smpte-ndf30") return BETYPE_smpte_ndf30;
    if (value == "smpte-df29.97") return BETYPE_smpte_df29_97;
    if (value == "smpte-ndf29.97") return BETYPE_smpte_ndf29_97;
    if (value == "tcf") return BETYPE_tcf;
    if (value == "time") return BETYPE_time;
    LogWarning("Unsupported value '%s' for data.BETYPE", value.c_str());
    return BETYPE_NONE;
}

std::string AttConverter::BooleanToStr(data_BOOLEAN data) const
{
    std::string value;
    switch (data) {
        case BOOLEAN_true: value = "true"; break;
        case BOOLEAN_false: value = "false"; break;
        default:
            LogWarning("Unknown value '%d' for data.BOOLEAN", data);
            value = "";
            break;
    }
    return value;
}

data_BOOLEAN AttConverter::StrToBoolean(std::string value) const
{
    if (value == "true") return BOOLEAN_true;
    if (value == "false") return BOOLEAN_false;
    LogWarning("Unsupported value '%s' for data.BOOLEAN", value.c_str());
    return BOOLEAN_NONE;
}

std::string AttConverter::CertaintyToStr(data_CERTAINTY data) const
{
    std::string value;
    switch (data) {
        case CERTAINTY_high: value = "high"; break;
        case CERTAINTY_medium: value = "medium"; break;
        case CERTAINTY_low: value = "low"; break;
        case CERTAINTY_unknown: value = "unknown"; break;
        default:
            LogWarning("Unknown value '%d' for data.CERTAINTY", data);
            value = "";
            break;
    }
    return value;
}

data_CERTAINTY AttConverter::StrToCertainty(std::string value) const
{
    if (value == "high") return CERTAINTY_high;
    if (value == "medium") return CERTAINTY_medium;
    if (value == "low") return CERTAINTY_low;
    if (value == "unknown") return CERTAINTY_unknown;
    LogWarning("Unsupported value '%s' for data.CERTAINTY", value.c_str());
    return CERTAINTY_NONE;
}

std::string AttConverter::ClefshapeToStr(data_CLEFSHAPE data) const
{
    std::string value;
    switch (data) {
        case CLEFSHAPE_G: value = "G"; break;
        case CLEFSHAPE_GG: value = "GG"; break;
        case CLEFSHAPE_F: value = "F"; break;
        case CLEFSHAPE_C: value = "C"; break;
        case CLEFSHAPE_perc: value = "perc"; break;
        case CLEFSHAPE_TAB: value = "TAB"; break;
        default:
            LogWarning("Unknown value '%d' for data.CLEFSHAPE", data);
            value = "";
            break;
    }
    return value;
}

data_CLEFSHAPE AttConverter::StrToClefshape(std::string value) const
{
    if (value == "G") return CLEFSHAPE_G;
    if (value == "GG") return CLEFSHAPE_GG;
    if (value == "F") return CLEFSHAPE_F;
    if (value == "C") return CLEFSHAPE_C;
    if (value == "perc") return CLEFSHAPE_perc;
    if (value == "TAB") return CLEFSHAPE_TAB;
    LogWarning("Unsupported value '%s' for data.CLEFSHAPE", value.c_str());
    return CLEFSHAPE_NONE;
}

std::string AttConverter::ColornamesToStr(data_COLORNAMES data) const
{
    std::string value;
    switch (data) {
        case COLORNAMES_aliceblue: value = "aliceblue"; break;
        case COLORNAMES_antiquewhite: value = "antiquewhite"; break;
        case COLORNAMES_aqua: value = "aqua"; break;
        case COLORNAMES_aquamarine: value = "aquamarine"; break;
        case COLORNAMES_azure: value = "azure"; break;
        case COLORNAMES_beige: value = "beige"; break;
        case COLORNAMES_bisque: value = "bisque"; break;
        case COLORNAMES_black: value = "black"; break;
        case COLORNAMES_blanchedalmond: value = "blanchedalmond"; break;
        case COLORNAMES_blue: value = "blue"; break;
        case COLORNAMES_blueviolet: value = "blueviolet"; break;
        case COLORNAMES_brown: value = "brown"; break;
        case COLORNAMES_burlywood: value = "burlywood"; break;
        case COLORNAMES_cadetblue: value = "cadetblue"; break;
        case COLORNAMES_chartreuse: value = "chartreuse"; break;
        case COLORNAMES_chocolate: value = "chocolate"; break;
        case COLORNAMES_coral: value = "coral"; break;
        case COLORNAMES_cornflowerblue: value = "cornflowerblue"; break;
        case COLORNAMES_cornsilk: value = "cornsilk"; break;
        case COLORNAMES_crimson: value = "crimson"; break;
        case COLORNAMES_cyan: value = "cyan"; break;
        case COLORNAMES_darkblue: value = "darkblue"; break;
        case COLORNAMES_darkcyan: value = "darkcyan"; break;
        case COLORNAMES_darkgoldenrod: value = "darkgoldenrod"; break;
        case COLORNAMES_darkgray: value = "darkgray"; break;
        case COLORNAMES_darkgreen: value = "darkgreen"; break;
        case COLORNAMES_darkgrey: value = "darkgrey"; break;
        case COLORNAMES_darkkhaki: value = "darkkhaki"; break;
        case COLORNAMES_darkmagenta: value = "darkmagenta"; break;
        case COLORNAMES_darkolivegreen: value = "darkolivegreen"; break;
        case COLORNAMES_darkorange: value = "darkorange"; break;
        case COLORNAMES_darkorchid: value = "darkorchid"; break;
        case COLORNAMES_darkred: value = "darkred"; break;
        case COLORNAMES_darksalmon: value = "darksalmon"; break;
        case COLORNAMES_darkseagreen: value = "darkseagreen"; break;
        case COLORNAMES_darkslateblue: value = "darkslateblue"; break;
        case COLORNAMES_darkslategray: value = "darkslategray"; break;
        case COLORNAMES_darkslategrey: value = "darkslategrey"; break;
        case COLORNAMES_darkturquoise: value = "darkturquoise"; break;
        case COLORNAMES_darkviolet: value = "darkviolet"; break;
        case COLORNAMES_deeppink: value = "deeppink"; break;
        case COLORNAMES_deepskyblue: value = "deepskyblue"; break;
        case COLORNAMES_dimgray: value = "dimgray"; break;
        case COLORNAMES_dimgrey: value = "dimgrey"; break;
        case COLORNAMES_dodgerblue: value = "dodgerblue"; break;
        case COLORNAMES_firebrick: value = "firebrick"; break;
        case COLORNAMES_floralwhite: value = "floralwhite"; break;
        case COLORNAMES_forestgreen: value = "forestgreen"; break;
        case COLORNAMES_fuchsia: value = "fuchsia"; break;
        case COLORNAMES_gainsboro: value = "gainsboro"; break;
        case COLORNAMES_ghostwhite: value = "ghostwhite"; break;
        case COLORNAMES_gold: value = "gold"; break;
        case COLORNAMES_goldenrod: value = "goldenrod"; break;
        case COLORNAMES_gray: value = "gray"; break;
        case COLORNAMES_green: value = "green"; break;
        case COLORNAMES_greenyellow: value = "greenyellow"; break;
        case COLORNAMES_grey: value = "grey"; break;
        case COLORNAMES_honeydew: value = "honeydew"; break;
        case COLORNAMES_hotpink: value = "hotpink"; break;
        case COLORNAMES_indianred: value = "indianred"; break;
        case COLORNAMES_indigo: value = "indigo"; break;
        case COLORNAMES_ivory: value = "ivory"; break;
        case COLORNAMES_khaki: value = "khaki"; break;
        case COLORNAMES_lavender: value = "lavender"; break;
        case COLORNAMES_lavenderblush: value = "lavenderblush"; break;
        case COLORNAMES_lawngreen: value = "lawngreen"; break;
        case COLORNAMES_lemonchiffon: value = "lemonchiffon"; break;
        case COLORNAMES_lightblue: value = "lightblue"; break;
        case COLORNAMES_lightcoral: value = "lightcoral"; break;
        case COLORNAMES_lightcyan: value = "lightcyan"; break;
        case COLORNAMES_lightgoldenrodyellow: value = "lightgoldenrodyellow"; break;
        case COLORNAMES_lightgray: value = "lightgray"; break;
        case COLORNAMES_lightgreen: value = "lightgreen"; break;
        case COLORNAMES_lightgrey: value = "lightgrey"; break;
        case COLORNAMES_lightpink: value = "lightpink"; break;
        case COLORNAMES_lightsalmon: value = "lightsalmon"; break;
        case COLORNAMES_lightseagreen: value = "lightseagreen"; break;
        case COLORNAMES_lightskyblue: value = "lightskyblue"; break;
        case COLORNAMES_lightslategray: value = "lightslategray"; break;
        case COLORNAMES_lightslategrey: value = "lightslategrey"; break;
        case COLORNAMES_lightsteelblue: value = "lightsteelblue"; break;
        case COLORNAMES_lightyellow: value = "lightyellow"; break;
        case COLORNAMES_lime: value = "lime"; break;
        case COLORNAMES_limegreen: value = "limegreen"; break;
        case COLORNAMES_linen: value = "linen"; break;
        case COLORNAMES_magenta: value = "magenta"; break;
        case COLORNAMES_maroon: value = "maroon"; break;
        case COLORNAMES_mediumaquamarine: value = "mediumaquamarine"; break;
        case COLORNAMES_mediumblue: value = "mediumblue"; break;
        case COLORNAMES_mediumorchid: value = "mediumorchid"; break;
        case COLORNAMES_mediumpurple: value = "mediumpurple"; break;
        case COLORNAMES_mediumseagreen: value = "mediumseagreen"; break;
        case COLORNAMES_mediumslateblue: value = "mediumslateblue"; break;
        case COLORNAMES_mediumspringgreen: value = "mediumspringgreen"; break;
        case COLORNAMES_mediumturquoise: value = "mediumturquoise"; break;
        case COLORNAMES_mediumvioletred: value = "mediumvioletred"; break;
        case COLORNAMES_midnightblue: value = "midnightblue"; break;
        case COLORNAMES_mintcream: value = "mintcream"; break;
        case COLORNAMES_mistyrose: value = "mistyrose"; break;
        case COLORNAMES_moccasin: value = "moccasin"; break;
        case COLORNAMES_navajowhite: value = "navajowhite"; break;
        case COLORNAMES_navy: value = "navy"; break;
        case COLORNAMES_oldlace: value = "oldlace"; break;
        case COLORNAMES_olive: value = "olive"; break;
        case COLORNAMES_olivedrab: value = "olivedrab"; break;
        case COLORNAMES_orange: value = "orange"; break;
        case COLORNAMES_orangered: value = "orangered"; break;
        case COLORNAMES_orchid: value = "orchid"; break;
        case COLORNAMES_palegoldenrod: value = "palegoldenrod"; break;
        case COLORNAMES_palegreen: value = "palegreen"; break;
        case COLORNAMES_paleturquoise: value = "paleturquoise"; break;
        case COLORNAMES_palevioletred: value = "palevioletred"; break;
        case COLORNAMES_papayawhip: value = "papayawhip"; break;
        case COLORNAMES_peachpuff: value = "peachpuff"; break;
        case COLORNAMES_peru: value = "peru"; break;
        case COLORNAMES_pink: value = "pink"; break;
        case COLORNAMES_plum: value = "plum"; break;
        case COLORNAMES_powderblue: value = "powderblue"; break;
        case COLORNAMES_purple: value = "purple"; break;
        case COLORNAMES_red: value = "red"; break;
        case COLORNAMES_rosybrown: value = "rosybrown"; break;
        case COLORNAMES_royalblue: value = "royalblue"; break;
        case COLORNAMES_saddlebrown: value = "saddlebrown"; break;
        case COLORNAMES_salmon: value = "salmon"; break;
        case COLORNAMES_sandybrown: value = "sandybrown"; break;
        case COLORNAMES_seagreen: value = "seagreen"; break;
        case COLORNAMES_seashell: value = "seashell"; break;
        case COLORNAMES_sienna: value = "sienna"; break;
        case COLORNAMES_silver: value = "silver"; break;
        case COLORNAMES_skyblue: value = "skyblue"; break;
        case COLORNAMES_slateblue: value = "slateblue"; break;
        case COLORNAMES_slategray: value = "slategray"; break;
        case COLORNAMES_slategrey: value = "slategrey"; break;
        case COLORNAMES_snow: value = "snow"; break;
        case COLORNAMES_springgreen: value = "springgreen"; break;
        case COLORNAMES_steelblue: value = "steelblue"; break;
        case COLORNAMES_tan: value = "tan"; break;
        case COLORNAMES_teal: value = "teal"; break;
        case COLORNAMES_thistle: value = "thistle"; break;
        case COLORNAMES_tomato: value = "tomato"; break;
        case COLORNAMES_turquoise: value = "turquoise"; break;
        case COLORNAMES_violet: value = "violet"; break;
        case COLORNAMES_wheat: value = "wheat"; break;
        case COLORNAMES_white: value = "white"; break;
        case COLORNAMES_whitesmoke: value = "whitesmoke"; break;
        case COLORNAMES_yellow: value = "yellow"; break;
        case COLORNAMES_yellowgreen: value = "yellowgreen"; break;
        default:
            LogWarning("Unknown value '%d' for data.COLORNAMES", data);
            value = "";
            break;
    }
    return value;
}

data_COLORNAMES AttConverter::StrToColornames(std::string value) const
{
    if (value == "aliceblue") return COLORNAMES_aliceblue;
    if (value == "antiquewhite") return COLORNAMES_antiquewhite;
    if (value == "aqua") return COLORNAMES_aqua;
    if (value == "aquamarine") return COLORNAMES_aquamarine;
    if (value == "azure") return COLORNAMES_azure;
    if (value == "beige") return COLORNAMES_beige;
    if (value == "bisque") return COLORNAMES_bisque;
    if (value == "black") return COLORNAMES_black;
    if (value == "blanchedalmond") return COLORNAMES_blanchedalmond;
    if (value == "blue") return COLORNAMES_blue;
    if (value == "blueviolet") return COLORNAMES_blueviolet;
    if (value == "brown") return COLORNAMES_brown;
    if (value == "burlywood") return COLORNAMES_burlywood;
    if (value == "cadetblue") return COLORNAMES_cadetblue;
    if (value == "chartreuse") return COLORNAMES_chartreuse;
    if (value == "chocolate") return COLORNAMES_chocolate;
    if (value == "coral") return COLORNAMES_coral;
    if (value == "cornflowerblue") return COLORNAMES_cornflowerblue;
    if (value == "cornsilk") return COLORNAMES_cornsilk;
    if (value == "crimson") return COLORNAMES_crimson;
    if (value == "cyan") return COLORNAMES_cyan;
    if (value == "darkblue") return COLORNAMES_darkblue;
    if (value == "darkcyan") return COLORNAMES_darkcyan;
    if (value == "darkgoldenrod") return COLORNAMES_darkgoldenrod;
    if (value == "darkgray") return COLORNAMES_darkgray;
    if (value == "darkgreen") return COLORNAMES_darkgreen;
    if (value == "darkgrey") return COLORNAMES_darkgrey;
    if (value == "darkkhaki") return COLORNAMES_darkkhaki;
    if (value == "darkmagenta") return COLORNAMES_darkmagenta;
    if (value == "darkolivegreen") return COLORNAMES_darkolivegreen;
    if (value == "darkorange") return COLORNAMES_darkorange;
    if (value == "darkorchid") return COLORNAMES_darkorchid;
    if (value == "darkred") return COLORNAMES_darkred;
    if (value == "darksalmon") return COLORNAMES_darksalmon;
    if (value == "darkseagreen") return COLORNAMES_darkseagreen;
    if (value == "darkslateblue") return COLORNAMES_darkslateblue;
    if (value == "darkslategray") return COLORNAMES_darkslategray;
    if (value == "darkslategrey") return COLORNAMES_darkslategrey;
    if (value == "darkturquoise") return COLORNAMES_darkturquoise;
    if (value == "darkviolet") return COLORNAMES_darkviolet;
    if (value == "deeppink") return COLORNAMES_deeppink;
    if (value == "deepskyblue") return COLORNAMES_deepskyblue;
    if (value == "dimgray") return COLORNAMES_dimgray;
    if (value == "dimgrey") return COLORNAMES_dimgrey;
    if (value == "dodgerblue") return COLORNAMES_dodgerblue;
    if (value == "firebrick") return COLORNAMES_firebrick;
    if (value == "floralwhite") return COLORNAMES_floralwhite;
    if (value == "forestgreen") return COLORNAMES_forestgreen;
    if (value == "fuchsia") return COLORNAMES_fuchsia;
    if (value == "gainsboro") return COLORNAMES_gainsboro;
    if (value == "ghostwhite") return COLORNAMES_ghostwhite;
    if (value == "gold") return COLORNAMES_gold;
    if (value == "goldenrod") return COLORNAMES_goldenrod;
    if (value == "gray") return COLORNAMES_gray;
    if (value == "green") return COLORNAMES_green;
    if (value == "greenyellow") return COLORNAMES_greenyellow;
    if (value == "grey") return COLORNAMES_grey;
    if (value == "honeydew") return COLORNAMES_honeydew;
    if (value == "hotpink") return COLORNAMES_hotpink;
    if (value == "indianred") return COLORNAMES_indianred;
    if (value == "indigo") return COLORNAMES_indigo;
    if (value == "ivory") return COLORNAMES_ivory;
    if (value == "khaki") return COLORNAMES_khaki;
    if (value == "lavender") return COLORNAMES_lavender;
    if (value == "lavenderblush") return COLORNAMES_lavenderblush;
    if (value == "lawngreen") return COLORNAMES_lawngreen;
    if (value == "lemonchiffon") return COLORNAMES_lemonchiffon;
    if (value == "lightblue") return COLORNAMES_lightblue;
    if (value == "lightcoral") return COLORNAMES_lightcoral;
    if (value == "lightcyan") return COLORNAMES_lightcyan;
    if (value == "lightgoldenrodyellow") return COLORNAMES_lightgoldenrodyellow;
    if (value == "lightgray") return COLORNAMES_lightgray;
    if (value == "lightgreen") return COLORNAMES_lightgreen;
    if (value == "lightgrey") return COLORNAMES_lightgrey;
    if (value == "lightpink") return COLORNAMES_lightpink;
    if (value == "lightsalmon") return COLORNAMES_lightsalmon;
    if (value == "lightseagreen") return COLORNAMES_lightseagreen;
    if (value == "lightskyblue") return COLORNAMES_lightskyblue;
    if (value == "lightslategray") return COLORNAMES_lightslategray;
    if (value == "lightslategrey") return COLORNAMES_lightslategrey;
    if (value == "lightsteelblue") return COLORNAMES_lightsteelblue;
    if (value == "lightyellow") return COLORNAMES_lightyellow;
    if (value == "lime") return COLORNAMES_lime;
    if (value == "limegreen") return COLORNAMES_limegreen;
    if (value == "linen") return COLORNAMES_linen;
    if (value == "magenta") return COLORNAMES_magenta;
    if (value == "maroon") return COLORNAMES_maroon;
    if (value == "mediumaquamarine") return COLORNAMES_mediumaquamarine;
    if (value == "mediumblue") return COLORNAMES_mediumblue;
    if (value == "mediumorchid") return COLORNAMES_mediumorchid;
    if (value == "mediumpurple") return COLORNAMES_mediumpurple;
    if (value == "mediumseagreen") return COLORNAMES_mediumseagreen;
    if (value == "mediumslateblue") return COLORNAMES_mediumslateblue;
    if (value == "mediumspringgreen") return COLORNAMES_mediumspringgreen;
    if (value == "mediumturquoise") return COLORNAMES_mediumturquoise;
    if (value == "mediumvioletred") return COLORNAMES_mediumvioletred;
    if (value == "midnightblue") return COLORNAMES_midnightblue;
    if (value == "mintcream") return COLORNAMES_mintcream;
    if (value == "mistyrose") return COLORNAMES_mistyrose;
    if (value == "moccasin") return COLORNAMES_moccasin;
    if (value == "navajowhite") return COLORNAMES_navajowhite;
    if (value == "navy") return COLORNAMES_navy;
    if (value == "oldlace") return COLORNAMES_oldlace;
    if (value == "olive") return COLORNAMES_olive;
    if (value == "olivedrab") return COLORNAMES_olivedrab;
    if (value == "orange") return COLORNAMES_orange;
    if (value == "orangered") return COLORNAMES_orangered;
    if (value == "orchid") return COLORNAMES_orchid;
    if (value == "palegoldenrod") return COLORNAMES_palegoldenrod;
    if (value == "palegreen") return COLORNAMES_palegreen;
    if (value == "paleturquoise") return COLORNAMES_paleturquoise;
    if (value == "palevioletred") return COLORNAMES_palevioletred;
    if (value == "papayawhip") return COLORNAMES_papayawhip;
    if (value == "peachpuff") return COLORNAMES_peachpuff;
    if (value == "peru") return COLORNAMES_peru;
    if (value == "pink") return COLORNAMES_pink;
    if (value == "plum") return COLORNAMES_plum;
    if (value == "powderblue") return COLORNAMES_powderblue;
    if (value == "purple") return COLORNAMES_purple;
    if (value == "red") return COLORNAMES_red;
    if (value == "rosybrown") return COLORNAMES_rosybrown;
    if (value == "royalblue") return COLORNAMES_royalblue;
    if (value == "saddlebrown") return COLORNAMES_saddlebrown;
    if (value == "salmon") return COLORNAMES_salmon;
    if (value == "sandybrown") return COLORNAMES_sandybrown;
    if (value == "seagreen") return COLORNAMES_seagreen;
    if (value == "seashell") return COLORNAMES_seashell;
    if (value == "sienna") return COLORNAMES_sienna;
    if (value == "silver") return COLORNAMES_silver;
    if (value == "skyblue") return COLORNAMES_skyblue;
    if (value == "slateblue") return COLORNAMES_slateblue;
    if (value == "slategray") return COLORNAMES_slategray;
    if (value == "slategrey") return COLORNAMES_slategrey;
    if (value == "snow") return COLORNAMES_snow;
    if (value == "springgreen") return COLORNAMES_springgreen;
    if (value == "steelblue") return COLORNAMES_steelblue;
    if (value == "tan") return COLORNAMES_tan;
    if (value == "teal") return COLORNAMES_teal;
    if (value == "thistle") return COLORNAMES_thistle;
    if (value == "tomato") return COLORNAMES_tomato;
    if (value == "turquoise") return COLORNAMES_turquoise;
    if (value == "violet") return COLORNAMES_violet;
    if (value == "wheat") return COLORNAMES_wheat;
    if (value == "white") return COLORNAMES_white;
    if (value == "whitesmoke") return COLORNAMES_whitesmoke;
    if (value == "yellow") return COLORNAMES_yellow;
    if (value == "yellowgreen") return COLORNAMES_yellowgreen;
    LogWarning("Unsupported value '%s' for data.COLORNAMES", value.c_str());
    return COLORNAMES_NONE;
}

std::string AttConverter::EnclosureToStr(data_ENCLOSURE data) const
{
    std::string value;
    switch (data) {
        case ENCLOSURE_paren: value = "paren"; break;
        case ENCLOSURE_brack: value = "brack"; break;
        default:
            LogWarning("Unknown value '%d' for data.ENCLOSURE", data);
            value = "";
            break;
    }
    return value;
}

data_ENCLOSURE AttConverter::StrToEnclosure(std::string value) const
{
    if (value == "paren") return ENCLOSURE_paren;
    if (value == "brack") return ENCLOSURE_brack;
    LogWarning("Unsupported value '%s' for data.ENCLOSURE", value.c_str());
    return ENCLOSURE_NONE;
}

std::string AttConverter::FillToStr(data_FILL data) const
{
    std::string value;
    switch (data) {
        case FILL_void: value = "void"; break;
        case FILL_solid: value = "solid"; break;
        case FILL_top: value = "top"; break;
        case FILL_bottom: value = "bottom"; break;
        case FILL_left: value = "left"; break;
        case FILL_right: value = "right"; break;
        default:
            LogWarning("Unknown value '%d' for data.FILL", data);
            value = "";
            break;
    }
    return value;
}

data_FILL AttConverter::StrToFill(std::string value) const
{
    if (value == "void") return FILL_void;
    if (value == "solid") return FILL_solid;
    if (value == "top") return FILL_top;
    if (value == "bottom") return FILL_bottom;
    if (value == "left") return FILL_left;
    if (value == "right") return FILL_right;
    LogWarning("Unsupported value '%s' for data.FILL", value.c_str());
    return FILL_NONE;
}

std::string AttConverter::FontsizetermToStr(data_FONTSIZETERM data) const
{
    std::string value;
    switch (data) {
        case FONTSIZETERM_xx_small: value = "xx-small"; break;
        case FONTSIZETERM_x_small: value = "x-small"; break;
        case FONTSIZETERM_small: value = "small"; break;
        case FONTSIZETERM_normal: value = "normal"; break;
        case FONTSIZETERM_large: value = "large"; break;
        case FONTSIZETERM_x_large: value = "x-large"; break;
        case FONTSIZETERM_xx_large: value = "xx-large"; break;
        case FONTSIZETERM_smaller: value = "smaller"; break;
        case FONTSIZETERM_larger: value = "larger"; break;
        case FONTSIZETERM_cue: value = "cue"; break;
        default:
            LogWarning("Unknown value '%d' for data.FONTSIZETERM", data);
            value = "";
            break;
    }
    return value;
}

data_FONTSIZETERM AttConverter::StrToFontsizeterm(std::string value) const
{
    if (value == "xx-small") return FONTSIZETERM_xx_small;
    if (value == "x-small") return FONTSIZETERM_x_small;
    if (value == "small") return FONTSIZETERM_small;
    if (value == "normal") return FONTSIZETERM_normal;
    if (value == "large") return FONTSIZETERM_large;
    if (value == "x-large") return FONTSIZETERM_x_large;
    if (value == "xx-large") return FONTSIZETERM_xx_large;
    if (value == "smaller") return FONTSIZETERM_smaller;
    if (value == "larger") return FONTSIZETERM_larger;
    if (value == "cue") return FONTSIZETERM_cue;
    LogWarning("Unsupported value '%s' for data.FONTSIZETERM", value.c_str());
    return FONTSIZETERM_NONE;
}

std::string AttConverter::FontstyleToStr(data_FONTSTYLE data) const
{
    std::string value;
    switch (data) {
        case FONTSTYLE_italic: value = "italic"; break;
        case FONTSTYLE_normal: value = "normal"; break;
        case FONTSTYLE_oblique: value = "oblique"; break;
        default:
            LogWarning("Unknown value '%d' for data.FONTSTYLE", data);
            value = "";
            break;
    }
    return value;
}

data_FONTSTYLE AttConverter::StrToFontstyle(std::string value) const
{
    if (value == "italic") return FONTSTYLE_italic;
    if (value == "normal") return FONTSTYLE_normal;
    if (value == "oblique") return FONTSTYLE_oblique;
    LogWarning("Unsupported value '%s' for data.FONTSTYLE", value.c_str());
    return FONTSTYLE_NONE;
}

std::string AttConverter::FontweightToStr(data_FONTWEIGHT data) const
{
    std::string value;
    switch (data) {
        case FONTWEIGHT_bold: value = "bold"; break;
        case FONTWEIGHT_normal: value = "normal"; break;
        default:
            LogWarning("Unknown value '%d' for data.FONTWEIGHT", data);
            value = "";
            break;
    }
    return value;
}

data_FONTWEIGHT AttConverter::StrToFontweight(std::string value) const
{
    if (value == "bold") return FONTWEIGHT_bold;
    if (value == "normal") return FONTWEIGHT_normal;
    LogWarning("Unsupported value '%s' for data.FONTWEIGHT", value.c_str());
    return FONTWEIGHT_NONE;
}

std::string AttConverter::GlissandoToStr(data_GLISSANDO data) const
{
    std::string value;
    switch (data) {
        case GLISSANDO_i: value = "i"; break;
        case GLISSANDO_m: value = "m"; break;
        case GLISSANDO_t: value = "t"; break;
        default:
            LogWarning("Unknown value '%d' for data.GLISSANDO", data);
            value = "";
            break;
    }
    return value;
}

data_GLISSANDO AttConverter::StrToGlissando(std::string value) const
{
    if (value == "i") return GLISSANDO_i;
    if (value == "m") return GLISSANDO_m;
    if (value == "t") return GLISSANDO_t;
    LogWarning("Unsupported value '%s' for data.GLISSANDO", value.c_str());
    return GLISSANDO_NONE;
}

std::string AttConverter::GraceToStr(data_GRACE data) const
{
    std::string value;
    switch (data) {
        case GRACE_acc: value = "acc"; break;
        case GRACE_unacc: value = "unacc"; break;
        case GRACE_unknown: value = "unknown"; break;
        default:
            LogWarning("Unknown value '%d' for data.GRACE", data);
            value = "";
            break;
    }
    return value;
}

data_GRACE AttConverter::StrToGrace(std::string value) const
{
    if (value == "acc") return GRACE_acc;
    if (value == "unacc") return GRACE_unacc;
    if (value == "unknown") return GRACE_unknown;
    LogWarning("Unsupported value '%s' for data.GRACE", value.c_str());
    return GRACE_NONE;
}

std::string AttConverter::HeadshapeListToStr(data_HEADSHAPE_list data) const
{
    std::string value;
    switch (data) {
        case HEADSHAPE_list_quarter: value = "quarter"; break;
        case HEADSHAPE_list_half: value = "half"; break;
        case HEADSHAPE_list_whole: value = "whole"; break;
        case HEADSHAPE_list_backslash: value = "backslash"; break;
        case HEADSHAPE_list_circle: value = "circle"; break;
        case HEADSHAPE_list_plus: value = "+"; break;
        case HEADSHAPE_list_diamond: value = "diamond"; break;
        case HEADSHAPE_list_isotriangle: value = "isotriangle"; break;
        case HEADSHAPE_list_oval: value = "oval"; break;
        case HEADSHAPE_list_piewedge: value = "piewedge"; break;
        case HEADSHAPE_list_rectangle: value = "rectangle"; break;
        case HEADSHAPE_list_rtriangle: value = "rtriangle"; break;
        case HEADSHAPE_list_semicircle: value = "semicircle"; break;
        case HEADSHAPE_list_slash: value = "slash"; break;
        case HEADSHAPE_list_square: value = "square"; break;
        case HEADSHAPE_list_x: value = "x"; break;
        default:
            LogWarning("Unknown value '%d' for data.HEADSHAPE.list", data);
            value = "";
            break;
    }
    return value;
}

data_HEADSHAPE_list AttConverter::StrToHeadshapeList(std::string value) const
{
    if (value == "quarter") return HEADSHAPE_list_quarter;
    if (value == "half") return HEADSHAPE_list_half;
    if (value == "whole") return HEADSHAPE_list_whole;
    if (value == "backslash") return HEADSHAPE_list_backslash;
    if (value == "circle") return HEADSHAPE_list_circle;
    if (value == "+") return HEADSHAPE_list_plus;
    if (value == "diamond") return HEADSHAPE_list_diamond;
    if (value == "isotriangle") return HEADSHAPE_list_isotriangle;
    if (value == "oval") return HEADSHAPE_list_oval;
    if (value == "piewedge") return HEADSHAPE_list_piewedge;
    if (value == "rectangle") return HEADSHAPE_list_rectangle;
    if (value == "rtriangle") return HEADSHAPE_list_rtriangle;
    if (value == "semicircle") return HEADSHAPE_list_semicircle;
    if (value == "slash") return HEADSHAPE_list_slash;
    if (value == "square") return HEADSHAPE_list_square;
    if (value == "x") return HEADSHAPE_list_x;
    LogWarning("Unsupported value '%s' for data.HEADSHAPE.list", value.c_str());
    return HEADSHAPE_list_NONE;
}

std::string AttConverter::HorizontalalignmentToStr(data_HORIZONTALALIGNMENT data) const
{
    std::string value;
    switch (data) {
        case HORIZONTALALIGNMENT_left: value = "left"; break;
        case HORIZONTALALIGNMENT_right: value = "right"; break;
        case HORIZONTALALIGNMENT_center: value = "center"; break;
        case HORIZONTALALIGNMENT_justify: value = "justify"; break;
        default:
            LogWarning("Unknown value '%d' for data.HORIZONTALALIGNMENT", data);
            value = "";
            break;
    }
    return value;
}

data_HORIZONTALALIGNMENT AttConverter::StrToHorizontalalignment(std::string value) const
{
    if (value == "left") return HORIZONTALALIGNMENT_left;
    if (value == "right") return HORIZONTALALIGNMENT_right;
    if (value == "center") return HORIZONTALALIGNMENT_center;
    if (value == "justify") return HORIZONTALALIGNMENT_justify;
    LogWarning("Unsupported value '%s' for data.HORIZONTALALIGNMENT", value.c_str());
    return HORIZONTALALIGNMENT_NONE;
}

std::string AttConverter::LigatureformToStr(data_LIGATUREFORM data) const
{
    std::string value;
    switch (data) {
        case LIGATUREFORM_recta: value = "recta"; break;
        case LIGATUREFORM_obliqua: value = "obliqua"; break;
        default:
            LogWarning("Unknown value '%d' for data.LIGATUREFORM", data);
            value = "";
            break;
    }
    return value;
}

data_LIGATUREFORM AttConverter::StrToLigatureform(std::string value) const
{
    if (value == "recta") return LIGATUREFORM_recta;
    if (value == "obliqua") return LIGATUREFORM_obliqua;
    LogWarning("Unsupported value '%s' for data.LIGATUREFORM", value.c_str());
    return LIGATUREFORM_NONE;
}

std::string AttConverter::LineformToStr(data_LINEFORM data) const
{
    std::string value;
    switch (data) {
        case LINEFORM_dashed: value = "dashed"; break;
        case LINEFORM_dotted: value = "dotted"; break;
        case LINEFORM_solid: value = "solid"; break;
        case LINEFORM_wavy: value = "wavy"; break;
        default:
            LogWarning("Unknown value '%d' for data.LINEFORM", data);
            value = "";
            break;
    }
    return value;
}

data_LINEFORM AttConverter::StrToLineform(std::string value) const
{
    if (value == "dashed") return LINEFORM_dashed;
    if (value == "dotted") return LINEFORM_dotted;
    if (value == "solid") return LINEFORM_solid;
    if (value == "wavy") return LINEFORM_wavy;
    LogWarning("Unsupported value '%s' for data.LINEFORM", value.c_str());
    return LINEFORM_NONE;
}

std::string AttConverter::LinestartendsymbolToStr(data_LINESTARTENDSYMBOL data) const
{
    std::string value;
    switch (data) {
        case LINESTARTENDSYMBOL_angledown: value = "angledown"; break;
        case LINESTARTENDSYMBOL_angleup: value = "angleup"; break;
        case LINESTARTENDSYMBOL_angleright: value = "angleright"; break;
        case LINESTARTENDSYMBOL_angleleft: value = "angleleft"; break;
        case LINESTARTENDSYMBOL_arrow: value = "arrow"; break;
        case LINESTARTENDSYMBOL_arrowopen: value = "arrowopen"; break;
        case LINESTARTENDSYMBOL_arrowwhite: value = "arrowwhite"; break;
        case LINESTARTENDSYMBOL_harpoonleft: value = "harpoonleft"; break;
        case LINESTARTENDSYMBOL_harpoonright: value = "harpoonright"; break;
        case LINESTARTENDSYMBOL_none: value = "none"; break;
        default:
            LogWarning("Unknown value '%d' for data.LINESTARTENDSYMBOL", data);
            value = "";
            break;
    }
    return value;
}

data_LINESTARTENDSYMBOL AttConverter::StrToLinestartendsymbol(std::string value) const
{
    if (value == "angledown") return LINESTARTENDSYMBOL_angledown;
    if (value == "angleup") return LINESTARTENDSYMBOL_angleup;
    if (value == "angleright") return LINESTARTENDSYMBOL_angleright;
    if (value == "angleleft") return LINESTARTENDSYMBOL_angleleft;
    if (value == "arrow") return LINESTARTENDSYMBOL_arrow;
    if (value == "arrowopen") return LINESTARTENDSYMBOL_arrowopen;
    if (value == "arrowwhite") return LINESTARTENDSYMBOL_arrowwhite;
    if (value == "harpoonleft") return LINESTARTENDSYMBOL_harpoonleft;
    if (value == "harpoonright") return LINESTARTENDSYMBOL_harpoonright;
    if (value == "none") return LINESTARTENDSYMBOL_none;
    LogWarning("Unsupported value '%s' for data.LINESTARTENDSYMBOL", value.c_str());
    return LINESTARTENDSYMBOL_NONE;
}

std::string AttConverter::LinewidthtermToStr(data_LINEWIDTHTERM data) const
{
    std::string value;
    switch (data) {
        case LINEWIDTHTERM_narrow: value = "narrow"; break;
        case LINEWIDTHTERM_medium: value = "medium"; break;
        case LINEWIDTHTERM_wide: value = "wide"; break;
        default:
            LogWarning("Unknown value '%d' for data.LINEWIDTHTERM", data);
            value = "";
            break;
    }
    return value;
}

data_LINEWIDTHTERM AttConverter::StrToLinewidthterm(std::string value) const
{
    if (value == "narrow") return LINEWIDTHTERM_narrow;
    if (value == "medium") return LINEWIDTHTERM_medium;
    if (value == "wide") return LINEWIDTHTERM_wide;
    LogWarning("Unsupported value '%s' for data.LINEWIDTHTERM", value.c_str());
    return LINEWIDTHTERM_NONE;
}

std::string AttConverter::MensurationsignToStr(data_MENSURATIONSIGN data) const
{
    std::string value;
    switch (data) {
        case MENSURATIONSIGN_C: value = "C"; break;
        case MENSURATIONSIGN_O: value = "O"; break;
        default:
            LogWarning("Unknown value '%d' for data.MENSURATIONSIGN", data);
            value = "";
            break;
    }
    return value;
}

data_MENSURATIONSIGN AttConverter::StrToMensurationsign(std::string value) const
{
    if (value == "C") return MENSURATIONSIGN_C;
    if (value == "O") return MENSURATIONSIGN_O;
    LogWarning("Unsupported value '%s' for data.MENSURATIONSIGN", value.c_str());
    return MENSURATIONSIGN_NONE;
}

std::string AttConverter::MetersignToStr(data_METERSIGN data) const
{
    std::string value;
    switch (data) {
        case METERSIGN_common: value = "common"; break;
        case METERSIGN_cut: value = "cut"; break;
        default:
            LogWarning("Unknown value '%d' for data.METERSIGN", data);
            value = "";
            break;
    }
    return value;
}

data_METERSIGN AttConverter::StrToMetersign(std::string value) const
{
    if (value == "common") return METERSIGN_common;
    if (value == "cut") return METERSIGN_cut;
    LogWarning("Unsupported value '%s' for data.METERSIGN", value.c_str());
    return METERSIGN_NONE;
}

std::string AttConverter::MidinamesToStr(data_MIDINAMES data) const
{
    std::string value;
    switch (data) {
        case MIDINAMES_Acoustic_Grand_Piano: value = "Acoustic_Grand_Piano"; break;
        case MIDINAMES_Bright_Acoustic_Piano: value = "Bright_Acoustic_Piano"; break;
        case MIDINAMES_Electric_Grand_Piano: value = "Electric_Grand_Piano"; break;
        case MIDINAMES_Honky_tonk_Piano: value = "Honky-tonk_Piano"; break;
        case MIDINAMES_Electric_Piano_1: value = "Electric_Piano_1"; break;
        case MIDINAMES_Electric_Piano_2: value = "Electric_Piano_2"; break;
        case MIDINAMES_Harpsichord: value = "Harpsichord"; break;
        case MIDINAMES_Clavi: value = "Clavi"; break;
        case MIDINAMES_Celesta: value = "Celesta"; break;
        case MIDINAMES_Glockenspiel: value = "Glockenspiel"; break;
        case MIDINAMES_Music_Box: value = "Music_Box"; break;
        case MIDINAMES_Vibraphone: value = "Vibraphone"; break;
        case MIDINAMES_Marimba: value = "Marimba"; break;
        case MIDINAMES_Xylophone: value = "Xylophone"; break;
        case MIDINAMES_Tubular_Bells: value = "Tubular_Bells"; break;
        case MIDINAMES_Dulcimer: value = "Dulcimer"; break;
        case MIDINAMES_Drawbar_Organ: value = "Drawbar_Organ"; break;
        case MIDINAMES_Percussive_Organ: value = "Percussive_Organ"; break;
        case MIDINAMES_Rock_Organ: value = "Rock_Organ"; break;
        case MIDINAMES_Church_Organ: value = "Church_Organ"; break;
        case MIDINAMES_Reed_Organ: value = "Reed_Organ"; break;
        case MIDINAMES_Accordion: value = "Accordion"; break;
        case MIDINAMES_Harmonica: value = "Harmonica"; break;
        case MIDINAMES_Tango_Accordion: value = "Tango_Accordion"; break;
        case MIDINAMES_Acoustic_Guitar_nylon: value = "Acoustic_Guitar_nylon"; break;
        case MIDINAMES_Acoustic_Guitar_steel: value = "Acoustic_Guitar_steel"; break;
        case MIDINAMES_Electric_Guitar_jazz: value = "Electric_Guitar_jazz"; break;
        case MIDINAMES_Electric_Guitar_clean: value = "Electric_Guitar_clean"; break;
        case MIDINAMES_Electric_Guitar_muted: value = "Electric_Guitar_muted"; break;
        case MIDINAMES_Overdriven_Guitar: value = "Overdriven_Guitar"; break;
        case MIDINAMES_Distortion_Guitar: value = "Distortion_Guitar"; break;
        case MIDINAMES_Guitar_harmonics: value = "Guitar_harmonics"; break;
        case MIDINAMES_Acoustic_Bass: value = "Acoustic_Bass"; break;
        case MIDINAMES_Electric_Bass_finger: value = "Electric_Bass_finger"; break;
        case MIDINAMES_Electric_Bass_pick: value = "Electric_Bass_pick"; break;
        case MIDINAMES_Fretless_Bass: value = "Fretless_Bass"; break;
        case MIDINAMES_Slap_Bass_1: value = "Slap_Bass_1"; break;
        case MIDINAMES_Slap_Bass_2: value = "Slap_Bass_2"; break;
        case MIDINAMES_Synth_Bass_1: value = "Synth_Bass_1"; break;
        case MIDINAMES_Synth_Bass_2: value = "Synth_Bass_2"; break;
        case MIDINAMES_Violin: value = "Violin"; break;
        case MIDINAMES_Viola: value = "Viola"; break;
        case MIDINAMES_Cello: value = "Cello"; break;
        case MIDINAMES_Contrabass: value = "Contrabass"; break;
        case MIDINAMES_Tremolo_Strings: value = "Tremolo_Strings"; break;
        case MIDINAMES_Pizzicato_Strings: value = "Pizzicato_Strings"; break;
        case MIDINAMES_Orchestral_Harp: value = "Orchestral_Harp"; break;
        case MIDINAMES_Timpani: value = "Timpani"; break;
        case MIDINAMES_String_Ensemble_1: value = "String_Ensemble_1"; break;
        case MIDINAMES_String_Ensemble_2: value = "String_Ensemble_2"; break;
        case MIDINAMES_SynthStrings_1: value = "SynthStrings_1"; break;
        case MIDINAMES_SynthStrings_2: value = "SynthStrings_2"; break;
        case MIDINAMES_Choir_Aahs: value = "Choir_Aahs"; break;
        case MIDINAMES_Voice_Oohs: value = "Voice_Oohs"; break;
        case MIDINAMES_Synth_Voice: value = "Synth_Voice"; break;
        case MIDINAMES_Orchestra_Hit: value = "Orchestra_Hit"; break;
        case MIDINAMES_Trumpet: value = "Trumpet"; break;
        case MIDINAMES_Trombone: value = "Trombone"; break;
        case MIDINAMES_Tuba: value = "Tuba"; break;
        case MIDINAMES_Muted_Trumpet: value = "Muted_Trumpet"; break;
        case MIDINAMES_French_Horn: value = "French_Horn"; break;
        case MIDINAMES_Brass_Section: value = "Brass_Section"; break;
        case MIDINAMES_SynthBrass_1: value = "SynthBrass_1"; break;
        case MIDINAMES_SynthBrass_2: value = "SynthBrass_2"; break;
        case MIDINAMES_Soprano_Sax: value = "Soprano_Sax"; break;
        case MIDINAMES_Alto_Sax: value = "Alto_Sax"; break;
        case MIDINAMES_Tenor_Sax: value = "Tenor_Sax"; break;
        case MIDINAMES_Baritone_Sax: value = "Baritone_Sax"; break;
        case MIDINAMES_Oboe: value = "Oboe"; break;
        case MIDINAMES_English_Horn: value = "English_Horn"; break;
        case MIDINAMES_Bassoon: value = "Bassoon"; break;
        case MIDINAMES_Clarinet: value = "Clarinet"; break;
        case MIDINAMES_Piccolo: value = "Piccolo"; break;
        case MIDINAMES_Flute: value = "Flute"; break;
        case MIDINAMES_Recorder: value = "Recorder"; break;
        case MIDINAMES_Pan_Flute: value = "Pan_Flute"; break;
        case MIDINAMES_Blown_Bottle: value = "Blown_Bottle"; break;
        case MIDINAMES_Shakuhachi: value = "Shakuhachi"; break;
        case MIDINAMES_Whistle: value = "Whistle"; break;
        case MIDINAMES_Ocarina: value = "Ocarina"; break;
        case MIDINAMES_Lead_1_square: value = "Lead_1_square"; break;
        case MIDINAMES_Lead_2_sawtooth: value = "Lead_2_sawtooth"; break;
        case MIDINAMES_Lead_3_calliope: value = "Lead_3_calliope"; break;
        case MIDINAMES_Lead_4_chiff: value = "Lead_4_chiff"; break;
        case MIDINAMES_Lead_5_charang: value = "Lead_5_charang"; break;
        case MIDINAMES_Lead_6_voice: value = "Lead_6_voice"; break;
        case MIDINAMES_Lead_7_fifths: value = "Lead_7_fifths"; break;
        case MIDINAMES_Lead_8_bass_and_lead: value = "Lead_8_bass_and_lead"; break;
        case MIDINAMES_Pad_1_new_age: value = "Pad_1_new_age"; break;
        case MIDINAMES_Pad_2_warm: value = "Pad_2_warm"; break;
        case MIDINAMES_Pad_3_polysynth: value = "Pad_3_polysynth"; break;
        case MIDINAMES_Pad_4_choir: value = "Pad_4_choir"; break;
        case MIDINAMES_Pad_5_bowed: value = "Pad_5_bowed"; break;
        case MIDINAMES_Pad_6_metallic: value = "Pad_6_metallic"; break;
        case MIDINAMES_Pad_7_halo: value = "Pad_7_halo"; break;
        case MIDINAMES_Pad_8_sweep: value = "Pad_8_sweep"; break;
        case MIDINAMES_FX_1_rain: value = "FX_1_rain"; break;
        case MIDINAMES_FX_2_soundtrack: value = "FX_2_soundtrack"; break;
        case MIDINAMES_FX_3_crystal: value = "FX_3_crystal"; break;
        case MIDINAMES_FX_4_atmosphere: value = "FX_4_atmosphere"; break;
        case MIDINAMES_FX_5_brightness: value = "FX_5_brightness"; break;
        case MIDINAMES_FX_6_goblins: value = "FX_6_goblins"; break;
        case MIDINAMES_FX_7_echoes: value = "FX_7_echoes"; break;
        case MIDINAMES_FX_8_sci_fi: value = "FX_8_sci-fi"; break;
        case MIDINAMES_Sitar: value = "Sitar"; break;
        case MIDINAMES_Banjo: value = "Banjo"; break;
        case MIDINAMES_Shamisen: value = "Shamisen"; break;
        case MIDINAMES_Koto: value = "Koto"; break;
        case MIDINAMES_Kalimba: value = "Kalimba"; break;
        case MIDINAMES_Bagpipe: value = "Bagpipe"; break;
        case MIDINAMES_Fiddle: value = "Fiddle"; break;
        case MIDINAMES_Shanai: value = "Shanai"; break;
        case MIDINAMES_Tinkle_Bell: value = "Tinkle_Bell"; break;
        case MIDINAMES_Agogo: value = "Agogo"; break;
        case MIDINAMES_Steel_Drums: value = "Steel_Drums"; break;
        case MIDINAMES_Woodblock: value = "Woodblock"; break;
        case MIDINAMES_Taiko_Drum: value = "Taiko_Drum"; break;
        case MIDINAMES_Melodic_Tom: value = "Melodic_Tom"; break;
        case MIDINAMES_Synth_Drum: value = "Synth_Drum"; break;
        case MIDINAMES_Reverse_Cymbal: value = "Reverse_Cymbal"; break;
        case MIDINAMES_Guitar_Fret_Noise: value = "Guitar_Fret_Noise"; break;
        case MIDINAMES_Breath_Noise: value = "Breath_Noise"; break;
        case MIDINAMES_Seashore: value = "Seashore"; break;
        case MIDINAMES_Bird_Tweet: value = "Bird_Tweet"; break;
        case MIDINAMES_Telephone_Ring: value = "Telephone_Ring"; break;
        case MIDINAMES_Helicopter: value = "Helicopter"; break;
        case MIDINAMES_Applause: value = "Applause"; break;
        case MIDINAMES_Gunshot: value = "Gunshot"; break;
        case MIDINAMES_Acoustic_Bass_Drum: value = "Acoustic_Bass_Drum"; break;
        case MIDINAMES_Bass_Drum_1: value = "Bass_Drum_1"; break;
        case MIDINAMES_Side_Stick: value = "Side_Stick"; break;
        case MIDINAMES_Acoustic_Snare: value = "Acoustic_Snare"; break;
        case MIDINAMES_Hand_Clap: value = "Hand_Clap"; break;
        case MIDINAMES_Electric_Snare: value = "Electric_Snare"; break;
        case MIDINAMES_Low_Floor_Tom: value = "Low_Floor_Tom"; break;
        case MIDINAMES_Closed_Hi_Hat: value = "Closed_Hi_Hat"; break;
        case MIDINAMES_High_Floor_Tom: value = "High_Floor_Tom"; break;
        case MIDINAMES_Pedal_Hi_Hat: value = "Pedal_Hi-Hat"; break;
        case MIDINAMES_Low_Tom: value = "Low_Tom"; break;
        case MIDINAMES_Open_Hi_Hat: value = "Open_Hi-Hat"; break;
        case MIDINAMES_Low_Mid_Tom: value = "Low-Mid_Tom"; break;
        case MIDINAMES_Hi_Mid_Tom: value = "Hi-Mid_Tom"; break;
        case MIDINAMES_Crash_Cymbal_1: value = "Crash_Cymbal_1"; break;
        case MIDINAMES_High_Tom: value = "High_Tom"; break;
        case MIDINAMES_Ride_Cymbal_1: value = "Ride_Cymbal_1"; break;
        case MIDINAMES_Chinese_Cymbal: value = "Chinese_Cymbal"; break;
        case MIDINAMES_Ride_Bell: value = "Ride_Bell"; break;
        case MIDINAMES_Tambourine: value = "Tambourine"; break;
        case MIDINAMES_Splash_Cymbal: value = "Splash_Cymbal"; break;
        case MIDINAMES_Cowbell: value = "Cowbell"; break;
        case MIDINAMES_Crash_Cymbal_2: value = "Crash_Cymbal_2"; break;
        case MIDINAMES_Vibraslap: value = "Vibraslap"; break;
        case MIDINAMES_Ride_Cymbal_2: value = "Ride_Cymbal_2"; break;
        case MIDINAMES_Hi_Bongo: value = "Hi_Bongo"; break;
        case MIDINAMES_Low_Bongo: value = "Low_Bongo"; break;
        case MIDINAMES_Mute_Hi_Conga: value = "Mute_Hi_Conga"; break;
        case MIDINAMES_Open_Hi_Conga: value = "Open_Hi_Conga"; break;
        case MIDINAMES_Low_Conga: value = "Low_Conga"; break;
        case MIDINAMES_High_Timbale: value = "High_Timbale"; break;
        case MIDINAMES_Low_Timbale: value = "Low_Timbale"; break;
        case MIDINAMES_High_Agogo: value = "High_Agogo"; break;
        case MIDINAMES_Low_Agogo: value = "Low_Agogo"; break;
        case MIDINAMES_Cabasa: value = "Cabasa"; break;
        case MIDINAMES_Maracas: value = "Maracas"; break;
        case MIDINAMES_Short_Whistle: value = "Short_Whistle"; break;
        case MIDINAMES_Long_Whistle: value = "Long_Whistle"; break;
        case MIDINAMES_Short_Guiro: value = "Short_Guiro"; break;
        case MIDINAMES_Long_Guiro: value = "Long_Guiro"; break;
        case MIDINAMES_Claves: value = "Claves"; break;
        case MIDINAMES_Hi_Wood_Block: value = "Hi_Wood_Block"; break;
        case MIDINAMES_Low_Wood_Block: value = "Low_Wood_Block"; break;
        case MIDINAMES_Mute_Cuica: value = "Mute_Cuica"; break;
        case MIDINAMES_Open_Cuica: value = "Open_Cuica"; break;
        case MIDINAMES_Mute_Triangle: value = "Mute_Triangle"; break;
        case MIDINAMES_Open_Triangle: value = "Open_Triangle"; break;
        default:
            LogWarning("Unknown value '%d' for data.MIDINAMES", data);
            value = "";
            break;
    }
    return value;
}

data_MIDINAMES AttConverter::StrToMidinames(std::string value) const
{
    if (value == "Acoustic_Grand_Piano") return MIDINAMES_Acoustic_Grand_Piano;
    if (value == "Bright_Acoustic_Piano") return MIDINAMES_Bright_Acoustic_Piano;
    if (value == "Electric_Grand_Piano") return MIDINAMES_Electric_Grand_Piano;
    if (value == "Honky-tonk_Piano") return MIDINAMES_Honky_tonk_Piano;
    if (value == "Electric_Piano_1") return MIDINAMES_Electric_Piano_1;
    if (value == "Electric_Piano_2") return MIDINAMES_Electric_Piano_2;
    if (value == "Harpsichord") return MIDINAMES_Harpsichord;
    if (value == "Clavi") return MIDINAMES_Clavi;
    if (value == "Celesta") return MIDINAMES_Celesta;
    if (value == "Glockenspiel") return MIDINAMES_Glockenspiel;
    if (value == "Music_Box") return MIDINAMES_Music_Box;
    if (value == "Vibraphone") return MIDINAMES_Vibraphone;
    if (value == "Marimba") return MIDINAMES_Marimba;
    if (value == "Xylophone") return MIDINAMES_Xylophone;
    if (value == "Tubular_Bells") return MIDINAMES_Tubular_Bells;
    if (value == "Dulcimer") return MIDINAMES_Dulcimer;
    if (value == "Drawbar_Organ") return MIDINAMES_Drawbar_Organ;
    if (value == "Percussive_Organ") return MIDINAMES_Percussive_Organ;
    if (value == "Rock_Organ") return MIDINAMES_Rock_Organ;
    if (value == "Church_Organ") return MIDINAMES_Church_Organ;
    if (value == "Reed_Organ") return MIDINAMES_Reed_Organ;
    if (value == "Accordion") return MIDINAMES_Accordion;
    if (value == "Harmonica") return MIDINAMES_Harmonica;
    if (value == "Tango_Accordion") return MIDINAMES_Tango_Accordion;
    if (value == "Acoustic_Guitar_nylon") return MIDINAMES_Acoustic_Guitar_nylon;
    if (value == "Acoustic_Guitar_steel") return MIDINAMES_Acoustic_Guitar_steel;
    if (value == "Electric_Guitar_jazz") return MIDINAMES_Electric_Guitar_jazz;
    if (value == "Electric_Guitar_clean") return MIDINAMES_Electric_Guitar_clean;
    if (value == "Electric_Guitar_muted") return MIDINAMES_Electric_Guitar_muted;
    if (value == "Overdriven_Guitar") return MIDINAMES_Overdriven_Guitar;
    if (value == "Distortion_Guitar") return MIDINAMES_Distortion_Guitar;
    if (value == "Guitar_harmonics") return MIDINAMES_Guitar_harmonics;
    if (value == "Acoustic_Bass") return MIDINAMES_Acoustic_Bass;
    if (value == "Electric_Bass_finger") return MIDINAMES_Electric_Bass_finger;
    if (value == "Electric_Bass_pick") return MIDINAMES_Electric_Bass_pick;
    if (value == "Fretless_Bass") return MIDINAMES_Fretless_Bass;
    if (value == "Slap_Bass_1") return MIDINAMES_Slap_Bass_1;
    if (value == "Slap_Bass_2") return MIDINAMES_Slap_Bass_2;
    if (value == "Synth_Bass_1") return MIDINAMES_Synth_Bass_1;
    if (value == "Synth_Bass_2") return MIDINAMES_Synth_Bass_2;
    if (value == "Violin") return MIDINAMES_Violin;
    if (value == "Viola") return MIDINAMES_Viola;
    if (value == "Cello") return MIDINAMES_Cello;
    if (value == "Contrabass") return MIDINAMES_Contrabass;
    if (value == "Tremolo_Strings") return MIDINAMES_Tremolo_Strings;
    if (value == "Pizzicato_Strings") return MIDINAMES_Pizzicato_Strings;
    if (value == "Orchestral_Harp") return MIDINAMES_Orchestral_Harp;
    if (value == "Timpani") return MIDINAMES_Timpani;
    if (value == "String_Ensemble_1") return MIDINAMES_String_Ensemble_1;
    if (value == "String_Ensemble_2") return MIDINAMES_String_Ensemble_2;
    if (value == "SynthStrings_1") return MIDINAMES_SynthStrings_1;
    if (value == "SynthStrings_2") return MIDINAMES_SynthStrings_2;
    if (value == "Choir_Aahs") return MIDINAMES_Choir_Aahs;
    if (value == "Voice_Oohs") return MIDINAMES_Voice_Oohs;
    if (value == "Synth_Voice") return MIDINAMES_Synth_Voice;
    if (value == "Orchestra_Hit") return MIDINAMES_Orchestra_Hit;
    if (value == "Trumpet") return MIDINAMES_Trumpet;
    if (value == "Trombone") return MIDINAMES_Trombone;
    if (value == "Tuba") return MIDINAMES_Tuba;
    if (value == "Muted_Trumpet") return MIDINAMES_Muted_Trumpet;
    if (value == "French_Horn") return MIDINAMES_French_Horn;
    if (value == "Brass_Section") return MIDINAMES_Brass_Section;
    if (value == "SynthBrass_1") return MIDINAMES_SynthBrass_1;
    if (value == "SynthBrass_2") return MIDINAMES_SynthBrass_2;
    if (value == "Soprano_Sax") return MIDINAMES_Soprano_Sax;
    if (value == "Alto_Sax") return MIDINAMES_Alto_Sax;
    if (value == "Tenor_Sax") return MIDINAMES_Tenor_Sax;
    if (value == "Baritone_Sax") return MIDINAMES_Baritone_Sax;
    if (value == "Oboe") return MIDINAMES_Oboe;
    if (value == "English_Horn") return MIDINAMES_English_Horn;
    if (value == "Bassoon") return MIDINAMES_Bassoon;
    if (value == "Clarinet") return MIDINAMES_Clarinet;
    if (value == "Piccolo") return MIDINAMES_Piccolo;
    if (value == "Flute") return MIDINAMES_Flute;
    if (value == "Recorder") return MIDINAMES_Recorder;
    if (value == "Pan_Flute") return MIDINAMES_Pan_Flute;
    if (value == "Blown_Bottle") return MIDINAMES_Blown_Bottle;
    if (value == "Shakuhachi") return MIDINAMES_Shakuhachi;
    if (value == "Whistle") return MIDINAMES_Whistle;
    if (value == "Ocarina") return MIDINAMES_Ocarina;
    if (value == "Lead_1_square") return MIDINAMES_Lead_1_square;
    if (value == "Lead_2_sawtooth") return MIDINAMES_Lead_2_sawtooth;
    if (value == "Lead_3_calliope") return MIDINAMES_Lead_3_calliope;
    if (value == "Lead_4_chiff") return MIDINAMES_Lead_4_chiff;
    if (value == "Lead_5_charang") return MIDINAMES_Lead_5_charang;
    if (value == "Lead_6_voice") return MIDINAMES_Lead_6_voice;
    if (value == "Lead_7_fifths") return MIDINAMES_Lead_7_fifths;
    if (value == "Lead_8_bass_and_lead") return MIDINAMES_Lead_8_bass_and_lead;
    if (value == "Pad_1_new_age") return MIDINAMES_Pad_1_new_age;
    if (value == "Pad_2_warm") return MIDINAMES_Pad_2_warm;
    if (value == "Pad_3_polysynth") return MIDINAMES_Pad_3_polysynth;
    if (value == "Pad_4_choir") return MIDINAMES_Pad_4_choir;
    if (value == "Pad_5_bowed") return MIDINAMES_Pad_5_bowed;
    if (value == "Pad_6_metallic") return MIDINAMES_Pad_6_metallic;
    if (value == "Pad_7_halo") return MIDINAMES_Pad_7_halo;
    if (value == "Pad_8_sweep") return MIDINAMES_Pad_8_sweep;
    if (value == "FX_1_rain") return MIDINAMES_FX_1_rain;
    if (value == "FX_2_soundtrack") return MIDINAMES_FX_2_soundtrack;
    if (value == "FX_3_crystal") return MIDINAMES_FX_3_crystal;
    if (value == "FX_4_atmosphere") return MIDINAMES_FX_4_atmosphere;
    if (value == "FX_5_brightness") return MIDINAMES_FX_5_brightness;
    if (value == "FX_6_goblins") return MIDINAMES_FX_6_goblins;
    if (value == "FX_7_echoes") return MIDINAMES_FX_7_echoes;
    if (value == "FX_8_sci-fi") return MIDINAMES_FX_8_sci_fi;
    if (value == "Sitar") return MIDINAMES_Sitar;
    if (value == "Banjo") return MIDINAMES_Banjo;
    if (value == "Shamisen") return MIDINAMES_Shamisen;
    if (value == "Koto") return MIDINAMES_Koto;
    if (value == "Kalimba") return MIDINAMES_Kalimba;
    if (value == "Bagpipe") return MIDINAMES_Bagpipe;
    if (value == "Fiddle") return MIDINAMES_Fiddle;
    if (value == "Shanai") return MIDINAMES_Shanai;
    if (value == "Tinkle_Bell") return MIDINAMES_Tinkle_Bell;
    if (value == "Agogo") return MIDINAMES_Agogo;
    if (value == "Steel_Drums") return MIDINAMES_Steel_Drums;
    if (value == "Woodblock") return MIDINAMES_Woodblock;
    if (value == "Taiko_Drum") return MIDINAMES_Taiko_Drum;
    if (value == "Melodic_Tom") return MIDINAMES_Melodic_Tom;
    if (value == "Synth_Drum") return MIDINAMES_Synth_Drum;
    if (value == "Reverse_Cymbal") return MIDINAMES_Reverse_Cymbal;
    if (value == "Guitar_Fret_Noise") return MIDINAMES_Guitar_Fret_Noise;
    if (value == "Breath_Noise") return MIDINAMES_Breath_Noise;
    if (value == "Seashore") return MIDINAMES_Seashore;
    if (value == "Bird_Tweet") return MIDINAMES_Bird_Tweet;
    if (value == "Telephone_Ring") return MIDINAMES_Telephone_Ring;
    if (value == "Helicopter") return MIDINAMES_Helicopter;
    if (value == "Applause") return MIDINAMES_Applause;
    if (value == "Gunshot") return MIDINAMES_Gunshot;
    if (value == "Acoustic_Bass_Drum") return MIDINAMES_Acoustic_Bass_Drum;
    if (value == "Bass_Drum_1") return MIDINAMES_Bass_Drum_1;
    if (value == "Side_Stick") return MIDINAMES_Side_Stick;
    if (value == "Acoustic_Snare") return MIDINAMES_Acoustic_Snare;
    if (value == "Hand_Clap") return MIDINAMES_Hand_Clap;
    if (value == "Electric_Snare") return MIDINAMES_Electric_Snare;
    if (value == "Low_Floor_Tom") return MIDINAMES_Low_Floor_Tom;
    if (value == "Closed_Hi_Hat") return MIDINAMES_Closed_Hi_Hat;
    if (value == "High_Floor_Tom") return MIDINAMES_High_Floor_Tom;
    if (value == "Pedal_Hi-Hat") return MIDINAMES_Pedal_Hi_Hat;
    if (value == "Low_Tom") return MIDINAMES_Low_Tom;
    if (value == "Open_Hi-Hat") return MIDINAMES_Open_Hi_Hat;
    if (value == "Low-Mid_Tom") return MIDINAMES_Low_Mid_Tom;
    if (value == "Hi-Mid_Tom") return MIDINAMES_Hi_Mid_Tom;
    if (value == "Crash_Cymbal_1") return MIDINAMES_Crash_Cymbal_1;
    if (value == "High_Tom") return MIDINAMES_High_Tom;
    if (value == "Ride_Cymbal_1") return MIDINAMES_Ride_Cymbal_1;
    if (value == "Chinese_Cymbal") return MIDINAMES_Chinese_Cymbal;
    if (value == "Ride_Bell") return MIDINAMES_Ride_Bell;
    if (value == "Tambourine") return MIDINAMES_Tambourine;
    if (value == "Splash_Cymbal") return MIDINAMES_Splash_Cymbal;
    if (value == "Cowbell") return MIDINAMES_Cowbell;
    if (value == "Crash_Cymbal_2") return MIDINAMES_Crash_Cymbal_2;
    if (value == "Vibraslap") return MIDINAMES_Vibraslap;
    if (value == "Ride_Cymbal_2") return MIDINAMES_Ride_Cymbal_2;
    if (value == "Hi_Bongo") return MIDINAMES_Hi_Bongo;
    if (value == "Low_Bongo") return MIDINAMES_Low_Bongo;
    if (value == "Mute_Hi_Conga") return MIDINAMES_Mute_Hi_Conga;
    if (value == "Open_Hi_Conga") return MIDINAMES_Open_Hi_Conga;
    if (value == "Low_Conga") return MIDINAMES_Low_Conga;
    if (value == "High_Timbale") return MIDINAMES_High_Timbale;
    if (value == "Low_Timbale") return MIDINAMES_Low_Timbale;
    if (value == "High_Agogo") return MIDINAMES_High_Agogo;
    if (value == "Low_Agogo") return MIDINAMES_Low_Agogo;
    if (value == "Cabasa") return MIDINAMES_Cabasa;
    if (value == "Maracas") return MIDINAMES_Maracas;
    if (value == "Short_Whistle") return MIDINAMES_Short_Whistle;
    if (value == "Long_Whistle") return MIDINAMES_Long_Whistle;
    if (value == "Short_Guiro") return MIDINAMES_Short_Guiro;
    if (value == "Long_Guiro") return MIDINAMES_Long_Guiro;
    if (value == "Claves") return MIDINAMES_Claves;
    if (value == "Hi_Wood_Block") return MIDINAMES_Hi_Wood_Block;
    if (value == "Low_Wood_Block") return MIDINAMES_Low_Wood_Block;
    if (value == "Mute_Cuica") return MIDINAMES_Mute_Cuica;
    if (value == "Open_Cuica") return MIDINAMES_Open_Cuica;
    if (value == "Mute_Triangle") return MIDINAMES_Mute_Triangle;
    if (value == "Open_Triangle") return MIDINAMES_Open_Triangle;
    LogWarning("Unsupported value '%s' for data.MIDINAMES", value.c_str());
    return MIDINAMES_NONE;
}

std::string AttConverter::ModeToStr(data_MODE data) const
{
    std::string value;
    switch (data) {
        case MODE_major: value = "major"; break;
        case MODE_minor: value = "minor"; break;
        case MODE_dorian: value = "dorian"; break;
        case MODE_phrygian: value = "phrygian"; break;
        case MODE_lydian: value = "lydian"; break;
        case MODE_mixolydian: value = "mixolydian"; break;
        case MODE_aeolian: value = "aeolian"; break;
        case MODE_locrian: value = "locrian"; break;
        default:
            LogWarning("Unknown value '%d' for data.MODE", data);
            value = "";
            break;
    }
    return value;
}

data_MODE AttConverter::StrToMode(std::string value) const
{
    if (value == "major") return MODE_major;
    if (value == "minor") return MODE_minor;
    if (value == "dorian") return MODE_dorian;
    if (value == "phrygian") return MODE_phrygian;
    if (value == "lydian") return MODE_lydian;
    if (value == "mixolydian") return MODE_mixolydian;
    if (value == "aeolian") return MODE_aeolian;
    if (value == "locrian") return MODE_locrian;
    LogWarning("Unsupported value '%s' for data.MODE", value.c_str());
    return MODE_NONE;
}

std::string AttConverter::ModsrelationshipToStr(data_MODSRELATIONSHIP data) const
{
    std::string value;
    switch (data) {
        case MODSRELATIONSHIP_preceding: value = "preceding"; break;
        case MODSRELATIONSHIP_succeeding: value = "succeeding"; break;
        case MODSRELATIONSHIP_original: value = "original"; break;
        case MODSRELATIONSHIP_host: value = "host"; break;
        case MODSRELATIONSHIP_constituent: value = "constituent"; break;
        case MODSRELATIONSHIP_otherVersion: value = "otherVersion"; break;
        case MODSRELATIONSHIP_otherFormat: value = "otherFormat"; break;
        case MODSRELATIONSHIP_isReferencedBy: value = "isReferencedBy"; break;
        case MODSRELATIONSHIP_references: value = "references"; break;
        default:
            LogWarning("Unknown value '%d' for data.MODSRELATIONSHIP", data);
            value = "";
            break;
    }
    return value;
}

data_MODSRELATIONSHIP AttConverter::StrToModsrelationship(std::string value) const
{
    if (value == "preceding") return MODSRELATIONSHIP_preceding;
    if (value == "succeeding") return MODSRELATIONSHIP_succeeding;
    if (value == "original") return MODSRELATIONSHIP_original;
    if (value == "host") return MODSRELATIONSHIP_host;
    if (value == "constituent") return MODSRELATIONSHIP_constituent;
    if (value == "otherVersion") return MODSRELATIONSHIP_otherVersion;
    if (value == "otherFormat") return MODSRELATIONSHIP_otherFormat;
    if (value == "isReferencedBy") return MODSRELATIONSHIP_isReferencedBy;
    if (value == "references") return MODSRELATIONSHIP_references;
    LogWarning("Unsupported value '%s' for data.MODSRELATIONSHIP", value.c_str());
    return MODSRELATIONSHIP_NONE;
}

std::string AttConverter::NonstaffplaceToStr(data_NONSTAFFPLACE data) const
{
    std::string value;
    switch (data) {
        case NONSTAFFPLACE_botmar: value = "botmar"; break;
        case NONSTAFFPLACE_topmar: value = "topmar"; break;
        case NONSTAFFPLACE_leftmar: value = "leftmar"; break;
        case NONSTAFFPLACE_rightmar: value = "rightmar"; break;
        case NONSTAFFPLACE_facing: value = "facing"; break;
        case NONSTAFFPLACE_overleaf: value = "overleaf"; break;
        case NONSTAFFPLACE_end: value = "end"; break;
        case NONSTAFFPLACE_inter: value = "inter"; break;
        case NONSTAFFPLACE_intra: value = "intra"; break;
        case NONSTAFFPLACE_super: value = "super"; break;
        case NONSTAFFPLACE_sub: value = "sub"; break;
        case NONSTAFFPLACE_inspace: value = "inspace"; break;
        case NONSTAFFPLACE_superimposed: value = "superimposed"; break;
        default:
            LogWarning("Unknown value '%d' for data.NONSTAFFPLACE", data);
            value = "";
            break;
    }
    return value;
}

data_NONSTAFFPLACE AttConverter::StrToNonstaffplace(std::string value) const
{
    if (value == "botmar") return NONSTAFFPLACE_botmar;
    if (value == "topmar") return NONSTAFFPLACE_topmar;
    if (value == "leftmar") return NONSTAFFPLACE_leftmar;
    if (value == "rightmar") return NONSTAFFPLACE_rightmar;
    if (value == "facing") return NONSTAFFPLACE_facing;
    if (value == "overleaf") return NONSTAFFPLACE_overleaf;
    if (value == "end") return NONSTAFFPLACE_end;
    if (value == "inter") return NONSTAFFPLACE_inter;
    if (value == "intra") return NONSTAFFPLACE_intra;
    if (value == "super") return NONSTAFFPLACE_super;
    if (value == "sub") return NONSTAFFPLACE_sub;
    if (value == "inspace") return NONSTAFFPLACE_inspace;
    if (value == "superimposed") return NONSTAFFPLACE_superimposed;
    LogWarning("Unsupported value '%s' for data.NONSTAFFPLACE", value.c_str());
    return NONSTAFFPLACE_NONE;
}

std::string AttConverter::NotationtypeToStr(data_NOTATIONTYPE data) const
{
    std::string value;
    switch (data) {
        case NOTATIONTYPE_cmn: value = "cmn"; break;
        case NOTATIONTYPE_mensural: value = "mensural"; break;
        case NOTATIONTYPE_mensural_black: value = "mensural.black"; break;
        case NOTATIONTYPE_mensural_white: value = "mensural.white"; break;
        case NOTATIONTYPE_neume: value = "neume"; break;
        case NOTATIONTYPE_tab: value = "tab"; break;
        default:
            LogWarning("Unknown value '%d' for data.NOTATIONTYPE", data);
            value = "";
            break;
    }
    return value;
}

data_NOTATIONTYPE AttConverter::StrToNotationtype(std::string value) const
{
    if (value == "cmn") return NOTATIONTYPE_cmn;
    if (value == "mensural") return NOTATIONTYPE_mensural;
    if (value == "mensural.black") return NOTATIONTYPE_mensural_black;
    if (value == "mensural.white") return NOTATIONTYPE_mensural_white;
    if (value == "neume") return NOTATIONTYPE_neume;
    if (value == "tab") return NOTATIONTYPE_tab;
    LogWarning("Unsupported value '%s' for data.NOTATIONTYPE", value.c_str());
    return NOTATIONTYPE_NONE;
}

std::string AttConverter::NoteheadmodifierListToStr(data_NOTEHEADMODIFIER_list data) const
{
    std::string value;
    switch (data) {
        case NOTEHEADMODIFIER_list_slash: value = "slash"; break;
        case NOTEHEADMODIFIER_list_backslash: value = "backslash"; break;
        case NOTEHEADMODIFIER_list_vline: value = "vline"; break;
        case NOTEHEADMODIFIER_list_hline: value = "hline"; break;
        case NOTEHEADMODIFIER_list_centerdot: value = "centerdot"; break;
        case NOTEHEADMODIFIER_list_paren: value = "paren"; break;
        case NOTEHEADMODIFIER_list_brack: value = "brack"; break;
        case NOTEHEADMODIFIER_list_box: value = "box"; break;
        case NOTEHEADMODIFIER_list_circle: value = "circle"; break;
        case NOTEHEADMODIFIER_list_dblwhole: value = "dblwhole"; break;
        default:
            LogWarning("Unknown value '%d' for data.NOTEHEADMODIFIER.list", data);
            value = "";
            break;
    }
    return value;
}

data_NOTEHEADMODIFIER_list AttConverter::StrToNoteheadmodifierList(std::string value) const
{
    if (value == "slash") return NOTEHEADMODIFIER_list_slash;
    if (value == "backslash") return NOTEHEADMODIFIER_list_backslash;
    if (value == "vline") return NOTEHEADMODIFIER_list_vline;
    if (value == "hline") return NOTEHEADMODIFIER_list_hline;
    if (value == "centerdot") return NOTEHEADMODIFIER_list_centerdot;
    if (value == "paren") return NOTEHEADMODIFIER_list_paren;
    if (value == "brack") return NOTEHEADMODIFIER_list_brack;
    if (value == "box") return NOTEHEADMODIFIER_list_box;
    if (value == "circle") return NOTEHEADMODIFIER_list_circle;
    if (value == "dblwhole") return NOTEHEADMODIFIER_list_dblwhole;
    LogWarning("Unsupported value '%s' for data.NOTEHEADMODIFIER.list", value.c_str());
    return NOTEHEADMODIFIER_list_NONE;
}

std::string AttConverter::OtherstaffToStr(data_OTHERSTAFF data) const
{
    std::string value;
    switch (data) {
        case OTHERSTAFF_above: value = "above"; break;
        case OTHERSTAFF_below: value = "below"; break;
        default:
            LogWarning("Unknown value '%d' for data.OTHERSTAFF", data);
            value = "";
            break;
    }
    return value;
}

data_OTHERSTAFF AttConverter::StrToOtherstaff(std::string value) const
{
    if (value == "above") return OTHERSTAFF_above;
    if (value == "below") return OTHERSTAFF_below;
    LogWarning("Unsupported value '%s' for data.OTHERSTAFF", value.c_str());
    return OTHERSTAFF_NONE;
}

std::string AttConverter::RotationdirectionToStr(data_ROTATIONDIRECTION data) const
{
    std::string value;
    switch (data) {
        case ROTATIONDIRECTION_none: value = "none"; break;
        case ROTATIONDIRECTION_down: value = "down"; break;
        case ROTATIONDIRECTION_left: value = "left"; break;
        case ROTATIONDIRECTION_ne: value = "ne"; break;
        case ROTATIONDIRECTION_nw: value = "nw"; break;
        case ROTATIONDIRECTION_se: value = "se"; break;
        case ROTATIONDIRECTION_sw: value = "sw"; break;
        default:
            LogWarning("Unknown value '%d' for data.ROTATIONDIRECTION", data);
            value = "";
            break;
    }
    return value;
}

data_ROTATIONDIRECTION AttConverter::StrToRotationdirection(std::string value) const
{
    if (value == "none") return ROTATIONDIRECTION_none;
    if (value == "down") return ROTATIONDIRECTION_down;
    if (value == "left") return ROTATIONDIRECTION_left;
    if (value == "ne") return ROTATIONDIRECTION_ne;
    if (value == "nw") return ROTATIONDIRECTION_nw;
    if (value == "se") return ROTATIONDIRECTION_se;
    if (value == "sw") return ROTATIONDIRECTION_sw;
    LogWarning("Unsupported value '%s' for data.ROTATIONDIRECTION", value.c_str());
    return ROTATIONDIRECTION_NONE;
}

std::string AttConverter::StaffitemBasicToStr(data_STAFFITEM_basic data) const
{
    std::string value;
    switch (data) {
        case STAFFITEM_basic_accid: value = "accid"; break;
        case STAFFITEM_basic_annot: value = "annot"; break;
        case STAFFITEM_basic_artic: value = "artic"; break;
        case STAFFITEM_basic_dir: value = "dir"; break;
        case STAFFITEM_basic_dynam: value = "dynam"; break;
        case STAFFITEM_basic_harm: value = "harm"; break;
        case STAFFITEM_basic_ornam: value = "ornam"; break;
        case STAFFITEM_basic_sp: value = "sp"; break;
        case STAFFITEM_basic_stageDir: value = "stageDir"; break;
        case STAFFITEM_basic_tempo: value = "tempo"; break;
        default:
            LogWarning("Unknown value '%d' for data.STAFFITEM.basic", data);
            value = "";
            break;
    }
    return value;
}

data_STAFFITEM_basic AttConverter::StrToStaffitemBasic(std::string value) const
{
    if (value == "accid") return STAFFITEM_basic_accid;
    if (value == "annot") return STAFFITEM_basic_annot;
    if (value == "artic") return STAFFITEM_basic_artic;
    if (value == "dir") return STAFFITEM_basic_dir;
    if (value == "dynam") return STAFFITEM_basic_dynam;
    if (value == "harm") return STAFFITEM_basic_harm;
    if (value == "ornam") return STAFFITEM_basic_ornam;
    if (value == "sp") return STAFFITEM_basic_sp;
    if (value == "stageDir") return STAFFITEM_basic_stageDir;
    if (value == "tempo") return STAFFITEM_basic_tempo;
    LogWarning("Unsupported value '%s' for data.STAFFITEM.basic", value.c_str());
    return STAFFITEM_basic_NONE;
}

std::string AttConverter::StaffitemCmnToStr(data_STAFFITEM_cmn data) const
{
    std::string value;
    switch (data) {
        case STAFFITEM_cmn_beam: value = "beam"; break;
        case STAFFITEM_cmn_bend: value = "bend"; break;
        case STAFFITEM_cmn_breath: value = "breath"; break;
        case STAFFITEM_cmn_cpMark: value = "cpMark"; break;
        case STAFFITEM_cmn_fermata: value = "fermata"; break;
        case STAFFITEM_cmn_fing: value = "fing"; break;
        case STAFFITEM_cmn_hairpin: value = "hairpin"; break;
        case STAFFITEM_cmn_harpPedal: value = "harpPedal"; break;
        case STAFFITEM_cmn_ligatureSpan: value = "ligatureSpan"; break;
        case STAFFITEM_cmn_lv: value = "lv"; break;
        case STAFFITEM_cmn_mordent: value = "mordent"; break;
        case STAFFITEM_cmn_octave: value = "octave"; break;
        case STAFFITEM_cmn_pedal: value = "pedal"; break;
        case STAFFITEM_cmn_reh: value = "reh"; break;
        case STAFFITEM_cmn_tie: value = "tie"; break;
        case STAFFITEM_cmn_trill: value = "trill"; break;
        case STAFFITEM_cmn_tuplet: value = "tuplet"; break;
        case STAFFITEM_cmn_turn: value = "turn"; break;
        default:
            LogWarning("Unknown value '%d' for data.STAFFITEM.cmn", data);
            value = "";
            break;
    }
    return value;
}

data_STAFFITEM_cmn AttConverter::StrToStaffitemCmn(std::string value) const
{
    if (value == "beam") return STAFFITEM_cmn_beam;
    if (value == "bend") return STAFFITEM_cmn_bend;
    if (value == "breath") return STAFFITEM_cmn_breath;
    if (value == "cpMark") return STAFFITEM_cmn_cpMark;
    if (value == "fermata") return STAFFITEM_cmn_fermata;
    if (value == "fing") return STAFFITEM_cmn_fing;
    if (value == "hairpin") return STAFFITEM_cmn_hairpin;
    if (value == "harpPedal") return STAFFITEM_cmn_harpPedal;
    if (value == "ligatureSpan") return STAFFITEM_cmn_ligatureSpan;
    if (value == "lv") return STAFFITEM_cmn_lv;
    if (value == "mordent") return STAFFITEM_cmn_mordent;
    if (value == "octave") return STAFFITEM_cmn_octave;
    if (value == "pedal") return STAFFITEM_cmn_pedal;
    if (value == "reh") return STAFFITEM_cmn_reh;
    if (value == "tie") return STAFFITEM_cmn_tie;
    if (value == "trill") return STAFFITEM_cmn_trill;
    if (value == "tuplet") return STAFFITEM_cmn_tuplet;
    if (value == "turn") return STAFFITEM_cmn_turn;
    LogWarning("Unsupported value '%s' for data.STAFFITEM.cmn", value.c_str());
    return STAFFITEM_cmn_NONE;
}

std::string AttConverter::StaffitemMensuralToStr(data_STAFFITEM_mensural data) const
{
    std::string value;
    switch (data) {
        case STAFFITEM_mensural_ligature: value = "ligature"; break;
        default:
            LogWarning("Unknown value '%d' for data.STAFFITEM.mensural", data);
            value = "";
            break;
    }
    return value;
}

data_STAFFITEM_mensural AttConverter::StrToStaffitemMensural(std::string value) const
{
    if (value == "ligature") return STAFFITEM_mensural_ligature;
    LogWarning("Unsupported value '%s' for data.STAFFITEM.mensural", value.c_str());
    return STAFFITEM_mensural_NONE;
}

std::string AttConverter::StaffrelBasicToStr(data_STAFFREL_basic data) const
{
    std::string value;
    switch (data) {
        case STAFFREL_basic_above: value = "above"; break;
        case STAFFREL_basic_below: value = "below"; break;
        default:
            LogWarning("Unknown value '%d' for data.STAFFREL.basic", data);
            value = "";
            break;
    }
    return value;
}

data_STAFFREL_basic AttConverter::StrToStaffrelBasic(std::string value) const
{
    if (value == "above") return STAFFREL_basic_above;
    if (value == "below") return STAFFREL_basic_below;
    LogWarning("Unsupported value '%s' for data.STAFFREL.basic", value.c_str());
    return STAFFREL_basic_NONE;
}

std::string AttConverter::StaffrelExtendedToStr(data_STAFFREL_extended data) const
{
    std::string value;
    switch (data) {
        case STAFFREL_extended_between: value = "between"; break;
        case STAFFREL_extended_within: value = "within"; break;
        default:
            LogWarning("Unknown value '%d' for data.STAFFREL.extended", data);
            value = "";
            break;
    }
    return value;
}

data_STAFFREL_extended AttConverter::StrToStaffrelExtended(std::string value) const
{
    if (value == "between") return STAFFREL_extended_between;
    if (value == "within") return STAFFREL_extended_within;
    LogWarning("Unsupported value '%s' for data.STAFFREL.extended", value.c_str());
    return STAFFREL_extended_NONE;
}

std::string AttConverter::StemmodifierToStr(data_STEMMODIFIER data) const
{
    std::string value;
    switch (data) {
        case STEMMODIFIER_none: value = "none"; break;
        case STEMMODIFIER_1slash: value = "1slash"; break;
        case STEMMODIFIER_2slash: value = "2slash"; break;
        case STEMMODIFIER_3slash: value = "3slash"; break;
        case STEMMODIFIER_4slash: value = "4slash"; break;
        case STEMMODIFIER_5slash: value = "5slash"; break;
        case STEMMODIFIER_6slash: value = "6slash"; break;
        case STEMMODIFIER_sprech: value = "sprech"; break;
        case STEMMODIFIER_z: value = "z"; break;
        default:
            LogWarning("Unknown value '%d' for data.STEMMODIFIER", data);
            value = "";
            break;
    }
    return value;
}

data_STEMMODIFIER AttConverter::StrToStemmodifier(std::string value) const
{
    if (value == "none") return STEMMODIFIER_none;
    if (value == "1slash") return STEMMODIFIER_1slash;
    if (value == "2slash") return STEMMODIFIER_2slash;
    if (value == "3slash") return STEMMODIFIER_3slash;
    if (value == "4slash") return STEMMODIFIER_4slash;
    if (value == "5slash") return STEMMODIFIER_5slash;
    if (value == "6slash") return STEMMODIFIER_6slash;
    if (value == "sprech") return STEMMODIFIER_sprech;
    if (value == "z") return STEMMODIFIER_z;
    LogWarning("Unsupported value '%s' for data.STEMMODIFIER", value.c_str());
    return STEMMODIFIER_NONE;
}

std::string AttConverter::StempositionToStr(data_STEMPOSITION data) const
{
    std::string value;
    switch (data) {
        case STEMPOSITION_left: value = "left"; break;
        case STEMPOSITION_right: value = "right"; break;
        case STEMPOSITION_center: value = "center"; break;
        default:
            LogWarning("Unknown value '%d' for data.STEMPOSITION", data);
            value = "";
            break;
    }
    return value;
}

data_STEMPOSITION AttConverter::StrToStemposition(std::string value) const
{
    if (value == "left") return STEMPOSITION_left;
    if (value == "right") return STEMPOSITION_right;
    if (value == "center") return STEMPOSITION_center;
    LogWarning("Unsupported value '%s' for data.STEMPOSITION", value.c_str());
    return STEMPOSITION_NONE;
}

std::string AttConverter::TextrenditionlistToStr(data_TEXTRENDITIONLIST data) const
{
    std::string value;
    switch (data) {
        case TEXTRENDITIONLIST_quote: value = "quote"; break;
        case TEXTRENDITIONLIST_quotedbl: value = "quotedbl"; break;
        case TEXTRENDITIONLIST_italic: value = "italic"; break;
        case TEXTRENDITIONLIST_oblique: value = "oblique"; break;
        case TEXTRENDITIONLIST_smcaps: value = "smcaps"; break;
        case TEXTRENDITIONLIST_bold: value = "bold"; break;
        case TEXTRENDITIONLIST_bolder: value = "bolder"; break;
        case TEXTRENDITIONLIST_lighter: value = "lighter"; break;
        case TEXTRENDITIONLIST_box: value = "box"; break;
        case TEXTRENDITIONLIST_circle: value = "circle"; break;
        case TEXTRENDITIONLIST_dbox: value = "dbox"; break;
        case TEXTRENDITIONLIST_tbox: value = "tbox"; break;
        case TEXTRENDITIONLIST_bslash: value = "bslash"; break;
        case TEXTRENDITIONLIST_fslash: value = "fslash"; break;
        case TEXTRENDITIONLIST_line_through: value = "line-through"; break;
        case TEXTRENDITIONLIST_none: value = "none"; break;
        case TEXTRENDITIONLIST_overline: value = "overline"; break;
        case TEXTRENDITIONLIST_overstrike: value = "overstrike"; break;
        case TEXTRENDITIONLIST_strike: value = "strike"; break;
        case TEXTRENDITIONLIST_sub: value = "sub"; break;
        case TEXTRENDITIONLIST_sup: value = "sup"; break;
        case TEXTRENDITIONLIST_superimpose: value = "superimpose"; break;
        case TEXTRENDITIONLIST_underline: value = "underline"; break;
        case TEXTRENDITIONLIST_x_through: value = "x-through"; break;
        case TEXTRENDITIONLIST_ltr: value = "ltr"; break;
        case TEXTRENDITIONLIST_rtl: value = "rtl"; break;
        case TEXTRENDITIONLIST_lro: value = "lro"; break;
        case TEXTRENDITIONLIST_rlo: value = "rlo"; break;
        default:
            LogWarning("Unknown value '%d' for data.TEXTRENDITIONLIST", data);
            value = "";
            break;
    }
    return value;
}

data_TEXTRENDITIONLIST AttConverter::StrToTextrenditionlist(std::string value) const
{
    if (value == "quote") return TEXTRENDITIONLIST_quote;
    if (value == "quotedbl") return TEXTRENDITIONLIST_quotedbl;
    if (value == "italic") return TEXTRENDITIONLIST_italic;
    if (value == "oblique") return TEXTRENDITIONLIST_oblique;
    if (value == "smcaps") return TEXTRENDITIONLIST_smcaps;
    if (value == "bold") return TEXTRENDITIONLIST_bold;
    if (value == "bolder") return TEXTRENDITIONLIST_bolder;
    if (value == "lighter") return TEXTRENDITIONLIST_lighter;
    if (value == "box") return TEXTRENDITIONLIST_box;
    if (value == "circle") return TEXTRENDITIONLIST_circle;
    if (value == "dbox") return TEXTRENDITIONLIST_dbox;
    if (value == "tbox") return TEXTRENDITIONLIST_tbox;
    if (value == "bslash") return TEXTRENDITIONLIST_bslash;
    if (value == "fslash") return TEXTRENDITIONLIST_fslash;
    if (value == "line-through") return TEXTRENDITIONLIST_line_through;
    if (value == "none") return TEXTRENDITIONLIST_none;
    if (value == "overline") return TEXTRENDITIONLIST_overline;
    if (value == "overstrike") return TEXTRENDITIONLIST_overstrike;
    if (value == "strike") return TEXTRENDITIONLIST_strike;
    if (value == "sub") return TEXTRENDITIONLIST_sub;
    if (value == "sup") return TEXTRENDITIONLIST_sup;
    if (value == "superimpose") return TEXTRENDITIONLIST_superimpose;
    if (value == "underline") return TEXTRENDITIONLIST_underline;
    if (value == "x-through") return TEXTRENDITIONLIST_x_through;
    if (value == "ltr") return TEXTRENDITIONLIST_ltr;
    if (value == "rtl") return TEXTRENDITIONLIST_rtl;
    if (value == "lro") return TEXTRENDITIONLIST_lro;
    if (value == "rlo") return TEXTRENDITIONLIST_rlo;
    LogWarning("Unsupported value '%s' for data.TEXTRENDITIONLIST", value.c_str());
    return TEXTRENDITIONLIST_NONE;
}

std::string AttConverter::AccidLogFuncToStr(accidLog_FUNC data) const
{
    std::string value;
    switch (data) {
        case accidLog_FUNC_caution: value = "caution"; break;
        case accidLog_FUNC_edit: value = "edit"; break;
        default:
            LogWarning("Unknown value '%d' for att.accid.log@func", data);
            value = "";
            break;
    }
    return value;
}

accidLog_FUNC AttConverter::StrToAccidLogFunc(std::string value) const
{
    if (value == "caution") return accidLog_FUNC_caution;
    if (value == "edit") return accidLog_FUNC_edit;
    LogWarning("Unsupported value '%s' for att.accid.log@func", value.c_str());
    return accidLog_FUNC_NONE;
}

std::string AttConverter::ArpegLogOrderToStr(arpegLog_ORDER data) const
{
    std::string value;
    switch (data) {
        case arpegLog_ORDER_up: value = "up"; break;
        case arpegLog_ORDER_down: value = "down"; break;
        case arpegLog_ORDER_nonarp: value = "nonarp"; break;
        default:
            LogWarning("Unknown value '%d' for att.arpeg.log@order", data);
            value = "";
            break;
    }
    return value;
}

arpegLog_ORDER AttConverter::StrToArpegLogOrder(std::string value) const
{
    if (value == "up") return arpegLog_ORDER_up;
    if (value == "down") return arpegLog_ORDER_down;
    if (value == "nonarp") return arpegLog_ORDER_nonarp;
    LogWarning("Unsupported value '%s' for att.arpeg.log@order", value.c_str());
    return arpegLog_ORDER_NONE;
}

std::string AttConverter::AudienceAudienceToStr(audience_AUDIENCE data) const
{
    std::string value;
    switch (data) {
        case audience_AUDIENCE_private: value = "private"; break;
        case audience_AUDIENCE_public: value = "public"; break;
        default:
            LogWarning("Unknown value '%d' for att.audience@audience", data);
            value = "";
            break;
    }
    return value;
}

audience_AUDIENCE AttConverter::StrToAudienceAudience(std::string value) const
{
    if (value == "private") return audience_AUDIENCE_private;
    if (value == "public") return audience_AUDIENCE_public;
    LogWarning("Unsupported value '%s' for att.audience@audience", value.c_str());
    return audience_AUDIENCE_NONE;
}

std::string AttConverter::BTremLogFormToStr(bTremLog_FORM data) const
{
    std::string value;
    switch (data) {
        case bTremLog_FORM_meas: value = "meas"; break;
        case bTremLog_FORM_unmeas: value = "unmeas"; break;
        default:
            LogWarning("Unknown value '%d' for att.bTrem.log@form", data);
            value = "";
            break;
    }
    return value;
}

bTremLog_FORM AttConverter::StrToBTremLogForm(std::string value) const
{
    if (value == "meas") return bTremLog_FORM_meas;
    if (value == "unmeas") return bTremLog_FORM_unmeas;
    LogWarning("Unsupported value '%s' for att.bTrem.log@form", value.c_str());
    return bTremLog_FORM_NONE;
}

std::string AttConverter::CurvatureCurvedirToStr(curvature_CURVEDIR data) const
{
    std::string value;
    switch (data) {
        case curvature_CURVEDIR_above: value = "above"; break;
        case curvature_CURVEDIR_below: value = "below"; break;
        case curvature_CURVEDIR_mixed: value = "mixed"; break;
        default:
            LogWarning("Unknown value '%d' for att.curvature@curvedir", data);
            value = "";
            break;
    }
    return value;
}

curvature_CURVEDIR AttConverter::StrToCurvatureCurvedir(std::string value) const
{
    if (value == "above") return curvature_CURVEDIR_above;
    if (value == "below") return curvature_CURVEDIR_below;
    if (value == "mixed") return curvature_CURVEDIR_mixed;
    LogWarning("Unsupported value '%s' for att.curvature@curvedir", value.c_str());
    return curvature_CURVEDIR_NONE;
}

std::string AttConverter::DotLogFormToStr(dotLog_FORM data) const
{
    std::string value;
    switch (data) {
        case dotLog_FORM_aug: value = "aug"; break;
        case dotLog_FORM_div: value = "div"; break;
        default:
            LogWarning("Unknown value '%d' for att.dot.log@form", data);
            value = "";
            break;
    }
    return value;
}

dotLog_FORM AttConverter::StrToDotLogForm(std::string value) const
{
    if (value == "aug") return dotLog_FORM_aug;
    if (value == "div") return dotLog_FORM_div;
    LogWarning("Unsupported value '%s' for att.dot.log@form", value.c_str());
    return dotLog_FORM_NONE;
}

std::string AttConverter::FTremLogFormToStr(fTremLog_FORM data) const
{
    std::string value;
    switch (data) {
        case fTremLog_FORM_meas: value = "meas"; break;
        case fTremLog_FORM_unmeas: value = "unmeas"; break;
        default:
            LogWarning("Unknown value '%d' for att.fTrem.log@form", data);
            value = "";
            break;
    }
    return value;
}

fTremLog_FORM AttConverter::StrToFTremLogForm(std::string value) const
{
    if (value == "meas") return fTremLog_FORM_meas;
    if (value == "unmeas") return fTremLog_FORM_unmeas;
    LogWarning("Unsupported value '%s' for att.fTrem.log@form", value.c_str());
    return fTremLog_FORM_NONE;
}

std::string AttConverter::GraceGrpLogAttachToStr(graceGrpLog_ATTACH data) const
{
    std::string value;
    switch (data) {
        case graceGrpLog_ATTACH_pre: value = "pre"; break;
        case graceGrpLog_ATTACH_post: value = "post"; break;
        case graceGrpLog_ATTACH_unknown: value = "unknown"; break;
        default:
            LogWarning("Unknown value '%d' for att.graceGrp.log@attach", data);
            value = "";
            break;
    }
    return value;
}

graceGrpLog_ATTACH AttConverter::StrToGraceGrpLogAttach(std::string value) const
{
    if (value == "pre") return graceGrpLog_ATTACH_pre;
    if (value == "post") return graceGrpLog_ATTACH_post;
    if (value == "unknown") return graceGrpLog_ATTACH_unknown;
    LogWarning("Unsupported value '%s' for att.graceGrp.log@attach", value.c_str());
    return graceGrpLog_ATTACH_NONE;
}

std::string AttConverter::HairpinLogFormToStr(hairpinLog_FORM data) const
{
    std::string value;
    switch (data) {
        case hairpinLog_FORM_cres: value = "cres"; break;
        case hairpinLog_FORM_dim: value = "dim"; break;
        default:
            LogWarning("Unknown value '%d' for att.hairpin.log@form", data);
            value = "";
            break;
    }
    return value;
}

hairpinLog_FORM AttConverter::StrToHairpinLogForm(std::string value) const
{
    if (value == "cres") return hairpinLog_FORM_cres;
    if (value == "dim") return hairpinLog_FORM_dim;
    LogWarning("Unsupported value '%s' for att.hairpin.log@form", value.c_str());
    return hairpinLog_FORM_NONE;
}

std::string AttConverter::HarpPedalLogAToStr(harpPedalLog_A data) const
{
    std::string value;
    switch (data) {
        case harpPedalLog_A_f: value = "f"; break;
        case harpPedalLog_A_n: value = "n"; break;
        case harpPedalLog_A_s: value = "s"; break;
        default:
            LogWarning("Unknown value '%d' for att.harpPedal.log@a", data);
            value = "";
            break;
    }
    return value;
}

harpPedalLog_A AttConverter::StrToHarpPedalLogA(std::string value) const
{
    if (value == "f") return harpPedalLog_A_f;
    if (value == "n") return harpPedalLog_A_n;
    if (value == "s") return harpPedalLog_A_s;
    LogWarning("Unsupported value '%s' for att.harpPedal.log@a", value.c_str());
    return harpPedalLog_A_NONE;
}

std::string AttConverter::HarpPedalLogBToStr(harpPedalLog_B data) const
{
    std::string value;
    switch (data) {
        case harpPedalLog_B_f: value = "f"; break;
        case harpPedalLog_B_n: value = "n"; break;
        case harpPedalLog_B_s: value = "s"; break;
        default:
            LogWarning("Unknown value '%d' for att.harpPedal.log@b", data);
            value = "";
            break;
    }
    return value;
}

harpPedalLog_B AttConverter::StrToHarpPedalLogB(std::string value) const
{
    if (value == "f") return harpPedalLog_B_f;
    if (value == "n") return harpPedalLog_B_n;
    if (value == "s") return harpPedalLog_B_s;
    LogWarning("Unsupported value '%s' for att.harpPedal.log@b", value.c_str());
    return harpPedalLog_B_NONE;
}

std::string AttConverter::HarpPedalLogCToStr(harpPedalLog_C data) const
{
    std::string value;
    switch (data) {
        case harpPedalLog_C_f: value = "f"; break;
        case harpPedalLog_C_n: value = "n"; break;
        case harpPedalLog_C_s: value = "s"; break;
        default:
            LogWarning("Unknown value '%d' for att.harpPedal.log@c", data);
            value = "";
            break;
    }
    return value;
}

harpPedalLog_C AttConverter::StrToHarpPedalLogC(std::string value) const
{
    if (value == "f") return harpPedalLog_C_f;
    if (value == "n") return harpPedalLog_C_n;
    if (value == "s") return harpPedalLog_C_s;
    LogWarning("Unsupported value '%s' for att.harpPedal.log@c", value.c_str());
    return harpPedalLog_C_NONE;
}

std::string AttConverter::HarpPedalLogDToStr(harpPedalLog_D data) const
{
    std::string value;
    switch (data) {
        case harpPedalLog_D_f: value = "f"; break;
        case harpPedalLog_D_n: value = "n"; break;
        case harpPedalLog_D_s: value = "s"; break;
        default:
            LogWarning("Unknown value '%d' for att.harpPedal.log@d", data);
            value = "";
            break;
    }
    return value;
}

harpPedalLog_D AttConverter::StrToHarpPedalLogD(std::string value) const
{
    if (value == "f") return harpPedalLog_D_f;
    if (value == "n") return harpPedalLog_D_n;
    if (value == "s") return harpPedalLog_D_s;
    LogWarning("Unsupported value '%s' for att.harpPedal.log@d", value.c_str());
    return harpPedalLog_D_NONE;
}

std::string AttConverter::HarpPedalLogEToStr(harpPedalLog_E data) const
{
    std::string value;
    switch (data) {
        case harpPedalLog_E_f: value = "f"; break;
        case harpPedalLog_E_n: value = "n"; break;
        case harpPedalLog_E_s: value = "s"; break;
        default:
            LogWarning("Unknown value '%d' for att.harpPedal.log@e", data);
            value = "";
            break;
    }
    return value;
}

harpPedalLog_E AttConverter::StrToHarpPedalLogE(std::string value) const
{
    if (value == "f") return harpPedalLog_E_f;
    if (value == "n") return harpPedalLog_E_n;
    if (value == "s") return harpPedalLog_E_s;
    LogWarning("Unsupported value '%s' for att.harpPedal.log@e", value.c_str());
    return harpPedalLog_E_NONE;
}

std::string AttConverter::HarpPedalLogFToStr(harpPedalLog_F data) const
{
    std::string value;
    switch (data) {
        case harpPedalLog_F_f: value = "f"; break;
        case harpPedalLog_F_n: value = "n"; break;
        case harpPedalLog_F_s: value = "s"; break;
        default:
            LogWarning("Unknown value '%d' for att.harpPedal.log@f", data);
            value = "";
            break;
    }
    return value;
}

harpPedalLog_F AttConverter::StrToHarpPedalLogF(std::string value) const
{
    if (value == "f") return harpPedalLog_F_f;
    if (value == "n") return harpPedalLog_F_n;
    if (value == "s") return harpPedalLog_F_s;
    LogWarning("Unsupported value '%s' for att.harpPedal.log@f", value.c_str());
    return harpPedalLog_F_NONE;
}

std::string AttConverter::HarpPedalLogGToStr(harpPedalLog_G data) const
{
    std::string value;
    switch (data) {
        case harpPedalLog_G_f: value = "f"; break;
        case harpPedalLog_G_n: value = "n"; break;
        case harpPedalLog_G_s: value = "s"; break;
        default:
            LogWarning("Unknown value '%d' for att.harpPedal.log@g", data);
            value = "";
            break;
    }
    return value;
}

harpPedalLog_G AttConverter::StrToHarpPedalLogG(std::string value) const
{
    if (value == "f") return harpPedalLog_G_f;
    if (value == "n") return harpPedalLog_G_n;
    if (value == "s") return harpPedalLog_G_s;
    LogWarning("Unsupported value '%s' for att.harpPedal.log@g", value.c_str());
    return harpPedalLog_G_NONE;
}

std::string AttConverter::MeiVersionMeiversionToStr(meiVersion_MEIVERSION data) const
{
    std::string value;
    switch (data) {
        case meiVersion_MEIVERSION_4_0_0: value = "4.0.0"; break;
        default:
            LogWarning("Unknown value '%d' for att.meiVersion@meiversion", data);
            value = "";
            break;
    }
    return value;
}

meiVersion_MEIVERSION AttConverter::StrToMeiVersionMeiversion(std::string value) const
{
    if (value == "4.0.0") return meiVersion_MEIVERSION_4_0_0;
    LogWarning("Unsupported value '%s' for att.meiVersion@meiversion", value.c_str());
    return meiVersion_MEIVERSION_NONE;
}

std::string AttConverter::MeterConformanceMetconToStr(meterConformance_METCON data) const
{
    std::string value;
    switch (data) {
        case meterConformance_METCON_c: value = "c"; break;
        case meterConformance_METCON_i: value = "i"; break;
        case meterConformance_METCON_o: value = "o"; break;
        default:
            LogWarning("Unknown value '%d' for att.meterConformance@metcon", data);
            value = "";
            break;
    }
    return value;
}

meterConformance_METCON AttConverter::StrToMeterConformanceMetcon(std::string value) const
{
    if (value == "c") return meterConformance_METCON_c;
    if (value == "i") return meterConformance_METCON_i;
    if (value == "o") return meterConformance_METCON_o;
    LogWarning("Unsupported value '%s' for att.meterConformance@metcon", value.c_str());
    return meterConformance_METCON_NONE;
}

std::string AttConverter::MeterSigGrpLogFuncToStr(meterSigGrpLog_FUNC data) const
{
    std::string value;
    switch (data) {
        case meterSigGrpLog_FUNC_alternating: value = "alternating"; break;
        case meterSigGrpLog_FUNC_interchanging: value = "interchanging"; break;
        case meterSigGrpLog_FUNC_mixed: value = "mixed"; break;
        default:
            LogWarning("Unknown value '%d' for att.meterSigGrp.log@func", data);
            value = "";
            break;
    }
    return value;
}

meterSigGrpLog_FUNC AttConverter::StrToMeterSigGrpLogFunc(std::string value) const
{
    if (value == "alternating") return meterSigGrpLog_FUNC_alternating;
    if (value == "interchanging") return meterSigGrpLog_FUNC_interchanging;
    if (value == "mixed") return meterSigGrpLog_FUNC_mixed;
    LogWarning("Unsupported value '%s' for att.meterSigGrp.log@func", value.c_str());
    return meterSigGrpLog_FUNC_NONE;
}

std::string AttConverter::MordentLogFormToStr(mordentLog_FORM data) const
{
    std::string value;
    switch (data) {
        case mordentLog_FORM_lower: value = "lower"; break;
        case mordentLog_FORM_upper: value = "upper"; break;
        default:
            LogWarning("Unknown value '%d' for att.mordent.log@form", data);
            value = "";
            break;
    }
    return value;
}

mordentLog_FORM AttConverter::StrToMordentLogForm(std::string value) const
{
    if (value == "lower") return mordentLog_FORM_lower;
    if (value == "upper") return mordentLog_FORM_upper;
    LogWarning("Unsupported value '%s' for att.mordent.log@form", value.c_str());
    return mordentLog_FORM_NONE;
}

std::string AttConverter::OctaveLogCollToStr(octaveLog_COLL data) const
{
    std::string value;
    switch (data) {
        case octaveLog_COLL_coll: value = "coll"; break;
        default:
            LogWarning("Unknown value '%d' for att.octave.log@coll", data);
            value = "";
            break;
    }
    return value;
}

octaveLog_COLL AttConverter::StrToOctaveLogColl(std::string value) const
{
    if (value == "coll") return octaveLog_COLL_coll;
    LogWarning("Unsupported value '%s' for att.octave.log@coll", value.c_str());
    return octaveLog_COLL_NONE;
}

std::string AttConverter::PedalLogDirToStr(pedalLog_DIR data) const
{
    std::string value;
    switch (data) {
        case pedalLog_DIR_down: value = "down"; break;
        case pedalLog_DIR_up: value = "up"; break;
        case pedalLog_DIR_half: value = "half"; break;
        case pedalLog_DIR_bounce: value = "bounce"; break;
        default:
            LogWarning("Unknown value '%d' for att.pedal.log@dir", data);
            value = "";
            break;
    }
    return value;
}

pedalLog_DIR AttConverter::StrToPedalLogDir(std::string value) const
{
    if (value == "down") return pedalLog_DIR_down;
    if (value == "up") return pedalLog_DIR_up;
    if (value == "half") return pedalLog_DIR_half;
    if (value == "bounce") return pedalLog_DIR_bounce;
    LogWarning("Unsupported value '%s' for att.pedal.log@dir", value.c_str());
    return pedalLog_DIR_NONE;
}

std::string AttConverter::PointingXlinkactuateToStr(pointing_XLINKACTUATE data) const
{
    std::string value;
    switch (data) {
        case pointing_XLINKACTUATE_onLoad: value = "onLoad"; break;
        case pointing_XLINKACTUATE_onRequest: value = "onRequest"; break;
        case pointing_XLINKACTUATE_none: value = "none"; break;
        case pointing_XLINKACTUATE_other: value = "other"; break;
        default:
            LogWarning("Unknown value '%d' for att.pointing@xlink:actuate", data);
            value = "";
            break;
    }
    return value;
}

pointing_XLINKACTUATE AttConverter::StrToPointingXlinkactuate(std::string value) const
{
    if (value == "onLoad") return pointing_XLINKACTUATE_onLoad;
    if (value == "onRequest") return pointing_XLINKACTUATE_onRequest;
    if (value == "none") return pointing_XLINKACTUATE_none;
    if (value == "other") return pointing_XLINKACTUATE_other;
    LogWarning("Unsupported value '%s' for att.pointing@xlink:actuate", value.c_str());
    return pointing_XLINKACTUATE_NONE;
}

std::string AttConverter::PointingXlinkshowToStr(pointing_XLINKSHOW data) const
{
    std::string value;
    switch (data) {
        case pointing_XLINKSHOW_new: value = "new"; break;
        case pointing_XLINKSHOW_replace: value = "replace"; break;
        case pointing_XLINKSHOW_embed: value = "embed"; break;
        case pointing_XLINKSHOW_none: value = "none"; break;
        case pointing_XLINKSHOW_other: value = "other"; break;
        default:
            LogWarning("Unknown value '%d' for att.pointing@xlink:show", data);
            value = "";
            break;
    }
    return value;
}

pointing_XLINKSHOW AttConverter::StrToPointingXlinkshow(std::string value) const
{
    if (value == "new") return pointing_XLINKSHOW_new;
    if (value == "replace") return pointing_XLINKSHOW_replace;
    if (value == "embed") return pointing_XLINKSHOW_embed;
    if (value == "none") return pointing_XLINKSHOW_none;
    if (value == "other") return pointing_XLINKSHOW_other;
    LogWarning("Unsupported value '%s' for att.pointing@xlink:show", value.c_str());
    return pointing_XLINKSHOW_NONE;
}

std::string AttConverter::RegularMethodMethodToStr(regularMethod_METHOD data) const
{
    std::string value;
    switch (data) {
        case regularMethod_METHOD_silent: value = "silent"; break;
        case regularMethod_METHOD_tags: value = "tags"; break;
        default:
            LogWarning("Unknown value '%d' for att.regularMethod@method", data);
            value = "";
            break;
    }
    return value;
}

regularMethod_METHOD AttConverter::StrToRegularMethodMethod(std::string value) const
{
    if (value == "silent") return regularMethod_METHOD_silent;
    if (value == "tags") return regularMethod_METHOD_tags;
    LogWarning("Unsupported value '%s' for att.regularMethod@method", value.c_str());
    return regularMethod_METHOD_NONE;
}

std::string AttConverter::StaffGroupingSymSymbolToStr(staffGroupingSym_SYMBOL data) const
{
    std::string value;
    switch (data) {
        case staffGroupingSym_SYMBOL_brace: value = "brace"; break;
        case staffGroupingSym_SYMBOL_bracket: value = "bracket"; break;
        case staffGroupingSym_SYMBOL_bracketsq: value = "bracketsq"; break;
        case staffGroupingSym_SYMBOL_line: value = "line"; break;
        case staffGroupingSym_SYMBOL_none: value = "none"; break;
        default:
            LogWarning("Unknown value '%d' for att.staffGroupingSym@symbol", data);
            value = "";
            break;
    }
    return value;
}

staffGroupingSym_SYMBOL AttConverter::StrToStaffGroupingSymSymbol(std::string value) const
{
    if (value == "brace") return staffGroupingSym_SYMBOL_brace;
    if (value == "bracket") return staffGroupingSym_SYMBOL_bracket;
    if (value == "bracketsq") return staffGroupingSym_SYMBOL_bracketsq;
    if (value == "line") return staffGroupingSym_SYMBOL_line;
    if (value == "none") return staffGroupingSym_SYMBOL_none;
    LogWarning("Unsupported value '%s' for att.staffGroupingSym@symbol", value.c_str());
    return staffGroupingSym_SYMBOL_NONE;
}

std::string AttConverter::SylLogConToStr(sylLog_CON data) const
{
    std::string value;
    switch (data) {
        case sylLog_CON_s: value = "s"; break;
        case sylLog_CON_d: value = "d"; break;
        case sylLog_CON_u: value = "u"; break;
        case sylLog_CON_t: value = "t"; break;
        case sylLog_CON_c: value = "c"; break;
        case sylLog_CON_v: value = "v"; break;
        case sylLog_CON_i: value = "i"; break;
        case sylLog_CON_b: value = "b"; break;
        default:
            LogWarning("Unknown value '%d' for att.syl.log@con", data);
            value = "";
            break;
    }
    return value;
}

sylLog_CON AttConverter::StrToSylLogCon(std::string value) const
{
    if (value == "s") return sylLog_CON_s;
    if (value == "d") return sylLog_CON_d;
    if (value == "u") return sylLog_CON_u;
    if (value == "t") return sylLog_CON_t;
    if (value == "c") return sylLog_CON_c;
    if (value == "v") return sylLog_CON_v;
    if (value == "i") return sylLog_CON_i;
    if (value == "b") return sylLog_CON_b;
    LogWarning("Unsupported value '%s' for att.syl.log@con", value.c_str());
    return sylLog_CON_NONE;
}

std::string AttConverter::SylLogWordposToStr(sylLog_WORDPOS data) const
{
    std::string value;
    switch (data) {
        case sylLog_WORDPOS_i: value = "i"; break;
        case sylLog_WORDPOS_m: value = "m"; break;
        case sylLog_WORDPOS_t: value = "t"; break;
        default:
            LogWarning("Unknown value '%d' for att.syl.log@wordpos", data);
            value = "";
            break;
    }
    return value;
}

sylLog_WORDPOS AttConverter::StrToSylLogWordpos(std::string value) const
{
    if (value == "i") return sylLog_WORDPOS_i;
    if (value == "m") return sylLog_WORDPOS_m;
    if (value == "t") return sylLog_WORDPOS_t;
    LogWarning("Unsupported value '%s' for att.syl.log@wordpos", value.c_str());
    return sylLog_WORDPOS_NONE;
}

std::string AttConverter::TargetEvalEvaluateToStr(targetEval_EVALUATE data) const
{
    std::string value;
    switch (data) {
        case targetEval_EVALUATE_all: value = "all"; break;
        case targetEval_EVALUATE_one: value = "one"; break;
        case targetEval_EVALUATE_none: value = "none"; break;
        default:
            LogWarning("Unknown value '%d' for att.targetEval@evaluate", data);
            value = "";
            break;
    }
    return value;
}

targetEval_EVALUATE AttConverter::StrToTargetEvalEvaluate(std::string value) const
{
    if (value == "all") return targetEval_EVALUATE_all;
    if (value == "one") return targetEval_EVALUATE_one;
    if (value == "none") return targetEval_EVALUATE_none;
    LogWarning("Unsupported value '%s' for att.targetEval@evaluate", value.c_str());
    return targetEval_EVALUATE_NONE;
}

std::string AttConverter::TempoLogFuncToStr(tempoLog_FUNC data) const
{
    std::string value;
    switch (data) {
        case tempoLog_FUNC_continuous: value = "continuous"; break;
        case tempoLog_FUNC_instantaneous: value = "instantaneous"; break;
        case tempoLog_FUNC_metricmod: value = "metricmod"; break;
        case tempoLog_FUNC_precedente: value = "precedente"; break;
        default:
            LogWarning("Unknown value '%d' for att.tempo.log@func", data);
            value = "";
            break;
    }
    return value;
}

tempoLog_FUNC AttConverter::StrToTempoLogFunc(std::string value) const
{
    if (value == "continuous") return tempoLog_FUNC_continuous;
    if (value == "instantaneous") return tempoLog_FUNC_instantaneous;
    if (value == "metricmod") return tempoLog_FUNC_metricmod;
    if (value == "precedente") return tempoLog_FUNC_precedente;
    LogWarning("Unsupported value '%s' for att.tempo.log@func", value.c_str());
    return tempoLog_FUNC_NONE;
}

std::string AttConverter::TurnLogFormToStr(turnLog_FORM data) const
{
    std::string value;
    switch (data) {
        case turnLog_FORM_lower: value = "lower"; break;
        case turnLog_FORM_upper: value = "upper"; break;
        default:
            LogWarning("Unknown value '%d' for att.turn.log@form", data);
            value = "";
            break;
    }
    return value;
}

turnLog_FORM AttConverter::StrToTurnLogForm(std::string value) const
{
    if (value == "lower") return turnLog_FORM_lower;
    if (value == "upper") return turnLog_FORM_upper;
    LogWarning("Unsupported value '%s' for att.turn.log@form", value.c_str());
    return turnLog_FORM_NONE;
}

std::string AttConverter::WhitespaceXmlspaceToStr(whitespace_XMLSPACE data) const
{
    std::string value;
    switch (data) {
        case whitespace_XMLSPACE_default: value = "default"; break;
        case whitespace_XMLSPACE_preserve: value = "preserve"; break;
        default:
            LogWarning("Unknown value '%d' for att.whitespace@xml:space", data);
            value = "";
            break;
    }
    return value;
}

whitespace_XMLSPACE AttConverter::StrToWhitespaceXmlspace(std::string value) const
{
    if (value == "default") return whitespace_XMLSPACE_default;
    if (value == "preserve") return whitespace_XMLSPACE_preserve;
    LogWarning("Unsupported value '%s' for att.whitespace@xml:space", value.c_str());
    return whitespace_XMLSPACE_NONE;
}

} // vrv namespace

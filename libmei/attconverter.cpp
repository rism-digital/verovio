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

std::string AttConverter::AccidentalGesturalToStr(data_ACCIDENTAL_GESTURAL data) const
{
    std::string value;
    switch (data) {
        case ACCIDENTAL_GESTURAL_s: value = "s"; break;
        case ACCIDENTAL_GESTURAL_f: value = "f"; break;
        case ACCIDENTAL_GESTURAL_ss: value = "ss"; break;
        case ACCIDENTAL_GESTURAL_ff: value = "ff"; break;
        case ACCIDENTAL_GESTURAL_n: value = "n"; break;
        case ACCIDENTAL_GESTURAL_su: value = "su"; break;
        case ACCIDENTAL_GESTURAL_sd: value = "sd"; break;
        case ACCIDENTAL_GESTURAL_fu: value = "fu"; break;
        case ACCIDENTAL_GESTURAL_fd: value = "fd"; break;
        default:
            LogWarning("Unknown value '%d' for data.ACCIDENTAL.GESTURAL", data);
            value = "";
            break;
    }
    return value;
}

data_ACCIDENTAL_GESTURAL AttConverter::StrToAccidentalGestural(std::string value, bool logWarning) const
{
    if (value == "s") return ACCIDENTAL_GESTURAL_s;
    if (value == "f") return ACCIDENTAL_GESTURAL_f;
    if (value == "ss") return ACCIDENTAL_GESTURAL_ss;
    if (value == "ff") return ACCIDENTAL_GESTURAL_ff;
    if (value == "n") return ACCIDENTAL_GESTURAL_n;
    if (value == "su") return ACCIDENTAL_GESTURAL_su;
    if (value == "sd") return ACCIDENTAL_GESTURAL_sd;
    if (value == "fu") return ACCIDENTAL_GESTURAL_fu;
    if (value == "fd") return ACCIDENTAL_GESTURAL_fd;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.ACCIDENTAL.GESTURAL", value.c_str());
    return ACCIDENTAL_GESTURAL_NONE;
}

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

data_ACCIDENTAL_WRITTEN AttConverter::StrToAccidentalWritten(std::string value, bool logWarning) const
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
    if (logWarning && !value.empty())
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

data_ARTICULATION AttConverter::StrToArticulation(std::string value, bool logWarning) const
{
    if (value == "acc") return ARTICULATION_acc;
    if (value == "stacc") return ARTICULATION_stacc;
    if (value == "ten") return ARTICULATION_ten;
    if (value == "stacciss") return ARTICULATION_stacciss;
    if (value == "marc") return ARTICULATION_marc;
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
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.ARTICULATION", value.c_str());
    return ARTICULATION_NONE;
}

std::string AttConverter::BarmethodToStr(data_BARMETHOD data) const
{
    std::string value;
    switch (data) {
        case BARMETHOD_mensur: value = "mensur"; break;
        case BARMETHOD_staff: value = "staff"; break;
        case BARMETHOD_takt: value = "takt"; break;
        default:
            LogWarning("Unknown value '%d' for data.BARMETHOD", data);
            value = "";
            break;
    }
    return value;
}

data_BARMETHOD AttConverter::StrToBarmethod(std::string value, bool logWarning) const
{
    if (value == "mensur") return BARMETHOD_mensur;
    if (value == "staff") return BARMETHOD_staff;
    if (value == "takt") return BARMETHOD_takt;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.BARMETHOD", value.c_str());
    return BARMETHOD_NONE;
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

data_BARRENDITION AttConverter::StrToBarrendition(std::string value, bool logWarning) const
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
    if (logWarning && !value.empty())
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

data_BEAMPLACE AttConverter::StrToBeamplace(std::string value, bool logWarning) const
{
    if (value == "above") return BEAMPLACE_above;
    if (value == "below") return BEAMPLACE_below;
    if (value == "mixed") return BEAMPLACE_mixed;
    if (logWarning && !value.empty())
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

data_BETYPE AttConverter::StrToBetype(std::string value, bool logWarning) const
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
    if (logWarning && !value.empty())
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

data_BOOLEAN AttConverter::StrToBoolean(std::string value, bool logWarning) const
{
    if (value == "true") return BOOLEAN_true;
    if (value == "false") return BOOLEAN_false;
    if (logWarning && !value.empty())
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

data_CERTAINTY AttConverter::StrToCertainty(std::string value, bool logWarning) const
{
    if (value == "high") return CERTAINTY_high;
    if (value == "medium") return CERTAINTY_medium;
    if (value == "low") return CERTAINTY_low;
    if (value == "unknown") return CERTAINTY_unknown;
    if (logWarning && !value.empty())
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

data_CLEFSHAPE AttConverter::StrToClefshape(std::string value, bool logWarning) const
{
    if (value == "G") return CLEFSHAPE_G;
    if (value == "GG") return CLEFSHAPE_GG;
    if (value == "F") return CLEFSHAPE_F;
    if (value == "C") return CLEFSHAPE_C;
    if (value == "perc") return CLEFSHAPE_perc;
    if (value == "TAB") return CLEFSHAPE_TAB;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.CLEFSHAPE", value.c_str());
    return CLEFSHAPE_NONE;
}

std::string AttConverter::ClusterToStr(data_CLUSTER data) const
{
    std::string value;
    switch (data) {
        case CLUSTER_white: value = "white"; break;
        case CLUSTER_black: value = "black"; break;
        case CLUSTER_chromatic: value = "chromatic"; break;
        default:
            LogWarning("Unknown value '%d' for data.CLUSTER", data);
            value = "";
            break;
    }
    return value;
}

data_CLUSTER AttConverter::StrToCluster(std::string value, bool logWarning) const
{
    if (value == "white") return CLUSTER_white;
    if (value == "black") return CLUSTER_black;
    if (value == "chromatic") return CLUSTER_chromatic;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.CLUSTER", value.c_str());
    return CLUSTER_NONE;
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
        case COLORNAMES_rebeccapurple: value = "rebeccapurple"; break;
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

data_COLORNAMES AttConverter::StrToColornames(std::string value, bool logWarning) const
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
    if (value == "rebeccapurple") return COLORNAMES_rebeccapurple;
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
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.COLORNAMES", value.c_str());
    return COLORNAMES_NONE;
}

std::string AttConverter::CompassdirectionToStr(data_COMPASSDIRECTION data) const
{
    std::string value;
    switch (data) {
        case COMPASSDIRECTION_n: value = "n"; break;
        case COMPASSDIRECTION_e: value = "e"; break;
        case COMPASSDIRECTION_s: value = "s"; break;
        case COMPASSDIRECTION_w: value = "w"; break;
        case COMPASSDIRECTION_ne: value = "ne"; break;
        case COMPASSDIRECTION_nw: value = "nw"; break;
        case COMPASSDIRECTION_se: value = "se"; break;
        case COMPASSDIRECTION_sw: value = "sw"; break;
        default:
            LogWarning("Unknown value '%d' for data.COMPASSDIRECTION", data);
            value = "";
            break;
    }
    return value;
}

data_COMPASSDIRECTION AttConverter::StrToCompassdirection(std::string value, bool logWarning) const
{
    if (value == "n") return COMPASSDIRECTION_n;
    if (value == "e") return COMPASSDIRECTION_e;
    if (value == "s") return COMPASSDIRECTION_s;
    if (value == "w") return COMPASSDIRECTION_w;
    if (value == "ne") return COMPASSDIRECTION_ne;
    if (value == "nw") return COMPASSDIRECTION_nw;
    if (value == "se") return COMPASSDIRECTION_se;
    if (value == "sw") return COMPASSDIRECTION_sw;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.COMPASSDIRECTION", value.c_str());
    return COMPASSDIRECTION_NONE;
}

std::string AttConverter::CompassdirectionBasicToStr(data_COMPASSDIRECTION_basic data) const
{
    std::string value;
    switch (data) {
        case COMPASSDIRECTION_basic_n: value = "n"; break;
        case COMPASSDIRECTION_basic_e: value = "e"; break;
        case COMPASSDIRECTION_basic_s: value = "s"; break;
        case COMPASSDIRECTION_basic_w: value = "w"; break;
        default:
            LogWarning("Unknown value '%d' for data.COMPASSDIRECTION.basic", data);
            value = "";
            break;
    }
    return value;
}

data_COMPASSDIRECTION_basic AttConverter::StrToCompassdirectionBasic(std::string value, bool logWarning) const
{
    if (value == "n") return COMPASSDIRECTION_basic_n;
    if (value == "e") return COMPASSDIRECTION_basic_e;
    if (value == "s") return COMPASSDIRECTION_basic_s;
    if (value == "w") return COMPASSDIRECTION_basic_w;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.COMPASSDIRECTION.basic", value.c_str());
    return COMPASSDIRECTION_basic_NONE;
}

std::string AttConverter::CompassdirectionExtendedToStr(data_COMPASSDIRECTION_extended data) const
{
    std::string value;
    switch (data) {
        case COMPASSDIRECTION_extended_ne: value = "ne"; break;
        case COMPASSDIRECTION_extended_nw: value = "nw"; break;
        case COMPASSDIRECTION_extended_se: value = "se"; break;
        case COMPASSDIRECTION_extended_sw: value = "sw"; break;
        default:
            LogWarning("Unknown value '%d' for data.COMPASSDIRECTION.extended", data);
            value = "";
            break;
    }
    return value;
}

data_COMPASSDIRECTION_extended AttConverter::StrToCompassdirectionExtended(std::string value, bool logWarning) const
{
    if (value == "ne") return COMPASSDIRECTION_extended_ne;
    if (value == "nw") return COMPASSDIRECTION_extended_nw;
    if (value == "se") return COMPASSDIRECTION_extended_se;
    if (value == "sw") return COMPASSDIRECTION_extended_sw;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.COMPASSDIRECTION.extended", value.c_str());
    return COMPASSDIRECTION_extended_NONE;
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

data_ENCLOSURE AttConverter::StrToEnclosure(std::string value, bool logWarning) const
{
    if (value == "paren") return ENCLOSURE_paren;
    if (value == "brack") return ENCLOSURE_brack;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.ENCLOSURE", value.c_str());
    return ENCLOSURE_NONE;
}

std::string AttConverter::EventrelToStr(data_EVENTREL data) const
{
    std::string value;
    switch (data) {
        case EVENTREL_above: value = "above"; break;
        case EVENTREL_below: value = "below"; break;
        case EVENTREL_left: value = "left"; break;
        case EVENTREL_right: value = "right"; break;
        case EVENTREL_above_left: value = "above-left"; break;
        case EVENTREL_above_right: value = "above-right"; break;
        case EVENTREL_below_left: value = "below-left"; break;
        case EVENTREL_below_right: value = "below-right"; break;
        default:
            LogWarning("Unknown value '%d' for data.EVENTREL", data);
            value = "";
            break;
    }
    return value;
}

data_EVENTREL AttConverter::StrToEventrel(std::string value, bool logWarning) const
{
    if (value == "above") return EVENTREL_above;
    if (value == "below") return EVENTREL_below;
    if (value == "left") return EVENTREL_left;
    if (value == "right") return EVENTREL_right;
    if (value == "above-left") return EVENTREL_above_left;
    if (value == "above-right") return EVENTREL_above_right;
    if (value == "below-left") return EVENTREL_below_left;
    if (value == "below-right") return EVENTREL_below_right;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.EVENTREL", value.c_str());
    return EVENTREL_NONE;
}

std::string AttConverter::EventrelBasicToStr(data_EVENTREL_basic data) const
{
    std::string value;
    switch (data) {
        case EVENTREL_basic_above: value = "above"; break;
        case EVENTREL_basic_below: value = "below"; break;
        case EVENTREL_basic_left: value = "left"; break;
        case EVENTREL_basic_right: value = "right"; break;
        default:
            LogWarning("Unknown value '%d' for data.EVENTREL.basic", data);
            value = "";
            break;
    }
    return value;
}

data_EVENTREL_basic AttConverter::StrToEventrelBasic(std::string value, bool logWarning) const
{
    if (value == "above") return EVENTREL_basic_above;
    if (value == "below") return EVENTREL_basic_below;
    if (value == "left") return EVENTREL_basic_left;
    if (value == "right") return EVENTREL_basic_right;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.EVENTREL.basic", value.c_str());
    return EVENTREL_basic_NONE;
}

std::string AttConverter::EventrelExtendedToStr(data_EVENTREL_extended data) const
{
    std::string value;
    switch (data) {
        case EVENTREL_extended_above_left: value = "above-left"; break;
        case EVENTREL_extended_above_right: value = "above-right"; break;
        case EVENTREL_extended_below_left: value = "below-left"; break;
        case EVENTREL_extended_below_right: value = "below-right"; break;
        default:
            LogWarning("Unknown value '%d' for data.EVENTREL.extended", data);
            value = "";
            break;
    }
    return value;
}

data_EVENTREL_extended AttConverter::StrToEventrelExtended(std::string value, bool logWarning) const
{
    if (value == "above-left") return EVENTREL_extended_above_left;
    if (value == "above-right") return EVENTREL_extended_above_right;
    if (value == "below-left") return EVENTREL_extended_below_left;
    if (value == "below-right") return EVENTREL_extended_below_right;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.EVENTREL.extended", value.c_str());
    return EVENTREL_extended_NONE;
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

data_FILL AttConverter::StrToFill(std::string value, bool logWarning) const
{
    if (value == "void") return FILL_void;
    if (value == "solid") return FILL_solid;
    if (value == "top") return FILL_top;
    if (value == "bottom") return FILL_bottom;
    if (value == "left") return FILL_left;
    if (value == "right") return FILL_right;
    if (logWarning && !value.empty())
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
        default:
            LogWarning("Unknown value '%d' for data.FONTSIZETERM", data);
            value = "";
            break;
    }
    return value;
}

data_FONTSIZETERM AttConverter::StrToFontsizeterm(std::string value, bool logWarning) const
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
    if (logWarning && !value.empty())
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

data_FONTSTYLE AttConverter::StrToFontstyle(std::string value, bool logWarning) const
{
    if (value == "italic") return FONTSTYLE_italic;
    if (value == "normal") return FONTSTYLE_normal;
    if (value == "oblique") return FONTSTYLE_oblique;
    if (logWarning && !value.empty())
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

data_FONTWEIGHT AttConverter::StrToFontweight(std::string value, bool logWarning) const
{
    if (value == "bold") return FONTWEIGHT_bold;
    if (value == "normal") return FONTWEIGHT_normal;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.FONTWEIGHT", value.c_str());
    return FONTWEIGHT_NONE;
}

std::string AttConverter::FrbrrelationshipToStr(data_FRBRRELATIONSHIP data) const
{
    std::string value;
    switch (data) {
        case FRBRRELATIONSHIP_hasAbridgement: value = "hasAbridgement"; break;
        case FRBRRELATIONSHIP_isAbridgementOf: value = "isAbridgementOf"; break;
        case FRBRRELATIONSHIP_hasAdaptation: value = "hasAdaptation"; break;
        case FRBRRELATIONSHIP_isAdaptationOf: value = "isAdaptationOf"; break;
        case FRBRRELATIONSHIP_hasAlternate: value = "hasAlternate"; break;
        case FRBRRELATIONSHIP_isAlternateOf: value = "isAlternateOf"; break;
        case FRBRRELATIONSHIP_hasArrangement: value = "hasArrangement"; break;
        case FRBRRELATIONSHIP_isArrangementOf: value = "isArrangementOf"; break;
        case FRBRRELATIONSHIP_hasComplement: value = "hasComplement"; break;
        case FRBRRELATIONSHIP_isComplementOf: value = "isComplementOf"; break;
        case FRBRRELATIONSHIP_hasEmbodiment: value = "hasEmbodiment"; break;
        case FRBRRELATIONSHIP_isEmbodimentOf: value = "isEmbodimentOf"; break;
        case FRBRRELATIONSHIP_hasExemplar: value = "hasExemplar"; break;
        case FRBRRELATIONSHIP_isExemplarOf: value = "isExemplarOf"; break;
        case FRBRRELATIONSHIP_hasImitation: value = "hasImitation"; break;
        case FRBRRELATIONSHIP_isImitationOf: value = "isImitationOf"; break;
        case FRBRRELATIONSHIP_hasPart: value = "hasPart"; break;
        case FRBRRELATIONSHIP_isPartOf: value = "isPartOf"; break;
        case FRBRRELATIONSHIP_hasRealization: value = "hasRealization"; break;
        case FRBRRELATIONSHIP_isRealizationOf: value = "isRealizationOf"; break;
        case FRBRRELATIONSHIP_hasReconfiguration: value = "hasReconfiguration"; break;
        case FRBRRELATIONSHIP_isReconfigurationOf: value = "isReconfigurationOf"; break;
        case FRBRRELATIONSHIP_hasReproduction: value = "hasReproduction"; break;
        case FRBRRELATIONSHIP_isReproductionOf: value = "isReproductionOf"; break;
        case FRBRRELATIONSHIP_hasRevision: value = "hasRevision"; break;
        case FRBRRELATIONSHIP_isRevisionOf: value = "isRevisionOf"; break;
        case FRBRRELATIONSHIP_hasSuccessor: value = "hasSuccessor"; break;
        case FRBRRELATIONSHIP_isSuccessorOf: value = "isSuccessorOf"; break;
        case FRBRRELATIONSHIP_hasSummarization: value = "hasSummarization"; break;
        case FRBRRELATIONSHIP_isSummarizationOf: value = "isSummarizationOf"; break;
        case FRBRRELATIONSHIP_hasSupplement: value = "hasSupplement"; break;
        case FRBRRELATIONSHIP_isSupplementOf: value = "isSupplementOf"; break;
        case FRBRRELATIONSHIP_hasTransformation: value = "hasTransformation"; break;
        case FRBRRELATIONSHIP_isTransformationOf: value = "isTransformationOf"; break;
        case FRBRRELATIONSHIP_hasTranslation: value = "hasTranslation"; break;
        case FRBRRELATIONSHIP_isTranslationOf: value = "isTranslationOf"; break;
        default:
            LogWarning("Unknown value '%d' for data.FRBRRELATIONSHIP", data);
            value = "";
            break;
    }
    return value;
}

data_FRBRRELATIONSHIP AttConverter::StrToFrbrrelationship(std::string value, bool logWarning) const
{
    if (value == "hasAbridgement") return FRBRRELATIONSHIP_hasAbridgement;
    if (value == "isAbridgementOf") return FRBRRELATIONSHIP_isAbridgementOf;
    if (value == "hasAdaptation") return FRBRRELATIONSHIP_hasAdaptation;
    if (value == "isAdaptationOf") return FRBRRELATIONSHIP_isAdaptationOf;
    if (value == "hasAlternate") return FRBRRELATIONSHIP_hasAlternate;
    if (value == "isAlternateOf") return FRBRRELATIONSHIP_isAlternateOf;
    if (value == "hasArrangement") return FRBRRELATIONSHIP_hasArrangement;
    if (value == "isArrangementOf") return FRBRRELATIONSHIP_isArrangementOf;
    if (value == "hasComplement") return FRBRRELATIONSHIP_hasComplement;
    if (value == "isComplementOf") return FRBRRELATIONSHIP_isComplementOf;
    if (value == "hasEmbodiment") return FRBRRELATIONSHIP_hasEmbodiment;
    if (value == "isEmbodimentOf") return FRBRRELATIONSHIP_isEmbodimentOf;
    if (value == "hasExemplar") return FRBRRELATIONSHIP_hasExemplar;
    if (value == "isExemplarOf") return FRBRRELATIONSHIP_isExemplarOf;
    if (value == "hasImitation") return FRBRRELATIONSHIP_hasImitation;
    if (value == "isImitationOf") return FRBRRELATIONSHIP_isImitationOf;
    if (value == "hasPart") return FRBRRELATIONSHIP_hasPart;
    if (value == "isPartOf") return FRBRRELATIONSHIP_isPartOf;
    if (value == "hasRealization") return FRBRRELATIONSHIP_hasRealization;
    if (value == "isRealizationOf") return FRBRRELATIONSHIP_isRealizationOf;
    if (value == "hasReconfiguration") return FRBRRELATIONSHIP_hasReconfiguration;
    if (value == "isReconfigurationOf") return FRBRRELATIONSHIP_isReconfigurationOf;
    if (value == "hasReproduction") return FRBRRELATIONSHIP_hasReproduction;
    if (value == "isReproductionOf") return FRBRRELATIONSHIP_isReproductionOf;
    if (value == "hasRevision") return FRBRRELATIONSHIP_hasRevision;
    if (value == "isRevisionOf") return FRBRRELATIONSHIP_isRevisionOf;
    if (value == "hasSuccessor") return FRBRRELATIONSHIP_hasSuccessor;
    if (value == "isSuccessorOf") return FRBRRELATIONSHIP_isSuccessorOf;
    if (value == "hasSummarization") return FRBRRELATIONSHIP_hasSummarization;
    if (value == "isSummarizationOf") return FRBRRELATIONSHIP_isSummarizationOf;
    if (value == "hasSupplement") return FRBRRELATIONSHIP_hasSupplement;
    if (value == "isSupplementOf") return FRBRRELATIONSHIP_isSupplementOf;
    if (value == "hasTransformation") return FRBRRELATIONSHIP_hasTransformation;
    if (value == "isTransformationOf") return FRBRRELATIONSHIP_isTransformationOf;
    if (value == "hasTranslation") return FRBRRELATIONSHIP_hasTranslation;
    if (value == "isTranslationOf") return FRBRRELATIONSHIP_isTranslationOf;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.FRBRRELATIONSHIP", value.c_str());
    return FRBRRELATIONSHIP_NONE;
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

data_GLISSANDO AttConverter::StrToGlissando(std::string value, bool logWarning) const
{
    if (value == "i") return GLISSANDO_i;
    if (value == "m") return GLISSANDO_m;
    if (value == "t") return GLISSANDO_t;
    if (logWarning && !value.empty())
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

data_GRACE AttConverter::StrToGrace(std::string value, bool logWarning) const
{
    if (value == "acc") return GRACE_acc;
    if (value == "unacc") return GRACE_unacc;
    if (value == "unknown") return GRACE_unknown;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.GRACE", value.c_str());
    return GRACE_NONE;
}

std::string AttConverter::HeadshapeToStr(data_HEADSHAPE data) const
{
    std::string value;
    switch (data) {
        case HEADSHAPE_quarter: value = "quarter"; break;
        case HEADSHAPE_half: value = "half"; break;
        case HEADSHAPE_whole: value = "whole"; break;
        case HEADSHAPE_backslash: value = "backslash"; break;
        case HEADSHAPE_circle: value = "circle"; break;
        case HEADSHAPE_plus: value = "+"; break;
        case HEADSHAPE_diamond: value = "diamond"; break;
        case HEADSHAPE_isotriangle: value = "isotriangle"; break;
        case HEADSHAPE_oval: value = "oval"; break;
        case HEADSHAPE_piewedge: value = "piewedge"; break;
        case HEADSHAPE_rectangle: value = "rectangle"; break;
        case HEADSHAPE_rtriangle: value = "rtriangle"; break;
        case HEADSHAPE_semicircle: value = "semicircle"; break;
        case HEADSHAPE_slash: value = "slash"; break;
        case HEADSHAPE_square: value = "square"; break;
        case HEADSHAPE_x: value = "x"; break;
        default:
            LogWarning("Unknown value '%d' for data.HEADSHAPE", data);
            value = "";
            break;
    }
    return value;
}

data_HEADSHAPE AttConverter::StrToHeadshape(std::string value, bool logWarning) const
{
    if (value == "quarter") return HEADSHAPE_quarter;
    if (value == "half") return HEADSHAPE_half;
    if (value == "whole") return HEADSHAPE_whole;
    if (value == "backslash") return HEADSHAPE_backslash;
    if (value == "circle") return HEADSHAPE_circle;
    if (value == "+") return HEADSHAPE_plus;
    if (value == "diamond") return HEADSHAPE_diamond;
    if (value == "isotriangle") return HEADSHAPE_isotriangle;
    if (value == "oval") return HEADSHAPE_oval;
    if (value == "piewedge") return HEADSHAPE_piewedge;
    if (value == "rectangle") return HEADSHAPE_rectangle;
    if (value == "rtriangle") return HEADSHAPE_rtriangle;
    if (value == "semicircle") return HEADSHAPE_semicircle;
    if (value == "slash") return HEADSHAPE_slash;
    if (value == "square") return HEADSHAPE_square;
    if (value == "x") return HEADSHAPE_x;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.HEADSHAPE", value.c_str());
    return HEADSHAPE_NONE;
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

data_HEADSHAPE_list AttConverter::StrToHeadshapeList(std::string value, bool logWarning) const
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
    if (logWarning && !value.empty())
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

data_HORIZONTALALIGNMENT AttConverter::StrToHorizontalalignment(std::string value, bool logWarning) const
{
    if (value == "left") return HORIZONTALALIGNMENT_left;
    if (value == "right") return HORIZONTALALIGNMENT_right;
    if (value == "center") return HORIZONTALALIGNMENT_center;
    if (value == "justify") return HORIZONTALALIGNMENT_justify;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.HORIZONTALALIGNMENT", value.c_str());
    return HORIZONTALALIGNMENT_NONE;
}

std::string AttConverter::LayerschemeToStr(data_LAYERSCHEME data) const
{
    std::string value;
    switch (data) {
        case LAYERSCHEME_1: value = "1"; break;
        case LAYERSCHEME_2o: value = "2o"; break;
        case LAYERSCHEME_2f: value = "2f"; break;
        case LAYERSCHEME_3o: value = "3o"; break;
        case LAYERSCHEME_3f: value = "3f"; break;
        default:
            LogWarning("Unknown value '%d' for data.LAYERSCHEME", data);
            value = "";
            break;
    }
    return value;
}

data_LAYERSCHEME AttConverter::StrToLayerscheme(std::string value, bool logWarning) const
{
    if (value == "1") return LAYERSCHEME_1;
    if (value == "2o") return LAYERSCHEME_2o;
    if (value == "2f") return LAYERSCHEME_2f;
    if (value == "3o") return LAYERSCHEME_3o;
    if (value == "3f") return LAYERSCHEME_3f;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.LAYERSCHEME", value.c_str());
    return LAYERSCHEME_NONE;
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

data_LIGATUREFORM AttConverter::StrToLigatureform(std::string value, bool logWarning) const
{
    if (value == "recta") return LIGATUREFORM_recta;
    if (value == "obliqua") return LIGATUREFORM_obliqua;
    if (logWarning && !value.empty())
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

data_LINEFORM AttConverter::StrToLineform(std::string value, bool logWarning) const
{
    if (value == "dashed") return LINEFORM_dashed;
    if (value == "dotted") return LINEFORM_dotted;
    if (value == "solid") return LINEFORM_solid;
    if (value == "wavy") return LINEFORM_wavy;
    if (logWarning && !value.empty())
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
        case LINESTARTENDSYMBOL_H: value = "H"; break;
        case LINESTARTENDSYMBOL_N: value = "N"; break;
        case LINESTARTENDSYMBOL_Th: value = "Th"; break;
        case LINESTARTENDSYMBOL_ThRetro: value = "ThRetro"; break;
        case LINESTARTENDSYMBOL_ThRetroInv: value = "ThRetroInv"; break;
        case LINESTARTENDSYMBOL_ThInv: value = "ThInv"; break;
        case LINESTARTENDSYMBOL_T: value = "T"; break;
        case LINESTARTENDSYMBOL_TInv: value = "TInv"; break;
        case LINESTARTENDSYMBOL_CH: value = "CH"; break;
        case LINESTARTENDSYMBOL_RH: value = "RH"; break;
        case LINESTARTENDSYMBOL_none: value = "none"; break;
        default:
            LogWarning("Unknown value '%d' for data.LINESTARTENDSYMBOL", data);
            value = "";
            break;
    }
    return value;
}

data_LINESTARTENDSYMBOL AttConverter::StrToLinestartendsymbol(std::string value, bool logWarning) const
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
    if (value == "H") return LINESTARTENDSYMBOL_H;
    if (value == "N") return LINESTARTENDSYMBOL_N;
    if (value == "Th") return LINESTARTENDSYMBOL_Th;
    if (value == "ThRetro") return LINESTARTENDSYMBOL_ThRetro;
    if (value == "ThRetroInv") return LINESTARTENDSYMBOL_ThRetroInv;
    if (value == "ThInv") return LINESTARTENDSYMBOL_ThInv;
    if (value == "T") return LINESTARTENDSYMBOL_T;
    if (value == "TInv") return LINESTARTENDSYMBOL_TInv;
    if (value == "CH") return LINESTARTENDSYMBOL_CH;
    if (value == "RH") return LINESTARTENDSYMBOL_RH;
    if (value == "none") return LINESTARTENDSYMBOL_none;
    if (logWarning && !value.empty())
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

data_LINEWIDTHTERM AttConverter::StrToLinewidthterm(std::string value, bool logWarning) const
{
    if (value == "narrow") return LINEWIDTHTERM_narrow;
    if (value == "medium") return LINEWIDTHTERM_medium;
    if (value == "wide") return LINEWIDTHTERM_wide;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.LINEWIDTHTERM", value.c_str());
    return LINEWIDTHTERM_NONE;
}

std::string AttConverter::MelodicfunctionToStr(data_MELODICFUNCTION data) const
{
    std::string value;
    switch (data) {
        case MELODICFUNCTION_aln: value = "aln"; break;
        case MELODICFUNCTION_ant: value = "ant"; break;
        case MELODICFUNCTION_app: value = "app"; break;
        case MELODICFUNCTION_apt: value = "apt"; break;
        case MELODICFUNCTION_arp: value = "arp"; break;
        case MELODICFUNCTION_arp7: value = "arp7"; break;
        case MELODICFUNCTION_aun: value = "aun"; break;
        case MELODICFUNCTION_chg: value = "chg"; break;
        case MELODICFUNCTION_cln: value = "cln"; break;
        case MELODICFUNCTION_ct: value = "ct"; break;
        case MELODICFUNCTION_ct7: value = "ct7"; break;
        case MELODICFUNCTION_cun: value = "cun"; break;
        case MELODICFUNCTION_cup: value = "cup"; break;
        case MELODICFUNCTION_et: value = "et"; break;
        case MELODICFUNCTION_ln: value = "ln"; break;
        case MELODICFUNCTION_ped: value = "ped"; break;
        case MELODICFUNCTION_rep: value = "rep"; break;
        case MELODICFUNCTION_ret: value = "ret"; break;
        case MELODICFUNCTION_23ret: value = "23ret"; break;
        case MELODICFUNCTION_78ret: value = "78ret"; break;
        case MELODICFUNCTION_sus: value = "sus"; break;
        case MELODICFUNCTION_43sus: value = "43sus"; break;
        case MELODICFUNCTION_98sus: value = "98sus"; break;
        case MELODICFUNCTION_76sus: value = "76sus"; break;
        case MELODICFUNCTION_un: value = "un"; break;
        case MELODICFUNCTION_un7: value = "un7"; break;
        case MELODICFUNCTION_upt: value = "upt"; break;
        case MELODICFUNCTION_upt7: value = "upt7"; break;
        default:
            LogWarning("Unknown value '%d' for data.MELODICFUNCTION", data);
            value = "";
            break;
    }
    return value;
}

data_MELODICFUNCTION AttConverter::StrToMelodicfunction(std::string value, bool logWarning) const
{
    if (value == "aln") return MELODICFUNCTION_aln;
    if (value == "ant") return MELODICFUNCTION_ant;
    if (value == "app") return MELODICFUNCTION_app;
    if (value == "apt") return MELODICFUNCTION_apt;
    if (value == "arp") return MELODICFUNCTION_arp;
    if (value == "arp7") return MELODICFUNCTION_arp7;
    if (value == "aun") return MELODICFUNCTION_aun;
    if (value == "chg") return MELODICFUNCTION_chg;
    if (value == "cln") return MELODICFUNCTION_cln;
    if (value == "ct") return MELODICFUNCTION_ct;
    if (value == "ct7") return MELODICFUNCTION_ct7;
    if (value == "cun") return MELODICFUNCTION_cun;
    if (value == "cup") return MELODICFUNCTION_cup;
    if (value == "et") return MELODICFUNCTION_et;
    if (value == "ln") return MELODICFUNCTION_ln;
    if (value == "ped") return MELODICFUNCTION_ped;
    if (value == "rep") return MELODICFUNCTION_rep;
    if (value == "ret") return MELODICFUNCTION_ret;
    if (value == "23ret") return MELODICFUNCTION_23ret;
    if (value == "78ret") return MELODICFUNCTION_78ret;
    if (value == "sus") return MELODICFUNCTION_sus;
    if (value == "43sus") return MELODICFUNCTION_43sus;
    if (value == "98sus") return MELODICFUNCTION_98sus;
    if (value == "76sus") return MELODICFUNCTION_76sus;
    if (value == "un") return MELODICFUNCTION_un;
    if (value == "un7") return MELODICFUNCTION_un7;
    if (value == "upt") return MELODICFUNCTION_upt;
    if (value == "upt7") return MELODICFUNCTION_upt7;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.MELODICFUNCTION", value.c_str());
    return MELODICFUNCTION_NONE;
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

data_MENSURATIONSIGN AttConverter::StrToMensurationsign(std::string value, bool logWarning) const
{
    if (value == "C") return MENSURATIONSIGN_C;
    if (value == "O") return MENSURATIONSIGN_O;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.MENSURATIONSIGN", value.c_str());
    return MENSURATIONSIGN_NONE;
}

std::string AttConverter::MeterformToStr(data_METERFORM data) const
{
    std::string value;
    switch (data) {
        case METERFORM_num: value = "num"; break;
        case METERFORM_denomsym: value = "denomsym"; break;
        case METERFORM_norm: value = "norm"; break;
        case METERFORM_invis: value = "invis"; break;
        default:
            LogWarning("Unknown value '%d' for data.METERFORM", data);
            value = "";
            break;
    }
    return value;
}

data_METERFORM AttConverter::StrToMeterform(std::string value, bool logWarning) const
{
    if (value == "num") return METERFORM_num;
    if (value == "denomsym") return METERFORM_denomsym;
    if (value == "norm") return METERFORM_norm;
    if (value == "invis") return METERFORM_invis;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.METERFORM", value.c_str());
    return METERFORM_NONE;
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

data_METERSIGN AttConverter::StrToMetersign(std::string value, bool logWarning) const
{
    if (value == "common") return METERSIGN_common;
    if (value == "cut") return METERSIGN_cut;
    if (logWarning && !value.empty())
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

data_MIDINAMES AttConverter::StrToMidinames(std::string value, bool logWarning) const
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
    if (logWarning && !value.empty())
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

data_MODE AttConverter::StrToMode(std::string value, bool logWarning) const
{
    if (value == "major") return MODE_major;
    if (value == "minor") return MODE_minor;
    if (value == "dorian") return MODE_dorian;
    if (value == "phrygian") return MODE_phrygian;
    if (value == "lydian") return MODE_lydian;
    if (value == "mixolydian") return MODE_mixolydian;
    if (value == "aeolian") return MODE_aeolian;
    if (value == "locrian") return MODE_locrian;
    if (logWarning && !value.empty())
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

data_MODSRELATIONSHIP AttConverter::StrToModsrelationship(std::string value, bool logWarning) const
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
    if (logWarning && !value.empty())
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

data_NONSTAFFPLACE AttConverter::StrToNonstaffplace(std::string value, bool logWarning) const
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
    if (logWarning && !value.empty())
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

data_NOTATIONTYPE AttConverter::StrToNotationtype(std::string value, bool logWarning) const
{
    if (value == "cmn") return NOTATIONTYPE_cmn;
    if (value == "mensural") return NOTATIONTYPE_mensural;
    if (value == "mensural.black") return NOTATIONTYPE_mensural_black;
    if (value == "mensural.white") return NOTATIONTYPE_mensural_white;
    if (value == "neume") return NOTATIONTYPE_neume;
    if (value == "tab") return NOTATIONTYPE_tab;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.NOTATIONTYPE", value.c_str());
    return NOTATIONTYPE_NONE;
}

std::string AttConverter::NoteheadmodifierToStr(data_NOTEHEADMODIFIER data) const
{
    std::string value;
    switch (data) {
        case NOTEHEADMODIFIER_slash: value = "slash"; break;
        case NOTEHEADMODIFIER_backslash: value = "backslash"; break;
        case NOTEHEADMODIFIER_vline: value = "vline"; break;
        case NOTEHEADMODIFIER_hline: value = "hline"; break;
        case NOTEHEADMODIFIER_centerdot: value = "centerdot"; break;
        case NOTEHEADMODIFIER_paren: value = "paren"; break;
        case NOTEHEADMODIFIER_brack: value = "brack"; break;
        case NOTEHEADMODIFIER_box: value = "box"; break;
        case NOTEHEADMODIFIER_circle: value = "circle"; break;
        case NOTEHEADMODIFIER_dblwhole: value = "dblwhole"; break;
        default:
            LogWarning("Unknown value '%d' for data.NOTEHEADMODIFIER", data);
            value = "";
            break;
    }
    return value;
}

data_NOTEHEADMODIFIER AttConverter::StrToNoteheadmodifier(std::string value, bool logWarning) const
{
    if (value == "slash") return NOTEHEADMODIFIER_slash;
    if (value == "backslash") return NOTEHEADMODIFIER_backslash;
    if (value == "vline") return NOTEHEADMODIFIER_vline;
    if (value == "hline") return NOTEHEADMODIFIER_hline;
    if (value == "centerdot") return NOTEHEADMODIFIER_centerdot;
    if (value == "paren") return NOTEHEADMODIFIER_paren;
    if (value == "brack") return NOTEHEADMODIFIER_brack;
    if (value == "box") return NOTEHEADMODIFIER_box;
    if (value == "circle") return NOTEHEADMODIFIER_circle;
    if (value == "dblwhole") return NOTEHEADMODIFIER_dblwhole;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.NOTEHEADMODIFIER", value.c_str());
    return NOTEHEADMODIFIER_NONE;
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

data_NOTEHEADMODIFIER_list AttConverter::StrToNoteheadmodifierList(std::string value, bool logWarning) const
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
    if (logWarning && !value.empty())
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

data_OTHERSTAFF AttConverter::StrToOtherstaff(std::string value, bool logWarning) const
{
    if (value == "above") return OTHERSTAFF_above;
    if (value == "below") return OTHERSTAFF_below;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.OTHERSTAFF", value.c_str());
    return OTHERSTAFF_NONE;
}

std::string AttConverter::RelationshipToStr(data_RELATIONSHIP data) const
{
    std::string value;
    switch (data) {
        case RELATIONSHIP_hasAbridgement: value = "hasAbridgement"; break;
        case RELATIONSHIP_isAbridgementOf: value = "isAbridgementOf"; break;
        case RELATIONSHIP_hasAdaptation: value = "hasAdaptation"; break;
        case RELATIONSHIP_isAdaptationOf: value = "isAdaptationOf"; break;
        case RELATIONSHIP_hasAlternate: value = "hasAlternate"; break;
        case RELATIONSHIP_isAlternateOf: value = "isAlternateOf"; break;
        case RELATIONSHIP_hasArrangement: value = "hasArrangement"; break;
        case RELATIONSHIP_isArrangementOf: value = "isArrangementOf"; break;
        case RELATIONSHIP_hasComplement: value = "hasComplement"; break;
        case RELATIONSHIP_isComplementOf: value = "isComplementOf"; break;
        case RELATIONSHIP_hasEmbodiment: value = "hasEmbodiment"; break;
        case RELATIONSHIP_isEmbodimentOf: value = "isEmbodimentOf"; break;
        case RELATIONSHIP_hasExemplar: value = "hasExemplar"; break;
        case RELATIONSHIP_isExemplarOf: value = "isExemplarOf"; break;
        case RELATIONSHIP_hasImitation: value = "hasImitation"; break;
        case RELATIONSHIP_isImitationOf: value = "isImitationOf"; break;
        case RELATIONSHIP_hasPart: value = "hasPart"; break;
        case RELATIONSHIP_isPartOf: value = "isPartOf"; break;
        case RELATIONSHIP_hasRealization: value = "hasRealization"; break;
        case RELATIONSHIP_isRealizationOf: value = "isRealizationOf"; break;
        case RELATIONSHIP_hasReconfiguration: value = "hasReconfiguration"; break;
        case RELATIONSHIP_isReconfigurationOf: value = "isReconfigurationOf"; break;
        case RELATIONSHIP_hasReproduction: value = "hasReproduction"; break;
        case RELATIONSHIP_isReproductionOf: value = "isReproductionOf"; break;
        case RELATIONSHIP_hasRevision: value = "hasRevision"; break;
        case RELATIONSHIP_isRevisionOf: value = "isRevisionOf"; break;
        case RELATIONSHIP_hasSuccessor: value = "hasSuccessor"; break;
        case RELATIONSHIP_isSuccessorOf: value = "isSuccessorOf"; break;
        case RELATIONSHIP_hasSummarization: value = "hasSummarization"; break;
        case RELATIONSHIP_isSummarizationOf: value = "isSummarizationOf"; break;
        case RELATIONSHIP_hasSupplement: value = "hasSupplement"; break;
        case RELATIONSHIP_isSupplementOf: value = "isSupplementOf"; break;
        case RELATIONSHIP_hasTransformation: value = "hasTransformation"; break;
        case RELATIONSHIP_isTransformationOf: value = "isTransformationOf"; break;
        case RELATIONSHIP_hasTranslation: value = "hasTranslation"; break;
        case RELATIONSHIP_isTranslationOf: value = "isTranslationOf"; break;
        case RELATIONSHIP_preceding: value = "preceding"; break;
        case RELATIONSHIP_succeeding: value = "succeeding"; break;
        case RELATIONSHIP_original: value = "original"; break;
        case RELATIONSHIP_host: value = "host"; break;
        case RELATIONSHIP_constituent: value = "constituent"; break;
        case RELATIONSHIP_otherVersion: value = "otherVersion"; break;
        case RELATIONSHIP_otherFormat: value = "otherFormat"; break;
        case RELATIONSHIP_isReferencedBy: value = "isReferencedBy"; break;
        case RELATIONSHIP_references: value = "references"; break;
        default:
            LogWarning("Unknown value '%d' for data.RELATIONSHIP", data);
            value = "";
            break;
    }
    return value;
}

data_RELATIONSHIP AttConverter::StrToRelationship(std::string value, bool logWarning) const
{
    if (value == "hasAbridgement") return RELATIONSHIP_hasAbridgement;
    if (value == "isAbridgementOf") return RELATIONSHIP_isAbridgementOf;
    if (value == "hasAdaptation") return RELATIONSHIP_hasAdaptation;
    if (value == "isAdaptationOf") return RELATIONSHIP_isAdaptationOf;
    if (value == "hasAlternate") return RELATIONSHIP_hasAlternate;
    if (value == "isAlternateOf") return RELATIONSHIP_isAlternateOf;
    if (value == "hasArrangement") return RELATIONSHIP_hasArrangement;
    if (value == "isArrangementOf") return RELATIONSHIP_isArrangementOf;
    if (value == "hasComplement") return RELATIONSHIP_hasComplement;
    if (value == "isComplementOf") return RELATIONSHIP_isComplementOf;
    if (value == "hasEmbodiment") return RELATIONSHIP_hasEmbodiment;
    if (value == "isEmbodimentOf") return RELATIONSHIP_isEmbodimentOf;
    if (value == "hasExemplar") return RELATIONSHIP_hasExemplar;
    if (value == "isExemplarOf") return RELATIONSHIP_isExemplarOf;
    if (value == "hasImitation") return RELATIONSHIP_hasImitation;
    if (value == "isImitationOf") return RELATIONSHIP_isImitationOf;
    if (value == "hasPart") return RELATIONSHIP_hasPart;
    if (value == "isPartOf") return RELATIONSHIP_isPartOf;
    if (value == "hasRealization") return RELATIONSHIP_hasRealization;
    if (value == "isRealizationOf") return RELATIONSHIP_isRealizationOf;
    if (value == "hasReconfiguration") return RELATIONSHIP_hasReconfiguration;
    if (value == "isReconfigurationOf") return RELATIONSHIP_isReconfigurationOf;
    if (value == "hasReproduction") return RELATIONSHIP_hasReproduction;
    if (value == "isReproductionOf") return RELATIONSHIP_isReproductionOf;
    if (value == "hasRevision") return RELATIONSHIP_hasRevision;
    if (value == "isRevisionOf") return RELATIONSHIP_isRevisionOf;
    if (value == "hasSuccessor") return RELATIONSHIP_hasSuccessor;
    if (value == "isSuccessorOf") return RELATIONSHIP_isSuccessorOf;
    if (value == "hasSummarization") return RELATIONSHIP_hasSummarization;
    if (value == "isSummarizationOf") return RELATIONSHIP_isSummarizationOf;
    if (value == "hasSupplement") return RELATIONSHIP_hasSupplement;
    if (value == "isSupplementOf") return RELATIONSHIP_isSupplementOf;
    if (value == "hasTransformation") return RELATIONSHIP_hasTransformation;
    if (value == "isTransformationOf") return RELATIONSHIP_isTransformationOf;
    if (value == "hasTranslation") return RELATIONSHIP_hasTranslation;
    if (value == "isTranslationOf") return RELATIONSHIP_isTranslationOf;
    if (value == "preceding") return RELATIONSHIP_preceding;
    if (value == "succeeding") return RELATIONSHIP_succeeding;
    if (value == "original") return RELATIONSHIP_original;
    if (value == "host") return RELATIONSHIP_host;
    if (value == "constituent") return RELATIONSHIP_constituent;
    if (value == "otherVersion") return RELATIONSHIP_otherVersion;
    if (value == "otherFormat") return RELATIONSHIP_otherFormat;
    if (value == "isReferencedBy") return RELATIONSHIP_isReferencedBy;
    if (value == "references") return RELATIONSHIP_references;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.RELATIONSHIP", value.c_str());
    return RELATIONSHIP_NONE;
}

std::string AttConverter::RotationToStr(data_ROTATION data) const
{
    std::string value;
    switch (data) {
        case ROTATION_none: value = "none"; break;
        case ROTATION_down: value = "down"; break;
        case ROTATION_left: value = "left"; break;
        case ROTATION_ne: value = "ne"; break;
        case ROTATION_nw: value = "nw"; break;
        case ROTATION_se: value = "se"; break;
        case ROTATION_sw: value = "sw"; break;
        default:
            LogWarning("Unknown value '%d' for data.ROTATION", data);
            value = "";
            break;
    }
    return value;
}

data_ROTATION AttConverter::StrToRotation(std::string value, bool logWarning) const
{
    if (value == "none") return ROTATION_none;
    if (value == "down") return ROTATION_down;
    if (value == "left") return ROTATION_left;
    if (value == "ne") return ROTATION_ne;
    if (value == "nw") return ROTATION_nw;
    if (value == "se") return ROTATION_se;
    if (value == "sw") return ROTATION_sw;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.ROTATION", value.c_str());
    return ROTATION_NONE;
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

data_ROTATIONDIRECTION AttConverter::StrToRotationdirection(std::string value, bool logWarning) const
{
    if (value == "none") return ROTATIONDIRECTION_none;
    if (value == "down") return ROTATIONDIRECTION_down;
    if (value == "left") return ROTATIONDIRECTION_left;
    if (value == "ne") return ROTATIONDIRECTION_ne;
    if (value == "nw") return ROTATIONDIRECTION_nw;
    if (value == "se") return ROTATIONDIRECTION_se;
    if (value == "sw") return ROTATIONDIRECTION_sw;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.ROTATIONDIRECTION", value.c_str());
    return ROTATIONDIRECTION_NONE;
}

std::string AttConverter::StaffitemToStr(data_STAFFITEM data) const
{
    std::string value;
    switch (data) {
        case STAFFITEM_accid: value = "accid"; break;
        case STAFFITEM_annot: value = "annot"; break;
        case STAFFITEM_artic: value = "artic"; break;
        case STAFFITEM_dir: value = "dir"; break;
        case STAFFITEM_dynam: value = "dynam"; break;
        case STAFFITEM_harm: value = "harm"; break;
        case STAFFITEM_ornam: value = "ornam"; break;
        case STAFFITEM_sp: value = "sp"; break;
        case STAFFITEM_stageDir: value = "stageDir"; break;
        case STAFFITEM_tempo: value = "tempo"; break;
        case STAFFITEM_beam: value = "beam"; break;
        case STAFFITEM_bend: value = "bend"; break;
        case STAFFITEM_bracketSpan: value = "bracketSpan"; break;
        case STAFFITEM_breath: value = "breath"; break;
        case STAFFITEM_cpMark: value = "cpMark"; break;
        case STAFFITEM_fermata: value = "fermata"; break;
        case STAFFITEM_fing: value = "fing"; break;
        case STAFFITEM_hairpin: value = "hairpin"; break;
        case STAFFITEM_harpPedal: value = "harpPedal"; break;
        case STAFFITEM_lv: value = "lv"; break;
        case STAFFITEM_mordent: value = "mordent"; break;
        case STAFFITEM_octave: value = "octave"; break;
        case STAFFITEM_pedal: value = "pedal"; break;
        case STAFFITEM_reh: value = "reh"; break;
        case STAFFITEM_tie: value = "tie"; break;
        case STAFFITEM_trill: value = "trill"; break;
        case STAFFITEM_tuplet: value = "tuplet"; break;
        case STAFFITEM_turn: value = "turn"; break;
        case STAFFITEM_ligature: value = "ligature"; break;
        default:
            LogWarning("Unknown value '%d' for data.STAFFITEM", data);
            value = "";
            break;
    }
    return value;
}

data_STAFFITEM AttConverter::StrToStaffitem(std::string value, bool logWarning) const
{
    if (value == "accid") return STAFFITEM_accid;
    if (value == "annot") return STAFFITEM_annot;
    if (value == "artic") return STAFFITEM_artic;
    if (value == "dir") return STAFFITEM_dir;
    if (value == "dynam") return STAFFITEM_dynam;
    if (value == "harm") return STAFFITEM_harm;
    if (value == "ornam") return STAFFITEM_ornam;
    if (value == "sp") return STAFFITEM_sp;
    if (value == "stageDir") return STAFFITEM_stageDir;
    if (value == "tempo") return STAFFITEM_tempo;
    if (value == "beam") return STAFFITEM_beam;
    if (value == "bend") return STAFFITEM_bend;
    if (value == "bracketSpan") return STAFFITEM_bracketSpan;
    if (value == "breath") return STAFFITEM_breath;
    if (value == "cpMark") return STAFFITEM_cpMark;
    if (value == "fermata") return STAFFITEM_fermata;
    if (value == "fing") return STAFFITEM_fing;
    if (value == "hairpin") return STAFFITEM_hairpin;
    if (value == "harpPedal") return STAFFITEM_harpPedal;
    if (value == "lv") return STAFFITEM_lv;
    if (value == "mordent") return STAFFITEM_mordent;
    if (value == "octave") return STAFFITEM_octave;
    if (value == "pedal") return STAFFITEM_pedal;
    if (value == "reh") return STAFFITEM_reh;
    if (value == "tie") return STAFFITEM_tie;
    if (value == "trill") return STAFFITEM_trill;
    if (value == "tuplet") return STAFFITEM_tuplet;
    if (value == "turn") return STAFFITEM_turn;
    if (value == "ligature") return STAFFITEM_ligature;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.STAFFITEM", value.c_str());
    return STAFFITEM_NONE;
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

data_STAFFITEM_basic AttConverter::StrToStaffitemBasic(std::string value, bool logWarning) const
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
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.STAFFITEM.basic", value.c_str());
    return STAFFITEM_basic_NONE;
}

std::string AttConverter::StaffitemCmnToStr(data_STAFFITEM_cmn data) const
{
    std::string value;
    switch (data) {
        case STAFFITEM_cmn_beam: value = "beam"; break;
        case STAFFITEM_cmn_bend: value = "bend"; break;
        case STAFFITEM_cmn_bracketSpan: value = "bracketSpan"; break;
        case STAFFITEM_cmn_breath: value = "breath"; break;
        case STAFFITEM_cmn_cpMark: value = "cpMark"; break;
        case STAFFITEM_cmn_fermata: value = "fermata"; break;
        case STAFFITEM_cmn_fing: value = "fing"; break;
        case STAFFITEM_cmn_hairpin: value = "hairpin"; break;
        case STAFFITEM_cmn_harpPedal: value = "harpPedal"; break;
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

data_STAFFITEM_cmn AttConverter::StrToStaffitemCmn(std::string value, bool logWarning) const
{
    if (value == "beam") return STAFFITEM_cmn_beam;
    if (value == "bend") return STAFFITEM_cmn_bend;
    if (value == "bracketSpan") return STAFFITEM_cmn_bracketSpan;
    if (value == "breath") return STAFFITEM_cmn_breath;
    if (value == "cpMark") return STAFFITEM_cmn_cpMark;
    if (value == "fermata") return STAFFITEM_cmn_fermata;
    if (value == "fing") return STAFFITEM_cmn_fing;
    if (value == "hairpin") return STAFFITEM_cmn_hairpin;
    if (value == "harpPedal") return STAFFITEM_cmn_harpPedal;
    if (value == "lv") return STAFFITEM_cmn_lv;
    if (value == "mordent") return STAFFITEM_cmn_mordent;
    if (value == "octave") return STAFFITEM_cmn_octave;
    if (value == "pedal") return STAFFITEM_cmn_pedal;
    if (value == "reh") return STAFFITEM_cmn_reh;
    if (value == "tie") return STAFFITEM_cmn_tie;
    if (value == "trill") return STAFFITEM_cmn_trill;
    if (value == "tuplet") return STAFFITEM_cmn_tuplet;
    if (value == "turn") return STAFFITEM_cmn_turn;
    if (logWarning && !value.empty())
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

data_STAFFITEM_mensural AttConverter::StrToStaffitemMensural(std::string value, bool logWarning) const
{
    if (value == "ligature") return STAFFITEM_mensural_ligature;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.STAFFITEM.mensural", value.c_str());
    return STAFFITEM_mensural_NONE;
}

std::string AttConverter::StaffrelToStr(data_STAFFREL data) const
{
    std::string value;
    switch (data) {
        case STAFFREL_above: value = "above"; break;
        case STAFFREL_below: value = "below"; break;
        case STAFFREL_between: value = "between"; break;
        case STAFFREL_within: value = "within"; break;
        default:
            LogWarning("Unknown value '%d' for data.STAFFREL", data);
            value = "";
            break;
    }
    return value;
}

data_STAFFREL AttConverter::StrToStaffrel(std::string value, bool logWarning) const
{
    if (value == "above") return STAFFREL_above;
    if (value == "below") return STAFFREL_below;
    if (value == "between") return STAFFREL_between;
    if (value == "within") return STAFFREL_within;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.STAFFREL", value.c_str());
    return STAFFREL_NONE;
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

data_STAFFREL_basic AttConverter::StrToStaffrelBasic(std::string value, bool logWarning) const
{
    if (value == "above") return STAFFREL_basic_above;
    if (value == "below") return STAFFREL_basic_below;
    if (logWarning && !value.empty())
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

data_STAFFREL_extended AttConverter::StrToStaffrelExtended(std::string value, bool logWarning) const
{
    if (value == "between") return STAFFREL_extended_between;
    if (value == "within") return STAFFREL_extended_within;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.STAFFREL.extended", value.c_str());
    return STAFFREL_extended_NONE;
}

std::string AttConverter::StemdirectionToStr(data_STEMDIRECTION data) const
{
    std::string value;
    switch (data) {
        case STEMDIRECTION_up: value = "up"; break;
        case STEMDIRECTION_down: value = "down"; break;
        case STEMDIRECTION_left: value = "left"; break;
        case STEMDIRECTION_right: value = "right"; break;
        case STEMDIRECTION_ne: value = "ne"; break;
        case STEMDIRECTION_se: value = "se"; break;
        case STEMDIRECTION_nw: value = "nw"; break;
        case STEMDIRECTION_sw: value = "sw"; break;
        default:
            LogWarning("Unknown value '%d' for data.STEMDIRECTION", data);
            value = "";
            break;
    }
    return value;
}

data_STEMDIRECTION AttConverter::StrToStemdirection(std::string value, bool logWarning) const
{
    if (value == "up") return STEMDIRECTION_up;
    if (value == "down") return STEMDIRECTION_down;
    if (value == "left") return STEMDIRECTION_left;
    if (value == "right") return STEMDIRECTION_right;
    if (value == "ne") return STEMDIRECTION_ne;
    if (value == "se") return STEMDIRECTION_se;
    if (value == "nw") return STEMDIRECTION_nw;
    if (value == "sw") return STEMDIRECTION_sw;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.STEMDIRECTION", value.c_str());
    return STEMDIRECTION_NONE;
}

std::string AttConverter::StemdirectionBasicToStr(data_STEMDIRECTION_basic data) const
{
    std::string value;
    switch (data) {
        case STEMDIRECTION_basic_up: value = "up"; break;
        case STEMDIRECTION_basic_down: value = "down"; break;
        default:
            LogWarning("Unknown value '%d' for data.STEMDIRECTION.basic", data);
            value = "";
            break;
    }
    return value;
}

data_STEMDIRECTION_basic AttConverter::StrToStemdirectionBasic(std::string value, bool logWarning) const
{
    if (value == "up") return STEMDIRECTION_basic_up;
    if (value == "down") return STEMDIRECTION_basic_down;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.STEMDIRECTION.basic", value.c_str());
    return STEMDIRECTION_basic_NONE;
}

std::string AttConverter::StemdirectionExtendedToStr(data_STEMDIRECTION_extended data) const
{
    std::string value;
    switch (data) {
        case STEMDIRECTION_extended_left: value = "left"; break;
        case STEMDIRECTION_extended_right: value = "right"; break;
        case STEMDIRECTION_extended_ne: value = "ne"; break;
        case STEMDIRECTION_extended_se: value = "se"; break;
        case STEMDIRECTION_extended_nw: value = "nw"; break;
        case STEMDIRECTION_extended_sw: value = "sw"; break;
        default:
            LogWarning("Unknown value '%d' for data.STEMDIRECTION.extended", data);
            value = "";
            break;
    }
    return value;
}

data_STEMDIRECTION_extended AttConverter::StrToStemdirectionExtended(std::string value, bool logWarning) const
{
    if (value == "left") return STEMDIRECTION_extended_left;
    if (value == "right") return STEMDIRECTION_extended_right;
    if (value == "ne") return STEMDIRECTION_extended_ne;
    if (value == "se") return STEMDIRECTION_extended_se;
    if (value == "nw") return STEMDIRECTION_extended_nw;
    if (value == "sw") return STEMDIRECTION_extended_sw;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.STEMDIRECTION.extended", value.c_str());
    return STEMDIRECTION_extended_NONE;
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

data_STEMMODIFIER AttConverter::StrToStemmodifier(std::string value, bool logWarning) const
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
    if (logWarning && !value.empty())
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

data_STEMPOSITION AttConverter::StrToStemposition(std::string value, bool logWarning) const
{
    if (value == "left") return STEMPOSITION_left;
    if (value == "right") return STEMPOSITION_right;
    if (value == "center") return STEMPOSITION_center;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.STEMPOSITION", value.c_str());
    return STEMPOSITION_NONE;
}

std::string AttConverter::TemperamentToStr(data_TEMPERAMENT data) const
{
    std::string value;
    switch (data) {
        case TEMPERAMENT_equal: value = "equal"; break;
        case TEMPERAMENT_just: value = "just"; break;
        case TEMPERAMENT_mean: value = "mean"; break;
        case TEMPERAMENT_pythagorean: value = "pythagorean"; break;
        default:
            LogWarning("Unknown value '%d' for data.TEMPERAMENT", data);
            value = "";
            break;
    }
    return value;
}

data_TEMPERAMENT AttConverter::StrToTemperament(std::string value, bool logWarning) const
{
    if (value == "equal") return TEMPERAMENT_equal;
    if (value == "just") return TEMPERAMENT_just;
    if (value == "mean") return TEMPERAMENT_mean;
    if (value == "pythagorean") return TEMPERAMENT_pythagorean;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.TEMPERAMENT", value.c_str());
    return TEMPERAMENT_NONE;
}

std::string AttConverter::TextrenditionToStr(data_TEXTRENDITION data) const
{
    std::string value;
    switch (data) {
        case TEXTRENDITION_quote: value = "quote"; break;
        case TEXTRENDITION_quotedbl: value = "quotedbl"; break;
        case TEXTRENDITION_italic: value = "italic"; break;
        case TEXTRENDITION_oblique: value = "oblique"; break;
        case TEXTRENDITION_smcaps: value = "smcaps"; break;
        case TEXTRENDITION_bold: value = "bold"; break;
        case TEXTRENDITION_bolder: value = "bolder"; break;
        case TEXTRENDITION_lighter: value = "lighter"; break;
        case TEXTRENDITION_box: value = "box"; break;
        case TEXTRENDITION_circle: value = "circle"; break;
        case TEXTRENDITION_dbox: value = "dbox"; break;
        case TEXTRENDITION_tbox: value = "tbox"; break;
        case TEXTRENDITION_bslash: value = "bslash"; break;
        case TEXTRENDITION_fslash: value = "fslash"; break;
        case TEXTRENDITION_line_through: value = "line-through"; break;
        case TEXTRENDITION_none: value = "none"; break;
        case TEXTRENDITION_overline: value = "overline"; break;
        case TEXTRENDITION_overstrike: value = "overstrike"; break;
        case TEXTRENDITION_strike: value = "strike"; break;
        case TEXTRENDITION_sub: value = "sub"; break;
        case TEXTRENDITION_sup: value = "sup"; break;
        case TEXTRENDITION_superimpose: value = "superimpose"; break;
        case TEXTRENDITION_underline: value = "underline"; break;
        case TEXTRENDITION_x_through: value = "x-through"; break;
        case TEXTRENDITION_ltr: value = "ltr"; break;
        case TEXTRENDITION_rtl: value = "rtl"; break;
        case TEXTRENDITION_lro: value = "lro"; break;
        case TEXTRENDITION_rlo: value = "rlo"; break;
        default:
            LogWarning("Unknown value '%d' for data.TEXTRENDITION", data);
            value = "";
            break;
    }
    return value;
}

data_TEXTRENDITION AttConverter::StrToTextrendition(std::string value, bool logWarning) const
{
    if (value == "quote") return TEXTRENDITION_quote;
    if (value == "quotedbl") return TEXTRENDITION_quotedbl;
    if (value == "italic") return TEXTRENDITION_italic;
    if (value == "oblique") return TEXTRENDITION_oblique;
    if (value == "smcaps") return TEXTRENDITION_smcaps;
    if (value == "bold") return TEXTRENDITION_bold;
    if (value == "bolder") return TEXTRENDITION_bolder;
    if (value == "lighter") return TEXTRENDITION_lighter;
    if (value == "box") return TEXTRENDITION_box;
    if (value == "circle") return TEXTRENDITION_circle;
    if (value == "dbox") return TEXTRENDITION_dbox;
    if (value == "tbox") return TEXTRENDITION_tbox;
    if (value == "bslash") return TEXTRENDITION_bslash;
    if (value == "fslash") return TEXTRENDITION_fslash;
    if (value == "line-through") return TEXTRENDITION_line_through;
    if (value == "none") return TEXTRENDITION_none;
    if (value == "overline") return TEXTRENDITION_overline;
    if (value == "overstrike") return TEXTRENDITION_overstrike;
    if (value == "strike") return TEXTRENDITION_strike;
    if (value == "sub") return TEXTRENDITION_sub;
    if (value == "sup") return TEXTRENDITION_sup;
    if (value == "superimpose") return TEXTRENDITION_superimpose;
    if (value == "underline") return TEXTRENDITION_underline;
    if (value == "x-through") return TEXTRENDITION_x_through;
    if (value == "ltr") return TEXTRENDITION_ltr;
    if (value == "rtl") return TEXTRENDITION_rtl;
    if (value == "lro") return TEXTRENDITION_lro;
    if (value == "rlo") return TEXTRENDITION_rlo;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.TEXTRENDITION", value.c_str());
    return TEXTRENDITION_NONE;
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

data_TEXTRENDITIONLIST AttConverter::StrToTextrenditionlist(std::string value, bool logWarning) const
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
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.TEXTRENDITIONLIST", value.c_str());
    return TEXTRENDITIONLIST_NONE;
}

std::string AttConverter::VerticalalignmentToStr(data_VERTICALALIGNMENT data) const
{
    std::string value;
    switch (data) {
        case VERTICALALIGNMENT_top: value = "top"; break;
        case VERTICALALIGNMENT_middle: value = "middle"; break;
        case VERTICALALIGNMENT_bottom: value = "bottom"; break;
        case VERTICALALIGNMENT_baseline: value = "baseline"; break;
        default:
            LogWarning("Unknown value '%d' for data.VERTICALALIGNMENT", data);
            value = "";
            break;
    }
    return value;
}

data_VERTICALALIGNMENT AttConverter::StrToVerticalalignment(std::string value, bool logWarning) const
{
    if (value == "top") return VERTICALALIGNMENT_top;
    if (value == "middle") return VERTICALALIGNMENT_middle;
    if (value == "bottom") return VERTICALALIGNMENT_bottom;
    if (value == "baseline") return VERTICALALIGNMENT_baseline;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for data.VERTICALALIGNMENT", value.c_str());
    return VERTICALALIGNMENT_NONE;
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

accidLog_FUNC AttConverter::StrToAccidLogFunc(std::string value, bool logWarning) const
{
    if (value == "caution") return accidLog_FUNC_caution;
    if (value == "edit") return accidLog_FUNC_edit;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.accid.log@func", value.c_str());
    return accidLog_FUNC_NONE;
}

std::string AttConverter::AnchoredTextLogFuncToStr(anchoredTextLog_FUNC data) const
{
    std::string value;
    switch (data) {
        case anchoredTextLog_FUNC_unknown: value = "unknown"; break;
        default:
            LogWarning("Unknown value '%d' for att.anchoredText.log@func", data);
            value = "";
            break;
    }
    return value;
}

anchoredTextLog_FUNC AttConverter::StrToAnchoredTextLogFunc(std::string value, bool logWarning) const
{
    if (value == "unknown") return anchoredTextLog_FUNC_unknown;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.anchoredText.log@func", value.c_str());
    return anchoredTextLog_FUNC_NONE;
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

arpegLog_ORDER AttConverter::StrToArpegLogOrder(std::string value, bool logWarning) const
{
    if (value == "up") return arpegLog_ORDER_up;
    if (value == "down") return arpegLog_ORDER_down;
    if (value == "nonarp") return arpegLog_ORDER_nonarp;
    if (logWarning && !value.empty())
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

audience_AUDIENCE AttConverter::StrToAudienceAudience(std::string value, bool logWarning) const
{
    if (value == "private") return audience_AUDIENCE_private;
    if (value == "public") return audience_AUDIENCE_public;
    if (logWarning && !value.empty())
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

bTremLog_FORM AttConverter::StrToBTremLogForm(std::string value, bool logWarning) const
{
    if (value == "meas") return bTremLog_FORM_meas;
    if (value == "unmeas") return bTremLog_FORM_unmeas;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.bTrem.log@form", value.c_str());
    return bTremLog_FORM_NONE;
}

std::string AttConverter::BeamRendFormToStr(beamRend_FORM data) const
{
    std::string value;
    switch (data) {
        case beamRend_FORM_acc: value = "acc"; break;
        case beamRend_FORM_mixed: value = "mixed"; break;
        case beamRend_FORM_rit: value = "rit"; break;
        case beamRend_FORM_norm: value = "norm"; break;
        default:
            LogWarning("Unknown value '%d' for att.beamRend@form", data);
            value = "";
            break;
    }
    return value;
}

beamRend_FORM AttConverter::StrToBeamRendForm(std::string value, bool logWarning) const
{
    if (value == "acc") return beamRend_FORM_acc;
    if (value == "mixed") return beamRend_FORM_mixed;
    if (value == "rit") return beamRend_FORM_rit;
    if (value == "norm") return beamRend_FORM_norm;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.beamRend@form", value.c_str());
    return beamRend_FORM_NONE;
}

std::string AttConverter::BeamingVisBeamrendToStr(beamingVis_BEAMREND data) const
{
    std::string value;
    switch (data) {
        case beamingVis_BEAMREND_acc: value = "acc"; break;
        case beamingVis_BEAMREND_rit: value = "rit"; break;
        case beamingVis_BEAMREND_norm: value = "norm"; break;
        default:
            LogWarning("Unknown value '%d' for att.beaming.vis@beam.rend", data);
            value = "";
            break;
    }
    return value;
}

beamingVis_BEAMREND AttConverter::StrToBeamingVisBeamrend(std::string value, bool logWarning) const
{
    if (value == "acc") return beamingVis_BEAMREND_acc;
    if (value == "rit") return beamingVis_BEAMREND_rit;
    if (value == "norm") return beamingVis_BEAMREND_norm;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.beaming.vis@beam.rend", value.c_str());
    return beamingVis_BEAMREND_NONE;
}

std::string AttConverter::BracketSpanLogFuncToStr(bracketSpanLog_FUNC data) const
{
    std::string value;
    switch (data) {
        case bracketSpanLog_FUNC_coloration: value = "coloration"; break;
        case bracketSpanLog_FUNC_cross_rhythm: value = "cross-rhythm"; break;
        case bracketSpanLog_FUNC_ligature: value = "ligature"; break;
        default:
            LogWarning("Unknown value '%d' for att.bracketSpan.log@func", data);
            value = "";
            break;
    }
    return value;
}

bracketSpanLog_FUNC AttConverter::StrToBracketSpanLogFunc(std::string value, bool logWarning) const
{
    if (value == "coloration") return bracketSpanLog_FUNC_coloration;
    if (value == "cross-rhythm") return bracketSpanLog_FUNC_cross_rhythm;
    if (value == "ligature") return bracketSpanLog_FUNC_ligature;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.bracketSpan.log@func", value.c_str());
    return bracketSpanLog_FUNC_NONE;
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

curvature_CURVEDIR AttConverter::StrToCurvatureCurvedir(std::string value, bool logWarning) const
{
    if (value == "above") return curvature_CURVEDIR_above;
    if (value == "below") return curvature_CURVEDIR_below;
    if (value == "mixed") return curvature_CURVEDIR_mixed;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.curvature@curvedir", value.c_str());
    return curvature_CURVEDIR_NONE;
}

std::string AttConverter::CurveLogFuncToStr(curveLog_FUNC data) const
{
    std::string value;
    switch (data) {
        case curveLog_FUNC_unknown: value = "unknown"; break;
        default:
            LogWarning("Unknown value '%d' for att.curve.log@func", data);
            value = "";
            break;
    }
    return value;
}

curveLog_FUNC AttConverter::StrToCurveLogFunc(std::string value, bool logWarning) const
{
    if (value == "unknown") return curveLog_FUNC_unknown;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.curve.log@func", value.c_str());
    return curveLog_FUNC_NONE;
}

std::string AttConverter::CutoutCutoutToStr(cutout_CUTOUT data) const
{
    std::string value;
    switch (data) {
        case cutout_CUTOUT_cutout: value = "cutout"; break;
        default:
            LogWarning("Unknown value '%d' for att.cutout@cutout", data);
            value = "";
            break;
    }
    return value;
}

cutout_CUTOUT AttConverter::StrToCutoutCutout(std::string value, bool logWarning) const
{
    if (value == "cutout") return cutout_CUTOUT_cutout;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.cutout@cutout", value.c_str());
    return cutout_CUTOUT_NONE;
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

dotLog_FORM AttConverter::StrToDotLogForm(std::string value, bool logWarning) const
{
    if (value == "aug") return dotLog_FORM_aug;
    if (value == "div") return dotLog_FORM_div;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.dot.log@form", value.c_str());
    return dotLog_FORM_NONE;
}

std::string AttConverter::EndingsEndingrendToStr(endings_ENDINGREND data) const
{
    std::string value;
    switch (data) {
        case endings_ENDINGREND_top: value = "top"; break;
        case endings_ENDINGREND_barred: value = "barred"; break;
        case endings_ENDINGREND_grouped: value = "grouped"; break;
        default:
            LogWarning("Unknown value '%d' for att.endings@ending.rend", data);
            value = "";
            break;
    }
    return value;
}

endings_ENDINGREND AttConverter::StrToEndingsEndingrend(std::string value, bool logWarning) const
{
    if (value == "top") return endings_ENDINGREND_top;
    if (value == "barred") return endings_ENDINGREND_barred;
    if (value == "grouped") return endings_ENDINGREND_grouped;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.endings@ending.rend", value.c_str());
    return endings_ENDINGREND_NONE;
}

std::string AttConverter::EpisemaVisFormToStr(episemaVis_FORM data) const
{
    std::string value;
    switch (data) {
        case episemaVis_FORM_h: value = "h"; break;
        case episemaVis_FORM_v: value = "v"; break;
        default:
            LogWarning("Unknown value '%d' for att.episema.vis@form", data);
            value = "";
            break;
    }
    return value;
}

episemaVis_FORM AttConverter::StrToEpisemaVisForm(std::string value, bool logWarning) const
{
    if (value == "h") return episemaVis_FORM_h;
    if (value == "v") return episemaVis_FORM_v;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.episema.vis@form", value.c_str());
    return episemaVis_FORM_NONE;
}

std::string AttConverter::EvidenceEvidenceToStr(evidence_EVIDENCE data) const
{
    std::string value;
    switch (data) {
        case evidence_EVIDENCE_internal: value = "internal"; break;
        case evidence_EVIDENCE_external: value = "external"; break;
        case evidence_EVIDENCE_conjecture: value = "conjecture"; break;
        default:
            LogWarning("Unknown value '%d' for att.evidence@evidence", data);
            value = "";
            break;
    }
    return value;
}

evidence_EVIDENCE AttConverter::StrToEvidenceEvidence(std::string value, bool logWarning) const
{
    if (value == "internal") return evidence_EVIDENCE_internal;
    if (value == "external") return evidence_EVIDENCE_external;
    if (value == "conjecture") return evidence_EVIDENCE_conjecture;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.evidence@evidence", value.c_str());
    return evidence_EVIDENCE_NONE;
}

std::string AttConverter::ExtSymGlyphauthToStr(extSym_GLYPHAUTH data) const
{
    std::string value;
    switch (data) {
        case extSym_GLYPHAUTH_smufl: value = "smufl"; break;
        default:
            LogWarning("Unknown value '%d' for att.extSym@glyph.auth", data);
            value = "";
            break;
    }
    return value;
}

extSym_GLYPHAUTH AttConverter::StrToExtSymGlyphauth(std::string value, bool logWarning) const
{
    if (value == "smufl") return extSym_GLYPHAUTH_smufl;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.extSym@glyph.auth", value.c_str());
    return extSym_GLYPHAUTH_NONE;
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

fTremLog_FORM AttConverter::StrToFTremLogForm(std::string value, bool logWarning) const
{
    if (value == "meas") return fTremLog_FORM_meas;
    if (value == "unmeas") return fTremLog_FORM_unmeas;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.fTrem.log@form", value.c_str());
    return fTremLog_FORM_NONE;
}

std::string AttConverter::FermataVisFormToStr(fermataVis_FORM data) const
{
    std::string value;
    switch (data) {
        case fermataVis_FORM_inv: value = "inv"; break;
        case fermataVis_FORM_norm: value = "norm"; break;
        default:
            LogWarning("Unknown value '%d' for att.fermata.vis@form", data);
            value = "";
            break;
    }
    return value;
}

fermataVis_FORM AttConverter::StrToFermataVisForm(std::string value, bool logWarning) const
{
    if (value == "inv") return fermataVis_FORM_inv;
    if (value == "norm") return fermataVis_FORM_norm;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.fermata.vis@form", value.c_str());
    return fermataVis_FORM_NONE;
}

std::string AttConverter::FermataVisShapeToStr(fermataVis_SHAPE data) const
{
    std::string value;
    switch (data) {
        case fermataVis_SHAPE_curved: value = "curved"; break;
        case fermataVis_SHAPE_square: value = "square"; break;
        case fermataVis_SHAPE_angular: value = "angular"; break;
        default:
            LogWarning("Unknown value '%d' for att.fermata.vis@shape", data);
            value = "";
            break;
    }
    return value;
}

fermataVis_SHAPE AttConverter::StrToFermataVisShape(std::string value, bool logWarning) const
{
    if (value == "curved") return fermataVis_SHAPE_curved;
    if (value == "square") return fermataVis_SHAPE_square;
    if (value == "angular") return fermataVis_SHAPE_angular;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.fermata.vis@shape", value.c_str());
    return fermataVis_SHAPE_NONE;
}

std::string AttConverter::FingGrpLogFormToStr(fingGrpLog_FORM data) const
{
    std::string value;
    switch (data) {
        case fingGrpLog_FORM_alter: value = "alter"; break;
        case fingGrpLog_FORM_combi: value = "combi"; break;
        case fingGrpLog_FORM_subst: value = "subst"; break;
        default:
            LogWarning("Unknown value '%d' for att.fingGrp.log@form", data);
            value = "";
            break;
    }
    return value;
}

fingGrpLog_FORM AttConverter::StrToFingGrpLogForm(std::string value, bool logWarning) const
{
    if (value == "alter") return fingGrpLog_FORM_alter;
    if (value == "combi") return fingGrpLog_FORM_combi;
    if (value == "subst") return fingGrpLog_FORM_subst;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.fingGrp.log@form", value.c_str());
    return fingGrpLog_FORM_NONE;
}

std::string AttConverter::FingGrpVisOrientToStr(fingGrpVis_ORIENT data) const
{
    std::string value;
    switch (data) {
        case fingGrpVis_ORIENT_horiz: value = "horiz"; break;
        case fingGrpVis_ORIENT_vert: value = "vert"; break;
        default:
            LogWarning("Unknown value '%d' for att.fingGrp.vis@orient", data);
            value = "";
            break;
    }
    return value;
}

fingGrpVis_ORIENT AttConverter::StrToFingGrpVisOrient(std::string value, bool logWarning) const
{
    if (value == "horiz") return fingGrpVis_ORIENT_horiz;
    if (value == "vert") return fingGrpVis_ORIENT_vert;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.fingGrp.vis@orient", value.c_str());
    return fingGrpVis_ORIENT_NONE;
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

graceGrpLog_ATTACH AttConverter::StrToGraceGrpLogAttach(std::string value, bool logWarning) const
{
    if (value == "pre") return graceGrpLog_ATTACH_pre;
    if (value == "post") return graceGrpLog_ATTACH_post;
    if (value == "unknown") return graceGrpLog_ATTACH_unknown;
    if (logWarning && !value.empty())
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

hairpinLog_FORM AttConverter::StrToHairpinLogForm(std::string value, bool logWarning) const
{
    if (value == "cres") return hairpinLog_FORM_cres;
    if (value == "dim") return hairpinLog_FORM_dim;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.hairpin.log@form", value.c_str());
    return hairpinLog_FORM_NONE;
}

std::string AttConverter::HarmAnlFormToStr(harmAnl_FORM data) const
{
    std::string value;
    switch (data) {
        case harmAnl_FORM_explicit: value = "explicit"; break;
        case harmAnl_FORM_implied: value = "implied"; break;
        default:
            LogWarning("Unknown value '%d' for att.harm.anl@form", data);
            value = "";
            break;
    }
    return value;
}

harmAnl_FORM AttConverter::StrToHarmAnlForm(std::string value, bool logWarning) const
{
    if (value == "explicit") return harmAnl_FORM_explicit;
    if (value == "implied") return harmAnl_FORM_implied;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.harm.anl@form", value.c_str());
    return harmAnl_FORM_NONE;
}

std::string AttConverter::HarmVisRendgridToStr(harmVis_RENDGRID data) const
{
    std::string value;
    switch (data) {
        case harmVis_RENDGRID_grid: value = "grid"; break;
        case harmVis_RENDGRID_gridtext: value = "gridtext"; break;
        case harmVis_RENDGRID_text: value = "text"; break;
        default:
            LogWarning("Unknown value '%d' for att.harm.vis@rendgrid", data);
            value = "";
            break;
    }
    return value;
}

harmVis_RENDGRID AttConverter::StrToHarmVisRendgrid(std::string value, bool logWarning) const
{
    if (value == "grid") return harmVis_RENDGRID_grid;
    if (value == "gridtext") return harmVis_RENDGRID_gridtext;
    if (value == "text") return harmVis_RENDGRID_text;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.harm.vis@rendgrid", value.c_str());
    return harmVis_RENDGRID_NONE;
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

harpPedalLog_A AttConverter::StrToHarpPedalLogA(std::string value, bool logWarning) const
{
    if (value == "f") return harpPedalLog_A_f;
    if (value == "n") return harpPedalLog_A_n;
    if (value == "s") return harpPedalLog_A_s;
    if (logWarning && !value.empty())
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

harpPedalLog_B AttConverter::StrToHarpPedalLogB(std::string value, bool logWarning) const
{
    if (value == "f") return harpPedalLog_B_f;
    if (value == "n") return harpPedalLog_B_n;
    if (value == "s") return harpPedalLog_B_s;
    if (logWarning && !value.empty())
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

harpPedalLog_C AttConverter::StrToHarpPedalLogC(std::string value, bool logWarning) const
{
    if (value == "f") return harpPedalLog_C_f;
    if (value == "n") return harpPedalLog_C_n;
    if (value == "s") return harpPedalLog_C_s;
    if (logWarning && !value.empty())
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

harpPedalLog_D AttConverter::StrToHarpPedalLogD(std::string value, bool logWarning) const
{
    if (value == "f") return harpPedalLog_D_f;
    if (value == "n") return harpPedalLog_D_n;
    if (value == "s") return harpPedalLog_D_s;
    if (logWarning && !value.empty())
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

harpPedalLog_E AttConverter::StrToHarpPedalLogE(std::string value, bool logWarning) const
{
    if (value == "f") return harpPedalLog_E_f;
    if (value == "n") return harpPedalLog_E_n;
    if (value == "s") return harpPedalLog_E_s;
    if (logWarning && !value.empty())
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

harpPedalLog_F AttConverter::StrToHarpPedalLogF(std::string value, bool logWarning) const
{
    if (value == "f") return harpPedalLog_F_f;
    if (value == "n") return harpPedalLog_F_n;
    if (value == "s") return harpPedalLog_F_s;
    if (logWarning && !value.empty())
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

harpPedalLog_G AttConverter::StrToHarpPedalLogG(std::string value, bool logWarning) const
{
    if (value == "f") return harpPedalLog_G_f;
    if (value == "n") return harpPedalLog_G_n;
    if (value == "s") return harpPedalLog_G_s;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.harpPedal.log@g", value.c_str());
    return harpPedalLog_G_NONE;
}

std::string AttConverter::LineLogFuncToStr(lineLog_FUNC data) const
{
    std::string value;
    switch (data) {
        case lineLog_FUNC_coloration: value = "coloration"; break;
        case lineLog_FUNC_ligature: value = "ligature"; break;
        case lineLog_FUNC_unknown: value = "unknown"; break;
        default:
            LogWarning("Unknown value '%d' for att.line.log@func", data);
            value = "";
            break;
    }
    return value;
}

lineLog_FUNC AttConverter::StrToLineLogFunc(std::string value, bool logWarning) const
{
    if (value == "coloration") return lineLog_FUNC_coloration;
    if (value == "ligature") return lineLog_FUNC_ligature;
    if (value == "unknown") return lineLog_FUNC_unknown;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.line.log@func", value.c_str());
    return lineLog_FUNC_NONE;
}

std::string AttConverter::LiquescentVisCurveToStr(liquescentVis_CURVE data) const
{
    std::string value;
    switch (data) {
        case liquescentVis_CURVE_a: value = "a"; break;
        case liquescentVis_CURVE_c: value = "c"; break;
        default:
            LogWarning("Unknown value '%d' for att.liquescent.vis@curve", data);
            value = "";
            break;
    }
    return value;
}

liquescentVis_CURVE AttConverter::StrToLiquescentVisCurve(std::string value, bool logWarning) const
{
    if (value == "a") return liquescentVis_CURVE_a;
    if (value == "c") return liquescentVis_CURVE_c;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.liquescent.vis@curve", value.c_str());
    return liquescentVis_CURVE_NONE;
}

std::string AttConverter::MeasurementUnitToStr(measurement_UNIT data) const
{
    std::string value;
    switch (data) {
        case measurement_UNIT_byte: value = "byte"; break;
        case measurement_UNIT_char: value = "char"; break;
        case measurement_UNIT_cm: value = "cm"; break;
        case measurement_UNIT_deg: value = "deg"; break;
        case measurement_UNIT_in: value = "in"; break;
        case measurement_UNIT_issue: value = "issue"; break;
        case measurement_UNIT_ft: value = "ft"; break;
        case measurement_UNIT_m: value = "m"; break;
        case measurement_UNIT_mm: value = "mm"; break;
        case measurement_UNIT_page: value = "page"; break;
        case measurement_UNIT_pc: value = "pc"; break;
        case measurement_UNIT_pt: value = "pt"; break;
        case measurement_UNIT_px: value = "px"; break;
        case measurement_UNIT_rad: value = "rad"; break;
        case measurement_UNIT_record: value = "record"; break;
        case measurement_UNIT_vol: value = "vol"; break;
        case measurement_UNIT_vu: value = "vu"; break;
        default:
            LogWarning("Unknown value '%d' for att.measurement@unit", data);
            value = "";
            break;
    }
    return value;
}

measurement_UNIT AttConverter::StrToMeasurementUnit(std::string value, bool logWarning) const
{
    if (value == "byte") return measurement_UNIT_byte;
    if (value == "char") return measurement_UNIT_char;
    if (value == "cm") return measurement_UNIT_cm;
    if (value == "deg") return measurement_UNIT_deg;
    if (value == "in") return measurement_UNIT_in;
    if (value == "issue") return measurement_UNIT_issue;
    if (value == "ft") return measurement_UNIT_ft;
    if (value == "m") return measurement_UNIT_m;
    if (value == "mm") return measurement_UNIT_mm;
    if (value == "page") return measurement_UNIT_page;
    if (value == "pc") return measurement_UNIT_pc;
    if (value == "pt") return measurement_UNIT_pt;
    if (value == "px") return measurement_UNIT_px;
    if (value == "rad") return measurement_UNIT_rad;
    if (value == "record") return measurement_UNIT_record;
    if (value == "vol") return measurement_UNIT_vol;
    if (value == "vu") return measurement_UNIT_vu;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.measurement@unit", value.c_str());
    return measurement_UNIT_NONE;
}

std::string AttConverter::MeiVersionMeiversionToStr(meiVersion_MEIVERSION data) const
{
    std::string value;
    switch (data) {
        case meiVersion_MEIVERSION_4_0_0: value = "4.0.0"; break;
        case meiVersion_MEIVERSION_4_0_1: value = "4.0.1"; break;
        default:
            LogWarning("Unknown value '%d' for att.meiVersion@meiversion", data);
            value = "";
            break;
    }
    return value;
}

meiVersion_MEIVERSION AttConverter::StrToMeiVersionMeiversion(std::string value, bool logWarning) const
{
    if (value == "4.0.0") return meiVersion_MEIVERSION_4_0_0;
    if (value == "4.0.1") return meiVersion_MEIVERSION_4_0_1;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.meiVersion@meiversion", value.c_str());
    return meiVersion_MEIVERSION_NONE;
}

std::string AttConverter::MensurVisFormToStr(mensurVis_FORM data) const
{
    std::string value;
    switch (data) {
        case mensurVis_FORM_horizontal: value = "horizontal"; break;
        case mensurVis_FORM_vertical: value = "vertical"; break;
        default:
            LogWarning("Unknown value '%d' for att.mensur.vis@form", data);
            value = "";
            break;
    }
    return value;
}

mensurVis_FORM AttConverter::StrToMensurVisForm(std::string value, bool logWarning) const
{
    if (value == "horizontal") return mensurVis_FORM_horizontal;
    if (value == "vertical") return mensurVis_FORM_vertical;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.mensur.vis@form", value.c_str());
    return mensurVis_FORM_NONE;
}

std::string AttConverter::MensuralVisMensurformToStr(mensuralVis_MENSURFORM data) const
{
    std::string value;
    switch (data) {
        case mensuralVis_MENSURFORM_horizontal: value = "horizontal"; break;
        case mensuralVis_MENSURFORM_vertical: value = "vertical"; break;
        default:
            LogWarning("Unknown value '%d' for att.mensural.vis@mensur.form", data);
            value = "";
            break;
    }
    return value;
}

mensuralVis_MENSURFORM AttConverter::StrToMensuralVisMensurform(std::string value, bool logWarning) const
{
    if (value == "horizontal") return mensuralVis_MENSURFORM_horizontal;
    if (value == "vertical") return mensuralVis_MENSURFORM_vertical;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.mensural.vis@mensur.form", value.c_str());
    return mensuralVis_MENSURFORM_NONE;
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

meterConformance_METCON AttConverter::StrToMeterConformanceMetcon(std::string value, bool logWarning) const
{
    if (value == "c") return meterConformance_METCON_c;
    if (value == "i") return meterConformance_METCON_i;
    if (value == "o") return meterConformance_METCON_o;
    if (logWarning && !value.empty())
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

meterSigGrpLog_FUNC AttConverter::StrToMeterSigGrpLogFunc(std::string value, bool logWarning) const
{
    if (value == "alternating") return meterSigGrpLog_FUNC_alternating;
    if (value == "interchanging") return meterSigGrpLog_FUNC_interchanging;
    if (value == "mixed") return meterSigGrpLog_FUNC_mixed;
    if (logWarning && !value.empty())
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

mordentLog_FORM AttConverter::StrToMordentLogForm(std::string value, bool logWarning) const
{
    if (value == "lower") return mordentLog_FORM_lower;
    if (value == "upper") return mordentLog_FORM_upper;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.mordent.log@form", value.c_str());
    return mordentLog_FORM_NONE;
}

std::string AttConverter::NcFormConToStr(ncForm_CON data) const
{
    std::string value;
    switch (data) {
        case ncForm_CON_g: value = "g"; break;
        case ncForm_CON_l: value = "l"; break;
        case ncForm_CON_e: value = "e"; break;
        default:
            LogWarning("Unknown value '%d' for att.ncForm@con", data);
            value = "";
            break;
    }
    return value;
}

ncForm_CON AttConverter::StrToNcFormCon(std::string value, bool logWarning) const
{
    if (value == "g") return ncForm_CON_g;
    if (value == "l") return ncForm_CON_l;
    if (value == "e") return ncForm_CON_e;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.ncForm@con", value.c_str());
    return ncForm_CON_NONE;
}

std::string AttConverter::NcFormCurveToStr(ncForm_CURVE data) const
{
    std::string value;
    switch (data) {
        case ncForm_CURVE_a: value = "a"; break;
        case ncForm_CURVE_c: value = "c"; break;
        default:
            LogWarning("Unknown value '%d' for att.ncForm@curve", data);
            value = "";
            break;
    }
    return value;
}

ncForm_CURVE AttConverter::StrToNcFormCurve(std::string value, bool logWarning) const
{
    if (value == "a") return ncForm_CURVE_a;
    if (value == "c") return ncForm_CURVE_c;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.ncForm@curve", value.c_str());
    return ncForm_CURVE_NONE;
}

std::string AttConverter::NcFormRellenToStr(ncForm_RELLEN data) const
{
    std::string value;
    switch (data) {
        case ncForm_RELLEN_l: value = "l"; break;
        case ncForm_RELLEN_s: value = "s"; break;
        default:
            LogWarning("Unknown value '%d' for att.ncForm@rellen", data);
            value = "";
            break;
    }
    return value;
}

ncForm_RELLEN AttConverter::StrToNcFormRellen(std::string value, bool logWarning) const
{
    if (value == "l") return ncForm_RELLEN_l;
    if (value == "s") return ncForm_RELLEN_s;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.ncForm@rellen", value.c_str());
    return ncForm_RELLEN_NONE;
}

std::string AttConverter::NoteAnlMensuralLigToStr(noteAnlMensural_LIG data) const
{
    std::string value;
    switch (data) {
        case noteAnlMensural_LIG_recta: value = "recta"; break;
        case noteAnlMensural_LIG_obliqua: value = "obliqua"; break;
        default:
            LogWarning("Unknown value '%d' for att.note.anl.mensural@lig", data);
            value = "";
            break;
    }
    return value;
}

noteAnlMensural_LIG AttConverter::StrToNoteAnlMensuralLig(std::string value, bool logWarning) const
{
    if (value == "recta") return noteAnlMensural_LIG_recta;
    if (value == "obliqua") return noteAnlMensural_LIG_obliqua;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.note.anl.mensural@lig", value.c_str());
    return noteAnlMensural_LIG_NONE;
}

std::string AttConverter::NoteGesExtremisToStr(noteGes_EXTREMIS data) const
{
    std::string value;
    switch (data) {
        case noteGes_EXTREMIS_highest: value = "highest"; break;
        case noteGes_EXTREMIS_lowest: value = "lowest"; break;
        default:
            LogWarning("Unknown value '%d' for att.note.ges@extremis", data);
            value = "";
            break;
    }
    return value;
}

noteGes_EXTREMIS AttConverter::StrToNoteGesExtremis(std::string value, bool logWarning) const
{
    if (value == "highest") return noteGes_EXTREMIS_highest;
    if (value == "lowest") return noteGes_EXTREMIS_lowest;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.note.ges@extremis", value.c_str());
    return noteGes_EXTREMIS_NONE;
}

std::string AttConverter::NoteHeadsHeadauthToStr(noteHeads_HEADAUTH data) const
{
    std::string value;
    switch (data) {
        case noteHeads_HEADAUTH_smufl: value = "smufl"; break;
        default:
            LogWarning("Unknown value '%d' for att.noteHeads@head.auth", data);
            value = "";
            break;
    }
    return value;
}

noteHeads_HEADAUTH AttConverter::StrToNoteHeadsHeadauth(std::string value, bool logWarning) const
{
    if (value == "smufl") return noteHeads_HEADAUTH_smufl;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.noteHeads@head.auth", value.c_str());
    return noteHeads_HEADAUTH_NONE;
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

octaveLog_COLL AttConverter::StrToOctaveLogColl(std::string value, bool logWarning) const
{
    if (value == "coll") return octaveLog_COLL_coll;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.octave.log@coll", value.c_str());
    return octaveLog_COLL_NONE;
}

std::string AttConverter::PbVisFoliumToStr(pbVis_FOLIUM data) const
{
    std::string value;
    switch (data) {
        case pbVis_FOLIUM_verso: value = "verso"; break;
        case pbVis_FOLIUM_recto: value = "recto"; break;
        default:
            LogWarning("Unknown value '%d' for att.pb.vis@folium", data);
            value = "";
            break;
    }
    return value;
}

pbVis_FOLIUM AttConverter::StrToPbVisFolium(std::string value, bool logWarning) const
{
    if (value == "verso") return pbVis_FOLIUM_verso;
    if (value == "recto") return pbVis_FOLIUM_recto;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.pb.vis@folium", value.c_str());
    return pbVis_FOLIUM_NONE;
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

pedalLog_DIR AttConverter::StrToPedalLogDir(std::string value, bool logWarning) const
{
    if (value == "down") return pedalLog_DIR_down;
    if (value == "up") return pedalLog_DIR_up;
    if (value == "half") return pedalLog_DIR_half;
    if (value == "bounce") return pedalLog_DIR_bounce;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.pedal.log@dir", value.c_str());
    return pedalLog_DIR_NONE;
}

std::string AttConverter::PedalLogFuncToStr(pedalLog_FUNC data) const
{
    std::string value;
    switch (data) {
        case pedalLog_FUNC_sustain: value = "sustain"; break;
        case pedalLog_FUNC_soft: value = "soft"; break;
        case pedalLog_FUNC_sostenuto: value = "sostenuto"; break;
        case pedalLog_FUNC_silent: value = "silent"; break;
        default:
            LogWarning("Unknown value '%d' for att.pedal.log@func", data);
            value = "";
            break;
    }
    return value;
}

pedalLog_FUNC AttConverter::StrToPedalLogFunc(std::string value, bool logWarning) const
{
    if (value == "sustain") return pedalLog_FUNC_sustain;
    if (value == "soft") return pedalLog_FUNC_soft;
    if (value == "sostenuto") return pedalLog_FUNC_sostenuto;
    if (value == "silent") return pedalLog_FUNC_silent;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.pedal.log@func", value.c_str());
    return pedalLog_FUNC_NONE;
}

std::string AttConverter::PedalVisFormToStr(pedalVis_FORM data) const
{
    std::string value;
    switch (data) {
        case pedalVis_FORM_line: value = "line"; break;
        case pedalVis_FORM_pedstar: value = "pedstar"; break;
        case pedalVis_FORM_altpedstar: value = "altpedstar"; break;
        default:
            LogWarning("Unknown value '%d' for att.pedal.vis@form", data);
            value = "";
            break;
    }
    return value;
}

pedalVis_FORM AttConverter::StrToPedalVisForm(std::string value, bool logWarning) const
{
    if (value == "line") return pedalVis_FORM_line;
    if (value == "pedstar") return pedalVis_FORM_pedstar;
    if (value == "altpedstar") return pedalVis_FORM_altpedstar;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.pedal.vis@form", value.c_str());
    return pedalVis_FORM_NONE;
}

std::string AttConverter::PianoPedalsPedalstyleToStr(pianoPedals_PEDALSTYLE data) const
{
    std::string value;
    switch (data) {
        case pianoPedals_PEDALSTYLE_line: value = "line"; break;
        case pianoPedals_PEDALSTYLE_pedstar: value = "pedstar"; break;
        case pianoPedals_PEDALSTYLE_altpedstar: value = "altpedstar"; break;
        default:
            LogWarning("Unknown value '%d' for att.pianoPedals@pedal.style", data);
            value = "";
            break;
    }
    return value;
}

pianoPedals_PEDALSTYLE AttConverter::StrToPianoPedalsPedalstyle(std::string value, bool logWarning) const
{
    if (value == "line") return pianoPedals_PEDALSTYLE_line;
    if (value == "pedstar") return pianoPedals_PEDALSTYLE_pedstar;
    if (value == "altpedstar") return pianoPedals_PEDALSTYLE_altpedstar;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.pianoPedals@pedal.style", value.c_str());
    return pianoPedals_PEDALSTYLE_NONE;
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

pointing_XLINKACTUATE AttConverter::StrToPointingXlinkactuate(std::string value, bool logWarning) const
{
    if (value == "onLoad") return pointing_XLINKACTUATE_onLoad;
    if (value == "onRequest") return pointing_XLINKACTUATE_onRequest;
    if (value == "none") return pointing_XLINKACTUATE_none;
    if (value == "other") return pointing_XLINKACTUATE_other;
    if (logWarning && !value.empty())
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

pointing_XLINKSHOW AttConverter::StrToPointingXlinkshow(std::string value, bool logWarning) const
{
    if (value == "new") return pointing_XLINKSHOW_new;
    if (value == "replace") return pointing_XLINKSHOW_replace;
    if (value == "embed") return pointing_XLINKSHOW_embed;
    if (value == "none") return pointing_XLINKSHOW_none;
    if (value == "other") return pointing_XLINKSHOW_other;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.pointing@xlink:show", value.c_str());
    return pointing_XLINKSHOW_NONE;
}

std::string AttConverter::RecordTypeRecordtypeToStr(recordType_RECORDTYPE data) const
{
    std::string value;
    switch (data) {
        case recordType_RECORDTYPE_a: value = "a"; break;
        case recordType_RECORDTYPE_c: value = "c"; break;
        case recordType_RECORDTYPE_d: value = "d"; break;
        case recordType_RECORDTYPE_e: value = "e"; break;
        case recordType_RECORDTYPE_f: value = "f"; break;
        case recordType_RECORDTYPE_g: value = "g"; break;
        case recordType_RECORDTYPE_i: value = "i"; break;
        case recordType_RECORDTYPE_j: value = "j"; break;
        case recordType_RECORDTYPE_k: value = "k"; break;
        case recordType_RECORDTYPE_m: value = "m"; break;
        case recordType_RECORDTYPE_o: value = "o"; break;
        case recordType_RECORDTYPE_p: value = "p"; break;
        case recordType_RECORDTYPE_r: value = "r"; break;
        case recordType_RECORDTYPE_t: value = "t"; break;
        default:
            LogWarning("Unknown value '%d' for att.recordType@recordtype", data);
            value = "";
            break;
    }
    return value;
}

recordType_RECORDTYPE AttConverter::StrToRecordTypeRecordtype(std::string value, bool logWarning) const
{
    if (value == "a") return recordType_RECORDTYPE_a;
    if (value == "c") return recordType_RECORDTYPE_c;
    if (value == "d") return recordType_RECORDTYPE_d;
    if (value == "e") return recordType_RECORDTYPE_e;
    if (value == "f") return recordType_RECORDTYPE_f;
    if (value == "g") return recordType_RECORDTYPE_g;
    if (value == "i") return recordType_RECORDTYPE_i;
    if (value == "j") return recordType_RECORDTYPE_j;
    if (value == "k") return recordType_RECORDTYPE_k;
    if (value == "m") return recordType_RECORDTYPE_m;
    if (value == "o") return recordType_RECORDTYPE_o;
    if (value == "p") return recordType_RECORDTYPE_p;
    if (value == "r") return recordType_RECORDTYPE_r;
    if (value == "t") return recordType_RECORDTYPE_t;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.recordType@recordtype", value.c_str());
    return recordType_RECORDTYPE_NONE;
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

regularMethod_METHOD AttConverter::StrToRegularMethodMethod(std::string value, bool logWarning) const
{
    if (value == "silent") return regularMethod_METHOD_silent;
    if (value == "tags") return regularMethod_METHOD_tags;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.regularMethod@method", value.c_str());
    return regularMethod_METHOD_NONE;
}

std::string AttConverter::RehearsalRehencloseToStr(rehearsal_REHENCLOSE data) const
{
    std::string value;
    switch (data) {
        case rehearsal_REHENCLOSE_box: value = "box"; break;
        case rehearsal_REHENCLOSE_circle: value = "circle"; break;
        case rehearsal_REHENCLOSE_none: value = "none"; break;
        default:
            LogWarning("Unknown value '%d' for att.rehearsal@reh.enclose", data);
            value = "";
            break;
    }
    return value;
}

rehearsal_REHENCLOSE AttConverter::StrToRehearsalRehenclose(std::string value, bool logWarning) const
{
    if (value == "box") return rehearsal_REHENCLOSE_box;
    if (value == "circle") return rehearsal_REHENCLOSE_circle;
    if (value == "none") return rehearsal_REHENCLOSE_none;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.rehearsal@reh.enclose", value.c_str());
    return rehearsal_REHENCLOSE_NONE;
}

std::string AttConverter::SbVisFormToStr(sbVis_FORM data) const
{
    std::string value;
    switch (data) {
        case sbVis_FORM_hash: value = "hash"; break;
        default:
            LogWarning("Unknown value '%d' for att.sb.vis@form", data);
            value = "";
            break;
    }
    return value;
}

sbVis_FORM AttConverter::StrToSbVisForm(std::string value, bool logWarning) const
{
    if (value == "hash") return sbVis_FORM_hash;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.sb.vis@form", value.c_str());
    return sbVis_FORM_NONE;
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

staffGroupingSym_SYMBOL AttConverter::StrToStaffGroupingSymSymbol(std::string value, bool logWarning) const
{
    if (value == "brace") return staffGroupingSym_SYMBOL_brace;
    if (value == "bracket") return staffGroupingSym_SYMBOL_bracket;
    if (value == "bracketsq") return staffGroupingSym_SYMBOL_bracketsq;
    if (value == "line") return staffGroupingSym_SYMBOL_line;
    if (value == "none") return staffGroupingSym_SYMBOL_none;
    if (logWarning && !value.empty())
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

sylLog_CON AttConverter::StrToSylLogCon(std::string value, bool logWarning) const
{
    if (value == "s") return sylLog_CON_s;
    if (value == "d") return sylLog_CON_d;
    if (value == "u") return sylLog_CON_u;
    if (value == "t") return sylLog_CON_t;
    if (value == "c") return sylLog_CON_c;
    if (value == "v") return sylLog_CON_v;
    if (value == "i") return sylLog_CON_i;
    if (value == "b") return sylLog_CON_b;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.syl.log@con", value.c_str());
    return sylLog_CON_NONE;
}

std::string AttConverter::SylLogWordposToStr(sylLog_WORDPOS data) const
{
    std::string value;
    switch (data) {
        case sylLog_WORDPOS_i: value = "i"; break;
        case sylLog_WORDPOS_m: value = "m"; break;
        case sylLog_WORDPOS_s: value = "s"; break;
        case sylLog_WORDPOS_t: value = "t"; break;
        default:
            LogWarning("Unknown value '%d' for att.syl.log@wordpos", data);
            value = "";
            break;
    }
    return value;
}

sylLog_WORDPOS AttConverter::StrToSylLogWordpos(std::string value, bool logWarning) const
{
    if (value == "i") return sylLog_WORDPOS_i;
    if (value == "m") return sylLog_WORDPOS_m;
    if (value == "s") return sylLog_WORDPOS_s;
    if (value == "t") return sylLog_WORDPOS_t;
    if (logWarning && !value.empty())
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

targetEval_EVALUATE AttConverter::StrToTargetEvalEvaluate(std::string value, bool logWarning) const
{
    if (value == "all") return targetEval_EVALUATE_all;
    if (value == "one") return targetEval_EVALUATE_one;
    if (value == "none") return targetEval_EVALUATE_none;
    if (logWarning && !value.empty())
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

tempoLog_FUNC AttConverter::StrToTempoLogFunc(std::string value, bool logWarning) const
{
    if (value == "continuous") return tempoLog_FUNC_continuous;
    if (value == "instantaneous") return tempoLog_FUNC_instantaneous;
    if (value == "metricmod") return tempoLog_FUNC_metricmod;
    if (value == "precedente") return tempoLog_FUNC_precedente;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.tempo.log@func", value.c_str());
    return tempoLog_FUNC_NONE;
}

std::string AttConverter::TupletVisNumformatToStr(tupletVis_NUMFORMAT data) const
{
    std::string value;
    switch (data) {
        case tupletVis_NUMFORMAT_count: value = "count"; break;
        case tupletVis_NUMFORMAT_ratio: value = "ratio"; break;
        default:
            LogWarning("Unknown value '%d' for att.tuplet.vis@num.format", data);
            value = "";
            break;
    }
    return value;
}

tupletVis_NUMFORMAT AttConverter::StrToTupletVisNumformat(std::string value, bool logWarning) const
{
    if (value == "count") return tupletVis_NUMFORMAT_count;
    if (value == "ratio") return tupletVis_NUMFORMAT_ratio;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.tuplet.vis@num.format", value.c_str());
    return tupletVis_NUMFORMAT_NONE;
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

turnLog_FORM AttConverter::StrToTurnLogForm(std::string value, bool logWarning) const
{
    if (value == "lower") return turnLog_FORM_lower;
    if (value == "upper") return turnLog_FORM_upper;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.turn.log@form", value.c_str());
    return turnLog_FORM_NONE;
}

std::string AttConverter::VoltaGroupingSymVoltasymToStr(voltaGroupingSym_VOLTASYM data) const
{
    std::string value;
    switch (data) {
        case voltaGroupingSym_VOLTASYM_brace: value = "brace"; break;
        case voltaGroupingSym_VOLTASYM_bracket: value = "bracket"; break;
        case voltaGroupingSym_VOLTASYM_bracketsq: value = "bracketsq"; break;
        case voltaGroupingSym_VOLTASYM_line: value = "line"; break;
        case voltaGroupingSym_VOLTASYM_none: value = "none"; break;
        default:
            LogWarning("Unknown value '%d' for att.voltaGroupingSym@voltasym", data);
            value = "";
            break;
    }
    return value;
}

voltaGroupingSym_VOLTASYM AttConverter::StrToVoltaGroupingSymVoltasym(std::string value, bool logWarning) const
{
    if (value == "brace") return voltaGroupingSym_VOLTASYM_brace;
    if (value == "bracket") return voltaGroupingSym_VOLTASYM_bracket;
    if (value == "bracketsq") return voltaGroupingSym_VOLTASYM_bracketsq;
    if (value == "line") return voltaGroupingSym_VOLTASYM_line;
    if (value == "none") return voltaGroupingSym_VOLTASYM_none;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.voltaGroupingSym@voltasym", value.c_str());
    return voltaGroupingSym_VOLTASYM_NONE;
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

whitespace_XMLSPACE AttConverter::StrToWhitespaceXmlspace(std::string value, bool logWarning) const
{
    if (value == "default") return whitespace_XMLSPACE_default;
    if (value == "preserve") return whitespace_XMLSPACE_preserve;
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for att.whitespace@xml:space", value.c_str());
    return whitespace_XMLSPACE_NONE;
}

} // vrv namespace

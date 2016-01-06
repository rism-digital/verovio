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

std::string AttConverter::AccidentalExplicitToStr(data_ACCIDENTAL_EXPLICIT data) 
{
    std::string value;
    switch(data)
    {
        case ACCIDENTAL_EXPLICIT_s : value = "s"; break;
        case ACCIDENTAL_EXPLICIT_f : value = "f"; break;
        case ACCIDENTAL_EXPLICIT_ss : value = "ss"; break;
        case ACCIDENTAL_EXPLICIT_x : value = "x"; break;
        case ACCIDENTAL_EXPLICIT_ff : value = "ff"; break;
        case ACCIDENTAL_EXPLICIT_xs : value = "xs"; break;
        case ACCIDENTAL_EXPLICIT_ts : value = "ts"; break;
        case ACCIDENTAL_EXPLICIT_tf : value = "tf"; break;
        case ACCIDENTAL_EXPLICIT_n : value = "n"; break;
        case ACCIDENTAL_EXPLICIT_nf : value = "nf"; break;
        case ACCIDENTAL_EXPLICIT_ns : value = "ns"; break;
        case ACCIDENTAL_EXPLICIT_su : value = "su"; break;
        case ACCIDENTAL_EXPLICIT_sd : value = "sd"; break;
        case ACCIDENTAL_EXPLICIT_fu : value = "fu"; break;
        case ACCIDENTAL_EXPLICIT_fd : value = "fd"; break;
        case ACCIDENTAL_EXPLICIT_nu : value = "nu"; break;
        case ACCIDENTAL_EXPLICIT_nd : value = "nd"; break;
        case ACCIDENTAL_EXPLICIT_1qf : value = "1qf"; break;
        case ACCIDENTAL_EXPLICIT_3qf : value = "3qf"; break;
        case ACCIDENTAL_EXPLICIT_1qs : value = "1qs"; break;
        case ACCIDENTAL_EXPLICIT_3qs : value = "3qs"; break;
        default:
            LogWarning("Unknown value '%d' for data.ACCIDENTAL.EXPLICIT", data);
            value = "";
            break;
    }
    return value;
}

data_ACCIDENTAL_EXPLICIT AttConverter::StrToAccidentalExplicit(std::string value)
{
    if (value == "s") return ACCIDENTAL_EXPLICIT_s;
    if (value == "f") return ACCIDENTAL_EXPLICIT_f;
    if (value == "ss") return ACCIDENTAL_EXPLICIT_ss;
    if (value == "x") return ACCIDENTAL_EXPLICIT_x;
    if (value == "ff") return ACCIDENTAL_EXPLICIT_ff;
    if (value == "xs") return ACCIDENTAL_EXPLICIT_xs;
    if (value == "ts") return ACCIDENTAL_EXPLICIT_ts;
    if (value == "tf") return ACCIDENTAL_EXPLICIT_tf;
    if (value == "n") return ACCIDENTAL_EXPLICIT_n;
    if (value == "nf") return ACCIDENTAL_EXPLICIT_nf;
    if (value == "ns") return ACCIDENTAL_EXPLICIT_ns;
    if (value == "su") return ACCIDENTAL_EXPLICIT_su;
    if (value == "sd") return ACCIDENTAL_EXPLICIT_sd;
    if (value == "fu") return ACCIDENTAL_EXPLICIT_fu;
    if (value == "fd") return ACCIDENTAL_EXPLICIT_fd;
    if (value == "nu") return ACCIDENTAL_EXPLICIT_nu;
    if (value == "nd") return ACCIDENTAL_EXPLICIT_nd;
    if (value == "1qf") return ACCIDENTAL_EXPLICIT_1qf;
    if (value == "3qf") return ACCIDENTAL_EXPLICIT_3qf;
    if (value == "1qs") return ACCIDENTAL_EXPLICIT_1qs;
    if (value == "3qs") return ACCIDENTAL_EXPLICIT_3qs;
    LogWarning("Unsupported value '%s' for data.ACCIDENTAL.EXPLICIT", value.c_str());
    return ACCIDENTAL_EXPLICIT_NONE;
}

std::string AttConverter::AccidentalImplicitToStr(data_ACCIDENTAL_IMPLICIT data) 
{
    std::string value;
    switch(data)
    {
        case ACCIDENTAL_IMPLICIT_s : value = "s"; break;
        case ACCIDENTAL_IMPLICIT_f : value = "f"; break;
        case ACCIDENTAL_IMPLICIT_ss : value = "ss"; break;
        case ACCIDENTAL_IMPLICIT_ff : value = "ff"; break;
        case ACCIDENTAL_IMPLICIT_n : value = "n"; break;
        case ACCIDENTAL_IMPLICIT_su : value = "su"; break;
        case ACCIDENTAL_IMPLICIT_sd : value = "sd"; break;
        case ACCIDENTAL_IMPLICIT_fu : value = "fu"; break;
        case ACCIDENTAL_IMPLICIT_fd : value = "fd"; break;
        default:
            LogWarning("Unknown value '%d' for data.ACCIDENTAL.IMPLICIT", data);
            value = "";
            break;
    }
    return value;
}

data_ACCIDENTAL_IMPLICIT AttConverter::StrToAccidentalImplicit(std::string value)
{
    if (value == "s") return ACCIDENTAL_IMPLICIT_s;
    if (value == "f") return ACCIDENTAL_IMPLICIT_f;
    if (value == "ss") return ACCIDENTAL_IMPLICIT_ss;
    if (value == "ff") return ACCIDENTAL_IMPLICIT_ff;
    if (value == "n") return ACCIDENTAL_IMPLICIT_n;
    if (value == "su") return ACCIDENTAL_IMPLICIT_su;
    if (value == "sd") return ACCIDENTAL_IMPLICIT_sd;
    if (value == "fu") return ACCIDENTAL_IMPLICIT_fu;
    if (value == "fd") return ACCIDENTAL_IMPLICIT_fd;
    LogWarning("Unsupported value '%s' for data.ACCIDENTAL.IMPLICIT", value.c_str());
    return ACCIDENTAL_IMPLICIT_NONE;
}

std::string AttConverter::ArticulationToStr(data_ARTICULATION data) 
{
    std::string value;
    switch(data)
    {
        case ARTICULATION_acc : value = "acc"; break;
        case ARTICULATION_stacc : value = "stacc"; break;
        case ARTICULATION_ten : value = "ten"; break;
        case ARTICULATION_stacciss : value = "stacciss"; break;
        case ARTICULATION_marc : value = "marc"; break;
        case ARTICULATION_marc_stacc : value = "marc-stacc"; break;
        case ARTICULATION_spicc : value = "spicc"; break;
        case ARTICULATION_doit : value = "doit"; break;
        case ARTICULATION_scoop : value = "scoop"; break;
        case ARTICULATION_rip : value = "rip"; break;
        case ARTICULATION_plop : value = "plop"; break;
        case ARTICULATION_fall : value = "fall"; break;
        case ARTICULATION_longfall : value = "longfall"; break;
        case ARTICULATION_bend : value = "bend"; break;
        case ARTICULATION_flip : value = "flip"; break;
        case ARTICULATION_smear : value = "smear"; break;
        case ARTICULATION_shake : value = "shake"; break;
        case ARTICULATION_dnbow : value = "dnbow"; break;
        case ARTICULATION_upbow : value = "upbow"; break;
        case ARTICULATION_harm : value = "harm"; break;
        case ARTICULATION_snap : value = "snap"; break;
        case ARTICULATION_fingernail : value = "fingernail"; break;
        case ARTICULATION_ten_stacc : value = "ten-stacc"; break;
        case ARTICULATION_damp : value = "damp"; break;
        case ARTICULATION_dampall : value = "dampall"; break;
        case ARTICULATION_open : value = "open"; break;
        case ARTICULATION_stop : value = "stop"; break;
        case ARTICULATION_dbltongue : value = "dbltongue"; break;
        case ARTICULATION_trpltongue : value = "trpltongue"; break;
        case ARTICULATION_heel : value = "heel"; break;
        case ARTICULATION_toe : value = "toe"; break;
        case ARTICULATION_tap : value = "tap"; break;
        case ARTICULATION_lhpizz : value = "lhpizz"; break;
        case ARTICULATION_dot : value = "dot"; break;
        case ARTICULATION_stroke : value = "stroke"; break;
        default:
            LogWarning("Unknown value '%d' for data.ARTICULATION", data);
            value = "";
            break;
    }
    return value;
}

data_ARTICULATION AttConverter::StrToArticulation(std::string value)
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

std::string AttConverter::BarplaceToStr(data_BARPLACE data) 
{
    std::string value;
    switch(data)
    {
        case BARPLACE_mensur : value = "mensur"; break;
        case BARPLACE_staff : value = "staff"; break;
        case BARPLACE_takt : value = "takt"; break;
        default:
            LogWarning("Unknown value '%d' for data.BARPLACE", data);
            value = "";
            break;
    }
    return value;
}

data_BARPLACE AttConverter::StrToBarplace(std::string value)
{
    if (value == "mensur") return BARPLACE_mensur;
    if (value == "staff") return BARPLACE_staff;
    if (value == "takt") return BARPLACE_takt;
    LogWarning("Unsupported value '%s' for data.BARPLACE", value.c_str());
    return BARPLACE_NONE;
}

std::string AttConverter::BarrenditionToStr(data_BARRENDITION data) 
{
    std::string value;
    switch(data)
    {
        case BARRENDITION_dashed : value = "dashed"; break;
        case BARRENDITION_dotted : value = "dotted"; break;
        case BARRENDITION_dbl : value = "dbl"; break;
        case BARRENDITION_dbldashed : value = "dbldashed"; break;
        case BARRENDITION_dbldotted : value = "dbldotted"; break;
        case BARRENDITION_end : value = "end"; break;
        case BARRENDITION_invis : value = "invis"; break;
        case BARRENDITION_rptstart : value = "rptstart"; break;
        case BARRENDITION_rptboth : value = "rptboth"; break;
        case BARRENDITION_rptend : value = "rptend"; break;
        case BARRENDITION_single : value = "single"; break;
        default:
            LogWarning("Unknown value '%d' for data.BARRENDITION", data);
            value = "";
            break;
    }
    return value;
}

data_BARRENDITION AttConverter::StrToBarrendition(std::string value)
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

std::string AttConverter::BetypeToStr(data_BETYPE data) 
{
    std::string value;
    switch(data)
    {
        case BETYPE_byte : value = "byte"; break;
        case BETYPE_smil : value = "smil"; break;
        case BETYPE_midi : value = "midi"; break;
        case BETYPE_mmc : value = "mmc"; break;
        case BETYPE_mtc : value = "mtc"; break;
        case BETYPE_smpte_25 : value = "smpte-25"; break;
        case BETYPE_smpte_24 : value = "smpte-24"; break;
        case BETYPE_smpte_df30 : value = "smpte-df30"; break;
        case BETYPE_smpte_ndf30 : value = "smpte-ndf30"; break;
        case BETYPE_smpte_df29_97 : value = "smpte-df29.97"; break;
        case BETYPE_smpte_ndf29_97 : value = "smpte-ndf29.97"; break;
        case BETYPE_tcf : value = "tcf"; break;
        case BETYPE_time : value = "time"; break;
        default:
            LogWarning("Unknown value '%d' for data.BETYPE", data);
            value = "";
            break;
    }
    return value;
}

data_BETYPE AttConverter::StrToBetype(std::string value)
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

std::string AttConverter::BooleanToStr(data_BOOLEAN data) 
{
    std::string value;
    switch(data)
    {
        case BOOLEAN_true : value = "true"; break;
        case BOOLEAN_false : value = "false"; break;
        default:
            LogWarning("Unknown value '%d' for data.BOOLEAN", data);
            value = "";
            break;
    }
    return value;
}

data_BOOLEAN AttConverter::StrToBoolean(std::string value)
{
    if (value == "true") return BOOLEAN_true;
    if (value == "false") return BOOLEAN_false;
    LogWarning("Unsupported value '%s' for data.BOOLEAN", value.c_str());
    return BOOLEAN_NONE;
}

std::string AttConverter::CertaintyToStr(data_CERTAINTY data) 
{
    std::string value;
    switch(data)
    {
        case CERTAINTY_high : value = "high"; break;
        case CERTAINTY_medium : value = "medium"; break;
        case CERTAINTY_low : value = "low"; break;
        case CERTAINTY_unknown : value = "unknown"; break;
        default:
            LogWarning("Unknown value '%d' for data.CERTAINTY", data);
            value = "";
            break;
    }
    return value;
}

data_CERTAINTY AttConverter::StrToCertainty(std::string value)
{
    if (value == "high") return CERTAINTY_high;
    if (value == "medium") return CERTAINTY_medium;
    if (value == "low") return CERTAINTY_low;
    if (value == "unknown") return CERTAINTY_unknown;
    LogWarning("Unsupported value '%s' for data.CERTAINTY", value.c_str());
    return CERTAINTY_NONE;
}

std::string AttConverter::ClefshapeToStr(data_CLEFSHAPE data) 
{
    std::string value;
    switch(data)
    {
        case CLEFSHAPE_G : value = "G"; break;
        case CLEFSHAPE_GG : value = "GG"; break;
        case CLEFSHAPE_F : value = "F"; break;
        case CLEFSHAPE_C : value = "C"; break;
        case CLEFSHAPE_perc : value = "perc"; break;
        case CLEFSHAPE_TAB : value = "TAB"; break;
        default:
            LogWarning("Unknown value '%d' for data.CLEFSHAPE", data);
            value = "";
            break;
    }
    return value;
}

data_CLEFSHAPE AttConverter::StrToClefshape(std::string value)
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

std::string AttConverter::ClusterToStr(data_CLUSTER data) 
{
    std::string value;
    switch(data)
    {
        case CLUSTER_white : value = "white"; break;
        case CLUSTER_black : value = "black"; break;
        case CLUSTER_chromatic : value = "chromatic"; break;
        default:
            LogWarning("Unknown value '%d' for data.CLUSTER", data);
            value = "";
            break;
    }
    return value;
}

data_CLUSTER AttConverter::StrToCluster(std::string value)
{
    if (value == "white") return CLUSTER_white;
    if (value == "black") return CLUSTER_black;
    if (value == "chromatic") return CLUSTER_chromatic;
    LogWarning("Unsupported value '%s' for data.CLUSTER", value.c_str());
    return CLUSTER_NONE;
}

std::string AttConverter::ColornamesToStr(data_COLORNAMES data) 
{
    std::string value;
    switch(data)
    {
        case COLORNAMES_aqua : value = "aqua"; break;
        case COLORNAMES_black : value = "black"; break;
        case COLORNAMES_blue : value = "blue"; break;
        case COLORNAMES_fuchsia : value = "fuchsia"; break;
        case COLORNAMES_gray : value = "gray"; break;
        case COLORNAMES_green : value = "green"; break;
        case COLORNAMES_lime : value = "lime"; break;
        case COLORNAMES_maroon : value = "maroon"; break;
        case COLORNAMES_navy : value = "navy"; break;
        case COLORNAMES_olive : value = "olive"; break;
        case COLORNAMES_purple : value = "purple"; break;
        case COLORNAMES_red : value = "red"; break;
        case COLORNAMES_silver : value = "silver"; break;
        case COLORNAMES_teal : value = "teal"; break;
        case COLORNAMES_white : value = "white"; break;
        case COLORNAMES_yellow : value = "yellow"; break;
        default:
            LogWarning("Unknown value '%d' for data.COLORNAMES", data);
            value = "";
            break;
    }
    return value;
}

data_COLORNAMES AttConverter::StrToColornames(std::string value)
{
    if (value == "aqua") return COLORNAMES_aqua;
    if (value == "black") return COLORNAMES_black;
    if (value == "blue") return COLORNAMES_blue;
    if (value == "fuchsia") return COLORNAMES_fuchsia;
    if (value == "gray") return COLORNAMES_gray;
    if (value == "green") return COLORNAMES_green;
    if (value == "lime") return COLORNAMES_lime;
    if (value == "maroon") return COLORNAMES_maroon;
    if (value == "navy") return COLORNAMES_navy;
    if (value == "olive") return COLORNAMES_olive;
    if (value == "purple") return COLORNAMES_purple;
    if (value == "red") return COLORNAMES_red;
    if (value == "silver") return COLORNAMES_silver;
    if (value == "teal") return COLORNAMES_teal;
    if (value == "white") return COLORNAMES_white;
    if (value == "yellow") return COLORNAMES_yellow;
    LogWarning("Unsupported value '%s' for data.COLORNAMES", value.c_str());
    return COLORNAMES_NONE;
}

std::string AttConverter::CurverenditionToStr(data_CURVERENDITION data) 
{
    std::string value;
    switch(data)
    {
        case CURVERENDITION_narrow : value = "narrow"; break;
        case CURVERENDITION_medium : value = "medium"; break;
        case CURVERENDITION_wide : value = "wide"; break;
        case CURVERENDITION_dashed : value = "dashed"; break;
        case CURVERENDITION_dotted : value = "dotted"; break;
        default:
            LogWarning("Unknown value '%d' for data.CURVERENDITION", data);
            value = "";
            break;
    }
    return value;
}

data_CURVERENDITION AttConverter::StrToCurverendition(std::string value)
{
    if (value == "narrow") return CURVERENDITION_narrow;
    if (value == "medium") return CURVERENDITION_medium;
    if (value == "wide") return CURVERENDITION_wide;
    if (value == "dashed") return CURVERENDITION_dashed;
    if (value == "dotted") return CURVERENDITION_dotted;
    LogWarning("Unsupported value '%s' for data.CURVERENDITION", value.c_str());
    return CURVERENDITION_NONE;
}

std::string AttConverter::EnclosureToStr(data_ENCLOSURE data) 
{
    std::string value;
    switch(data)
    {
        case ENCLOSURE_paren : value = "paren"; break;
        case ENCLOSURE_brack : value = "brack"; break;
        default:
            LogWarning("Unknown value '%d' for data.ENCLOSURE", data);
            value = "";
            break;
    }
    return value;
}

data_ENCLOSURE AttConverter::StrToEnclosure(std::string value)
{
    if (value == "paren") return ENCLOSURE_paren;
    if (value == "brack") return ENCLOSURE_brack;
    LogWarning("Unsupported value '%s' for data.ENCLOSURE", value.c_str());
    return ENCLOSURE_NONE;
}

std::string AttConverter::FontstyleToStr(data_FONTSTYLE data) 
{
    std::string value;
    switch(data)
    {
        case FONTSTYLE_italic : value = "italic"; break;
        case FONTSTYLE_normal : value = "normal"; break;
        case FONTSTYLE_oblique : value = "oblique"; break;
        default:
            LogWarning("Unknown value '%d' for data.FONTSTYLE", data);
            value = "";
            break;
    }
    return value;
}

data_FONTSTYLE AttConverter::StrToFontstyle(std::string value)
{
    if (value == "italic") return FONTSTYLE_italic;
    if (value == "normal") return FONTSTYLE_normal;
    if (value == "oblique") return FONTSTYLE_oblique;
    LogWarning("Unsupported value '%s' for data.FONTSTYLE", value.c_str());
    return FONTSTYLE_NONE;
}

std::string AttConverter::FontweightToStr(data_FONTWEIGHT data) 
{
    std::string value;
    switch(data)
    {
        case FONTWEIGHT_bold : value = "bold"; break;
        case FONTWEIGHT_normal : value = "normal"; break;
        default:
            LogWarning("Unknown value '%d' for data.FONTWEIGHT", data);
            value = "";
            break;
    }
    return value;
}

data_FONTWEIGHT AttConverter::StrToFontweight(std::string value)
{
    if (value == "bold") return FONTWEIGHT_bold;
    if (value == "normal") return FONTWEIGHT_normal;
    LogWarning("Unsupported value '%s' for data.FONTWEIGHT", value.c_str());
    return FONTWEIGHT_NONE;
}

std::string AttConverter::GlissandoToStr(data_GLISSANDO data) 
{
    std::string value;
    switch(data)
    {
        case GLISSANDO_i : value = "i"; break;
        case GLISSANDO_m : value = "m"; break;
        case GLISSANDO_t : value = "t"; break;
        default:
            LogWarning("Unknown value '%d' for data.GLISSANDO", data);
            value = "";
            break;
    }
    return value;
}

data_GLISSANDO AttConverter::StrToGlissando(std::string value)
{
    if (value == "i") return GLISSANDO_i;
    if (value == "m") return GLISSANDO_m;
    if (value == "t") return GLISSANDO_t;
    LogWarning("Unsupported value '%s' for data.GLISSANDO", value.c_str());
    return GLISSANDO_NONE;
}

std::string AttConverter::GraceToStr(data_GRACE data) 
{
    std::string value;
    switch(data)
    {
        case GRACE_acc : value = "acc"; break;
        case GRACE_unacc : value = "unacc"; break;
        case GRACE_unknown : value = "unknown"; break;
        default:
            LogWarning("Unknown value '%d' for data.GRACE", data);
            value = "";
            break;
    }
    return value;
}

data_GRACE AttConverter::StrToGrace(std::string value)
{
    if (value == "acc") return GRACE_acc;
    if (value == "unacc") return GRACE_unacc;
    if (value == "unknown") return GRACE_unknown;
    LogWarning("Unsupported value '%s' for data.GRACE", value.c_str());
    return GRACE_NONE;
}

std::string AttConverter::HorizontalalignmentToStr(data_HORIZONTALALIGNMENT data) 
{
    std::string value;
    switch(data)
    {
        case HORIZONTALALIGNMENT_left : value = "left"; break;
        case HORIZONTALALIGNMENT_right : value = "right"; break;
        case HORIZONTALALIGNMENT_center : value = "center"; break;
        case HORIZONTALALIGNMENT_justify : value = "justify"; break;
        default:
            LogWarning("Unknown value '%d' for data.HORIZONTALALIGNMENT", data);
            value = "";
            break;
    }
    return value;
}

data_HORIZONTALALIGNMENT AttConverter::StrToHorizontalalignment(std::string value)
{
    if (value == "left") return HORIZONTALALIGNMENT_left;
    if (value == "right") return HORIZONTALALIGNMENT_right;
    if (value == "center") return HORIZONTALALIGNMENT_center;
    if (value == "justify") return HORIZONTALALIGNMENT_justify;
    LogWarning("Unsupported value '%s' for data.HORIZONTALALIGNMENT", value.c_str());
    return HORIZONTALALIGNMENT_NONE;
}

std::string AttConverter::IneumeformToStr(data_INEUMEFORM data) 
{
    std::string value;
    switch(data)
    {
        case INEUMEFORM_liquescent1 : value = "liquescent1"; break;
        case INEUMEFORM_liquescent2 : value = "liquescent2"; break;
        case INEUMEFORM_tied : value = "tied"; break;
        case INEUMEFORM_tiedliquescent1 : value = "tiedliquescent1"; break;
        case INEUMEFORM_tiedliquescent2 : value = "tiedliquescent2"; break;
        default:
            LogWarning("Unknown value '%d' for data.INEUMEFORM", data);
            value = "";
            break;
    }
    return value;
}

data_INEUMEFORM AttConverter::StrToIneumeform(std::string value)
{
    if (value == "liquescent1") return INEUMEFORM_liquescent1;
    if (value == "liquescent2") return INEUMEFORM_liquescent2;
    if (value == "tied") return INEUMEFORM_tied;
    if (value == "tiedliquescent1") return INEUMEFORM_tiedliquescent1;
    if (value == "tiedliquescent2") return INEUMEFORM_tiedliquescent2;
    LogWarning("Unsupported value '%s' for data.INEUMEFORM", value.c_str());
    return INEUMEFORM_NONE;
}

std::string AttConverter::IneumenameToStr(data_INEUMENAME data) 
{
    std::string value;
    switch(data)
    {
        case INEUMENAME_pessubpunctis : value = "pessubpunctis"; break;
        case INEUMENAME_climacus : value = "climacus"; break;
        case INEUMENAME_scandicus : value = "scandicus"; break;
        case INEUMENAME_bistropha : value = "bistropha"; break;
        case INEUMENAME_tristropha : value = "tristropha"; break;
        case INEUMENAME_pressusminor : value = "pressusminor"; break;
        case INEUMENAME_pressusmaior : value = "pressusmaior"; break;
        default:
            LogWarning("Unknown value '%d' for data.INEUMENAME", data);
            value = "";
            break;
    }
    return value;
}

data_INEUMENAME AttConverter::StrToIneumename(std::string value)
{
    if (value == "pessubpunctis") return INEUMENAME_pessubpunctis;
    if (value == "climacus") return INEUMENAME_climacus;
    if (value == "scandicus") return INEUMENAME_scandicus;
    if (value == "bistropha") return INEUMENAME_bistropha;
    if (value == "tristropha") return INEUMENAME_tristropha;
    if (value == "pressusminor") return INEUMENAME_pressusminor;
    if (value == "pressusmaior") return INEUMENAME_pressusmaior;
    LogWarning("Unsupported value '%s' for data.INEUMENAME", value.c_str());
    return INEUMENAME_NONE;
}

std::string AttConverter::LayerschemeToStr(data_LAYERSCHEME data) 
{
    std::string value;
    switch(data)
    {
        case LAYERSCHEME_1 : value = "1"; break;
        case LAYERSCHEME_2o : value = "2o"; break;
        case LAYERSCHEME_2f : value = "2f"; break;
        case LAYERSCHEME_3o : value = "3o"; break;
        case LAYERSCHEME_3f : value = "3f"; break;
        default:
            LogWarning("Unknown value '%d' for data.LAYERSCHEME", data);
            value = "";
            break;
    }
    return value;
}

data_LAYERSCHEME AttConverter::StrToLayerscheme(std::string value)
{
    if (value == "1") return LAYERSCHEME_1;
    if (value == "2o") return LAYERSCHEME_2o;
    if (value == "2f") return LAYERSCHEME_2f;
    if (value == "3o") return LAYERSCHEME_3o;
    if (value == "3f") return LAYERSCHEME_3f;
    LogWarning("Unsupported value '%s' for data.LAYERSCHEME", value.c_str());
    return LAYERSCHEME_NONE;
}

std::string AttConverter::LigatureformToStr(data_LIGATUREFORM data) 
{
    std::string value;
    switch(data)
    {
        case LIGATUREFORM_recta : value = "recta"; break;
        case LIGATUREFORM_obliqua : value = "obliqua"; break;
        default:
            LogWarning("Unknown value '%d' for data.LIGATUREFORM", data);
            value = "";
            break;
    }
    return value;
}

data_LIGATUREFORM AttConverter::StrToLigatureform(std::string value)
{
    if (value == "recta") return LIGATUREFORM_recta;
    if (value == "obliqua") return LIGATUREFORM_obliqua;
    LogWarning("Unsupported value '%s' for data.LIGATUREFORM", value.c_str());
    return LIGATUREFORM_NONE;
}

std::string AttConverter::LineformToStr(data_LINEFORM data) 
{
    std::string value;
    switch(data)
    {
        case LINEFORM_dashed : value = "dashed"; break;
        case LINEFORM_dotted : value = "dotted"; break;
        case LINEFORM_solid : value = "solid"; break;
        case LINEFORM_wavy : value = "wavy"; break;
        default:
            LogWarning("Unknown value '%d' for data.LINEFORM", data);
            value = "";
            break;
    }
    return value;
}

data_LINEFORM AttConverter::StrToLineform(std::string value)
{
    if (value == "dashed") return LINEFORM_dashed;
    if (value == "dotted") return LINEFORM_dotted;
    if (value == "solid") return LINEFORM_solid;
    if (value == "wavy") return LINEFORM_wavy;
    LogWarning("Unsupported value '%s' for data.LINEFORM", value.c_str());
    return LINEFORM_NONE;
}

std::string AttConverter::LinestartendsymbolToStr(data_LINESTARTENDSYMBOL data) 
{
    std::string value;
    switch(data)
    {
        case LINESTARTENDSYMBOL_angledown : value = "angledown"; break;
        case LINESTARTENDSYMBOL_angleup : value = "angleup"; break;
        case LINESTARTENDSYMBOL_angleright : value = "angleright"; break;
        case LINESTARTENDSYMBOL_angleleft : value = "angleleft"; break;
        case LINESTARTENDSYMBOL_arrow : value = "arrow"; break;
        case LINESTARTENDSYMBOL_arrowopen : value = "arrowopen"; break;
        case LINESTARTENDSYMBOL_arrowwhite : value = "arrowwhite"; break;
        case LINESTARTENDSYMBOL_harpoonleft : value = "harpoonleft"; break;
        case LINESTARTENDSYMBOL_harpoonright : value = "harpoonright"; break;
        case LINESTARTENDSYMBOL_none : value = "none"; break;
        default:
            LogWarning("Unknown value '%d' for data.LINESTARTENDSYMBOL", data);
            value = "";
            break;
    }
    return value;
}

data_LINESTARTENDSYMBOL AttConverter::StrToLinestartendsymbol(std::string value)
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

std::string AttConverter::LinewidthtermToStr(data_LINEWIDTHTERM data) 
{
    std::string value;
    switch(data)
    {
        case LINEWIDTHTERM_narrow : value = "narrow"; break;
        case LINEWIDTHTERM_medium : value = "medium"; break;
        case LINEWIDTHTERM_wide : value = "wide"; break;
        default:
            LogWarning("Unknown value '%d' for data.LINEWIDTHTERM", data);
            value = "";
            break;
    }
    return value;
}

data_LINEWIDTHTERM AttConverter::StrToLinewidthterm(std::string value)
{
    if (value == "narrow") return LINEWIDTHTERM_narrow;
    if (value == "medium") return LINEWIDTHTERM_medium;
    if (value == "wide") return LINEWIDTHTERM_wide;
    LogWarning("Unsupported value '%s' for data.LINEWIDTHTERM", value.c_str());
    return LINEWIDTHTERM_NONE;
}

std::string AttConverter::MelodicfunctionToStr(data_MELODICFUNCTION data) 
{
    std::string value;
    switch(data)
    {
        case MELODICFUNCTION_aln : value = "aln"; break;
        case MELODICFUNCTION_ant : value = "ant"; break;
        case MELODICFUNCTION_app : value = "app"; break;
        case MELODICFUNCTION_apt : value = "apt"; break;
        case MELODICFUNCTION_arp : value = "arp"; break;
        case MELODICFUNCTION_arp7 : value = "arp7"; break;
        case MELODICFUNCTION_aun : value = "aun"; break;
        case MELODICFUNCTION_chg : value = "chg"; break;
        case MELODICFUNCTION_cln : value = "cln"; break;
        case MELODICFUNCTION_ct : value = "ct"; break;
        case MELODICFUNCTION_ct7 : value = "ct7"; break;
        case MELODICFUNCTION_cun : value = "cun"; break;
        case MELODICFUNCTION_cup : value = "cup"; break;
        case MELODICFUNCTION_et : value = "et"; break;
        case MELODICFUNCTION_ln : value = "ln"; break;
        case MELODICFUNCTION_ped : value = "ped"; break;
        case MELODICFUNCTION_rep : value = "rep"; break;
        case MELODICFUNCTION_ret : value = "ret"; break;
        case MELODICFUNCTION_23ret : value = "23ret"; break;
        case MELODICFUNCTION_78ret : value = "78ret"; break;
        case MELODICFUNCTION_sus : value = "sus"; break;
        case MELODICFUNCTION_43sus : value = "43sus"; break;
        case MELODICFUNCTION_98sus : value = "98sus"; break;
        case MELODICFUNCTION_76sus : value = "76sus"; break;
        case MELODICFUNCTION_un : value = "un"; break;
        case MELODICFUNCTION_un7 : value = "un7"; break;
        case MELODICFUNCTION_upt : value = "upt"; break;
        case MELODICFUNCTION_upt7 : value = "upt7"; break;
        default:
            LogWarning("Unknown value '%d' for data.MELODICFUNCTION", data);
            value = "";
            break;
    }
    return value;
}

data_MELODICFUNCTION AttConverter::StrToMelodicfunction(std::string value)
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
    LogWarning("Unsupported value '%s' for data.MELODICFUNCTION", value.c_str());
    return MELODICFUNCTION_NONE;
}

std::string AttConverter::MensurationsignToStr(data_MENSURATIONSIGN data) 
{
    std::string value;
    switch(data)
    {
        case MENSURATIONSIGN_C : value = "C"; break;
        case MENSURATIONSIGN_O : value = "O"; break;
        default:
            LogWarning("Unknown value '%d' for data.MENSURATIONSIGN", data);
            value = "";
            break;
    }
    return value;
}

data_MENSURATIONSIGN AttConverter::StrToMensurationsign(std::string value)
{
    if (value == "C") return MENSURATIONSIGN_C;
    if (value == "O") return MENSURATIONSIGN_O;
    LogWarning("Unsupported value '%s' for data.MENSURATIONSIGN", value.c_str());
    return MENSURATIONSIGN_NONE;
}

std::string AttConverter::MetersignToStr(data_METERSIGN data) 
{
    std::string value;
    switch(data)
    {
        case METERSIGN_common : value = "common"; break;
        case METERSIGN_cut : value = "cut"; break;
        default:
            LogWarning("Unknown value '%d' for data.METERSIGN", data);
            value = "";
            break;
    }
    return value;
}

data_METERSIGN AttConverter::StrToMetersign(std::string value)
{
    if (value == "common") return METERSIGN_common;
    if (value == "cut") return METERSIGN_cut;
    LogWarning("Unsupported value '%s' for data.METERSIGN", value.c_str());
    return METERSIGN_NONE;
}

std::string AttConverter::MidinamesToStr(data_MIDINAMES data) 
{
    std::string value;
    switch(data)
    {
        case MIDINAMES_Acoustic_Grand_Piano : value = "Acoustic_Grand_Piano"; break;
        case MIDINAMES_Bright_Acoustic_Piano : value = "Bright_Acoustic_Piano"; break;
        case MIDINAMES_Electric_Grand_Piano : value = "Electric_Grand_Piano"; break;
        case MIDINAMES_Honky_tonk_Piano : value = "Honky-tonk_Piano"; break;
        case MIDINAMES_Electric_Piano_1 : value = "Electric_Piano_1"; break;
        case MIDINAMES_Electric_Piano_2 : value = "Electric_Piano_2"; break;
        case MIDINAMES_Harpsichord : value = "Harpsichord"; break;
        case MIDINAMES_Clavi : value = "Clavi"; break;
        case MIDINAMES_Celesta : value = "Celesta"; break;
        case MIDINAMES_Glockenspiel : value = "Glockenspiel"; break;
        case MIDINAMES_Music_Box : value = "Music_Box"; break;
        case MIDINAMES_Vibraphone : value = "Vibraphone"; break;
        case MIDINAMES_Marimba : value = "Marimba"; break;
        case MIDINAMES_Xylophone : value = "Xylophone"; break;
        case MIDINAMES_Tubular_Bells : value = "Tubular_Bells"; break;
        case MIDINAMES_Dulcimer : value = "Dulcimer"; break;
        case MIDINAMES_Drawbar_Organ : value = "Drawbar_Organ"; break;
        case MIDINAMES_Percussive_Organ : value = "Percussive_Organ"; break;
        case MIDINAMES_Rock_Organ : value = "Rock_Organ"; break;
        case MIDINAMES_Church_Organ : value = "Church_Organ"; break;
        case MIDINAMES_Reed_Organ : value = "Reed_Organ"; break;
        case MIDINAMES_Accordion : value = "Accordion"; break;
        case MIDINAMES_Harmonica : value = "Harmonica"; break;
        case MIDINAMES_Tango_Accordion : value = "Tango_Accordion"; break;
        case MIDINAMES_Acoustic_Guitar_nylon : value = "Acoustic_Guitar_nylon"; break;
        case MIDINAMES_Acoustic_Guitar_steel : value = "Acoustic_Guitar_steel"; break;
        case MIDINAMES_Electric_Guitar_jazz : value = "Electric_Guitar_jazz"; break;
        case MIDINAMES_Electric_Guitar_clean : value = "Electric_Guitar_clean"; break;
        case MIDINAMES_Electric_Guitar_muted : value = "Electric_Guitar_muted"; break;
        case MIDINAMES_Overdriven_Guitar : value = "Overdriven_Guitar"; break;
        case MIDINAMES_Distortion_Guitar : value = "Distortion_Guitar"; break;
        case MIDINAMES_Guitar_harmonics : value = "Guitar_harmonics"; break;
        case MIDINAMES_Acoustic_Bass : value = "Acoustic_Bass"; break;
        case MIDINAMES_Electric_Bass_finger : value = "Electric_Bass_finger"; break;
        case MIDINAMES_Electric_Bass_pick : value = "Electric_Bass_pick"; break;
        case MIDINAMES_Fretless_Bass : value = "Fretless_Bass"; break;
        case MIDINAMES_Slap_Bass_1 : value = "Slap_Bass_1"; break;
        case MIDINAMES_Slap_Bass_2 : value = "Slap_Bass_2"; break;
        case MIDINAMES_Synth_Bass_1 : value = "Synth_Bass_1"; break;
        case MIDINAMES_Synth_Bass_2 : value = "Synth_Bass_2"; break;
        case MIDINAMES_Violin : value = "Violin"; break;
        case MIDINAMES_Viola : value = "Viola"; break;
        case MIDINAMES_Cello : value = "Cello"; break;
        case MIDINAMES_Contrabass : value = "Contrabass"; break;
        case MIDINAMES_Tremolo_Strings : value = "Tremolo_Strings"; break;
        case MIDINAMES_Pizzicato_Strings : value = "Pizzicato_Strings"; break;
        case MIDINAMES_Orchestral_Harp : value = "Orchestral_Harp"; break;
        case MIDINAMES_Timpani : value = "Timpani"; break;
        case MIDINAMES_String_Ensemble_1 : value = "String_Ensemble_1"; break;
        case MIDINAMES_String_Ensemble_2 : value = "String_Ensemble_2"; break;
        case MIDINAMES_SynthStrings_1 : value = "SynthStrings_1"; break;
        case MIDINAMES_SynthStrings_2 : value = "SynthStrings_2"; break;
        case MIDINAMES_Choir_Aahs : value = "Choir_Aahs"; break;
        case MIDINAMES_Voice_Oohs : value = "Voice_Oohs"; break;
        case MIDINAMES_Synth_Voice : value = "Synth_Voice"; break;
        case MIDINAMES_Orchestra_Hit : value = "Orchestra_Hit"; break;
        case MIDINAMES_Trumpet : value = "Trumpet"; break;
        case MIDINAMES_Trombone : value = "Trombone"; break;
        case MIDINAMES_Tuba : value = "Tuba"; break;
        case MIDINAMES_Muted_Trumpet : value = "Muted_Trumpet"; break;
        case MIDINAMES_French_Horn : value = "French_Horn"; break;
        case MIDINAMES_Brass_Section : value = "Brass_Section"; break;
        case MIDINAMES_SynthBrass_1 : value = "SynthBrass_1"; break;
        case MIDINAMES_SynthBrass_2 : value = "SynthBrass_2"; break;
        case MIDINAMES_Soprano_Sax : value = "Soprano_Sax"; break;
        case MIDINAMES_Alto_Sax : value = "Alto_Sax"; break;
        case MIDINAMES_Tenor_Sax : value = "Tenor_Sax"; break;
        case MIDINAMES_Baritone_Sax : value = "Baritone_Sax"; break;
        case MIDINAMES_Oboe : value = "Oboe"; break;
        case MIDINAMES_English_Horn : value = "English_Horn"; break;
        case MIDINAMES_Bassoon : value = "Bassoon"; break;
        case MIDINAMES_Clarinet : value = "Clarinet"; break;
        case MIDINAMES_Piccolo : value = "Piccolo"; break;
        case MIDINAMES_Flute : value = "Flute"; break;
        case MIDINAMES_Recorder : value = "Recorder"; break;
        case MIDINAMES_Pan_Flute : value = "Pan_Flute"; break;
        case MIDINAMES_Blown_Bottle : value = "Blown_Bottle"; break;
        case MIDINAMES_Shakuhachi : value = "Shakuhachi"; break;
        case MIDINAMES_Whistle : value = "Whistle"; break;
        case MIDINAMES_Ocarina : value = "Ocarina"; break;
        case MIDINAMES_Lead_1_square : value = "Lead_1_square"; break;
        case MIDINAMES_Lead_2_sawtooth : value = "Lead_2_sawtooth"; break;
        case MIDINAMES_Lead_3_calliope : value = "Lead_3_calliope"; break;
        case MIDINAMES_Lead_4_chiff : value = "Lead_4_chiff"; break;
        case MIDINAMES_Lead_5_charang : value = "Lead_5_charang"; break;
        case MIDINAMES_Lead_6_voice : value = "Lead_6_voice"; break;
        case MIDINAMES_Lead_7_fifths : value = "Lead_7_fifths"; break;
        case MIDINAMES_Lead_8_bass_and_lead : value = "Lead_8_bass_and_lead"; break;
        case MIDINAMES_Pad_1_new_age : value = "Pad_1_new_age"; break;
        case MIDINAMES_Pad_2_warm : value = "Pad_2_warm"; break;
        case MIDINAMES_Pad_3_polysynth : value = "Pad_3_polysynth"; break;
        case MIDINAMES_Pad_4_choir : value = "Pad_4_choir"; break;
        case MIDINAMES_Pad_5_bowed : value = "Pad_5_bowed"; break;
        case MIDINAMES_Pad_6_metallic : value = "Pad_6_metallic"; break;
        case MIDINAMES_Pad_7_halo : value = "Pad_7_halo"; break;
        case MIDINAMES_Pad_8_sweep : value = "Pad_8_sweep"; break;
        case MIDINAMES_FX_1_rain : value = "FX_1_rain"; break;
        case MIDINAMES_FX_2_soundtrack : value = "FX_2_soundtrack"; break;
        case MIDINAMES_FX_3_crystal : value = "FX_3_crystal"; break;
        case MIDINAMES_FX_4_atmosphere : value = "FX_4_atmosphere"; break;
        case MIDINAMES_FX_5_brightness : value = "FX_5_brightness"; break;
        case MIDINAMES_FX_6_goblins : value = "FX_6_goblins"; break;
        case MIDINAMES_FX_7_echoes : value = "FX_7_echoes"; break;
        case MIDINAMES_FX_8_sci_fi : value = "FX_8_sci-fi"; break;
        case MIDINAMES_Sitar : value = "Sitar"; break;
        case MIDINAMES_Banjo : value = "Banjo"; break;
        case MIDINAMES_Shamisen : value = "Shamisen"; break;
        case MIDINAMES_Koto : value = "Koto"; break;
        case MIDINAMES_Kalimba : value = "Kalimba"; break;
        case MIDINAMES_Bagpipe : value = "Bagpipe"; break;
        case MIDINAMES_Fiddle : value = "Fiddle"; break;
        case MIDINAMES_Shanai : value = "Shanai"; break;
        case MIDINAMES_Tinkle_Bell : value = "Tinkle_Bell"; break;
        case MIDINAMES_Agogo : value = "Agogo"; break;
        case MIDINAMES_Steel_Drums : value = "Steel_Drums"; break;
        case MIDINAMES_Woodblock : value = "Woodblock"; break;
        case MIDINAMES_Taiko_Drum : value = "Taiko_Drum"; break;
        case MIDINAMES_Melodic_Tom : value = "Melodic_Tom"; break;
        case MIDINAMES_Synth_Drum : value = "Synth_Drum"; break;
        case MIDINAMES_Reverse_Cymbal : value = "Reverse_Cymbal"; break;
        case MIDINAMES_Guitar_Fret_Noise : value = "Guitar_Fret_Noise"; break;
        case MIDINAMES_Breath_Noise : value = "Breath_Noise"; break;
        case MIDINAMES_Seashore : value = "Seashore"; break;
        case MIDINAMES_Bird_Tweet : value = "Bird_Tweet"; break;
        case MIDINAMES_Telephone_Ring : value = "Telephone_Ring"; break;
        case MIDINAMES_Helicopter : value = "Helicopter"; break;
        case MIDINAMES_Applause : value = "Applause"; break;
        case MIDINAMES_Gunshot : value = "Gunshot"; break;
        case MIDINAMES_Acoustic_Bass_Drum : value = "Acoustic_Bass_Drum"; break;
        case MIDINAMES_Bass_Drum_1 : value = "Bass_Drum_1"; break;
        case MIDINAMES_Side_Stick : value = "Side_Stick"; break;
        case MIDINAMES_Acoustic_Snare : value = "Acoustic_Snare"; break;
        case MIDINAMES_Hand_Clap : value = "Hand_Clap"; break;
        case MIDINAMES_Electric_Snare : value = "Electric_Snare"; break;
        case MIDINAMES_Low_Floor_Tom : value = "Low_Floor_Tom"; break;
        case MIDINAMES_Closed_Hi_Hat : value = "Closed_Hi_Hat"; break;
        case MIDINAMES_High_Floor_Tom : value = "High_Floor_Tom"; break;
        case MIDINAMES_Pedal_Hi_Hat : value = "Pedal_Hi-Hat"; break;
        case MIDINAMES_Low_Tom : value = "Low_Tom"; break;
        case MIDINAMES_Open_Hi_Hat : value = "Open_Hi-Hat"; break;
        case MIDINAMES_Low_Mid_Tom : value = "Low-Mid_Tom"; break;
        case MIDINAMES_Hi_Mid_Tom : value = "Hi-Mid_Tom"; break;
        case MIDINAMES_Crash_Cymbal_1 : value = "Crash_Cymbal_1"; break;
        case MIDINAMES_High_Tom : value = "High_Tom"; break;
        case MIDINAMES_Ride_Cymbal_1 : value = "Ride_Cymbal_1"; break;
        case MIDINAMES_Chinese_Cymbal : value = "Chinese_Cymbal"; break;
        case MIDINAMES_Ride_Bell : value = "Ride_Bell"; break;
        case MIDINAMES_Tambourine : value = "Tambourine"; break;
        case MIDINAMES_Splash_Cymbal : value = "Splash_Cymbal"; break;
        case MIDINAMES_Cowbell : value = "Cowbell"; break;
        case MIDINAMES_Crash_Cymbal_2 : value = "Crash_Cymbal_2"; break;
        case MIDINAMES_Vibraslap : value = "Vibraslap"; break;
        case MIDINAMES_Ride_Cymbal_2 : value = "Ride_Cymbal_2"; break;
        case MIDINAMES_Hi_Bongo : value = "Hi_Bongo"; break;
        case MIDINAMES_Low_Bongo : value = "Low_Bongo"; break;
        case MIDINAMES_Mute_Hi_Conga : value = "Mute_Hi_Conga"; break;
        case MIDINAMES_Open_Hi_Conga : value = "Open_Hi_Conga"; break;
        case MIDINAMES_Low_Conga : value = "Low_Conga"; break;
        case MIDINAMES_High_Timbale : value = "High_Timbale"; break;
        case MIDINAMES_Low_Timbale : value = "Low_Timbale"; break;
        case MIDINAMES_High_Agogo : value = "High_Agogo"; break;
        case MIDINAMES_Low_Agogo : value = "Low_Agogo"; break;
        case MIDINAMES_Cabasa : value = "Cabasa"; break;
        case MIDINAMES_Maracas : value = "Maracas"; break;
        case MIDINAMES_Short_Whistle : value = "Short_Whistle"; break;
        case MIDINAMES_Long_Whistle : value = "Long_Whistle"; break;
        case MIDINAMES_Short_Guiro : value = "Short_Guiro"; break;
        case MIDINAMES_Long_Guiro : value = "Long_Guiro"; break;
        case MIDINAMES_Claves : value = "Claves"; break;
        case MIDINAMES_Hi_Wood_Block : value = "Hi_Wood_Block"; break;
        case MIDINAMES_Low_Wood_Block : value = "Low_Wood_Block"; break;
        case MIDINAMES_Mute_Cuica : value = "Mute_Cuica"; break;
        case MIDINAMES_Open_Cuica : value = "Open_Cuica"; break;
        case MIDINAMES_Mute_Triangle : value = "Mute_Triangle"; break;
        case MIDINAMES_Open_Triangle : value = "Open_Triangle"; break;
        default:
            LogWarning("Unknown value '%d' for data.MIDINAMES", data);
            value = "";
            break;
    }
    return value;
}

data_MIDINAMES AttConverter::StrToMidinames(std::string value)
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

std::string AttConverter::ModeToStr(data_MODE data) 
{
    std::string value;
    switch(data)
    {
        case MODE_major : value = "major"; break;
        case MODE_minor : value = "minor"; break;
        case MODE_dorian : value = "dorian"; break;
        case MODE_phrygian : value = "phrygian"; break;
        case MODE_lydian : value = "lydian"; break;
        case MODE_mixolydian : value = "mixolydian"; break;
        case MODE_aeolian : value = "aeolian"; break;
        case MODE_locrian : value = "locrian"; break;
        default:
            LogWarning("Unknown value '%d' for data.MODE", data);
            value = "";
            break;
    }
    return value;
}

data_MODE AttConverter::StrToMode(std::string value)
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

std::string AttConverter::NotationtypeToStr(data_NOTATIONTYPE data) 
{
    std::string value;
    switch(data)
    {
        case NOTATIONTYPE_cmn : value = "cmn"; break;
        case NOTATIONTYPE_mensural : value = "mensural"; break;
        case NOTATIONTYPE_mensural_black : value = "mensural.black"; break;
        case NOTATIONTYPE_mensural_white : value = "mensural.white"; break;
        case NOTATIONTYPE_neume : value = "neume"; break;
        case NOTATIONTYPE_tab : value = "tab"; break;
        default:
            LogWarning("Unknown value '%d' for data.NOTATIONTYPE", data);
            value = "";
            break;
    }
    return value;
}

data_NOTATIONTYPE AttConverter::StrToNotationtype(std::string value)
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

std::string AttConverter::NoteheadmodifierListToStr(data_NOTEHEADMODIFIER_list data) 
{
    std::string value;
    switch(data)
    {
        case NOTEHEADMODIFIER_list_slash : value = "slash"; break;
        case NOTEHEADMODIFIER_list_backslash : value = "backslash"; break;
        case NOTEHEADMODIFIER_list_vline : value = "vline"; break;
        case NOTEHEADMODIFIER_list_hline : value = "hline"; break;
        case NOTEHEADMODIFIER_list_centerdot : value = "centerdot"; break;
        case NOTEHEADMODIFIER_list_paren : value = "paren"; break;
        case NOTEHEADMODIFIER_list_brack : value = "brack"; break;
        case NOTEHEADMODIFIER_list_box : value = "box"; break;
        case NOTEHEADMODIFIER_list_circle : value = "circle"; break;
        case NOTEHEADMODIFIER_list_dblwhole : value = "dblwhole"; break;
        default:
            LogWarning("Unknown value '%d' for data.NOTEHEADMODIFIER.list", data);
            value = "";
            break;
    }
    return value;
}

data_NOTEHEADMODIFIER_list AttConverter::StrToNoteheadmodifierList(std::string value)
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

std::string AttConverter::OtherstaffToStr(data_OTHERSTAFF data) 
{
    std::string value;
    switch(data)
    {
        case OTHERSTAFF_above : value = "above"; break;
        case OTHERSTAFF_below : value = "below"; break;
        default:
            LogWarning("Unknown value '%d' for data.OTHERSTAFF", data);
            value = "";
            break;
    }
    return value;
}

data_OTHERSTAFF AttConverter::StrToOtherstaff(std::string value)
{
    if (value == "above") return OTHERSTAFF_above;
    if (value == "below") return OTHERSTAFF_below;
    LogWarning("Unsupported value '%s' for data.OTHERSTAFF", value.c_str());
    return OTHERSTAFF_NONE;
}

std::string AttConverter::PlaceToStr(data_PLACE data) 
{
    std::string value;
    switch(data)
    {
        case PLACE_above : value = "above"; break;
        case PLACE_below : value = "below"; break;
        default:
            LogWarning("Unknown value '%d' for data.PLACE", data);
            value = "";
            break;
    }
    return value;
}

data_PLACE AttConverter::StrToPlace(std::string value)
{
    if (value == "above") return PLACE_above;
    if (value == "below") return PLACE_below;
    LogWarning("Unsupported value '%s' for data.PLACE", value.c_str());
    return PLACE_NONE;
}

std::string AttConverter::RotationdirectionToStr(data_ROTATIONDIRECTION data) 
{
    std::string value;
    switch(data)
    {
        case ROTATIONDIRECTION_none : value = "none"; break;
        case ROTATIONDIRECTION_down : value = "down"; break;
        case ROTATIONDIRECTION_left : value = "left"; break;
        case ROTATIONDIRECTION_ne : value = "ne"; break;
        case ROTATIONDIRECTION_nw : value = "nw"; break;
        case ROTATIONDIRECTION_se : value = "se"; break;
        case ROTATIONDIRECTION_sw : value = "sw"; break;
        default:
            LogWarning("Unknown value '%d' for data.ROTATIONDIRECTION", data);
            value = "";
            break;
    }
    return value;
}

data_ROTATIONDIRECTION AttConverter::StrToRotationdirection(std::string value)
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

std::string AttConverter::SizeToStr(data_SIZE data) 
{
    std::string value;
    switch(data)
    {
        case SIZE_normal : value = "normal"; break;
        case SIZE_cue : value = "cue"; break;
        default:
            LogWarning("Unknown value '%d' for data.SIZE", data);
            value = "";
            break;
    }
    return value;
}

data_SIZE AttConverter::StrToSize(std::string value)
{
    if (value == "normal") return SIZE_normal;
    if (value == "cue") return SIZE_cue;
    LogWarning("Unsupported value '%s' for data.SIZE", value.c_str());
    return SIZE_NONE;
}

std::string AttConverter::StaffrelToStr(data_STAFFREL data) 
{
    std::string value;
    switch(data)
    {
        case STAFFREL_above : value = "above"; break;
        case STAFFREL_below : value = "below"; break;
        case STAFFREL_within : value = "within"; break;
        default:
            LogWarning("Unknown value '%d' for data.STAFFREL", data);
            value = "";
            break;
    }
    return value;
}

data_STAFFREL AttConverter::StrToStaffrel(std::string value)
{
    if (value == "above") return STAFFREL_above;
    if (value == "below") return STAFFREL_below;
    if (value == "within") return STAFFREL_within;
    LogWarning("Unsupported value '%s' for data.STAFFREL", value.c_str());
    return STAFFREL_NONE;
}

std::string AttConverter::StemmodifierToStr(data_STEMMODIFIER data) 
{
    std::string value;
    switch(data)
    {
        case STEMMODIFIER_none : value = "none"; break;
        case STEMMODIFIER_1slash : value = "1slash"; break;
        case STEMMODIFIER_2slash : value = "2slash"; break;
        case STEMMODIFIER_3slash : value = "3slash"; break;
        case STEMMODIFIER_4slash : value = "4slash"; break;
        case STEMMODIFIER_5slash : value = "5slash"; break;
        case STEMMODIFIER_6slash : value = "6slash"; break;
        case STEMMODIFIER_sprech : value = "sprech"; break;
        case STEMMODIFIER_z : value = "z"; break;
        default:
            LogWarning("Unknown value '%d' for data.STEMMODIFIER", data);
            value = "";
            break;
    }
    return value;
}

data_STEMMODIFIER AttConverter::StrToStemmodifier(std::string value)
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

std::string AttConverter::StempositionToStr(data_STEMPOSITION data) 
{
    std::string value;
    switch(data)
    {
        case STEMPOSITION_left : value = "left"; break;
        case STEMPOSITION_right : value = "right"; break;
        case STEMPOSITION_center : value = "center"; break;
        default:
            LogWarning("Unknown value '%d' for data.STEMPOSITION", data);
            value = "";
            break;
    }
    return value;
}

data_STEMPOSITION AttConverter::StrToStemposition(std::string value)
{
    if (value == "left") return STEMPOSITION_left;
    if (value == "right") return STEMPOSITION_right;
    if (value == "center") return STEMPOSITION_center;
    LogWarning("Unsupported value '%s' for data.STEMPOSITION", value.c_str());
    return STEMPOSITION_NONE;
}

std::string AttConverter::TemperamentToStr(data_TEMPERAMENT data) 
{
    std::string value;
    switch(data)
    {
        case TEMPERAMENT_equal : value = "equal"; break;
        case TEMPERAMENT_just : value = "just"; break;
        case TEMPERAMENT_mean : value = "mean"; break;
        case TEMPERAMENT_pythagorean : value = "pythagorean"; break;
        default:
            LogWarning("Unknown value '%d' for data.TEMPERAMENT", data);
            value = "";
            break;
    }
    return value;
}

data_TEMPERAMENT AttConverter::StrToTemperament(std::string value)
{
    if (value == "equal") return TEMPERAMENT_equal;
    if (value == "just") return TEMPERAMENT_just;
    if (value == "mean") return TEMPERAMENT_mean;
    if (value == "pythagorean") return TEMPERAMENT_pythagorean;
    LogWarning("Unsupported value '%s' for data.TEMPERAMENT", value.c_str());
    return TEMPERAMENT_NONE;
}

std::string AttConverter::TextrenditionlistToStr(data_TEXTRENDITIONLIST data) 
{
    std::string value;
    switch(data)
    {
        case TEXTRENDITIONLIST_xx_small : value = "xx-small"; break;
        case TEXTRENDITIONLIST_x_small : value = "x-small"; break;
        case TEXTRENDITIONLIST_small : value = "small"; break;
        case TEXTRENDITIONLIST_medium : value = "medium"; break;
        case TEXTRENDITIONLIST_large : value = "large"; break;
        case TEXTRENDITIONLIST_x_large : value = "x-large"; break;
        case TEXTRENDITIONLIST_xx_large : value = "xx-large"; break;
        case TEXTRENDITIONLIST_smaller : value = "smaller"; break;
        case TEXTRENDITIONLIST_larger : value = "larger"; break;
        case TEXTRENDITIONLIST_italic : value = "italic"; break;
        case TEXTRENDITIONLIST_oblique : value = "oblique"; break;
        case TEXTRENDITIONLIST_smcaps : value = "smcaps"; break;
        case TEXTRENDITIONLIST_bold : value = "bold"; break;
        case TEXTRENDITIONLIST_bolder : value = "bolder"; break;
        case TEXTRENDITIONLIST_lighter : value = "lighter"; break;
        case TEXTRENDITIONLIST_box : value = "box"; break;
        case TEXTRENDITIONLIST_circle : value = "circle"; break;
        case TEXTRENDITIONLIST_dbox : value = "dbox"; break;
        case TEXTRENDITIONLIST_tbox : value = "tbox"; break;
        case TEXTRENDITIONLIST_bslash : value = "bslash"; break;
        case TEXTRENDITIONLIST_fslash : value = "fslash"; break;
        case TEXTRENDITIONLIST_line_through : value = "line-through"; break;
        case TEXTRENDITIONLIST_none : value = "none"; break;
        case TEXTRENDITIONLIST_overline : value = "overline"; break;
        case TEXTRENDITIONLIST_overstrike : value = "overstrike"; break;
        case TEXTRENDITIONLIST_strike : value = "strike"; break;
        case TEXTRENDITIONLIST_sub : value = "sub"; break;
        case TEXTRENDITIONLIST_sup : value = "sup"; break;
        case TEXTRENDITIONLIST_underline : value = "underline"; break;
        case TEXTRENDITIONLIST_ltr : value = "ltr"; break;
        case TEXTRENDITIONLIST_rtl : value = "rtl"; break;
        case TEXTRENDITIONLIST_lro : value = "lro"; break;
        case TEXTRENDITIONLIST_rlo : value = "rlo"; break;
        default:
            LogWarning("Unknown value '%d' for data.TEXTRENDITIONLIST", data);
            value = "";
            break;
    }
    return value;
}

data_TEXTRENDITIONLIST AttConverter::StrToTextrenditionlist(std::string value)
{
    if (value == "xx-small") return TEXTRENDITIONLIST_xx_small;
    if (value == "x-small") return TEXTRENDITIONLIST_x_small;
    if (value == "small") return TEXTRENDITIONLIST_small;
    if (value == "medium") return TEXTRENDITIONLIST_medium;
    if (value == "large") return TEXTRENDITIONLIST_large;
    if (value == "x-large") return TEXTRENDITIONLIST_x_large;
    if (value == "xx-large") return TEXTRENDITIONLIST_xx_large;
    if (value == "smaller") return TEXTRENDITIONLIST_smaller;
    if (value == "larger") return TEXTRENDITIONLIST_larger;
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
    if (value == "underline") return TEXTRENDITIONLIST_underline;
    if (value == "ltr") return TEXTRENDITIONLIST_ltr;
    if (value == "rtl") return TEXTRENDITIONLIST_rtl;
    if (value == "lro") return TEXTRENDITIONLIST_lro;
    if (value == "rlo") return TEXTRENDITIONLIST_rlo;
    LogWarning("Unsupported value '%s' for data.TEXTRENDITIONLIST", value.c_str());
    return TEXTRENDITIONLIST_NONE;
}

std::string AttConverter::UneumeformToStr(data_UNEUMEFORM data) 
{
    std::string value;
    switch(data)
    {
        case UNEUMEFORM_liquescent1 : value = "liquescent1"; break;
        case UNEUMEFORM_liquescent2 : value = "liquescent2"; break;
        case UNEUMEFORM_liquescent3 : value = "liquescent3"; break;
        case UNEUMEFORM_quilismatic : value = "quilismatic"; break;
        case UNEUMEFORM_rectangular : value = "rectangular"; break;
        case UNEUMEFORM_rhombic : value = "rhombic"; break;
        case UNEUMEFORM_tied : value = "tied"; break;
        default:
            LogWarning("Unknown value '%d' for data.UNEUMEFORM", data);
            value = "";
            break;
    }
    return value;
}

data_UNEUMEFORM AttConverter::StrToUneumeform(std::string value)
{
    if (value == "liquescent1") return UNEUMEFORM_liquescent1;
    if (value == "liquescent2") return UNEUMEFORM_liquescent2;
    if (value == "liquescent3") return UNEUMEFORM_liquescent3;
    if (value == "quilismatic") return UNEUMEFORM_quilismatic;
    if (value == "rectangular") return UNEUMEFORM_rectangular;
    if (value == "rhombic") return UNEUMEFORM_rhombic;
    if (value == "tied") return UNEUMEFORM_tied;
    LogWarning("Unsupported value '%s' for data.UNEUMEFORM", value.c_str());
    return UNEUMEFORM_NONE;
}

std::string AttConverter::UneumenameToStr(data_UNEUMENAME data) 
{
    std::string value;
    switch(data)
    {
        case UNEUMENAME_punctum : value = "punctum"; break;
        case UNEUMENAME_virga : value = "virga"; break;
        case UNEUMENAME_pes : value = "pes"; break;
        case UNEUMENAME_clivis : value = "clivis"; break;
        case UNEUMENAME_torculus : value = "torculus"; break;
        case UNEUMENAME_torculusresupinus : value = "torculusresupinus"; break;
        case UNEUMENAME_porrectus : value = "porrectus"; break;
        case UNEUMENAME_porrectusflexus : value = "porrectusflexus"; break;
        case UNEUMENAME_apostropha : value = "apostropha"; break;
        case UNEUMENAME_oriscus : value = "oriscus"; break;
        case UNEUMENAME_pressusmaior : value = "pressusmaior"; break;
        case UNEUMENAME_pressusminor : value = "pressusminor"; break;
        case UNEUMENAME_virgastrata : value = "virgastrata"; break;
        default:
            LogWarning("Unknown value '%d' for data.UNEUMENAME", data);
            value = "";
            break;
    }
    return value;
}

data_UNEUMENAME AttConverter::StrToUneumename(std::string value)
{
    if (value == "punctum") return UNEUMENAME_punctum;
    if (value == "virga") return UNEUMENAME_virga;
    if (value == "pes") return UNEUMENAME_pes;
    if (value == "clivis") return UNEUMENAME_clivis;
    if (value == "torculus") return UNEUMENAME_torculus;
    if (value == "torculusresupinus") return UNEUMENAME_torculusresupinus;
    if (value == "porrectus") return UNEUMENAME_porrectus;
    if (value == "porrectusflexus") return UNEUMENAME_porrectusflexus;
    if (value == "apostropha") return UNEUMENAME_apostropha;
    if (value == "oriscus") return UNEUMENAME_oriscus;
    if (value == "pressusmaior") return UNEUMENAME_pressusmaior;
    if (value == "pressusminor") return UNEUMENAME_pressusminor;
    if (value == "virgastrata") return UNEUMENAME_virgastrata;
    LogWarning("Unsupported value '%s' for data.UNEUMENAME", value.c_str());
    return UNEUMENAME_NONE;
}

std::string AttConverter::AccidLogFuncToStr(accidLog_FUNC data) 
{
    std::string value;
    switch(data)
    {
        case accidLog_FUNC_caution : value = "caution"; break;
        case accidLog_FUNC_edit : value = "edit"; break;
        default:
            LogWarning("Unknown value '%d' for att.accid.log@func", data);
            value = "";
            break;
    }
    return value;
}

accidLog_FUNC AttConverter::StrToAccidLogFunc(std::string value)
{
    if (value == "caution") return accidLog_FUNC_caution;
    if (value == "edit") return accidLog_FUNC_edit;
    LogWarning("Unsupported value '%s' for att.accid.log@func", value.c_str());
    return accidLog_FUNC_NONE;
}

std::string AttConverter::ArpegLogOrderToStr(arpegLog_ORDER data) 
{
    std::string value;
    switch(data)
    {
        case arpegLog_ORDER_up : value = "up"; break;
        case arpegLog_ORDER_down : value = "down"; break;
        case arpegLog_ORDER_nonarp : value = "nonarp"; break;
        default:
            LogWarning("Unknown value '%d' for att.arpeg.log@order", data);
            value = "";
            break;
    }
    return value;
}

arpegLog_ORDER AttConverter::StrToArpegLogOrder(std::string value)
{
    if (value == "up") return arpegLog_ORDER_up;
    if (value == "down") return arpegLog_ORDER_down;
    if (value == "nonarp") return arpegLog_ORDER_nonarp;
    LogWarning("Unsupported value '%s' for att.arpeg.log@order", value.c_str());
    return arpegLog_ORDER_NONE;
}

std::string AttConverter::BTremLogFormToStr(bTremLog_FORM data) 
{
    std::string value;
    switch(data)
    {
        case bTremLog_FORM_meas : value = "meas"; break;
        case bTremLog_FORM_unmeas : value = "unmeas"; break;
        default:
            LogWarning("Unknown value '%d' for att.bTrem.log@form", data);
            value = "";
            break;
    }
    return value;
}

bTremLog_FORM AttConverter::StrToBTremLogForm(std::string value)
{
    if (value == "meas") return bTremLog_FORM_meas;
    if (value == "unmeas") return bTremLog_FORM_unmeas;
    LogWarning("Unsupported value '%s' for att.bTrem.log@form", value.c_str());
    return bTremLog_FORM_NONE;
}

std::string AttConverter::BeamingVisBeamrendToStr(beamingVis_BEAMREND data) 
{
    std::string value;
    switch(data)
    {
        case beamingVis_BEAMREND_acc : value = "acc"; break;
        case beamingVis_BEAMREND_rit : value = "rit"; break;
        case beamingVis_BEAMREND_norm : value = "norm"; break;
        default:
            LogWarning("Unknown value '%d' for att.beaming.vis@beam.rend", data);
            value = "";
            break;
    }
    return value;
}

beamingVis_BEAMREND AttConverter::StrToBeamingVisBeamrend(std::string value)
{
    if (value == "acc") return beamingVis_BEAMREND_acc;
    if (value == "rit") return beamingVis_BEAMREND_rit;
    if (value == "norm") return beamingVis_BEAMREND_norm;
    LogWarning("Unsupported value '%s' for att.beaming.vis@beam.rend", value.c_str());
    return beamingVis_BEAMREND_NONE;
}

std::string AttConverter::BeamrendFormToStr(beamrend_FORM data) 
{
    std::string value;
    switch(data)
    {
        case beamrend_FORM_acc : value = "acc"; break;
        case beamrend_FORM_mixed : value = "mixed"; break;
        case beamrend_FORM_rit : value = "rit"; break;
        case beamrend_FORM_norm : value = "norm"; break;
        default:
            LogWarning("Unknown value '%d' for att.beamrend@form", data);
            value = "";
            break;
    }
    return value;
}

beamrend_FORM AttConverter::StrToBeamrendForm(std::string value)
{
    if (value == "acc") return beamrend_FORM_acc;
    if (value == "mixed") return beamrend_FORM_mixed;
    if (value == "rit") return beamrend_FORM_rit;
    if (value == "norm") return beamrend_FORM_norm;
    LogWarning("Unsupported value '%s' for att.beamrend@form", value.c_str());
    return beamrend_FORM_NONE;
}

std::string AttConverter::CurvatureCurvedirToStr(curvature_CURVEDIR data) 
{
    std::string value;
    switch(data)
    {
        case curvature_CURVEDIR_above : value = "above"; break;
        case curvature_CURVEDIR_below : value = "below"; break;
        case curvature_CURVEDIR_mixed : value = "mixed"; break;
        default:
            LogWarning("Unknown value '%d' for att.curvature@curvedir", data);
            value = "";
            break;
    }
    return value;
}

curvature_CURVEDIR AttConverter::StrToCurvatureCurvedir(std::string value)
{
    if (value == "above") return curvature_CURVEDIR_above;
    if (value == "below") return curvature_CURVEDIR_below;
    if (value == "mixed") return curvature_CURVEDIR_mixed;
    LogWarning("Unsupported value '%s' for att.curvature@curvedir", value.c_str());
    return curvature_CURVEDIR_NONE;
}

std::string AttConverter::CutoutCutoutToStr(cutout_CUTOUT data) 
{
    std::string value;
    switch(data)
    {
        case cutout_CUTOUT_cutout : value = "cutout"; break;
        default:
            LogWarning("Unknown value '%d' for att.cutout@cutout", data);
            value = "";
            break;
    }
    return value;
}

cutout_CUTOUT AttConverter::StrToCutoutCutout(std::string value)
{
    if (value == "cutout") return cutout_CUTOUT_cutout;
    LogWarning("Unsupported value '%s' for att.cutout@cutout", value.c_str());
    return cutout_CUTOUT_NONE;
}

std::string AttConverter::DotLogFormToStr(dotLog_FORM data) 
{
    std::string value;
    switch(data)
    {
        case dotLog_FORM_aug : value = "aug"; break;
        case dotLog_FORM_div : value = "div"; break;
        default:
            LogWarning("Unknown value '%d' for att.dot.log@form", data);
            value = "";
            break;
    }
    return value;
}

dotLog_FORM AttConverter::StrToDotLogForm(std::string value)
{
    if (value == "aug") return dotLog_FORM_aug;
    if (value == "div") return dotLog_FORM_div;
    LogWarning("Unsupported value '%s' for att.dot.log@form", value.c_str());
    return dotLog_FORM_NONE;
}

std::string AttConverter::EndingsEndingrendToStr(endings_ENDINGREND data) 
{
    std::string value;
    switch(data)
    {
        case endings_ENDINGREND_top : value = "top"; break;
        case endings_ENDINGREND_barred : value = "barred"; break;
        case endings_ENDINGREND_grouped : value = "grouped"; break;
        default:
            LogWarning("Unknown value '%d' for att.endings@ending.rend", data);
            value = "";
            break;
    }
    return value;
}

endings_ENDINGREND AttConverter::StrToEndingsEndingrend(std::string value)
{
    if (value == "top") return endings_ENDINGREND_top;
    if (value == "barred") return endings_ENDINGREND_barred;
    if (value == "grouped") return endings_ENDINGREND_grouped;
    LogWarning("Unsupported value '%s' for att.endings@ending.rend", value.c_str());
    return endings_ENDINGREND_NONE;
}

std::string AttConverter::FTremLogFormToStr(fTremLog_FORM data) 
{
    std::string value;
    switch(data)
    {
        case fTremLog_FORM_meas : value = "meas"; break;
        case fTremLog_FORM_unmeas : value = "unmeas"; break;
        default:
            LogWarning("Unknown value '%d' for att.fTrem.log@form", data);
            value = "";
            break;
    }
    return value;
}

fTremLog_FORM AttConverter::StrToFTremLogForm(std::string value)
{
    if (value == "meas") return fTremLog_FORM_meas;
    if (value == "unmeas") return fTremLog_FORM_unmeas;
    LogWarning("Unsupported value '%s' for att.fTrem.log@form", value.c_str());
    return fTremLog_FORM_NONE;
}

std::string AttConverter::FermataVisFormToStr(fermataVis_FORM data) 
{
    std::string value;
    switch(data)
    {
        case fermataVis_FORM_inv : value = "inv"; break;
        case fermataVis_FORM_norm : value = "norm"; break;
        default:
            LogWarning("Unknown value '%d' for att.fermata.vis@form", data);
            value = "";
            break;
    }
    return value;
}

fermataVis_FORM AttConverter::StrToFermataVisForm(std::string value)
{
    if (value == "inv") return fermataVis_FORM_inv;
    if (value == "norm") return fermataVis_FORM_norm;
    LogWarning("Unsupported value '%s' for att.fermata.vis@form", value.c_str());
    return fermataVis_FORM_NONE;
}

std::string AttConverter::FermataVisShapeToStr(fermataVis_SHAPE data) 
{
    std::string value;
    switch(data)
    {
        case fermataVis_SHAPE_curved : value = "curved"; break;
        case fermataVis_SHAPE_square : value = "square"; break;
        case fermataVis_SHAPE_angular : value = "angular"; break;
        default:
            LogWarning("Unknown value '%d' for att.fermata.vis@shape", data);
            value = "";
            break;
    }
    return value;
}

fermataVis_SHAPE AttConverter::StrToFermataVisShape(std::string value)
{
    if (value == "curved") return fermataVis_SHAPE_curved;
    if (value == "square") return fermataVis_SHAPE_square;
    if (value == "angular") return fermataVis_SHAPE_angular;
    LogWarning("Unsupported value '%s' for att.fermata.vis@shape", value.c_str());
    return fermataVis_SHAPE_NONE;
}

std::string AttConverter::HairpinLogFormToStr(hairpinLog_FORM data) 
{
    std::string value;
    switch(data)
    {
        case hairpinLog_FORM_cres : value = "cres"; break;
        case hairpinLog_FORM_dim : value = "dim"; break;
        default:
            LogWarning("Unknown value '%d' for att.hairpin.log@form", data);
            value = "";
            break;
    }
    return value;
}

hairpinLog_FORM AttConverter::StrToHairpinLogForm(std::string value)
{
    if (value == "cres") return hairpinLog_FORM_cres;
    if (value == "dim") return hairpinLog_FORM_dim;
    LogWarning("Unsupported value '%s' for att.hairpin.log@form", value.c_str());
    return hairpinLog_FORM_NONE;
}

std::string AttConverter::HarmVisRendgridToStr(harmVis_RENDGRID data) 
{
    std::string value;
    switch(data)
    {
        case harmVis_RENDGRID_grid : value = "grid"; break;
        case harmVis_RENDGRID_gridtext : value = "gridtext"; break;
        case harmVis_RENDGRID_text : value = "text"; break;
        default:
            LogWarning("Unknown value '%d' for att.harm.vis@rendgrid", data);
            value = "";
            break;
    }
    return value;
}

harmVis_RENDGRID AttConverter::StrToHarmVisRendgrid(std::string value)
{
    if (value == "grid") return harmVis_RENDGRID_grid;
    if (value == "gridtext") return harmVis_RENDGRID_gridtext;
    if (value == "text") return harmVis_RENDGRID_text;
    LogWarning("Unsupported value '%s' for att.harm.vis@rendgrid", value.c_str());
    return harmVis_RENDGRID_NONE;
}

std::string AttConverter::HarpPedalLogAToStr(harpPedalLog_A data) 
{
    std::string value;
    switch(data)
    {
        case harpPedalLog_A_f : value = "f"; break;
        case harpPedalLog_A_n : value = "n"; break;
        case harpPedalLog_A_s : value = "s"; break;
        default:
            LogWarning("Unknown value '%d' for att.harpPedal.log@a", data);
            value = "";
            break;
    }
    return value;
}

harpPedalLog_A AttConverter::StrToHarpPedalLogA(std::string value)
{
    if (value == "f") return harpPedalLog_A_f;
    if (value == "n") return harpPedalLog_A_n;
    if (value == "s") return harpPedalLog_A_s;
    LogWarning("Unsupported value '%s' for att.harpPedal.log@a", value.c_str());
    return harpPedalLog_A_NONE;
}

std::string AttConverter::HarpPedalLogBToStr(harpPedalLog_B data) 
{
    std::string value;
    switch(data)
    {
        case harpPedalLog_B_f : value = "f"; break;
        case harpPedalLog_B_n : value = "n"; break;
        case harpPedalLog_B_s : value = "s"; break;
        default:
            LogWarning("Unknown value '%d' for att.harpPedal.log@b", data);
            value = "";
            break;
    }
    return value;
}

harpPedalLog_B AttConverter::StrToHarpPedalLogB(std::string value)
{
    if (value == "f") return harpPedalLog_B_f;
    if (value == "n") return harpPedalLog_B_n;
    if (value == "s") return harpPedalLog_B_s;
    LogWarning("Unsupported value '%s' for att.harpPedal.log@b", value.c_str());
    return harpPedalLog_B_NONE;
}

std::string AttConverter::HarpPedalLogCToStr(harpPedalLog_C data) 
{
    std::string value;
    switch(data)
    {
        case harpPedalLog_C_f : value = "f"; break;
        case harpPedalLog_C_n : value = "n"; break;
        case harpPedalLog_C_s : value = "s"; break;
        default:
            LogWarning("Unknown value '%d' for att.harpPedal.log@c", data);
            value = "";
            break;
    }
    return value;
}

harpPedalLog_C AttConverter::StrToHarpPedalLogC(std::string value)
{
    if (value == "f") return harpPedalLog_C_f;
    if (value == "n") return harpPedalLog_C_n;
    if (value == "s") return harpPedalLog_C_s;
    LogWarning("Unsupported value '%s' for att.harpPedal.log@c", value.c_str());
    return harpPedalLog_C_NONE;
}

std::string AttConverter::HarpPedalLogDToStr(harpPedalLog_D data) 
{
    std::string value;
    switch(data)
    {
        case harpPedalLog_D_f : value = "f"; break;
        case harpPedalLog_D_n : value = "n"; break;
        case harpPedalLog_D_s : value = "s"; break;
        default:
            LogWarning("Unknown value '%d' for att.harpPedal.log@d", data);
            value = "";
            break;
    }
    return value;
}

harpPedalLog_D AttConverter::StrToHarpPedalLogD(std::string value)
{
    if (value == "f") return harpPedalLog_D_f;
    if (value == "n") return harpPedalLog_D_n;
    if (value == "s") return harpPedalLog_D_s;
    LogWarning("Unsupported value '%s' for att.harpPedal.log@d", value.c_str());
    return harpPedalLog_D_NONE;
}

std::string AttConverter::HarpPedalLogEToStr(harpPedalLog_E data) 
{
    std::string value;
    switch(data)
    {
        case harpPedalLog_E_f : value = "f"; break;
        case harpPedalLog_E_n : value = "n"; break;
        case harpPedalLog_E_s : value = "s"; break;
        default:
            LogWarning("Unknown value '%d' for att.harpPedal.log@e", data);
            value = "";
            break;
    }
    return value;
}

harpPedalLog_E AttConverter::StrToHarpPedalLogE(std::string value)
{
    if (value == "f") return harpPedalLog_E_f;
    if (value == "n") return harpPedalLog_E_n;
    if (value == "s") return harpPedalLog_E_s;
    LogWarning("Unsupported value '%s' for att.harpPedal.log@e", value.c_str());
    return harpPedalLog_E_NONE;
}

std::string AttConverter::HarpPedalLogFToStr(harpPedalLog_F data) 
{
    std::string value;
    switch(data)
    {
        case harpPedalLog_F_f : value = "f"; break;
        case harpPedalLog_F_n : value = "n"; break;
        case harpPedalLog_F_s : value = "s"; break;
        default:
            LogWarning("Unknown value '%d' for att.harpPedal.log@f", data);
            value = "";
            break;
    }
    return value;
}

harpPedalLog_F AttConverter::StrToHarpPedalLogF(std::string value)
{
    if (value == "f") return harpPedalLog_F_f;
    if (value == "n") return harpPedalLog_F_n;
    if (value == "s") return harpPedalLog_F_s;
    LogWarning("Unsupported value '%s' for att.harpPedal.log@f", value.c_str());
    return harpPedalLog_F_NONE;
}

std::string AttConverter::HarpPedalLogGToStr(harpPedalLog_G data) 
{
    std::string value;
    switch(data)
    {
        case harpPedalLog_G_f : value = "f"; break;
        case harpPedalLog_G_n : value = "n"; break;
        case harpPedalLog_G_s : value = "s"; break;
        default:
            LogWarning("Unknown value '%d' for att.harpPedal.log@g", data);
            value = "";
            break;
    }
    return value;
}

harpPedalLog_G AttConverter::StrToHarpPedalLogG(std::string value)
{
    if (value == "f") return harpPedalLog_G_f;
    if (value == "n") return harpPedalLog_G_n;
    if (value == "s") return harpPedalLog_G_s;
    LogWarning("Unsupported value '%s' for att.harpPedal.log@g", value.c_str());
    return harpPedalLog_G_NONE;
}

std::string AttConverter::MeiversionMeiversionToStr(meiversion_MEIVERSION data) 
{
    std::string value;
    switch(data)
    {
        case meiversion_MEIVERSION_3_0_0 : value = "3.0.0"; break;
        default:
            LogWarning("Unknown value '%d' for att.meiversion@meiversion", data);
            value = "";
            break;
    }
    return value;
}

meiversion_MEIVERSION AttConverter::StrToMeiversionMeiversion(std::string value)
{
    if (value == "3.0.0") return meiversion_MEIVERSION_3_0_0;
    LogWarning("Unsupported value '%s' for att.meiversion@meiversion", value.c_str());
    return meiversion_MEIVERSION_NONE;
}

std::string AttConverter::MensurVisFormToStr(mensurVis_FORM data) 
{
    std::string value;
    switch(data)
    {
        case mensurVis_FORM_horizontal : value = "horizontal"; break;
        case mensurVis_FORM_vertical : value = "vertical"; break;
        default:
            LogWarning("Unknown value '%d' for att.mensur.vis@form", data);
            value = "";
            break;
    }
    return value;
}

mensurVis_FORM AttConverter::StrToMensurVisForm(std::string value)
{
    if (value == "horizontal") return mensurVis_FORM_horizontal;
    if (value == "vertical") return mensurVis_FORM_vertical;
    LogWarning("Unsupported value '%s' for att.mensur.vis@form", value.c_str());
    return mensurVis_FORM_NONE;
}

std::string AttConverter::MensuralVisMensurformToStr(mensuralVis_MENSURFORM data) 
{
    std::string value;
    switch(data)
    {
        case mensuralVis_MENSURFORM_horizontal : value = "horizontal"; break;
        case mensuralVis_MENSURFORM_vertical : value = "vertical"; break;
        default:
            LogWarning("Unknown value '%d' for att.mensural.vis@mensur.form", data);
            value = "";
            break;
    }
    return value;
}

mensuralVis_MENSURFORM AttConverter::StrToMensuralVisMensurform(std::string value)
{
    if (value == "horizontal") return mensuralVis_MENSURFORM_horizontal;
    if (value == "vertical") return mensuralVis_MENSURFORM_vertical;
    LogWarning("Unsupported value '%s' for att.mensural.vis@mensur.form", value.c_str());
    return mensuralVis_MENSURFORM_NONE;
}

std::string AttConverter::MeterSigVisFormToStr(meterSigVis_FORM data) 
{
    std::string value;
    switch(data)
    {
        case meterSigVis_FORM_num : value = "num"; break;
        case meterSigVis_FORM_denomsym : value = "denomsym"; break;
        case meterSigVis_FORM_norm : value = "norm"; break;
        case meterSigVis_FORM_invis : value = "invis"; break;
        default:
            LogWarning("Unknown value '%d' for att.meterSig.vis@form", data);
            value = "";
            break;
    }
    return value;
}

meterSigVis_FORM AttConverter::StrToMeterSigVisForm(std::string value)
{
    if (value == "num") return meterSigVis_FORM_num;
    if (value == "denomsym") return meterSigVis_FORM_denomsym;
    if (value == "norm") return meterSigVis_FORM_norm;
    if (value == "invis") return meterSigVis_FORM_invis;
    LogWarning("Unsupported value '%s' for att.meterSig.vis@form", value.c_str());
    return meterSigVis_FORM_NONE;
}

std::string AttConverter::MeterSigDefaultVisMeterrendToStr(meterSigDefaultVis_METERREND data) 
{
    std::string value;
    switch(data)
    {
        case meterSigDefaultVis_METERREND_num : value = "num"; break;
        case meterSigDefaultVis_METERREND_denomsym : value = "denomsym"; break;
        case meterSigDefaultVis_METERREND_norm : value = "norm"; break;
        case meterSigDefaultVis_METERREND_invis : value = "invis"; break;
        default:
            LogWarning("Unknown value '%d' for att.meterSigDefault.vis@meter.rend", data);
            value = "";
            break;
    }
    return value;
}

meterSigDefaultVis_METERREND AttConverter::StrToMeterSigDefaultVisMeterrend(std::string value)
{
    if (value == "num") return meterSigDefaultVis_METERREND_num;
    if (value == "denomsym") return meterSigDefaultVis_METERREND_denomsym;
    if (value == "norm") return meterSigDefaultVis_METERREND_norm;
    if (value == "invis") return meterSigDefaultVis_METERREND_invis;
    LogWarning("Unsupported value '%s' for att.meterSigDefault.vis@meter.rend", value.c_str());
    return meterSigDefaultVis_METERREND_NONE;
}

std::string AttConverter::MeterconformanceMetconToStr(meterconformance_METCON data) 
{
    std::string value;
    switch(data)
    {
        case meterconformance_METCON_c : value = "c"; break;
        case meterconformance_METCON_i : value = "i"; break;
        case meterconformance_METCON_o : value = "o"; break;
        default:
            LogWarning("Unknown value '%d' for att.meterconformance@metcon", data);
            value = "";
            break;
    }
    return value;
}

meterconformance_METCON AttConverter::StrToMeterconformanceMetcon(std::string value)
{
    if (value == "c") return meterconformance_METCON_c;
    if (value == "i") return meterconformance_METCON_i;
    if (value == "o") return meterconformance_METCON_o;
    LogWarning("Unsupported value '%s' for att.meterconformance@metcon", value.c_str());
    return meterconformance_METCON_NONE;
}

std::string AttConverter::MordentLogFormToStr(mordentLog_FORM data) 
{
    std::string value;
    switch(data)
    {
        case mordentLog_FORM_inv : value = "inv"; break;
        case mordentLog_FORM_norm : value = "norm"; break;
        default:
            LogWarning("Unknown value '%d' for att.mordent.log@form", data);
            value = "";
            break;
    }
    return value;
}

mordentLog_FORM AttConverter::StrToMordentLogForm(std::string value)
{
    if (value == "inv") return mordentLog_FORM_inv;
    if (value == "norm") return mordentLog_FORM_norm;
    LogWarning("Unsupported value '%s' for att.mordent.log@form", value.c_str());
    return mordentLog_FORM_NONE;
}

std::string AttConverter::NoteLogMensuralLigToStr(noteLogMensural_LIG data) 
{
    std::string value;
    switch(data)
    {
        case noteLogMensural_LIG_recta : value = "recta"; break;
        case noteLogMensural_LIG_obliqua : value = "obliqua"; break;
        default:
            LogWarning("Unknown value '%d' for att.note.log.mensural@lig", data);
            value = "";
            break;
    }
    return value;
}

noteLogMensural_LIG AttConverter::StrToNoteLogMensuralLig(std::string value)
{
    if (value == "recta") return noteLogMensural_LIG_recta;
    if (value == "obliqua") return noteLogMensural_LIG_obliqua;
    LogWarning("Unsupported value '%s' for att.note.log.mensural@lig", value.c_str());
    return noteLogMensural_LIG_NONE;
}

std::string AttConverter::OctaveLogCollToStr(octaveLog_COLL data) 
{
    std::string value;
    switch(data)
    {
        case octaveLog_COLL_coll : value = "coll"; break;
        default:
            LogWarning("Unknown value '%d' for att.octave.log@coll", data);
            value = "";
            break;
    }
    return value;
}

octaveLog_COLL AttConverter::StrToOctaveLogColl(std::string value)
{
    if (value == "coll") return octaveLog_COLL_coll;
    LogWarning("Unsupported value '%s' for att.octave.log@coll", value.c_str());
    return octaveLog_COLL_NONE;
}

std::string AttConverter::PbVisFoliumToStr(pbVis_FOLIUM data) 
{
    std::string value;
    switch(data)
    {
        case pbVis_FOLIUM_verso : value = "verso"; break;
        case pbVis_FOLIUM_recto : value = "recto"; break;
        default:
            LogWarning("Unknown value '%d' for att.pb.vis@folium", data);
            value = "";
            break;
    }
    return value;
}

pbVis_FOLIUM AttConverter::StrToPbVisFolium(std::string value)
{
    if (value == "verso") return pbVis_FOLIUM_verso;
    if (value == "recto") return pbVis_FOLIUM_recto;
    LogWarning("Unsupported value '%s' for att.pb.vis@folium", value.c_str());
    return pbVis_FOLIUM_NONE;
}

std::string AttConverter::PedalLogDirToStr(pedalLog_DIR data) 
{
    std::string value;
    switch(data)
    {
        case pedalLog_DIR_down : value = "down"; break;
        case pedalLog_DIR_up : value = "up"; break;
        case pedalLog_DIR_half : value = "half"; break;
        case pedalLog_DIR_bounce : value = "bounce"; break;
        default:
            LogWarning("Unknown value '%d' for att.pedal.log@dir", data);
            value = "";
            break;
    }
    return value;
}

pedalLog_DIR AttConverter::StrToPedalLogDir(std::string value)
{
    if (value == "down") return pedalLog_DIR_down;
    if (value == "up") return pedalLog_DIR_up;
    if (value == "half") return pedalLog_DIR_half;
    if (value == "bounce") return pedalLog_DIR_bounce;
    LogWarning("Unsupported value '%s' for att.pedal.log@dir", value.c_str());
    return pedalLog_DIR_NONE;
}

std::string AttConverter::PedalVisFormToStr(pedalVis_FORM data) 
{
    std::string value;
    switch(data)
    {
        case pedalVis_FORM_line : value = "line"; break;
        case pedalVis_FORM_pedstar : value = "pedstar"; break;
        case pedalVis_FORM_altpedstar : value = "altpedstar"; break;
        default:
            LogWarning("Unknown value '%d' for att.pedal.vis@form", data);
            value = "";
            break;
    }
    return value;
}

pedalVis_FORM AttConverter::StrToPedalVisForm(std::string value)
{
    if (value == "line") return pedalVis_FORM_line;
    if (value == "pedstar") return pedalVis_FORM_pedstar;
    if (value == "altpedstar") return pedalVis_FORM_altpedstar;
    LogWarning("Unsupported value '%s' for att.pedal.vis@form", value.c_str());
    return pedalVis_FORM_NONE;
}

std::string AttConverter::PianopedalsPedalstyleToStr(pianopedals_PEDALSTYLE data) 
{
    std::string value;
    switch(data)
    {
        case pianopedals_PEDALSTYLE_line : value = "line"; break;
        case pianopedals_PEDALSTYLE_pedstar : value = "pedstar"; break;
        case pianopedals_PEDALSTYLE_altpedstar : value = "altpedstar"; break;
        default:
            LogWarning("Unknown value '%d' for att.pianopedals@pedal.style", data);
            value = "";
            break;
    }
    return value;
}

pianopedals_PEDALSTYLE AttConverter::StrToPianopedalsPedalstyle(std::string value)
{
    if (value == "line") return pianopedals_PEDALSTYLE_line;
    if (value == "pedstar") return pianopedals_PEDALSTYLE_pedstar;
    if (value == "altpedstar") return pianopedals_PEDALSTYLE_altpedstar;
    LogWarning("Unsupported value '%s' for att.pianopedals@pedal.style", value.c_str());
    return pianopedals_PEDALSTYLE_NONE;
}

std::string AttConverter::PlistEvaluateToStr(plist_EVALUATE data) 
{
    std::string value;
    switch(data)
    {
        case plist_EVALUATE_all : value = "all"; break;
        case plist_EVALUATE_one : value = "one"; break;
        case plist_EVALUATE_none : value = "none"; break;
        default:
            LogWarning("Unknown value '%d' for att.plist@evaluate", data);
            value = "";
            break;
    }
    return value;
}

plist_EVALUATE AttConverter::StrToPlistEvaluate(std::string value)
{
    if (value == "all") return plist_EVALUATE_all;
    if (value == "one") return plist_EVALUATE_one;
    if (value == "none") return plist_EVALUATE_none;
    LogWarning("Unsupported value '%s' for att.plist@evaluate", value.c_str());
    return plist_EVALUATE_NONE;
}

std::string AttConverter::PointingXlinkactuateToStr(pointing_XLINKACTUATE data) 
{
    std::string value;
    switch(data)
    {
        case pointing_XLINKACTUATE_onLoad : value = "onLoad"; break;
        case pointing_XLINKACTUATE_onRequest : value = "onRequest"; break;
        case pointing_XLINKACTUATE_none : value = "none"; break;
        case pointing_XLINKACTUATE_other : value = "other"; break;
        default:
            LogWarning("Unknown value '%d' for att.pointing@xlink:actuate", data);
            value = "";
            break;
    }
    return value;
}

pointing_XLINKACTUATE AttConverter::StrToPointingXlinkactuate(std::string value)
{
    if (value == "onLoad") return pointing_XLINKACTUATE_onLoad;
    if (value == "onRequest") return pointing_XLINKACTUATE_onRequest;
    if (value == "none") return pointing_XLINKACTUATE_none;
    if (value == "other") return pointing_XLINKACTUATE_other;
    LogWarning("Unsupported value '%s' for att.pointing@xlink:actuate", value.c_str());
    return pointing_XLINKACTUATE_NONE;
}

std::string AttConverter::PointingXlinkshowToStr(pointing_XLINKSHOW data) 
{
    std::string value;
    switch(data)
    {
        case pointing_XLINKSHOW_new : value = "new"; break;
        case pointing_XLINKSHOW_replace : value = "replace"; break;
        case pointing_XLINKSHOW_embed : value = "embed"; break;
        case pointing_XLINKSHOW_none : value = "none"; break;
        case pointing_XLINKSHOW_other : value = "other"; break;
        default:
            LogWarning("Unknown value '%d' for att.pointing@xlink:show", data);
            value = "";
            break;
    }
    return value;
}

pointing_XLINKSHOW AttConverter::StrToPointingXlinkshow(std::string value)
{
    if (value == "new") return pointing_XLINKSHOW_new;
    if (value == "replace") return pointing_XLINKSHOW_replace;
    if (value == "embed") return pointing_XLINKSHOW_embed;
    if (value == "none") return pointing_XLINKSHOW_none;
    if (value == "other") return pointing_XLINKSHOW_other;
    LogWarning("Unsupported value '%s' for att.pointing@xlink:show", value.c_str());
    return pointing_XLINKSHOW_NONE;
}

std::string AttConverter::RegularmethodMethodToStr(regularmethod_METHOD data) 
{
    std::string value;
    switch(data)
    {
        case regularmethod_METHOD_silent : value = "silent"; break;
        case regularmethod_METHOD_tags : value = "tags"; break;
        default:
            LogWarning("Unknown value '%d' for att.regularmethod@method", data);
            value = "";
            break;
    }
    return value;
}

regularmethod_METHOD AttConverter::StrToRegularmethodMethod(std::string value)
{
    if (value == "silent") return regularmethod_METHOD_silent;
    if (value == "tags") return regularmethod_METHOD_tags;
    LogWarning("Unsupported value '%s' for att.regularmethod@method", value.c_str());
    return regularmethod_METHOD_NONE;
}

std::string AttConverter::RehearsalRehencloseToStr(rehearsal_REHENCLOSE data) 
{
    std::string value;
    switch(data)
    {
        case rehearsal_REHENCLOSE_box : value = "box"; break;
        case rehearsal_REHENCLOSE_circle : value = "circle"; break;
        case rehearsal_REHENCLOSE_none : value = "none"; break;
        default:
            LogWarning("Unknown value '%d' for att.rehearsal@reh.enclose", data);
            value = "";
            break;
    }
    return value;
}

rehearsal_REHENCLOSE AttConverter::StrToRehearsalRehenclose(std::string value)
{
    if (value == "box") return rehearsal_REHENCLOSE_box;
    if (value == "circle") return rehearsal_REHENCLOSE_circle;
    if (value == "none") return rehearsal_REHENCLOSE_none;
    LogWarning("Unsupported value '%s' for att.rehearsal@reh.enclose", value.c_str());
    return rehearsal_REHENCLOSE_NONE;
}

std::string AttConverter::SbVisFormToStr(sbVis_FORM data) 
{
    std::string value;
    switch(data)
    {
        case sbVis_FORM_hash : value = "hash"; break;
        default:
            LogWarning("Unknown value '%d' for att.sb.vis@form", data);
            value = "";
            break;
    }
    return value;
}

sbVis_FORM AttConverter::StrToSbVisForm(std::string value)
{
    if (value == "hash") return sbVis_FORM_hash;
    LogWarning("Unsupported value '%s' for att.sb.vis@form", value.c_str());
    return sbVis_FORM_NONE;
}

std::string AttConverter::StaffgroupingsymSymbolToStr(staffgroupingsym_SYMBOL data) 
{
    std::string value;
    switch(data)
    {
        case staffgroupingsym_SYMBOL_brace : value = "brace"; break;
        case staffgroupingsym_SYMBOL_bracket : value = "bracket"; break;
        case staffgroupingsym_SYMBOL_bracketsq : value = "bracketsq"; break;
        case staffgroupingsym_SYMBOL_line : value = "line"; break;
        case staffgroupingsym_SYMBOL_none : value = "none"; break;
        default:
            LogWarning("Unknown value '%d' for att.staffgroupingsym@symbol", data);
            value = "";
            break;
    }
    return value;
}

staffgroupingsym_SYMBOL AttConverter::StrToStaffgroupingsymSymbol(std::string value)
{
    if (value == "brace") return staffgroupingsym_SYMBOL_brace;
    if (value == "bracket") return staffgroupingsym_SYMBOL_bracket;
    if (value == "bracketsq") return staffgroupingsym_SYMBOL_bracketsq;
    if (value == "line") return staffgroupingsym_SYMBOL_line;
    if (value == "none") return staffgroupingsym_SYMBOL_none;
    LogWarning("Unsupported value '%s' for att.staffgroupingsym@symbol", value.c_str());
    return staffgroupingsym_SYMBOL_NONE;
}

std::string AttConverter::SylLogConToStr(sylLog_CON data) 
{
    std::string value;
    switch(data)
    {
        case sylLog_CON_s : value = "s"; break;
        case sylLog_CON_d : value = "d"; break;
        case sylLog_CON_u : value = "u"; break;
        case sylLog_CON_t : value = "t"; break;
        case sylLog_CON_c : value = "c"; break;
        case sylLog_CON_v : value = "v"; break;
        case sylLog_CON_i : value = "i"; break;
        case sylLog_CON_b : value = "b"; break;
        default:
            LogWarning("Unknown value '%d' for att.syl.log@con", data);
            value = "";
            break;
    }
    return value;
}

sylLog_CON AttConverter::StrToSylLogCon(std::string value)
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

std::string AttConverter::SylLogWordposToStr(sylLog_WORDPOS data) 
{
    std::string value;
    switch(data)
    {
        case sylLog_WORDPOS_i : value = "i"; break;
        case sylLog_WORDPOS_m : value = "m"; break;
        case sylLog_WORDPOS_t : value = "t"; break;
        default:
            LogWarning("Unknown value '%d' for att.syl.log@wordpos", data);
            value = "";
            break;
    }
    return value;
}

sylLog_WORDPOS AttConverter::StrToSylLogWordpos(std::string value)
{
    if (value == "i") return sylLog_WORDPOS_i;
    if (value == "m") return sylLog_WORDPOS_m;
    if (value == "t") return sylLog_WORDPOS_t;
    LogWarning("Unsupported value '%s' for att.syl.log@wordpos", value.c_str());
    return sylLog_WORDPOS_NONE;
}

std::string AttConverter::TupletVisNumformatToStr(tupletVis_NUMFORMAT data) 
{
    std::string value;
    switch(data)
    {
        case tupletVis_NUMFORMAT_count : value = "count"; break;
        case tupletVis_NUMFORMAT_ratio : value = "ratio"; break;
        default:
            LogWarning("Unknown value '%d' for att.tuplet.vis@num.format", data);
            value = "";
            break;
    }
    return value;
}

tupletVis_NUMFORMAT AttConverter::StrToTupletVisNumformat(std::string value)
{
    if (value == "count") return tupletVis_NUMFORMAT_count;
    if (value == "ratio") return tupletVis_NUMFORMAT_ratio;
    LogWarning("Unsupported value '%s' for att.tuplet.vis@num.format", value.c_str());
    return tupletVis_NUMFORMAT_NONE;
}

std::string AttConverter::TurnLogFormToStr(turnLog_FORM data) 
{
    std::string value;
    switch(data)
    {
        case turnLog_FORM_inv : value = "inv"; break;
        case turnLog_FORM_norm : value = "norm"; break;
        default:
            LogWarning("Unknown value '%d' for att.turn.log@form", data);
            value = "";
            break;
    }
    return value;
}

turnLog_FORM AttConverter::StrToTurnLogForm(std::string value)
{
    if (value == "inv") return turnLog_FORM_inv;
    if (value == "norm") return turnLog_FORM_norm;
    LogWarning("Unsupported value '%s' for att.turn.log@form", value.c_str());
    return turnLog_FORM_NONE;
}

std::string AttConverter::WhitespaceXmlspaceToStr(whitespace_XMLSPACE data) 
{
    std::string value;
    switch(data)
    {
        case whitespace_XMLSPACE_default : value = "default"; break;
        case whitespace_XMLSPACE_preserve : value = "preserve"; break;
        default:
            LogWarning("Unknown value '%d' for att.whitespace@xml:space", data);
            value = "";
            break;
    }
    return value;
}

whitespace_XMLSPACE AttConverter::StrToWhitespaceXmlspace(std::string value)
{
    if (value == "default") return whitespace_XMLSPACE_default;
    if (value == "preserve") return whitespace_XMLSPACE_preserve;
    LogWarning("Unsupported value '%s' for att.whitespace@xml:space", value.c_str());
    return whitespace_XMLSPACE_NONE;
}

} // vrv namespace

/////////////////////////////////////////////////////////////////////////////
// Name:        att.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Laurent Pugin. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "att.h"

//----------------------------------------------------------------------------

#include <stdlib.h>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Att
//----------------------------------------------------------------------------

Att::Att()
{
}

Att::~Att()
{
}
        
std::string Att::StrToStr(std::string str)
{
    return str;
}

// Basic converters for writing    

std::string Att::DblToStr(double data)
{
    return StringFormat("%f", data);
}
    
std::string Att::IntToStr(int data)
{
    return StringFormat("%d", data);
}
    
// Basic converters for reading

double Att::StrToDbl(std::string value)
{
    return atof( value.c_str() );
}
int Att::StrToInt(std::string value)
{
    return atoi( value.c_str() );
}

// Converters for writing and reading

std::string Att::AccidentalImplicitToStr(data_ACCIDENTAL_IMPLICIT data)
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
            LogWarning("Unknown mode '%d'", data);
            value = "";
            break;
    }
    return value;
}
        
data_ACCIDENTAL_IMPLICIT Att::StrToAccidentalImplicit(std::string value)
{
	if (value == "s") return ACCIDENTAL_IMPLICIT_s;
	else if (value == "f") return ACCIDENTAL_IMPLICIT_f;
	else if (value == "ss") return ACCIDENTAL_IMPLICIT_ss;
	else if (value == "ff") return ACCIDENTAL_IMPLICIT_ff;
	else if (value == "n") return ACCIDENTAL_IMPLICIT_n;
	else if (value == "su") return ACCIDENTAL_IMPLICIT_su;
	else if (value == "sd") return ACCIDENTAL_IMPLICIT_sd;
	else if (value == "fu") return ACCIDENTAL_IMPLICIT_fu;
	else if (value == "fd") return ACCIDENTAL_IMPLICIT_fd;
	else {
		LogWarning("Unsupported pitch name '%s'", value.c_str() );
	}
	return ACCIDENTAL_IMPLICIT_NONE;
}
    
std::string Att::BarRenditionToStr(data_BARRENDITION data)
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
            LogWarning("Unknown mode '%d'", data);
            value = "";
            break;
    }
    return value;
}

data_BARRENDITION Att::StrToBarRendition(std::string value)
{
	if (value == "dashed") return BARRENDITION_dashed;
	else if (value == "dotted") return BARRENDITION_dotted;
	else if (value == "dbl") return BARRENDITION_dbl;
	else if (value == "dbldashed") return BARRENDITION_dbldashed;
	else if (value == "dbldotted") return BARRENDITION_dbldotted;
	else if (value == "end") return BARRENDITION_end;
	else if (value == "invis") return BARRENDITION_invis;
	else if (value == "rptstart") return BARRENDITION_rptstart;
	else if (value == "rptboth") return BARRENDITION_rptboth;
	else if (value == "rptend") return BARRENDITION_rptend;
	else if (value == "single") return BARRENDITION_single;
	else {
		LogWarning("Unsupported pitch name '%s'", value.c_str() );
	}
	return BARRENDITION_NONE;
}

std::string Att::BoolToStr(data_BOOLEAN data)
{
    std::string value;
    switch(data)
    {
        case BOOLEAN_true : value = "true"; break;
        case BOOLEAN_false : value = "false"; break;
        default:
            LogWarning("Unknown mode '%d'", data);
            value = "";
            break;
    }
    return value;
    
}
    
data_BOOLEAN Att::StrToBool(std::string value)
{
	if (value == "true") return BOOLEAN_true;
	else if (value == "false") return BOOLEAN_false;
	else {
		LogWarning("Unsupported pitch name '%s'", value.c_str() );
	}
	return BOOLEAN_NONE;
}
    
std::string Att::ClefShapeToStr( data_CLEFSHAPE data )
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
            LogWarning("Unknown clef shape '%d'", data);
            value = "";
            break;
    }
    return value;
}
    
data_CLEFSHAPE Att::StrToClefShape( std::string value )
{
    data_CLEFSHAPE data = CLEFSHAPE_NONE;
    if ( value == "G" ) data = CLEFSHAPE_G;
    else if ( value == "F" ) data = CLEFSHAPE_F;
    else if ( value == "C" ) data = CLEFSHAPE_C;
    else if ( value == "perc" ) data = CLEFSHAPE_perc;
    else
    {
        LogWarning("Unsupported clef with @shape '%s'", value.c_str());
    }
    return data;
}    

std::string Att::ConToStr(data_CON data)
{
    std::string value;
    switch(data)
    {
        case CON_s : value = "s"; break;
        case CON_d : value = "d"; break;
        case CON_u : value = "u"; break;
        case CON_t : value = "t"; break;
        case CON_c : value = "c"; break;
        case CON_v : value = "v"; break;
        case CON_i : value = "i"; break;
        case CON_b : value = "b"; break;
        default:
            LogWarning("Unknown mode '%d'", data);
            value = "";
            break;
    }
    return value;
}

data_CON Att::StrToCon(std::string value)
{
    if (value == "s") return CON_s;
    else if (value == "d") return CON_d;
    else if (value == "u") return CON_u;
    else if (value == "t") return CON_t;
    else if (value == "c") return CON_c;
    else if (value == "v") return CON_v;
    else if (value == "i") return CON_i;
    else if (value == "b") return CON_b;
    else {
        LogWarning("Unsupported pitch name '%s'", value.c_str() );
    }
    return CON_NONE;
}
    
std::string Att::KeySignatureToStr(data_KEYSIGNATURE data)
{
    std::string value;
    switch(data)
    {
        case KEYSIGNATURE_7f : value = "7f"; break;
        case KEYSIGNATURE_6f : value = "6f"; break;
        case KEYSIGNATURE_5f : value = "5f"; break;
        case KEYSIGNATURE_4f : value = "4f"; break;
        case KEYSIGNATURE_3f : value = "3f"; break;
        case KEYSIGNATURE_2f : value = "2f"; break;
        case KEYSIGNATURE_1f : value = "1f"; break;
        case KEYSIGNATURE_0 : value = "0"; break;
        case KEYSIGNATURE_1s : value = "1s"; break;
        case KEYSIGNATURE_2s : value = "2s"; break;
        case KEYSIGNATURE_3s : value = "3s"; break;
        case KEYSIGNATURE_4s : value = "4s"; break;
        case KEYSIGNATURE_5s : value = "5s"; break;
        case KEYSIGNATURE_6s : value = "6s"; break;
        case KEYSIGNATURE_7s : value = "7s"; break;
        case KEYSIGNATURE_mixed : value = "mixed"; break;
        default:
            LogWarning("Unknown mode '%d'", data);
            value = "";
            break;
    }
    return value;
}
    
data_KEYSIGNATURE Att::StrToKeySignature(std::string value)
{
	if (value == "7f") return KEYSIGNATURE_7f;
	else if (value == "6f") return KEYSIGNATURE_6f;
	else if (value == "5f") return KEYSIGNATURE_5f;
	else if (value == "4f") return KEYSIGNATURE_4f;
	else if (value == "3f") return KEYSIGNATURE_3f;
	else if (value == "2f") return KEYSIGNATURE_2f;
	else if (value == "1f") return KEYSIGNATURE_1f;
	else if (value == "0") return KEYSIGNATURE_0;
	else if (value == "1s") return KEYSIGNATURE_1s;
	else if (value == "2s") return KEYSIGNATURE_2s;
	else if (value == "3s") return KEYSIGNATURE_3s;
	else if (value == "4s") return KEYSIGNATURE_4s;
	else if (value == "5s") return KEYSIGNATURE_5s;
	else if (value == "6s") return KEYSIGNATURE_6s;
	else if (value == "7s") return KEYSIGNATURE_7s;
	else if (value == "mixed") return KEYSIGNATURE_mixed;
	else {
		LogWarning("Unsupported pitch name '%s'", value.c_str() );
	}
	return KEYSIGNATURE_NONE;
}
    
std::string Att::LigatureToStr(data_LIGATURE data)
{
    std::string value;
    switch(data)
    {
        case LIGATURE_recta : value = "recta"; break;
        case LIGATURE_obliqua : value = "obliqua"; break;
        default:
            LogWarning("Unknown mode '%d'", data);
            value = "";
            break;
    }
    return value;
}

data_LIGATURE Att::StrToLigature(std::string value)
{
    if (value == "recta") return LIGATURE_recta;
    else if (value == "obliqua") return LIGATURE_obliqua;
    else {
        LogWarning("Unsupported pitch name '%s'", value.c_str() );
    }
    return LIGATURE_NONE;
}
    
std::string Att::MensurationSignToStr( data_MENSURATIONSIGN data )
{
    std::string value;
    switch(data)
    {
        case MENSURATIONSIGN_C : value = "C"; break;
        case MENSURATIONSIGN_O : value = "O"; break;
        default:
            LogWarning("Unknown mensur sign '%d'", data);
            value = "";
            break;
    }
    return value;
}

data_MENSURATIONSIGN Att::StrToMensurationSign(std::string value)
{
    if (value == "C") return MENSURATIONSIGN_C;
    else if (value == "O") return MENSURATIONSIGN_O;
    else {
        LogWarning("Unsupported mensur sign '%s'", value.c_str() );
    }
    // default
    return MENSURATIONSIGN_C;
}

std::string Att::MeterSignToStr(data_METERSIGN data)
{
    std::string value;
    switch(data)
    {
        case METERSIGN_common : value = "common"; break;
        case METERSIGN_cut : value = "cut"; break;
        default:
            LogWarning("Unknown meterSig sym '%d'", data);
            value = "";
            break;
    }
    return value;
}

data_METERSIGN Att::StrToMeterSign(std::string value)
{
    if (value == "common") return METERSIGN_common;
    else if (value == "cut") return METERSIGN_cut;
    else {
        LogWarning("Unsupported meter sign '%s'", value.c_str() );
    }
    // default
    return METERSIGN_NONE;
}
    
std::string Att::ModeToStr(data_MODE data)
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
            LogWarning("Unknown mode '%d'", data);
            value = "";
            break;
    }
    return value;
}
    
data_MODE Att::StrToMode(std::string value)
{
    if (value == "major") return MODE_major;
    else if (value == "minor") return MODE_minor;
    else if (value == "dorian") return MODE_dorian;
    else if (value == "phrygian") return MODE_phrygian;
    else if (value == "lydian") return MODE_lydian;
    else if (value == "mixolydian") return MODE_mixolydian;
    else if (value == "aeolian") return MODE_aeolian;
    else if (value == "locrian") return MODE_locrian;
    else {
        LogWarning("Unsupported mode '%s'", value.c_str() );
    }
    // default
    return MODE_NONE;
}
    
std::string Att::OctaveDisToStr( data_OCTAVE_DIS data )
{
    std::string value;
    switch(data)
    {
        case OCTAVE_DIS_8 : value = "8"; break;
        case OCTAVE_DIS_15 : value = "15"; break;
        case OCTAVE_DIS_22 : value = "22"; break;
        default:
            LogWarning("Unknown octave dis '%d'", data);
            value = "";
            break;
    }
    return value;
}

data_OCTAVE_DIS Att::StrToOctaveDis(std::string value)
{
    if (value == "8") return OCTAVE_DIS_8;
    else if (value == "15") return OCTAVE_DIS_15;
    else if (value == "22") return OCTAVE_DIS_22;
    else {
        LogWarning("Unsupported octave dis '%s'", value.c_str() );
    }
    // default
    return OCTAVE_DIS_NONE;
}
    
std::string Att::OrientationToStr(data_ORIENTATION data)
{
    std::string value;
    switch(data)
    {
        case ORIENTATION_reversed : value = "reversed"; break;
        case ORIENTATION_90CW : value = "90CW"; break;
        case ORIENTATION_90CCW : value = "90CCW"; break;
        default:
            LogWarning("Unknown mode '%d'", data);
            value = "";
            break;
    }
    return value;
}
    
data_ORIENTATION Att::StrToOrientation(std::string value)
{
	if (value == "reversed") return ORIENTATION_reversed;
	else if (value == "90CW") return ORIENTATION_90CW;
	else if (value == "90CCW") return ORIENTATION_90CCW;
	else {
		LogWarning("Unsupported pitch name '%s'", value.c_str() );
	}
	return ORIENTATION_NONE;
}
    
std::string Att::PitchNameToStr(data_PITCHNAME data)
{
    std::string value;
    switch(data)
    {
        case PITCHNAME_c : value = "c"; break;
        case PITCHNAME_d : value = "d"; break;
        case PITCHNAME_e : value = "e"; break;
        case PITCHNAME_f : value = "f"; break;
        case PITCHNAME_g : value = "g"; break;
        case PITCHNAME_a : value = "a"; break;
        case PITCHNAME_b : value = "b"; break;
        default:
            LogWarning("Unknown mode '%d'", data);
            value = "";
            break;
    }
    return value;
}
    
data_PITCHNAME Att::StrToPitchName(std::string value)
{
    if (value == "c") return PITCHNAME_c;
    else if (value == "d") return PITCHNAME_d;
    else if (value == "e") return PITCHNAME_e;
    else if (value == "f") return PITCHNAME_f;
    else if (value == "g") return PITCHNAME_g;
    else if (value == "a") return PITCHNAME_a;
    else if (value == "b") return PITCHNAME_b;
    else {
        LogWarning("Unsupported pitch name '%s'", value.c_str() );
    }
    // default
    return PITCHNAME_NONE;
}
    
std::string Att::PlaceToStr( data_PLACE data )
{
    std::string value;
    switch(data)
    {
        case PLACE_above : value = "above"; break;
        case PLACE_below : value = "below"; break;
        default:
            LogWarning("Unknown place '%d'", data);
            value = "";
            break;
    }
    return value;
}
    
data_PLACE Att::StrToPlace(std::string value)
{
    if (value == "below") return PLACE_below;
    else if (value == "above") return PLACE_above;
    else {
        LogWarning("Unsupported place '%s'", value.c_str() );
    }
    // default
    return PLACE_NONE;
}    

std::string Att::StemDirectionToStr(data_STEMDIRECTION data)
{
    std::string value;
    switch(data)
    {
        case STEMDIRECTION_up : value = "up"; break;
        case STEMDIRECTION_down : value = "down"; break;
        default:
            LogWarning("Unknown mode '%d'", data);
            value = "";
            break;
    }
    return value;
}

data_STEMDIRECTION Att::StrToStemDirection(std::string value)
{
    if (value == "up") return STEMDIRECTION_up;
    else if (value == "down") return STEMDIRECTION_down;
    else {
        LogWarning("Unsupported pitch name '%s'", value.c_str() );
    }
    return STEMDIRECTION_NONE;
}

std::string Att::StemPositionToStr(data_STEMPOSITION data)
{
    std::string value;
    switch(data)
    {
        case STEMPOSITION_left : value = "left"; break;
        case STEMPOSITION_right : value = "right"; break;
        case STEMPOSITION_center : value = "center"; break;
        default:
            LogWarning("Unknown mode '%d'", data);
            value = "";
            break;
    }
    return value;
}

data_STEMPOSITION Att::StrToStemPosition(std::string value)
{
    if (value == "left") return STEMPOSITION_left;
    else if (value == "right") return STEMPOSITION_right;
    else if (value == "center") return STEMPOSITION_center;
    else {
        LogWarning("Unsupported pitch name '%s'", value.c_str() );
    }
    return STEMPOSITION_NONE;
}

std::string Att::WordPosToStr(data_WORDPOS data)
{
    std::string value;
    switch(data)
    {
        case WORDPOS_i : value = "i"; break;
        case WORDPOS_m : value = "m"; break;
        case WORDPOS_t : value = "t"; break;
        default:
            LogWarning("Unknown mode '%d'", data);
            value = "";
            break;
    }
    return value;
}

data_WORDPOS Att::StrToWordPos(std::string value)
{
    if (value == "i") return WORDPOS_i;
    else if (value == "m") return WORDPOS_m;
    else if (value == "t") return WORDPOS_t;
    else {
        LogWarning("Unsupported pitch name '%s'", value.c_str() );
    }
    return WORDPOS_NONE;
}
    
} // namespace vrv


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

#include "object.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Att
//----------------------------------------------------------------------------

Att::Att():
    LibmeiConverter()
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

std::string Att::BeatrptRendToStr(data_BEATRPT_REND data)
{
    std::string value;
    switch(data)
    {
        case BEATRPT_REND_4 : value = "4"; break;
        case BEATRPT_REND_8 : value = "8"; break;
        case BEATRPT_REND_16 : value = "16"; break;
        case BEATRPT_REND_32 : value = "32"; break;
        case BEATRPT_REND_64 : value = "64"; break;
        case BEATRPT_REND_128 : value = "128"; break;
        case BEATRPT_REND_mixed : value = "mixed"; break;
        default:
            LogWarning("Unknown beatrpt rend '%d'", data);
            value = "";
            break;
    }
    return value;
}

data_BEATRPT_REND Att::StrToBeatrptRend(std::string value)
{
    if (value == "4") return BEATRPT_REND_4;
    else if (value == "8") return BEATRPT_REND_8;
    else if (value == "16") return BEATRPT_REND_16;
    else if (value == "32") return BEATRPT_REND_32;
    else if (value == "64") return BEATRPT_REND_64;
    else if (value == "128") return BEATRPT_REND_128;
    else if (value == "mixed") return BEATRPT_REND_mixed;
    else {
        LogWarning("Unsupported beatrpt rend '%s'", value.c_str() );
    }
    return BEATRPT_REND_NONE;
}
    
    
std::string Att::DurationToStr(data_DURATION data)
{
    std::string value;
    if (data == DURATION_maxima) value = "maxima";
    else if (data == DURATION_longa) value = "longa";
    else if (data == DURATION_brevis) value = "brevis";
    else if (data == DURATION_semibrevis) value = "semibrevis";
    else if (data == DURATION_minima) value = "minima";
    else if (data == DURATION_semiminima) value = "semiminima";
    else if (data == DURATION_fusa) value = "fusa";
    else if (data == DURATION_semifusa) value = "semifusa";
    else if (data == DURATION_long) value = "long";
    else if (data == DURATION_breve) value = "breve";
    else if (data == DURATION_1) value = "1";
    else if (data == DURATION_2) value = "2";
    else if (data == DURATION_4) value = "4";
    else if (data == DURATION_8) value = "8";
    else if (data == DURATION_16) value = "16";
    else if (data == DURATION_32) value = "32";
    else if (data == DURATION_64) value = "64";
    else if (data == DURATION_128) value = "128";
    else if (data == DURATION_256) value = "256";
    else {
        LogWarning("Unknown dur '%d'", data);
        value = "4";
    }
    return value;
}

data_DURATION Att::StrToDuration(std::string value)
{
    data_DURATION dur;
    if (value == "maxima") dur = DURATION_maxima;
    else if (value == "longa") dur = DURATION_longa;
    else if (value == "brevis") dur = DURATION_brevis;
    else if (value == "semibrevis") dur = DURATION_semibrevis;
    else if (value == "minima") dur = DURATION_minima;
    else if (value == "semiminima") dur = DURATION_semiminima;
    else if (value == "fusa") dur = DURATION_fusa;
    else if (value == "semifusa") dur = DURATION_semifusa;
    else if (value == "long") dur = DURATION_long;
    else if (value == "breve") dur = DURATION_breve;
    else if (value == "1") dur = DURATION_1;
    else if (value == "2") dur = DURATION_2;
    else if (value == "4") dur = DURATION_4;
    else if (value == "8") dur = DURATION_8;
    else if (value == "16") dur = DURATION_16;
    else if (value == "32") dur = DURATION_32;
    else if (value == "64") dur = DURATION_64;
    else if (value == "128") dur = DURATION_128;
    else if (value == "256") dur = DURATION_256;
    else {
        if ((value.length() > 0) && (value[value.length()-1] == 'p')) {
            //LogWarning("PPQ duration dur_s are not supported"); // remove it for now
        }
        else {
            LogWarning("Unknown dur '%s'", value.c_str());
        }
        dur = DURATION_NONE;
    }
    return dur;
}
    
std::string Att::KeysignatureToStr(data_KEYSIGNATURE data)
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
            LogWarning("Unknown key signature '%d'", data);
            value = "";
            break;
    }
    return value;
}
    
data_KEYSIGNATURE Att::StrToKeysignature(std::string value)
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
		LogWarning("Unsupported key signature '%s'", value.c_str() );
	}
	return KEYSIGNATURE_NONE;
}

std::string Att::ModusmaiorToStr(data_MODUSMAIOR data)
{
    std::string value;
    switch(data)
    {
        case MODUSMAIOR_2 : value = "2"; break;
        case MODUSMAIOR_3 : value = "3"; break;
        default:
            LogWarning("Unknown modusmaior '%d'", data);
            value = "";
            break;
    }
    return value;
}

data_MODUSMAIOR Att::StrToModusmaior(std::string value)
{
    if (value == "2") return MODUSMAIOR_2;
    else if (value == "3") return MODUSMAIOR_3;
    else {
        LogWarning("Unsupported modusmaior '%s'", value.c_str() );
    }
    // default
    return MODUSMAIOR_NONE;
}

std::string Att::ModusminorToStr(data_MODUSMINOR data)
{
    std::string value;
    switch(data)
    {
        case MODUSMINOR_2 : value = "2"; break;
        case MODUSMINOR_3 : value = "3"; break;
        default:
            LogWarning("Unknown modusmaior '%d'", data);
            value = "";
            break;
    }
    return value;
}

data_MODUSMINOR Att::StrToModusminor(std::string value)
{
    if (value == "2") return MODUSMINOR_2;
    else if (value == "3") return MODUSMINOR_3;
    else {
        LogWarning("Unsupported modusmaior '%s'", value.c_str() );
    }
    // default
    return MODUSMINOR_NONE;
}
    
std::string Att::PitchnameToStr(data_PITCHNAME data)
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
            LogWarning("Unknown pitch name '%d'", data);
            value = "";
            break;
    }
    return value;
}

data_PITCHNAME Att::StrToPitchname(std::string value)
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
            LogWarning("Unknown orientation '%d'", data);
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
        LogWarning("Unsupported orientation '%s'", value.c_str() );
    }
    return ORIENTATION_NONE;
}

std::string Att::ProlatioToStr(data_PROLATIO data)
{
    std::string value;
    switch(data)
    {
        case PROLATIO_2 : value = "2"; break;
        case PROLATIO_3 : value = "3"; break;
        default:
            LogWarning("Unknown prolatio '%d'", data);
            value = "";
            break;
    }
    return value;
}

data_PROLATIO Att::StrToProlatio(std::string value)
{
    if (value == "2") return PROLATIO_2;
    else if (value == "3") return PROLATIO_3;
    else {
        LogWarning("Unsupported prolatio '%s'", value.c_str() );
    }
    // default
    return PROLATIO_NONE;
}

std::string Att::TempusToStr(data_TEMPUS data)
{
    std::string value;
    switch(data)
    {
        case TEMPUS_2 : value = "2"; break;
        case TEMPUS_3 : value = "3"; break;
        default:
            LogWarning("Unknown tempus '%d'", data);
            value = "";
            break;
    }
    return value;
}

data_TEMPUS Att::StrToTempus(std::string value)
{
    if (value == "2") return TEMPUS_2;
    else if (value == "3") return TEMPUS_3;
    else {
        LogWarning("Unsupported tempus '%s'", value.c_str() );
    }
    // default
    return TEMPUS_NONE;
}
    
    
std::string Att::TieToStr(data_TIE data)
{
    std::string value;
    switch(data)
    {
        case TIE_i : value = "i"; break;
        case TIE_m : value = "m"; break;
        case TIE_t : value = "t"; break;
        default:
            LogWarning("Unknown tie '%d'", data);
            value = "";
            break;
    }
    return value;
}

data_TIE Att::StrToTie(std::string value)
{
    if (value == "i") return TIE_i;
    else if (value == "m") return TIE_m;
    else if (value == "t") return TIE_t;
    else {
        LogWarning("Unsupported tie '%s'", value.c_str() );
    }
    return TIE_NONE;
}
    
    
std::string Att::StemdirectionToStr(data_STEMDIRECTION data)
{
    std::string value;
    switch(data)
    {
        case STEMDIRECTION_up : value = "up"; break;
        case STEMDIRECTION_down : value = "down"; break;
        default:
            LogWarning("Unknown stem direction '%d'", data);
            value = "";
            break;
    }
    return value;
}

data_STEMDIRECTION Att::StrToStemdirection(std::string value)
{
    if (value == "up") return STEMDIRECTION_up;
    else if (value == "down") return STEMDIRECTION_down;
    else {
        LogWarning("Unsupported stem direction '%s'", value.c_str() );
    }
    return STEMDIRECTION_NONE;
}

std::string Att::MeasurebeatToStr(data_MEASUREBEAT data)
{
    return "0m0.0";
}
    
data_MEASUREBEAT Att::StrToMeasurebeat(std::string value)
{
    return std::make_pair(0, 1.0);
}
    
//----------------------------------------------------------------------------
// AttComparison
//----------------------------------------------------------------------------

bool AttComparison::operator()(Object *object)
{
    if  ( object->Is() == m_classId ) {
        return true;
    }
    return false;
}
    
bool AttComparison::MatchesType(Object *object)
{
    if  ( object->Is() == m_classId ) {
        return true;
    }
    return false;
}
    
} // namespace vrv


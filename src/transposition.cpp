/////////////////////////////////////////////////////////////////////////////
// Name:        transposition.cpp
// Author:      Craig Stuart Sapp
// Created:     03/12/2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

//
// References:
//            http://www.ccarh.org/publications/reprints/base40
//            https://github.com/craigsapp/humlib/blob/master/src/tool-transpose.cpp
//
// Description:   Draft implementation of a transposition system for verovio.
//                There is a main() function at the bottom of the file for demo/testing.
//                There are two classes in this file:
//                   TransPitch: pitch representation as three integers:
//                            m_pname: diatonic pitch class integer from C=0 to B=6.
//                            m_accid: chromatic alterations in semitones (0=natural, -1=flat).
//                            m_oct: octave number (4 = middle-C octave).
//                   Transposer: transposition system which uses TransPitch as a user interface.
//                   (Add MEI to TransPitch conversions in TransPitch class, or use external
//                    code to interface to verovio attributes for <note>).
//                   The default maximum accidental handling is +/- two sharps/flats (base-40).
//                   Use the Transposer::SetMaxAccid() to set the maximum allowed accidental
//                   count.  Transposer::SetBase40() is equivalent to Transposer::SetMaxAccid(2),
//                   and Transposer::SetBase600() is equivalent to Transposer::SetMaxAccid(42).
//
// Todo: Probably useful to add an autowrap feature to force unrepresentable pitches to be
//     moved to enharmonic equivalent pitches (better than leaving a pitch undefined).
//     For example, F#### in a system that cannot represent more than two or three
//     sharps would be converted to G##, probably with a warning message.  From F####
//     to G## is up a diminished second ("d2").
//

// Diatonic pitch class integers:
// These could be converted into an enum provided
// that the same value are assigned to each class.
#define dpc_C 0 /* Integer for Diatonic pitch class for C */
#define dpc_D 1
#define dpc_E 2
#define dpc_F 3
#define dpc_G 4
#define dpc_A 5
#define dpc_B 6

#include "transposition.h"

#include <cctype>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include "attdef.h"
#include "atttypes.h"
#include "vrv.h"
#include "vrvdef.h"

////////////////////////////////////////////////////////////////////////////
//
// The TransPitch class is an interface for storing information about notes which
// will be used in the Transposer class.  The diatonic pitch class, chromatic alteration
// of the diatonic pitch and the octave are store in the class.  Names given to the
// parameters are analogous to MEI note attributes.  Note that note@accid can be also
// note/accid in MEI data, and other complications that need to be resolved into
// storing the correct pitch information in TransPitch.
//
namespace vrv {

//////////////////////////////
//
// TransPitch::Tpitch -- TransPitch constructor.
//

TransPitch::TransPitch(int aPname, int anAccid, int anOct)
{
    SetPitch(aPname, anAccid, anOct);
}

TransPitch::TransPitch(data_PITCHNAME pname, data_ACCIDENTAL_GESTURAL accidG, data_ACCIDENTAL_WRITTEN accidW, int oct)
{
    SetPitch(pname - PITCHNAME_c, GetChromaticAlteration(accidG, accidW), oct);
}

TransPitch::TransPitch(const TransPitch &pitch)
{
    m_pname = pitch.m_pname;
    m_accid = pitch.m_accid;
    m_oct = pitch.m_oct;
}

int TransPitch::GetChromaticAlteration(data_ACCIDENTAL_GESTURAL accidG, data_ACCIDENTAL_WRITTEN accidW)
{
    switch (accidG) {
        case ACCIDENTAL_GESTURAL_ff: return -2;
        // case ACCIDENTAL_GESTURAL_fd: return -1.5;
        case ACCIDENTAL_GESTURAL_f: return -1;
        // case ACCIDENTAL_GESTURAL_fu: return 0.5;
        case ACCIDENTAL_GESTURAL_n: return 0;
        // case ACCIDENTAL_GESTURAL_sd: return 0.5;
        case ACCIDENTAL_GESTURAL_s: return 1;
        // case ACCIDENTAL_GESTURAL_su: return 1.5;
        case ACCIDENTAL_GESTURAL_ss: return 2;
        default: break;
    }
    switch (accidW) {
        case ACCIDENTAL_WRITTEN_tf: return -3;
        case ACCIDENTAL_WRITTEN_ff: return -2;
        // case ACCIDENTAL_WRITTEN_3qf: return -1.5;
        // case ACCIDENTAL_WRITTEN_fd: return -1.5;
        case ACCIDENTAL_WRITTEN_f: return -1;
        case ACCIDENTAL_WRITTEN_nf: return -1;
        // case ACCIDENTAL_WRITTEN_fu: return -0.5;
        // case ACCIDENTAL_WRITTEN_1qf: return -0.5;
        // case ACCIDENTAL_WRITTEN_nd: return -0.5;
        case ACCIDENTAL_WRITTEN_n: return 0;
        // case ACCIDENTAL_WRITTEN_nu: return 0.5;
        // case ACCIDENTAL_WRITTEN_1qs: return 0.5;
        // case ACCIDENTAL_WRITTEN_sd: return 0.5;
        case ACCIDENTAL_WRITTEN_ns: return 1;
        case ACCIDENTAL_WRITTEN_s: return 1;
        // case ACCIDENTAL_WRITTEN_su: return 1.5;
        // case ACCIDENTAL_WRITTEN_3qs: return 1.5;
        case ACCIDENTAL_WRITTEN_ss: return 2;
        case ACCIDENTAL_WRITTEN_x: return 2;
        case ACCIDENTAL_WRITTEN_xs: return 3;
        case ACCIDENTAL_WRITTEN_sx: return 3;
        case ACCIDENTAL_WRITTEN_ts: return 3;
        default: break;
    }
    return 0;
}

data_ACCIDENTAL_GESTURAL TransPitch::GetAccidG() const
{
    switch (m_accid) {
        case -2: return ACCIDENTAL_GESTURAL_ff;
        case -1: return ACCIDENTAL_GESTURAL_f;
        case 0: return ACCIDENTAL_GESTURAL_n;
        case 1: return ACCIDENTAL_GESTURAL_s;
        case 2: return ACCIDENTAL_GESTURAL_ss;
        default: break;
    }
    LogWarning("Transposition: Could not get Gestural Accidental for %i", m_accid);
    return ACCIDENTAL_GESTURAL_NONE;
}

data_ACCIDENTAL_WRITTEN TransPitch::GetAccidW() const
{
    switch (m_accid) {
        case -3: return ACCIDENTAL_WRITTEN_tf;
        case -2: return ACCIDENTAL_WRITTEN_ff;
        case -1: return ACCIDENTAL_WRITTEN_f;
        case 0: return ACCIDENTAL_WRITTEN_n;
        case 1: return ACCIDENTAL_WRITTEN_s;
        case 2: return ACCIDENTAL_WRITTEN_x;
        case 3: return ACCIDENTAL_WRITTEN_xs;
        default: break;
    }
    LogWarning("Transposition: Could not get Written Accidental for %i", m_accid);
    return ACCIDENTAL_WRITTEN_NONE;
}

data_PITCHNAME TransPitch::GetPitchName() const
{
    return static_cast<data_PITCHNAME>(m_pname + PITCHNAME_c);
}

std::wstring TransPitch::GetPitchString() const
{
    wchar_t pitchLetter = (m_pname + ('C' - 'A')) % 7 + 'A';
    switch (m_accid) {
        case -2: return std::wstring({ pitchLetter, L'𝄫' });
        case -1: return std::wstring({ pitchLetter, L'♭' });
        case 0: return std::wstring({ pitchLetter });
        case 1: return std::wstring({ pitchLetter, L'♯' });
        case 2: return std::wstring({ pitchLetter, L'♯', L'♯' });
        default: LogError("Transposition: Could not get Accidental for %i", m_accid);
    }
    return L"";
}
//////////////////////////////
//
// operator= TransPitch -- copy operator for pitches.
//

TransPitch &TransPitch::operator=(const TransPitch &pitch)
{
    if (this != &pitch) {
        m_pname = pitch.m_pname;
        m_accid = pitch.m_accid;
        m_oct = pitch.m_oct;
    }
    return *this;
}

//////////////////////////////
//
// TransPitch::operator> -- Compare two pitches diatonically (accidental alterations are ignored).
//    Higher pitches are greater than lower pitches.

bool TransPitch::operator>(const TransPitch &pitch)
{
    if (this == &pitch) return false;

    if (m_oct > pitch.m_oct) return true;
    if (m_oct < pitch.m_oct) return false;

    if (m_pname > pitch.m_pname) return true;
    return false;
}

//////////////////////////////
//
// TransPitch::operator< -- Compare two pitches diatonically (accidental alterations are ignored).
//    Lower pitches are less than higher pitches.

bool TransPitch::operator<(const TransPitch &pitch)
{
    if (this == &pitch) return false;

    if (m_oct < pitch.m_oct) return true;
    if (m_oct > pitch.m_oct) return false;

    if (m_pname < pitch.m_pname) return true;
    return false;
}

//////////////////////////////
//
// TransPitch::operator++ -- Transpose a pitch up one diatonic step.  Any chromatic alteration
//     will remain the same.  For example, B-flat 3 will be incremented to C-flat 4.

TransPitch &TransPitch::operator++()
{
    if (m_pname != dpc_B) {
        m_pname++;
    }
    else {
        m_pname = dpc_C;
        m_oct++;
    }
    return *this;
}

TransPitch TransPitch::operator++(int)
{
    TransPitch temp = *this;
    ++*this;
    return temp;
}

//////////////////////////////
//
// TransPitch::operator-- -- Transpose a pitch down one diatonic step.  Any chromatic alteration
//     will remain the same.  For example, C-sharp 4 will be decremented to B-sharp 3.

TransPitch &TransPitch::operator--()
{
    if (m_pname != dpc_C) {
        m_pname--;
    }
    else {
        m_pname = dpc_B;
        m_oct--;
    }
    return *this;
}

TransPitch TransPitch::operator--(int)
{
    TransPitch temp = *this;
    --*this;
    return temp;
}

//////////////////////////////
//
// TransPitch::IsValid -- returns true if the absolute value of the accidental
//     is less than or equal to the max value.

bool TransPitch::IsValid(int maxAccid)
{
    return abs(m_accid) <= abs(maxAccid);
}

//////////////////////////////
//
// TransPitch::SetPitch -- Set the attributes for a pitch all at once.
//

void TransPitch::SetPitch(int aPname, int anAccid, int anOct)
{
    m_pname = aPname;
    m_accid = anAccid;
    m_oct = anOct;
}

//////////////////////////////
//
// operator<< TransPitch -- Print pitch data as string for debugging.
//

std::ostream &operator<<(std::ostream &out, const TransPitch &pitch)
{
    switch (pitch.m_pname) {
        case dpc_C: out << "C"; break;
        case dpc_D: out << "D"; break;
        case dpc_E: out << "E"; break;
        case dpc_F: out << "F"; break;
        case dpc_G: out << "G"; break;
        case dpc_A: out << "A"; break;
        case dpc_B: out << "B"; break;
        default: out << "X";
    }
    if (pitch.m_accid > 0) {
        for (int i = 0; i < pitch.m_accid; i++) {
            out << "#";
        }
    }
    else if (pitch.m_accid < 0) {
        for (int i = 0; i < abs(pitch.m_accid); i++) {
            out << "b";
        }
    }
    out << pitch.m_oct;
    return out;
}

///////////////////////////////////////////////////////////////////////////

//////////////////////////////
//
// Transposer::Transposer -- Transposer constructor.
//

Transposer::Transposer()
{
    // Initialize with base-40 system by default:
    SetMaxAccid(2);
}

//////////////////////////////
//
// Transposer::~Transposer -- Transposer deconstructor.
//

Transposer::~Transposer()
{
    // do nothing;
}

//////////////////////////////
//
// Transposer::SetTransposition -- Set the transposition value which is an
//   interval class in the current base system.  When Transposer::SetMaxAccid()
//   or Transposer::SetBase*() are called, the transposition value will be set
//   to 0 (a perfect unison).  The integer is a base-40 class of number.  If you
//   want to transpose by semitone, do not use this option but rather the
//   SetTranspose(int keyFifths, string semitones) function or the
//   SetTranspose(int keyFifths, int semitones) function that are defined
//   further below.
//

bool Transposer::SetTransposition(int transVal)
{
    m_transpose = transVal;
    return true;
}

// Use a string to set the interval class in the current base system.  For example,
//  "+M2" means up a major second, which is the integer 6 in base-40.

bool Transposer::SetTransposition(const std::string &transString)
{
    m_transpose = GetInterval(transString);
    return m_transpose != INVALID_INTERVAL_CLASS;
}

// Set transposition interval based on two pitches that represent the source data
// key tonic and the target key tonic.

bool Transposer::SetTransposition(const TransPitch &fromPitch, const std::string &toString)
{
    TransPitch toPitch;
    if (GetKeyTonic(toString, toPitch)) {
        // Determine proper octave offset.
        int numSigns = toPitch.m_oct;
        m_transpose = GetInterval(fromPitch, toPitch);
        // A transposition with n plus or minus signs should never be more than n octaves away.
        if (numSigns > 0 && m_transpose > PerfectOctaveClass() * numSigns) {
            m_transpose -= PerfectOctaveClass();
        }
        else if (numSigns < 0 && m_transpose < PerfectOctaveClass() * numSigns) {
            m_transpose += PerfectOctaveClass();
        }
        // A transposition with 0 plus or minus signs should never be more than 1/2 an octave away.
        else if (numSigns == 0 && m_transpose > PerfectOctaveClass() / 2) {
            m_transpose -= PerfectOctaveClass();
        }
        else if (numSigns == 0 && m_transpose < -1 * PerfectOctaveClass() / 2) {
            m_transpose += PerfectOctaveClass();
        }
        return true;
    }
    return false;
}

// Set the transposition based on the key signature (or inferred key signature coming from
// the keySig@pname/keySig@accid/keySig@mode information) and a string containing the
// semitone transposition.

bool Transposer::SetTransposition(int keyFifths, const std::string &semitones)
{
    if (!IsValidSemitones(semitones)) {
        return false;
    }
    int semis = stoi(semitones);
    return SetTransposition(keyFifths, semis);
}

// Note the order of the variables (key signature information is first in all
// cases where there are two input parametrs to SetTransposition().

bool Transposer::SetTransposition(int keyFifths, int semitones)
{
    int intervalClass = SemitonesToIntervalClass(keyFifths, semitones);
    return SetTransposition(intervalClass);
}

//////////////////////////////
//
// Transposer::SemitonesToIntervalClass -- convert semitones plus key
//     signature information into an integer interval class.
//

int Transposer::SemitonesToIntervalClass(int keyFifths, int semitones)
{
    int sign = semitones < 0 ? -1 : +1;
    semitones = semitones < 0 ? -semitones : semitones;
    int octave = semitones / 12;
    semitones = semitones - octave * 12;
    int sum1, sum2;
    std::string interval = "P1";
    switch (semitones) {
        case 0: interval = "P1"; break;

        case 1:
            sum1 = keyFifths - 5 * sign;
            sum2 = keyFifths + 7 * sign;
            interval = abs(sum1) < abs(sum2) ? "m2" : "A1";
            break;

        case 2:
            sum1 = keyFifths + 2 * sign;
            sum2 = keyFifths - 10 * sign;
            interval = abs(sum1) < abs(sum2) ? "M2" : "d3";
            break;

        case 3:
            sum1 = keyFifths - 3 * sign;
            sum2 = keyFifths + 9 * sign;
            interval = abs(sum1) < abs(sum2) ? "m3" : "A2";
            break;

        case 4:
            sum1 = keyFifths + 4 * sign;
            sum2 = keyFifths - 8 * sign;
            interval = abs(sum1) < abs(sum2) ? "M3" : "d4";
            break;

        case 5:
            sum1 = keyFifths - 1 * sign;
            sum2 = keyFifths + 11 * sign;
            interval = abs(sum1) < abs(sum2) ? "P4" : "A3";
            break;

        case 6:
            sum1 = keyFifths + 6 * sign;
            sum2 = keyFifths - 6 * sign;
            interval = abs(sum1) < abs(sum2) ? "A4" : "d5";
            break;

        case 7:
            sum1 = keyFifths + 1 * sign;
            sum2 = keyFifths - 11 * sign;
            interval = abs(sum1) < abs(sum2) ? "P5" : "d6";
            break;

        case 8:
            sum1 = keyFifths - 4 * sign;
            sum2 = keyFifths + 8 * sign;
            interval = abs(sum1) < abs(sum2) ? "m6" : "A5";
            break;

        case 9:
            sum1 = keyFifths + 3 * sign;
            sum2 = keyFifths - 9 * sign;
            interval = abs(sum1) < abs(sum2) ? "M6" : "d7";
            break;

        case 10:
            sum1 = keyFifths - 2 * sign;
            sum2 = keyFifths + 10 * sign;
            interval = abs(sum1) < abs(sum2) ? "m7" : "A6";
            break;

        case 11:
            sum1 = keyFifths + 5 * sign;
            sum2 = keyFifths - 7 * sign;
            interval = abs(sum1) < abs(sum2) ? "M7" : "d8";
            break;
    }

    interval = sign < 0 ? "-" + interval : "+" + interval;
    int intint = GetInterval(interval);
    intint += sign * octave * m_base;
    return intint;
}

//////////////////////////////
//
// Transposer::SemitonesToIntervalName -- convert semitones plus key
//     signature information into an interval name string.
//

std::string Transposer::SemitonesToIntervalName(int keyFifths, int semitones)
{
    int intervalClass = SemitonesToIntervalClass(keyFifths, semitones);
    return GetIntervalName(intervalClass);
}

//////////////////////////////
//
// Transposer::IntervalToSemitones --  Convert a base interval class into
//   semitones.  Multiple enharmonic equivalent interval classes will collapse into
//   a single semitone value, so the process is not completely reversable
//   by calling Transposer::SemitonesToIntervalClass(), but for simple
//   intervals it will be reversable.
//

int Transposer::IntervalToSemitones(int interval)
{
    int sign = interval < 0 ? -1 : +1;
    interval = interval < 0 ? -interval : interval;
    int octave = interval / m_base;
    int intervalClass = interval - octave * m_base;
    int diatonic = 0;
    int chromatic = 0;
    IntervalToDiatonicChromatic(diatonic, chromatic, intervalClass);
    if ((diatonic != INVALID_INTERVAL_CLASS) && (chromatic != INVALID_INTERVAL_CLASS)) {
        return (m_diatonic2semitone.at(diatonic) + chromatic) * sign + 12 * octave;
    }
    else {
        return INVALID_INTERVAL_CLASS;
    }
}

//  Conversion from an interval name string into semitones:

int Transposer::IntervalToSemitones(const std::string &intervalName)
{
    int interval = GetInterval(intervalName);
    return IntervalToSemitones(interval);
}

//////////////////////////////
//
// Transposer::GetTranspositionIntervalClass -- return the interval class integer
//   that was set for use with Transposer::Transpose.
//

int Transposer::GetTranspositionIntervalClass()
{
    return m_transpose;
}

//////////////////////////////
//
// Transposer::GetTranspositionIntervalName -- return the interval integer
//   as a string name that was set for use with Transposer::Transpose.
//
std::string Transposer::GetTranspositionIntervalName()
{
    return GetIntervalName(m_transpose);
}

//////////////////////////////
//
// Transposer::Transpose -- Do a transposition at the stored transposition interval, or
//   with a temporary provided integer interval class, or a temporary interval name.
//

void Transposer::Transpose(TransPitch &pitch)
{
    int ipitch = TransPitchToIntegerPitch(pitch);
    ipitch += m_transpose;
    pitch = IntegerPitchToTransPitch(ipitch);
}

int Transposer::Transpose(int ipitch)
{
    return ipitch + m_transpose;
}

// Use a temporary transposition value in the following
// two functions. To save for later use of Transposer::Transpose
// without specifying the transposition interval, store
// transposition value with Transposer::SetTransposition() first.

void Transposer::Transpose(TransPitch &pitch, int transVal)
{
    int ipitch = TransPitchToIntegerPitch(pitch);
    ipitch += transVal;
    pitch = IntegerPitchToTransPitch(ipitch);
}

void Transposer::Transpose(TransPitch &pitch, const std::string &transString)
{
    int transVal = GetInterval(transString);
    int ipitch = TransPitchToIntegerPitch(pitch);
    ipitch += transVal;
    pitch = IntegerPitchToTransPitch(ipitch);
}

//////////////////////////////
//
// Transposer::GetBase -- Return the integer interval class representing an octave.
//

int Transposer::GetBase()
{
    return m_base;
}

//////////////////////////////
//
// Transposer::GetMaxAccid -- Return the maximum possible absolute accidental value
//     that can be represented by the current transposition base.
//

int Transposer::GetMaxAccid()
{
    return m_maxAccid;
}

//////////////////////////////
//
// Transposer::SetMaxAccid -- Calculate variables related to a specific base system.
//

void Transposer::SetMaxAccid(int maxAccid)
{
    m_maxAccid = abs(maxAccid);
    m_base = 7 * (2 * m_maxAccid + 1) + 5;
    CalculateDiatonicMapping();
    m_transpose = 0;
}

//////////////////////////////
//
// Transposer::CalculateDiatonicMaping -- Calculate the integer values for the
//    natural diatonic pitch classes: C, D, E, F, G, A, and B in the current
//    base system.
//

void Transposer::CalculateDiatonicMapping()
{
    int m2 = m_maxAccid * 2 + 1;
    int M2 = m2 + 1;
    m_diatonicMapping.resize(7);
    m_diatonicMapping[dpc_C] = m_maxAccid;
    m_diatonicMapping[dpc_D] = m_diatonicMapping[dpc_C] + M2;
    m_diatonicMapping[dpc_E] = m_diatonicMapping[dpc_D] + M2;
    m_diatonicMapping[dpc_F] = m_diatonicMapping[dpc_E] + m2;
    m_diatonicMapping[dpc_G] = m_diatonicMapping[dpc_F] + M2;
    m_diatonicMapping[dpc_A] = m_diatonicMapping[dpc_G] + M2;
    m_diatonicMapping[dpc_B] = m_diatonicMapping[dpc_A] + M2;
}

//////////////////////////////
//
// Transposer::GetKeyTonic -- Convert a key tonic string into a TransPitch
//      where the octave is the direction it should go.
//      Should conform to the following regular expression:
//          ([+]*|[-]*)([A-Ga-g])([Ss#]*|[Ffb]*)

bool Transposer::GetKeyTonic(const std::string &keyTonic, TransPitch &tonic)
{
    int octave = 0;
    int pitch = 0;
    int accid = 0;
    int state = 0;
    for (unsigned int i = 0; i < (unsigned int)keyTonic.size(); i++) {
        switch (state) {
            case 0:
                switch (keyTonic[i]) {
                    case '-': octave--; break;
                    case '+': octave++; break;
                    default:
                        state++;
                        i--;
                        break;
                }
                break;
            case 1:
                state++;
                switch (keyTonic[i]) {
                    case 'C':
                    case 'c': pitch = 0; break;
                    case 'D':
                    case 'd': pitch = 1; break;
                    case 'E':
                    case 'e': pitch = 2; break;
                    case 'F':
                    case 'f': pitch = 3; break;
                    case 'G':
                    case 'g': pitch = 4; break;
                    case 'A':
                    case 'a': pitch = 5; break;
                    case 'B':
                    case 'b': pitch = 6; break;
                    default: LogWarning("Invalid keytonic pitch character: %c", keyTonic[i]); return false;
                }
                break;
            case 2:
                switch (keyTonic[i]) {
                    case 'F':
                    case 'f':
                    case 'b': accid--; break;
                    case 'S':
                    case 's':
                    case '#': accid++; break;
                    default: LogWarning("Invalid keytonic accid character: %c", keyTonic[i]); return false;
                }
                break;
        }
    }

    tonic = TransPitch(pitch, accid, octave);
    return true;
}

//////////////////////////////
//
// Transposer::GetInterval -- Convert a diatonic interval with chromatic
//     quality and direction into an integer interval class.   Input string
//     is in the format: direction + quality + diatonic interval.
//     Such as +M2 for up a major second, -P5 is down a perfect fifth.
//     Regular expression that the string should conform to:
//            (-|\+?)([Pp]|M|m|[aA]+|[dD]+)(\d+)
//

int Transposer::GetInterval(const std::string &intervalName)
{
    std::string direction;
    std::string quality;
    std::string number;
    int state = 0;

    for (int i = 0; i < (int)intervalName.size(); i++) {
        switch (state) {
            case 0: // direction or quality expected
                switch (intervalName[i]) {
                    case '-': // interval is down
                        direction = "-";
                        state++;
                        break;
                    case '+': // interval is up
                        direction += "";
                        state++;
                        break;
                    default: // interval is up by default
                        direction += "";
                        state++;
                        i--;
                        break;
                }
                break;

            case 1: // quality expected
                if (std::isdigit(intervalName[i])) {
                    state++;
                    i--;
                }
                else {
                    switch (intervalName[i]) {
                        case 'M': // major
                            quality = "M";
                            break;
                        case 'm': // minor
                            quality = "m";
                            break;
                        case 'P': // perfect
                        case 'p': quality = "P"; break;
                        case 'D': // diminished
                        case 'd': quality += "d"; break;
                        case 'A': // augmented
                        case 'a': quality += "A"; break;
                    }
                }
                break;

            case 2: // digit expected
                if (std::isdigit(intervalName[i])) {
                    number += intervalName[i];
                }
                break;
        }
    }

    if (quality.empty()) {
        LogError("Interval name requires a chromatic quality: %s", intervalName.c_str());
        return INVALID_INTERVAL_CLASS;
    }

    if (number.empty()) {
        LogError("Interval name requires a diatonic interval number: %s", intervalName.c_str());
        return INVALID_INTERVAL_CLASS;
    }

    int dnum = stoi(number);
    if (dnum == 0) {
        LogError("Integer interval number cannot be zero: %s", intervalName.c_str());
        return INVALID_INTERVAL_CLASS;
    }
    dnum--;
    int octave = dnum / 7;
    dnum = dnum - octave * 7;

    int base = 0;
    int adjust = 0;

    switch (dnum) {
        case 0: // unison
            base = PerfectUnisonClass();
            if (quality[0] == 'A') {
                adjust = (int)quality.size();
            }
            else if (quality[0] == 'd') {
                adjust = -(int)quality.size();
            }
            else if (quality != "P") {
                LogError("Error in interval quality: %s", intervalName.c_str());
                return INVALID_INTERVAL_CLASS;
            }
            break;
        case 1: // second
            if (quality == "M") {
                base = MajorSecondClass();
            }
            else if (quality == "m") {
                base = MinorSecondClass();
            }
            else if (quality[0] == 'A') {
                base = MajorSecondClass();
                adjust = (int)quality.size();
            }
            else if (quality[0] == 'd') {
                base = MinorSecondClass();
                adjust = -(int)quality.size();
            }
            else {
                LogError("Error in interval quality: %s", intervalName.c_str());
                return INVALID_INTERVAL_CLASS;
            }
            break;
        case 2: // third
            if (quality == "M") {
                base = MajorThirdClass();
            }
            else if (quality == "m") {
                base = MinorThirdClass();
            }
            else if (quality[0] == 'A') {
                base = MajorThirdClass();
                adjust = (int)quality.size();
            }
            else if (quality[0] == 'd') {
                base = MinorThirdClass();
                adjust = -(int)quality.size();
            }
            else {
                LogError("Error in interval quality: %s", intervalName.c_str());
                return INVALID_INTERVAL_CLASS;
            }
            break;
        case 3: // fourth
            base = PerfectFourthClass();
            if (quality[0] == 'A') {
                adjust = (int)quality.size();
            }
            else if (quality[0] == 'd') {
                adjust = -(int)quality.size();
            }
            else if (quality != "P") {
                LogError("Error in interval quality: %s", intervalName.c_str());
                return INVALID_INTERVAL_CLASS;
            }
            break;
        case 4: // fifth
            base = PerfectFifthClass();
            if (quality[0] == 'A') {
                adjust = (int)quality.size();
            }
            else if (quality[0] == 'd') {
                adjust = -(int)quality.size();
            }
            else if (quality != "P") {
                LogError("Error in interval quality: %s", intervalName.c_str());
                return INVALID_INTERVAL_CLASS;
            }
            break;
        case 5: // sixth
            if (quality == "M") {
                base = MajorSixthClass();
            }
            else if (quality == "m") {
                base = MinorSixthClass();
            }
            else if (quality[0] == 'A') {
                base = MajorSixthClass();
                adjust = (int)quality.size();
            }
            else if (quality[0] == 'd') {
                base = MinorSixthClass();
                adjust = -(int)quality.size();
            }
            else {
                LogError("Error in interval quality: %s", intervalName.c_str());
                return INVALID_INTERVAL_CLASS;
            }
            break;
        case 6: // seventh
            if (quality == "M") {
                base = MajorSeventhClass();
            }
            else if (quality == "m") {
                base = MinorSeventhClass();
            }
            else if (quality[0] == 'A') {
                base = MajorSeventhClass();
                adjust = (int)quality.size();
            }
            else if (quality[0] == 'd') {
                base = MinorSeventhClass();
                adjust = -(int)quality.size();
            }
            else {
                LogError("Error in interval quality: %s", intervalName.c_str());
                return INVALID_INTERVAL_CLASS;
            }
            break;
    }

    if (direction == "-") {
        return -((octave * m_base) + base + adjust);
    }
    else {
        return (octave * m_base) + base + adjust;
    }
}

//////////////////////////////
//
// Transposer::PerfectUnisonClass -- Return the integer interval class
//     representing a perfect unison.
//

int Transposer::PerfectUnisonClass()
{
    return 0;
}

//////////////////////////////
//
// Transposer::MinorSecondClass -- Return the integer interval class
//     representing a minor second.
//

int Transposer::MinorSecondClass()
{
    return m_diatonicMapping[3] - m_diatonicMapping[2]; // F - E
}

//////////////////////////////
//
// Transposer::MajorSecondClass -- Return the integer interval class
//    representing a major second.
//

int Transposer::MajorSecondClass()
{
    return m_diatonicMapping[1] - m_diatonicMapping[0]; // D - C
}

//////////////////////////////
//
// Transposer::MinorThirdClass -- Return the integer interval class
//    representing a minor third.
//

int Transposer::MinorThirdClass()
{
    return m_diatonicMapping[3] - m_diatonicMapping[1]; // F - D
}

//////////////////////////////
//
// Transposer::MajorThirdClass -- Return the integer interval class
//    representing a major third.
//

int Transposer::MajorThirdClass()
{
    return m_diatonicMapping[2] - m_diatonicMapping[0]; // E - C
}

//////////////////////////////
//
// Transposer::PerfectFourthClass -- Return the integer interval class
//    representing a perfect fourth.
//

int Transposer::PerfectFourthClass()
{
    return m_diatonicMapping[3] - m_diatonicMapping[0]; // F - C
}

//////////////////////////////
//
// Transposer::PerfectFifthClass -- Return the integer interval class
//    representing a perfect fifth.
//

int Transposer::PerfectFifthClass()
{
    return m_diatonicMapping[4] - m_diatonicMapping[0]; // G - C
}

//////////////////////////////
//
// Transposer::MinorSixthClass -- Return the integer interval class
//    representing a minor sixth.
//

int Transposer::MinorSixthClass()
{
    return m_diatonicMapping[5] - m_diatonicMapping[0] - 1; // A - C - 1;
}

//////////////////////////////
//
// Transposer::MajorSixthClass -- Return the integer interval class
//    representing a major sixth.
//

int Transposer::MajorSixthClass()
{
    return m_diatonicMapping[5] - m_diatonicMapping[0]; // A - C
}

//////////////////////////////
//
// Transposer::MinorSeventhClass -- Return the integer interval class
//    representing a minor sixth.
//

int Transposer::MinorSeventhClass()
{
    return m_diatonicMapping[6] - m_diatonicMapping[0] - 1; // B - C - 1
}

//////////////////////////////
//
// Transposer::MajorSeventhClass -- Return the integer interval class
//    representing a major sixth.
//

int Transposer::MajorSeventhClass()
{
    return m_diatonicMapping[6] - m_diatonicMapping[0]; // B - C
}

//////////////////////////////
//
// Transposer::octaveClass -- Return the integer interval class
//    representing a major second.
//

int Transposer::PerfectOctaveClass()
{
    return m_base;
}

//////////////////////////////
//
// Transposer::TransPitchToIntegerPitch -- Convert a pitch (octave/diatonic pitch class/chromatic
//     alteration) into an integer value according to the current base.
//

int Transposer::TransPitchToIntegerPitch(const TransPitch &pitch)
{
    return pitch.m_oct * m_base + m_diatonicMapping[pitch.m_pname] + pitch.m_accid;
}

//////////////////////////////
//
// Transposer::IntegerPitchToTransPitch -- Convert an integer within the current base
//    into a pitch (octave/diatonic pitch class/chromatic alteration).  Pitches
//    with negative octaves will have to be tested.
//

TransPitch Transposer::IntegerPitchToTransPitch(int ipitch)
{
    TransPitch pitch;
    pitch.m_oct = ipitch / m_base;
    int chroma = ipitch - pitch.m_oct * m_base;
    int mindiff = -1000;
    int mini = -1;

    int targetdiff = m_maxAccid;

    if (chroma > m_base / 2) {
        // search from B downwards
        mindiff = chroma - m_diatonicMapping.back();
        mini = (int)m_diatonicMapping.size() - 1;
        for (int i = (int)m_diatonicMapping.size() - 2; i >= 0; i--) {
            int diff = chroma - m_diatonicMapping[i];
            if (abs(diff) < abs(mindiff)) {
                mindiff = diff;
                mini = i;
            }
            if (abs(mindiff) <= targetdiff) {
                break;
            }
        }
    }
    else {
        // search from C upwards
        mindiff = chroma - m_diatonicMapping[0];
        mini = 0;
        for (int i = 1; i < (int)m_diatonicMapping.size(); i++) {
            int diff = chroma - m_diatonicMapping[i];
            if (abs(diff) < abs(mindiff)) {
                mindiff = diff;
                mini = i;
            }
            if (abs(mindiff) <= targetdiff) {
                break;
            }
        }
    }
    pitch.m_pname = mini;
    pitch.m_accid = mindiff;
    return pitch;
}

//////////////////////////////
//
// Transposer::SetBase40 -- Standard chromatic alteration mode, allowing up to double sharp/flats.
//

void Transposer::SetBase40()
{
    SetMaxAccid(2);
}

//////////////////////////////
//
// Transposer::SetBase600 -- Extended chromatic alteration mode, allowing up to 42 sharp/flats.
//

void Transposer::SetBase600()
{
    SetMaxAccid(42);
}

//////////////////////////////
//
// Transposer::GetInterval -- Return the interval between two pitches.
//    If the second pitch is higher than the first, then the interval will be
//    positive; otherwise, the interval will be negative.
//

int Transposer::GetInterval(const TransPitch &p1, const TransPitch &p2)
{
    return TransPitchToIntegerPitch(p2) - TransPitchToIntegerPitch(p1);
}

// Similar function to GetInterval, but the integer interval class is converted
// into a string that is not dependent on a base:

std::string Transposer::GetIntervalName(const TransPitch &p1, const TransPitch &p2)
{
    int iclass = GetInterval(p1, p2);
    return GetIntervalName(iclass);
}

std::string Transposer::GetIntervalName(int intervalClass)
{
    std::string direction;
    if (intervalClass < 0) {
        direction = "-";
        intervalClass = -intervalClass;
    }

    int octave = intervalClass / m_base;
    int chroma = intervalClass - octave * m_base;

    int mindiff = chroma;
    int mini = 0;
    for (int i = 1; i < (int)m_diatonicMapping.size(); i++) {
        int diff = chroma - (m_diatonicMapping[i] - m_diatonicMapping[0]);
        if (abs(diff) < abs(mindiff)) {
            mindiff = diff;
            mini = i;
        }
        if (abs(mindiff) <= m_maxAccid) {
            break;
        }
    }

    int number = INVALID_INTERVAL_CLASS;
    int diminished = 0;
    int augmented = 0;
    std::string quality;

    switch (mini) {
        case 0: // unison
            number = 1;
            if (mindiff == 0) {
                quality = "P";
            }
            else if (mindiff < 0) {
                diminished = -mindiff;
            }
            else if (mindiff > 0) {
                augmented = mindiff;
            }
            break;
        case 1: // second
            number = 2;
            if (mindiff == 0) {
                quality = "M";
            }
            else if (mindiff == -1) {
                quality = "m";
            }
            else if (mindiff < 0) {
                diminished = -mindiff - 1;
            }
            else if (mindiff > 0) {
                augmented = mindiff;
            }
            break;
        case 2: // third
            number = 3;
            if (mindiff == 0) {
                quality = "M";
            }
            else if (mindiff == -1) {
                quality = "m";
            }
            else if (mindiff < 0) {
                diminished = -mindiff - 1;
            }
            else if (mindiff > 0) {
                augmented = mindiff;
            }
            break;
        case 3: // fourth
            number = 4;
            if (mindiff == 0) {
                quality = "P";
            }
            else if (mindiff < 0) {
                diminished = -mindiff;
            }
            else if (mindiff > 0) {
                augmented = mindiff;
            }
            break;
        case 4: // fifth
            number = 5;
            if (mindiff == 0) {
                quality = "P";
            }
            else if (mindiff < 0) {
                diminished = -mindiff;
            }
            else if (mindiff > 0) {
                augmented = mindiff;
            }
            break;
        case 5: // sixth
            number = 6;
            if (mindiff == 0) {
                quality = "M";
            }
            else if (mindiff == -1) {
                quality = "m";
            }
            else if (mindiff < 0) {
                diminished = -mindiff - 1;
            }
            else if (mindiff > 0) {
                augmented = mindiff;
            }
            break;
        case 6: // seventh
            number = 7;
            if (mindiff == 0) {
                quality = "M";
            }
            else if (mindiff == -1) {
                quality = "m";
            }
            else if (mindiff < 0) {
                diminished = -mindiff - 1;
            }
            else if (mindiff > 0) {
                augmented = mindiff;
            }
            break;
    }

    if (quality.empty()) {
        if (augmented) {
            for (int i = 0; i < augmented; i++) {
                quality += "A";
            }
        }
        else if (diminished) {
            for (int i = 0; i < diminished; i++) {
                quality += "d";
            }
        }
        else {
            quality = "?";
        }
    }

    number += octave * 7;

    std::string output = direction;
    output += quality;
    output += std::to_string(number);

    return output;
}

//////////////////////////////
//
// Transposer::IntervalToCircleOfFifths -- Returns the circle-of-fiths count
//    that is represented by the given interval class or interval string.
//    Examples:  "P5"  => +1      "-P5" => -1
//               "P4"  => -1      "-P4" => +1
//               "M2"  => +2      "m7"  => -2
//               "M6"  => +3      "m3"  => -3
//               "M3"  => +4      "m6"  => -4
//               "M7"  => +5      "m2"  => -5
//               "A4"  => +6      "d5"  => -6
//               "A1"  => +7      "d1"  => -7
//
// If a key-signature plus the transposition interval in circle-of-fifths format
// is greater than +/-7, Then the -/+ 7 should be added to the key signature to
// avoid double sharp/flats in the key signature (and the transposition interval
// should be adjusted accordingly).
//

int Transposer::IntervalToCircleOfFifths(const std::string &transstring)
{
    int intervalClass = GetInterval(transstring);
    return IntervalToCircleOfFifths(intervalClass);
}

int Transposer::IntervalToCircleOfFifths(int transval)
{
    if (transval < 0) {
        transval = (m_base * 100 + transval) % m_base;
    }
    else if (transval == 0) {
        return 0;
    }
    else {
        transval %= m_base;
    }

    int p5 = PerfectFifthClass();
    int p4 = PerfectFourthClass();
    for (int i = 1; i < m_base; i++) {
        if ((p5 * i) % m_base == transval) {
            return i;
        }
        if ((p4 * i) % m_base == transval) {
            return -i;
        }
    }
    return INVALID_INTERVAL_CLASS;
}

//////////////////////////////
//
// Transposer::CircleOfFifthsToIntervalClass -- Inputs a circle-of-fifths value and
//   returns the interval class as an integer in the current base.
//

int Transposer::CircleOfFifthsToIntervalClass(int fifths)
{
    if (fifths == 0) {
        return 0;
    }
    else if (fifths > 0) {
        return (PerfectFifthClass() * fifths) % m_base;
    }
    else {
        return (PerfectFourthClass() * (-fifths)) % m_base;
    }
}

//////////////////////////////
//
// Transposer::CircleOfFifthsToIntervalName -- Convert a circle-of-fifths position
//    into an interval string.
//

std::string Transposer::CircleOfFifthsToIntervalName(int fifths)
{
    int intervalClass = CircleOfFifthsToIntervalClass(fifths);
    return GetIntervalName(intervalClass);
}

//////////////////////////////
//
// Transposer::CircleOfFifthsToMajorTonic -- Return the tonic
//    of the major key that has the given key signature.  Return
//    value is in the 0th octave.
//

TransPitch Transposer::CircleOfFifthsToMajorTonic(int fifths)
{
    int intervalClass = CircleOfFifthsToIntervalClass(fifths);
    return IntegerPitchToTransPitch((GetCPitchClass() + intervalClass) % GetBase());
}

//////////////////////////////
//
// Transposer::CircleOfFifthsToMinorTonic -- Return the tonic
//    of the minor key that has the given key signature.  Return
//    value is in the 0th octave.
//

TransPitch Transposer::CircleOfFifthsToMinorTonic(int fifths)
{
    int intervalClass = CircleOfFifthsToIntervalClass(fifths);
    return IntegerPitchToTransPitch((GetAPitchClass() + intervalClass) % GetBase());
}

//////////////////////////////
//
// Transposer::CircleOfFifthsToDorianTonic -- Return the tonic
//    of the dorian key that has the given key signature.  Return
//    value is in the 0th octave.
//

TransPitch Transposer::CircleOfFifthsToDorianTonic(int fifths)
{
    int intervalClass = CircleOfFifthsToIntervalClass(fifths);
    return IntegerPitchToTransPitch((GetDPitchClass() + intervalClass) % GetBase());
}

//////////////////////////////
//
// Transposer::CircleOfFifthsToPhrygianTonic -- Return the tonic
//    of the phrygian key that has the given key signature.  Return
//    value is in the 0th octave.
//

TransPitch Transposer::CircleOfFifthsToPhrygianTonic(int fifths)
{
    int intervalClass = CircleOfFifthsToIntervalClass(fifths);
    return IntegerPitchToTransPitch((GetEPitchClass() + intervalClass) % GetBase());
}

//////////////////////////////
//
// Transposer::CircleOfFifthsToLydianTonic -- Return the tonic
//    of the lydian key that has the given key signature.  Return
//    value is in the 0th octave.
//

TransPitch Transposer::CircleOfFifthsToLydianTonic(int fifths)
{
    int intervalClass = CircleOfFifthsToIntervalClass(fifths);
    return IntegerPitchToTransPitch((GetFPitchClass() + intervalClass) % GetBase());
}

//////////////////////////////
//
// Transposer::CircleOfFifthsToMixolydianTonic -- Return the tonic
//    of the mixolydian key that has the given key signature.  Return
//    value is in the 0th octave.
//

TransPitch Transposer::CircleOfFifthsToMixolydianTonic(int fifths)
{
    int intervalClass = CircleOfFifthsToIntervalClass(fifths);
    return IntegerPitchToTransPitch((GetGPitchClass() + intervalClass) % GetBase());
}

//////////////////////////////
//
// Transposer::CircleOfFifthsToLocrianTonic -- Return the tonic
//    of the locrian key that has the given key signature.  Return
//    value is in the 0th octave.
//

TransPitch Transposer::CircleOfFifthsToLocrianTonic(int fifths)
{
    int intervalClass = CircleOfFifthsToIntervalClass(fifths);
    return IntegerPitchToTransPitch((GetBPitchClass() + intervalClass) % GetBase());
}

//////////////////////////////
//
// Transposer::DiatonicChromaticToIntervalClass -- Convert a diatonic/chromatic interval
//    into a base-n interval class integer.
//      +1D +1C = m2
//      +1D +2C = M2
//      +1D +3C = A2
//      +2D +4C = M3
//      +2D +3C = m3
//      +2D +2C = m3
//      +2D +1C = d3
//      +3D +5C = P4
//      +3D +6C = A4
//      +3D +4C = d4
//
//

std::string Transposer::DiatonicChromaticToIntervalName(int diatonic, int chromatic)
{
    if (diatonic == 0) {
        std::string output;
        if (chromatic == 0) {
            output += "P";
        }
        else if (chromatic > 0) {
            for (int i = 0; i < chromatic; i++) {
                output += "A";
            }
        }
        else {
            for (int i = 0; i < -chromatic; i++) {
                output += "d";
            }
        }
        output += "1";
        return output;
    }

    int octave = 0;
    std::string direction;
    if (diatonic < 0) {
        direction = "-";
        octave = -diatonic / 7;
        diatonic = (-diatonic - octave * 7);
        chromatic = -chromatic;
    }
    else {
        octave = diatonic / 7;
        diatonic = diatonic - octave * 7;
    }

    int augmented = 0;
    int diminished = 0;
    std::string quality;

    switch (abs(diatonic)) {
        case 0: // unsion
            if (chromatic == 0) {
                quality = "P";
            }
            else if (chromatic > 0) {
                augmented = chromatic;
            }
            else {
                diminished = chromatic;
            }
            break;
        case 1: // second
            if (chromatic == 2) {
                quality = "M";
            }
            else if (chromatic == 1) {
                quality = "m";
            }
            else if (chromatic > 2) {
                augmented = chromatic - 2;
            }
            else {
                diminished = chromatic - 1;
            }
            break;
        case 2: // third
            if (chromatic == 4) {
                quality = "M";
            }
            else if (chromatic == 3) {
                quality = "m";
            }
            else if (chromatic > 4) {
                augmented = chromatic - 4;
            }
            else {
                diminished = chromatic - 3;
            }
            break;
        case 3: // fourth
            if (chromatic == 5) {
                quality = "P";
            }
            else if (chromatic > 5) {
                augmented = chromatic - 5;
            }
            else {
                diminished = chromatic - 5;
            }
            break;
        case 4: // fifth
            if (chromatic == 7) {
                quality = "P";
            }
            else if (chromatic > 7) {
                augmented = chromatic - 7;
            }
            else {
                diminished = chromatic - 7;
            }
            break;
        case 5: // sixth
            if (chromatic == 9) {
                quality = "M";
            }
            else if (chromatic == 8) {
                quality = "m";
            }
            else if (chromatic > 9) {
                augmented = chromatic - 9;
            }
            else {
                diminished = chromatic - 8;
            }
            break;
        case 6: // seventh
            if (chromatic == 11) {
                quality = "M";
            }
            else if (chromatic == 10) {
                quality = "m";
            }
            else if (chromatic > 11) {
                augmented = chromatic - 11;
            }
            else {
                diminished = chromatic - 10;
            }
            break;
    }

    augmented = abs(augmented);
    diminished = abs(diminished);

    if (quality.empty()) {
        if (augmented) {
            for (int i = 0; i < augmented; i++) {
                quality += "A";
            }
        }
        else if (diminished) {
            for (int i = 0; i < diminished; i++) {
                quality += "d";
            }
        }
    }

    return direction + quality + std::to_string(octave * 7 + diatonic + 1);
}

//////////////////////////////
//
// Transposer::DiatonicChromaticToIntervalClass --
//

int Transposer::DiatonicChromaticToIntervalClass(int diatonic, int chromatic)
{
    std::string intervalName = DiatonicChromaticToIntervalName(diatonic, chromatic);
    return GetInterval(intervalName);
}

//////////////////////////////
//
// Transposer::IntervalToDiatonicChromatic --
//

void Transposer::IntervalToDiatonicChromatic(int &diatonic, int &chromatic, int intervalClass)
{
    std::string intervalName = GetIntervalName(intervalClass);
    IntervalToDiatonicChromatic(diatonic, chromatic, intervalName);
}

void Transposer::IntervalToDiatonicChromatic(int &diatonic, int &chromatic, const std::string &intervalName)
{
    int direction = 1;
    std::string quality;
    std::string number;
    int state = 0;

    for (int i = 0; i < (int)intervalName.size(); i++) {
        switch (state) {
            case 0: // direction or quality expected
                switch (intervalName[i]) {
                    case '-': // interval is down
                        direction = -1;
                        state++;
                        break;
                    case '+': // interval is up
                        direction = 1;
                        state++;
                        break;
                    default: // interval is up by default
                        direction = 1;
                        state++;
                        i--;
                        break;
                }
                break;

            case 1: // quality expected
                if (std::isdigit(intervalName[i])) {
                    state++;
                    i--;
                }
                else {
                    switch (intervalName[i]) {
                        case 'M': // major
                            quality = "M";
                            break;
                        case 'm': // minor
                            quality = "m";
                            break;
                        case 'P': // perfect
                        case 'p': quality = "P"; break;
                        case 'D': // diminished
                        case 'd': quality += "d"; break;
                        case 'A': // augmented
                        case 'a': quality += "A"; break;
                    }
                }
                break;

            case 2: // digit expected
                if (std::isdigit(intervalName[i])) {
                    number += intervalName[i];
                }
                break;
        }
    }

    if (quality.empty()) {
        LogError("Interval requires a chromatic quality: %s", intervalName.c_str());
        chromatic = INVALID_INTERVAL_CLASS;
        diatonic = INVALID_INTERVAL_CLASS;
        return;
    }

    if (number.empty()) {
        LogError("Interval requires a diatonic interval number: %s", intervalName.c_str());
        chromatic = INVALID_INTERVAL_CLASS;
        diatonic = INVALID_INTERVAL_CLASS;
        return;
    }

    int dnum = stoi(number);
    if (dnum == 0) {
        LogError("Integer interval number cannot be zero: %s", intervalName.c_str());
        chromatic = INVALID_INTERVAL_CLASS;
        diatonic = INVALID_INTERVAL_CLASS;
        return;
    }
    dnum--;
    int octave = dnum / 7;
    dnum = dnum - octave * 7;

    diatonic = direction * (octave * 7 + dnum);
    chromatic = 0;

    switch (dnum) {
        case 0: // unison
            if (quality[0] == 'A') {
                chromatic = (int)quality.size();
            }
            else if (quality[0] == 'd') {
                chromatic = -(int)quality.size();
            }
            else if (quality == "P") {
                chromatic = 0;
            }
            else {
                LogError("Error in Interval quality: %s", intervalName.c_str());
                chromatic = INVALID_INTERVAL_CLASS;
                diatonic = INVALID_INTERVAL_CLASS;
                return;
            }
            break;
        case 1: // second
            if (quality == "M") {
                chromatic = 2;
            }
            else if (quality == "m") {
                chromatic = 1;
            }
            else if (quality[0] == 'A') {
                chromatic = 2 + (int)quality.size();
            }
            else if (quality[0] == 'd') {
                chromatic = 1 - (int)quality.size();
            }
            else {
                LogError("Error in Interval quality: %s", intervalName.c_str());
                chromatic = INVALID_INTERVAL_CLASS;
                diatonic = INVALID_INTERVAL_CLASS;
                return;
            }
            break;
        case 2: // third
            if (quality == "M") {
                chromatic = 4;
            }
            else if (quality == "m") {
                chromatic = 3;
            }
            else if (quality[0] == 'A') {
                chromatic = 4 + (int)quality.size();
            }
            else if (quality[0] == 'd') {
                chromatic = 3 - (int)quality.size();
            }
            else {
                LogError("Error in Interval quality: %s", intervalName.c_str());
                chromatic = INVALID_INTERVAL_CLASS;
                diatonic = INVALID_INTERVAL_CLASS;
                return;
            }
            break;
        case 3: // fourth
            if (quality[0] == 'A') {
                chromatic = 5 + (int)quality.size();
            }
            else if (quality[0] == 'd') {
                chromatic = 5 - (int)quality.size();
            }
            else if (quality == "P") {
                chromatic = 5;
            }
            else {
                LogError("Error in Interval quality: %s", intervalName.c_str());
                chromatic = INVALID_INTERVAL_CLASS;
                diatonic = INVALID_INTERVAL_CLASS;
                return;
            }
            break;
        case 4: // fifth
            if (quality[0] == 'A') {
                chromatic = 7 + (int)quality.size();
            }
            else if (quality[0] == 'd') {
                chromatic = 7 - (int)quality.size();
            }
            else if (quality == "P") {
                chromatic = 7;
            }
            else {
                LogError("Error in Interval quality: %s", intervalName.c_str());
                chromatic = INVALID_INTERVAL_CLASS;
                diatonic = INVALID_INTERVAL_CLASS;
                return;
            }
            break;
        case 5: // sixth
            if (quality == "M") {
                chromatic = 9;
            }
            else if (quality == "m") {
                chromatic = 8;
            }
            else if (quality[0] == 'A') {
                chromatic = 9 + (int)quality.size();
            }
            else if (quality[0] == 'd') {
                chromatic = 8 - (int)quality.size();
            }
            else {
                LogError("Error in Interval quality: %s", intervalName.c_str());
                chromatic = INVALID_INTERVAL_CLASS;
                diatonic = INVALID_INTERVAL_CLASS;
                return;
            }
            break;
        case 6: // seventh
            if (quality == "M") {
                chromatic = 11;
            }
            else if (quality == "m") {
                chromatic = 10;
            }
            else if (quality[0] == 'A') {
                chromatic = 11 + (int)quality.size();
            }
            else if (quality[0] == 'd') {
                chromatic = 10 - (int)quality.size();
            }
            else {
                LogError("Error in Interval quality: %s", intervalName.c_str());
                chromatic = INVALID_INTERVAL_CLASS;
                diatonic = INVALID_INTERVAL_CLASS;
                return;
            }
            break;
    }
    chromatic *= direction;
}

//////////////////////////////
//
// Transposer::IsValidIntervalName -- Returns true if the input string
//    is a valid chromatic interval string.  A valid interval name will match
//    this regular expression:
//          (-|\+?)([Pp]|M|m|[aA]+|[dD]+)([1-9][0-9]*)
//
//    Components of the regular expression:
//
//    1:  (-|\+?) == an optional direction for the interval.  When there
//                   is no sign, a + sign is implied.  A sign on a unison (P1)
//                   will be ignored.
//    2:  ([Pp]|M|m|[aA]+|[dD]+) == The chromatic quality of the following
//                   diatonic interval number.  Meanings of the letters:
//                      P or p = perfect
//                      M      = major
//                      m      = minor
//                      A or a = augmented
//                      d or D = diminished
//                   unisons (1), fourths, fifths and octaves (8) and octave multiples
//                   of these intervals can be prefixed by P but not by M or m.  Seconds,
//                   thirds, sixths, sevenths and octave transpositions of those intervals
//                   can be prefixed by M and m but not by P.  All intervals can be prefixed
//                   with A or d (or AA/dd for doubly augmented/diminished, etc.).  M and m
//                   are case sensitive, but P, A, and d are case insensitive.  This function
//                   does not check the correct pairing of M/m and P for the diatonic intervals
//                   (such as the invalid interval construct P3 for a perfect third).
//                   Transposer::GetInterval(const std::string &intervalName) will do a
//                   more thorough check for invalid pairings.  This function is used mainly to
//                   determine whether an interval or a key tonic is being used in the --transpose
//                   option for verovio.
//     3: ([1-9][0-9]*) == a positive integer representing the diatonic interval.  1 = unison,
//                   2 = second, 3 = third, and so on.  Compound intervals are allowed, such as
//                   9 for a nineth (2nd plus a perfect octave), 15 for two perfect octaves.
//
//

bool Transposer::IsValidIntervalName(const std::string &name)
{
    std::string pattern = "(-|\\+?)([Pp]|M|m|[aA]+|[dD]+)([1-9][0-9]*)";
    if (std::regex_search(name, std::regex(pattern))) {
        return true;
    }
    else {
        return false;
    }
}

//////////////////////////////
//
// Transposer::IsValidSemitones -- Returns true if the input string
//    is a valid semitone interval string.  A valid interval name will match
//    this regular expression:
//          ^(-|\+?)(\d+)$
//
//    Components of the regular expression:
//
//    1:  (-|\+?) == an optional direction for the interval.  When there
//                   is no sign, a + sign is implied.  A sign on 0
//                   will be ignored.
//    2:  (\d+)   == The number of semitones.  0 means transpose at the
//                   unison (i.e., transpose to same pitch as input).
//                   12 means an octave, 14 means a second plus an octave,
//                   24 means two octaves.
//

bool Transposer::IsValidSemitones(const std::string &name)
{
    std::string pattern = "^(-|\\+?)(\\d+)$";
    if (std::regex_search(name, std::regex(pattern))) {
        return true;
    }
    else {
        return false;
    }
}

//////////////////////////////
//
// Transposer::IsValidKeyTonicName -- Returns true if the input string
//    is a valid key tonic which can be used to calculate a transposition
//    interval based on the current key.  A valid key tonic will match
//    this regular expression:
//          ([+]*|[-]*)([A-Ga-g])([Ss#]*|[Ffb]*)
//
//    Components of the regular expression:
//
//    1: ([+]*|[-]*) == An optional sign for the direction of the
//                      transposition.  If there is no sign, then
//                      the closest tonic pitch class to the tonic
//                      of the data will be selected.  When The
//                      sign is double/tripled/etc., additional
//                      octaves will be added to the transposition.
//    2: ([A-Ga-g]) ==  The pname letter of the tonic key.  The letter
//                      is case insensitive, so "g" and "G" have the
//                      same meaning.
//    3: ([Ss#]*|[Ffb]*) == An optional accidental alteration of the
//                      pname letter, such as eF, ef, eb, EF, Ef, or Eb,
//                      all meaning e-flat, and aS, as, a#, AS, As, or
//                      A# all meaning a-sharp.
//

bool Transposer::IsValidKeyTonic(const std::string &name)
{
    std::string pattern = "([+]*|[-]*)([A-Ga-g])([Ss#]*|[Ffb]*)";
    if (std::regex_search(name, std::regex(pattern))) {
        return true;
    }
    else {
        return false;
    }
}

/*

/////////////////////////////////////////////////////
//
// Test program for Transposer class:
//

int main(void)
{
    TransPitch pitch(dpc_C, 0, 4); // middle C

    Transposer transpose;

    // transpose.SetBase40() is the default system.
    transpose.SetTransposition(transpose.PerfectFifthClass());
    std::cout << "Starting pitch:\t\t\t\t" << pitch << std::endl;
    transpose.Transpose(pitch);
    std::cout << "Transposed up a perfect fifth:\t\t" << pitch << std::endl;

    // testing use of a different base for transposition:
    transpose.SetBase600(); // allows up to 42 sharps or flats
    // Note that transpose value is cleared when setAccid() or SetBase*() is called.
    transpose.SetTransposition(-transpose.PerfectFifthClass());
    transpose.Transpose(pitch);
    std::cout << "Transposed back down a perfect fifth:\t" << pitch << std::endl;

    // testing use of interval string
    transpose.SetTransposition("-m3");
    transpose.Transpose(pitch);
    std::cout << "Transposed down a minor third:\t\t" << pitch << std::endl;

    // testing validation system for under/overflows:
    std::cout << std::endl;
    pitch.SetPitch(dpc_C, 2, 4); // C##4
    std::cout << "Initial pitch:\t\t" << pitch << std::endl;
    transpose.Transpose(pitch, "A4"); // now F###4
    bool valid = pitch.IsValid(2);
    std::cout << "Up an aug. 4th:\t\t" << pitch;
    if (!valid) {
        std::cout << "\t(not valid in base-40 system)";
    }
    std::cout << std::endl;

    // calculate interval between two pitches:
    std::cout << std::endl;
    std::cout << "TESTING INTERVAL NAMES IN BASE-40:" << std::endl;
    transpose.SetBase40();
    TransPitch p1(dpc_C, 0, 4);
    TransPitch p2(dpc_F, 2, 4);
    std::cout << "\tInterval between " << p1 << " and " << p2;
    std::cout << " is " << transpose.GetIntervalName(p1, p2) << std::endl;
    TransPitch p3(dpc_G, -2, 3);
    std::cout << "\tInterval between " << p1 << " and " << p3;
    std::cout << " is " << transpose.GetIntervalName(p1, p3) << std::endl;

    std::cout << "TESTING INTERVAL NAMES IN BASE-600:" << std::endl;
    transpose.SetBase600();
    std::cout << "\tInterval between " << p1 << " and " << p2;
    std::cout << " is " << transpose.GetIntervalName(p1, p2) << std::endl;
    std::cout << "\tInterval between " << p1 << " and " << p3;
    std::cout << " is " << transpose.GetIntervalName(p1, p3) << std::endl;
    std::cout << std::endl;

    std::cout << "TESTING INTERVAL NAME TO CIRCLE-OF-FIFTHS:" << std::endl;
    std::cout << "\tM6 should be 3:  " << transpose.IntervalToCircleOfFifths("M6") << std::endl;
    std::cout << "\tm6 should be -4: " << transpose.IntervalToCircleOfFifths("m6") << std::endl;

    std::cout << "TESTING CIRCLE-OF-FIFTHS TO INTERVAL NAME:" << std::endl;
    std::cout << "\t3 should be M6:  " << transpose.CircleOfFifthsToIntervalName(3) << std::endl;
    std::cout << "\t-4 should be m6: " << transpose.CircleOfFifthsToIntervalName(-4) << std::endl;
    std::cout << std::endl;

    std::cout << "TESTING INTERVAL NAME TO DIATONIC/CHROMATIC:" << std::endl;
    std::cout << "\tD-1,C-2 should be -M2:  " << transpose.DiatonicChromaticToIntervalName(-1, -2) << std::endl;
    std::cout << "\tD3,C6 should be A4:     " << transpose.DiatonicChromaticToIntervalName(3, 6) << std::endl;

    int chromatic;
    int diatonic;

    std::cout << "TESTING DIATONIC/CHROMATIC TO INTERVAL NAME:" << std::endl;
    std::cout << "\t-M2 should be D-1,C-2:  ";
    transpose.IntervalToDiatonicChromatic(diatonic, chromatic, "-M2");
    std::cout << "D" << diatonic << ",C" << chromatic << std::endl;
    std::cout << "\tA4 should be D3,C6:     ";
    transpose.IntervalToDiatonicChromatic(diatonic, chromatic, "A4");
    std::cout << "D" << diatonic << ",C" << chromatic << std::endl;

    return 0;
}

*/

/* Example output from test program:

    Starting pitch:                        C4
    Transposed up a perfect fifth:         G4
    Transposed back down a perfect fifth:  C4
    Transposed down a minor third:         A3

    Initial pitch:   C##4
    Up an aug. 4th:  F###4 (not valid in base-40 system)

    TESTING INTERVAL NAMES IN BASE-40:
        Interval between C4 and F##4 is AA4
        Interval between C4 and Gbb3 is -AA4
    TESTING INTERVAL NAMES IN BASE-600:
        Interval between C4 and F##4 is AA4
        Interval between C4 and Gbb3 is -AA4

    TESTING INTERVAL NAME TO CIRCLE-OF-FIFTHS:
        M6 should be 3:  3
        m6 should be -4: -4
    TESTING CIRCLE-OF-FIFTHS TO INTERVAL NAME:
        3 should be M6:  M6
        -4 should be m6: m6

    TESTING INTERVAL NAME TO DIATONIC/CHROMATIC:
        D-1,C-2 should be -M2:  -M2
        D3,C6 should be A4:     A4
    TESTING DIATONIC/CHROMATIC TO INTERVAL NAME:
        -M2 should be D-1,C-2:  D-1,C-2
        A4 should be D3,C6:     D3,C6

 */

} // namespace vrv

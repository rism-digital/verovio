/////////////////////////////////////////////////////////////////////////////
// Name:        transpose.cpp
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
//                   TPitch: pitch representation as three integers:
//                            pname: diatonic pitch class integer from C=0 to B=6.
//                            accid: chromatic alterations in semitones (0=natural, -1=flat).
//                            oct: octave number (4 = middle-C octave).
//                   Transpose: transposition system which uses TPitch as a user interface.
//                   (Add MEI to TPitch conversions in TPitch class, or use external
//                    code to interface to verovio attributes for <note>).
//                   The default maximum accidental handling is +/- two sharps/flats (base-40).
//                   Use the Transpose::setMaxAccid() to set the maximum allowed accidental
//                   count.  Transpose::setBase40() is equivalent to Transpose::setMaxAccid(2),
//                   and Transpose::setBase600() is equivalent to Transpose::setMaxAccid(42).
//
// Todo: Probably useful to add an autowrap feature to force unrepresentable pitches to be
//     moved to enharmonic equivalent pitches (better than leaving a pitch undefined).
//     For example, F#### in a system that cannot represent more than two or three
//     sharps would be converted to G##, probably with a warning message.  From F####
//     to G## is up a diminished second ("d2").
//

#define INVALID_INTERVAL_CLASS -123456789

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

#include "transpose.h"

#include <cctype>
#include <iostream>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////
//
// The TPitch class is an interface for storing information about notes which
// will be used in the Transpose class.  The diatonic pitch class, chromatic alteration
// of the diatonic pitch and the octave are store in the class.  Names given to the
// parameters are analogous to MEI note attributes.  Note that note@accid can be also
// note/accid in MEI data, and other complications that need to be resolved into
// storing the correct pitch information in TPitch.
//
namespace vrv {

//////////////////////////////
//
// TPitch::Tpitch -- TPitch constructor.
//

TPitch::TPitch(int aPname, int anAccid, int anOct)
{
    setPitch(aPname, anAccid, anOct);
}

TPitch::TPitch(const TPitch &pitch)
{
    pname = pitch.pname;
    accid = pitch.accid;
    oct = pitch.oct;
}

//////////////////////////////
//
// operator= TPitch -- copy operator for pitches.
//

TPitch &TPitch::operator=(const TPitch &pitch)
{
    if (this != &pitch) {
        pname = pitch.pname;
        accid = pitch.accid;
        oct = pitch.oct;
    }
    return *this;
}

//////////////////////////////
//
// TPitch::isValid -- returns true if the absolute value of the accidental
//     is less than or equal to the max value.

bool TPitch::isValid(int maxAccid)
{
    return abs(accid) <= abs(maxAccid);
}

//////////////////////////////
//
// TPitch::setPitch -- Set the attributes for a pitch all at once.
//

void TPitch::setPitch(int aPname, int anAccid, int anOct)
{
    pname = aPname;
    accid = anAccid;
    oct = anOct;
}

//////////////////////////////
//
// operator<< TPitch -- Print pitch data as string for debugging.
//

std::ostream &operator<<(std::ostream &out, const TPitch &pitch)
{
    switch (pitch.pname) {
        case dpc_C: out << "C"; break;
        case dpc_D: out << "D"; break;
        case dpc_E: out << "E"; break;
        case dpc_F: out << "F"; break;
        case dpc_G: out << "G"; break;
        case dpc_A: out << "A"; break;
        case dpc_B: out << "B"; break;
        default: out << "X";
    }
    if (pitch.accid > 0) {
        for (int i = 0; i < pitch.accid; i++) {
            out << "#";
        }
    }
    else if (pitch.accid < 0) {
        for (int i = 0; i < abs(pitch.accid); i++) {
            out << "b";
        }
    }
    out << pitch.oct;
    return out;
}

///////////////////////////////////////////////////////////////////////////

//////////////////////////////
//
// Transpose::Transpose -- Transpose constructor.
//

Transpose::Transpose()
{
    // Initialize with base-40 system by default:
    setMaxAccid(2);
}

//////////////////////////////
//
// Transpose::~Transpose -- Transpose deconstructor.
//

Transpose::~Transpose()
{
    // do nothing;
}

//////////////////////////////
//
// Transpose::setTransposition -- Set the transposition value which is an
//   interval class in the current base system.  When Transpose::setMaxAccid()
//   or Transpose.setBase*() are called, the transposition value will be set
//   to 0 (a perfect unison).
//

void Transpose::setTransposition(int transVal)
{
    m_transpose = transVal;
}

// Use a string to set the interval class in the current base system.  For example,
//  "+M2" means up a major second, which is the integer 6 in base-40.

void Transpose::setTransposition(const std::string &transString)
{
    m_transpose = getIntervalClass(transString);
}

//////////////////////////////
//
// Transpose::transpose -- Do a transposition at the stored transposition interval, or
//   with a temporary provided integer interval class, or a temporary interval name.
//

void Transpose::transpose(TPitch &pitch)
{
    int ipitch = pitchToInteger(pitch);
    ipitch += m_transpose;
    pitch = integerToPitch(ipitch);
}

// Use a temporary transposition value in the following
// two functions. To save for later use of Transpose::transpose
// without specifying the transposition interval, store
// transposition value with Transpose::setTransposition() first.

void Transpose::transpose(TPitch &pitch, int transVal)
{
    int ipitch = pitchToInteger(pitch);
    ipitch += transVal;
    pitch = integerToPitch(ipitch);
}

void Transpose::transpose(TPitch &pitch, const std::string &transString)
{
    int transVal = getIntervalClass(transString);
    int ipitch = pitchToInteger(pitch);
    ipitch += transVal;
    pitch = integerToPitch(ipitch);
}

//////////////////////////////
//
// Transpose::getBase -- Return the integer interval class representing an octave.
//

int Transpose::getBase()
{
    return m_base;
}

//////////////////////////////
//
// Transpose::getMaxAccid -- Return the maximum possible absolute accidental value
//     that can be represented by the current transposition base.
//

int Transpose::getMaxAccid()
{
    return m_maxAccid;
}

//////////////////////////////
//
// Transpose::setMaxAccid -- Calculate variables related to a specific base system.
//

void Transpose::setMaxAccid(int maxAccid)
{
    m_maxAccid = abs(maxAccid);
    m_base = 7 * (2 * m_maxAccid + 1) + 5;
    calculateDiatonicMapping();
    m_transpose = 0;
}

//////////////////////////////
//
// Transpose::calculateDiatonicMaping -- Calculate the integer values for the
//    natural diatonic pitch classes: C, D, E, F, G, A, and B in the current
//    base system.
//

void Transpose::calculateDiatonicMapping()
{
    int M2 = majorSecondClass();
    int m2 = M2 - 1;
    m_diatonicMapping.resize(7);
    m_diatonicMapping[dpc_C] = m_maxAccid + 1;
    m_diatonicMapping[dpc_D] = m_diatonicMapping[dpc_C] + M2;
    m_diatonicMapping[dpc_E] = m_diatonicMapping[dpc_D] + M2;
    m_diatonicMapping[dpc_F] = m_diatonicMapping[dpc_E] + m2;
    m_diatonicMapping[dpc_G] = m_diatonicMapping[dpc_F] + M2;
    m_diatonicMapping[dpc_A] = m_diatonicMapping[dpc_G] + M2;
    m_diatonicMapping[dpc_B] = m_diatonicMapping[dpc_A] + M2;
}

//////////////////////////////
//
// Transpose::getIntervalClass -- Convert a diatonic interval with chromatic
//     quality and direction into an integer interval class.   Input string
//     is in the format: direction + quality + diatonic interval.
//     Such as +M2 for up a major second, -P5 is down a perfect fifth.
//     Regular expression that the string should conform to:
//            (-|\+?)([Pp]|M|m|[aA]+|[dD]+)(\d+)
//

int Transpose::getIntervalClass(const std::string &intervalName)
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
        std::cerr << "Interval requires a chromatic quality: " << intervalName << std::endl;
        return INVALID_INTERVAL_CLASS;
    }

    if (number.empty()) {
        std::cerr << "Interval requires a diatonic interval number: " << intervalName << std::endl;
        return INVALID_INTERVAL_CLASS;
    }

    int dnum = stoi(number);
    if (dnum == 0) {
        std::cerr << "Integer interval number cannot be zero: " << intervalName << std::endl;
        return INVALID_INTERVAL_CLASS;
    }
    dnum--;
    int octave = dnum / 7;
    dnum = dnum - octave * 7;

    int base = 0;
    int adjust = 0;

    switch (dnum) {
        case 0: // unison
            base = perfectUnisonClass();
            if (quality[0] == 'A') {
                adjust = (int)quality.size();
            }
            else if (quality[0] == 'd') {
                adjust = -(int)quality.size();
            }
            else if (quality != "P") {
                std::cerr << "Error in interval quality: " << intervalName << std::endl;
                return INVALID_INTERVAL_CLASS;
            }
            break;
        case 1: // second
            if (quality == "M") {
                base = majorSecondClass();
            }
            else if (quality == "m") {
                base = minorSecondClass();
            }
            else if (quality[0] == 'A') {
                base = majorSecondClass();
                adjust = (int)quality.size();
            }
            else if (quality[0] == 'd') {
                base = minorSecondClass();
                adjust = -(int)quality.size();
            }
            else {
                std::cerr << "Error in interval quality: " << intervalName << std::endl;
                return INVALID_INTERVAL_CLASS;
            }
            break;
        case 2: // third
            if (quality == "M") {
                base = majorThirdClass();
            }
            else if (quality == "m") {
                base = minorThirdClass();
            }
            else if (quality[0] == 'A') {
                base = majorThirdClass();
                adjust = (int)quality.size();
            }
            else if (quality[0] == 'd') {
                base = minorThirdClass();
                adjust = -(int)quality.size();
            }
            else {
                std::cerr << "Error in interval quality: " << intervalName << std::endl;
                return INVALID_INTERVAL_CLASS;
            }
            break;
        case 3: // fourth
            base = perfectFourthClass();
            if (quality[0] == 'A') {
                adjust = (int)quality.size();
            }
            else if (quality[0] == 'd') {
                adjust = -(int)quality.size();
            }
            else if (quality != "P") {
                std::cerr << "Error in interval quality: " << intervalName << std::endl;
                return INVALID_INTERVAL_CLASS;
            }
            break;
        case 4: // fifth
            base = perfectFifthClass();
            if (quality[0] == 'A') {
                adjust = (int)quality.size();
            }
            else if (quality[0] == 'd') {
                adjust = -(int)quality.size();
            }
            else if (quality != "P") {
                std::cerr << "Error in interval quality: " << intervalName << std::endl;
                return INVALID_INTERVAL_CLASS;
            }
            break;
        case 5: // sixth
            if (quality == "M") {
                base = majorSixthClass();
            }
            else if (quality == "m") {
                base = minorSixthClass();
            }
            else if (quality[0] == 'A') {
                base = majorSixthClass();
                adjust = (int)quality.size();
            }
            else if (quality[0] == 'd') {
                base = minorSixthClass();
                adjust = -(int)quality.size();
            }
            else {
                std::cerr << "Error in interval quality: " << intervalName << std::endl;
                return INVALID_INTERVAL_CLASS;
            }
            break;
        case 6: // seventh
            if (quality == "M") {
                base = majorSeventhClass();
            }
            else if (quality == "m") {
                base = minorSeventhClass();
            }
            else if (quality[0] == 'A') {
                base = majorSeventhClass();
                adjust = (int)quality.size();
            }
            else if (quality[0] == 'd') {
                base = minorSeventhClass();
                adjust = -(int)quality.size();
            }
            else {
                std::cerr << "Error in interval quality: " << intervalName << std::endl;
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
// Transpose::perfectUnisonClass -- Return the integer interval class
//     representing a perfect unison.
//

int Transpose::perfectUnisonClass()
{
    return 0;
}

//////////////////////////////
//
// Transpose::minorSecondClass -- Return the integer interval class
//     representing a minor second.
//

int Transpose::minorSecondClass()
{
    return m_maxAccid * 2 + 1;
}

//////////////////////////////
//
// Transpose::majorSecondClass -- Return the integer interval class
//    representing a major second.
//

int Transpose::majorSecondClass()
{
    return minorSecondClass() + 1;
}

//////////////////////////////
//
// Transpose::minorThirdClass -- Return the integer interval class
//    representing a minor third.
//

int Transpose::minorThirdClass()
{
    return majorThirdClass() - 1;
}

//////////////////////////////
//
// Transpose::majorThirdClass -- Return the integer interval class
//    representing a major third.
//

int Transpose::majorThirdClass()
{
    return 2 * majorSecondClass();
}

//////////////////////////////
//
// Transpose::perfectFourthClass -- Return the integer interval class
//    representing a perfect fourth.
//

int Transpose::perfectFourthClass()
{
    return perfectOctaveClass() - perfectFifthClass();
}

//////////////////////////////
//
// Transpose::perfectFifthClass -- Return the integer interval class
//    representing a perfect fifth.
//

int Transpose::perfectFifthClass()
{
    return 3 * majorSecondClass() + minorSecondClass();
}

//////////////////////////////
//
// Transpose::minorSixthClass -- Return the integer interval class
//    representing a minor sixth.
//

int Transpose::minorSixthClass()
{
    return perfectOctaveClass() - majorThirdClass();
}

//////////////////////////////
//
// Transpose::majorSixthClass -- Return the integer interval class
//    representing a major sixth.
//

int Transpose::majorSixthClass()
{
    return perfectOctaveClass() - minorThirdClass();
}

//////////////////////////////
//
// Transpose::minorSeventhClass -- Return the integer interval class
//    representing a minor sixth.
//

int Transpose::minorSeventhClass()
{
    return perfectOctaveClass() - majorSecondClass();
}

//////////////////////////////
//
// Transpose::majorSeventhClass -- Return the integer interval class
//    representing a major sixth.
//

int Transpose::majorSeventhClass()
{
    return perfectOctaveClass() - minorSecondClass();
}

//////////////////////////////
//
// Transpose::octaveClass -- Return the integer interval class
//    representing a major second.
//

int Transpose::perfectOctaveClass()
{
    return m_base;
}

//////////////////////////////
//
// Transpose::pitchToInteger -- Convert a pitch (octave/diatonic pitch class/chromatic
//     alteration) into an integer value according to the current base.
//

int Transpose::pitchToInteger(const TPitch &pitch)
{
    return pitch.oct * m_base + m_diatonicMapping[pitch.pname] + pitch.accid;
}

//////////////////////////////
//
// Transpose::integerToPitch -- Convert an integer within the current base
//    into a pitch (octave/diatonic pitch class/chromatic alteration).  Pitches
//    with negative octaves will have to be tested.
//

TPitch Transpose::integerToPitch(int ipitch)
{
    TPitch pitch;
    pitch.oct = ipitch / m_base;
    int chroma = ipitch - pitch.oct * m_base;
    int mindiff = -1000;
    int mini = -1;

    int targetdiff = m_maxAccid;

    if (chroma > m_base / 2) {
        // search from B downwards
        mindiff = chroma - m_diatonicMapping.back();
        mini = (int)m_diatonicMapping.size() - 1;
        for (int i = m_diatonicMapping.size() - 2; i >= 0; i--) {
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
    pitch.pname = mini;
    pitch.accid = mindiff;
    return pitch;
}

//////////////////////////////
//
// Transpose::setBase40 -- Allow up to double sharp/flats.
//

void Transpose::setBase40()
{
    setMaxAccid(2);
}

//////////////////////////////
//
// Transpose::setBase600 -- Allow up to 42 sharp/flats.
//

void Transpose::setBase600()
{
    setMaxAccid(42);
}

//////////////////////////////
//
// Transpose::getIntervalClass -- Return the interval between two pitches.
//    If the second pitch is higher than the first, then the interval will be
//    positive; otherwise, the interval will be negative.
//

int Transpose::getIntervalClass(const TPitch &p1, const TPitch &p2)
{
    return pitchToInteger(p2) - pitchToInteger(p1);
}

// similar function, but the integer interval class is converted into a string
// that is not dependent on a base.

std::string Transpose::getIntervalName(const TPitch &p1, const TPitch &p2)
{
    int iclass = getIntervalClass(p1, p2);
    return getIntervalName(iclass);
}

std::string Transpose::getIntervalName(int intervalClass)
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

    int number = -123456789;
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
// Transpose::intervalToCircleOfFifths -- Returns the circle-of-fiths count
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

int Transpose::intervalToCircleOfFifths(const std::string &transstring)
{
    int intervalClass = getIntervalClass(transstring);
    return intervalToCircleOfFifths(intervalClass);
}

int Transpose::intervalToCircleOfFifths(int transval)
{
    if (transval < 0) {
        transval = (m_base * 100 - transval) % m_base;
    }
    int p5 = perfectFifthClass();
    int p4 = perfectFourthClass();
    if (transval == 0) {
        return 0;
    }
    for (int i = 1; i < m_base / 2; i++) {
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
// Transpose::circleOfFifthsToIntervalClass -- Inputs a circle-of-fifths value and
//   returns the interval class as an integer in the current base.
//

int Transpose::circleOfFifthsToIntervalClass(int fifths)
{
    if (fifths == 0) {
        return 0;
    }
    else if (fifths > 0) {
        return (perfectFifthClass() * fifths) % m_base;
    }
    else {
        return (perfectFourthClass() * (-fifths)) % m_base;
    }
}

//////////////////////////////
//
// Transpose::circleOfFifthsToIntervalName -- Convert a circle-of-fifths position
//    into an interval string.
//

std::string Transpose::circleOfFifthsToIntervalName(int fifths)
{
    int intervalClass = circleOfFifthsToIntervalClass(fifths);
    return getIntervalName(intervalClass);
}

//////////////////////////////
//
// Transpose::diatonicChromaticToIntervalClass -- Convert a diatonic/chromatic interval
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

std::string Transpose::diatonicChromaticToIntervalName(int diatonic, int chromatic)
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
// Transpose::diatonicChromaticToIntervalClass --
//

int Transpose::diatonicChromaticToIntervalClass(int diatonic, int chromatic)
{
    std::string intervalName = diatonicChromaticToIntervalName(diatonic, chromatic);
    return getIntervalClass(intervalName);
}

//////////////////////////////
//
// Transpose::intervalToDiatonicChromatic --
//

void Transpose::intervalToDiatonicChromatic(int &diatonic, int &chromatic, int intervalClass)
{
    std::string intervalName = getIntervalName(intervalClass);
    intervalToDiatonicChromatic(diatonic, chromatic, intervalName);
}

void Transpose::intervalToDiatonicChromatic(int &diatonic, int &chromatic, const std::string &intervalName)
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
        std::cerr << "Interval requires a chromatic quality: " << intervalName << std::endl;
        chromatic = INVALID_INTERVAL_CLASS;
        diatonic = INVALID_INTERVAL_CLASS;
        return;
    }

    if (number.empty()) {
        std::cerr << "Interval requires a diatonic interval number: " << intervalName << std::endl;
        chromatic = INVALID_INTERVAL_CLASS;
        diatonic = INVALID_INTERVAL_CLASS;
        return;
    }

    int dnum = stoi(number);
    if (dnum == 0) {
        std::cerr << "Integer interval number cannot be zero: " << intervalName << std::endl;
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
                std::cerr << "Error in interval quality: " << intervalName << std::endl;
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
                std::cerr << "Error in interval quality: " << intervalName << std::endl;
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
                std::cerr << "Error in interval quality: " << intervalName << std::endl;
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
                std::cerr << "Error in interval quality: " << intervalName << std::endl;
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
                std::cerr << "Error in interval quality: " << intervalName << std::endl;
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
                std::cerr << "Error in interval quality: " << intervalName << std::endl;
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
                std::cerr << "Error in interval quality: " << intervalName << std::endl;
                chromatic = INVALID_INTERVAL_CLASS;
                diatonic = INVALID_INTERVAL_CLASS;
                return;
            }
            break;
    }
    chromatic *= direction;
}

/*

/////////////////////////////////////////////////////
//
// Test program for Transpose class:
//

int main(void)
{
    TPitch pitch(dpc_C, 0, 4); // middle C

    Transpose transpose;

    // transpose.setBase40() is the default system.
    transpose.setTransposition(transpose.perfectFifthClass());
    std::cout << "Starting pitch:\t\t\t\t" << pitch << std::endl;
    transpose.transpose(pitch);
    std::cout << "Transposed up a perfect fifth:\t\t" << pitch << std::endl;

    // testing use of a different base for transposition:
    transpose.setBase600(); // allows up to 42 sharps or flats
    // Note that transpose value is cleared when setAccid() or setBase*() is called.
    transpose.setTransposition(-transpose.perfectFifthClass());
    transpose.transpose(pitch);
    std::cout << "Transposed back down a perfect fifth:\t" << pitch << std::endl;

    // testing use of interval string
    transpose.setTransposition("-m3");
    transpose.transpose(pitch);
    std::cout << "Transposed down a minor third:\t\t" << pitch << std::endl;

    // testing validation system for under/overflows:
    std::cout << std::endl;
    pitch.setPitch(dpc_C, 2, 4); // C##4
    std::cout << "Initial pitch:\t\t" << pitch << std::endl;
    transpose.transpose(pitch, "A4"); // now F###4
    bool valid = pitch.isValid(2);
    std::cout << "Up an aug. 4th:\t\t" << pitch;
    if (!valid) {
        std::cout << "\t(not valid in base-40 system)";
    }
    std::cout << std::endl;

    // calculate interval between two pitches:
    std::cout << std::endl;
    std::cout << "TESTING INTERVAL NAMES IN BASE-40:" << std::endl;
    transpose.setBase40();
    TPitch p1(dpc_C, 0, 4);
    TPitch p2(dpc_F, 2, 4);
    std::cout << "\tInterval between " << p1 << " and " << p2;
    std::cout << " is " << transpose.getIntervalName(p1, p2) << std::endl;
    TPitch p3(dpc_G, -2, 3);
    std::cout << "\tInterval between " << p1 << " and " << p3;
    std::cout << " is " << transpose.getIntervalName(p1, p3) << std::endl;

    std::cout << "TESTING INTERVAL NAMES IN BASE-600:" << std::endl;
    transpose.setBase600();
    std::cout << "\tInterval between " << p1 << " and " << p2;
    std::cout << " is " << transpose.getIntervalName(p1, p2) << std::endl;
    std::cout << "\tInterval between " << p1 << " and " << p3;
    std::cout << " is " << transpose.getIntervalName(p1, p3) << std::endl;
    std::cout << std::endl;

    std::cout << "TESTING INTERVAL NAME TO CIRCLE-OF-FIFTHS:" << std::endl;
    std::cout << "\tM6 should be 3:  " << transpose.intervalToCircleOfFifths("M6") << std::endl;
    std::cout << "\tm6 should be -4: " << transpose.intervalToCircleOfFifths("m6") << std::endl;

    std::cout << "TESTING CIRCLE-OF-FIFTHS TO INTERVAL NAME:" << std::endl;
    std::cout << "\t3 should be M6:  " << transpose.circleOfFifthsToIntervalName(3) << std::endl;
    std::cout << "\t-4 should be m6: " << transpose.circleOfFifthsToIntervalName(-4) << std::endl;
    std::cout << std::endl;

    std::cout << "TESTING INTERVAL NAME TO DIATONIC/CHROMATIC:" << std::endl;
    std::cout << "\tD-1,C-2 should be -M2:  " << transpose.diatonicChromaticToIntervalName(-1, -2) << std::endl;
    std::cout << "\tD3,C6 should be A4:     " << transpose.diatonicChromaticToIntervalName(3, 6) << std::endl;

    int chromatic;
    int diatonic;

    std::cout << "TESTING DIATONIC/CHROMATIC TO INTERVAL NAME:" << std::endl;
    std::cout << "\t-M2 should be D-1,C-2:  ";
    transpose.intervalToDiatonicChromatic(diatonic, chromatic, "-M2");
    std::cout << "D" << diatonic << ",C" << chromatic << std::endl;
    std::cout << "\tA4 should be D3,C6:     ";
    transpose.intervalToDiatonicChromatic(diatonic, chromatic, "A4");
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

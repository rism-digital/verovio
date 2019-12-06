//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Dec  3 11:42:25 PST 2019
// Last Modified: Wed Dec  4 09:19:46 PST 2019
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

////////////////////////////////////////////////////////////////////////////
//
// The Transpose class is an interface for transposing notes represented in the
// TPitch class format.
//

class Transpose {
public:
    Transpose();
    ~Transpose();

    void setBase40();
    void setBase600();
    int getBase();
    int getMaxAccid();
    void setMaxAccid(int maxAccid);
    int getIntervalClass(const std::string &intervalName);
    int pitchToInteger(const TPitch &pitch);
    TPitch integerToPitch(int ipitch);
    void setTransposition(int transVal);
    void setTransposition(const std::string &transString);
    void transpose(TPitch &pitch);
    void transpose(TPitch &pitch, int transVal);
    void transpose(TPitch &pitch, const std::string &transString);
    int getIntervalClass(const TPitch &p1, const TPitch &p2);
    std::string getIntervalName(const TPitch &p1, const TPitch &p2);
    std::string getIntervalName(int interval);

    // Convenience functions for calculating common interval classes.
    // augmented classes can be calculated by adding 1 to
    // perfect/major classes, and diminished classes can be
    // calcualted by subtracting 1 from perfect/minor classes.
    int perfectUnisonClass();
    int minorSecondClass();
    int majorSecondClass();
    int minorThirdClass();
    int majorThirdClass();
    int perfectFourthClass();
    int perfectFifthClass();
    int minorSixthClass();
    int majorSixthClass();
    int minorSeventhClass();
    int majorSeventhClass();
    int perfectOctaveClass();

protected:
    int m_base; // integer representation for perfect octave
    int m_maxAccid; // maximum allowable sharp/flats for transposing
    int m_transpose; // integer interval class for transposing
    std::vector<int> m_diatonicMapping; // pitch integers for each natural diatonic pitch class

private:
    void calculateDiatonicMapping();
};

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
            if (abs(mindiff) <= m_maxAccid) {
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
            if (abs(mindiff) <= m_maxAccid) {
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

std::string Transpose::getIntervalName(int interval)
{
    std::string direction;
    if (interval < 0) {
        direction = "-";
        interval = -interval;
    }

    int octave = interval / m_base;
    int chroma = interval - octave * m_base;

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

/////////////////////////////////////////////////////

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

    return 0;
}

/* Example output from test program:

   Starting pitch:                       C4
   Transposed up a perfect fifth:        G4
   Transposed back down a perfect fifth: C4
   Transposed down a minor third:        A3

   Initial pitch:   C##4
   Up an aug. 4th:  F###4  (not valid in base-40 system)

   TESTING INTERVAL NAMES IN BASE-40:
      Interval between C4 and F##4 is AA4
      Interval between C4 and Gbb3 is -AA4
   TESTING INTERVAL NAMES IN BASE-600:
      Interval between C4 and F##4 is AA4
      Interval between C4 and Gbb3 is -AA4
 */

} // namespace vrv

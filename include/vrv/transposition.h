/////////////////////////////////////////////////////////////////////////////
// Name:        transpose.h
// Author:      Craig Stuart Sapp
// Created:     03/12/2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TRANSPOSE_H__
#define __VRV_TRANSPOSE_H__

#include <iostream>
#include <string>
#include <vector>

namespace vrv {

class TPitch;
class Transpose;

////////////////////////////////////////////////////////////////////////////
//
// The TPitch class is an interface for storing information about notes that will
// be used in the Transpose class.  The diatonic pitch class, chromatic alteration
// of the diatonic pitch and the octave are store in the class.  Names given to the
// parameters are analogous to MEI note attributes.  Note that note@accid can be also
// note/accid in MEI data, and other complications that need to be resolved into
// storing the correct pitch information in TPitch.
//

class TPitch {
public:
    int pname; // diatonic pitch class name: C = 0, D = 1, ... B = 6.
    int accid; // chromatic alteration: 0 = natural, 1 = sharp, -2 = flat, +2 = double sharp
    int oct; // octave number: 4 = middle-C octave

    TPitch(){};
    TPitch(int aPname, int anAccid, int anOct);
    TPitch(const TPitch &pitch);
    TPitch &operator=(const TPitch &pitch);
    bool isValid(int maxAccid);
    void setPitch(int aPname, int anAccid, int anOct);
};

std::ostream &operator<<(std::ostream &out, const TPitch &pitch);

////////////////////////////////////////////////////////////////////////////
//
// The Transpose class is an interface for transposing notes represented in the
// TPitch class format.
//

class Transpose {
public:
    Transpose();
    ~Transpose();

    int getBase();
    int getMaxAccid();
    void setMaxAccid(int maxAccid);
    void setBase40();
    void setBase600();
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
    std::string getIntervalName(int intervalClass);
    int intervalToCircleOfFifths(const std::string &transString);
    int intervalToCircleOfFifths(int transval);
    std::string circleOfFifthsToIntervalName(int fifths);
    int circleOfFifthsToIntervalClass(int fifths);
    std::string diatonicChromaticToIntervalName(int diatonic, int chromatic);
    int diatonicChromaticToIntervalClass(int diatonic, int chromatic);
    void intervalToDiatonicChromatic(int &diatonic, int &chromatic, int intervalClass);
    void intervalToDiatonicChromatic(int &diatonic, int &chromatic, const std::string &intervalName);

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

} // namespace vrv

#endif

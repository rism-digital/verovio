/////////////////////////////////////////////////////////////////////////////
// Name:        transposition.h
// Author:      Craig Stuart Sapp
// Created:     03/12/2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TRANSPOSE_H__
#define __VRV_TRANSPOSE_H__

#include <iostream>
#include <string>
#include <vector>

#include "attdef.h"
#include "atttypes.h"

#define INVALID_INTERVAL_CLASS -123456789

namespace vrv {

class TransPitch;
class Transposer;

////////////////////////////////////////////////////////////////////////////
//
// The TransPitch class is an interface for storing information about notes that will
// be used in the Transposer class.  The diatonic pitch class, chromatic alteration
// of the diatonic pitch and the octave are store in the class.  Names given to the
// parameters are analogous to MEI note attributes.  Note that note@accid can be also
// note/accid in MEI data, and other complications that need to be resolved into
// storing the correct pitch information in TransPitch.
//

class TransPitch {
public:
    int m_pname; // diatonic pitch class name: C = 0, D = 1, ... B = 6.
    int m_accid; // chromatic alteration: 0 = natural, 1 = sharp, -2 = flat, +2 = double sharp
    int m_oct; // octave number: 4 = middle-C octave

    TransPitch(){};
    TransPitch(int aPname, int anAccid, int anOct);
    TransPitch(data_PITCHNAME pname, data_ACCIDENTAL_GESTURAL accidG, data_ACCIDENTAL_WRITTEN accidW, int oct);
    TransPitch(const TransPitch &pitch);
    TransPitch &operator=(const TransPitch &pitch);
    static int GetChromaticAlteration(data_ACCIDENTAL_GESTURAL accidG, data_ACCIDENTAL_WRITTEN accidW);
    data_ACCIDENTAL_GESTURAL GetAccidG() const;
    data_ACCIDENTAL_WRITTEN GetAccidW() const;
    data_PITCHNAME GetPitchName() const;
    std::wstring GetPitchString() const;
    bool IsValid(int maxAccid);
    void SetPitch(int aPname, int anAccid, int anOct);
};

std::ostream &operator<<(std::ostream &out, const TransPitch &pitch);

////////////////////////////////////////////////////////////////////////////
//
// The Transposer class is an interface for transposing notes represented in the
// TransPitch class format.
//

class Transposer {
public:
    Transposer();
    ~Transposer();

    int GetBase();
    int GetMaxAccid();
    void SetMaxAccid(int maxAccid);
    void SetBase40();
    void SetBase600();
    int GetIntervalClass(const std::string &intervalName);
    int Subtract(const TransPitch &minuend, const TransPitch &subtrahend);
    int PitchToInteger(const TransPitch &pitch);
    TransPitch IntegerToPitch(int ipitch);
    void SetTransposition(int transVal);
    bool SetTransposition(const std::string &transString);
    bool SetTransposition(const TransPitch &fromPitch, const std::string &toString);
    int GetTranspositionIntervalClass();
    std::string GetTranspositionIntervalName();
    void Transpose(TransPitch &pitch);
    int Transpose(int iPitch);
    void Transpose(TransPitch &pitch, int transVal);
    void Transpose(TransPitch &pitch, const std::string &transString);
    bool GetKeyTonic(const std::string &keyTonic, TransPitch &tonic);
    int GetIntervalClass(const TransPitch &p1, const TransPitch &p2);
    std::string GetIntervalName(const TransPitch &p1, const TransPitch &p2);
    std::string GetIntervalName(int intervalClass);
    int IntervalToCircleOfFifths(const std::string &transString);
    int IntervalToCircleOfFifths(int transval);
    std::string CircleOfFifthsToIntervalName(int fifths);
    int CircleOfFifthsToIntervalClass(int fifths);
    TransPitch CircleOfFifthsToPitch(int fifths);
    std::string DiatonicChromaticToIntervalName(int diatonic, int chromatic);
    int DiatonicChromaticToIntervalClass(int diatonic, int chromatic);
    void IntervalToDiatonicChromatic(int &diatonic, int &chromatic, int intervalClass);
    void IntervalToDiatonicChromatic(int &diatonic, int &chromatic, const std::string &intervalName);

    // Convenience functions for calculating common interval classes.
    // augmented classes can be calculated by adding 1 to
    // perfect/major classes, and diminished classes can be
    // calcualted by subtracting 1 from perfect/minor classes.
    int PerfectUnisonClass();
    int MinorSecondClass();
    int MajorSecondClass();
    int MinorThirdClass();
    int MajorThirdClass();
    int PerfectFourthClass();
    int PerfectFifthClass();
    int MinorSixthClass();
    int MajorSixthClass();
    int MinorSeventhClass();
    int MajorSeventhClass();
    int PerfectOctaveClass();

protected:
    int m_base; // integer representation for perfect octave
    int m_maxAccid; // maximum allowable sharp/flats for transposing
    int m_transpose; // integer interval class for transposing
    std::vector<int> m_diatonicMapping; // pitch integers for each natural diatonic pitch class

private:
    void CalculateDiatonicMapping();
};

} // namespace vrv

#endif

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
    // diatonic pitch class name of pitch: C = 0, D = 1, ... B = 6.
    int m_pname;

    // chromatic alteration of pitch: 0 = natural, 1 = sharp, -2 = flat, +2 = double sharp
    int m_accid;

    // octave number of pitch: 4 = middle-C octave
    int m_oct;

    TransPitch(){};
    TransPitch(int aPname, int anAccid, int anOct);
    TransPitch(data_PITCHNAME pname, data_ACCIDENTAL_GESTURAL accidG, data_ACCIDENTAL_WRITTEN accidW, int oct);
    TransPitch(const TransPitch &pitch);

    // TransPitch operators
    TransPitch &operator=(const TransPitch &pitch);
    // lesser/greater operators
    bool operator>(const TransPitch &pitch);
    bool operator<(const TransPitch &pitch);
    // increament/decrement operators
    TransPitch &operator++();
    TransPitch operator++(int);
    TransPitch &operator--();
    TransPitch operator--(int);

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

    // Set the interval class for an octave (default is 40, +/- two sharps/flats).
    void SetMaxAccid(int maxAccid);
    int GetMaxAccid();
    void SetBase40();
    void SetBase600();
    int GetBase();

    // Set the transposition amount for use with Transpose() functions.  These functions
    // need to be rerun after SetMaxAccid() or SetBase*() are called; otherwise, the
    // transposition will be 0/P1/unison.
    bool SetTransposition(int transVal);
    bool SetTransposition(const std::string &transString);
    bool SetTransposition(const TransPitch &fromPitch, const std::string &toString);
    bool SetTransposition(int keyFifths, int semitones);
    bool SetTransposition(int keyFifths, const std::string &semitones);

    // Accessor functions for retrieving stored transposition interval.
    int GetTranspositionIntervalClass();
    std::string GetTranspositionIntervalName();

    // Transpostion based on stored transposition interval.
    void Transpose(TransPitch &pitch);
    int Transpose(int iPitch);

    // Transpose based on second input parameter (not with stored transposition interval).
    void Transpose(TransPitch &pitch, int transVal);
    void Transpose(TransPitch &pitch, const std::string &transString);

    // Convert between integer intervals and interval name strings:
    std::string GetIntervalName(const TransPitch &p1, const TransPitch &p2);
    std::string GetIntervalName(int intervalClass);
    int GetInterval(const std::string &intervalName);

    // Convert between TransPitch class and integer pitch and interval representations.
    int TransPitchToIntegerPitch(const TransPitch &pitch);
    TransPitch IntegerPitchToTransPitch(int ipitch);
    int GetInterval(const TransPitch &p1, const TransPitch &p2);

    // Convert between Semitones and integer interval representation.
    std::string SemitonesToIntervalName(int keyFifths, int semitones);
    int SemitonesToIntervalClass(int keyFifths, int semitones);
    int IntervalToSemitones(int intervalClass);
    int IntervalToSemitones(const std::string &intervalName);

    // Circle-of-fifths related functions.
    int IntervalToCircleOfFifths(const std::string &transString);
    int IntervalToCircleOfFifths(int transval);
    std::string CircleOfFifthsToIntervalName(int fifths);
    int CircleOfFifthsToIntervalClass(int fifths);

    // Key-signature related functions.
    bool GetKeyTonic(const std::string &keyTonic, TransPitch &tonic);
    TransPitch CircleOfFifthsToMajorTonic(int fifths);
    TransPitch CircleOfFifthsToMinorTonic(int fifths);
    TransPitch CircleOfFifthsToDorianTonic(int fifths);
    TransPitch CircleOfFifthsToPhrygianTonic(int fifths);
    TransPitch CircleOfFifthsToLydianTonic(int fifths);
    TransPitch CircleOfFifthsToMixolydianTonic(int fifths);
    TransPitch CircleOfFifthsToLocrianTonic(int fifths);

    // Conversions between diatonic/chromatic system and integer system of intervals.
    std::string DiatonicChromaticToIntervalName(int diatonic, int chromatic);
    int DiatonicChromaticToIntervalClass(int diatonic, int chromatic);
    void IntervalToDiatonicChromatic(int &diatonic, int &chromatic, int intervalClass);
    void IntervalToDiatonicChromatic(int &diatonic, int &chromatic, const std::string &intervalName);

    // Convenience functions for calculating common interval classes.  Augmented classes
    // can be calculated by adding 1 to perfect/major classes, and diminished classes can be
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

    // Convenience functions for acessing m_diatonicMapping.
    int GetCPitchClass() { return m_diatonicMapping[0]; }
    int GetDPitchClass() { return m_diatonicMapping[1]; }
    int GetEPitchClass() { return m_diatonicMapping[2]; }
    int GetFPitchClass() { return m_diatonicMapping[3]; }
    int GetGPitchClass() { return m_diatonicMapping[4]; }
    int GetAPitchClass() { return m_diatonicMapping[5]; }
    int GetBPitchClass() { return m_diatonicMapping[6]; }

    // Input string validity helper functions.
    static bool IsValidIntervalName(const std::string &name);
    static bool IsValidKeyTonic(const std::string &name);
    static bool IsValidSemitones(const std::string &name);

protected:
    // integer representation for perfect octave:
    int m_base;

    // maximum allowable sharp/flats for transposing:
    int m_maxAccid;

    // integer interval class for transposing:
    int m_transpose;

    // pitch integers for each natural diatonic pitch class:
    std::vector<int> m_diatonicMapping;

    // used to calculate semitones between diatonic pitch classes:
    const std::vector<int> m_diatonic2semitone{ 0, 2, 4, 5, 7, 9, 11 };

private:
    void CalculateDiatonicMapping();
};

} // namespace vrv

#endif

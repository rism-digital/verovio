/////////////////////////////////////////////////////////////////////////////
// Name:        iogabc.h
// Author:      David Rizo
// Created:     2025
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_IOGABC_H__
#define __VRV_IOGABC_H__

#include <optional>
#include <string>
#include <utility>

//----------------------------------------------------------------------------

#include "attdef.h"
#include "iobase.h"
#include "vrvdef.h"

namespace vrv {
class Layer;
class Nc;
class Syllable;

//----------------------------------------------------------------------------
// GABCInput
//----------------------------------------------------------------------------

enum Prefixes { NO_SPACE, REMOVE_FIRST_STEM, NEUMATIC_CUT, OBLIQUE_LIGATURE };

class GABCInput : public Input {
public:
    GABCInput(Doc *doc);
    virtual ~GABCInput();
    bool Import(const std::string &gabc) override;

private:
    using PitchOctaveType = std::pair<data_PITCHNAME, int>;

    // parsing methods
    void AddAccidental(Syllable *syllable, data_ACCIDENTAL_WRITTEN accid, data_PITCHNAME pname, data_OCTAVE oct);
    void AddEpisema(Nc *nc, const std::string &form);
    void AddLiquescent(Nc *nc, curvatureDirection_CURVE curve);
    std::optional<Prefixes> FindPrefix(const std::string &music, int &currentIndex);
    int ProcessBarline(const std::string &music, int currentIndex, Layer *layer);
    bool ProcessClef(const std::string &word);
    int ProcessCustos(const std::string &word);
    void ProcessInput(const std::string &gabc);
    void ProcessNeume(const std::string &music, Syllable *syllable);
    int ProcessSuffix(const std::string &music, int currentIndex, Nc *nc, Nc *previousNC);
    void ProcessWord(const std::string &lyrics, const std::string &music, sylLog_WORDPOS wordpos, sylLog_CON con);

    //----------------//
    // Static methods //
    //----------------//

    static char GetCharAt(const std::string &input, int index);
    static int PitchToDiatonicNumber(data_PITCHNAME pname, int oct);
    static curvatureDirection_CURVE InferLiquescentCurve(const Nc *previousNC, const Nc *currentNC);
    static PitchOctaveType MakePitchFromDiatonicIndex(int absoluteDiatonicIndex);
    static std::optional<PitchOctaveType> FindPitch(char ch, int clefPitchOffset);

public:
    //
private:
    int m_currentClefPitchOffset;
    Layer *m_layer;
};

} // namespace vrv

#endif

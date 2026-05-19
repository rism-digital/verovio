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

enum GABCPrefixes { GABC_NO_SPACE, GABC_REMOVE_FIRST_STEM, GABC_NEUMATIC_CUT, GABC_OBLIQUE_LIGATURE };

class GABCInput : public Input {
public:
    GABCInput(Doc *doc);
    virtual ~GABCInput();
    bool Import(const std::string &gabc) override;

private:
    using PitchOctaveType = std::pair<data_PITCHNAME, int>;

    // parsing methods
    std::optional<GABCPrefixes> FindPrefix(const std::string &music, int &currentIndex);
    bool ProcessClef(const std::string &word);
    int ProcessCustos(const std::string &word);
    void ProcessInput(const std::string &gabc);
    void ProcessNeume(const std::string &music, Syllable *syllable);
    int ProcessSuffix(const std::string &music, int currentIndex, Nc *nc, Nc *previousNC);
    void ProcessWord(const std::string &lyrics, const std::string &music, sylLog_WORDPOS wordpos, sylLog_CON con);

    //----------------//
    // Static methods //
    //----------------//

    static void AddAccidental(Syllable *syllable, data_ACCIDENTAL_WRITTEN accid, data_PITCHNAME pname, data_OCTAVE oct);
    static void AddEpisema(Nc *nc, episemaVis_FORM form, data_EVENTREL place);
    static void AddLiquescent(Nc *nc, curvatureDirection_CURVE curve);
    static std::optional<PitchOctaveType> FindPitch(char ch, int clefPitchOffset);
    static char GetCharAt(const std::string &input, int index);
    static curvatureDirection_CURVE InferLiquescentCurve(const Nc *previousNC, const Nc *currentNC);
    static PitchOctaveType MakePitchFromDiatonicIndex(int absoluteDiatonicIndex);
    static int PitchToDiatonicNumber(data_PITCHNAME pname, int oct);
    static int ProcessBarline(const std::string &music, int currentIndex, Layer *layer);

public:
    //
private:
    int m_currentClefPitchOffset;
    Layer *m_layer;
    // 17-may-2026 set when a `cb<n>` clef is parsed so Import() can add a KeySig to the StaffDef.
    bool m_pendingFlatOnClef = false;
    // 18-may-2026 accumulator of MEI @type tokens to apply to the *next* Nc created by ProcessNeume.
    // Used by prefix handlers (FindPrefix) for variants that have no dedicated MEI element in this
    // build: the neumatic-cut size flavors (`//`, `/0`, `/[n]`), the no-space prefix (`!`), and the
    // deprecated remove-first-stem prefix (`@`, S-GABC §6.1). Cleared after each Nc is created.
    std::string m_pendingNcType;
};

} // namespace vrv

#endif

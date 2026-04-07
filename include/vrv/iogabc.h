/////////////////////////////////////////////////////////////////////////////
// Name:        iogabc.h
// Author:      David Rizo
// Created:     2025
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_IOGABC_H__
#define __VRV_IOGABC_H__

#include <string>
#include <vector>

//----------------------------------------------------------------------------

#include "iobase.h"
#include "pugixml.hpp"
#include "vrvdef.h"

namespace vrv {
    class Layer;
    class Syllable;

//----------------------------------------------------------------------------
// GABCInput
//----------------------------------------------------------------------------

enum Prefixes {
    NO_SPACE,
    REMOVE_FIRST_STEM,
    NEUMATIC_CUT,
    OBLIQUE_LIGATURE
};

//TODO Comentarios estilo Verovio
class GABCInput : public Input {
public:
    // constructors and destructors
    GABCInput(Doc *doc);
    virtual ~GABCInput();

    bool Import(const std::string &volpiano) override;

private:
    // We have an internal structure with the equivalences of pitches to the G2.
    // In order to compute the pitch depending on the clef, we apply an offset.
    // To speed up, when the clef is found, this offset is updated
    // By default, if clef not present, we use G2 as reference
    int currentClefPitchOffset;

    Layer* layer;

    void addAccidental(Syllable *syllable, data_ACCIDENTAL_WRITTEN accid, data_PITCHNAME pname, data_OCTAVE oct);
    void addEpisema(Nc *nc, const std::string form);    
    void addLiquescent(Nc *nc, curvatureDirection_CURVE curve);

    // It returns a prefix and advances the index or NONE if not found.
    std::optional<Prefixes> findPrefix(const std::string& music, int& currentIndex);

    // It returns the number of processed chars
    int processBarline(const std::string& music, int currentIndex, Layer* layer);
    // It returns true if processed
    bool processClef(const std::string& word);
    void processInput(const std::string& gabc);
    // It returns the number of processed chars
    int processCustos(const std::string& word);
    void processNeume(const std::string& music, Syllable* syllable);
    void processWord(const std::string& lyrics, const std::string& music, sylLog_WORDPOS wordpos, sylLog_CON con);
        // it returns the consumed characters, 0 if it's not a suffix
    int processSuffix(const std::string& music, int currentIndex, Nc* nc, Nc* previousNC);


public:
};

} // namespace vrv

#endif

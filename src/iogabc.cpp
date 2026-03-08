/////////////////////////////////////////////////////////////////////////////
// Name:        iogabc.cpp
// Author:      David Rizo
// Created:     2025
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "iogabc.h"

//----------------------------------------------------------------------------

#include <array>
#include <cassert>
#include <cctype>
#include <fstream>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>

//----------------------------------------------------------------------------

#include "accid.h"
#include "atttypes.h"
#include "barline.h"
#include "clef.h"
#include "custos.h"
#include "doc.h"
#include "layer.h"
#include "liquescent.h"
#include "mdiv.h"
#include "measure.h"
#include "neume.h"
#include "nc.h"
#include "oriscus.h"
#include "quilisma.h"
#include "score.h"
#include "section.h"
#include "staff.h"
#include "staffdef.h"
#include "staffgrp.h"
#include "syl.h"
#include "syllable.h"
#include "text.h"
#include "vrv.h"


//////////////////////////////////////////////////////////////////////////
// Note: we've developed this parser thinking of the speed and the ease for others to understand.
// For easiness to maintain, and due to the simplicity of the encoding, 
// we've avoided here formal parsing techniques or regular expressions. 
// Following other importers, this parser suposes the input is valid.
//////////////////////////////////////////////////////////////////////////
// Helper functions
// It returns '\0' if the index is out of bounds, 
inline char getCharAt(std::string input, int index) {
    assert(index >= 0); //TODO Ver cómo lo hacen ellos

    auto idx = static_cast<std::size_t>(index);
    if (idx < 0 || idx >= input.size()) {
        return '\0'; 
    }
    return input[idx];    
}

inline void debug(std::string message) {
    std::cout << "DEBUG: " << message << "\n";
}

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// GABCInput
//----------------------------------------------------------------------------

GABCInput::GABCInput(Doc *doc) : Input(doc) {}

GABCInput::~GABCInput() {}




//////////////////////////////////////////////////////////////////////////
// Clefs
// Tuple = <Clef shape, clef line, pitch deviation from G2>
using ClefOctaveOffsetType = std::tuple<data_CLEFSHAPE, int, int>;

static const std::map<std::string, ClefOctaveOffsetType> GABC_CLEFS{
    {"c1",  {CLEFSHAPE_C, 1, 0}}, // TODO offset
    {"c2",  {CLEFSHAPE_C, 2, 0}},
    {"c3",  {CLEFSHAPE_C, 3, 0}},
    {"c4",  {CLEFSHAPE_C, 4, 0}},
    {"f2",  {CLEFSHAPE_F, 2, -1}},
    {"f3",  {CLEFSHAPE_F, 3, 0}},
    {"f4",  {CLEFSHAPE_F, 4, 0}},
    {"cb3", {CLEFSHAPE_C, 3, 0}}, // TODO With a flat
};


bool 
GABCInput::processClef(const std::string& word) {
    auto it = GABC_CLEFS.find(word);
    if (it == GABC_CLEFS.end()) {
        debug("Not a clef");
        return false; // not a clef
    }

    const auto& [shape, line, offset] = it->second; // tuple = {shape, line, offset}

    Clef* clef = new Clef();
    clef->IsAttribute(true);
    clef->SetLine(line);
    clef->SetShape(shape);
    layer->AddChild(clef); // TODO: "bajo" con staffDef

    this->currentClefPitchOffset = offset;
    debug("Clef found " + word);
    return true;
    // TODO: several clefs c1@c4, f3@f4
}

int
GABCInput::processCustos(const std::string& word) {
    char firstChar = getCharAt(word, 0);
    int processedChars = 0;
    if (firstChar == 'Z') {
        Custos* custos = new Custos(); //TODO posición?
        layer->AddChild(custos); 
        processedChars = 1;
    } else if (firstChar == 'z') {
        //TODO Qué hace el 0?
        Custos* custos = new Custos(); //TODO posición? -- función con el caso Z
        layer->AddChild(custos); 
        processedChars = 1;
        char nextChar = getCharAt(word, 1);
        if (nextChar == '0') {
            processedChars = 2; 
        } //TODO Qué hacemos con el 0?        
    }

    if (processedChars > 0) {
        debug("Custos found " + word);
    }
    return processedChars;
}


// It returns a prefix and advances the index or NONE if not found.
std::optional<Prefixes> 
GABCInput::findPrefix(const std::string& music, int& currentIndex) { 
    int nextCurrentIndex = currentIndex; 
    std::optional<Prefixes> result = std::nullopt; 
    switch (getCharAt(music, currentIndex)) { 
        case '!': 
            nextCurrentIndex++; 
            result = NO_SPACE; 
            break; 
        case '@': 
            nextCurrentIndex++; 
            result = REMOVE_FIRST_STEM; 
            break; 
        case '/': nextCurrentIndex++; 
            char char2 = getCharAt(music, nextCurrentIndex); 
            if (char2 == '/') { 
                // Prefix: "//" 
                result = NEUMATIC_CUT; //TODO Tipo 
            } else if (char2 == '0') { 
                    // Prefix: "/0" 
                    result = NEUMATIC_CUT; 
                    //TODO Tipo 
            } else if (char2 == '[') { 
                // Prefix: '[' '-'? [1-6] ']' 
                nextCurrentIndex++; 
                char next = getCharAt(music, nextCurrentIndex); 
                bool negative = false; 
                if (next == '-') { 
                    nextCurrentIndex++; 
                    next = getCharAt(music, nextCurrentIndex); 
                    negative = true; 
                } 
                if (next >= '1' && next <= '6') { 
                    int value = next - '0'; 
                    if (negative) { 
                        value = -value; 
                    } 
                    result = NEUMATIC_CUT; 
                    //TODO Tipo 
                } 
            } 
            break; 
    } 
    if (result.has_value()) { 
        debug("Prefix found");
        currentIndex = nextCurrentIndex; 
    } else {
        debug("No prefix found");
    }
    return result; 
}

void 
GABCInput::addAccidental(Nc *nc, data_ACCIDENTAL_WRITTEN accid) {
    Accid *accidChild = new Accid();
    accidChild->SetAccid(accid);
    nc->AddChild(accidChild);
}

void 
GABCInput::addEpisema(Nc *nc, const std::string form) {
    /*TODO no existe ?? Episema *episema = new Episema();
    episema->SetForm(form);
    nc->AddChild(episema);*/
}

//TODO curva anterior - depende del pitch NC anterior - Rising - Falling melody en DLFM
void
GABCInput::addLiquescent(Nc *nc, curvatureDirection_CURVE curve) {
    Liquescent *liquescent = new Liquescent();
    nc->AddChild(liquescent);
    nc->SetCurve(curve);
}

static int pitchToDiatonicNumber(data_PITCHNAME pname, int oct) {
    int diatonic = 0;
    switch (pname) {
        case PITCHNAME_c: diatonic = 0; break;
        case PITCHNAME_d: diatonic = 1; break;
        case PITCHNAME_e: diatonic = 2; break;
        case PITCHNAME_f: diatonic = 3; break;
        case PITCHNAME_g: diatonic = 4; break;
        case PITCHNAME_a: diatonic = 5; break;
        case PITCHNAME_b: diatonic = 6; break;
        default:          diatonic = 0; break;
    }
    return oct * 7 + diatonic;
}

static curvatureDirection_CURVE inferLiquescentCurve(const Nc* previousNC, const Nc* currentNC) {
    if (previousNC == nullptr || currentNC == nullptr) {
        return curvatureDirection_CURVE_NONE;
    }

    int prevPitch = pitchToDiatonicNumber(previousNC->GetPname(), previousNC->GetOct());
    int currPitch = pitchToDiatonicNumber(currentNC->GetPname(), currentNC->GetOct());

    if (currPitch > prevPitch) {
        return curvatureDirection_CURVE_a; // rising melody
    }
    if (currPitch < prevPitch) {
        return curvatureDirection_CURVE_c; // falling melody
    }
    return curvatureDirection_CURVE_NONE;
}

int
GABCInput::processSuffix(const std::string& music, int currentIndex, Nc* nc, Nc* previousNC) {
    int processedChars = 0;
    char nextChar = getCharAt(music, currentIndex);

    switch (nextChar) {
        case 'V':
            processedChars++;
            nc->SetTilt(COMPASSDIRECTION_n);
            break;

        case 'v':
            processedChars++;
            nc->SetTilt(COMPASSDIRECTION_s);
            break;

        case 's': // strophicus
            processedChars++;
            // TODO if needed: encode repeated strophicus logic at neume level
            break;

        case 'w': // quilisma
        case 'W':
            processedChars++;
            nc->AddChild(new Quilisma());
            break;

        case 'o': // oriscus
        case 'O':
            processedChars++;
            nc->AddChild(new Oriscus());
            break;

        case '~': // liquescent depending on melodic contour
            processedChars++;
            addLiquescent(nc, inferLiquescentCurve(previousNC, nc));
            break;

        case '>': // liquescent two tails down
            processedChars++;
            addLiquescent(nc, curvatureDirection_CURVE_c);
            break;

        case '<': // liquescent two tails up
            processedChars++;
            addLiquescent(nc, curvatureDirection_CURVE_a);
            break;

        case 'x': // flat
            processedChars++;
            addAccidental(nc, ACCIDENTAL_WRITTEN_f);
            break;

        case '#': // sharp
            processedChars++;
            addAccidental(nc, ACCIDENTAL_WRITTEN_s);
            break;

        case 'y': // natural
            processedChars++;
            addAccidental(nc, ACCIDENTAL_WRITTEN_n);
            break;

        case '\'': // episema_vertical
            processedChars++;
            break;

        case '_': // episema_horizontal
            processedChars++;
            nextChar = getCharAt(music, currentIndex + processedChars);
            if (nextChar >= '0' && nextChar <= '5') {
                processedChars++;
                // TODO horizontal episema subtype if needed
            }
            break;

        case '.': // punctum_mora
            processedChars++;
            nextChar = getCharAt(music, currentIndex + processedChars);
            if (nextChar == '.') {
                processedChars++;
                nc->SetDots(2);
            }
            else {
                nc->SetDots(1);
            }
            break;

        default:
            break;
    }

    return processedChars;
}


int 
GABCInput::processBarline(const std::string& music, int currentIndex, Layer* layer) {
    int processedChars = 0; 
    switch (getCharAt(music, currentIndex)) { 
        case ':': 
            debug("Barline found: single");
            processedChars++; //TODO
            BarLine *barline = new BarLine();
            barline->SetForm(BARRENDITION_single);
            layer->AddChild(barline);
            break;
    }
    return processedChars;
}



// (pitch, octave)
using PitchOctaveType = std::pair<data_PITCHNAME, int>;

// ---------- a..m → [PITCHNAME_*, octave] mapping ----------
static constexpr std::array<PitchOctaveType, 13> kLetterMap = {{
    {PITCHNAME_b, 2}, // a -> B2
    {PITCHNAME_c, 3}, // b -> C3
    {PITCHNAME_d, 3}, // c -> D3
    {PITCHNAME_e, 3}, // d -> E3
    {PITCHNAME_f, 3}, // e -> F3
    {PITCHNAME_g, 3}, // f -> G3
    {PITCHNAME_a, 3}, // g -> A3
    {PITCHNAME_b, 3}, // h -> B3
    {PITCHNAME_c, 4}, // i -> C4
    {PITCHNAME_d, 4}, // j -> D4
    {PITCHNAME_e, 4}, // k -> E4
    {PITCHNAME_f, 4}, // l -> F4
    {PITCHNAME_g, 4}, // m -> G4
}};

std::optional<PitchOctaveType>
findPitch(char ch, int currentClefPitchOffset) {
    int idx = ch + currentClefPitchOffset - 'a';

    std::size_t uidx = static_cast<std::size_t>(idx);
    if (uidx >= kLetterMap.size()) {
        return std::nullopt;
    } else {
        return kLetterMap[uidx];
    }
}


Neume*
GABCInput::processNeume(const std::string& music) {
    debug("Processing neume");

    Neume* neume = new Neume();
    Nc* previousNC = nullptr;

    int currentIndex = 0;
    while (static_cast<std::size_t>(currentIndex) < music.size()) {
        std::optional<Prefixes> prefixOpt = findPrefix(music, currentIndex);
        std::optional<PitchOctaveType> pitchOctOpt = std::nullopt;

        char ch = getCharAt(music, currentIndex);
        bool diamond = false;

        if (ch >= 'A' && ch <= 'M') {
            // Rhombus / inclinatum
            ch -= ('A' - 'a');
            diamond = true;
        }

        pitchOctOpt = findPitch(ch, this->currentClefPitchOffset);

        if (pitchOctOpt.has_value()) {
            currentIndex++;
            auto [pname, oct] = pitchOctOpt.value();

            Nc* currentNC = new Nc();
            neume->AddChild(currentNC);
            currentNC->SetPname(pname);
            currentNC->SetOct(oct);

            if (diamond) {
                currentNC->SetTilt(COMPASSDIRECTION_se);
            }

            if (prefixOpt.has_value()) {
                Prefixes prefix = prefixOpt.value();
                std::cout << "Prefix found: " << static_cast<int>(prefix) << "\n"; // TODO usarlo
            }

            // Process all suffixes attached to this note
            while (static_cast<std::size_t>(currentIndex) < music.size()) {
                int suffixChars = processSuffix(music, currentIndex, currentNC, previousNC);
                if (suffixChars == 0) {
                    break;
                }
                currentIndex += suffixChars;
            }

            previousNC = currentNC;
        }
        else {
            int processedChars = processBarline(music, currentIndex, layer);
            if (processedChars > 0) {
                currentIndex += processedChars;
            }
            else {
                debug(std::string("Unknown neume character: ") + static_cast<char>(ch));
                currentIndex++;
            }
        }

        // TODO ¿si hay prefijo pero no nota?
        // TODO rombos - efectos vocales....
    }

    return neume;
}


void 
GABCInput::processWord(const std::string& lyrics, const std::string& music) {    
        Syllable* syllable = new Syllable();
        layer->AddChild(syllable);

        if (!lyrics.empty()) {
            debug("Processing lyrics: " + lyrics);
            // <syl> with <text>
            Syl* syl = new Syl();
            syllable->AddChild(syl);
            Text* t = new Text();
            t->SetText(std::u32string(lyrics.begin(), lyrics.end())); 
            syl->AddChild(t);
        }

        if (!music.empty()) {
            debug("Processing music: " + music);
            bool isClef = processClef(music);
            if (!isClef) {
                bool isCustos = processCustos(music);
                if (!isCustos) {
                    Neume* neume = processNeume(music);
                    syllable->AddChild(neume);
                }
            }
        }

}

void
GABCInput::processInput(const std::string& gabc) {
    // Normalize: make ')' also act as a separator when not followed by space
    std::string norm;
    norm.reserve(gabc.size() * 2);
    for (char c : gabc) {
        norm.push_back(c);
        if (c == ')') norm.push_back(' ');   // <-- key trick
    }


    std::istringstream iss(norm);   // splits on any whitespace by default
    std::string word;
    while (iss >> word) {
        std::cerr << "Processing word: " << word << "\n"; //TODO
        std::string lyrics, music;
        auto open  = word.find('(');
        auto close = word.rfind(')');

        if (open != std::string::npos && close != std::string::npos && close > open) {
            lyrics = word.substr(0, open); // may be empty
            music  = word.substr(open + 1, close - open - 1); // may be empty
        } else {
            lyrics = word; // no parentheses, take whole word as lyrics //TODO
        }

        processWord(lyrics, music);
    }    
}

bool 
GABCInput::Import(const std::string &gabc)
{
    currentClefPitchOffset = 0;
    m_doc->Reset();
    m_doc->SetType(Raw);
    m_doc->SetMensuralMusicOnly(BOOLEAN_true); // no measures

    Mdiv *mdiv = new Mdiv();
    mdiv->SetVisibility(Visible);
    m_doc->AddChild(mdiv);
    Score *score = new Score();
    mdiv->AddChild(score);
    Section *section = new Section();
    score->AddChild(section);
    Staff *staff = new Staff(1);
    Measure *measure = new Measure(UNMEASURED, 1);
    this->layer = new Layer();
    this->layer->SetN(1);
    staff->AddChild(this->layer);
    measure->AddChild(staff);
    section->AddChild(measure);

    processInput(gabc);    

    // add minimal scoreDef
    StaffGrp *staffGrp = new StaffGrp();
    StaffDef *staffDef = new StaffDef();
    staffDef->SetNotationtype(NOTATIONTYPE_neume);
    staffDef->SetN(1);
    staffDef->SetLines(4); //TODO code for lines
    staffGrp->AddChild(staffDef);
    m_doc->GetFirstScoreDef()->AddChild(staffGrp);

    m_doc->ConvertToPageBasedDoc();

    return true;
}

} // namespace vrv

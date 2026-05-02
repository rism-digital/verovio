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
#include "nc.h"
#include "neume.h"
#include "oriscus.h"
#include "quilisma.h"
#include "score.h"
#include "section.h"
#include "staff.h"
#include "staffdef.h"
#include "staffgrp.h"
#include "strophicus.h"
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

namespace vrv {

//----------------------------------------------------------------------------
// GABCInput
//----------------------------------------------------------------------------

GABCInput::GABCInput(Doc *doc) : Input(doc) {}

GABCInput::~GABCInput() {}

char GABCInput::GetCharAt(const std::string &input, int index)
{
    assert(index >= 0);

    auto idx = static_cast<std::size_t>(index);
    if (idx >= input.size()) {
        return '\0';
    }
    return input[idx];
}

void GABCInput::Debug(const std::string &message)
{
    std::cerr << "DEBUG: " << message << "\n";
}

//////////////////////////////////////////////////////////////////////////
// Clefs
// Tuple = <Clef shape, clef line, diatonic pitch deviation from C2>
using ClefPitchOffsetType = std::tuple<data_CLEFSHAPE, int, int>;

// GABC letters a..m are interpreted relative to the active clef. We anchor the
// base table on c2, following the older reference converter:
//   c2: a..m = e3 f3 g3 a3 b3 c4 d4 e4 f4 g4 a4 b4 c5
// Other clefs are expressed as diatonic offsets from that c2 mapping.
static const std::map<std::string, ClefPitchOffsetType> GABC_CLEFS{
    { "c1", { CLEFSHAPE_C, 1, 2 } }, { "c2", { CLEFSHAPE_C, 2, 0 } }, { "c3", { CLEFSHAPE_C, 3, -2 } },
    { "c4", { CLEFSHAPE_C, 4, -4 } }, { "f2", { CLEFSHAPE_F, 2, -4 } }, { "f3", { CLEFSHAPE_F, 3, -6 } },
    { "f4", { CLEFSHAPE_F, 4, -8 } }, { "cb3", { CLEFSHAPE_C, 3, -2 } }, // TODO add the flat sign itself
};

bool GABCInput::ProcessClef(const std::string &word)
{
    auto it = GABC_CLEFS.find(word);
    if (it == GABC_CLEFS.end()) {
        Debug("Not a clef");
        return false; // not a clef
    }

    const auto &[shape, line, offset] = it->second; // tuple = {shape, line, offset}

    Clef *clef = new Clef();
    clef->SetLine(line);
    clef->SetShape(shape);
    layer->AddChild(clef); // TODO: "bajo" con staffDef

    this->currentClefPitchOffset = offset;
    Debug("Clef found " + word);
    return true;
    // TODO: several clefs c1@c4, f3@f4
}

int GABCInput::ProcessCustos(const std::string &word)
{
    char firstChar = GetCharAt(word, 0);
    int processedChars = 0;
    if (firstChar == 'Z') {
        Custos *custos = new Custos(); // TODO posición?
        layer->AddChild(custos);
        processedChars = 1;
    }
    else if (firstChar == 'z') {
        // TODO Qué hace el 0?
        Custos *custos = new Custos(); // TODO posición? -- función con el caso Z
        layer->AddChild(custos);
        processedChars = 1;
        char nextChar = GetCharAt(word, 1);
        if (nextChar == '0') {
            processedChars = 2;
        } // TODO Qué hacemos con el 0?
    }

    if (processedChars > 0) {
        Debug("Custos found " + word);
    }
    return processedChars;
}

// It returns a prefix and advances the index or NONE if not found.
std::optional<Prefixes> GABCInput::FindPrefix(const std::string &music, int &currentIndex)
{
    int nextCurrentIndex = currentIndex;
    std::optional<Prefixes> result = std::nullopt;
    // º = U+00BA = UTF-8 bytes 0xC2 0xBA: oblique ligature prefix
    if (static_cast<unsigned char>(GetCharAt(music, currentIndex)) == 0xC2
        && static_cast<unsigned char>(GetCharAt(music, currentIndex + 1)) == 0xBA) {
        currentIndex += 2;
        Debug("Prefix found: oblique ligature");
        return OBLIQUE_LIGATURE;
    }

    switch (GetCharAt(music, currentIndex)) {
        case '!':
            nextCurrentIndex++;
            result = NO_SPACE;
            break;
        case '@':
            nextCurrentIndex++;
            result = REMOVE_FIRST_STEM;
            break;
        case '/':
            nextCurrentIndex++;
            char char2 = GetCharAt(music, nextCurrentIndex);
            if (char2 == '/') {
                // Prefix: "//"
                result = NEUMATIC_CUT; // TODO Tipo
            }
            else if (char2 == '0') {
                // Prefix: "/0"
                result = NEUMATIC_CUT;
                // TODO Tipo
            }
            else if (char2 == '[') {
                // Prefix: '[' '-'? [1-6] ']'
                nextCurrentIndex++;
                char next = GetCharAt(music, nextCurrentIndex);
                bool negative = false;
                if (next == '-') {
                    nextCurrentIndex++;
                    next = GetCharAt(music, nextCurrentIndex);
                    negative = true;
                }
                if (next >= '1' && next <= '6') {
                    int value = next - '0';
                    if (negative) {
                        value = -value;
                    }
                    result = NEUMATIC_CUT;
                    // TODO Tipo
                }
            }
            break;
    }
    if (result.has_value()) {
        Debug("Prefix found");
        currentIndex = nextCurrentIndex;
    }
    else {
        Debug("No prefix found");
    }
    return result;
}

void GABCInput::AddAccidental(Syllable *syllable, data_ACCIDENTAL_WRITTEN accid, data_PITCHNAME pname, data_OCTAVE oct)
{
    Debug("Accidental found: " + std::to_string(accid));
    Accid *accidElem = new Accid();
    accidElem->SetAccid(accid);
    accidElem->SetPloc(pname);
    accidElem->SetOloc(oct);
    syllable->AddChild(accidElem);
}

void GABCInput::AddEpisema(Nc *nc, const std::string &form)
{
    /*TODO no existe ?? Episema *episema = new Episema();
    episema->SetForm(form);
    nc->AddChild(episema);*/
}

// TODO curva anterior - depende del pitch NC anterior - Rising - Falling melody en DLFM
void GABCInput::AddLiquescent(Nc *nc, curvatureDirection_CURVE curve)
{
    Liquescent *liquescent = new Liquescent();
    nc->AddChild(liquescent);
    nc->SetCurve(curve);
}

int GABCInput::PitchToDiatonicNumber(data_PITCHNAME pname, int oct)
{
    int diatonic = 0;
    switch (pname) {
        case PITCHNAME_c: diatonic = 0; break;
        case PITCHNAME_d: diatonic = 1; break;
        case PITCHNAME_e: diatonic = 2; break;
        case PITCHNAME_f: diatonic = 3; break;
        case PITCHNAME_g: diatonic = 4; break;
        case PITCHNAME_a: diatonic = 5; break;
        case PITCHNAME_b: diatonic = 6; break;
        default: diatonic = 0; break;
    }
    return oct * 7 + diatonic;
}

curvatureDirection_CURVE GABCInput::InferLiquescentCurve(const Nc *previousNC, const Nc *currentNC)
{
    if (previousNC == nullptr || currentNC == nullptr) {
        return curvatureDirection_CURVE_NONE;
    }

    int prevPitch = PitchToDiatonicNumber(previousNC->GetPname(), previousNC->GetOct());
    int currPitch = PitchToDiatonicNumber(currentNC->GetPname(), currentNC->GetOct());

    if (currPitch > prevPitch) {
        return curvatureDirection_CURVE_a; // rising melody
    }
    if (currPitch < prevPitch) {
        return curvatureDirection_CURVE_c; // falling melody
    }
    return curvatureDirection_CURVE_NONE;
}

int GABCInput::ProcessSuffix(const std::string &music, int currentIndex, Nc *nc, Nc *previousNC)
{
    int processedChars = 0;
    char nextChar = GetCharAt(music, currentIndex);

    switch (nextChar) {
        case 'V':
            processedChars++;
            nc->SetTilt(COMPASSDIRECTION_n);
            break;

        case 'v':
            processedChars++;
            nc->SetTilt(COMPASSDIRECTION_s);
            break;

        case 's': // strophicus — handled before the suffix loop in ProcessNeume
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
            AddLiquescent(nc, InferLiquescentCurve(previousNC, nc));
            break;

        case '>': // liquescent two tails down
            processedChars++;
            AddLiquescent(nc, curvatureDirection_CURVE_c);
            // Cephalicus only when there is no explicit virga following
            if (GetCharAt(music, currentIndex + processedChars) != 'V') {
                nc->SetType("cephalicus");
            }
            break;

        case '<': // liquescent two tails up
            processedChars++;
            AddLiquescent(nc, curvatureDirection_CURVE_a);
            break;

        case '\'': // episema_vertical
            processedChars++;
            break;

        case '_': // episema_horizontal
            processedChars++;
            nextChar = GetCharAt(music, currentIndex + processedChars);
            if (nextChar >= '0' && nextChar <= '5') {
                processedChars++;
                // TODO horizontal episema subtype if needed
            }
            break;

        case '.': // punctum_mora
            processedChars++;
            nextChar = GetCharAt(music, currentIndex + processedChars);
            if (nextChar == '.') {
                processedChars++;
                nc->SetDots(2);
            }
            else {
                nc->SetDots(1);
            }
            break;

        default: break;
    }

    return processedChars;
}

int GABCInput::ProcessBarline(const std::string &music, int currentIndex, Layer *layer)
{
    int processedChars = 0;
    switch (GetCharAt(music, currentIndex)) {
        case ':':
            Debug("Barline found: single");
            processedChars++; // TODO
            BarLine *barline = new BarLine();
            barline->SetForm(BARRENDITION_single);
            layer->AddChild(barline);
            break;
    }
    return processedChars;
}

// (pitch, octave)
static constexpr std::array<data_PITCHNAME, 7> kDiatonicPitchNames = { {
    PITCHNAME_c,
    PITCHNAME_d,
    PITCHNAME_e,
    PITCHNAME_f,
    PITCHNAME_g,
    PITCHNAME_a,
    PITCHNAME_b,
} };

GABCInput::PitchOctaveType GABCInput::MakePitchFromDiatonicIndex(int absoluteDiatonicIndex)
{
    // absoluteDiatonicIndex is counted from C0 = 0.
    int octave = absoluteDiatonicIndex / 7;
    int pitchClass = absoluteDiatonicIndex % 7;
    if (pitchClass < 0) {
        pitchClass += 7;
        octave -= 1;
    }
    return { kDiatonicPitchNames[static_cast<std::size_t>(pitchClass)], octave };
}

std::optional<GABCInput::PitchOctaveType> GABCInput::FindPitch(char ch, int clefPitchOffset)
{
    if (ch < 'a' || ch > 'm') {
        return std::nullopt;
    }

    // Base c2 mapping: a..m = e3..c5.
    constexpr int c2BaseDiatonicIndexForA = 3 * 7 + 2; // E3
    int letterIndex = ch - 'a';
    int absoluteDiatonicIndex = c2BaseDiatonicIndexForA + letterIndex + clefPitchOffset;

    return MakePitchFromDiatonicIndex(absoluteDiatonicIndex);
}

void GABCInput::ProcessNeume(const std::string &music, Syllable *syllable)
{
    Debug("Processing neume");

    Neume *neume = new Neume();
    Nc *previousNC = nullptr;
    bool ligateNext = false; // set after OBLIQUE_LIGATURE to mark the following nc as ligated
    bool lastWasStrophicus = false; // true when last nc was a strophicus
    data_PITCHNAME strophicusPname = PITCHNAME_NONE;
    int strophicusOct = 0;

    int currentIndex = 0;
    while (static_cast<std::size_t>(currentIndex) < music.size()) {

        char ch = GetCharAt(music, currentIndex);

        // Check for simple '/' neume separator (not '//', '/0', '/[')
        if (ch == '/') {
            char next = GetCharAt(music, currentIndex + 1);
            if (next != '/' && next != '0' && next != '[') {
                if (lastWasStrophicus) {
                    // Only skip the separator if the next note is also a strophicus (pitch + 's')
                    bool nextIsStrophicus = (next >= 'a' && next <= 'm') && (GetCharAt(music, currentIndex + 2) == 's');
                    if (nextIsStrophicus) {
                        currentIndex++;
                        continue;
                    }
                }
                // Finalize current neume and start a new one.
                // previousNC is kept for potential curve inference in the next neume.
                lastWasStrophicus = false;
                syllable->AddChild(neume);
                neume = new Neume();
                currentIndex++;
                continue;
            }
        }

        // Repeated bare 's' after a strophicus nc: create another nc at the same pitch
        if (lastWasStrophicus && ch == 's') {
            Nc *strophNC = new Nc();
            strophNC->SetPname(strophicusPname);
            strophNC->SetOct(static_cast<data_OCTAVE>(strophicusOct));
            strophNC->AddChild(new Strophicus());
            neume->AddChild(strophNC);
            previousNC = strophNC;
            currentIndex++;
            continue;
        }

        // Any other character ends the strophicus group
        lastWasStrophicus = false;

        std::optional<Prefixes> prefixOpt = FindPrefix(music, currentIndex);

        ch = GetCharAt(music, currentIndex); // re-read after prefix may have consumed chars
        bool diamond = false;

        if (ch >= 'A' && ch <= 'M') {
            // Rhombus / inclinatum
            ch -= ('A' - 'a');
            diamond = true;
        }

        auto pitchOctOpt = FindPitch(ch, this->currentClefPitchOffset);

        if (pitchOctOpt.has_value()) {
            currentIndex++;
            auto [pname, oct] = pitchOctOpt.value();

            Nc *currentNC = new Nc();
            currentNC->SetPname(pname);
            currentNC->SetOct(oct);

            if (diamond) {
                currentNC->SetTilt(COMPASSDIRECTION_se);
            }

            if (prefixOpt.has_value() && prefixOpt.value() == OBLIQUE_LIGATURE) {
                // Oblique ligature: this nc gets the oblique tilt and starts the ligature
                currentNC->SetTilt(COMPASSDIRECTION_n);
                currentNC->SetLigated(BOOLEAN_true);
                ligateNext = true;
            }
            else if (ligateNext) {
                // Second nc in the oblique ligature
                currentNC->SetLigated(BOOLEAN_true);
                ligateNext = false;
            }

            // Check for accidental suffix first — it goes to syllable before the neume
            char nextChar = GetCharAt(music, currentIndex);
            if (nextChar == 'x') {
                currentIndex++;
                AddAccidental(syllable, ACCIDENTAL_WRITTEN_f, pname, oct);
            }
            else if (nextChar == '#') {
                currentIndex++;
                AddAccidental(syllable, ACCIDENTAL_WRITTEN_s, pname, oct);
            }
            else if (nextChar == 'y') {
                currentIndex++;
                AddAccidental(syllable, ACCIDENTAL_WRITTEN_n, pname, oct);
            }

            // Strophicus: consume 's' before the general suffix loop
            nextChar = GetCharAt(music, currentIndex);
            if (nextChar == 's') {
                currentIndex++;
                currentNC->AddChild(new Strophicus());
                lastWasStrophicus = true;
                strophicusPname = pname;
                strophicusOct = oct;
            }

            // A liquescent note always starts its own neume.
            // If the current neume already has ncs, finalize it first.
            // previousNC is kept so InferLiquescentCurve can use it across the neume boundary.
            nextChar = GetCharAt(music, currentIndex);
            if ((nextChar == '~' || nextChar == '>' || nextChar == '<') && neume->GetChildCount() > 0) {
                syllable->AddChild(neume);
                neume = new Neume();
            }

            neume->AddChild(currentNC);

            // Process remaining suffixes attached to this note
            while (static_cast<std::size_t>(currentIndex) < music.size()) {
                int suffixChars = ProcessSuffix(music, currentIndex, currentNC, previousNC);
                if (suffixChars == 0) {
                    break;
                }
                currentIndex += suffixChars;
            }

            previousNC = currentNC;
        }
        else {
            int processedChars = ProcessBarline(music, currentIndex, layer);
            if (processedChars > 0) {
                currentIndex += processedChars;
            }
            else {
                Debug(std::string("Unknown neume character: ") + static_cast<char>(ch));
                currentIndex++;
            }
        }
    }

    syllable->AddChild(neume);
}

void GABCInput::ProcessWord(const std::string &lyrics, const std::string &music, sylLog_WORDPOS wordpos, sylLog_CON con)
{
    Syllable *syllable = new Syllable();
    layer->AddChild(syllable);

    if (!lyrics.empty()) {
        Debug("Processing lyrics: " + lyrics);
        Syl *syl = new Syl();
        if (wordpos != sylLog_WORDPOS_NONE) syl->SetWordpos(wordpos);
        if (con != sylLog_CON_NONE) syl->SetCon(con);
        syllable->AddChild(syl);
        Text *t = new Text();
        t->SetText(std::u32string(lyrics.begin(), lyrics.end()));
        syl->AddChild(t);
    }

    if (!music.empty()) {
        Debug("Processing music: " + music);
        bool isClef = ProcessClef(music);
        if (!isClef) {
            bool isCustos = ProcessCustos(music);
            if (!isCustos) {
                ProcessNeume(music, syllable);
            }
        }
    }
}

void GABCInput::ProcessInput(const std::string &gabc)
{
    // Parse tokens preserving word-boundary information.
    // A "word boundary" exists when whitespace appears between syllables in the original input.
    // Empty-lyrics tokens (clefs, barlines) do not consume the pending boundary.
    struct Token {
        std::string lyrics, music;
        bool wordBoundaryBefore;
    };
    std::vector<Token> tokens;

    int i = 0;
    const int n = static_cast<int>(gabc.size());
    bool pendingBoundary = true; // start of input counts as a word boundary

    while (i < n) {
        // Consume whitespace — sets pendingBoundary
        while (i < n && std::isspace(static_cast<unsigned char>(gabc[i]))) {
            pendingBoundary = true;
            i++;
        }
        if (i >= n) break;

        // Read lyrics (everything before the opening '(' or whitespace)
        std::string lyrics;
        while (i < n && gabc[i] != '(' && !std::isspace(static_cast<unsigned char>(gabc[i]))) {
            lyrics += gabc[i++];
        }

        // Read music between '(' and ')'
        std::string music;
        if (i < n && gabc[i] == '(') {
            i++; // skip '('
            while (i < n && gabc[i] != ')') {
                music += gabc[i++];
            }
            if (i < n) i++; // skip ')'
        }

        bool boundary = pendingBoundary;
        // Only lyrics tokens consume the pending boundary; clef/barline tokens leave it intact
        if (!lyrics.empty()) {
            pendingBoundary = false;
        }

        Debug("Processing token: lyrics='" + lyrics + "' music='" + music + "'");
        tokens.push_back({ lyrics, music, boundary });
    }

    // Assign wordpos and con, then process each token
    const int count = static_cast<int>(tokens.size());
    for (int ti = 0; ti < count; ti++) {
        sylLog_WORDPOS wordpos = sylLog_WORDPOS_NONE;
        sylLog_CON con = sylLog_CON_NONE;

        if (!tokens[ti].lyrics.empty()) {
            bool isFirst = tokens[ti].wordBoundaryBefore;

            // isLast: next lyrics token starts a new word (or there is none)
            bool isLast = true;
            for (int j = ti + 1; j < count; j++) {
                if (!tokens[j].lyrics.empty()) {
                    isLast = tokens[j].wordBoundaryBefore;
                    break;
                }
            }

            if (isFirst && isLast)
                wordpos = sylLog_WORDPOS_s;
            else if (isFirst) {
                wordpos = sylLog_WORDPOS_i;
                con = sylLog_CON_d;
            }
            else if (isLast)
                wordpos = sylLog_WORDPOS_t;
            else {
                wordpos = sylLog_WORDPOS_m;
                con = sylLog_CON_d;
            }
        }

        ProcessWord(tokens[ti].lyrics, tokens[ti].music, wordpos, con);
    }
}

bool GABCInput::Import(const std::string &gabc)
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

    ProcessInput(gabc);

    // add minimal scoreDef
    StaffGrp *staffGrp = new StaffGrp();
    StaffDef *staffDef = new StaffDef();
    staffDef->SetNotationtype(NOTATIONTYPE_neume);
    staffDef->SetN(1);
    staffDef->SetLines(4); // TODO code for lines
    staffGrp->AddChild(staffDef);
    m_doc->GetFirstScoreDef()->AddChild(staffGrp);

    m_doc->ConvertToPageBasedDoc();

    return true;
}

} // namespace vrv

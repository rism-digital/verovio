/////////////////////////////////////////////////////////////////////////////
// Name:        iogabc.cpp
// Author:      David Rizo
// Created:     2025
// Copyright (c) Authors and others. All rights reserved.
//
// The grammar implemented here follows the S-GABC paper:
//   Rizo et al., "A Preliminary Proposal for a Systematic GABC Encoding of
//   Gregorian Chant", DLfM 2024.
// LaTeX source (master copy used while iterating on this branch):
//   /Users/drizo/cmg/investigacion/congresos/2024/dlfm2024/DLfM[2024] - S-GABC/
//     sample-authordraft.tex
// `grule <name>` mentions in the inline 17-may-2026 comments map to
// \label{<name>} inside that .tex (the GABC grammar figure).
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
#include "divline.h"
#include "doc.h"
#include "episema.h"
#include "keyaccid.h"
#include "keysig.h"
#include "layer.h"
#include "liquescent.h"
#include "mdiv.h"
#include "measure.h"
#include "nc.h"
#include "neume.h"
#include "options.h"
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

//////////////////////////////////////////////////////////////////////////
// Clefs
// Tuple = <Clef shape, clef line, diatonic pitch deviation from C2>
using ClefPitchOffsetType = std::tuple<data_CLEFSHAPE, int, int>;

// GABC letters a..m are interpreted relative to the active clef. We anchor the
// base table on c2, following the older reference converter:
//   c2: a..m = e3 f3 g3 a3 b3 c4 d4 e4 f4 g4 a4 b4 c5
// Other clefs are expressed as diatonic offsets from that c2 mapping.
// 17-may-2026 added c5 / f5 (grule clef, grule clef_number num_5; only valid on a 5-line staff,
// see Inconsistencies table feature 3) and cb3 (grule clef_flat, see paper section "Brief intro
// to GABC"). The flat sign for cb3 is materialised below as a KeySig attached to the StaffDef.
static const std::map<std::string, ClefPitchOffsetType> GABC_CLEFS{
    { "c1", { CLEFSHAPE_C, 1, 2 } },
    { "c2", { CLEFSHAPE_C, 2, 0 } },
    { "c3", { CLEFSHAPE_C, 3, -2 } },
    { "c4", { CLEFSHAPE_C, 4, -4 } },
    { "c5", { CLEFSHAPE_C, 5, -6 } },
    { "f2", { CLEFSHAPE_F, 2, -4 } },
    { "f3", { CLEFSHAPE_F, 3, -6 } },
    { "f4", { CLEFSHAPE_F, 4, -8 } },
    { "f5", { CLEFSHAPE_F, 5, -10 } },
    { "cb1", { CLEFSHAPE_C, 1, 2 } },
    { "cb2", { CLEFSHAPE_C, 2, 0 } },
    { "cb3", { CLEFSHAPE_C, 3, -2 } },
    { "cb4", { CLEFSHAPE_C, 4, -4 } },
};

bool GABCInput::ProcessClef(const std::string &word)
{
    // Handle the GABC multi-clef syntax `c2@c4` / `f3@f4` (paper section "Inconsistencies
    // between engraving software", Inconsistencies table feature 1). The first clef is the
    // pitch reference; the second clef appears on the staff for visual context only.
    // Secondary is emitted first so the primary ends up last in the MEI layer — Verovio uses
    // the last clef before notes to determine visual staff position.
    std::string::size_type at = word.find('@');
    if (at != std::string::npos) {
        const std::string first = word.substr(0, at);
        const std::string second = word.substr(at + 1);
        if (GABC_CLEFS.find(first) == GABC_CLEFS.end() || GABC_CLEFS.find(second) == GABC_CLEFS.end()) {
            return false;
        }
        // Add secondary clef to the layer first, then primary last so that the
        // primary is the active rendering clef (Verovio uses the last clef before
        // notes to determine visual position). m_currentClefPitchOffset ends up
        // holding the primary (first) clef's offset — correct for pitch mapping.
        this->ProcessClef(second);
        this->ProcessClef(first);
        return true;
    }

    auto it = GABC_CLEFS.find(word);
    if (it == GABC_CLEFS.end()) {
        LogDebug("Not a clef");
        return false;
    }

    const auto &[shape, line, offset] = it->second;

    Clef *clef = new Clef();
    clef->SetLine(line);
    clef->SetShape(shape);
    m_layer->AddChild(clef);

    // grule clef_flat (`cb<n>`). The flat sign attached to the clef is rendered by
    // emitting a KeySig with a single b-flat KeyAccid on the staff line that holds `b` for the
    // chosen clef. We keep a flag so Import() can attach the KeySig to the StaffDef.
    if (!word.empty() && word.length() >= 2 && word[1] == 'b') {
        m_pendingFlatOnClef = true;
    }

    m_currentClefPitchOffset = offset;
    LogDebug("Clef found %s", word.c_str());
    return true;
}

int GABCInput::ProcessCustos(const std::string &word)
{
    // 18-may-2026 GABC custos grammar (grule custos): three lexical variants exist and the
    // distinction is musicologically meaningful, so we preserve it via Custos/@type instead of
    // collapsing all three to the same MEI element.
    //   `Z`  → manual / forced custos placed at the explicit GABC position.
    //   `z`  → automatic end-of-line custos (the engraver inserts it implicitly).
    //   `z0` → automatic custos with the "no-clef-change-follows" annotation; some engravers
    //          (e.g. Gregorio) suppress its rendering, others draw it greyed out. We keep the
    //          element so the round-trip is lossless; downstream tooling / a future render
    //          option can choose to hide it.
    // The exact horizontal position relative to the next clef change is a render-time concern
    // and is left to the layout pass — we only set the type tag here.
    char firstChar = GetCharAt(word, 0);
    int processedChars = 0;
    if (firstChar == 'Z') {
        Custos *custos = new Custos();
        custos->SetType("manual");
        m_layer->AddChild(custos);
        processedChars = 1;
    }
    else if (firstChar == 'z') {
        Custos *custos = new Custos();
        processedChars = 1;
        char nextChar = GetCharAt(word, 1);
        if (nextChar == '0') {
            custos->SetType("auto-suppressed");
            processedChars = 2;
        }
        else {
            custos->SetType("auto");
        }
        m_layer->AddChild(custos);
    }

    if (processedChars > 0) {
        LogDebug("Custos found %s", word.c_str());
    }
    return processedChars;
}

// It returns a prefix and advances the index or NONE if not found.
// 18-may-2026 Side effect: when a prefix carries variant information that we want to preserve
// across to the next Nc (cut size, no-space, deprecated remove-first-stem), the tag is appended
// to m_pendingNcType. ProcessNeume drains the accumulator into the next Nc's @type and clears it.
// The oblique-ligature prefix continues to be communicated via the return value because it
// triggers structural (tilt/ligated) attribute changes, not a free-form @type tag.
std::optional<GABCPrefixes> GABCInput::FindPrefix(const std::string &music, int &currentIndex)
{
    int nextCurrentIndex = currentIndex;
    std::optional<GABCPrefixes> result = std::nullopt;
    // º = U+00BA = UTF-8 bytes 0xC2 0xBA: oblique ligature prefix
    if (static_cast<unsigned char>(GetCharAt(music, currentIndex)) == 0xC2
        && static_cast<unsigned char>(GetCharAt(music, currentIndex + 1)) == 0xBA) {
        currentIndex += 2;
        LogDebug("Prefix found: oblique ligature");
        return GABC_OBLIQUE_LIGATURE;
    }

    // Helper: append a token to m_pendingNcType using the space-separated MEI att.typed convention.
    auto appendPendingType = [this](const std::string &tag) {
        if (!m_pendingNcType.empty()) m_pendingNcType += " ";
        m_pendingNcType += tag;
    };

    switch (GetCharAt(music, currentIndex)) {
        case '!':
            // 18-may-2026 grule no_space (`!`). No dedicated MEI element; tag the next Nc so
            // engravers wanting GABC-faithful spacing can pick it up. Paper table mei1.
            nextCurrentIndex++;
            appendPendingType("no-space");
            result = GABC_NO_SPACE;
            break;
        case '@':
            // 18-may-2026 grule remove_first_stem (`@`). S-GABC §6.1 proposes removing this
            // rule from the grammar. We continue to parse it for backward compatibility with
            // existing GABC files, but (a) emit a deprecation warning to stderr/log and
            // (b) tag the next Nc with @type="remove-first-stem" so curated editions can
            // locate and migrate these occurrences automatically.
            nextCurrentIndex++;
            LogWarning("GABC '@' (remove_first_stem) prefix is deprecated per S-GABC §6.1; "
                       "the next Nc will be tagged @type=\"remove-first-stem\" for round-trip.");
            appendPendingType("remove-first-stem");
            result = GABC_REMOVE_FIRST_STEM;
            break;
        case '/':
            // 18-may-2026 grule neumatic_cut. We now preserve the *flavor* of the cut on the
            // next Nc's @type. MEI has no element for the cut itself in this Verovio build,
            // and the cut also acts as a neume separator (handled in ProcessNeume); the @type
            // captures the visual width hint so round-trip is non-lossy.
            //   `//`   → "neumatic-cut-double"   (wider neumatic spacing)
            //   `/0`   → "neumatic-cut-zero"     (no space — tight neumatic group)
            //   `/[n]` → "neumatic-cut-n<n>"     (size 1..6; negative form "n--<n>" for squashing)
            // A bare `/` (the standard neume separator) is *not* a cut and is handled outside
            // this function in ProcessNeume — it does not even enter this switch's `/` branch
            // because the inner lookahead simply leaves `result` unset.
            nextCurrentIndex++;
            char char2 = GetCharAt(music, nextCurrentIndex);
            if (char2 == '/') {
                appendPendingType("neumatic-cut-double");
                result = GABC_NEUMATIC_CUT;
            }
            else if (char2 == '0') {
                appendPendingType("neumatic-cut-zero");
                result = GABC_NEUMATIC_CUT;
            }
            else if (char2 == '[') {
                // Prefix: '[' '-'? [1-6] ']' (the closing ']' is accepted but tolerated if absent
                // — some GABC sources in the wild omit it). On match we advance past the digit so
                // the caller resumes after the size token; the optional ']' is also consumed when
                // present.
                nextCurrentIndex++; // past '['
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
                    nextCurrentIndex++; // past the digit
                    if (GetCharAt(music, nextCurrentIndex) == ']') {
                        nextCurrentIndex++; // past the closing ']'
                    }
                    std::string tag = "neumatic-cut-n";
                    if (value < 0) {
                        tag += "-";
                        tag += static_cast<char>('0' + (-value));
                    }
                    else {
                        tag += static_cast<char>('0' + value);
                    }
                    appendPendingType(tag);
                    result = GABC_NEUMATIC_CUT;
                }
            }
            break;
    }
    if (result.has_value()) {
        LogDebug("Prefix found");
        currentIndex = nextCurrentIndex;
    }
    else {
        LogDebug("No prefix found");
    }
    return result;
}

void GABCInput::AddAccidental(Syllable *syllable, data_ACCIDENTAL_WRITTEN accid, data_PITCHNAME pname, data_OCTAVE oct)
{
    LogDebug("Accidental found: %i", accid);
    Accid *accidElem = new Accid();
    accidElem->SetAccid(accid);
    accidElem->SetPloc(pname);
    accidElem->SetOloc(oct);
    syllable->AddChild(accidElem);
}

void GABCInput::AddEpisema(Nc *nc, episemaVis_FORM form, data_EVENTREL place)
{
    Episema *episema = new Episema();
    episema->SetForm(form);
    episema->SetPlace(place);
    nc->AddChild(episema);
}

void GABCInput::AddLiquescent(Nc *nc, curvatureDirection_CURVE curve)
{
    //const bool gabcNoTailsOption = m_doc->GetOptions()->m_liquescentWithoutTails.GetValue();
    Liquescent *liquescent = new Liquescent();
    nc->AddChild(liquescent);
    nc->SetCurve(curve);
}

int GABCInput::ProcessSuffix(const std::string &music, int currentIndex, Nc *nc, Nc *previousNC)
{
    int processedChars = 0;
    char nextChar = GetCharAt(music, currentIndex);
    char nextChar2 = '\0';
    switch (nextChar) {
        case 'V':
            // 17-may-2026 grule virga_left. Aquitanian sources use a north-east tilt instead of
            // the north tilt used for square notation (S-GABC paper, Table mei1, second-row note
            // "with tilt='ne' for Aquitanian"). The choice is exposed as a CLI option.
            processedChars++;
            nc->SetTilt(
                m_doc->GetOptions()->m_gabcAquitanianContext.GetValue() ? COMPASSDIRECTION_ne : COMPASSDIRECTION_n);
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
            this->AddLiquescent(nc, InferLiquescentCurve(previousNC, nc));
            break;

        case '>': // liquescent two tails down
            processedChars++;
            this->AddLiquescent(nc, curvatureDirection_CURVE_c);
            // Cephalicus only when there is no explicit virga following (nextChar2) or preceeding (nc->get...)
            nextChar2 = GetCharAt(music, currentIndex + processedChars);
            if (nextChar != 'v' && nextChar2 != 'V' && !nc->GetTilt()) {
                nc->SetType("cephalicus");
            }
            break;

        case '<': // liquescent two tails up
            processedChars++;
            this->AddLiquescent(nc, curvatureDirection_CURVE_a);
            nc->SetType("epiphonus");
            break;

        case '\'':
            processedChars++;
            this->AddEpisema(nc, episemaVis_FORM_v, EVENTREL_below);
            break;

        case '_':
            processedChars++;
            this->AddEpisema(nc, episemaVis_FORM_h, EVENTREL_above);
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
    // 18-may-2026 GABC divisio grammar (paper "Brief introduction to GABC", table mei1 row
    // "Divisions"). Four lexical variants exist; previously only `:` was handled and was emitted
    // as a generic CMN BarLine. The MEI neume module provides <divLine> with a `form` enum that
    // maps cleanly to the GABC distinctions:
    //   `,`   divisio minima   → DivLine @form="virgula"
    //   `;`   divisio minor    → DivLine @form="minima"
    //   `:`   divisio maior    → DivLine @form="maior"
    //   `::`  divisio finalis  → DivLine @form="finalis"
    // The double-colon must be tested before the single colon (longest-match). DivLine inherits
    // from LayerElement so it is accepted by Layer::IsSupportedChild. We no longer emit a CMN
    // BarLine for these — the neume module's element is the semantically correct choice.
    int processedChars = 0;
    char ch = GetCharAt(music, currentIndex);
    char next = GetCharAt(music, currentIndex + 1);

    std::optional<divLineLog_FORM> form;
    if (ch == ':' && next == ':') {
        form = divLineLog_FORM_finalis;
        processedChars = 2;
    }
    else if (ch == ':') {
        form = divLineLog_FORM_maior;
        processedChars = 1;
    }
    else if (ch == ';') {
        form = divLineLog_FORM_minima;
        processedChars = 1;
    }
    else if (ch == ',') {
        form = divLineLog_FORM_virgula;
        processedChars = 1;
    }

    if (form.has_value()) {
        LogDebug("Divisio found: %c (form=%d)", ch, static_cast<int>(form.value()));
        DivLine *divLine = new DivLine();
        divLine->SetForm(form.value());
        layer->AddChild(divLine);
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

void GABCInput::ProcessNeume(const std::string &music, Syllable *syllable)
{
    LogDebug("Processing neume");

    Neume *neume = NULL;
    Nc *previousNC = NULL;
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
                    // Skip the separator if the next element is another strophicus:
                    // either explicit 'pitch + s' form or bare 's' repetition form
                    bool nextIsStrophicus
                        = ((next >= 'a' && next <= 'm') && (GetCharAt(music, currentIndex + 2) == 's'))
                        || (next == 's');
                    if (nextIsStrophicus) {
                        currentIndex++;
                        continue;
                    }
                }
                // Finalize current neume and start a new one.
                // previousNC is kept for potential curve inference in the next neume.
                lastWasStrophicus = false;
                if (neume) syllable->AddChild(neume);
                neume = NULL;
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
            if (!neume) neume = new Neume();
            neume->AddChild(strophNC);
            previousNC = strophNC;
            currentIndex++;
            continue;
        }

        // Any other character ends the strophicus group
        lastWasStrophicus = false;

        // 17-may-2026 S-GABC proposed symbols (paper section "Missing Features and Proposal",
        // Table "Summary of Proposed Missing Symbols"). They are only honored when the
        // gabcExtendedSymbols option is enabled, otherwise they fall through to the
        // "unknown character" branch and are skipped.
        if (m_doc->GetOptions()->m_gabcExtendedSymbols.GetValue()) {
            if (ch == 'r') {
                // S-GABC proposal #3 — uncertain reading. Emit an empty Nc tagged @type="uncertain"
                // so the position in the neume is preserved but no pitch is committed.
                Nc *uncertainNc = new Nc();
                uncertainNc->SetType("uncertain");
                if (!neume) neume = new Neume();
                neume->AddChild(uncertainNc);
                previousNC = uncertainNc;
                currentIndex++;
                continue;
            }
            if (ch == '"') {
                // S-GABC proposal #1 — clarifying line. MEI has no neume-internal divider element
                // in this Verovio build, so we record it as @type="clarifying-line" on the *next*
                // Nc by re-using the strophicus-anchor strategy: tag the previous Nc and consume.
                if (previousNC) {
                    std::string current = previousNC->HasType() ? previousNC->GetType() : std::string();
                    if (!current.empty()) current += " ";
                    current += "clarifying-line";
                    previousNC->SetType(current);
                }
                currentIndex++;
                continue;
            }
        }

        std::optional<GABCPrefixes> prefixOpt = this->FindPrefix(music, currentIndex);

        ch = GetCharAt(music, currentIndex); // re-read after prefix may have consumed chars
        bool diamond = false;

        if (ch >= 'A' && ch <= 'M') {
            // Rhombus / inclinatum
            ch -= ('A' - 'a');
            diamond = true;
        }

        auto pitchOctOpt = FindPitch(ch, m_currentClefPitchOffset);

        if (pitchOctOpt.has_value()) {
            currentIndex++;
            auto [pname, oct] = pitchOctOpt.value();

            Nc *currentNC = new Nc();
            currentNC->SetPname(pname);
            currentNC->SetOct(oct);

            if (diamond) {
                currentNC->SetTilt(COMPASSDIRECTION_se);
            }

            if (prefixOpt.has_value() && prefixOpt.value() == GABC_OBLIQUE_LIGATURE) {
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

            // An accidental suffix (x/#/y) means the pitch letter only marks position —
            // no note is produced. The Nc is discarded; only the Accid goes to the syllable.
            char nextChar = GetCharAt(music, currentIndex);
            if (nextChar == 'x') {
                currentIndex++;
                AddAccidental(syllable, ACCIDENTAL_WRITTEN_f, pname, oct);
                delete currentNC;
                continue;
            }
            else if (nextChar == '#') {
                currentIndex++;
                AddAccidental(syllable, ACCIDENTAL_WRITTEN_s, pname, oct);
                delete currentNC;
                continue;
            }
            else if (nextChar == 'y') {
                currentIndex++;
                AddAccidental(syllable, ACCIDENTAL_WRITTEN_n, pname, oct);
                delete currentNC;
                continue;
            }

            // Strophicus: consume 's' before the general suffix loop.
            // Also handle the anchor form: pitch letter followed by '/s' (e.g. i/sss),
            // which is equivalent to is/ss — the pitch letter sets the strophicus pitch
            // but produces no note itself; the bare 's' handler emits all strophicus ncs.
            nextChar = GetCharAt(music, currentIndex);
            if (nextChar == 's') {
                currentIndex++;
                currentNC->AddChild(new Strophicus());
                lastWasStrophicus = true;
                strophicusPname = pname;
                strophicusOct = oct;
            }
            else if (nextChar == '/' && GetCharAt(music, currentIndex + 1) == 's') {
                lastWasStrophicus = true;
                strophicusPname = pname;
                strophicusOct = oct;
                delete currentNC;
                continue;
            }

            // A liquescent note always starts its own neume.
            // If the current neume already has ncs, finalize it first.
            // previousNC is kept so InferLiquescentCurve can use it across the neume boundary.
            nextChar = GetCharAt(music, currentIndex);
            if ((nextChar == '~' || nextChar == '>' || nextChar == '<') && neume) {
                syllable->AddChild(neume);
                neume = NULL;
            }

            if (!neume) neume = new Neume();
            neume->AddChild(currentNC);

            // 18-may-2026 drain any pending @type tokens accumulated by FindPrefix (cut size,
            // no-space, remove-first-stem) into this Nc and clear the accumulator. Placed AFTER
            // the accidental / `/s` discard branches above so the prefix info isn't lost on an
            // Nc that is about to be deleted (e.g. `!gx` would otherwise drop the no-space tag
            // when `gx` resolves to just an Accid). Done before the suffix loop so that
            // suffix-driven @type writes (episema-*, cephalicus, etc.) are appended after the
            // prefix tokens, preserving paper-order of attributes.
            if (!m_pendingNcType.empty()) {
                std::string current = currentNC->HasType() ? currentNC->GetType() : std::string();
                if (!current.empty()) current += " ";
                current += m_pendingNcType;
                currentNC->SetType(current);
                m_pendingNcType.clear();
            }

            // Process remaining suffixes attached to this note
            while (static_cast<std::size_t>(currentIndex) < music.size()) {
                int suffixChars = this->ProcessSuffix(music, currentIndex, currentNC, previousNC);
                if (suffixChars == 0) {
                    break;
                }
                currentIndex += suffixChars;
            }

            previousNC = currentNC;
        }
        else {
            int processedChars = this->ProcessBarline(music, currentIndex, m_layer);
            if (processedChars > 0) {
                currentIndex += processedChars;
            }
            else {
                LogDebug("Unknown neume character: %c", static_cast<char>(ch));
                currentIndex++;
            }
        }
    }

    if (neume) syllable->AddChild(neume);
}

void GABCInput::ProcessWord(const std::string &lyrics, const std::string &music, sylLog_WORDPOS wordpos, sylLog_CON con)
{
    Syllable *syllable = new Syllable();
    m_layer->AddChild(syllable);

    if (!lyrics.empty()) {
        LogDebug("Processing lyrics: %s", lyrics.c_str());
        Syl *syl = new Syl();
        if (wordpos != sylLog_WORDPOS_NONE) syl->SetWordpos(wordpos);
        if (con != sylLog_CON_NONE) syl->SetCon(con);
        syllable->AddChild(syl);
        Text *t = new Text();
        t->SetText(std::u32string(lyrics.begin(), lyrics.end()));
        syl->AddChild(t);
    }

    if (!music.empty()) {
        LogDebug("Processing music: %s", music.c_str());
        bool isClef = this->ProcessClef(music);
        if (!isClef) {
            bool isCustos = this->ProcessCustos(music);
            if (!isCustos) {
                this->ProcessNeume(music, syllable);
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

        LogDebug("Processing token: lyrics='%s' music='%s'", lyrics.c_str(), music.c_str());
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

        this->ProcessWord(tokens[ti].lyrics, tokens[ti].music, wordpos, con);
    }
}

bool GABCInput::Import(const std::string &gabc)
{
    m_currentClefPitchOffset = 0;
    m_pendingFlatOnClef = false;
    m_pendingNcType.clear(); // 18-may-2026 reset cross-token accumulator from any prior Import call
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
    m_layer = new Layer();
    m_layer->SetN(1);
    staff->AddChild(m_layer);
    measure->AddChild(staff);
    section->AddChild(measure);

    this->ProcessInput(gabc);

    // add minimal scoreDef
    StaffGrp *staffGrp = new StaffGrp();
    StaffDef *staffDef = new StaffDef();
    staffDef->SetNotationtype(NOTATIONTYPE_neume);
    staffDef->SetN(1);
    // 17-may-2026 The number of staff lines comes from the gabcStaffLines CLI option, which mirrors
    // the GABC `staff-lines:` header (paper Inconsistencies table, feature 2). The default is 4.
    staffDef->SetLines(m_doc->GetOptions()->m_gabcStaffLines.GetValue());

    // 17-may-2026 grule clef_flat — attach a one-accidental KeySig to the StaffDef when a `cb<n>`
    // clef was seen in the input. The flat is on `b` (B-flat), which is the only flat allowed in
    // GABC. See paper section "Brief introduction to GABC".
    if (m_pendingFlatOnClef) {
        KeySig *keySig = new KeySig();
        KeyAccid *keyAccid = new KeyAccid();
        keyAccid->SetAccid(ACCIDENTAL_WRITTEN_f);
        keyAccid->SetPname(PITCHNAME_b);
        keySig->AddChild(keyAccid);
        staffDef->AddChild(keySig);
    }

    staffGrp->AddChild(staffDef);
    m_doc->GetFirstScoreDef()->AddChild(staffGrp);

    m_doc->ConvertToPageBasedDoc();

    return true;
}

//----------------------------------------------------------------------------
// Static methods
//----------------------------------------------------------------------------

char GABCInput::GetCharAt(const std::string &input, int index)
{
    assert(index >= 0);

    auto idx = static_cast<std::size_t>(index);
    if (idx >= input.size()) {
        return '\0';
    }
    return input[idx];
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
    if (previousNC == NULL || currentNC == NULL) {
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
    // 17-may-2026 grule square_pitch extended past `m` with `n` and `p` (the letter `o` is reserved
    // for the oriscus suffix, see grule oriscus). The paper notes these characters are necessary
    // when a c5 clef is in use because `m` only reaches the space above the fifth line — see
    // Inconsistencies table, feature 4.
    if (ch < 'a' || ch > 'p' || ch == 'o') {
        return std::nullopt;
    }

    // Base c2 mapping: a..m = e3..c5.
    constexpr int c2BaseDiatonicIndexForA = 3 * 7 + 2; // E3
    int letterIndex = ch - 'a';
    if (ch == 'p') letterIndex = 14; // collapse the gap left by skipping `o`
    int absoluteDiatonicIndex = c2BaseDiatonicIndexForA + letterIndex + clefPitchOffset;

    return MakePitchFromDiatonicIndex(absoluteDiatonicIndex);
}

} // namespace vrv

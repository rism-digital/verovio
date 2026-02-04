/////////////////////////////////////////////////////////////////////////////
// Name:        customtuning.cpp
// Author:      Karim Ratib
// Created:     2025
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "customtuning.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "iomusxml.h"
#include "note.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// CustomTuning
//----------------------------------------------------------------------------

CustomTuning::CustomTuning(const std::string &tuningDef, Doc *doc, bool useMusicXmlAccidentals)
{
    assert(doc);
    m_doc = doc;
    try {
        m_tuning = Tunings::Tuning(Tunings::parseASCLData(tuningDef));
        CreateNoteMapping(useMusicXmlAccidentals);
    }
    catch (Tunings::TuningError &e) {
        LogError("Custom tuning: Invalid tuning definition: %s", e.what());
    }
}

/**
 * Map the tuning note names to MEI notes.
 *
 * - Convert accidentals from MusicXML or MEI to SMuFL
 * - Detect enharmonics separated by `/`
 * - Detect multiple accidentals separated by `+`
 * - Ignore natural accidental
 */
void CustomTuning::CreateNoteMapping(bool useMusicXmlAccidentals)
{
    assert(m_doc);

    m_noteMap.clear();
    for (const auto &note : m_tuning.notationMapping.names) {
        std::smatch note_names;
        std::regex note_name_regex("(?:^|\\/)([A-G])([^\\/\\s]*)");
        std::string::const_iterator search_start(note.cbegin());
        while (std::regex_search(search_start, note.cend(), note_names, note_name_regex)) {
            std::string mei = note_names[1].str();
            std::smatch accid_names;
            std::regex accid_name_regex("([^\\+]+)\\+?");
            std::string accids = note_names[2].str();
            std::string::const_iterator accid_start(accids.cbegin());
            while (std::regex_search(accid_start, accids.cend(), accid_names, accid_name_regex)) {
                std::string accid = accid_names[1].str();
                std::string glyphName;
                char32_t glyph = 0;
                if ((glyph = m_doc->GetResources().GetGlyphCode(accid))) {
                    glyphName = accid;
                }
                else if ((glyph = GetAccidGlyph(accid, useMusicXmlAccidentals))) {
                    glyphName = GetGlyphName(glyph);
                }
                if (!glyph) {
                    LogError("Custom tuning: Tuning accidental \"%s\" is neither a %s accidental nor a SMuFL glyph",
                        accid.c_str(), useMusicXmlAccidentals ? "MusicXML" : "MEI");
                }
                else if (glyph != SMUFL_E261_accidentalNatural && !glyphName.empty()) {
                    if (accid_start != accids.cbegin()) mei += "+";
                    mei += glyphName;
                }
                accid_start = accid_names.suffix().first;
            }
            m_noteMap.insert({ mei, note });

            // get next match
            search_start = note_names.suffix().first;
        }
    }
}

/**
 * Convert an accidental (MEI or MusicXML) to a SMuFL glyph.
 */
char32_t CustomTuning::GetAccidGlyph(std::string accid, bool useMusicXmlAccidentals) const
{
    if (useMusicXmlAccidentals) {
        data_ACCIDENTAL_WRITTEN accidental = MusicXmlInput::ConvertAccidentalToAccid(accid);
        return Accid::GetAccidGlyph(accidental);
    }

    InstAccidental accidental;
    return Accid::GetAccidGlyph(accidental.StrToAccidentalWritten(accid));
}

/**
 * Get a SMuFL glyph name.
 */
std::string CustomTuning::GetGlyphName(char32_t smufl) const
{
    assert(m_doc);
    const Glyph *glyph = m_doc->GetResources().GetGlyph(smufl);
    if (glyph) {
        return glyph->GetName();
    }
    LogError("Custom tuning: SMuFL glyph U+%04X not found in glyph table", smufl);
    return "";
}

/**
 * Retrieve MIDI pitch for a note in the custom tuning.
 *
 * Uses the constructed note map to look up the tuning note names.
 * Falls back to the standard note MIDI pitch calculation if lookup fails.
 */
int CustomTuning::GetMIDIPitch(const Note *note, const int shift, const int octaveShift) const
{
    // construct note name for tuning lookup
    data_PITCHNAME pname = note->GetPname();
    if (note->HasPnameGes()) pname = note->GetPnameGes();
    std::string noteName(1, (pname - 1 + ('C' - 'A')) % 7 + 'A');

    int accs = 0;
    for (const Object *object : note->FindAllDescendantsByType(ACCID)) {
        const Accid *accid = vrv_cast<const Accid *>(object);
        const AttAccidental *att = dynamic_cast<const AttAccidental *>(accid);
        assert(att);

        char32_t glyph = 0;
        std::string glyphName;
        if (accid->HasGlyphName()) {
            glyph = m_doc->GetResources().GetGlyphCode(accid->GetGlyphName());
        }
        else if (accid->HasAccid()) {
            glyph = GetAccidGlyph(att->AccidentalWrittenToStr(accid->GetAccid()), false);
        }
        else if (accid->HasAccidGes()) {
            glyph = GetAccidGlyph(att->AccidentalGesturalToStr(accid->GetAccidGes()), false);
        }
        if (glyph) {
            glyphName = GetGlyphName(glyph);
        }
        if (glyph != SMUFL_E261_accidentalNatural && !glyphName.empty()) {
            if (accs++) noteName += "+";
            noteName += glyphName;
        }
    }
    int oct = note->GetOct() + octaveShift;
    if (note->HasOctGes()) oct = note->GetOctGes();

    // lookup note in the tuning and map it to a MIDI key
    try {
        // FIXME! Special case: when we encounter a B pitch that's in scale degree 0, increment its oct by 1
        // otherwise, it would be in the lower octave
        const auto it = std::find(
            m_tuning.notationMapping.names.begin(), m_tuning.notationMapping.names.end(), m_noteMap.at(noteName));
        int scalePosition = (it - m_tuning.notationMapping.names.begin() + 1) % m_tuning.notationMapping.count;
        if (pname == PITCHNAME_b && scalePosition == 0) oct++;
        return m_tuning.midiNoteForNoteName(m_noteMap.at(noteName), oct);
    }
    catch (Tunings::TuningError &e) {
        LogError("Custom tuning: Error mapping note to tuning: %s", e.what());
    }
    catch (std::out_of_range &e) {
        LogError("Custom tuning: Error mapping note to tuning: %s not mapped", noteName.c_str());
    }
    return note->GetMIDIPitch(shift, octaveShift);
}

} // namespace vrv

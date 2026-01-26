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
    try {
        m_tuning = Tunings::Tuning(Tunings::parseASCLData(tuningDef));
        CreateNoteMapping(doc, useMusicXmlAccidentals);
    }
    catch (Tunings::TuningError &e) {
        LogError("Invalid tuning definition: %s", e.what());
    }
}

/**
 * Map the tuning note names to MEI notes.
 *
 * - Convert tuning accidentals from MusicXML or MEI and keep SMuFL accidentals as-is
 * - Detect enharmonics separated by `/`
 * - Detect multiple accidentals separated by `+`
 */
void CustomTuning::CreateNoteMapping(Doc *doc, bool useMusicXmlAccidentals)
{
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
                bool valid = false;
                if (doc->GetResources().GetGlyphCode(accid)) {
                    if (accid_start != accids.cbegin()) mei += "+";
                    mei += accid;
                    valid = true;
                }
                else {
                    InstAccidental accidental;
                    accidental.SetAccid(useMusicXmlAccidentals ? MusicXmlInput::ConvertAccidentalToAccid(accid)
                                                               : accidental.StrToAccidentalWritten(accid));
                    if (accidental.HasAccid()) {
                        valid = true;
                        if (accidental.GetAccid() != ACCIDENTAL_WRITTEN_n) {
                            if (accid_start != accids.cbegin()) mei += "+";
                            mei += accidental.AccidentalWrittenToStr(accidental.GetAccid());
                        }
                    }
                }
                if (!valid) {
                    LogError("Tuning accidental \"%s\" is neither a %s accidental nor a SMuFL glyph", accid.c_str(),
                        useMusicXmlAccidentals ? "MusicXML" : "MEI");
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
        if (accid->GetAccid() == ACCIDENTAL_WRITTEN_n || accid->GetAccidGes() == ACCIDENTAL_GESTURAL_n) {
            if (accid->HasGlyphName()) {
                if (accs++) noteName += "+";
                noteName += accid->GetGlyphName();
            }
        }
        else if (accid->HasAccid()) {
            if (accs++) noteName += "+";
            noteName += att->AccidentalWrittenToStr(accid->GetAccid());
        }
        else if (accid->HasAccidGes()) {
            if (accs++) noteName += "+";
            noteName += att->AccidentalGesturalToStr(accid->GetAccidGes());
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
        LogError("Error mapping note to tuning: %s", e.what());
    }
    catch (std::out_of_range &e) {
        LogError("Error mapping note to tuning: %s not mapped.", noteName.c_str());
    }
    return note->GetMIDIPitch(shift, octaveShift);
}

} // namespace vrv

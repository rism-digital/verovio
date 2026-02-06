/////////////////////////////////////////////////////////////////////////////
// Name:        customtuning.h
// Author:      Karim Ratib
// Created:     2025
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CUSTOMTUNING_H__
#define __VRV_CUSTOMTUNING_H__

#include "Tunings.h"

//----------------------------------------------------------------------------

#include <map>

//----------------------------------------------------------------------------

namespace vrv {

class Doc;
class Note;

//----------------------------------------------------------------------------
// Typedefs
//----------------------------------------------------------------------------

typedef std::map<std::string, std::string> TuningNoteMap;

//----------------------------------------------------------------------------
// CustomTuning
//----------------------------------------------------------------------------

class CustomTuning {
public:
    CustomTuning() {}
    CustomTuning(const std::string &tuningDef, Doc *doc, bool useMusicXmlAccidentals);

    /**
     * @name Getters and validity checkers
     */
    ///@{
    bool IsValid() const { return m_tuning.notationMapping.count > 0; }
    Tunings::Tuning &GetTuning() { return m_tuning; }
    const Tunings::Tuning &GetTuning() const { return m_tuning; }
    TuningNoteMap &GetNoteMap() { return m_noteMap; }
    const TuningNoteMap &GetNoteMap() const { return m_noteMap; }
    ///@}

    /**
     * @name Get MIDI note name for given MEI note
     */
    ///@{
    int GetMIDIPitch(const Note *note, const int shift, const int octaveShift) const;
    ///@}

private:
    /**
     * @name Helpers to create various needed mappings.
     */
    ///@{
    void CreateGlyphMapping(Doc *doc);
    void CreateNoteMapping(bool useMusicXmlAccidentals);
    ///@}

    /*
     * @name Helper functions for SMuFL glyphs
     */
    ///@{
    char32_t GetGlyphCode(const std::string &glyphName) const;
    std::string GetGlyphName(char32_t glyphCode) const;
    char32_t GetAccidGlyph(const std::string &accid, bool useMusicXmlAccidentals) const;
    ///@}

public:
    //

private:
    Tunings::Tuning m_tuning;
    TuningNoteMap m_noteMap;
    std::map<std::string, char32_t> m_glyphNames;
    std::map<char32_t, std::string> m_glyphCodes;
};

} // namespace vrv

#endif

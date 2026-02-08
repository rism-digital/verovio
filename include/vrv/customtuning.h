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
    std::map<std::string, std::string> &GetNoteMap() { return m_noteMap; }
    const std::map<std::string, std::string> &GetNoteMap() const { return m_noteMap; }
    ///@}

    /**
     * @name Get MIDI note name for given MEI note
     */
    ///@{
    int GetMIDIPitch(const Note *note, const int shift, const int octaveShift) const;
    ///@}

    /*
     * @name Helper functions for SMuFL glyphs
     */
    ///@{
    static char32_t GetGlyphCode(const std::string &glyphName, Doc *doc = NULL);
    static std::string GetGlyphName(char32_t glyphCode, Doc *doc = NULL);
    static char32_t GetAccidGlyph(const std::string &accid, bool useMusicXmlAccidentals);
    ///@}

private:
    /**
     * @name Helpers to create various needed mappings.
     */
    ///@{
    static void CreateGlyphMapping(Doc *doc);
    void CreateNoteMapping(bool useMusicXmlAccidentals);
    ///@}

public:
    //

private:
    Tunings::Tuning m_tuning;
    std::map<std::string, std::string> m_noteMap;

    //----------------//
    // Static members //
    //----------------//

    static std::map<std::string, char32_t> s_glyphNames;
    static std::map<char32_t, std::string> s_glyphCodes;
};

} // namespace vrv

#endif

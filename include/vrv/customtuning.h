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
     * Create the note mapping.
     */
    void CreateNoteMapping(bool useMusicXmlAccidentals);

    /*
     * @name Helper functions for SMuFL glyphs
     */
    ///@{
    char32_t GetAccidGlyph(std::string accid, bool useMusicXmlAccidentals) const;
    std::string GetGlyphName(char32_t accidental) const;
    ///@}

public:
    //

private:
    const Doc *m_doc;
    Tunings::Tuning m_tuning;
    TuningNoteMap m_noteMap;
};

} // namespace vrv

#endif

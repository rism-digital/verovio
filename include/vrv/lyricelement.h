/////////////////////////////////////////////////////////////////////////////
// Name:        lyricelement.h
// Author:      Simon Waloschek
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_LYRICELEMENT_H__
#define __VRV_LYRICELEMENT_H__

#include "atts_shared.h"
#include "layerelement.h"
#include "offsetinterface.h"

#include <algorithm>
#include <utility>

namespace vrv {

class Syl;
class Volta;

//----------------------------------------------------------------------------
// LyricElement
//----------------------------------------------------------------------------

/** Shared implementation for note-attached verse and refrain lyrics. */
class LyricElement : public LayerElement,
                     public OffsetInterface,
                     public AttColor,
                     public AttLang,
                     public AttPlacementRelStaff,
                     public AttTypography,
                     public AttVoltaGroupingSym {
public:
    virtual ~LyricElement();
    void Reset() override;

    /** Add sung text or an editorial wrapper to a lyric container. */
    bool IsSupportedChild(ClassId classId) override;

    OffsetInterface *GetOffsetInterface() override { return vrv_cast<OffsetInterface *>(this); }
    const OffsetInterface *GetOffsetInterface() const override { return vrv_cast<const OffsetInterface *>(this); }

    /** Return the number of alternative lyric rows represented by volta children. */
    int GetVoltaCount() const;
    /** Return the lowest and highest document-wide drawing tracks used by this container's volta children. */
    std::pair<int, int> GetVoltaDrawingRange() const;
    /** Return whether the container also has syllables outside volta containers. */
    bool HasDirectSyl() const;
    /** Return whether the document-wide lyric group has a direct-syllable track. */
    bool HasDrawingDirectSylTrack() const { return m_drawingDirectSylTrack || this->HasDirectSyl(); }
    void SetDrawingDirectSylTrack() const { m_drawingDirectSylTrack = true; }
    void ResetDrawingDirectSylTrack() { m_drawingDirectSylTrack = false; }
    /** Return the total number of lyric rows, including a separate direct-syllable row. */
    int GetLyricLineCount() const;
    /** Return the one-based lyric row occupied by a volta alternative. */
    int GetVoltaLineN(const Volta *volta) const;

    /** Drawing slot used for vertical lyric alignment. */
    int GetDrawingVerseN() const { return m_drawingVerseN; }
    void SetDrawingVerseN(int verseN) const { m_drawingVerseN = std::max(verseN, 1); }
    /** Stable processing group used to isolate connectors and horizontal spacing. */
    int GetDrawingLyricGroupN() const { return m_drawingLyricGroupN; }
    void SetDrawingLyricGroupN(int groupN) const { m_drawingLyricGroupN = std::max(groupN, 1); }
    void ResetDrawingLyricGroup()
    {
        m_drawingVerseN = 1;
        m_drawingLyricGroupN = 1;
        m_drawingDirectSylTrack = false;
    }

    /** Calculate and apply a horizontal lyric-overlap adjustment. */
    int AdjustPosition(int &overlap, int freeSpace, const Doc *doc);

protected:
    explicit LyricElement(ClassId classId);

private:
    mutable int m_drawingVerseN;
    mutable int m_drawingLyricGroupN;
    mutable bool m_drawingDirectSylTrack;
};

} // namespace vrv

#endif

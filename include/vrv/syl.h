/////////////////////////////////////////////////////////////////////////////
// Name:        syl.h
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SYL_H__
#define __VRV_SYL_H__

#include "atts_shared.h"
#include "facsimileinterface.h"
#include "layerelement.h"
#include "timeinterface.h"

namespace vrv {

class AdjustSylSpacingParams;
class Note;
class TextElement;

//----------------------------------------------------------------------------
// Syl
//----------------------------------------------------------------------------

/**
 * Syl is a TimeSpanningInterface for managing syllable connectors. This means
 * that TimeSpanningInterface attributes will not be read from the MEI but
 * pointers will be populated in Object::PrepareLyrics and Object::PrepareLyricsEnd
 */

class Syl : public LayerElement,
            public TextListInterface,
            public TimeSpanningInterface,
            public AttLang,
            public AttTypography,
            public AttSylLog {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Syl();
    virtual ~Syl();
    Object *Clone() const override { return new Syl(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Syl"; }
    ///@}

    /** Override the method since it is align to the staff */
    bool IsRelativeToStaff() const override { return true; }

    /**
     * @name Getter to interfaces
     */
    ///@{
    TimePointInterface *GetTimePointInterface() override { return vrv_cast<TimePointInterface *>(this); }
    const TimePointInterface *GetTimePointInterface() const override
    {
        return vrv_cast<const TimePointInterface *>(this);
    }
    TimeSpanningInterface *GetTimeSpanningInterface() override { return vrv_cast<TimeSpanningInterface *>(this); }
    const TimeSpanningInterface *GetTimeSpanningInterface() const override
    {
        return vrv_cast<const TimeSpanningInterface *>(this);
    }
    ///@}

    /**
     * Add an element (text, rend. etc.) to a syl.
     * Only supported elements will be actually added to the child list.
     */
    bool IsSupportedChild(Object *object) override;

    /**
     * Calculate the spacing needed depending on the @worpos and @con
     */
    int CalcConnectorSpacing(Doc *doc, int staffSize);

    int GetDrawingWidth() const;
    int GetDrawingHeight() const;

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::PrepareLyrics
     */
    int PrepareLyrics(FunctorParams *functorParams) override;

    /**
     * See Object::PrepareStaffCurrentTimeSpanning
     */
    int PrepareStaffCurrentTimeSpanning(FunctorParams *functorParams) override;

    /**
     * See Object::GenerateMIDI
     */
    int GenerateMIDI(FunctorParams *functorParams) override;

    /**
     * See Object::ResetData
     */
    int ResetData(FunctorParams *functorParams) override;

    /** Create a default zone for a syl based on syllable. */
    bool CreateDefaultZone(Doc *doc);

private:
    //
public:
    /**
     * The verse number with multiple verses
     * Value is 1 by default, set in PrepareLyrics
     */
    int m_drawingVerse;

    /**
     * A pointer to the next syllable of the word.
     * It is not set when the end of the lyric is not another syl but a note for extenders
     */
    Syl *m_nextWordSyl;

private:
};

} // namespace vrv

#endif

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

class Note;
class TextElement;

//----------------------------------------------------------------------------
// Syl
//----------------------------------------------------------------------------

/**
 * Syl is a TimeSpanningInterface for managing syllable connectors. This means
 * that TimeSpanningInterface attributes will not be read from the MEI but
 * pointers will be populated in the PrepareLyricsFunctor.
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
    std::string GetClassName() const override { return "syl"; }
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
    bool IsSupportedChild(ClassId classId) override;

    /**
     * Calculate the hyphen length using the text font
     */
    int CalcHyphenLength(Doc *doc, int staffSize);

    /**
     * Calculate the spacing needed depending on the @worpos and @con
     */
    int CalcConnectorSpacing(Doc *doc, int staffSize);

    int GetDrawingWidth() const;
    int GetDrawingHeight() const;

    //----------------//
    // Static methods //
    //----------------//

    /**
     * Adjust proportionally to the lyric size
     */
    static void AdjustToLyricSize(const Doc *doc, int &value);

    //----------//
    // Functors //
    //----------//

    /**
     * Interface for class functor visitation
     */
    ///@{
    FunctorCode Accept(Functor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(Functor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
    ///@}

    /** Create a default zone for a syl based on syllable. */
    bool CreateDefaultZone(Doc *doc);

private:
    //
public:
    /**
     * The verse number with multiple verses
     * Value is 1 by default, set in PrepareLyrics
     */
    int m_drawingVerseN;
    /** The verse place (below by default) */
    data_STAFFREL m_drawingVersePlace;

    /**
     * A pointer to the next syllable of the word.
     * It is not set when the end of the lyric is not another syl but a note for extenders
     */
    Syl *m_nextWordSyl;

private:
};

} // namespace vrv

#endif

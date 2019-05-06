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
            public FacsimileInterface,
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
    virtual void Reset();
    virtual std::string GetClassName() const { return "Syl"; }
    virtual ClassId GetClassId() const { return SYL; }
    ///@}

    /** Override the method since it is align to the staff */
    virtual bool IsRelativeToStaff() const { return true; }

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual FacsimileInterface *GetFacsimileInterface() { return dynamic_cast<FacsimileInterface *>(this); }
    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    virtual TimeSpanningInterface *GetTimeSpanningInterface() { return dynamic_cast<TimeSpanningInterface *>(this); }
    ///@}

    /**
     * Add an element (text, rend. etc.) to a syl.
     * Only supported elements will be actually added to the child list.
     */
    virtual void AddChild(Object *object);

    /**
     * Calculate the adjustment according to the overlap and the free space available before.
     * Will move the syllable accordingly.
     * Called from Syl::AdjustSylSpacing and System::AdjustSylSpacingEnd
     */
    int CalcHorizontalAdjustment(int &overlap, AdjustSylSpacingParams *params);

    virtual int GetDrawingX() const;
    virtual int GetDrawingY() const;

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::PrepareLyrics
     */
    virtual int PrepareLyrics(FunctorParams *functorParams);

    /**
     * See Object::FillStaffCurrentTimeSpanning
     */
    virtual int FillStaffCurrentTimeSpanning(FunctorParams *functorParams);

    /**
     * See Object::AdjustSylSpacing
     */
    virtual int AdjustSylSpacing(FunctorParams *functorParams);

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

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

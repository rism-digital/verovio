/////////////////////////////////////////////////////////////////////////////
// Name:        syl.h
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SYL_H__
#define __VRV_SYL_H__

#include "atts_shared.h"
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
 * pointers will be populated in Object::PrepareLyrics and Object::PrepareLyricsEnd
 */

class Syl : public LayerElement,
            public TextListInterface,
            public TimeSpanningInterface,
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
    virtual std::string GetClassName() { return "Syl"; };
    virtual ClassId Is() { return SYL; };
    ///@}

    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    virtual TimeSpanningInterface *GetTimeSpanningInterface() { return dynamic_cast<TimeSpanningInterface *>(this); }

    /**
     * Add an element (text, rend. etc.) to a syl.
     * Only supported elements will be actually added to the child list.
     */
    void AddTextElement(TextElement *element);

    //----------//
    // Functors //
    //----------//

    /**
     * Functor for setting wordpos and connector ends
     * The functor is processed by staff/layer/verse using an ArrayOfAttComparisons filter
     * See PrepareDarwing
     */
    virtual int PrepareLyrics(ArrayPtrVoid *params);

    /**
     * See Object::FillStaffCurrentTimeSpanning
     */
    virtual int FillStaffCurrentTimeSpanning(ArrayPtrVoid *params);

    /**
     * Reset the drawing values before calling PrepareDrawing after changes.
     */
    virtual int ResetDrawing(ArrayPtrVoid *params);

private:
    //
public:
    /**
     * The verse number with multiple verses
     * Value is 1 by default, set in PrepareLyrics
     */
    int m_drawingVerse;

private:
};

} // namespace vrv

#endif

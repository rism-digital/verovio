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
    virtual std::string GetClassName() const { return "Syl"; }
    virtual ClassId Is() const { return SYL; }
    ///@}

    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    virtual TimeSpanningInterface *GetTimeSpanningInterface() { return dynamic_cast<TimeSpanningInterface *>(this); }

    /**
     * Add an element (text, rend. etc.) to a syl.
     * Only supported elements will be actually added to the child list.
     */
    virtual void AddChild(Object *object);

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

private:
};

} // namespace vrv

#endif

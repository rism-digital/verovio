/////////////////////////////////////////////////////////////////////////////
// Name:        verse.h
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_VERSE_H__
#define __VRV_VERSE_H__

#include "atts_shared.h"
#include "layerelement.h"

namespace vrv {

class LabelAbbr;
class Syl;

//----------------------------------------------------------------------------
// Verse
//----------------------------------------------------------------------------

class Verse : public LayerElement, public AttColor, public AttLang, public AttNInteger, public AttTypography {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Verse();
    virtual ~Verse();
    virtual Object *Clone() const { return new Verse(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Verse"; }
    virtual ClassId GetClassId() const { return VERSE; }
    ///@}

    /**
     * Add an element (a syl) to a verse.
     * Only Syl elements will be actually added to the verse.
     */
    virtual bool IsSupportedChild(Object *object);

    /**
     * Calculate the adjustment according to the overlap and the free space available before.
     * Will move the verse accordingly.
     * Called from Verse::AdjustSylSpacing and System::AdjustSylSpacingEnd
     */
    int AdjustPosition(int &overlap, int freeSpace, Doc *doc);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::AlignVertically
     */
    virtual int AlignVertically(FunctorParams *functorParams);

    /**
     * See Object::AdjustSylSpacing
     */
    virtual int AdjustSylSpacing(FunctorParams *functorParams);

    /**
     * See Object::PrepareProcessingLists
     */
    virtual int PrepareProcessingLists(FunctorParams *functorParams);

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

private:
    //
public:
    /**
     *  A pointer to the labelAbbr
     */
    LabelAbbr *m_drawingLabelAbbr;

private:
};

} // namespace vrv

#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        barline.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_BARLINE_H__
#define __VRV_BARLINE_H__

#include "atts_shared.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// BarLine
//----------------------------------------------------------------------------

/**
 * This class models the MEI <barLine> element.
 */
class BarLine : public LayerElement,
                public AttBarLineLog,
                public AttColor,
                public AttNNumberLike,
                public AttVisibility {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes.
     */
    ///@{
    BarLine();
    virtual ~BarLine();
    virtual Object *Clone() const { return new BarLine(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "BarLine"; }
    virtual ClassId GetClassId() const { return BARLINE; }
    ///@}

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

    /**
     * Use to set the alignment for the Measure BarLine members.
     * This is as special case where we need to add to the measure aligner.
     */
    bool SetAlignment(Alignment *alignment);

    /*
     * Return true if the barLine type requires repetition dots to be drawn.
     */
    bool HasRepetitionDots() const;

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ConvertToCastOffMensural
     */
    virtual int ConvertToCastOffMensural(FunctorParams *params);

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// BarLineAttr
//----------------------------------------------------------------------------

/**
 * This class models the barLine related attributes of a MEI measure.
 */
class BarLineAttr : public BarLine {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * No Reset() method is required.
     */
    ///@{
    BarLineAttr();
    virtual ~BarLineAttr();
    virtual Object *Clone() const { return new BarLineAttr(*this); }
    virtual std::string GetClassName() const { return "BarLineAttr"; }
    virtual ClassId GetClassId() const { return (m_isLeft ? BARLINE_ATTR_LEFT : BARLINE_ATTR_RIGHT); }
    ///@}

    void SetLeft() { m_isLeft = true; }

private:
    //
public:
    //
private:
    /** A flag for left barlines (right if false) */
    bool m_isLeft;
};

} // namespace vrv

#endif

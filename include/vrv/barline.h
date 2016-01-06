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
class BarLine: public LayerElement,
    public AttBarLineLog
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes.
     */
    ///@{
    BarLine();
    virtual ~BarLine();
    virtual void Reset();
    virtual Object* Clone() { return new BarLine(*this); };
    virtual std::string GetClassName() { return "BarLine"; };
    virtual ClassId Is() { return BARLINE; };
    ///@}
    
    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() { return true; };
    
    /**
     * Use to set the alignment for the Measure BarLine members.
     * This is as special case where we need to add to the measure aligner.
     */
    void SetAlignment(Alignment *alignment) { m_alignment = alignment; };
    
    /*
     * Return true if the barLine type requires repetition dots to be drawn.
     */
    bool HasRepetitionDots();
    
private:
    
public:

private:
    
};
    
//----------------------------------------------------------------------------
// BarLineAttr
//----------------------------------------------------------------------------

/**
 * This class models the barLine related attributes of a MEI measure.
 */
class BarLineAttr: public BarLine
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes.
     * No Reset() method required.
     */
    ///@{
    BarLineAttr();
    virtual ~BarLineAttr();
    virtual Object* Clone() { return new BarLineAttr(*this); };
    virtual std::string GetClassName() { return "BarLineAttr"; };
    virtual ClassId Is() { return BARLINE_ATTR; };
    ///@}
};
    
} // namespace vrv

#endif

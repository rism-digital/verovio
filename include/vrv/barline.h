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
// Barline
//----------------------------------------------------------------------------

/** 
 * This class models the MEI <barLine> element. 
 */
class Barline: public LayerElement,
    public AttBarLineLog
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes.
     */
    ///@{
    Barline();
    virtual ~Barline();
    virtual void Reset();
    virtual Object* Clone() { return new Barline(*this); };
    virtual std::string GetClassName( ) { return "Barline"; };
    virtual ClassId Is() { return BARLINE; };
    ///@}
    
    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() { return true; };
    
    /**
     * Use to set the alignment for the Measure Barline members.
     * This is as special case where we need to add to the measure aligner.
     */
    void SetAlignment( Alignment *alignment ) { m_alignment = alignment; };
    
    /*
     * Return true if the barLine type requires repetition dots to be drawn.
     */
    bool HasRepetitionDots( );
    
private:
    
public:

private:
    
};
    
//----------------------------------------------------------------------------
// BarlineAttr
//----------------------------------------------------------------------------

/**
 * This class models the barLine related attributes of a MEI measure.
 */
class BarlineAttr: public Barline
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * No Reset() method is required.
     */
    ///@{
    BarlineAttr();
    virtual ~BarlineAttr();
    virtual Object* Clone() { return new BarlineAttr(*this); };
    virtual std::string GetClassName( ) { return "BarlineAttr"; };
    virtual ClassId Is() { return BARLINE_ATTR; };
    ///@}
};
    
} // namespace vrv

#endif

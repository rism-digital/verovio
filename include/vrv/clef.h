/////////////////////////////////////////////////////////////////////////////
// Name:        clef.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_CLEF_H__
#define __VRV_CLEF_H__

#include "atts_shared.h"
#include "layerelement.h"

namespace vrv {
    
class ClefAttr;
    
//----------------------------------------------------------------------------
// Clef
//----------------------------------------------------------------------------

/** 
 * This class models the MEI <clef> element. 
 */
class Clef: public LayerElement,
    public AttClefshape,
    public AttLineloc,
    public AttOctavedisplacement
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes.
     */
    ///@{
    Clef();
    Clef( ClefAttr *clefAttr );
    virtual ~Clef();
    virtual void Reset();
    virtual Object* Clone() { return new Clef(*this); };
    virtual std::string GetClassName( ) { return "Clef"; };
    ///@}
    
    /**
     * Comparison operator. 
     * Check if the LayerElement if a Clef and compare attributes
     */
    virtual bool operator==(Object& other);
    
    /**
     * Return the offset of the clef
     */
    int GetClefOffset();
    
    /**
     * This is used for false clef offset calculation.
     * Returns 4 bytes with, from right to left, line, shape, dis and displace
     */
    int GetClefId();
    
private:
    
public:

private:
    
};
    
//----------------------------------------------------------------------------
// ClefAttr
//----------------------------------------------------------------------------

/**
 * This class models the MEI @clef attributes in scoreDef or staffDef elements.
 */
class ClefAttr: public Object,
    public AttCleffingLog
{
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    ClefAttr();
    virtual ~ClefAttr();
    virtual void Reset();
    virtual std::string GetClassName( ) { return "ClefAttr"; };
    virtual Object* Clone() { return new ClefAttr(*this); };
    ///@}
    
    /**
     * Comparison operator.
     * Check if the LayerElement if a ClefAttr and compare attributes
     */
    virtual bool operator==(Object& other);
    
private:
    
public:
    
private:
    
};

} // namespace vrv

#endif

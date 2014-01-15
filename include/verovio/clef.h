/////////////////////////////////////////////////////////////////////////////
// Name:        clef.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_CLEF_H__
#define __VRV_CLEF_H__

#include "vrvdef.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Clef
//----------------------------------------------------------------------------

/** 
 * This class models the MEI <clef> element. 
 */
class Clef: public LayerElement
{
public:
    // constructors and destructors
    Clef();
    virtual ~Clef();
    
    /**
     * Comparison operator. 
     * Check if the LayerElement if a Clef and compare attributes
     */
    virtual bool operator==(Object& other);
    
    virtual std::string GetClassName( ) { return "Clef"; };
    
    /**
     * Return the offset of the clef
     */
    int GetClefOffset();
    
    /**
     * Set the value for the clef.
     */
	virtual void SetValue( int value, int flag = 0 );
    
private:
    
public:
    /** Indicates the shape and the line of the clef using clef ids  */
    ClefId m_clefId;

private:
    
};

} // namespace vrv

#endif

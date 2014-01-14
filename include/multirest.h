//
//  musmultirest.h
//  aruspix
//
//  Created by Rodolfo Zitellini on 02/12/13.
//  Copyright (c) 2013 com.aruspix.www. All rights reserved.
//

#ifndef __MUS_MULTIREST_H__
#define __MUS_MULTIREST_H__

#include "vrvdef.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// MultiRest
//----------------------------------------------------------------------------

/** 
 * This class models the MEI <musmultirest> element. 
 */
class MultiRest: public LayerElement
{
public:
    // constructors and destructors
    MultiRest();
    MultiRest(int number);
    virtual ~MultiRest();
    
    /**
     * Comparison operator. 
     * Check if the LayerElement if a Clef and compare attributes
     */
    virtual bool operator==(Object& other);
    
    virtual std::string MusClassName( ) { return "MultiRest"; };
    
    /**
     * Get the multi measure rest number
     */
    int GetNumber() {return m_number;};
    
    /**
     * Set the multi measure rest number.
     */
	void SetNumber( int number ) {m_number = number;};
    
private:
    /** Indicates the shape and the line of the clef using clef ids  */
    int m_number;
    
public:

private:
    
};

} // namespace vrv    

#endif

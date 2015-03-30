/////////////////////////////////////////////////////////////////////////////
// Name:        space.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_SPACE_H__
#define __VRV_SPACE_H__

#include "durationinterface.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Space
//----------------------------------------------------------------------------

/** 
 * This class models the MEI <space>
 */
class Space: public LayerElement, public DurationInterface
{
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    Space( );
    virtual ~Space();
    virtual void Reset();
    virtual std::string GetClassName( ){ return "Space"; }; ;
    ///@}

private:
    
public:

private:
    
};

} // namespace vrv    

#endif

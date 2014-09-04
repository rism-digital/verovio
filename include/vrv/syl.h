/////////////////////////////////////////////////////////////////////////////
// Name:        syl.h
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_SYL_H__
#define __VRV_SYL_H__

#include "atts_shared.h"
#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// Syl
//----------------------------------------------------------------------------

class Syl: public DocObject,
    public AttSylLog
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Syl();
    virtual ~Syl();
    virtual void Reset();
    virtual std::string GetClassName( ) { return "Syl"; };
    ///@}
    
    /**
     * Add syl to a syl.
     */
    void AddSyl(Syl *syl);
    
protected:

private:
    
public:
    
private:

};

} // namespace vrv    
    
#endif

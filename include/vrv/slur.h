/////////////////////////////////////////////////////////////////////////////
// Name:        slur.h
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_SLUR_H__
#define __VRV_SLUR_H__

#include "measureelement.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Slur
//----------------------------------------------------------------------------

class Slur: public MeasureElement, public TimeSpanningInterface
{
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    Slur();
    virtual ~Slur();
    virtual void Reset();
    virtual std::string GetClassName( ) { return "Slur"; };
    ///@}
    
    //----------//
    // Functors //
    //----------//
    
    /**
     * See Object::FillStaffCurrentTimeSpanning
     */
    virtual int FillStaffCurrentTimeSpanning( ArrayPtrVoid params );
    
    /**
     * See Object::PrepareTimeSpanning
     */
    virtual int PrepareTimeSpanning( ArrayPtrVoid params );
    
private:
    
public:
    
private:
    
};

} // namespace vrv    
    
#endif

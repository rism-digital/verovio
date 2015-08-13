/////////////////////////////////////////////////////////////////////////////
// Name:        tie.h
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_TIE_H__
#define __VRV_TIE_H__

#include "measureelement.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Tie
// A TIE class is a simple slur - it takes in account only two notes, the first and second:
// NOTE tie NOTE tie etc
//----------------------------------------------------------------------------

class Tie: public MeasureElement, public TimeSpanningInterface
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Tie();
    virtual ~Tie();
    virtual void Reset();
    virtual std::string GetClassName( ) { return "Tie"; };
    virtual int Is() { return TIE; };
    ///@}
    ///@}
    
    //----------//
    // Functors //
    //----------//
    
    /**
     * See Object::FillStaffCurrentTimeSpanning
     */
    virtual int FillStaffCurrentTimeSpanning( ArrayPtrVoid *params );
    
    /**
     * See Object::PrepareTimeSpanning
     */
    virtual int PrepareTimeSpanning( ArrayPtrVoid *params );
    
    /**
     * Reset the drawing values before calling PrepareDrawing after changes.
     */
    virtual int ResetDarwing( ArrayPtrVoid *params );
    
private:
    
public:
    
private:
    
};

} // namespace vrv    
    
#endif

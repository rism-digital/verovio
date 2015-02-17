/////////////////////////////////////////////////////////////////////////////
// Name:        slur.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "slur.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// Slur
//----------------------------------------------------------------------------

Slur::Slur():
    MeasureElement("slur-"), TimeSpanningInterface()
{
    Reset();
}


Slur::~Slur()
{
}
        
void Slur::Reset()
{
    MeasureElement::Reset();
    TimeSpanningInterface::Reset();
}

//----------------------------------------------------------------------------
// Slur functor methods
//----------------------------------------------------------------------------

int Slur::PrepareTimeSpanning( ArrayPtrVoid params )
{
    // Pass it to the pseudo functor of the interface
    return TimeSpanningInterface::PrepareTimeSpanning(params, this);
}
    
int Slur::FillStaffCurrentTimeSpanning( ArrayPtrVoid params )
{
    // Pass it to the pseudo functor of the interface
    return  TimeSpanningInterface::FillStaffCurrentTimeSpanning(params, this);
}
    
int Slur::ResetDarwing( ArrayPtrVoid params )
{
    // Pass it to the pseudo functor of the interface
    return  TimeSpanningInterface::ResetDrawing(params, this);
};
    
} // namespace vrv

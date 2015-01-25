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

#include "note.h"

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
    // param 0: the IntTree
    std::vector<TimeSpanningInterface*> *slurs = static_cast<std::vector<TimeSpanningInterface*>*>(params[0]);
    bool *fillList = static_cast<bool*>(params[1]);
    
    if ((*fillList)==false) {
        return FUNCTOR_CONTINUE;
    }
    
    this->SetUuidStr();
    slurs->push_back(this);
    
    return FUNCTOR_CONTINUE;
}
    
} // namespace vrv

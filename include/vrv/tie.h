/////////////////////////////////////////////////////////////////////////////
// Name:        tie.h
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_TIE_H__
#define __VRV_TIE_H__

#include "layerelement.h"

namespace vrv {
    
class Note;

//----------------------------------------------------------------------------
// Tie
// A TIE class is a simple slur - it takes in account only two notes, the first and second:
// NOTE tie NOTE tie etc
//----------------------------------------------------------------------------

class Tie: public LayerElement
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
    ///@}
    
    /**
     * @name Set and get the first and second note of the tie
     * The setter asserts that no note was previously set.
     */
    ///@{
    void SetStart( Note *note );
    void SetEnd( Note *note );
    Note *GetStart() { return m_first; };
    Note *GetEnd() { return m_second; };
    ///@}
    
private:
    
public:
    
private:
    Note *m_first;
    Note *m_second;
    
};

} // namespace vrv    
    
#endif

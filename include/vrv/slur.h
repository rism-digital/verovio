/////////////////////////////////////////////////////////////////////////////
// Name:        slur.h
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_SLUR_H__
#define __VRV_SLUR_H__

#include "layerelement.h"

namespace vrv {
    
class Note;

//----------------------------------------------------------------------------
// Slur
//----------------------------------------------------------------------------

class Slur: public LayerElement
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
    
    /**
     * @name Set and get the first and second note of the tie
     * The setter asserts that no note was previously set.
     */
    ///@{
    void SetFirstNote( Note *note );
    void SetSecondNote( Note *note );
    Note *GetFirstNote() { return m_first; };
    Note *GetSecondNote() { return m_second; };
    ///@}
    
private:
    
public:
    
private:
    Note *m_first;
    Note *m_second;
    
};

} // namespace vrv    
    
#endif

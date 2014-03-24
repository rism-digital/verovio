/////////////////////////////////////////////////////////////////////////////
// Name:        slur.h
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_SLUR_H__
#define __VRV_SLUR_H__

#include "layer.h"
#include "note.h"
#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// Slur
//----------------------------------------------------------------------------

class Slur: public LayerElement
{
public:
    // constructors and destructors
    Slur();
    virtual ~Slur();
    
    virtual std::string GetClassName( ) { return "Slur"; };
    
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

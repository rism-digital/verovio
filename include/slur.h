/////////////////////////////////////////////////////////////////////////////
// Name:        musslur.h
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __MUS_SLUR_H__
#define __MUS_SLUR_H__

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
    
    virtual std::string MusClassName( ) { return "Slur"; };
    
	int GetNoteCount() const { return (int)m_children.size(); };
    
    /**
     * Add an element (a note or a rest) to a beam.
     * Only Note or Rest elements will be actually added to the beam.
     */
    void AddNote(LayerElement *element);
    
private:
    
public:
    
private:
    
};

} // namespace vrv    
    
#endif

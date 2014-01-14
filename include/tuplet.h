/////////////////////////////////////////////////////////////////////////////
// Name:        mustuplet.h
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __MUS_TUPLET_H__
#define __MUS_TUPLET_H__

#include "layer.h"
#include "note.h"
#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// Tuplet
//----------------------------------------------------------------------------

class Tuplet: public LayerElement, public ObjectListInterface
{
public:
    // constructors and destructors
    Tuplet();
    virtual ~Tuplet();
    
    virtual std::string MusClassName( ) { return "Tuplet"; };

	int GetNoteCount() const { return (int)m_children.size(); };
    
    /**
     * Add an element (a note or a rest) to a tuplet.
     * Only Note or Rest elements will be actually added to the beam.
     */
    void AddElement(LayerElement *element);
    
protected:
    /**
     * Filter the list for a specific class.
     * For example, keep only notes in Beam
     */
    virtual void FilterList();
    
private:
    
public:
    int m_num;
    int m_numbase;
    
private:

};

} // namespace vrv    
    
#endif

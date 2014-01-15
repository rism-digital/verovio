/////////////////////////////////////////////////////////////////////////////
// Name:        beam.h
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////
//

#ifndef __VRV_BEAM_H__
#define __VRV_BEAM_H__

#include "layerelement.h"
#include "note.h"

namespace vrv {

//----------------------------------------------------------------------------
// Beam
//----------------------------------------------------------------------------

class Beam: public LayerElement, public ObjectListInterface
{
public:
    // constructors and destructors
    Beam();
    virtual ~Beam();
    
    virtual std::string GetClassName( ) { return "Beam"; };
    
    int GetNoteCount() const { return (int)m_children.size(); };
    
    /**
     * Add an element (a note or a rest) to a beam.
     * Only Note or Rest elements will be actually added to the beam.
     */
    void AddElement(LayerElement *element);
    
    // functor
    //virtual int Save( ArrayPtrVoid params );
    
protected:
    /**
     * Filter the list for a specific class.
     * For example, keep only notes in Beam
     */
    virtual void FilterList();
    
private:

public:
    
private:
    
};
} // namespace vrv
#endif

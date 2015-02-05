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
#include "drawinglistinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Beam
//----------------------------------------------------------------------------

class Beam: public LayerElement, public ObjectListInterface, public DrawingListInterface
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes.
     */
    ///@{
    Beam();
    virtual ~Beam();
    virtual void Reset();
    virtual std::string GetClassName( ) { return "Beam"; };
    ///@}
    
    int GetNoteCount() const { return (int)m_children.size(); };
    
    /**
     * Add an element (a note or a rest) to a beam.
     * Only Note or Rest elements will be actually added to the beam.
     */
    void AddLayerElement(LayerElement *element);
    
    /**
     * @name Set and get the stem direction of the beam.
     */
    ///@{
    void SetDrawingStemDir( data_STEMDIRECTION stemDirection ) { m_drawingStemDir = stemDirection; };
    data_STEMDIRECTION GetDrawingStemDir() { return m_drawingStemDir; };
    
protected:
    /**
     * Filter the list for a specific class.
     * For example, keep only notes in Beam
     */
    virtual void FilterList();
    
private:
    data_STEMDIRECTION m_drawingStemDir;

public:
    
private:
    
};
} // namespace vrv
#endif

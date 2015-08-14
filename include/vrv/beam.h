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
    
// maximum number of partials allow
#define MAX_DURATION_PARTIALS 16
    
enum  {
    PARTIAL_NONE = 0,
    PARTIAL_THROUGH,
    PARTIAL_RIGHT,
    PARTIAL_LEFT
};

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
    virtual ClassId Is() { return BEAM; };
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
    
    /**
     *
     */
    const ArrayOfBeamElementCoords *GetElementCoords() { return &m_beamElementCoords; };
    
protected:
    /**
     * Filter the list for a specific class.
     * For example, keep only notes in Beam
     * This also initializes the m_beamElementCoords vector
     */
    virtual void FilterList( ListOfObjects *childList );
    /**
     * Initializes the m_beamElementCoords vector objects.
     * This is called by Beam::FilterList
     */
    void InitCoords( ListOfObjects *childList );
    
    /**
     * Clear the m_beamElementCoords vector and delete all the objects.
     */
    void ClearCoords();
    
    
private:

public:
    
private:
    /**
     * The drawing stem direction of the beam
     **/
    data_STEMDIRECTION m_drawingStemDir;
    /**
     * An array of the coordinates for each element
     **/
    ArrayOfBeamElementCoords m_beamElementCoords;
    
};
    
//----------------------------------------------------------------------------
// BeamElementCoord
//----------------------------------------------------------------------------


class BeamElementCoord
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    BeamElementCoord() { m_element = NULL; };
    virtual ~BeamElementCoord();
    
    int m_x;
    int m_y; // represents the point farthest from the beam
    int m_yTop; // y value of topmost note
    int m_yBottom; // y value of bottom-most note
    int m_yBeam; // y value of stem top position
    int m_dur; // drawing duration
    int m_breaksec;
    char m_partialFlags[MAX_DURATION_PARTIALS];
    LayerElement *m_element;
};
    
    
} // namespace vrv

#endif

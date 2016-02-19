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
#include "drawinginterface.h"

namespace vrv {
    
// the maximum allowed number of partials
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
     * Reset method resets all attribute classes.
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
     * Return information about the position in the beam
     */
    ///@{
    bool IsFirstInBeam(LayerElement *element);
    bool IsLastInBeam(LayerElement *element);
    ///@}
    
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
     * An array of coordinates for each element
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

    
//----------------------------------------------------------------------------
// BeamParams
//----------------------------------------------------------------------------

/**
 * Class for storing drawing parameters when calculating beams.
 * See View::DrawBeam and View::CalcBeam
 */
    
class BeamParams
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    BeamParams() {};
    virtual ~BeamParams() {};
    
    // values to be set before calling CalcBeam
    bool m_changingDur;
    bool m_beamHasChord;
    bool m_hasMultipleStemDir;
    bool m_cueSize;
    int m_shortestDur;
    data_STEMDIRECTION m_stemDir;
    
    // values set by CalcBeam
    int m_beamWidth;
    int m_beamWidthBlack;
    int m_beamWidthWhite;
    double m_startingY; // the initial position of the beam
    double m_beamSlope; // the slope of the beam
    double m_verticalBoost; //extra height to ensure the beam clears all the noteheads
};

} // namespace vrv

#endif

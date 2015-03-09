/////////////////////////////////////////////////////////////////////////////
// Name:        chord.h
// Author:      Andrew Horwitz
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_CHORD_H__
#define __VRV_CHORD_H__

#include <typeinfo>

//----------------------------------------------------------------------------

#include "atts_shared.h"
#include "durationinterface.h"
#include "layerelement.h"
#include "object.h"

namespace vrv {
    
//----------------------------------------------------------------------------
// Chord
//----------------------------------------------------------------------------

/**
 * This class represents a collection of notes in the same layer with the
 *    same onset time.
 * A chord is contained in a layer.
 * It contains notes.
 */
    
class Chord: public LayerElement, public ObjectListInterface, public DurationInterface, 
    public AttColoration,
    public AttCommon,
    public AttStemmed,
    public AttTiepresent
{
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    Chord( );
    virtual ~Chord();
    virtual void Reset();
    virtual std::string GetClassName( ) { return "Chord"; };
    
    /**
     * Add an element (only note supported) to a chord.
     */
    void AddLayerElement(LayerElement *element);

    virtual void FilterList( ListOfObjects *childlist );
    
    void GetYExtremes(int *yMax, int *yMin);
    
    /**
     * Returns list of notes that have accidentals
     */
    ListOfObjects GenerateAccidList();
    
    /**
     * @name Set and get the stem direction of the beam.
     */
    ///@{
    void SetDrawingStemDir( data_STEMDIRECTION stemDirection ) { m_drawingStemDir = stemDirection; };
    data_STEMDIRECTION GetDrawingStemDir() { return m_drawingStemDir; };

    ///@}
    
    //----------//
    // Functors //
    //----------//
    
    /**
     * See Object::PrepareTieAttr
     */
    virtual int PrepareTieAttr( ArrayPtrVoid params );
    
    
    /**
     * See Object::PrepareTieAttr
     */
    virtual int PrepareTieAttrEnd( ArrayPtrVoid params );
    
protected:
    /**
     * Clear the m_clusters vector and delete all the objects.
     */
    void ClearClusters();
    
private:
    data_STEMDIRECTION m_drawingStemDir;
    
public:
    std::list<ChordCluster*> m_clusters;
    
    /** 
     * Number of ledger lines for the chord where:
     * m_ledgerLines[0][x] is single-length, m_ledgerLines[1][x] is double-length
     * m_ledgerLines[x][0] is below staff, m_ledgerLines[x][1] is above staff
     */
    int m_ledgerLines[2][2];
    
    /**
     * Positions of dots in the chord to avoid overlapping
     */
    std::list<int> m_dots;
};

} // namespace vrv

#endif
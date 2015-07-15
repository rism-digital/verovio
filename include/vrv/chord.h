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

namespace vrv {

#define ledgermin(a,b) (((a)<(b))?(a):(b))
#define ledgermax(a,b) (((a)>(b))?(a):(b))
    
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
    ///@}
    
    /**
     * Add an element (only note supported) to a chord.
     */
    void AddLayerElement(LayerElement *element);

    virtual void FilterList( ListOfObjects *childlist );
    
    void GetYExtremes(int *yMax, int *yMin);
    
    /**
     * Returns list of notes that have accidentals
     */
    void ResetAccidList();
    
    /**
     * Prepares a 2D grid of booleans to track where accidentals are placed.
     * Further documentation in chord.cpp comments.
     */
    void ResetAccidSpace(int fullUnit);
    
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
     * Staff * is each staff for which the chord has notes and maps to:
     * a four char vector acting as a 2D array (2x2) where:
     * [0][x] is single-length, [1][x] is double-length
     * [x][0] is below staff, [x][1] is above staff
     */
    MapOfLedgerLineFlags m_drawingLedgerLines;
    
    /**
     * Positions of dots in the chord to avoid overlapping
     */
    std::list<int> m_dots;
    
    /**
     * Variables related to preventing overlapping in the X dimension for accidentals
     */
    std::vector<Note*> m_accidList;
    std::vector< std::vector<bool> > m_accidSpace;
    int m_accidSpaceTop, m_accidSpaceBot, m_accidSpaceLeft;
};

} // namespace vrv

#endif
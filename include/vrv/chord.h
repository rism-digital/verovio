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

    void FilterList();
    
    void GetYExtremes(int *yMax, int *yMin);
    
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
    
private:
    data_STEMDIRECTION m_drawingStemDir;
};

} // namespace vrv

#endif
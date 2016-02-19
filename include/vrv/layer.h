/////////////////////////////////////////////////////////////////////////////
// Name:        layer.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_LAYER_H__
#define __VRV_LAYER_H__

#include "atts_shared.h"
#include "clef.h"
#include "drawinginterface.h"
#include "object.h"

namespace vrv {

class DeviceContext;
class LayerElement;
class Note;
class ScoreDef;
class StaffDef;

//----------------------------------------------------------------------------
// Layer
//----------------------------------------------------------------------------

/**
 * This class represents a layer in a laid-out score (Doc).
 * A Layer is contained in a Staff.
 * It contains LayerElement objects.
*/
class Layer: public DocObject, public DrawingListInterface, public ObjectListInterface, public StaffDefDrawingInterface,
    public AttCommon
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Layer( );
    virtual ~Layer();
    virtual void Reset();
    virtual std::string GetClassName( ) { return "Layer"; };
    virtual ClassId Is() { return LAYER; };
    ///@}
	
    /**
     * @name Methods for adding allowed content
     */
    ///@{
	void AddLayerElement( LayerElement *element, int idx = -1 );
    ///@}
    	    
    /**
     * Return the index position of the layer in its staff parent.
     * The index position is 0-based.
     */
    int GetLayerIdx() const { return Object::GetIdx(); };
    
	LayerElement *GetPrevious( LayerElement *element );
	LayerElement *GetAtPos( int x );
	LayerElement *Insert( LayerElement *element, int x ); // return a pointer to the inserted element
    
    /** 
     * Get the current clef for the test element.
     * Goes back on the layer until a clef is found.
     * This is used when inserting a note by passing a y position because we need
     * to know the clef in order to get the pitch.
     */
	Clef *GetClef ( LayerElement *test );
    
    /** 
     * Return the clef offset for the position x.
     * The method uses Layer::GetClef first to find the clef before test.
     */
    int GetClefOffset( LayerElement *test  );
    
    /**
     * Set drawing clef, keysig and mensur if necessary and if available.
     * Also set the current clef.
     */
    void SetDrawingAndCurrentValues( StaffDef *currentStaffDef );
    
    /**
     * @name Set and get the stem direction of the layer.
     * This stays STEMDIRECTION_NONE with on single layer in the staff. 
     */
    ///@{
    void SetDrawingStemDir( data_STEMDIRECTION stemDirection ) { m_drawingStemDir = stemDirection; };
    data_STEMDIRECTION GetDrawingStemDir() { return m_drawingStemDir; };
    ///@}
    
    
    //----------//
    // Functors //
    //----------//
    
    /**
     * Align horizontally the content of a layer.
     */
    virtual int AlignHorizontally( ArrayPtrVoid *params );

    /**
     * Align horizontally the content of a layer.
     */
    virtual int AlignHorizontallyEnd( ArrayPtrVoid *params );
    
    /**
     * Builds a tree of ints (IntTree) with the staff/layer/verse numbers
     * and for staff/layer to be then processed.
     */
    virtual int PrepareProcessingLists( ArrayPtrVoid *params );

    /**
     * Set the drawing position (m_drawingX and m_drawingY) values for objects
     */
    virtual int SetDrawingXY( ArrayPtrVoid *params );
    
    /**
     * Functor for setting mRpt drawing numbers (if required)
     * See implementation and Object::PrepareRpt
     */
    virtual int PrepareRpt( ArrayPtrVoid *params );
    
private:
    
public:
    
protected:
    
private:
    /**
     *
     */
    data_STEMDIRECTION m_drawingStemDir;
    
};

} // namespace vrv

#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        drawinglistinterface
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_DRAWING_LIST_INTERFACE_H__
#define __VRV_DRAWING_LIST_INTERFACE_H__

#include "object.h"

namespace vrv {
    
class LayerElement;

//----------------------------------------------------------------------------
// DrawingListInterface
//----------------------------------------------------------------------------

/** 
 * This class is an interface for elements with duration, such as notes and rests.
 * It is not an abstract class but should not be instanciate directly.
 */
class DrawingListInterface
{
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    DrawingListInterface();
    virtual ~DrawingListInterface();
    virtual void Reset();
    ///@}
    
    /**
     * Add an element to the drawing list.
     * The drawing list is used to postponed the drawing of elements
     * that need to be drawn in a particular order.
     * For example, we need to draw beams before tuplets
     */
    void AddToDrawingList( LayerElement *element );
    
    /**
     * Return the drawing list.
     * This is used when actually drawing the list (see View::DrawLayerList)
     */
    ListOfObjects *GetDrawingList( );
    
    /**
     * Reset the drawing list.
     * Clears the list - called when the layer starts to be drawn
     */
    void ResetDrawingList( );
    
private:
    
public:

private:
    /** The list of object for which drawing is postponed */
    ListOfObjects m_drawingList;
};
    
} // namespace vrv 

#endif

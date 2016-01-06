/////////////////////////////////////////////////////////////////////////////
// Name:        dot.h
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_DOT_H__
#define __VRV_DOT_H__

#include "layerelement.h"
#include "positioninterface.h"

namespace vrv {
    
class Note;

//----------------------------------------------------------------------------
// Dot
//----------------------------------------------------------------------------

class Dot: public LayerElement, public PositionInterface
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Dot();
    virtual ~Dot();
    virtual void Reset();
    virtual std::string GetClassName() { return "Dot"; };
    virtual ClassId Is() { return DOT; };
    ///@}
    
    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() { return true; };
    
    //----------//
    // Functors //
    //----------//
    
    /**
     * See Object::PreparePointersByLayer
     */
    virtual int PreparePointersByLayer(ArrayPtrVoid *params);
    
    /**
     * Reset the drawing values before calling PrepareDrawing after changes.
     */
    virtual int ResetDrawing(ArrayPtrVoid *params);
    
protected:

private:
    
public:
    /**
     * A pointer to the note the point relates to.
     */
    Note *m_drawingNote;
    
private:

};

} // namespace vrv
    
#endif

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

class Dot : public LayerElement, public PositionInterface, public AttColor, public AttDotLog {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Dot();
    virtual ~Dot();
    virtual Object *Clone() const { return new Dot(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Dot"; }
    virtual ClassId GetClassId() const { return DOT; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual PositionInterface *GetPositionInterface() { return dynamic_cast<PositionInterface *>(this); }
    ///@}

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::PreparePointersByLayer
     */
    virtual int PreparePointersByLayer(FunctorParams *functorParams);

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

    /**
     * See Object::ResetHorizontalAlignment
     */
    virtual int ResetHorizontalAlignment(FunctorParams *functorParams);

private:
    //
public:
    /**
     * A pointer to the note the point relates to.
     */
    Note *m_drawingNote;

    /**
     * A pointer to the next LayerElement (note, barLine, etc.)
     */
    LayerElement *m_drawingNextElement;

private:
};

} // namespace vrv

#endif

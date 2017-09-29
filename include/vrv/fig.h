/////////////////////////////////////////////////////////////////////////////
// Name:        fig.h
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FIG_H__
#define __VRV_FIG_H__

#include "atts_shared.h"
#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// Fig
//----------------------------------------------------------------------------

/**
 * This class models the MEI <fig>
 */
class Fig : public Object, public AttTyped {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    Fig();
    virtual ~Fig();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Fig"; }
    virtual ClassId GetClassId() const { return FIG; }
    ///@}

    /**
     * Add an element (svg) to an fig.
     * Only supported elements will be actually added to the child list.
     */
    virtual void AddChild(Object *object);
    
    /**
     * @name Get the X and Y drawing position
     */
    ///@{
    virtual int GetDrawingX() const;
    virtual int GetDrawingY() const;
    ///@}
    
    /**
     * @name Get and set the X and Y drawing relative positions
     */
    ///@{
    int GetDrawingXRel() const { return m_drawingXRel; }
    virtual void SetDrawingXRel(int drawingXRel);
    int GetDrawingYRel() const { return m_drawingYRel; }
    virtual void SetDrawingYRel(int drawingYRel);
    ///@}
    
    //----------//
    // Functors //
    //----------//
    
    /**
     * See Object::ResetVerticalAlignment
     */
    virtual int ResetVerticalAlignment(FunctorParams *functorParams);
                     
    /**
     * See Object::AlignVertically
     */
    ///@{
    virtual int AlignVertically(FunctorParams *functorParams);
    ///@}
    
private:
    //
public:
    //
private:
    /**
     * The Y drawing relative position of the object.
     */
    int m_drawingYRel;
    
    /**
     * The X drawing relative position of the object.
     */
    int m_drawingXRel;
};

} // namespace vrv

#endif

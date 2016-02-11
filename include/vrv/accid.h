/////////////////////////////////////////////////////////////////////////////
// Name:        accid.h
// Author:      Laurent Pugin
// Created:     201X
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_ACCID_H__
#define __VRV_ACCID_H__

#include "layerelement.h"
#include "positioninterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Accid
//----------------------------------------------------------------------------

class Accid: public LayerElement, public PositionInterface,
    public AttAccidental,
    public AttAccidLog
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Accid();
    virtual ~Accid();
    virtual void Reset();
    virtual std::string GetClassName( ) { return "Accid"; };
    virtual ClassId Is() { return ACCID; };
    ///@}
    
    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() { return true; };
    
    
    //----------//
    // Functors //
    //----------//
    
    /**
     * See Object::PreparePointersByLayer
     */
    virtual int PreparePointersByLayer( ArrayPtrVoid *params );
    
protected:

private:
    
public:
    /**
     * Indicates if is cue size for accid object created for @accid.
     * See Note::PreparePointersByLayer and View::DrawAccid
     */
    bool m_drawingCueSize;
    
private:

};

} // namespace vrv    
    
#endif

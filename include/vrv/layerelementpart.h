/////////////////////////////////////////////////////////////////////////////
// Name:        layerelementpart.h
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_LAYER_ELEMENT_PART_H__
#define __VRV_LAYER_ELEMENT_PART_H__

#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Stem
//----------------------------------------------------------------------------

/**
 * This class models a stem as a layer element part and has not direct MEI equivlatent.
 */
class Stem : public LayerElement {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Stem();
    virtual ~Stem();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Stem"; }
    virtual ClassId GetClassId() const { return STEM; }
    ///@}
    
    //----------//
    // Functors //
    //----------//
    
    /**
     * Overwritten version of Save that avoids anything to be written
     */
    ///@{
    virtual int Save(FunctorParams *functorParams) { return FUNCTOR_CONTINUE; }
    virtual int SaveEnd(FunctorParams *functorParams) { return FUNCTOR_CONTINUE; }
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif

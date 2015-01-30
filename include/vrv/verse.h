/////////////////////////////////////////////////////////////////////////////
// Name:        verse.h
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_VERSE_H__
#define __VRV_VERSE_H__

#include "atts_shared.h"
#include "layerelement.h"

namespace vrv {
    
class Syl;

//----------------------------------------------------------------------------
// Verse
//----------------------------------------------------------------------------

class Verse: public LayerElement,
    public AttCommon
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Verse();
    virtual ~Verse();
    virtual void Reset();
    virtual std::string GetClassName( ) { return "Verse"; };
    ///@}
    
    /**
     * Add an element (a syl) to a verse.
     * Only Syl elements will be actually added to the verse.
     */
    void AddLayerElement(LayerElement *element);
    
    //----------//
    // Functors //
    //----------//
    
    /**
     * Align the content of a staff vertically.
     * See Object::AlignVertically
     */
    virtual int AlignVertically( ArrayPtrVoid params );
    
    /**
     * Builds a tree of int (IntTree) with the staff/layer/verse numbers
     * and for staff/layer to be then processed.
     */
    virtual int PrepareProcessingLists( ArrayPtrVoid params );
    
protected:

private:
    
public:
    
private:

};

} // namespace vrv    
    
#endif

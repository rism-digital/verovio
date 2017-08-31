/////////////////////////////////////////////////////////////////////////////
// Name:        tuplet.h
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TUPLET_H__
#define __VRV_TUPLET_H__

#include "atts_cmn.h"
#include "atts_shared.h"
#include "atts_visual.h"
#include "layerelement.h"

namespace vrv {

class Note;

//----------------------------------------------------------------------------
// Tuplet
//----------------------------------------------------------------------------

class Tuplet : public LayerElement,
               public ObjectListInterface,
               public AttColor,
               public AttDurationRatio,
               public AttNumberPlacement,
               public AttTupletVis {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Tuplet();
    virtual ~Tuplet();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Tuplet"; }
    virtual ClassId GetClassId() const { return TUPLET; }
    ///@}

    /**
     * Add an element (a note or a rest) to a tuplet.
     * Only Note or Rest elements will be actually added to the beam.
     */
    virtual void AddChild(Object *object);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::PrepareLayerElementParts
     */
    virtual int PrepareLayerElementParts(FunctorParams *functorParams);

protected:
    /**
     * Filter the list for a specific class.
     * For example, keep only notes in Beam
     */
    virtual void FilterList(ListOfObjects *childList);

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        durationinterface.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_DURATION_INTERFACE_H__
#define __VRV_DURATION_INTERFACE_H__

#include "atts_cmn.h"
#include "atts_shared.h"

namespace vrv {

class Object;

//----------------------------------------------------------------------------
// DurationInterface
//----------------------------------------------------------------------------

/** 
 * This class is an interface for elements with duration, such as notes and rests.
 * It is not an abstract class but should not be instanciate directly.
 */
class DurationInterface:
    public AttAugmentdots,
    public AttBeamsecondary,
    public AttDurationMusical,
    public AttDurationPerformed,
    public AttDurationRatio,
    public AttFermatapresent
{
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    DurationInterface();
    virtual ~DurationInterface();
    virtual void Reset();
    ///@}
    
    /* Set the logic (gestural) duration */
    virtual void SetDurationGes( int value );
    
    /**
     * Returns the duration (in double) for the element.
     * It returns 0.0 for grace notes.
     * Careful: this method is not overriding LayerElement::GetAlignementDuration since
     * LayerElement and DurationInterface have no inheritance link.
     */
    virtual double GetAlignementDuration( int num, int numbase );
    
    /**
     * Look if the note or rest is in a beam.
     * Look for the fist beam parent and check is the note is in is content list.
     * Looking in the content list is necessary for grace notes or imbricated beams.
     */
    bool IsInBeam( Object *noteOrRest );
    
    /**
     * Return true if the note or rest is the first of a beam.
     */
    bool IsFirstInBeam( Object *noteOrRest );
    
    /**
     * Return true if the note or rest is the last of a beam.
     */
    bool IsLastInBeam( Object *noteOrRest );
    
    /**
     * Inteface comparison operator. 
     * Check if the LayerElement has a DurationInterface and compare attributes
     */
    bool HasIdenticalDurationInterface(DurationInterface  *otherDurationInterface);
    
private:
    
public:

private:
    
};
    
} // namespace vrv 

#endif

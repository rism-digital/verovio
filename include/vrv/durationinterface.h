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

class Mensur;
class Object;

//----------------------------------------------------------------------------
// DurationInterface
//----------------------------------------------------------------------------

/** 
 * This class is an interface for elements with duration, such as notes and rests.
 * It is not an abstract class but should not be instanciated directly.
 */
class DurationInterface: public Interface,
    public AttAugmentdots,
    public AttBeamsecondary,
    public AttDurationMusical,
    public AttDurationPerformed,
    public AttDurationRatio,
    public AttFermatapresent,
    public AttStaffident
{
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method resets all attribute classes
     */
    ///@{
    DurationInterface();
    virtual ~DurationInterface();
    virtual void Reset();
    virtual InterfaceId IsInterface() { return INTERFACE_DURATION; };
    ///@}SetDurationGes
    
    /**
     * Returns the duration (in double) for the element.
     * It returns 0.0 for grace notes.
     * Careful: this method is not overriding LayerElement::GetAlignmentDuration since
     * LayerElement and DurationInterface have no inheritance link.
     */
    virtual double GetAlignmentDuration( int num, int numbase );
    
    /**
     * Returns the duration (in double) for the element for mensural notation
     * Currently this assume brevis equality (through DUR_MENSURAL_REF) and would
     * need to be modified for shorter equality in later repertoire.
     */
    virtual double GetAlignmentMensuralDuration( int num, int numbase, Mensur *currentMensur );
    
    /**
     * Return true if the note or rest is the first of a beam.
     */
    bool IsFirstInBeam( LayerElement *noteOrRest );
    
    /**
     * Return true if the note or rest is the last of a beam.
     */
    bool IsLastInBeam( LayerElement *noteOrRest );

    
    /**
     * Return the actual duration of the note, for both CMN and mensural durations
     * See data_DURATION
     * For CMN, it is the same (DURATION_1 == DUR_1)
     * For mensural, we need to apply the DUR_MENSURAL_MASK
     */
    int GetActualDur();
    
    /**
     * Return true if the value is a mensural (DURATION_longa, brevis, etc.)
     */
    bool IsMensural();
    
    /**
     * Interface comparison operator.
     * Check if the LayerElement has a DurationInterface and compare attributes
     */
    bool HasIdenticalDurationInterface(DurationInterface  *otherDurationInterface);
    
private:
    
public:

private:
    
};
    
} // namespace vrv 

#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        durationinterface.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_DURATION_INTERFACE_H__
#define __VRV_DURATION_INTERFACE_H__

namespace vrv {

class Object;

//----------------------------------------------------------------------------
// DurationInterface
//----------------------------------------------------------------------------

/** 
 * This class is an interface for elements with duration, such as notes and rests.
 * It is not an abstract class but should not be instanciate directly.
 */
class DurationInterface
{
public:
    // constructors and destructors
    DurationInterface();
    virtual ~DurationInterface();
    
    virtual void SetDuration( int value );
    /* Set the logic (gestural) duration */
    virtual void SetDurationGes( int value );
    
    
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
    /** Indicates a break in the beaming */
    unsigned char m_breakSec;
    /** Indicates the number of augmentation dots */
    unsigned char m_dots;
    /** Indicates the duration */
    int m_dur;
    /** Indicates the logical (gestural) duration */
    int m_durGes;
    /** Indicates the numerator of the duration ratio */
    int m_num;
    /** Indicates the denominator of the duration ratio */
    int m_numBase;
    /** Indicates if intial, medial or termial part of a tuplet for up to 6 nesting/overlaping tuplets */
    bool m_fermata;
    

private:
    
};
    
} // namespace vrv 

#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        positioninterface.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_POSITION_INTERFACE_H__
#define __VRV_POSITION_INTERFACE_H__

namespace vrv {

//----------------------------------------------------------------------------
// PositionInterface
//----------------------------------------------------------------------------

/** 
 * This class is an interface for elements with a position on the staff, such as rests.
 * It is not an abstract class but should not be instanciate directly.
 * For now, the position is handled in a similar way that for PitchInterface, that is with a pitch and octave. 
 */
class PositionInterface
{
public:
    // constructors and destructors
    PositionInterface();
    virtual ~PositionInterface();
    
    /** Set the position */
    virtual void SetPosition( int pname, int oct );
    
    /** Get the position */
    virtual bool GetPosition( int *pname, int *oct );
    
    /**
     * Inteface comparison operator. 
     * Check if the LayerElement has a PositionInterface and compare attributes
     */
    bool HasIdenticalPositionInterface(PositionInterface  *otherPitchInterface);

    
private:
    
public:
    /** Indicates the octave information */
    char m_oct;
    /** Indicates the pitch name */
    unsigned char m_pname;

private:
    
};
    
} // namespace vrv    

#endif

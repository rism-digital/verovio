/////////////////////////////////////////////////////////////////////////////
// Name:        pitchinterface.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_PITCH_INTERFACE_H__
#define __VRV_PITCH_INTERFACE_H__

namespace vrv {

//----------------------------------------------------------------------------
// PitchInterface
//----------------------------------------------------------------------------

/** 
 * This class is an interface for elements with pitch, such as notes and neumes.
 * It is not an abstract class but should not be instanciate directly.
 */
class PitchInterface
{
public:
    // constructors and destructors
    PitchInterface();
    virtual ~PitchInterface();
 
    /** Set the pitch */
    virtual void SetPitch( int pname, int oct );
    
    /** Get the pitch */
    virtual bool GetPitch( int *pname, int *oct );
    
    /**
     * Interface comparison operator. 
     * Check if the LayerElement has a PitchInterface and compare attributes
     */
    bool HasIdenticalPitchInterface(PitchInterface  *otherPitchInterface);

    
private:
    
public:
    /** Indicates the accidental */
    unsigned char m_accid;
    /** Provides the octave information */
    char m_oct;
    /** Indicates the pitch name */
    unsigned char m_pname;

private:
    
};

} // namespace vrv    

#endif

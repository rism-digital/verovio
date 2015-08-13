/////////////////////////////////////////////////////////////////////////////
// Name:        timeinterface.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_TIME_INTERFACE_H__
#define __VRV_TIME_INTERFACE_H__

#include "atts_shared.h"
#include "vrvdef.h"

namespace vrv {

class DocObject;
class LayerElement;

//----------------------------------------------------------------------------
// TimeSpanningInterface
//----------------------------------------------------------------------------

/** 
 * This class is an interface for elements with duration, such as notes and rests.
 * It is not an abstract class but should not be instanciate directly.
 */
class TimeSpanningInterface: public AttInterface,
    public AttStartendid,
    public AttStartid,
    public AttTimestampMusical,
    public AttTimestamp2Musical
{
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    TimeSpanningInterface();
    virtual ~TimeSpanningInterface();
    virtual void Reset();
    ///@}
    
    /**
     * @name Set and get the first and second LayerElement
     * The setter asserts that no LayerElement was previously set.
     */
    ///@{
    void SetStart( LayerElement *start );
    void SetEnd( LayerElement *end );
    LayerElement *GetStart() { return m_start; };
    LayerElement *GetEnd() { return m_end; };
    ///@}
    
    /**
     *
     */
    bool SetStartAndEnd( LayerElement *element );
    
    /**
     *
     */
    bool HasStartAndEnd( ) { return ( m_start && m_end ); };
    
    /**
     *
     */
    void SetUuidStr();
    
   
protected:
    
    //-----------------//
    // Pseudo functors //
    //-----------------//
    
    /**
     * We have functor in the interface for avoiding code duplication in each implementation class.
     * Since we are in an interface, we need to pass the DocObject (implementation) to
     * the functor method. These not called by the Process/Call loop but by the implementaion
     * classes explicitely. See Slur::FillStaffCurrentTimeSpanning for an example.
     */
    
    /**
     * See Object::FillStaffCurrentTimeSpanning
     */
    virtual int FillStaffCurrentTimeSpanning( ArrayPtrVoid *params, DocObject *object );
    
    /**
     * See Object::PrepareTimeSpanning
     */
    virtual int PrepareTimeSpanning( ArrayPtrVoid *params, DocObject *object );
    
    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing( ArrayPtrVoid *params, DocObject *object );
    
private:
    /**
     *
     */
    std::string ExtractUuidFragment(std::string refUuid);
     
    
public:

private:
    LayerElement *m_start;
    LayerElement *m_end;
    std::string m_startUuid, m_endUuid;
    
};
    
} // namespace vrv 

#endif

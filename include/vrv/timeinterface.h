/////////////////////////////////////////////////////////////////////////////
// Name:        timeinterface.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_TIME_INTERFACE_H__
#define __VRV_TIME_INTERFACE_H__

#include "atts_shared.h"

namespace vrv {

class LayerElement;

//----------------------------------------------------------------------------
// TimeSpanningInterface
//----------------------------------------------------------------------------

/** 
 * This class is an interface for elements with duration, such as notes and rests.
 * It is not an abstract class but should not be instanciate directly.
 */
class TimeSpanningInterface:
    public AttStartendid,
    public AttStartid
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
    void SetStart( LayerElement *note );
    void SetEnd( LayerElement *note );
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
    void SetUuidStr();

    
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

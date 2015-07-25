/////////////////////////////////////////////////////////////////////////////
// Name:        textdirective.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_TEXT_DIRECTIVE_H__
#define __VRV_TEXT_DIRECTIVE_H__

#include "measureelement.h"
#include "textdirinterface.h"

namespace vrv {
    
//----------------------------------------------------------------------------
// MeasureTempo
//----------------------------------------------------------------------------

/**
 * This class is an interface for <tempo> elements at the measure level
 */
class MeasureTempo: public MeasureElement, public TempoInterface
{
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    MeasureTempo();
    virtual ~MeasureTempo();
    virtual void Reset();
    ///@}
    
protected:
    
private:
    
public:
    
private:
    
};
    
} // namespace vrv 

#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        textdirective.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_TEXT_DIRECTIVE_H__
#define __VRV_TEXT_DIRECTIVE_H__

#include "floatingelement.h"
#include "textdirinterface.h"

namespace vrv {
    
//----------------------------------------------------------------------------
// MeasureTempo
//----------------------------------------------------------------------------

/**
 * This class is an interface for <tempo> elements at the measure level
 */
class Tempo: public FloatingElement, public TextDirInterface
{
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    Tempo();
    virtual ~Tempo();
    virtual void Reset();
    virtual std::string GetClassName( ) { return "Tempo"; };
    virtual ClassId Is() { return TEMPO; };
    ///@}
    
protected:
    
private:
    
public:
    
private:
    
};
    
} // namespace vrv 

#endif

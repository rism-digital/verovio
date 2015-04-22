/////////////////////////////////////////////////////////////////////////////
// Name:        mensur.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_MENSUR_H__
#define __VRV_MENSUR_H__

#include "atts_mensural.h"
#include "atts_shared.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Mensur
//----------------------------------------------------------------------------

class MensurAttr;
    
/** 
 * This class models the MEI <mensur> element. 
 */
class Mensur: public LayerElement,
    public AttDurationRatio,
    public AttMensuralShared,
    public AttMensurLog,
    public AttMensurVis,
    public AttSlashcount
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes.
     */
    ///@{
    Mensur();
    Mensur( MensurAttr *mensurAttr );
    virtual ~Mensur();
    virtual void Reset();
    virtual std::string GetClassName( ) { return "Mensur"; };
    virtual Object* Clone() { return new Mensur(*this); };
    ///@}
    
private:
    
public:
    /** 
     * Static member for setting a value from a controller.
     * Used for example in SetValue
     */
    static int s_num;
    /** 
     * Static member for setting a value from a controller.
     * Used for examle in SetValue.
     */
    static int s_numBase;

private:
    
};

    
//----------------------------------------------------------------------------
// MensurAttr
//----------------------------------------------------------------------------

/**
 * This class models the MEI @mensur attributes in scoreDef or staffDef elements.
 */
class MensurAttr: public Object,
    public AttMensuralShared,
    public AttMensuralLog
{
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    MensurAttr();
    virtual ~MensurAttr();
    virtual void Reset();
    virtual std::string GetClassName( ) { return "MensurAttr"; };
    virtual Object* Clone() { return new MensurAttr(*this); };
    ///@}
    
private:
    
public:
    
private:
    
};

} // namespace vrv

#endif

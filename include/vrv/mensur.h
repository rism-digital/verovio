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
    public AttMensurLog,
    public AttMensurVis,
    public AttSlashcount
{
public:
    // constructors and destructors
    Mensur();
    Mensur( MensurAttr *mensurAttr );
    virtual ~Mensur();
    virtual void Reset();
    virtual std::string GetClassName( ) { return "Mensur"; };
    virtual Object* Clone() { return new Mensur(*this); };
    ///@}
    
    /**
     * Comparison operator. 
     * Check if the LayerElement if a Mensur and compare attributes
     */
    virtual bool operator==(Object& other);
    
    /**
     * Set the value for the mensur.
     */
	//virtual void SetValue( int value, int flag = 0 );
    
private:
    
public:
    /** Indicates the number of dots with the sign (max 1 for now). */
    //unsigned char m_dot;
    /** Indicates the numerator of the duration ratio. */
    //int m_num;
    /** Indicates the denominator of the duration ratio. */
    //int m_numBase;
    /** Indicates if the sign is reversed. */
    //bool m_reversed;
    /** Indicates the sign of the mensuration signature. */
    //data_MENSURATIONSIGN m_sign;
    /** Indicates the number of slashes with the sign (max 1 for now). */
    //unsigned char m_slash;
    
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
    public AttMensurDefaultLog
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
    
    /**
     * Comparison operator.
     * Check if the LayerElement if a MensurAttr and compare attributes
     */
    virtual bool operator==(Object& other);
    
private:
    
public:
    
private:
    
};

} // namespace vrv

#endif

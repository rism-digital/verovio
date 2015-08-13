/////////////////////////////////////////////////////////////////////////////
// Name:        metersig.h
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_METERSIG_H__
#define __VRV_METERSIG_H__

#include "atts_shared.h"
#include "layerelement.h"

namespace vrv {
    
class MeterSigAttr;

//----------------------------------------------------------------------------
// MeterSig
//----------------------------------------------------------------------------

/** 
 * This class models the MEI <meterSig> element. 
 */
class MeterSig: public LayerElement,
    public AttMeterSigLog
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes.
     */
    ///@{
    MeterSig();
    MeterSig( MeterSigAttr *meterSigAttr );
    void Init();
    virtual ~MeterSig();
    virtual void Reset();
    virtual std::string GetClassName( ) { return "MeterSig"; };
    virtual int Is() { return METER_SIG; };
    virtual Object* Clone() { return new MeterSig(*this); };
    ///@}

private:
    
public:

private:
    
};
    

//----------------------------------------------------------------------------
// MeterSigAttr
//----------------------------------------------------------------------------

/**
 * This class models the MEI @meter attributes in scoreDef or staffDef elements.
 */
class MeterSigAttr: public Object,
    public AttMeterSigDefaultLog,
    public AttMeterSigDefaultVis
{
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    MeterSigAttr();
    virtual ~MeterSigAttr();
    virtual void Reset();
    virtual std::string GetClassName( ) { return "MeterSigAttr"; };
    virtual int Is() { return METER_SIG_ATTR; };
    virtual Object* Clone() { return new MeterSigAttr(*this); };
    ///@}
    
private:
    
public:
    
private:
    
};

} // namespace vrv

#endif

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
class MeterSig: public LayerElement, public AttMeterSigLog
{
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes.
     */
    ///@{
    MeterSig();
    MeterSig( MeterSigAttr *meterSigAttr );
    virtual ~MeterSig();
    void Reset();
    virtual std::string GetClassName( ) { return "MeterSig"; };
    virtual Object* Clone() { return new MeterSig(*this); };
    ///@}
    
    /**
     * Return the default horizontal spacing after a keysig.
     */
    virtual int GetHorizontalSpacing( ) { return 20; };
    
    /**
     * Comparison operator. 
     * Check if the LayerElement if a MeterSig and compare attributes
     */
    virtual bool operator==(Object& other);

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
class MeterSigAttr: public Object, public AttMeterSigDefaultLog, public AttMeterSigDefaultVis
{
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    MeterSigAttr();
    virtual ~MeterSigAttr();
    void Reset();
    virtual std::string GetClassName( ) { return "MeterSigAttr"; };
    virtual Object* Clone() { return new MeterSigAttr(*this); };
    ///@}
    
    /**
     * Comparison operator.
     * Check if the LayerElement if a MeterSigAttr and compare attributes
     */
    virtual bool operator==(Object& other);
    
private:
    
public:
    
private:
    
};

} // namespace vrv

#endif

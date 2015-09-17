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

class ScoreDefInterface;

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
    MeterSig( ScoreDefInterface *meterSigAttr );
    void Init();
    virtual ~MeterSig();
    virtual void Reset();
    virtual std::string GetClassName( ) { return "MeterSig"; };
    virtual ClassId Is() { return METERSIG; };
    virtual Object* Clone() { return new MeterSig(*this); };
    ///@}
    
    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() { return true; };

private:
    
public:

private:
    
};

} // namespace vrv

#endif

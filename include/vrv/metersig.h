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
class MeterSig : public LayerElement, public AttMeterSigLog, public AttMeterSigVis {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes.
     */
    ///@{
    MeterSig();
    MeterSig(const ScoreDefInterface *meterSigAttr);
    void Init();
    virtual ~MeterSig();
    virtual void Reset();
    virtual std::string GetClassName() const { return "MeterSig"; }
    virtual ClassId GetClassId() const { return METERSIG; }
    virtual Object *Clone() const { return new MeterSig(*this); }
    ///@}

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }
    
    /** Convert rendition to form */
    meterSigVis_FORM meterSigDefaultVisToMeterSigVis (meterSigDefaultVis_METERREND rend);

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif

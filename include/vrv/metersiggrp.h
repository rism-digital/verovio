/////////////////////////////////////////////////////////////////////////////
// Name:        metersiggrp.h
// Author:      Andriy Makarchuk
// Created:     2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_METERSIGGRP_H__
#define __VRV_METERSIGGRP_H__

#include "atts_cmn.h"
#include "atts_mei.h"
#include "atts_shared.h"
#include "metersig.h"
#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// MeterSigGrp
//----------------------------------------------------------------------------

/**
 * This class represents a MEI staffGrp.
 * It contains StaffDef objects.
 */
class MeterSigGrp : public Object,
                    public ObjectListInterface,
                    public LinkingInterface,
                    public AttBasic,
                    public AttLabelled,
                    public AttTyped,
                    public AttMeterSigGrpLog {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    MeterSigGrp();
    virtual ~MeterSigGrp();
    virtual Object *Clone() const { return new MeterSigGrp(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "MeterSigGrp"; }
    virtual ClassId GetClassId() const { return METERSIGGRP; }
    ///@}
    
    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual LinkingInterface *GetLinkingInterface() { return dynamic_cast<LinkingInterface *>(this); }
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    virtual bool IsSupportedChild(Object *object);
    ///@}

    //----------//
    // Functors //
    //----------//

protected:
    //
private:
    //
}; // MeterSigGrp

} // namespace vrv

#endif // __VRV_METERSIGGRP_H__
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
#include "linkinginterface.h"
#include "object.h"

namespace vrv {

class MeterSig;

//----------------------------------------------------------------------------
// MeterSigGrp
//----------------------------------------------------------------------------

/**
 * This class represents a MEI meterSigGrp.
 * It contains meterSigGrp objects.
 */
class MeterSigGrp : public Object,
                    public ObjectListInterface,
                    public LinkingInterface,
                    public AttBasic,
                    public AttLabelled,
                    public AttMeterSigGrpLog,
                    public AttTyped {
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
    virtual LinkingInterface *GetLinkingInterface() { return this; }
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    virtual bool IsSupportedChild(Object *object);
    ///@}

    /**
     * Add specified measureId to the m_alternatingMeasures vector
     */
    void AddMeasureIdToVector(const std::string &measureId);

    /**
     * Get simplified (i.e. single metersig with count/unit) based on the MeterSigGrp function
     */
    MeterSig *GetSimplifiedMeterSig();

    /**
     * Set counter for the alternating meterSigGrp based on the provided measureId
     */
    void SetMeasureBasedCount(const std::string &measureId);

    //----------//
    // Functors //
    //----------//
    /**
     * See Object::AlignHorizontally
     */
    int AlignHorizontally(FunctorParams *functorParams);

protected:
    /**
     * Filter the flat list and keep only meterSigGrp elements.
     */
    virtual void FilterList(ArrayOfObjects *childList);

private:
    // vector with alternating measures Ids (used only with meterSigGrpLog_FUNC_alternating)
    std::vector<std::string> m_alternatingMeasures;
    // counter for the meterSig to be used with meterSigGrpLog_FUNC_alternating
    int m_count = 0;
}; // MeterSigGrp

} // namespace vrv

#endif // __VRV_METERSIGGRP_H__
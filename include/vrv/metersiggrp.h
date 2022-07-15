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

class Measure;
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
    Object *Clone() const override { return new MeterSigGrp(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "MeterSigGrp"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    LinkingInterface *GetLinkingInterface() override { return vrv_cast<LinkingInterface *>(this); }
    const LinkingInterface *GetLinkingInterface() const override { return vrv_cast<const LinkingInterface *>(this); }
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    bool IsSupportedChild(Object *object) override;
    ///@}

    /**
     * Add specified measureId to the m_alternatingMeasures vector
     */
    void AddAlternatingMeasureToVector(const Measure *measure);

    /**
     * Get simplified (i.e. single metersig with count/unit) based on the MeterSigGrp function
     */
    MeterSig *GetSimplifiedMeterSig() const;

    /**
     * Set counter for the alternating meterSigGrp based on the provided measureId
     */
    void SetMeasureBasedCount(const Measure *measure);

    //----------//
    // Functors //
    //----------//
    /**
     * See Object::AlignHorizontally
     */
    int AlignHorizontally(FunctorParams *functorParams) override;

protected:
    /**
     * Filter the flat list and keep only meterSigGrp elements.
     */
    void FilterList(ListOfConstObjects &childList) const override;

private:
    // vector with alternating measures to be used only with meterSigGrpLog_FUNC_alternating
    std::vector<const Measure *> m_alternatingMeasures;
    // counter for the meterSig to be used with meterSigGrpLog_FUNC_alternating
    int m_count = 0;
}; // MeterSigGrp

} // namespace vrv

#endif // __VRV_METERSIGGRP_H__

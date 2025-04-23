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
#include "layerelement.h"

namespace vrv {

class Measure;

//----------------------------------------------------------------------------
// MeterSigGrp
//----------------------------------------------------------------------------

/**
 * This class represents a MEI meterSigGrp.
 * It contains meterSigGrp objects.
 */
class MeterSigGrp : public LayerElement, public ObjectListInterface, public AttBasic, public AttMeterSigGrpLog {
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
    std::string GetClassName() const override { return "meterSigGrp"; }
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    bool IsSupportedChild(ClassId classId) override;
    ///@}

    /** Override the method since check is required */
    bool IsScoreDefElement() const override { return (this->GetParent() && this->GetFirstAncestor(SCOREDEF)); }

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
     * Interface for class functor visitation
     */
    ///@{
    FunctorCode Accept(Functor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(Functor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
    ///@}

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

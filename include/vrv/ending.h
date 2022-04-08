/////////////////////////////////////////////////////////////////////////////
// Name:        ending.h
// Author:      Laurent Pugin
// Created:     14/07/2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ENDING_H__
#define __VRV_ENDING_H__

#include "atts_shared.h"
#include "systemelement.h"
#include "systemmilestone.h"

namespace vrv {

class Measure;

//----------------------------------------------------------------------------
// Ending
//----------------------------------------------------------------------------

/**
 * This class represents a MEI ending.
 * It can be both a container (in score-based MEI) and a milestone (in page-based MEI).
 * It inherits from FloatingElement for spanning drawing features.
 */
class Ending : public SystemElement, public SystemMilestoneInterface, public AttLineRend, public AttNNumberLike {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Ending();
    virtual ~Ending();
    Object *Clone() const override { return new Ending(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Ending"; }
    ///@}

    /**
     * Method for adding allowed content
     */
    bool IsSupportedChild(Object *object) override;

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ConvertToPageBased
     */
    ///@{
    int ConvertToPageBased(FunctorParams *functorParams) override;
    int ConvertToPageBasedEnd(FunctorParams *functorParams) override;
    ///@}

    /**
     * See Object::PrepareMilestones
     */
    int PrepareMilestones(FunctorParams *functorParams) override;

    /**
     * See Object::ResetData
     */
    int ResetData(FunctorParams *functorParams) override;

    /**
     * See Object::CastOffSystems
     */
    int CastOffSystems(FunctorParams *functorParams) override;

    /**
     * See Object::CastOffEncoding
     */
    int CastOffEncoding(FunctorParams *functorParams) override;

    /**
     * See Object::PrepareFloatingGrps
     */
    int PrepareFloatingGrps(FunctorParams *functorParams) override;

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif

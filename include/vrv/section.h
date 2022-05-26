/////////////////////////////////////////////////////////////////////////////
// Name:        section.h
// Author:      Laurent Pugin
// Created:     24/08/2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SECTION_H__
#define __VRV_SECTION_H__

#include "atts_shared.h"
#include "atts_visual.h"
#include "systemelement.h"
#include "systemmilestone.h"

namespace vrv {

class Section;

//----------------------------------------------------------------------------
// Section
//----------------------------------------------------------------------------

/**
 * This class represents a MEI section.
 * It can be both a container (in score-based MEI) and a milestone (in page-based MEI)
 */
class Section : public SystemElement, public SystemMilestoneInterface, public AttNNumberLike, public AttSectionVis {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Section();
    virtual ~Section();
    Object *Clone() const override { return new Section(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Section"; }
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
     * See Object::ConvertToUnCastOffMensural
     */
    int ConvertToUnCastOffMensural(FunctorParams *functorParams) override;

    /**
     * See Object::PrepareMilestones
     */
    int PrepareMilestones(FunctorParams *functorParams) override;

    /**
     * See Object::ResetData
     */
    int ResetData(FunctorParams *functorParams) override;

    /**
     * See Object::AlignMeasures
     */
    int AlignMeasures(FunctorParams *functorParams) override;

    /**
     * See Object::JustifyX
     */
    int JustifyX(FunctorParams *functorParams) override;

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif

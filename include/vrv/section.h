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
     * Interface for class functor visitation
     */
    ///@{
    FunctorCode Accept(MutableFunctor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(MutableFunctor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
    ///@}

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

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif

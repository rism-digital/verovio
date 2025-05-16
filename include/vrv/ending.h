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
class Ending : public SystemElement,
               public SystemMilestoneInterface,
               public AttLabelled,
               public AttLineRend,
               public AttLineRendBase,
               public AttNNumberLike {
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
    std::string GetClassName() const override { return "ending"; }
    ///@}

    /**
     * Method for adding allowed content
     */
    bool IsSupportedChild(ClassId classId) override;

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

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif

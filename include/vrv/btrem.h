/////////////////////////////////////////////////////////////////////////////
// Name:        btrem.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_BTREM_H__
#define __VRV_BTREM_H__

#include "atts_cmn.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// BTrem (bowed tremolo)
//----------------------------------------------------------------------------

/**
 * This class models the MEI <bTrem> element.
 */
class BTrem : public LayerElement,
              public AttNumbered,
              public AttNumberPlacement,
              public AttTremForm,
              public AttTremMeasured {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    BTrem();
    virtual ~BTrem();
    Object *Clone() const override { return new BTrem(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "bTrem"; }
    ///@}

    /**
     * Add an element (a note or a chord) to a fTrem.
     * Only Note or Chord elements will be actually added to the fTrem.
     */
    bool IsSupportedChild(ClassId classId) override;

    /**
     * Get stem mod if encoded explicitly, or determine based on duration of bTrem and underlying elements
     */
    data_STEMMODIFIER GetDrawingStemMod() const override;

    /**
     * Calculate the duration of an individual note in a measured tremolo
     */
    data_DURATION CalcIndividualNoteDuration() const;

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

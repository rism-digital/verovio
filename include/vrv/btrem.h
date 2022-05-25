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
              public AttBTremLog,
              public AttNumbered,
              public AttNumberPlacement,
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
    std::string GetClassName() const override { return "BTrem"; }
    ///@}

    /**
     * Add an element (a note or a chord) to a fTrem.
     * Only Note or Chord elements will be actually added to the fTrem.
     */
    bool IsSupportedChild(Object *object) override;

    /**
     * Get stem mod if encoded explicitly, or determine based on duration of bTrem and underlying elements
     */
    data_STEMMODIFIER GetDrawingStemMod() const override;

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::GenerateMIDI
     */
    int GenerateMIDI(FunctorParams *functorParams) override;

private:
    /**
     * Calculate the duration of an individual note in a measured tremolo
     */
    data_DURATION CalcIndividualNoteDuration() const;

public:
    //
private:
};

} // namespace vrv

#endif

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
class BTrem : public LayerElement, public AttBTremLog, public AttTremMeasured {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    BTrem();
    virtual ~BTrem();
    virtual Object *Clone() const { return new BTrem(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "BTrem"; }
    ///@}

    /**
     * Add an element (a note or a chord) to a fTrem.
     * Only Note or Chord elements will be actually added to the fTrem.
     */
    virtual bool IsSupportedChild(Object *object);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::GenerateMIDI
     */
    virtual int GenerateMIDI(FunctorParams *functorParams);

private:
    /**
     * Calculate the duration of an individual note in a measured tremolo
     */
    data_DURATION CalcIndividualNoteDuration();

public:
    //
private:
};

} // namespace vrv

#endif

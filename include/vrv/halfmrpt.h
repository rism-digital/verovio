/////////////////////////////////////////////////////////////////////////////
// Name:        halfmrpt.h
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_HALFMRPT_H__
#define __VRV_HALFMRPT_H__

#include "atts_cmn.h"
#include "atts_shared.h"
#include "atts_visual.h"
#include "beam.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// HalfmRpt (half-measure repeat)
//----------------------------------------------------------------------------

/**
 * This class models the MEI <halfmRpt> element.
 */
class HalfmRpt : public LayerElement, public AttColor {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    HalfmRpt();
    virtual ~HalfmRpt();
    Object *Clone() const override { return new HalfmRpt(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "HalfmRpt"; }
    ///@}

    /** Override the method since alignment is required */
    bool HasToBeAligned() const override { return true; }

    //----------//
    // Functors //
    //----------//

    /**
     * @name See Object::GenerateMIDI
     */
    ///@{
    int GenerateMIDI(FunctorParams *functorParams) override;
    ///@}

    /**
     * See Object::PrepareRpt
     */
    int PrepareRpt(FunctorParams *functorParams) override;

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif

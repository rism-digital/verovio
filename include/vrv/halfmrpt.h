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
    virtual Object *Clone() const { return new HalfmRpt(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "HalfmRpt"; }
    virtual ClassId GetClassId() const { return HALFMRPT; }
    ///@}

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

    //----------//
    // Functors //
    //----------//

    /**
     * @name See Object::GenerateMIDI
     */
    ///@{
    virtual int GenerateMIDI(FunctorParams *functorParams);
    ///@}

    /**
     * See Object::PrepareRpt
     */
    virtual int PrepareRpt(FunctorParams *functorParams);

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif

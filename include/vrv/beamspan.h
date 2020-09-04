/////////////////////////////////////////////////////////////////////////////
// Name:        beamspan.h
// Author:      Andriy Makarchuk
// Created:     2020
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_BEAMSPAN_H__
#define __VRV_BEAMSPAN_H__

#include "atts_cmn.h"
#include "controlelement.h"
#include "drawinginterface.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// BeamSpan
//----------------------------------------------------------------------------

/**
 * This class models the MEI <beamSpan> element.
 */
class BeamSpan : public ControlElement,
                 public TimeSpanningInterface,
                 public BeamDrawingInterface,
                 public AttColor,
                 public AttBeamedWith,
                 public AttBeamRend {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    BeamSpan();
    virtual ~BeamSpan();
    virtual Object *Clone() const { return new BeamSpan(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "BeamSpan"; }
    virtual ClassId GetClassId() const { return BEAMSPAN; }
    ///@}

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif //__VRV_BEAMSPAN_H__
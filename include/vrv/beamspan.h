/////////////////////////////////////////////////////////////////////////////
// Name:        beamspan.h
// Author:      Andriy Makarchuk
// Created:     2020
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_BEAMSPAN_H__
#define __VRV_BEAMSPAN_H__

#include "atts_cmn.h"
#include "beam.h"
#include "controlelement.h"
#include "drawinginterface.h"
#include "plistinterface.h"
#include "timeinterface.h"

namespace vrv {

class Layer;
class Staff;

//----------------------------------------------------------------------------
// BeamSpan
//----------------------------------------------------------------------------

/**
 * This class models the MEI <beamSpan> element.
 */
class BeamSpan : public ControlElement,
                 public BeamDrawingInterface,
                 public PlistInterface,
                 public TimeSpanningInterface,
                 public AttBeamedWith,
                 public AttBeamRend,
                 public AttColor {
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

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual PlistInterface *GetPlistInterface() { return dynamic_cast<PlistInterface *>(this); }
    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    virtual TimeSpanningInterface *GetTimeSpanningInterface() { return dynamic_cast<TimeSpanningInterface *>(this); }
    ////@}

    ArrayOfObjects GetBeamSpanElementList(Layer* layer, Staff* staff);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::CalcStem
     */
    virtual int CalcStem(FunctorParams *functorParams);

    /**
     * See Object::ResolveBeamSpanElements
     */
    virtual int ResolveBeamSpanElements(FunctorParams *);

private:
    //
public:
    //
    BeamSegment m_beamSegment;

private:
    //
    ArrayOfObjects m_beamedElements;
};

} // namespace vrv

#endif //__VRV_BEAMSPAN_H__
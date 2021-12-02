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
private:
    using SpanIndexVector = std::vector<std::pair<vrv::ArrayOfObjects::iterator, Object *> >;
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    BeamSpan();
    virtual ~BeamSpan();
    Object *Clone() const override { return new BeamSpan(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "BeamSpan"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    PlistInterface *GetPlistInterface() override { return dynamic_cast<PlistInterface *>(this); }
    TimePointInterface *GetTimePointInterface() override { return dynamic_cast<TimePointInterface *>(this); }
    TimeSpanningInterface *GetTimeSpanningInterface() override { return dynamic_cast<TimeSpanningInterface *>(this); }
    ////@}

    // Helper to get element list for the beamSpan - elements are acquired from all layerElements that are located
    // in between start and end of the beamSpan
    ArrayOfObjects GetBeamSpanElementList(Layer* layer, Staff* staff);

    /**
     * @name Initialize and cleam beam segments
     */
    ///@{
    void InitBeamSegments();
    void ClearBeamSegments();
    ////@}

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::CalcStem
     */
    int CalcStem(FunctorParams *functorParams) override;

    /**
     * See Object::ResolveBeamSpanElements
     */
    int ResolveBeamSpanElements(FunctorParams *) override;

    /**
     * See Object::ResolveSpanningBeamSpans
     */
    int ResolveSpanningBeamSpans(FunctorParams *) override;

private:
    // Helper for breaking one big spanning beamSpan into smaller beamSpans
    bool AddSpanningSegment(Doc *doc, const SpanIndexVector &elements, int index, bool newSegment = true);

public:
    //
    std::vector<BeamSegment *> m_beamSegments;

private:
    //
    ArrayOfObjects m_beamedElements;
};

} // namespace vrv

#endif //__VRV_BEAMSPAN_H__
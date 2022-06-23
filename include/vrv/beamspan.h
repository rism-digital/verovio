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
class System;

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
    using SpanIndexVector = std::vector<std::pair<vrv::ArrayOfObjects::iterator, Object *>>;

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
    PlistInterface *GetPlistInterface() override { return vrv_cast<PlistInterface *>(this); }
    const PlistInterface *GetPlistInterface() const override { return vrv_cast<const PlistInterface *>(this); }
    TimePointInterface *GetTimePointInterface() override { return vrv_cast<TimePointInterface *>(this); }
    const TimePointInterface *GetTimePointInterface() const override
    {
        return vrv_cast<const TimePointInterface *>(this);
    }
    TimeSpanningInterface *GetTimeSpanningInterface() override { return vrv_cast<TimeSpanningInterface *>(this); }
    const TimeSpanningInterface *GetTimeSpanningInterface() const override
    {
        return vrv_cast<const TimeSpanningInterface *>(this);
    }
    ////@}

    /**
     * @name Initialize and clear beam segments
     */
    ///@{
    void InitBeamSegments();
    void ClearBeamSegments();
    ////@}

    /**
     * Access the beam segments
     */
    ///@{
    BeamSpanSegment *GetSegmentForSystem(const System *system);
    const BeamSpanSegment *GetSegmentForSystem(const System *system) const;
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ResetHorizontalAlignment
     */
    int ResetHorizontalAlignment(FunctorParams *functorParams) override;

    /**
     * See Object::CalcStem
     */
    int CalcStem(FunctorParams *functorParams) override;

    /**
     * See Object::PrepareBeamSpanElements
     */
    int PrepareBeamSpanElements(FunctorParams *) override;

    /**
     * See Object::CalcSpanningBeamSpans
     */
    int CalcSpanningBeamSpans(FunctorParams *) override;

private:
    // Helper for breaking one big spanning beamSpan into smaller beamSpans
    bool AddSpanningSegment(const Doc *doc, const SpanIndexVector &elements, int index, bool newSegment = true);

    // Helper to get element list for the beamSpan - elements are acquired from all layerElements that are located
    // in between start and end of the beamSpan
    ArrayOfObjects GetBeamSpanElementList(Layer *layer, const Staff *staff);

public:
    //
private:
    /**
     * Array of beam segments
     */
    std::vector<BeamSpanSegment *> m_beamSegments;
    /**
     * Array of beamed elements
     */
    ArrayOfObjects m_beamedElements;
};

} // namespace vrv

#endif //__VRV_BEAMSPAN_H__

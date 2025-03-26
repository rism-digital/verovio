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
                 public AttBeamRend {
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
    std::string GetClassName() const override { return "beamSpan"; }
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
    BeamSpanSegment *GetSegment(int index) { return m_beamSegments.at(index); }
    const BeamSpanSegment *GetSegment(int index) const { return m_beamSegments.at(index); }
    BeamSpanSegment *GetSegmentForSystem(const System *system);
    const BeamSpanSegment *GetSegmentForSystem(const System *system) const;
    ///@}

    /**
     * Getter and setter for the beamed elements
     */
    ///@{
    const ArrayOfObjects &GetBeamedElements() const { return m_beamedElements; }
    void ResetBeamedElements() { m_beamedElements.clear(); }
    void SetBeamedElements(const ArrayOfObjects &beamedElements) { m_beamedElements = beamedElements; }
    ///@}

    /**
     * Break one big spanning beamSpan into smaller beamSpans
     */
    ///@{
    using SpanIndexVector = std::vector<std::pair<vrv::ArrayOfObjects::const_iterator, Object *>>;
    bool AddSpanningSegment(const Doc *doc, const SpanIndexVector &elements, int index, bool newSegment = true);
    ///@}

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

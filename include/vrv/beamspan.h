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
    virtual int CalcStem(FunctorParams *functorParams);

    /**
     * See Object::ResolveBeamSpanElements
     */
    virtual int ResolveBeamSpanElements(FunctorParams *);

    /**
     * See Object::ResolveSpanningBeamSpans
     */
    virtual int ResolveSpanningBeamSpans(FunctorParams *);

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
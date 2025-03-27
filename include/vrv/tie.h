/////////////////////////////////////////////////////////////////////////////
// Name:        tie.h
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TIE_H__
#define __VRV_TIE_H__

#include "controlelement.h"
#include "timeinterface.h"

namespace vrv {

class Chord;
class Layer;
class Note;

//----------------------------------------------------------------------------
// Tie
//----------------------------------------------------------------------------

/**
 * This class models the MEI <tie> element.
 */
class Tie : public ControlElement, public TimeSpanningInterface, public AttCurvature, public AttLineRendBase {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Tie();
    Tie(ClassId classId);
    virtual ~Tie();
    Object *Clone() const override { return new Tie(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "tie"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
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
    ///@}

    virtual bool CalculatePosition(
        const Doc *doc, const Staff *staff, int x1, int x2, int spanningType, Point bezier[4]);

    /**
     * Calculate starting/ending point for the ties in the chords with adjacent notes
     */
    int CalculateAdjacentChordXOffset(const Doc *doc, const Staff *staff, const Chord *parentChord, const Note *note,
        curvature_CURVEDIR drawingCurveDir, int initialX, bool isStartPoint) const;

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
    // Update tie positioning based overlaps with accidentals in cases with enharmonic ties
    bool AdjustEnharmonicTies(const Doc *doc, const FloatingCurvePositioner *curve, Point bezier[4],
        const Note *startNote, const Note *endNote, curvature_CURVEDIR drawingCurveDir) const;

    // Calculate the initial X position and return true if the tie endpoints should be adjusted vertically
    bool CalculateXPosition(const Doc *doc, const Staff *staff, const Chord *startParentChord,
        const Chord *endParentChord, int spanningType, bool isOuterChordNote, Point &startPoint, Point &endPoint,
        curvature_CURVEDIR drawingCurveDir) const;

    // Helper function to get preferred curve direction based on the number of conditions (like note direction, position
    // on the staff, etc.)
    curvature_CURVEDIR GetPreferredCurveDirection(const Layer *layer, const Note *note, const Chord *startParentChord,
        data_STEMDIRECTION noteStemDir, bool isAboveStaffCenter) const;

    // Update tie positioning based on the overlaps with possible layerElements such as dots/flags
    void UpdateTiePositioning(const FloatingCurvePositioner *curve, Point bezier[4], const LayerElement *durElement,
        const Note *startNote, int height, curvature_CURVEDIR drawingCurveDir) const;

public:
    //
private:
};

} // namespace vrv

#endif

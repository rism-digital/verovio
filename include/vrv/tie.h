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
class Tie : public ControlElement,
            public TimeSpanningInterface,
            public AttColor,
            public AttCurvature,
            public AttCurveRend {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Tie();
    Tie(ClassId classId);
    Tie(ClassId classId, const std::string &classIdStr);
    virtual ~Tie();
    Object *Clone() const override { return new Tie(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Tie"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    TimePointInterface *GetTimePointInterface() override { return dynamic_cast<TimePointInterface *>(this); }
    TimeSpanningInterface *GetTimeSpanningInterface() override { return dynamic_cast<TimeSpanningInterface *>(this); }
    ///@}

    virtual bool CalculatePosition(Doc *doc, Staff *staff, int x1, int x2, int spanningType, Point bezier[4]);

    /**
     * Calculate starting/ending point for the ties in the chords with adjacent notes
     */
    int CalculateAdjacentChordXOffset(Doc *doc, Staff *staff, Chord *parentChord, Note *note,
        curvature_CURVEDIR drawingCurveDir, int initialX, bool isStartPoint);

    //----------//
    // Functors //
    //----------//

    /**
     * see Object::ResolveMIDITies
     */
    ///@{
    int ResolveMIDITies(FunctorParams *functorParams) override;
    ///@}

private:
    // Update tie positioning based overlaps with accidentals in cases with enharmonic ties
    bool AdjustEnharmonicTies(Doc *doc, FloatingCurvePositioner *curve, Point bezier[4], Note *startNote, Note *endNote,
        curvature_CURVEDIR drawingCurveDir);

    // Calculate initial position X position and return stem direction of the startNote
    void CalculateXPosition(Doc *doc, Staff *staff, Chord *startParentChord, Chord *endParentChord, int spanningType,
        bool isOuterChordNote, Point &startPoint, Point &endPoint, curvature_CURVEDIR drawingCurveDir);

    // Helper function to get preferred curve direction based on the number of conditions (like note direction, position
    // on the staff, etc.)
    curvature_CURVEDIR GetPreferredCurveDirection(
        Layer *layer, Note *note, Chord *startParentChord, data_STEMDIRECTION noteStemDir, bool isAboveStaffCenter);

    // Update tie positioning based on the overlaps with possible layerElements such as dots/flags
    void UpdateTiePositioning(FloatingCurvePositioner *curve, Point bezier[4], LayerElement *durElement,
        Note *startNote, int height, curvature_CURVEDIR drawingCurveDir);

public:
    //
private:
};

} // namespace vrv

#endif

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
    virtual Object *Clone() const { return new Tie(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Tie"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    virtual TimeSpanningInterface *GetTimeSpanningInterface() { return dynamic_cast<TimeSpanningInterface *>(this); }
    ///@}

    virtual bool CalculatePosition(Doc *doc, Staff *staff, int x1, int x2, int spanningType, Point bezier[4]);

    //----------//
    // Functors //
    //----------//

    /**
     * see Object::ResolveMIDITies
     */
    ///@{
    virtual int ResolveMIDITies(FunctorParams *functorParams);
    ///@}

private:
    // Calculate initial position X position and return stem direction of the startNote
    void CalculateXPosition(Doc *doc, Staff *staff, Chord *startParentChord, Chord *endParentChord, int spanningType,
        bool isOuterChordNote, Point &startPoint, Point &endPoint);

    // Helper function to get preferred curve direction based on the number of conditions (like note direction, position
    // on the staff, etc.)
    curvature_CURVEDIR GetPreferredCurveDirection(
        Layer *layer, Note *note, Chord *startParentChord, data_STEMDIRECTION noteStemDir, bool isAboveStaffCenter);

    // Update tie positioning based on the overlaps with posible layerElements such as dots/flags
    void UpdateTiePositioning(FloatingCurvePositioner *curve, Point bezier[4], LayerElement *durElement,
        Note *startNote, int height, curvature_CURVEDIR drawingCurveDir);

public:
    //
private:
};

} // namespace vrv

#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        pitchinterface.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PITCH_INTERFACE_H__
#define __VRV_PITCH_INTERFACE_H__

#include "atts_gestural.h"
#include "atts_shared.h"

namespace vrv {

class Clef;
class Layer;

//----------------------------------------------------------------------------
// PitchInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for elements with pitch, such as notes and neumes.
 * It is not an abstract class but should not be instanciated directly.
 */
class PitchInterface : public Interface, public AttNoteGes, public AttOctave, public AttPitch {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method resets all attribute classes
     */
    ///@{
    PitchInterface();
    virtual ~PitchInterface();
    virtual void Reset();
    virtual InterfaceId IsInterface() { return INTERFACE_PITCH; }
    ///@}

    /**
     * Interface comparison operator.
     * Checks if the LayerElement has a PitchInterface and compares attributes
     */
    bool HasIdenticalPitchInterface(PitchInterface *otherPitchInterface);

    /**
     * Adjust the pname and the octave for values outside the range.
     * To be tested
     */
    static void AdjustPname(int &pname, int &oct);

    /**
     * Shift pname and octave by a certain number of steps.
     */
    void AdjustPitchByOffset(int pitchOffset);

    /**
     * Get steps between calling object and parameter.
     * Returns calling pitch minus parameter pitch.
     */
    int PitchDifferenceTo(PitchInterface *pi);

    /**
     * adjust the pitch value so that it stays in the same x,y position
     * given it's new and old clefs
     */
    void AdjustPitchForNewClef(Clef *oldClef, Clef *newClef);

    /**
     * Calculate the loc for a note, rest or chord considering its position in the layer.
     * By default for chord takes the top note, but the bottom note otherwise.
     * E.g., return 0 for and C4 with clef C1, -2 with clef G2.
     */
    static int CalcLoc(LayerElement *element, Layer *layer, LayerElement *crossStaffElement, bool topChordNote = true);

    /**
     * Calculate the loc for a pitch and octave and considerting the clef loc offset.
     * E.g., return 0 for and C4 with clef C1, -2 with clef G2.
     */
    static int CalcLoc(data_PITCHNAME pname, int oct, int clefLocOffset);

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif

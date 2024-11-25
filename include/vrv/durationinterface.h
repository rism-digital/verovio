/////////////////////////////////////////////////////////////////////////////
// Name:        durationinterface.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_DURATION_INTERFACE_H__
#define __VRV_DURATION_INTERFACE_H__

#include "atts_cmn.h"
#include "atts_gestural.h"
#include "atts_mensural.h"
#include "atts_shared.h"
#include "fraction.h"
#include "interface.h"

namespace vrv {

class Mensur;
class Object;

//----------------------------------------------------------------------------
// DurationInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for elements with duration, such as notes and rests.
 * It is not an abstract class but should not be instanciated directly.
 */
class DurationInterface : public Interface,
                          public AttAugmentDots,
                          public AttBeamSecondary,
                          public AttDurationGes,
                          public AttDurationLog,
                          public AttDurationQuality,
                          public AttDurationRatio,
                          public AttFermataPresent,
                          public AttStaffIdent {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method resets all attribute classes
     */
    ///@{
    DurationInterface();
    virtual ~DurationInterface();
    void Reset() override;
    InterfaceId IsInterface() const override { return INTERFACE_DURATION; }
    ///@}

    /**
     * @name Set and get the default duration
     */
    ///@{
    void SetDurDefault(data_DURATION dur) { m_durDefault = dur; }
    data_DURATION GetDurDefault() const { return m_durDefault; }
    ///@}

    /**
     * Returns the duration (in Fraction) for the element.
     * It returns 0/1 for grace notes.
     * Careful: this method is not overriding LayerElement::GetAlignmentDuration since
     * LayerElement and DurationInterface have no inheritance link.
     */
    Fraction GetInterfaceAlignmentDuration(int num, int numBase) const;

    /**
     * Returns the duration (in Fraction) for the element for mensural notation
     * The level of equality is given in equivalence (brevis, semibrevis or minima)
     */
    Fraction GetInterfaceAlignmentMensuralDuration(
        int num, int numBase, const Mensur *currentMensur, data_DURATION equivalence) const;

    /**
     * @name Return the duration (in Fraction) for the corresponding level of equivalence
     */
    ///@{
    Fraction DurationWithBrevisEquivalence(
        int num, int numBase, const Mensur *currentMensur, data_DURATION noteDur) const;
    Fraction DurationWithSemibrevisEquivalence(
        int num, int numBase, const Mensur *currentMensur, data_DURATION noteDur) const;
    Fraction DurationWithMinimaEquivalence(
        int num, int numBase, const Mensur *currentMensur, data_DURATION noteDur) const;
    ///@}

    /**
     * Return true if the note or rest is the first of a beam.
     */
    bool IsFirstInBeam(const LayerElement *noteOrRest) const;

    /**
     * Return true if the note or rest is the last of a beam.
     */
    bool IsLastInBeam(const LayerElement *noteOrRest) const;

    /**
     * @name Return the actual (gestural) duration of the note, for both CMN and mensural durations
     * See data_DURATION
     * For Mensural, it is the same (DURATION_2 == DURATION_minima)
     */
    ///@{
    data_DURATION GetActualDur() const;
    data_DURATION GetActualDurGes() const;
    ///@}

    /**
     * If the element is part of a chord, return the chord actual duration, otherwise the note actual duration.
     * Since we need to check what the element is, we need to pass it as parameter.
     */
    data_DURATION GetNoteOrChordDur(const LayerElement *element) const;

    /**
     * Return true if the value is a mensural (DURATION_longa, brevis, etc.)
     */
    bool IsMensuralDur() const;

    /**
     * Interface comparison operator.
     * Check if the LayerElement has a DurationInterface and compare attributes
     */
    bool HasIdenticalDurationInterface(const DurationInterface *otherDurationInterface) const;

    /**
     * MIDI timing information
     */
    ///@{
    void SetScoreTimeOnset(Fraction scoreTime);
    void SetRealTimeOnsetSeconds(double timeInSeconds);
    void SetScoreTimeOffset(Fraction scoreTime);
    void SetRealTimeOffsetSeconds(double timeInSeconds);
    void SetScoreTimeTiedDuration(Fraction timeInSeconds);
    Fraction GetScoreTimeOnset() const;
    double GetRealTimeOnsetMilliseconds() const;
    Fraction GetScoreTimeOffset() const;
    Fraction GetScoreTimeTiedDuration() const;
    double GetRealTimeOffsetMilliseconds() const;
    Fraction GetScoreTimeDuration() const;
    ///@}

    //-----------------//
    // Pseudo functors //
    //-----------------//

    /**
     * We have functor code in the interface for avoiding code duplication in each implementation class.
     * Since we are in an interface, we need to pass the object (implementation) to
     * the pseudo functor method.
     */

private:
    /**
     * Calculate the actual duration => translate mensural values
     */
    data_DURATION CalcActualDur(data_DURATION dur) const;

public:
    //
private:
    /**
     * The score-time onset of the note in the measure (duration from the start of measure in
     * quarter notes).
     */
    Fraction m_scoreTimeOnset;

    /**
     * The score-time off-time of the note in the measure (duration from the start of the measure
     * in quarter notes).  This is the duration of the printed note.  If the note is the start of
     * a tied group, the score time of the tied group is this variable plus m_scoreTimeTiedDuration.
     * If this note is a secondary note in a tied group, then this value is the score time end
     * of the printed note, and the m_scoreTimeTiedDuration is -1.0 to indicate that it should not
     * be exported when creating a MIDI file.
     */
    Fraction m_scoreTimeOffset;

    /**
     * The time in milliseconds since the start of the measure element that contains the note.
     */
    double m_realTimeOnsetMilliseconds;

    /**
     * The time in milliseconds since the start of the measure element to end of printed note.
     * The real-time duration of a tied group is not currently tracked (this gets complicated
     * if there is a tempo change during a note sustain, which is currently not supported).
     */
    double m_realTimeOffsetMilliseconds;

    /**
     * If the note is the first in a tied group, then m_scoreTimeTiedDuration contains the
     * score-time duration (in quarter notes) of all tied notes in the group after this note.
     * If the note is a secondary note in a tied group, then this variable is set to -1.0 to
     * indicate that it should not be written to MIDI output.
     */
    Fraction m_scoreTimeTiedDuration;

    /**
     * The default duration: extracted from scoreDef/staffDef and used when no duration attribute is given
     */
    data_DURATION m_durDefault;
};

} // namespace vrv

#endif

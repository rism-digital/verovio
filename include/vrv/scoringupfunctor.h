/////////////////////////////////////////////////////////////////////////////
// Name:        scoringupfunctor.h
// Author:      Martha E. M. Thomae Elias
// Created:     2024
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////
///
#ifndef __VRV_SCORINGUPFUNCTOR_H__
#define __VRV_SCORINGUPFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// ScoringUpFunctor
//----------------------------------------------------------------------------

/**
 * This class...
 */
class ScoringUpFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    ScoringUpFunctor();
    virtual ~ScoringUpFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Methods
     */
    /**
     * @name: Divide the notes of a voice into sequences to be processed individualy
     */
    ///@{
    void ProcessPerfectMensurPassage(
        const ArrayOfElementDurPairs &m_dursInVoiceWithSameMensur, data_DURATION noteLevel);
    std::vector<ArrayOfElementDurPairs> SubdivideIntoBoundedSequences(
        const ArrayOfElementDurPairs &dursInVoiceSameMensur, data_DURATION boundUnit);
    void ProcessBoundedSequences(const std::vector<ArrayOfElementDurPairs> &listOfSequences, data_DURATION boundUnit);
    void ProcessBoundedSequences(const ArrayOfElementDurPairs &sequence, data_DURATION boundUnit);
    ArrayOfElementDurPairs GetBoundedNotes(const ArrayOfElementDurPairs &sequence, data_DURATION boundUnit);
    ///@}

    /**
     * @name: Apply the principles of imperfection and alteration (find @dur.quality of notes: perfecta / imperfecta /
     * altera) for sequences with or without dots of division
     */
    ///@{
    bool EvalDotOfDiv(const ArrayOfElementDurPairs &middleSeq, const ArrayOfElementDurPairs &sequence, int dotInd,
        data_DURATION unit);
    void FindDurQuals(const ArrayOfElementDurPairs &middleSeq, double valueInUnit, data_DURATION boundUnit);
    ///@}

    /**
     * @name: Find the duration value of the note in minims or another given unit
     */
    ///@{
    double GetDurNumberValue(
        const std::pair<LayerElement *, data_DURATION> &elementDurPair, bool followedByDot, LayerElement *nextElement);
    double GetValueInMinims(const ArrayOfElementDurPairs &middleSeq);
    double GetValueInUnit(double valueInMinims, data_DURATION unit);
    ///@}

    /**
     * @name Apply the modifications of imperfection and alteration or leaves the notes with their default perfect value
     */
    ///@{
    Note *ImperfectionAPP(const ArrayOfElementDurPairs &sequence, data_DURATION boundUnit);
    Note *ImperfectionAPA(const ArrayOfElementDurPairs &sequence, data_DURATION boundUnit);
    Note *Alteration(const ArrayOfElementDurPairs &sequence, data_DURATION boundUnit);
    bool LeavePerfect(const ArrayOfElementDurPairs &sequence, data_DURATION boundUnit);
    void ApplyAugmentationsAndPerfections();
    ///@}

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitLayerEnd(Layer *layer) override;
    FunctorCode VisitLayerElement(LayerElement *layerElement) override;
    ///@}
    ///

protected:
    //
private:
    //

public:
    //

private:
    // The current score time in the measure (incremented by each element)
    double m_currentScoreTime;
    // The current Mensur
    Mensur *m_currentMensur;
    int m_modusMaior;
    int m_modusMinor;
    int m_tempus;
    int m_prolatio;
    // The data of each voice in the form of an array of pairs made up of 'mensuration' {m_modusMaior, m_modusMinor,
    // m_tempus, m_prolatio} and the notes/rests/dots in that voice that follows that mensuration
    // (m_dursInVoiceWithSameMensur)
    std::vector<std::pair<std::tuple<int, int, int, int>, ArrayOfElementDurPairs>> m_voiceData;
    // Vector of pairs of elements (that are notes, rests, or dots) and their durations
    ArrayOfElementDurPairs m_dursInVoiceWithSameMensur;
    // Vector of vectors of pairs of elements (that are notes, rests, or dots) and their durations
    std::vector<ArrayOfElementDurPairs> m_listOfSequences;
    // List of pairs made of the form {note, following_dot} for notes followed by dots of augmentation
    std::list<std::pair<Note *, Dot *>> m_listOfAugNotesDotsPairs;
    // List of pairs made of the form {note, following_dot} for notes followed by dots of perfection
    std::list<std::pair<Note *, Dot *>> m_listOfPerfNotesDotsPairs;
};

} // namespace vrv

#endif /* scoringupfunctor_h */

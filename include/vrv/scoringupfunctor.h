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
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Methods
     */
    /**
     * @name: Divide the notes of a voice into sequences to be processed individualy
     */
    ///@{
    std::vector<ArrayOfElementDurPairs> SubdivideIntoBoundedSequences(
        const ArrayOfElementDurPairs &dursInVoiceSameMensur);
    ///@}

    /**
     * @name: Find @dur.quality of notes (perfecta / imperfecta / altera)
     */
    ///@{
    void ProcessBoundedSequences(const std::vector<ArrayOfElementDurPairs> &listOfSequences);
    void ProcessBoundedSequences(const ArrayOfElementDurPairs &sequence);
    ArrayOfElementDurPairs GetBoundedNotes(const ArrayOfElementDurPairs &sequence);
    bool EvalDotOfDiv(const ArrayOfElementDurPairs &middleSeq, const ArrayOfElementDurPairs &sequence, int dotInd);
    void FindDurQuals(const ArrayOfElementDurPairs &middleSeq, double valueInUnit);
    ///@}

    /**
     * @name: Find the duration value of the note in minims
     */
    ///@{
    double GetValueInUnit(double valueInMinims, data_DURATION unit);
    double GetValueInMinims(const ArrayOfElementDurPairs &middleSeq);
    double GetDurNumberValue(
        const std::pair<LayerElement *, data_DURATION> &elementDurPair, bool followedByDot, LayerElement *nextElement);
    ///@}

    /**
     * @name Apply principles of imperfection and alteration
     */
    ///@{
    Note *ImperfectionAPP(const ArrayOfElementDurPairs &sequence);
    Note *ImperfectionAPA(const ArrayOfElementDurPairs &sequence);
    Note *Alteration(const ArrayOfElementDurPairs &sequence);
    bool LeavePerfect(const ArrayOfElementDurPairs &sequence);
    ///@}

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitLayer(Layer *layer) override;
    FunctorCode VisitLayerElement(LayerElement *layerElement) override;
    ///@}
    ///

protected:
    //
private:
    int m_modusMaior;
    int m_modusMinor;
    int m_tempus;
    int m_prolatio;
    ArrayOfElementDurPairs m_dursInVoiceSameMensur;
    std::vector<ArrayOfElementDurPairs> m_listOfSequences;

public:
    //

private:
    // The current score time in the measure (incremented by each element)
    double m_currentScoreTime;
    // The current Mensur
    Mensur *m_currentMensur;
    // The current notation type
    data_NOTATIONTYPE m_notationType;
};

} // namespace vrv

#endif /* scoringupfunctor_h */

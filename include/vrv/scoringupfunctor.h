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
    std::vector<ArrayOfElementDurPairs> SubdivideSeq(const ArrayOfElementDurPairs &dursInVoiceSameMensur);
    ///@}

    /**
     * @name: Find @dur.quality of notes (perfecta / imperfecta / altera)
     */
    ///@{
    void FindDurQuals(const std::vector<ArrayOfElementDurPairs> &listOfSequences);
    void FindDurQuals(const ArrayOfElementDurPairs &sequence);
    ///@}

    /**
     * @name: Find the duration value of the note in minims
     */
    ///@{
    double GetDurNumberValue(
        std::pair<LayerElement *, data_DURATION> elementDurPair, bool followedByDot, LayerElement *nextElement);
    ///@}

    /**
     * @name Apply principles of imperfection and alteration
     */
    ///@{
    bool ImperfectionAPP(const ArrayOfElementDurPairs &sequence);
    bool ImperfectionAPA(const ArrayOfElementDurPairs &sequence);
    bool Alteration(const ArrayOfElementDurPairs &sequence);
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
    //
public:
    std::map<std::string, int> m_mensuration;
    ArrayOfElementDurPairs m_dursInVoiceSameMensur;
    std::vector<ArrayOfElementDurPairs> m_listOfSequences;

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

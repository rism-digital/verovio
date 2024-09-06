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
     * Other methods
     */
    std::vector<std::vector<std::pair<LayerElement*, data_DURATION>>> SubdivideSeq(std::vector<std::pair<LayerElement*, data_DURATION>> dursInVoiceSameMensur);
    void FindDurQuals(std::vector<std::vector<std::pair<LayerElement*, data_DURATION>>> listOfSequences);
    void FindDurQuals(std::vector<std::pair<LayerElement*, data_DURATION>> sequence);
    double GetDurNumberValue(std::pair<LayerElement*, data_DURATION> elementDurPair);
    bool ImperfectionAPP(std::vector<std::pair<LayerElement*, data_DURATION>> sequence);
    bool ImperfectionAPA(std::vector<std::pair<LayerElement*, data_DURATION>> sequence);
    bool Alteration(std::vector<std::pair<LayerElement*, data_DURATION>> sequence);
    bool LeavePerfect(std::vector<std::pair<LayerElement*, data_DURATION>> sequence);
    
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
    std::map<std::string, int> mensuration;
    std::vector<std::pair<LayerElement*, data_DURATION>> dursInVoiceSameMensur = {};
    std::vector<std::vector<std::pair<LayerElement*, data_DURATION>>> listOfSequences;
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

/////////////////////////////////////////////////////////////////////////////
// Name:        scoringupfunctor.cpp
// Author:      Martha E. M. Thomae Elias
// Created:     2024
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////
///
#include "scoringupfunctor.h"

//----------------------------------------------------------------------------

#include "layer.h"
#include "page.h"
#include "note.h"
#include "rest.h"
#include "staff.h"
#include "system.h"
#include "vrv.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// ScoringUpFunctor
//----------------------------------------------------------------------------

std::vector<std::pair<std::string, data_DURATION>> dursInVoiceSameMensur = {};
//std::vector<data_DURATION> dursInVoiceSameMensur = {};
void subdivideSeq(std::vector<std::pair<std::string, data_DURATION>> dursInVoiceSameMensur);

ScoringUpFunctor::ScoringUpFunctor() : Functor()
{
    m_currentScoreTime = 0.0;
    m_currentMensur = NULL;
    m_notationType = NOTATIONTYPE_cmn;
}

FunctorCode ScoringUpFunctor::VisitLayer(Layer *layer)
{
    m_currentScoreTime = 0.0;
    m_currentMensur = layer->GetCurrentMensur();
    if (!dursInVoiceSameMensur.empty()){
        subdivideSeq(dursInVoiceSameMensur);
        dursInVoiceSameMensur = {}; //restart for next voice (layer)
    }
    return FUNCTOR_CONTINUE;
}

FunctorCode ScoringUpFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (layerElement->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    LayerElement *element = layerElement->ThisOrSameasLink();

    if (element->Is(REST) || element->Is(NOTE)) {
        data_DURATION dur;
        std::string xmlid;
        if (element->Is(NOTE)) {
            Note *note = vrv_cast<Note *>(element);
            assert(note);
            xmlid = note->GetID();
            dur = note->GetDur();
            //note->SetDurQuality(DURQUALITY_mensural_imperfecta);
        } else {
            Rest *rest = vrv_cast<Rest *>(element);
            assert(rest);
            xmlid = rest->GetID();
            dur = rest->GetDur();
        } dursInVoiceSameMensur.insert(dursInVoiceSameMensur.end(), {xmlid, dur});
    } else if (element->Is(MENSUR)) {
        this->m_currentMensur = vrv_cast<Mensur *>(layerElement);
    }return FUNCTOR_CONTINUE;
}

void subdivideSeq(std::vector<std::pair<std::string, data_DURATION>> dursInVoiceSameMensur)
{
    std::vector<std::vector<std::pair<std::string, data_DURATION>>> sequence = {};
    std::vector<std::pair<std::string, data_DURATION>> subsequence = {};
    for(std::pair<std::string, data_DURATION> xmlIdDurPair : dursInVoiceSameMensur){
        data_DURATION dur = xmlIdDurPair.second;
        if (dur == DURATION_brevis || dur == DURATION_longa || dur == DURATION_maxima) {
            sequence.insert(sequence.end(), subsequence);
            subsequence = {xmlIdDurPair};
        } else {
            subsequence.insert(subsequence.end(), xmlIdDurPair);
        }LogDebug("dur is:", dur);
    }
}

} // namespace vrv

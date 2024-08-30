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
std::vector<std::vector<std::pair<std::string, data_DURATION>>> sequence;
std::vector<std::vector<std::pair<std::string, data_DURATION>>> subdivideSeq(std::vector<std::pair<std::string, data_DURATION>> dursInVoiceSameMensur);
void findDurQuals(std::vector<std::vector<std::pair<std::string, data_DURATION>>> sequence);
void findDurQuals(std::vector<std::pair<std::string, data_DURATION>> subsequence);
double durNumberValue(data_DURATION dur);
void imperfectionAPP(std::vector<std::pair<std::string, data_DURATION>> subsequence);
void imperfectionAPA(std::vector<std::pair<std::string, data_DURATION>> subsequence);
void alteration(std::vector<std::pair<std::string, data_DURATION>> subsequence);

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
        sequence = subdivideSeq(dursInVoiceSameMensur);
        findDurQuals(sequence);
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

std::vector<std::vector<std::pair<std::string, data_DURATION>>> subdivideSeq(std::vector<std::pair<std::string, data_DURATION>> dursInVoiceSameMensur)
{
    std::vector<std::vector<std::pair<std::string, data_DURATION>>> sequence = {};
    std::vector<std::pair<std::string, data_DURATION>> subsequence = {};
    for(std::pair<std::string, data_DURATION> xmlIdDurPair : dursInVoiceSameMensur){
        data_DURATION dur = xmlIdDurPair.second;
        if (dur == DURATION_brevis || dur == DURATION_longa || dur == DURATION_maxima) {
            subsequence.insert(subsequence.end(), xmlIdDurPair);
            sequence.insert(sequence.end(), subsequence);
            subsequence = {xmlIdDurPair};
        } else {
            subsequence.insert(subsequence.end(), xmlIdDurPair);
        }LogDebug("dur is:", dur);
    }
    return sequence;
}

void findDurQuals(std::vector<std::vector<std::pair<std::string, data_DURATION>>> sequence){
    for (std::vector<std::pair<std::string, data_DURATION>> subseq: sequence){
        findDurQuals(subseq);
    }
}

void findDurQuals(std::vector<std::pair<std::string, data_DURATION>> subsequence){
    double sum = 0;
    for (std::pair<std::string, data_DURATION> xmlIdDurPair : subsequence){
        data_DURATION dur = xmlIdDurPair.second;
        sum += durNumberValue(dur);
    }

    int remainder = (int)sum % 3;
    bool dotOfPerf = false;         //When true, it forces a perfect value
    bool dotOfImperf = false;       //When true, it forces imperfection a parte post (a.p.p.)
    bool smallNoteValue = false;    //Flag that evaluates the value of the penultimate note in the subsequence. When true, it doesn't allow for alteration
    bool simileAnteSimile = false;  //Flag that evaluates the value of the note following the last note of the subsequence, checking if it is greater or equal to the last note of the subsequence. When true, it doesn't allow for Imperfection a parte ante (a.p.a.)

    // Principles of imperfection and alteration (and their exceptions).
    // The implementation is based on the rules outlined by Willi Apel, with perfect groupings of notes and the remainder:
    if (sum <= 3) { // For sum = 0, 1, 2, or 3
        switch (remainder) {
            case 0:
                break; //No modifications
            case 1:
                if (dotOfPerf) {
                    imperfectionAPA(subsequence);
                    break;
                }//Other, the default case:
                imperfectionAPP(subsequence);
                break;
            case 2:
                if (dotOfImperf || smallNoteValue) {
                    imperfectionAPP(subsequence);
                    imperfectionAPA(subsequence);
                    break;
                }//Other, the default case:
                alteration(subsequence);
                break;
        }
    } else { // For sum > 3
        switch (remainder) {
            case 0:
                if (dotOfPerf || smallNoteValue) {
                    break; //No modifications
                }//Other, the default case:
                imperfectionAPP(subsequence);
                alteration(subsequence);
                break;
            case 1:
                if (dotOfPerf) {
                    imperfectionAPA(subsequence);
                    break;
                }//Other, the default case:
                imperfectionAPP(subsequence);
                break;
            case 2:
                if (dotOfPerf || simileAnteSimile) {
                    alteration(subsequence);
                    break;
                }//Other, the default case:
                imperfectionAPP(subsequence);
                imperfectionAPA(subsequence);
                break;
        }
    }// Missing "rest at" conditions
}

double durNumberValue(data_DURATION dur) {
    double durnum = 0;
    switch(dur) {
        case DURATION_semibrevis:
            durnum = 1;
            break;
        case DURATION_minima:
            durnum = 0.5;
            break;
        case DURATION_semiminima:
            durnum = 0.25;
            break;
        case DURATION_fusa:
            durnum = 0.125;
            break;
        case DURATION_semifusa:
            durnum = 0.0625;
            break;
    } return durnum;
}

void imperfectionAPP(std::vector<std::pair<std::string, data_DURATION>> subsequence){
    std::string firstNoteID = subsequence.at(0).first;
    Note *firstNote; // still need to find this based on the ID (or to pass to the function the element itself
    firstNote->SetDurQuality(DURQUALITY_mensural_imperfecta);
}

void imperfectionAPA(std::vector<std::pair<std::string, data_DURATION>> subsequence){
    std::string lastNoteID = subsequence.at(-1).first;
    Note *lastNote; // still need to find this based on the ID (or to pass to the function the element itself
    lastNote->SetDurQuality(DURQUALITY_mensural_imperfecta);
}

void alteration(std::vector<std::pair<std::string, data_DURATION>> subsequence){
    std::string penultNoteID = subsequence.at(-2).first;
    Note *penultNote; // still need to find this based on the ID (or to pass to the function the element itself
    penultNote->SetDurQuality(DURQUALITY_mensural_altera);
}


} // namespace vrv

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
#include <map>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// ScoringUpFunctor
//----------------------------------------------------------------------------

std::vector<std::pair<LayerElement*, data_DURATION>> dursInVoiceSameMensur = {};

std::vector<std::vector<std::pair<LayerElement*, data_DURATION>>> listOfSequences;
std::vector<std::vector<std::pair<LayerElement*, data_DURATION>>> subdivideSeq(std::vector<std::pair<LayerElement*, data_DURATION>> dursInVoiceSameMensur);

void findDurQuals(std::vector<std::vector<std::pair<LayerElement*, data_DURATION>>> listOfSequences);
void findDurQuals(std::vector<std::pair<LayerElement*, data_DURATION>> sequence);
double durNumberValue(data_DURATION dur);
bool imperfectionAPP(std::vector<std::pair<LayerElement*, data_DURATION>> sequence);
bool imperfectionAPA(std::vector<std::pair<LayerElement*, data_DURATION>> sequence);
bool alteration(std::vector<std::pair<LayerElement*, data_DURATION>> sequence);

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
        listOfSequences = subdivideSeq(dursInVoiceSameMensur);
        findDurQuals(listOfSequences);
        dursInVoiceSameMensur = {}; //restart for next voice (layer)
    }
    return FUNCTOR_CONTINUE;
}

FunctorCode ScoringUpFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (layerElement->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    LayerElement *element = layerElement->ThisOrSameasLink();

    if (element->Is(REST) || element->Is(NOTE) || element->Is(DOT)) {
        data_DURATION dur = DURATION_NONE;
        if (element->Is(NOTE)) {
            Note *note = vrv_cast<Note *>(element);
            assert(note);
            dur = note->GetDur();
        } else if (element->Is(REST)) {
            Rest *rest = vrv_cast<Rest *>(element);
            assert(rest);
            dur = rest->GetDur();
        } dursInVoiceSameMensur.insert(dursInVoiceSameMensur.end(), {element, dur});
    } else if (element->Is(MENSUR)) {
        this->m_currentMensur = vrv_cast<Mensur *>(layerElement);
    }return FUNCTOR_CONTINUE;
}

std::vector<std::vector<std::pair<LayerElement*, data_DURATION>>> subdivideSeq(std::vector<std::pair<LayerElement*, data_DURATION>> dursInVoiceSameMensur)
{
    std::vector<std::vector<std::pair<LayerElement*, data_DURATION>>> listOfSequences = {};
    std::vector<std::pair<LayerElement*, data_DURATION>> sequence = {};
    for(std::pair<LayerElement*, data_DURATION> elementDurPair : dursInVoiceSameMensur){
        data_DURATION dur = elementDurPair.second;
        if (dur == DURATION_brevis || dur == DURATION_longa || dur == DURATION_maxima) {
            sequence.insert(sequence.end(), elementDurPair);
            listOfSequences.insert(listOfSequences.end(), sequence);
            sequence = {elementDurPair};
        } else {
            sequence.insert(sequence.end(), elementDurPair);
        }LogDebug("dur is:", dur);
    }
    return listOfSequences;
}

void findDurQuals(std::vector<std::vector<std::pair<LayerElement*, data_DURATION>>> listOfSequences){
    for (std::vector<std::pair<LayerElement*, data_DURATION>> subseq: listOfSequences){
        findDurQuals(subseq);
    }
}

void findDurQuals(std::vector<std::pair<LayerElement*, data_DURATION>> sequence){
    double sum = 0;
    for (std::pair<LayerElement*, data_DURATION> elementDurPair : sequence){
        data_DURATION dur = elementDurPair.second;
        sum += durNumberValue(dur);
    }

    int remainder = (int)sum % 3;
    bool alterationFlag, impappFlag, impapaFlag;
    bool dotOfPerf = false;         //When true, it forces a perfect value
    bool dotOfImperf = false;       //When true, it forces imperfection a parte post (a.p.p.)
    bool simileAnteSimile = false;  //Flag that evaluates the value of the note following the last note of the sequence, checking if it is greater or equal to the last note of the sequence. When true, it doesn't allow for Imperfection a parte ante (a.p.a.)

    // Principles of imperfection and alteration (and their exceptions).
    // The implementation is based on the rules outlined by Willi Apel, with perfect groupings of notes and the remainder:
    if (sum <= 3) { // For sum = 0, 1, 2, or 3
        switch (remainder) {
            case 0:
                break; //No modifications
            case 1:
                impappFlag = imperfectionAPP(sequence);
                if (!impappFlag || dotOfPerf) {
                    imperfectionAPA(sequence);
                    break;
                } break;
            case 2:
                //smallNoteValue = isPenultimateValueShort(sequence);
                //restValue = isPenultimateValueARest(sequence);
                alterationFlag = alteration(sequence);
                if (!alterationFlag || dotOfImperf) {
                    imperfectionAPP(sequence);
                    imperfectionAPA(sequence);
                    break;
                } break;
        }
    } else { // For sum > 3
        switch (remainder) {
            case 0:
                //smallNoteValue = isPenultimateValueShort(sequence);
                //restValue = isPenultimateValueARest(sequence);
                impappFlag = imperfectionAPP(sequence);
                alterationFlag = alteration(sequence);
                if (!alterationFlag || dotOfPerf) {
                    break; //No modifications
                } break;
            case 1:
                impappFlag = imperfectionAPP(sequence);
                if (!impappFlag || dotOfPerf) {
                    imperfectionAPA(sequence);
                    break;
                } break;
            case 2:
                impappFlag = imperfectionAPP(sequence);
                impapaFlag = imperfectionAPA(sequence);
                if (!impappFlag || !impapaFlag ||dotOfPerf || simileAnteSimile) {
                    alteration(sequence);
                    break;
                } break;
        }
    }
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

bool imperfectionAPP(std::vector<std::pair<LayerElement*, data_DURATION>> sequence) {
    std::pair<LayerElement*, data_DURATION> firstElementDurPair = sequence.at(0);
    LayerElement* firstElement = firstElementDurPair.first;
    data_DURATION firstDur = firstElementDurPair.second;
    if (firstElement->Is(NOTE) && firstDur == DURATION_brevis){
        Note *firstNote = vrv_cast<Note *>(firstElement);
        assert(firstNote);
        firstNote->SetDurQuality(DURQUALITY_mensural_imperfecta);
        return true;
    } else {
        return false;
    }
}

bool imperfectionAPA(std::vector<std::pair<LayerElement*, data_DURATION>> sequence) {
    std::pair<LayerElement*, data_DURATION> lastElementDurPair = sequence.at(sequence.size()-1);
    LayerElement* lastElement = lastElementDurPair.first;
    data_DURATION lastDur = lastElementDurPair.second;
    if (lastElement->Is(NOTE) && lastDur == DURATION_brevis){
        Note *lastNote = vrv_cast<Note *>(lastElement);
        assert(lastNote);
        lastNote->SetDurQuality(DURQUALITY_mensural_imperfecta);
        return true;
    } else {
        return false;
    }
}

bool alteration(std::vector<std::pair<LayerElement*, data_DURATION>> sequence){
    std::pair<LayerElement*, data_DURATION> penultElementDurPair = sequence.at(sequence.size()-2);
    LayerElement* penultElement = penultElementDurPair.first;
    //Evaluates what is the type of the penultimate element in the sequence; when it is a rest, it forbids alteration (return false).
    if (penultElement->Is(NOTE)) {
        Note *penultNote = vrv_cast<Note *>(penultElement);
        assert(penultNote);
        data_DURATION penultDur = penultNote->GetDur();
        //Evaluates the value of the penultimate note in the sequence; when it is a small note value, it forbids alteration (return false); and when it is a larger note value, it alters the note (and returns true).
        if (penultDur == DURATION_semibrevis) {
            penultNote->SetDurQuality(DURQUALITY_mensural_altera);
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

} // namespace vrv

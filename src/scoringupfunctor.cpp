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

std::map<std::string, Note*> notesDictionary;
std::map<std::string, Rest*> restsDictionary;
std::vector<std::pair<std::string, data_DURATION>> dursInVoiceSameMensur = {};

std::vector<std::vector<std::pair<std::string, data_DURATION>>> listOfSequences;
std::vector<std::vector<std::pair<std::string, data_DURATION>>> subdivideSeq(std::vector<std::pair<std::string, data_DURATION>> dursInVoiceSameMensur);
void findDurQuals(std::vector<std::vector<std::pair<std::string, data_DURATION>>> listOfSequences, std::map<std::string, Note*> notesDictionary, std::map<std::string, Rest*> restsDictionary);
void findDurQuals(std::vector<std::pair<std::string, data_DURATION>> sequence, std::map<std::string, Note*> notesDictionary, std::map<std::string, Rest*> restsDictionary);
double durNumberValue(data_DURATION dur);
void imperfectionAPP(std::vector<std::pair<std::string, data_DURATION>> sequence, std::map<std::string, Note*> notesDictionary, std::map<std::string, Rest*> restsDictionary);
void imperfectionAPA(std::vector<std::pair<std::string, data_DURATION>> sequence, std::map<std::string, Note*> notesDictionary, std::map<std::string, Rest*> restsDictionary);
void alteration(std::vector<std::pair<std::string, data_DURATION>> sequence, std::map<std::string, Note*> notesDictionary, std::map<std::string, Rest*> restsDictionary);
bool isPenultimateValueARest(std::vector<std::pair<std::string, data_DURATION>> sequence, std::map<std::string, Rest*> restsDictionary);
bool isPenultimateValueShort(std::vector<std::pair<std::string, data_DURATION>> sequence, std::map<std::string, Note*> notesDictionary);

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
        findDurQuals(listOfSequences, notesDictionary, restsDictionary);
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
            notesDictionary[xmlid] = note;
            //note->SetDurQuality(DURQUALITY_mensural_imperfecta);
        } else {
            Rest *rest = vrv_cast<Rest *>(element);
            assert(rest);
            xmlid = rest->GetID();
            dur = rest->GetDur();
            restsDictionary[xmlid] = rest;
        } dursInVoiceSameMensur.insert(dursInVoiceSameMensur.end(), {xmlid, dur});
    } else if (element->Is(MENSUR)) {
        this->m_currentMensur = vrv_cast<Mensur *>(layerElement);
    }return FUNCTOR_CONTINUE;
}

std::vector<std::vector<std::pair<std::string, data_DURATION>>> subdivideSeq(std::vector<std::pair<std::string, data_DURATION>> dursInVoiceSameMensur)
{
    std::vector<std::vector<std::pair<std::string, data_DURATION>>> listOfSequences = {};
    std::vector<std::pair<std::string, data_DURATION>> sequence = {};
    for(std::pair<std::string, data_DURATION> xmlIdDurPair : dursInVoiceSameMensur){
        data_DURATION dur = xmlIdDurPair.second;
        if (dur == DURATION_brevis || dur == DURATION_longa || dur == DURATION_maxima) {
            sequence.insert(sequence.end(), xmlIdDurPair);
            listOfSequences.insert(listOfSequences.end(), sequence);
            sequence = {xmlIdDurPair};
        } else {
            sequence.insert(sequence.end(), xmlIdDurPair);
        }LogDebug("dur is:", dur);
    }
    return listOfSequences;
}

void findDurQuals(std::vector<std::vector<std::pair<std::string, data_DURATION>>> listOfSequences, std::map<std::string, Note*> notesDictionary, std::map<std::string, Rest*> restsDictionary){
    for (std::vector<std::pair<std::string, data_DURATION>> subseq: listOfSequences){
        findDurQuals(subseq, notesDictionary, restsDictionary);
    }
}

void findDurQuals(std::vector<std::pair<std::string, data_DURATION>> sequence, std::map<std::string, Note*> notesDictionary, std::map<std::string, Rest*> restsDictionary){
    double sum = 0;
    for (std::pair<std::string, data_DURATION> xmlIdDurPair : sequence){
        data_DURATION dur = xmlIdDurPair.second;
        sum += durNumberValue(dur);
    }

    int remainder = (int)sum % 3;
    bool dotOfPerf = false;         //When true, it forces a perfect value
    bool dotOfImperf = false;       //When true, it forces imperfection a parte post (a.p.p.)
    bool smallNoteValue = false;    //Flag that evaluates the value of the penultimate note in the sequence. When true, it doesn't allow for alteration
    bool restValue = false;
    bool simileAnteSimile = false;  //Flag that evaluates the value of the note following the last note of the sequence, checking if it is greater or equal to the last note of the sequence. When true, it doesn't allow for Imperfection a parte ante (a.p.a.)

    // Principles of imperfection and alteration (and their exceptions).
    // The implementation is based on the rules outlined by Willi Apel, with perfect groupings of notes and the remainder:
    if (sum <= 3) { // For sum = 0, 1, 2, or 3
        switch (remainder) {
            case 0:
                break; //No modifications
            case 1:
                if (dotOfPerf) {
                    imperfectionAPA(sequence, notesDictionary, restsDictionary);
                    break;
                }//Other, the default case:
                imperfectionAPP(sequence, notesDictionary, restsDictionary);
                break;
            case 2:
                smallNoteValue = isPenultimateValueShort(sequence, notesDictionary);
                restValue = isPenultimateValueARest(sequence, restsDictionary);
                if (dotOfImperf || smallNoteValue || restValue) {
                    imperfectionAPP(sequence, notesDictionary, restsDictionary);
                    imperfectionAPA(sequence, notesDictionary, restsDictionary);
                    break;
                }//Other, the default case:
                alteration(sequence, notesDictionary, restsDictionary);
                break;
        }
    } else { // For sum > 3
        switch (remainder) {
            case 0:
                if (dotOfPerf || smallNoteValue || isPenultimateValueARest(sequence, restsDictionary)) {
                    break; //No modifications
                }//Other, the default case:
                imperfectionAPP(sequence, notesDictionary, restsDictionary);
                alteration(sequence, notesDictionary, restsDictionary);
                break;
            case 1:
                if (dotOfPerf) {
                    imperfectionAPA(sequence, notesDictionary, restsDictionary);
                    break;
                }//Other, the default case:
                imperfectionAPP(sequence, notesDictionary, restsDictionary);
                break;
            case 2:
                if (dotOfPerf || simileAnteSimile) {
                    alteration(sequence, notesDictionary, restsDictionary);
                    break;
                }//Other, the default case:
                imperfectionAPP(sequence, notesDictionary, restsDictionary);
                imperfectionAPA(sequence, notesDictionary, restsDictionary);
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

void imperfectionAPP(std::vector<std::pair<std::string, data_DURATION>> sequence, std::map<std::string, Note*> notesDictionary, std::map<std::string, Rest*> restsDictionary){
    std::string firstNoteID = sequence.at(0).first;
    Note *firstNote = notesDictionary[firstNoteID];
    firstNote->SetDurQuality(DURQUALITY_mensural_imperfecta);
    LogDebug("thenote!");
}

void imperfectionAPA(std::vector<std::pair<std::string, data_DURATION>> sequence, std::map<std::string, Note*> notesDictionary, std::map<std::string, Rest*> restsDictionary){
    std::string lastNoteID = sequence.at(sequence.size()-1).first;
    Note *lastNote = notesDictionary[lastNoteID];
    lastNote->SetDurQuality(DURQUALITY_mensural_imperfecta);
    LogDebug("thenote!");
}

void alteration(std::vector<std::pair<std::string, data_DURATION>> sequence, std::map<std::string, Note*> notesDictionary, std::map<std::string, Rest*> restsDictionary){
    std::string penultNoteID = sequence.at(sequence.size()-2).first;
    Note *penultNote = notesDictionary[penultNoteID];
    penultNote->SetDurQuality(DURQUALITY_mensural_altera);
    LogDebug("thenote!");
}

bool isPenultimateValueARest(std::vector<std::pair<std::string, data_DURATION>> sequence, std::map<std::string, Rest*> restsDictionary){
    std::string penultNoteID = sequence.at(sequence.size()-2).first;
    if (restsDictionary[penultNoteID]) {return true;}
    else {return false;}
}

bool isPenultimateValueShort(std::vector<std::pair<std::string, data_DURATION>> sequence, std::map<std::string, Note*> notesDictionary){
    std::string penultNoteID = sequence.at(sequence.size()-2).first;
    bool smallNoteValue = false;
    if (notesDictionary[penultNoteID]) {
        Note *penultNote = notesDictionary[penultNoteID];
        data_DURATION dur = penultNote->GetDur();
        if (dur == DURATION_fusa || dur == DURATION_semifusa || dur == DURATION_minima || dur == DURATION_semiminima){
            smallNoteValue = true;
        }
    } return smallNoteValue;
}

} // namespace vrv

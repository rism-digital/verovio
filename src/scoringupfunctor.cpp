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

std::vector<std::pair<std::string, Note>> notes = {};
std::vector<std::pair<std::string, Rest>> rests = {};
std::vector<std::pair<std::string, data_DURATION>> dursInVoiceSameMensur = {};

std::vector<std::vector<std::pair<std::string, data_DURATION>>> listOfSequences;
std::vector<std::vector<std::pair<std::string, data_DURATION>>> subdivideSeq(std::vector<std::pair<std::string, data_DURATION>> dursInVoiceSameMensur);
void findDurQuals(std::vector<std::vector<std::pair<std::string, data_DURATION>>> listOfSequences, std::vector<std::pair<std::string, Note>> *notes, std::vector<std::pair<std::string, Rest>> *rests);
void findDurQuals(std::vector<std::pair<std::string, data_DURATION>> sequence, std::vector<std::pair<std::string, Note>> *notes, std::vector<std::pair<std::string, Rest>> *rests);
double durNumberValue(data_DURATION dur);
void imperfectionAPP(std::vector<std::pair<std::string, data_DURATION>> sequence, std::vector<std::pair<std::string, Note>> *notes, std::vector<std::pair<std::string, Rest>> *rests);
void imperfectionAPA(std::vector<std::pair<std::string, data_DURATION>> sequence, std::vector<std::pair<std::string, Note>> *notes, std::vector<std::pair<std::string, Rest>> *rests);
void alteration(std::vector<std::pair<std::string, data_DURATION>> sequence, std::vector<std::pair<std::string, Note>> *notes, std::vector<std::pair<std::string, Rest>> *rests);

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
        findDurQuals(listOfSequences, &notes, &rests);
        dursInVoiceSameMensur = {}; //restart for next voice (layer)
        notes = {};
        rests = {};
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
            notes.insert(notes.end(), {xmlid, *note});
            //note->SetDurQuality(DURQUALITY_mensural_imperfecta);
        } else {
            Rest *rest = vrv_cast<Rest *>(element);
            assert(rest);
            xmlid = rest->GetID();
            dur = rest->GetDur();
            rests.insert(rests.end(), {xmlid, *rest});
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

void findDurQuals(std::vector<std::vector<std::pair<std::string, data_DURATION>>> listOfSequences, std::vector<std::pair<std::string, Note>> *notes, std::vector<std::pair<std::string, Rest>> *rests){
    for (std::vector<std::pair<std::string, data_DURATION>> subseq: listOfSequences){
        findDurQuals(subseq, notes, rests);
    }
}

void findDurQuals(std::vector<std::pair<std::string, data_DURATION>> sequence, std::vector<std::pair<std::string, Note>> *notes, std::vector<std::pair<std::string, Rest>> *rests){
    double sum = 0;
    for (std::pair<std::string, data_DURATION> xmlIdDurPair : sequence){
        data_DURATION dur = xmlIdDurPair.second;
        sum += durNumberValue(dur);
    }

    int remainder = (int)sum % 3;
    bool dotOfPerf = false;         //When true, it forces a perfect value
    bool dotOfImperf = false;       //When true, it forces imperfection a parte post (a.p.p.)
    bool smallNoteValue = false;    //Flag that evaluates the value of the penultimate note in the sequence. When true, it doesn't allow for alteration
    bool simileAnteSimile = false;  //Flag that evaluates the value of the note following the last note of the sequence, checking if it is greater or equal to the last note of the sequence. When true, it doesn't allow for Imperfection a parte ante (a.p.a.)

    // Principles of imperfection and alteration (and their exceptions).
    // The implementation is based on the rules outlined by Willi Apel, with perfect groupings of notes and the remainder:
    if (sum <= 3) { // For sum = 0, 1, 2, or 3
        switch (remainder) {
            case 0:
                break; //No modifications
            case 1:
                if (dotOfPerf) {
                    imperfectionAPA(sequence, notes, rests);
                    break;
                }//Other, the default case:
                imperfectionAPP(sequence, notes, rests);
                break;
            case 2:
                if (dotOfImperf || smallNoteValue) {
                    imperfectionAPP(sequence, notes, rests);
                    imperfectionAPA(sequence, notes, rests);
                    break;
                }//Other, the default case:
                alteration(sequence, notes, rests);
                break;
        }
    } else { // For sum > 3
        switch (remainder) {
            case 0:
                if (dotOfPerf || smallNoteValue) {
                    break; //No modifications
                }//Other, the default case:
                imperfectionAPP(sequence, notes, rests);
                alteration(sequence, notes, rests);
                break;
            case 1:
                if (dotOfPerf) {
                    imperfectionAPA(sequence, notes, rests);
                    break;
                }//Other, the default case:
                imperfectionAPP(sequence, notes, rests);
                break;
            case 2:
                if (dotOfPerf || simileAnteSimile) {
                    alteration(sequence, notes, rests);
                    break;
                }//Other, the default case:
                imperfectionAPP(sequence, notes, rests);
                imperfectionAPA(sequence, notes, rests);
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

void imperfectionAPP(std::vector<std::pair<std::string, data_DURATION>> sequence, std::vector<std::pair<std::string, Note>> *notes, std::vector<std::pair<std::string, Rest>> *rests){
    std::string firstNoteID = sequence.at(0).first;
    for(std::pair<std::string, Note> note : *notes){
        if(note.first == firstNoteID){
            Note *theNote = &note.second;
            theNote->SetDurQuality(DURQUALITY_mensural_imperfecta);
            LogDebug("the note");
            break;
        }
    }
}

void imperfectionAPA(std::vector<std::pair<std::string, data_DURATION>> sequence, std::vector<std::pair<std::string, Note>> *notes, std::vector<std::pair<std::string, Rest>> *rests){
    //std::string lastNoteID = sequence.at(-1).first;
    //Note *lastNote; // still need to find this based on the ID (or to pass to the function the element itself
    //lastNote->SetDurQuality(DURQUALITY_mensural_imperfecta);
}

void alteration(std::vector<std::pair<std::string, data_DURATION>> sequence, std::vector<std::pair<std::string, Note>> *notes, std::vector<std::pair<std::string, Rest>> *rests){
    std::string penultNoteID = sequence.at(sequence.size()-2).first;
    for(std::pair<std::string, Note> note : *notes){
        if(note.first == penultNoteID){
            Note *theNote = &note.second;
            theNote->SetDurQuality(DURQUALITY_mensural_altera);
            LogDebug("the note");
            break;
        }
    }
}


} // namespace vrv

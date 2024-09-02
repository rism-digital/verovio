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

std::map<std::string, int> mensuration;
std::vector<std::pair<LayerElement*, data_DURATION>> dursInVoiceSameMensur = {};

std::vector<std::vector<std::pair<LayerElement*, data_DURATION>>> listOfSequences;
std::vector<std::vector<std::pair<LayerElement*, data_DURATION>>> subdivideSeq(std::vector<std::pair<LayerElement*, data_DURATION>> dursInVoiceSameMensur);

void findDurQuals(std::vector<std::vector<std::pair<LayerElement*, data_DURATION>>> listOfSequences);
void findDurQuals(std::vector<std::pair<LayerElement*, data_DURATION>> sequence);
double durNumberValue(std::pair<LayerElement*, data_DURATION> elementDurPair);
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
        this->m_currentMensur = vrv_cast<Mensur *>(element);
        if (m_currentMensur->GetModusmaior() == MODUSMAIOR_3) {mensuration["modusmaior"] = 3;}
        else {mensuration["modusmaior"] = 2;}
        if (m_currentMensur->GetModusminor() == MODUSMINOR_3) {mensuration["modusminor"] = 3;}
        else {mensuration["modusminor"] = 2;}
        if (m_currentMensur->GetTempus() == TEMPUS_3) {mensuration["tempus"] = 3;}
        else {mensuration["tempus"] = 2;}
        if (m_currentMensur->GetProlatio() == PROLATIO_3) {mensuration["prolatio"] = 3;}
        else {mensuration["prolatio"] = 2;}
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
    std::vector<std::pair<LayerElement*, data_DURATION>> middleSeq = {};
    if (sequence.size() > 2) {
        middleSeq = {sequence.begin() + 1, sequence.end() - 1};
    }

    // Value in minims:
    double sum = 0;
    for (std::pair<LayerElement*, data_DURATION> elementDurPair : middleSeq){
        sum += durNumberValue(elementDurPair);
    } sum = sum/2;
    int remainder = (int)sum % 3;

    // Flags:
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

double durNumberValue(std::pair<LayerElement*, data_DURATION> elementDurPair) {
    data_DURQUALITY_mensural durquality;
    data_DURATION dur = elementDurPair.second;
    LayerElement* element = elementDurPair.first;
    if (element->Is(NOTE)){
        Note *note = vrv_cast<Note *>(element);
        assert(note);
        durquality = note->GetDurQuality();
    }
    int modusminor = mensuration["modusminor"];
    int tempus = mensuration["tempus"];
    int prolatio = mensuration["prolatio"];
    int longaDefaultVal = modusminor * tempus * prolatio;
    int brevisDefaultVal = tempus * prolatio;
    int semibrevisDefaultVal = prolatio;
    double durnum = 0;
    switch(dur) {
        case DURATION_longa:
            if (modusminor == 2 || durquality == DURQUALITY_mensural_imperfecta) {
                durnum = 2 * brevisDefaultVal;
            } else if (modusminor == 3 || durquality == DURQUALITY_mensural_perfecta) {
                durnum = 3 * brevisDefaultVal;
            } break;
        case DURATION_brevis:
            if (tempus == 2 || durquality == DURQUALITY_mensural_imperfecta) {
                durnum = 2 * semibrevisDefaultVal;
            } else if (tempus == 3 || durquality == DURQUALITY_mensural_perfecta) {
                durnum = 3 * semibrevisDefaultVal;
            } break;
        case DURATION_semibrevis:
            if (prolatio == 2 || durquality == DURQUALITY_mensural_imperfecta) {
                durnum = 2;
            } else if (prolatio == 3 || durquality == DURQUALITY_mensural_perfecta) {
                durnum = 3;
            } break;
        case DURATION_minima:
            durnum = 1;
            break;
        case DURATION_semiminima:
            durnum = 0.5;
            break;
        case DURATION_fusa:
            durnum = 0.25;
            break;
        case DURATION_semifusa:
            durnum = 0.125;
            break;
        default:
            break;
    } return durnum;
}

bool imperfectionAPP(std::vector<std::pair<LayerElement*, data_DURATION>> sequence) {
    std::pair<LayerElement*, data_DURATION> firstElementDurPair = sequence.at(0);
    LayerElement* firstElement = firstElementDurPair.first;
    data_DURATION firstDur = firstElementDurPair.second;
    /// Evaluates if the first note in the sequence is a brevis. If it is, then it imperfects it (and returns true). If it is a rest or a larger note, then it forbids imperfection (and returns false).
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
    /// Evaluates if the last note in the sequence is a brevis. If it is, then it imperfects it (and returns true). If it is a rest or a larger note, then it forbids imperfection (and returns false).
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
    data_DURATION penultDur = penultElementDurPair.second;
    ///Evaluates what is the type of the penultimate element in the sequence. If it is a rest, it forbids alteration (returns false). If it is a note, and the note is a 'semibrevis', it alters the note (and returns true).
    if (penultElement->Is(NOTE) && penultDur == DURATION_semibrevis) {
        Note *penultNote = vrv_cast<Note *>(penultElement);
        assert(penultNote);
        penultNote->SetDurQuality(DURQUALITY_mensural_altera);
        return true;
    } else {
        return false;
    }
}

} // namespace vrv

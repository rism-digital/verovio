/////////////////////////////////////////////////////////////////////////////
// Name:        scoringupfunctor.cpp
// Author:      Martha E. M. Thomae Elias
// Created:     2024
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////
///
#include "scoringupfunctor.h"

//----------------------------------------------------------------------------

#include <map>

//----------------------------------------------------------------------------

#include "dot.h"
#include "layer.h"
#include "note.h"
#include "page.h"
#include "rest.h"
#include "staff.h"
#include "system.h"
#include "vrv.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// ScoringUpFunctor
//----------------------------------------------------------------------------

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
    /*data_MODUSMAIOR modusmaior = m_currentMensur->GetModusmaior();
    data_MODUSMINOR modusminor = m_currentMensur->GetModusminor();
    data_TEMPUS tempus = m_currentMensur->GetTempus();
    data_PROLATIO prolatio = m_currentMensur->GetProlatio();*/
    // Doesn't get it from the staffDef, right?//
    if (!m_dursInVoiceSameMensur.empty()) {
        m_listOfSequences = SubdivideSeq(m_dursInVoiceSameMensur);
        FindDurQuals(m_listOfSequences);
        m_dursInVoiceSameMensur = {}; // restart for next voice (layer)
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
        }
        else if (element->Is(REST)) {
            Rest *rest = vrv_cast<Rest *>(element);
            assert(rest);
            dur = rest->GetDur();
        }
        m_dursInVoiceSameMensur.insert(m_dursInVoiceSameMensur.end(), { element, dur });
    }
    else if (element->Is(MENSUR)) {
        this->m_currentMensur = vrv_cast<Mensur *>(element);
        if (m_currentMensur->GetModusmaior() == MODUSMAIOR_3) {
            m_mensuration["modusmaior"] = 3;
        }
        else {
            m_mensuration["modusmaior"] = 2;
        }
        if (m_currentMensur->GetModusminor() == MODUSMINOR_3) {
            m_mensuration["modusminor"] = 3;
        }
        else {
            m_mensuration["modusminor"] = 2;
        }
        if (m_currentMensur->GetTempus() == TEMPUS_3) {
            m_mensuration["tempus"] = 3;
        }
        else {
            m_mensuration["tempus"] = 2;
        }
        if (m_currentMensur->GetProlatio() == PROLATIO_3) {
            m_mensuration["prolatio"] = 3;
        }
        else {
            m_mensuration["prolatio"] = 2;
        }
    }
    return FUNCTOR_CONTINUE;
}

std::vector<ArrayOfElementDurPairs> ScoringUpFunctor::SubdivideSeq(const ArrayOfElementDurPairs &dursInVoiceSameMensur)
{
    std::vector<ArrayOfElementDurPairs> listOfSequences = {};
    ArrayOfElementDurPairs sequence = {};
    for (std::pair<LayerElement *, data_DURATION> elementDurPair : dursInVoiceSameMensur) {
        data_DURATION dur = elementDurPair.second;
        if (dur == DURATION_brevis || dur == DURATION_longa || dur == DURATION_maxima) {
            sequence.insert(sequence.end(), elementDurPair);
            listOfSequences.insert(listOfSequences.end(), sequence);
            sequence = { elementDurPair };
        }
        else {
            sequence.insert(sequence.end(), elementDurPair);
        }
        LogDebug("dur is:", dur);
    }
    return listOfSequences;
}

void ScoringUpFunctor::FindDurQuals(const std::vector<ArrayOfElementDurPairs> &listOfSequences)
{
    for (ArrayOfElementDurPairs subseq : listOfSequences) {
        FindDurQuals(subseq);
    }
}

void ScoringUpFunctor::FindDurQuals(const ArrayOfElementDurPairs &sequence)
{
    ArrayOfElementDurPairs middleSeq = {};
    if (sequence.size() >= 2) {
        data_DURATION firstNoteDur = sequence.at(0).second;
        if (firstNoteDur == DURATION_semibrevis || firstNoteDur == DURATION_minima
            || firstNoteDur == DURATION_semiminima || firstNoteDur == DURATION_fusa
            || firstNoteDur == DURATION_semifusa) {
            middleSeq = { sequence.begin(), sequence.end() - 1 };
        }
        else {
            middleSeq = { sequence.begin() + 1, sequence.end() - 1 };
        }
    }

    // Check if there are dots in the middleSeq and how many. The dots in the middleSeq are the only ones that have the
    // possibility of being a dot of imperfection (or alteration) or a dot of augmentation---the dots of perfection are
    // not in the middleSeq.
    std::vector<int> indecesOfDots;
    for (int i = 0; i < middleSeq.size(); i++) {
        std::pair<LayerElement *, data_DURATION> elementDurPair = middleSeq.at(i);
        if (elementDurPair.first->Is(DOT)) {
            indecesOfDots.insert(indecesOfDots.end(), i);
        }
    }
    int numberOfDots = (int)indecesOfDots.size();
    if (numberOfDots >= 1) {
        // Take first dot and evaluate if it is a dot of imperfection (check if it is "integer number of semibreves"
        // away from the beginning of the sequence, and if the rest of the sequence still sums an integer number)

        // Take last dot and evaluate if it is a dot of division (check if it is "integer number of semibreves" away
        // from the end of the sequence, and if the rest of the sequence still sums an integer number)

        // If neither, all dots are dots of augmentation
        if (numberOfDots == 1) {
        }
    }

    // Value in minims:
    double sum = 0;
    bool followedByDot = false;
    LayerElement *nextElement = NULL;
    for (int i = 0; i < middleSeq.size(); i++) {
        std::pair<LayerElement *, data_DURATION> elementDurPair = middleSeq.at(i);
        // Check if there is a dot after the element being evaluated
        if (i + 1 < middleSeq.size()) {
            nextElement = middleSeq.at(i + 1).first;
            followedByDot = nextElement->Is(DOT);
        }
        else {
            followedByDot = false;
        }
        sum += GetDurNumberValue(elementDurPair, followedByDot, nextElement);
    }
    sum = sum / 2;
    int remainder = (int)sum % 3;

    // CHECK SUM --> IS IT INTEGER when dot of division???
    // CHECK REMAINDER

    // Flags:
    bool alterationFlag, impappFlag, impapaFlag;
    bool dotOfImperf = false; // When true, it forces imperfection a parte post (a.p.p.)
    bool simileAnteSimile = false; // Flag that evaluates the value of the note following the last note of the sequence,
                                   // checking if it is greater or equal to the last note of the sequence. When true, it
                                   // doesn't allow for Imperfection a parte ante (a.p.a.)

    // Principles of imperfection and Alteration (and their exceptions).
    // The implementation is based on the rules outlined by Willi Apel, with perfect groupings of notes and the
    // remainder:
    if (sum < 3) { // For sum = 0, 1, 2, or 3
        switch (remainder) {
            case 0: break; // No modifications
            case 1:
                impappFlag = ImperfectionAPP(sequence);
                if (!impappFlag) {
                    ImperfectionAPA(sequence);
                    break;
                }
                break;
            case 2:
                alterationFlag = Alteration(sequence);
                if (!alterationFlag || dotOfImperf) {
                    ImperfectionAPP(sequence);
                    ImperfectionAPA(sequence);
                    break;
                }
                break;
        }
    }
    else if (sum == 3) {
        bool leavePerfectFlag = LeavePerfect(sequence);
        if (!leavePerfectFlag || dotOfImperf) {
        }
    }
    else { // For sum > 3
        switch (remainder) {
            case 0:
                impappFlag = ImperfectionAPP(sequence);
                alterationFlag = Alteration(sequence);
                if (!alterationFlag || !impappFlag) {
                    break; // No modifications
                }
                break;
            case 1:
                impappFlag = ImperfectionAPP(sequence);
                if (!impappFlag) {
                    ImperfectionAPA(sequence);
                    break;
                }
                break;
            case 2:
                impappFlag = ImperfectionAPP(sequence);
                impapaFlag = ImperfectionAPA(sequence);
                if (!impappFlag || !impapaFlag || simileAnteSimile) {
                    Alteration(sequence);
                    break;
                }
                break;
        }
    }
}

double ScoringUpFunctor::GetDurNumberValue(
    std::pair<LayerElement *, data_DURATION> elementDurPair, bool followedByDot, LayerElement *nextElement)
{
    data_DURQUALITY_mensural durquality;
    data_DURATION dur = elementDurPair.second;
    LayerElement *element = elementDurPair.first;
    Note *note;
    if (element->Is(NOTE)) {
        note = vrv_cast<Note *>(element);
        assert(note);
        durquality = note->GetDurQuality();
    }
    int modusminor = m_mensuration["modusminor"];
    int tempus = m_mensuration["tempus"];
    int prolatio = m_mensuration["prolatio"];
    // int longaDefaultVal = modusminor * tempus * prolatio;
    int brevisDefaultVal = tempus * prolatio;
    int semibrevisDefaultVal = prolatio;
    double durnum = 0;
    switch (dur) {
        case DURATION_longa:
            if (modusminor == 3 || durquality == DURQUALITY_mensural_perfecta || followedByDot) {
                durnum = 3 * brevisDefaultVal;
            }
            else if (modusminor == 2 || durquality == DURQUALITY_mensural_imperfecta) {
                durnum = 2 * brevisDefaultVal;
            }
            break;
        case DURATION_brevis:
            if (tempus == 3 || durquality == DURQUALITY_mensural_perfecta || followedByDot) {
                durnum = 3 * semibrevisDefaultVal;
            }
            else if (tempus == 2 || durquality == DURQUALITY_mensural_imperfecta) {
                durnum = 2 * semibrevisDefaultVal;
            }
            break;
        case DURATION_semibrevis:
            if (prolatio == 3 || durquality == DURQUALITY_mensural_perfecta || followedByDot) {
                durnum = 3;
            }
            else if (prolatio == 2 || durquality == DURQUALITY_mensural_imperfecta) {
                durnum = 2;
            }
            break;
        case DURATION_minima:
            if (followedByDot) {
                durnum = 1.5;
                note->SetDurQuality(DURQUALITY_mensural_perfecta);
                Dot *dot = vrv_cast<Dot *>(nextElement);
                dot->SetForm(dotLog_FORM_aug);
            }
            else {
                durnum = 1;
            }
            break;
        case DURATION_semiminima:
            if (followedByDot) {
                durnum = 0.75;
                note->SetDurQuality(DURQUALITY_mensural_perfecta);
                Dot *dot = vrv_cast<Dot *>(nextElement);
                dot->SetForm(dotLog_FORM_aug);
            }
            else {
                durnum = 0.5;
            }
            break;
        case DURATION_fusa:
            if (followedByDot) {
                durnum = 0.375;
                note->SetDurQuality(DURQUALITY_mensural_perfecta);
                Dot *dot = vrv_cast<Dot *>(nextElement);
                dot->SetForm(dotLog_FORM_aug);
            }
            else {
                durnum = 0.25;
            }
            break;
        case DURATION_semifusa: durnum = 0.125; break;
        default: break;
    }
    return durnum;
}

bool ScoringUpFunctor::ImperfectionAPP(const ArrayOfElementDurPairs &sequence)
{
    std::pair<LayerElement *, data_DURATION> firstElementDurPair = sequence.at(0);
    LayerElement *firstElement = firstElementDurPair.first;
    data_DURATION firstDur = firstElementDurPair.second;
    LayerElement *nextElement;
    /// Evaluates if the first note in the sequence is a brevis. If it is, then it imperfects it (and returns true). If
    /// it is a rest or a larger note, then it forbids imperfection (and returns false).
    if (firstElement->Is(NOTE) && firstDur == DURATION_brevis) {
        /// Before imperfecting the starting brevis, check if it is followed by a dot. If it is, this would be a dot of
        /// perfectio, i.e., a dot that forces perfection (forbidding the imperfection and returning false). Otherwise,
        /// if there is no dot following the starting brevis, proceed to perform the imperfection a.p.p. and return
        /// true.
        nextElement = sequence.at(1).first;
        if (nextElement->Is(DOT)) {
            /// Dot of perfection (type of dot of division)
            Dot *dot = vrv_cast<Dot *>(nextElement);
            dot->SetForm(dotLog_FORM_div);
            return false;
        }
        else {
            /// Imperfection a.p.p. (i.e., imperfection by the following notes), unless the note has already been
            /// modified by the previous sequence
            Note *firstNote = vrv_cast<Note *>(firstElement);
            assert(firstNote);
            if (firstNote->HasDurQuality()) {
                /// Already modified before
                return false;
            }
            else {
                /// Imperfection a.p.p.
                firstNote->SetDurQuality(DURQUALITY_mensural_imperfecta);
                return true;
            }
        }
    }
    else {
        return false;
    }
}

bool ScoringUpFunctor::ImperfectionAPA(const ArrayOfElementDurPairs &sequence)
{
    std::pair<LayerElement *, data_DURATION> lastElementDurPair = sequence.at(sequence.size() - 1);
    LayerElement *lastElement = lastElementDurPair.first;
    data_DURATION lastDur = lastElementDurPair.second;
    /// Evaluates if the last note in the sequence is a brevis. If it is, then it imperfects it (and returns true). If
    /// it is a rest or a larger note, then it forbids imperfection (and returns false).
    if (lastElement->Is(NOTE) && lastDur == DURATION_brevis) {
        Note *lastNote = vrv_cast<Note *>(lastElement);
        assert(lastNote);
        lastNote->SetDurQuality(DURQUALITY_mensural_imperfecta);
        return true;
    }
    else {
        return false;
    }
}

bool ScoringUpFunctor::Alteration(const ArrayOfElementDurPairs &sequence)
{
    std::pair<LayerElement *, data_DURATION> penultElementDurPair = sequence.at(sequence.size() - 2);
    LayerElement *penultElement = penultElementDurPair.first;
    data_DURATION penultDur = penultElementDurPair.second;
    /// Evaluates what is the type of the penultimate element in the sequence. If it is a rest, it forbids Alteration
    /// (returns false). If it is a note, and the note is a 'semibrevis', it alters the note (and returns true).
    if (penultElement->Is(NOTE) && penultDur == DURATION_semibrevis) {
        Note *penultNote = vrv_cast<Note *>(penultElement);
        assert(penultNote);
        penultNote->SetDurQuality(DURQUALITY_mensural_altera);
        return true;
    }
    else {
        return false;
    }
}

bool ScoringUpFunctor::LeavePerfect(const ArrayOfElementDurPairs &sequence)
{
    return true;
}

} // namespace vrv

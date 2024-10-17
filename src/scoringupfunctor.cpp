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
        m_listOfSequences = SubdivideIntoBoundedSequences(m_dursInVoiceSameMensur);
        ProcessBoundedSequences(m_listOfSequences);
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
            m_modusMaior = 3;
        }
        else {
            m_modusMaior = 2;
        }
        if (m_currentMensur->GetModusminor() == MODUSMINOR_3) {
            m_modusMinor = 3;
        }
        else {
            m_modusMinor = 2;
        }
        if (m_currentMensur->GetTempus() == TEMPUS_3) {
            m_tempus = 3;
        }
        else {
            m_tempus = 2;
        }
        if (m_currentMensur->GetProlatio() == PROLATIO_3) {
            m_prolatio = 3;
        }
        else {
            m_prolatio = 2;
        }
    }
    return FUNCTOR_CONTINUE;
}

std::vector<ArrayOfElementDurPairs> ScoringUpFunctor::SubdivideIntoBoundedSequences(
    const ArrayOfElementDurPairs &dursInVoiceSameMensur)
{
    std::vector<ArrayOfElementDurPairs> listOfBoundedSequences = {};
    ArrayOfElementDurPairs boundedSequence = {};
    for (std::pair<LayerElement *, data_DURATION> elementDurPair : dursInVoiceSameMensur) {
        data_DURATION dur = elementDurPair.second;
        if (dur == DURATION_brevis || dur == DURATION_longa || dur == DURATION_maxima) {
            boundedSequence.insert(boundedSequence.end(), elementDurPair);
            listOfBoundedSequences.insert(listOfBoundedSequences.end(), boundedSequence);
            boundedSequence = { elementDurPair };
        }
        else {
            boundedSequence.insert(boundedSequence.end(), elementDurPair);
        }
        LogDebug("dur is:", dur);
    }
    return listOfBoundedSequences;
}

ArrayOfElementDurPairs ScoringUpFunctor::GetBoundedNotes(const ArrayOfElementDurPairs &sequence)
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
    return middleSeq;
}

bool ScoringUpFunctor::EvalDotOfDiv(
    const ArrayOfElementDurPairs &middleSeq, const ArrayOfElementDurPairs &sequence, int dotInd)
{
    // Initial assumption: the sequence doesn't have a dot of division
    bool flagDotOfDiv = false;
    // Evaluate if the dot passed to the method (by the dotInd) is behaving as a dot of division
    ArrayOfElementDurPairs middleSeq1 = { middleSeq.begin(), middleSeq.begin() + dotInd };
    ArrayOfElementDurPairs middleSeq2 = { middleSeq.begin() + dotInd + 1, middleSeq.end() };
    double sum1 = GetValueInUnit(GetValueInMinims(middleSeq1), DURATION_semibrevis);
    double sum2 = GetValueInUnit(GetValueInMinims(middleSeq2), DURATION_semibrevis);
    // This condition is to discard it being a dot of perfection
    if (middleSeq1.size() != 0) {
        // This other condition is to evaluate if it is a dot of division
        if ((sum1 == (int)sum1) and (sum2 == (int)sum2)) {
            // This is a "dot of division"
            flagDotOfDiv = true;
            // Then it divides the sequence into the following two:
            ArrayOfElementDurPairs seq1 = { sequence.begin(), sequence.begin() + dotInd + 1 };
            ArrayOfElementDurPairs seq2 = { sequence.begin() + dotInd + 2, sequence.end() };
            // Encode the dot of division:
            LayerElement *dotElement = sequence.at(dotInd + 1).first;
            Dot *dot = vrv_cast<Dot *>(dotElement);
            dot->SetForm(dotLog_FORM_div);
            // Encode its effect on the notes preceding and following:
            FindDurQuals(seq1, sum1);
            FindDurQuals(seq2, sum2);
        }
    }
    return flagDotOfDiv;
}

void ScoringUpFunctor::ProcessBoundedSequences(const std::vector<ArrayOfElementDurPairs> &listOfSequences)
{
    for (ArrayOfElementDurPairs sequence : listOfSequences) {
        ProcessBoundedSequences(sequence);
    }
}

void ScoringUpFunctor::ProcessBoundedSequences(const ArrayOfElementDurPairs &sequence)
{
    // Get the notes in the middle of the boundaries of the sequence
    ArrayOfElementDurPairs middleSeq = GetBoundedNotes(sequence);

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
    // 0. No dots
    double sum;
    if (numberOfDots == 0) {
        sum = GetValueInUnit(GetValueInMinims(middleSeq), DURATION_semibrevis);
        FindDurQuals(sequence, sum);
    }
    // 1. Single dot in middle sequence sequence
    else if (numberOfDots == 1) {
        // If there is one dot,
        int dotInd = indecesOfDots.at(0);
        bool isDotOfDiv = EvalDotOfDiv(middleSeq, sequence, dotInd);
        if (not isDotOfDiv) {
            // This is a dot of augmentation
            sum = GetValueInUnit(GetValueInMinims(middleSeq), DURATION_semibrevis);
            FindDurQuals(sequence, sum);
        }
    }
    // 3. More than one dot in middle sequence
    else if (numberOfDots > 1) {
        // Take first dot and evaluate if it is a dot of imperfection (check if it is "integer number of semibreves"
        // away from the beginning of the sequence, and if the rest of the sequence still sums an integer number)
        int startDotInd = indecesOfDots.at(0);
        bool isStartDotOfDiv = EvalDotOfDiv(middleSeq, sequence, startDotInd);

        // Take last dot and evaluate if it is a dot of division (check if it is "integer number of semibreves" away
        // from the end of the sequence, and if the rest of the sequence still sums an integer number)
        int endDotInd = indecesOfDots.at(indecesOfDots.size() - 1);
        bool isEndDotOfDiv = EvalDotOfDiv(middleSeq, sequence, endDotInd);

        // If neither, all dots are dots of augmentation
        if (not isStartDotOfDiv and not isEndDotOfDiv) {
            // This is a dot of augmentation
            sum = GetValueInUnit(GetValueInMinims(middleSeq), DURATION_semibrevis);
            FindDurQuals(sequence, sum);
        }
    }
}

void ScoringUpFunctor::FindDurQuals(const ArrayOfElementDurPairs &sequence, double valueInSmallerUnit)
{
    double sum = valueInSmallerUnit; // JUST IN THIS CASE
    int remainder = (int)sum % 3;

    // CHECK SUM --> IS IT INTEGER when dot of division???
    // CHECK REMAINDER

    // Candidates and Flags:
    /// The following 'candidate' variables are used to evaluate whether a note is a candidate for a given context-based
    /// modification (i.e., imperfection app, imperfection apa, or alteration). When the 'candidate' variable is NULL,
    /// then the note is not candidate for that given type of modification (e.g., when alterationCandidate is NULL, the
    /// penultimate note of the sequence is not a candidate for alteration; when the impappCandidate is NULL, the first
    /// note of the sequence is not candidate for an imperfection a parte post; and when the impapaCandidate is NULL,
    /// the last note of the sequence is not candidate for an imperfection a parte ante).
    Note *impappCandidate;
    Note *impapaCandidate;
    Note *alterationCandidate;
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
                // Evaluate 'default' case: Imperfection APP
                impappCandidate = ImperfectionAPP(sequence);
                if (impappCandidate) {
                    impappCandidate->SetDurQuality(DURQUALITY_mensural_imperfecta);
                    break;
                }
                // Evaluate 'alternative' case: Imperfection APA
                impapaCandidate = ImperfectionAPA(sequence);
                if (impapaCandidate) {
                    impapaCandidate->SetDurQuality(DURQUALITY_mensural_imperfecta);
                    break;
                }
                // No case
                std::cout << "NO OPTION!" << std::endl;
                break;
            case 2:
                // Evaluate 'default' case: Alteration
                alterationCandidate = Alteration(sequence);
                if (alterationCandidate && !dotOfImperf) {
                    alterationCandidate->SetDurQuality(DURQUALITY_mensural_altera);
                    break;
                }
                // Evaluate 'alternative' case: Imperfection APP and Imperfection APA
                impappCandidate = ImperfectionAPP(sequence);
                impapaCandidate = ImperfectionAPA(sequence);
                if (impappCandidate && impapaCandidate) {
                    impappCandidate->SetDurQuality(DURQUALITY_mensural_imperfecta);
                    impapaCandidate->SetDurQuality(DURQUALITY_mensural_imperfecta);
                    break;
                }
                // No case
                std::cout << "NO OPTION!" << std::endl;
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
                // Evaluate 'default' case: Imperfection APP and Alteration
                impappCandidate = ImperfectionAPP(sequence);
                alterationCandidate = Alteration(sequence);
                if (impappCandidate && alterationCandidate) {
                    impappCandidate->SetDurQuality(DURQUALITY_mensural_imperfecta);
                    alterationCandidate->SetDurQuality(DURQUALITY_mensural_altera);
                    break;
                }
                // Evaluate 'alternative' case: No modifications
                break;
            case 1:
                // Evaluate 'default' case: Imperfection APP
                impappCandidate = ImperfectionAPP(sequence);
                if (impappCandidate) {
                    impappCandidate->SetDurQuality(DURQUALITY_mensural_imperfecta);
                    break;
                }
                // Evaluate 'alternative' case: Imperfection APA
                impapaCandidate = ImperfectionAPA(sequence);
                if (impapaCandidate) {
                    impapaCandidate->SetDurQuality(DURQUALITY_mensural_imperfecta);
                    break;
                } // No case
                std::cout << "NO OPTION!" << std::endl;
                break;
            case 2:
                // Evaluate 'default' case: Imperfection APP and Imperfection APA
                impappCandidate = ImperfectionAPP(sequence);
                impapaCandidate = ImperfectionAPA(sequence);
                if (impappCandidate && impapaCandidate && !simileAnteSimile) {
                    impappCandidate->SetDurQuality(DURQUALITY_mensural_imperfecta);
                    impapaCandidate->SetDurQuality(DURQUALITY_mensural_imperfecta);
                    break;
                }
                // Evaluate 'alternative' case: Alteration
                alterationCandidate = Alteration(sequence);
                if (alterationCandidate) {
                    alterationCandidate->SetDurQuality(DURQUALITY_mensural_altera);
                    break;
                }
                // No case
                std::cout << "NO OPTION!" << std::endl;
                break;
        }
    }
}
double ScoringUpFunctor::GetValueInMinims(const ArrayOfElementDurPairs &middleSeq)
{
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
    return sum;
}

double ScoringUpFunctor::GetValueInUnit(double valueInMinims, data_DURATION unit)
{
    double valueInUnit = 0.0;
    if (unit == DURATION_semibrevis) {
        valueInUnit = valueInMinims / 2;
        // imperfect mensur
        // MISSING perfect mensur
    }
    else if (unit == DURATION_brevis) {
        valueInUnit = valueInMinims / 4;
        // imperfect mensur
        // MISSING perfect mensur
    }
    else if (unit == DURATION_longa) {
        valueInUnit = valueInMinims / 8;
        // imperfect mensur
        // MISSING perfect mensur
    }
    else if (unit == DURATION_maxima) {
        valueInUnit = valueInMinims / 16;
        // imperfect mensur
        // MISSING perfect mensur
    }
    return valueInUnit;
}

double ScoringUpFunctor::GetDurNumberValue(
    const std::pair<LayerElement *, data_DURATION> &elementDurPair, bool followedByDot, LayerElement *nextElement)
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
    // int longaDefaultVal = m_modusMinor * m_tempus * m_prolatio;
    int brevisDefaultVal = m_tempus * m_prolatio;
    int semibrevisDefaultVal = m_prolatio;
    double durnum = 0;
    switch (dur) {
        case DURATION_longa:
            if (m_modusMinor == 3 || durquality == DURQUALITY_mensural_perfecta || followedByDot) {
                durnum = 3 * brevisDefaultVal;
            }
            else if (m_modusMinor == 2 || durquality == DURQUALITY_mensural_imperfecta) {
                durnum = 2 * brevisDefaultVal;
            }
            break;
        case DURATION_brevis:
            if (m_tempus == 3 || durquality == DURQUALITY_mensural_perfecta || followedByDot) {
                durnum = 3 * semibrevisDefaultVal;
            }
            else if (m_tempus == 2 || durquality == DURQUALITY_mensural_imperfecta) {
                durnum = 2 * semibrevisDefaultVal;
            }
            break;
        case DURATION_semibrevis:
            if (m_prolatio == 3 || durquality == DURQUALITY_mensural_perfecta || followedByDot) {
                durnum = 3;
            }
            else if (m_prolatio == 2 || durquality == DURQUALITY_mensural_imperfecta) {
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

Note *ScoringUpFunctor::ImperfectionAPP(const ArrayOfElementDurPairs &sequence)
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
            return NULL;
        }
        else {
            /// Imperfection a.p.p. (i.e., imperfection by the following notes), unless the note has already been
            /// modified by the previous sequence
            Note *firstNote = vrv_cast<Note *>(firstElement);
            assert(firstNote);
            if (firstNote->HasDurQuality()) {
                /// Already modified before
                return NULL;
            }
            else {
                /// Imperfection a.p.p.
                // firstNote->SetDurQuality(DURQUALITY_mensural_imperfecta);
                return firstNote;
            }
        }
    }
    else {
        return NULL;
    }
}

Note *ScoringUpFunctor::ImperfectionAPA(const ArrayOfElementDurPairs &sequence)
{
    std::pair<LayerElement *, data_DURATION> lastElementDurPair = sequence.at(sequence.size() - 1);
    LayerElement *lastElement = lastElementDurPair.first;
    data_DURATION lastDur = lastElementDurPair.second;
    /// Evaluates if the last note in the sequence is a brevis. If it is, then it imperfects it (and returns true). If
    /// it is a rest or a larger note, then it forbids imperfection (and returns false).
    if (lastElement->Is(NOTE) && lastDur == DURATION_brevis) {
        Note *lastNote = vrv_cast<Note *>(lastElement);
        assert(lastNote);
        // lastNote->SetDurQuality(DURQUALITY_mensural_imperfecta);
        return lastNote;
    }
    else {
        return NULL;
    }
}

Note *ScoringUpFunctor::Alteration(const ArrayOfElementDurPairs &sequence)
{
    std::pair<LayerElement *, data_DURATION> penultElementDurPair = sequence.at(sequence.size() - 2);
    LayerElement *penultElement = penultElementDurPair.first;
    data_DURATION penultDur = penultElementDurPair.second;
    /// Evaluates what is the type of the penultimate element in the sequence. If it is a rest, it forbids Alteration
    /// (returns false). If it is a note, and the note is a 'semibrevis', it alters the note (and returns true).
    if (penultElement->Is(NOTE) && penultDur == DURATION_semibrevis) {
        Note *penultNote = vrv_cast<Note *>(penultElement);
        assert(penultNote);
        // penultNote->SetDurQuality(DURQUALITY_mensural_altera);
        return penultNote;
    }
    else {
        return NULL;
    }
}

bool ScoringUpFunctor::LeavePerfect(const ArrayOfElementDurPairs &sequence)
{
    return true;
}

} // namespace vrv

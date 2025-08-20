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
    m_currentMensur = NULL;
    std::vector<std::pair<MensInfo, ArrayOfElementDurPairs>> m_voiceData = {};
    ArrayOfElementDurPairs m_dursInVoiceWithSameMensur = {};
    std::vector<ArrayOfElementDurPairs> m_listOfSequences = {};
    std::list<std::pair<Note *, Dot *>> m_listOfAugNotesDotsPairs = {};
    std::list<std::pair<Note *, Dot *>> m_listOfPerfNotesDotsPairs = {};
}

FunctorCode ScoringUpFunctor::VisitLayerEnd(Layer *layer)
{
    // Add the last passage (as there won't be any more changes in mensuration)
    m_voiceData.insert(m_voiceData.end(), { m_mensInfo, m_dursInVoiceWithSameMensur });
    // Start: (assumption, mensuration is defined in layer, even the starting one; staffDef mensuration definitions will
    // be ignored for now)
    for (std::pair<MensInfo, ArrayOfElementDurPairs> mensurPassage : m_voiceData) {
        m_mensInfo = mensurPassage.first;
        ArrayOfElementDurPairs dursInVoiceWithSameMensur = mensurPassage.second;
        // Process each perfect mensuration passage from lowest to highest note level
        if (m_mensInfo.prolatio == 3) {
            this->ProcessPerfectMensurPassage(dursInVoiceWithSameMensur, DURATION_semibrevis);
        }
        if (m_mensInfo.tempus == 3) {
            this->ProcessPerfectMensurPassage(dursInVoiceWithSameMensur, DURATION_brevis);
        }
        if (m_mensInfo.modusminor == 3) {
            this->ProcessPerfectMensurPassage(dursInVoiceWithSameMensur, DURATION_longa);
        }
        if (m_mensInfo.modusmaior == 3) {
            this->ProcessPerfectMensurPassage(dursInVoiceWithSameMensur, DURATION_maxima);
        }
    }
    // restart for next voice (layer)
    m_voiceData = {};
    m_dursInVoiceWithSameMensur = {};

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
        m_dursInVoiceWithSameMensur.insert(m_dursInVoiceWithSameMensur.end(), { element, dur });
    }
    else if (element->Is(MENSUR)) {
        // The new mensuration indicates the end of the passage with the old mensuration;
        // therefore, (1) let's store the old passage and its mensuration,
        m_voiceData.insert(m_voiceData.end(), { m_mensInfo, m_dursInVoiceWithSameMensur });
        // and (2) start collecting the data for the new passage
        this->m_currentMensur = vrv_cast<Mensur *>(element);
        m_mensInfo.modusmaior = abs(m_currentMensur->GetModusmaior());
        m_mensInfo.modusminor = abs(m_currentMensur->GetModusminor());
        m_mensInfo.tempus = abs(m_currentMensur->GetTempus());
        m_mensInfo.prolatio = abs(m_currentMensur->GetProlatio());
        // Establish the mensuration of the new passage and empty the m_dursInVoiceWithSameMensur vector to start
        // collecting the notes, rests, and dots in the new passage
        m_dursInVoiceWithSameMensur = {};
    }
    return FUNCTOR_CONTINUE;
}

void ScoringUpFunctor::ProcessPerfectMensurPassage(
    const ArrayOfElementDurPairs &m_dursInVoiceWithSameMensur, data_DURATION noteLevel)
{
    m_listOfSequences = this->SubdivideIntoBoundedSequences(m_dursInVoiceWithSameMensur, noteLevel);
    this->ProcessBoundedSequences(m_listOfSequences, noteLevel);
}

// Functions to find and process the bounded sequences:
// 1. Find the bounded sequences by dividing all the notes in a given mensuration and voice into sequences bounded by
// "supposedly" perfect notes
std::vector<ArrayOfElementDurPairs> ScoringUpFunctor::SubdivideIntoBoundedSequences(
    const ArrayOfElementDurPairs &dursInVoiceSameMensur, data_DURATION boundUnit)
{
    std::vector<ArrayOfElementDurPairs> listOfBoundedSequences = {};
    ArrayOfElementDurPairs boundedSequence = {};

    // Depending on the boundUnit which could be a semibrevis (when in major prolation), a breve (when in perfect
    // tempus), a longa (when in perfect modus minor), or a maxima (when in perfect modus major). The boundaries become
    // the boundUnit note and any note of a higher value:
    if (boundUnit == DURATION_semibrevis) {
        for (std::pair<LayerElement *, data_DURATION> elementDurPair : dursInVoiceSameMensur) {
            data_DURATION dur = elementDurPair.second;
            if (dur == DURATION_semibrevis || dur == DURATION_brevis || dur == DURATION_longa
                || dur == DURATION_maxima) {
                boundedSequence.insert(boundedSequence.end(), elementDurPair);
                listOfBoundedSequences.insert(listOfBoundedSequences.end(), boundedSequence);
                boundedSequence = { elementDurPair };
            }
            else {
                boundedSequence.insert(boundedSequence.end(), elementDurPair);
            }
        }
    }
    else if (boundUnit == DURATION_brevis) {
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
        }
    }
    else if (boundUnit == DURATION_longa) {
        for (std::pair<LayerElement *, data_DURATION> elementDurPair : dursInVoiceSameMensur) {
            data_DURATION dur = elementDurPair.second;
            if (dur == DURATION_longa || dur == DURATION_maxima) {
                boundedSequence.insert(boundedSequence.end(), elementDurPair);
                listOfBoundedSequences.insert(listOfBoundedSequences.end(), boundedSequence);
                boundedSequence = { elementDurPair };
            }
            else {
                boundedSequence.insert(boundedSequence.end(), elementDurPair);
            }
        }
    }
    else if (boundUnit == DURATION_maxima) {
        for (std::pair<LayerElement *, data_DURATION> elementDurPair : dursInVoiceSameMensur) {
            data_DURATION dur = elementDurPair.second;
            if (dur == DURATION_maxima) {
                boundedSequence.insert(boundedSequence.end(), elementDurPair);
                listOfBoundedSequences.insert(listOfBoundedSequences.end(), boundedSequence);
                boundedSequence = { elementDurPair };
            }
            else {
                boundedSequence.insert(boundedSequence.end(), elementDurPair);
            }
        }
    }
    return listOfBoundedSequences;
}

// 2. Process the bounded sequences, with the list of all bounded sequences as argument
void ScoringUpFunctor::ProcessBoundedSequences(
    const std::vector<ArrayOfElementDurPairs> &listOfSequences, data_DURATION boundUnit)
{
    for (ArrayOfElementDurPairs sequence : listOfSequences) {
        this->ProcessBoundedSequences(sequence, boundUnit);
    }
}

// 3. Process the bounded sequences, with the individual bounded sequences as argument
void ScoringUpFunctor::ProcessBoundedSequences(const ArrayOfElementDurPairs &sequence, data_DURATION boundUnit)
{
    // Get the notes in the middle of the boundaries of the sequence
    ArrayOfElementDurPairs middleSeq = this->GetBoundedNotes(sequence, boundUnit);

    // Check if there are dots in the middleSeq and how many. The dots in the middleSeq are the only ones that have the
    // possibility of being a dot of imperfection (or alteration) or a dot of augmentation---the dots of perfection are
    // not in the middleSeq.
    std::vector<int> indecesOfDots;
    for (int i = 0; i < (int)middleSeq.size(); i++) {
        std::pair<LayerElement *, data_DURATION> elementDurPair = middleSeq.at(i);
        if (elementDurPair.first->Is(DOT)) {
            indecesOfDots.insert(indecesOfDots.end(), i);
        }
    }
    int numberOfDots = (int)indecesOfDots.size();
    // 0. No dots
    double sum;
    if (numberOfDots == 0) {
        sum = this->GetValueInUnit(this->GetValueInMinims(middleSeq), boundUnit);
        this->FindDurQuals(sequence, sum, boundUnit);
        this->ApplyAugmentationsAndPerfections();
    }
    // 1. Single dot in middle sequence sequence
    else if (numberOfDots == 1) {
        // If there is one dot,
        int dotInd = indecesOfDots.at(0);
        bool isDotOfDiv = this->EvalDotOfDiv(middleSeq, sequence, dotInd, boundUnit);
        if (not isDotOfDiv) {
            // This is a dot of augmentation
            sum = this->GetValueInUnit(this->GetValueInMinims(middleSeq), boundUnit);
            this->FindDurQuals(sequence, sum, boundUnit);
            this->ApplyAugmentationsAndPerfections();
        }
    }
    // 3. More than one dot in middle sequence
    else if (numberOfDots > 1) {
        // Take first dot and evaluate if it is a dot of imperfection (check if it is "integer number of semibreves"
        // away from the beginning of the sequence, and if the rest of the sequence still sums an integer number)
        int startDotInd = indecesOfDots.at(0);
        bool isStartDotOfDiv = this->EvalDotOfDiv(middleSeq, sequence, startDotInd, boundUnit);

        // Take last dot and evaluate if it is a dot of division (check if it is "integer number of semibreves" away
        // from the end of the sequence, and if the rest of the sequence still sums an integer number)
        int endDotInd = indecesOfDots.at(indecesOfDots.size() - 1);
        bool isEndDotOfDiv = this->EvalDotOfDiv(middleSeq, sequence, endDotInd, boundUnit);

        // If neither, all dots are dots of augmentation
        if (not isStartDotOfDiv and not isEndDotOfDiv) {
            // This is a dot of augmentation
            sum = this->GetValueInUnit(this->GetValueInMinims(middleSeq), boundUnit);
            this->FindDurQuals(sequence, sum, boundUnit);
            this->ApplyAugmentationsAndPerfections();
        }
    }
}

// 4. Get the notes bounded by the "supposedly" perfect notes (the 'middle note of the bounded sequence')
ArrayOfElementDurPairs ScoringUpFunctor::GetBoundedNotes(
    const ArrayOfElementDurPairs &sequence, data_DURATION boundUnit)
{
    data_DURATION firstNoteDur;
    ArrayOfElementDurPairs middleSeq = {};
    if (sequence.size() >= 2) {
        if (boundUnit == DURATION_semibrevis) {
            firstNoteDur = sequence.at(0).second;
            if (firstNoteDur == DURATION_minima || firstNoteDur == DURATION_semiminima || firstNoteDur == DURATION_fusa
                || firstNoteDur == DURATION_semifusa) {
                middleSeq = { sequence.begin(), sequence.end() - 1 };
            }
            else {
                middleSeq = { sequence.begin() + 1, sequence.end() - 1 };
            }
        }
        else if (boundUnit == DURATION_brevis) {
            firstNoteDur = sequence.at(0).second;
            if (firstNoteDur == DURATION_semibrevis || firstNoteDur == DURATION_minima
                || firstNoteDur == DURATION_semiminima || firstNoteDur == DURATION_fusa
                || firstNoteDur == DURATION_semifusa) {
                middleSeq = { sequence.begin(), sequence.end() - 1 };
            }
            else {
                middleSeq = { sequence.begin() + 1, sequence.end() - 1 };
            }
        }
        else if (boundUnit == DURATION_longa) {
            firstNoteDur = sequence.at(0).second;
            if (firstNoteDur == DURATION_brevis || firstNoteDur == DURATION_semibrevis
                || firstNoteDur == DURATION_minima || firstNoteDur == DURATION_semiminima
                || firstNoteDur == DURATION_fusa || firstNoteDur == DURATION_semifusa) {
                middleSeq = { sequence.begin(), sequence.end() - 1 };
            }
            else {
                middleSeq = { sequence.begin() + 1, sequence.end() - 1 };
            }
        }
        else if (boundUnit == DURATION_maxima) {
            firstNoteDur = sequence.at(0).second;
            if (firstNoteDur == DURATION_longa || firstNoteDur == DURATION_brevis || firstNoteDur == DURATION_semibrevis
                || firstNoteDur == DURATION_minima || firstNoteDur == DURATION_semiminima
                || firstNoteDur == DURATION_fusa || firstNoteDur == DURATION_semifusa) {
                middleSeq = { sequence.begin(), sequence.end() - 1 };
            }
            else {
                middleSeq = { sequence.begin() + 1, sequence.end() - 1 };
            }
        }
    }
    return middleSeq;
}

// Get the value of a note or collection of consecutinve notes in a given unit (minims, semibreves, or breves):
// 1. Get value of the single note in minims
double ScoringUpFunctor::GetDurNumberValue(
    const std::pair<LayerElement *, data_DURATION> &elementDurPair, bool followedByDot, LayerElement *nextElement)
{
    data_DURQUALITY_mensural durquality = DURQUALITY_mensural_NONE;
    data_DURATION dur = elementDurPair.second;
    Note *note = vrv_cast<Note *>(elementDurPair.first);
    if (!note) return 0.0;
    durquality = note->GetDurQuality();
    int longaDefaultVal = m_mensInfo.modusminor * m_mensInfo.tempus * m_mensInfo.prolatio;
    int brevisDefaultVal = m_mensInfo.tempus * m_mensInfo.prolatio;
    int semibrevisDefaultVal = m_mensInfo.prolatio;
    double durnum = 0;
    switch (dur) {
        case DURATION_longa:
            if (durquality == DURQUALITY_mensural_altera) {
                durnum = 2 * longaDefaultVal;
            }
            else if (m_mensInfo.modusminor == 3 || durquality == DURQUALITY_mensural_perfecta || followedByDot) {
                durnum = 3 * brevisDefaultVal;
                if (m_mensInfo.modusminor == 2 && followedByDot) {
                    // Candidate for augmentation (<dot form="aug"> and @num="2", @numbase="3")
                    Dot *dot = vrv_cast<Dot *>(nextElement);
                    m_listOfAugNotesDotsPairs.push_back({ note, dot });
                }
                else if (m_mensInfo.modusminor == 3 && followedByDot) {
                    // Candidate for dot of perfection (<dot form="div"> and @dur.quality="perfecta")
                    Dot *dot = vrv_cast<Dot *>(nextElement);
                    m_listOfPerfNotesDotsPairs.push_back({ note, dot });
                }
            }
            else if (m_mensInfo.modusminor == 2 || durquality == DURQUALITY_mensural_imperfecta) {
                durnum = 2 * brevisDefaultVal;
            }
            break;
        case DURATION_brevis:
            if (durquality == DURQUALITY_mensural_altera) {
                durnum = 2 * brevisDefaultVal;
            }
            else if (m_mensInfo.tempus == 3 || durquality == DURQUALITY_mensural_perfecta || followedByDot) {
                durnum = 3 * semibrevisDefaultVal;
                if (m_mensInfo.tempus == 2 && followedByDot) {
                    // Candidate for augmentation (<dot form="aug"> and @num="2", @numbase="3")
                    Dot *dot = vrv_cast<Dot *>(nextElement);
                    m_listOfAugNotesDotsPairs.push_back({ note, dot });
                }
                else if (m_mensInfo.tempus == 3 && followedByDot) {
                    // Candidate for dot of perfection (<dot form="div"> and @dur.quality="perfecta")
                    Dot *dot = vrv_cast<Dot *>(nextElement);
                    m_listOfPerfNotesDotsPairs.push_back({ note, dot });
                }
            }
            else if (m_mensInfo.tempus == 2 || durquality == DURQUALITY_mensural_imperfecta) {
                durnum = 2 * semibrevisDefaultVal;
            }
            break;
        case DURATION_semibrevis:
            if (durquality == DURQUALITY_mensural_altera) {
                durnum = 2 * semibrevisDefaultVal;
            }
            else if (m_mensInfo.prolatio == 3 || durquality == DURQUALITY_mensural_perfecta || followedByDot) {
                durnum = 3;
                if (m_mensInfo.prolatio == 2 && followedByDot) {
                    // Candidate for augmentation (<dot form="aug"> and @num="2", @numbase="3")
                    Dot *dot = vrv_cast<Dot *>(nextElement);
                    m_listOfAugNotesDotsPairs.push_back({ note, dot });
                }
                else if (m_mensInfo.prolatio == 3 && followedByDot) {
                    // Candidate for dot of perfection (<dot form="div"> and @dur.quality="perfecta")
                    Dot *dot = vrv_cast<Dot *>(nextElement);
                    m_listOfPerfNotesDotsPairs.push_back({ note, dot });
                }
            }
            else if (m_mensInfo.prolatio == 2 || durquality == DURQUALITY_mensural_imperfecta) {
                durnum = 2;
            }
            break;
        case DURATION_minima:
            if (followedByDot) {
                durnum = 1.5;
                // Candidate for augmentation (<dot form="aug"> and @num="2", @numbase="3")
                Dot *dot = vrv_cast<Dot *>(nextElement);
                m_listOfAugNotesDotsPairs.push_back({ note, dot });
            }
            else if (durquality == DURQUALITY_mensural_altera) {
                durnum = 2;
            }
            else {
                durnum = 1;
            }
            break;
        case DURATION_semiminima:
            if (followedByDot) {
                durnum = 0.75;
                // Candidate for augmentation (<dot form="aug"> and @num="2", @numbase="3")
                Dot *dot = vrv_cast<Dot *>(nextElement);
                m_listOfAugNotesDotsPairs.push_back({ note, dot });
            }
            else {
                durnum = 0.5;
            }
            break;
        case DURATION_fusa:
            if (followedByDot) {
                durnum = 0.375;
                // Candidate for augmentation (<dot form="aug"> and @num="2", @numbase="3")
                Dot *dot = vrv_cast<Dot *>(nextElement);
                m_listOfAugNotesDotsPairs.push_back({ note, dot });
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

// 2. Get value of a collection of notes in minims
double ScoringUpFunctor::GetValueInMinims(const ArrayOfElementDurPairs &middleSeq)
{
    // Value in minims:
    double sum = 0;
    bool followedByDot = false;
    LayerElement *nextElement = NULL;
    for (int i = 0; i < (int)middleSeq.size(); i++) {
        std::pair<LayerElement *, data_DURATION> elementDurPair = middleSeq.at(i);
        // Check if there is a dot after the element being evaluated
        if (i + 1 < (int)middleSeq.size()) {
            nextElement = middleSeq.at(i + 1).first;
            followedByDot = nextElement->Is(DOT);
        }
        else {
            followedByDot = false;
        }
        sum += this->GetDurNumberValue(elementDurPair, followedByDot, nextElement);
    }
    return sum;
}

// 3. Get value of a collection of notes in a given note unit (e.g., in semibrevis or brevis)
double ScoringUpFunctor::GetValueInUnit(double valueInMinims, data_DURATION unit)
{
    double valueInUnit = 0.0;
    if (unit == DURATION_semibrevis) {
        valueInUnit = valueInMinims;
    }
    else if (unit == DURATION_brevis) {
        valueInUnit = valueInMinims / (m_mensInfo.prolatio);
    }
    else if (unit == DURATION_longa) {
        valueInUnit = valueInMinims / (m_mensInfo.prolatio * m_mensInfo.tempus);
    }
    else if (unit == DURATION_maxima) {
        valueInUnit = valueInMinims / (m_mensInfo.prolatio * m_mensInfo.tempus * m_mensInfo.modusminor);
    }
    return valueInUnit;
}

// Find the quality of notes by applying the "principles of imperfection and alteration" based on the number of notes
// between a bounded sequence
void ScoringUpFunctor::FindDurQuals(
    const ArrayOfElementDurPairs &sequence, double valueInSmallerUnit, data_DURATION boundUnit)
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
                impappCandidate = this->ImperfectionAPP(sequence, boundUnit);
                if (impappCandidate) {
                    impappCandidate->SetDurQuality(DURQUALITY_mensural_imperfecta);
                    break;
                }
                // Evaluate 'alternative' case: Imperfection APA
                impapaCandidate = this->ImperfectionAPA(sequence, boundUnit);
                if (impapaCandidate) {
                    impapaCandidate->SetDurQuality(DURQUALITY_mensural_imperfecta);
                    break;
                }
                // No case
                std::cout << "NO OPTION!" << std::endl;
                break;
            case 2:
                // Evaluate 'default' case: Alteration
                alterationCandidate = this->Alteration(sequence, boundUnit);
                if (alterationCandidate && !dotOfImperf) {
                    alterationCandidate->SetDurQuality(DURQUALITY_mensural_altera);
                    break;
                }
                // Evaluate 'alternative' case: Imperfection APP and Imperfection APA
                impappCandidate = this->ImperfectionAPP(sequence, boundUnit);
                impapaCandidate = this->ImperfectionAPA(sequence, boundUnit);
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
        bool leavePerfectFlag = this->LeavePerfect(sequence, boundUnit);
        if (!leavePerfectFlag || dotOfImperf) {
        }
    }
    else { // For sum > 3
        switch (remainder) {
            case 0:
                // Evaluate 'default' case: Imperfection APP and Alteration
                impappCandidate = this->ImperfectionAPP(sequence, boundUnit);
                alterationCandidate = this->Alteration(sequence, boundUnit);
                if (impappCandidate && alterationCandidate) {
                    impappCandidate->SetDurQuality(DURQUALITY_mensural_imperfecta);
                    alterationCandidate->SetDurQuality(DURQUALITY_mensural_altera);
                    break;
                }
                // Evaluate 'alternative' case: No modifications
                break;
            case 1:
                // Evaluate 'default' case: Imperfection APP
                impappCandidate = this->ImperfectionAPP(sequence, boundUnit);
                if (impappCandidate) {
                    impappCandidate->SetDurQuality(DURQUALITY_mensural_imperfecta);
                    break;
                }
                // Evaluate 'alternative' case: Imperfection APA
                impapaCandidate = this->ImperfectionAPA(sequence, boundUnit);
                if (impapaCandidate) {
                    impapaCandidate->SetDurQuality(DURQUALITY_mensural_imperfecta);
                    break;
                } // No case
                std::cout << "NO OPTION!" << std::endl;
                break;
            case 2:
                // Evaluate 'default' case: Imperfection APP and Imperfection APA
                impappCandidate = this->ImperfectionAPP(sequence, boundUnit);
                impapaCandidate = this->ImperfectionAPA(sequence, boundUnit);
                if (impappCandidate && impapaCandidate && !simileAnteSimile) {
                    impappCandidate->SetDurQuality(DURQUALITY_mensural_imperfecta);
                    impapaCandidate->SetDurQuality(DURQUALITY_mensural_imperfecta);
                    break;
                }
                // Evaluate 'alternative' case: Alteration
                alterationCandidate = this->Alteration(sequence, boundUnit);
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

// Context-dependent modifications:
// 1. Imperfection a parte post (imperfection by the notes that follow)
Note *ScoringUpFunctor::ImperfectionAPP(const ArrayOfElementDurPairs &sequence, data_DURATION boundUnit)
{
    std::pair<LayerElement *, data_DURATION> firstElementDurPair = sequence.at(0);
    LayerElement *firstElement = firstElementDurPair.first;
    data_DURATION firstDur = firstElementDurPair.second;
    LayerElement *nextElement;
    /// Evaluates if the first note in the sequence is a brevis. If it is, then it imperfects it (and returns true). If
    /// it is a rest or a larger note, then it forbids imperfection (and returns false).
    if (firstElement->Is(NOTE) && firstDur == boundUnit) {
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

// 2. Imperfection a parte ante (imperfection by the notes preceding)
Note *ScoringUpFunctor::ImperfectionAPA(const ArrayOfElementDurPairs &sequence, data_DURATION boundUnit)
{
    std::pair<LayerElement *, data_DURATION> lastElementDurPair = sequence.at(sequence.size() - 1);
    LayerElement *lastElement = lastElementDurPair.first;
    data_DURATION lastDur = lastElementDurPair.second;
    /// Evaluates if the last note in the sequence is a brevis. If it is, then it imperfects it (and returns true). If
    /// it is a rest or a larger note, then it forbids imperfection (and returns false).
    if (lastElement->Is(NOTE) && lastDur == boundUnit) {
        Note *lastNote = vrv_cast<Note *>(lastElement);
        assert(lastNote);
        // lastNote->SetDurQuality(DURQUALITY_mensural_imperfecta);
        return lastNote;
    }
    else {
        return NULL;
    }
}

// 3. Alteration (making a note twice as long)
Note *ScoringUpFunctor::Alteration(const ArrayOfElementDurPairs &sequence, data_DURATION boundUnit)
{
    std::pair<LayerElement *, data_DURATION> penultElementDurPair = sequence.at(sequence.size() - 2);
    LayerElement *penultElement = penultElementDurPair.first;
    data_DURATION penultDur = penultElementDurPair.second;
    /// Evaluates what is the type of the penultimate element in the sequence. If it is a rest, it forbids Alteration
    /// (returns false). If it is a note, and the note is a 'semibrevis', it alters the note (and returns true).
    std::map<data_DURATION, data_DURATION> nextSmallLevel
        = { { DURATION_maxima, DURATION_longa }, { DURATION_longa, DURATION_brevis },
              { DURATION_brevis, DURATION_semibrevis }, { DURATION_semibrevis, DURATION_minima } };
    data_DURATION smallerValue = nextSmallLevel[boundUnit];
    if (penultElement->Is(NOTE) && penultDur == smallerValue) {
        Note *penultNote = vrv_cast<Note *>(penultElement);
        assert(penultNote);
        // penultNote->SetDurQuality(DURQUALITY_mensural_altera);
        return penultNote;
    }
    else {
        return NULL;
    }
}

// 4. No modification
bool ScoringUpFunctor::LeavePerfect(const ArrayOfElementDurPairs &sequence, data_DURATION boundUnit)
{
    return true;
}

// 5. Apply augmentations (due to a dot of augmentation) and perfections (due to a dot of perfection)
void ScoringUpFunctor::ApplyAugmentationsAndPerfections()
{
    for (std::pair<Note *, Dot *> pairNoteAndDot : m_listOfAugNotesDotsPairs) {
        pairNoteAndDot.first->SetDurQuality(DURQUALITY_mensural_perfecta);
        pairNoteAndDot.second->SetForm(dotLog_FORM_aug);
    }
    for (std::pair<Note *, Dot *> pairNoteAndDot : m_listOfPerfNotesDotsPairs) {
        pairNoteAndDot.first->SetDurQuality(DURQUALITY_mensural_perfecta);
        pairNoteAndDot.second->SetForm(dotLog_FORM_div);
    }
    m_listOfAugNotesDotsPairs.clear();
    m_listOfPerfNotesDotsPairs.clear();
}

// Evaluation of whether a dot in a given position (dotInd) is acting as a dot of division. If it is, apply the
// principles of imperfection and alteration to the sequence of notes preceding and following the dot, and return true.
bool ScoringUpFunctor::EvalDotOfDiv(
    const ArrayOfElementDurPairs &middleSeq, const ArrayOfElementDurPairs &sequence, int dotInd, data_DURATION unit)
{
    // Initial assumption: the sequence doesn't have a dot of division
    bool flagDotOfDiv = false;
    // Evaluate if the dot passed to the method (by the dotInd) is behaving as a dot of division
    ArrayOfElementDurPairs middleSeq1 = { middleSeq.begin(), middleSeq.begin() + dotInd };
    ArrayOfElementDurPairs middleSeq2 = { middleSeq.begin() + dotInd + 1, middleSeq.end() };
    double sum1 = this->GetValueInUnit(this->GetValueInMinims(middleSeq1), unit);
    double sum2 = this->GetValueInUnit(this->GetValueInMinims(middleSeq2), unit);
    // This condition is to discard it being a dot of perfection
    if (middleSeq1.size() != 0) {
        // This other condition is to evaluate if it is a dot of division
        if ((sum1 == (int)sum1) and (sum2 == (int)sum2)) {
            // This is a "dot of division"
            flagDotOfDiv = true;
            // Then it divides the sequence into the following two:
            ArrayOfElementDurPairs seq1 = { sequence.begin(), sequence.begin() + dotInd + 2 };
            ArrayOfElementDurPairs seq2 = { sequence.begin() + dotInd + 2, sequence.end() };
            // Encode the dot of division:
            LayerElement *dotElement = sequence.at(dotInd + 1).first;
            Dot *dot = vrv_cast<Dot *>(dotElement);
            dot->SetForm(dotLog_FORM_div);
            // Encode its effect on the notes preceding and following:
            this->FindDurQuals(seq1, sum1, unit);
            this->FindDurQuals(seq2, sum2, unit);
            this->ApplyAugmentationsAndPerfections();
        }
        else {
            m_listOfAugNotesDotsPairs.clear();
            m_listOfPerfNotesDotsPairs.clear();
        }
    }
    return flagDotOfDiv;
}

} // namespace vrv

/////////////////////////////////////////////////////////////////////////////
// Name:        transposefunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "transposefunctor.h"

//----------------------------------------------------------------------------

#include "harm.h"
#include "layer.h"
#include "mdiv.h"
#include "rest.h"
#include "score.h"
#include "staff.h"
#include "transposition.h"
#include "vrv.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// TransposeFunctor
//----------------------------------------------------------------------------

TransposeFunctor::TransposeFunctor(Doc *doc, Transposer *transposer) : DocFunctor(doc)
{
    m_transposer = transposer;
}

FunctorCode TransposeFunctor::VisitHarm(Harm *harm)
{
    unsigned int position = 0;
    TransPitch pitch;
    if (harm->GetRootPitch(pitch, position)) {
        m_transposer->Transpose(pitch);
        harm->SetRootPitch(pitch, position);
    }

    // Transpose bass notes (the "/F#" in "G#m7/F#")
    if (harm->GetBassPitch(pitch)) {
        m_transposer->Transpose(pitch);
        harm->SetBassPitch(pitch);
    }

    return FUNCTOR_SIBLINGS;
}

FunctorCode TransposeFunctor::VisitKeySig(KeySig *keySig)
{
    // Store current KeySig
    const int staffN = this->GetStaffNForKeySig(keySig);
    m_keySigForStaffN[staffN] = keySig;

    // Transpose
    const int sig = keySig->GetFifthsInt();

    int intervalClass = m_transposer->CircleOfFifthsToIntervalClass(sig);
    intervalClass = m_transposer->Transpose(intervalClass);
    int fifths = m_transposer->IntervalToCircleOfFifths(intervalClass);

    if (fifths == INVALID_INTERVAL_CLASS) {
        keySig->SetSig({ -1, ACCIDENTAL_WRITTEN_NONE });
    }
    else if (fifths < 0) {
        keySig->SetSig({ -fifths, ACCIDENTAL_WRITTEN_f });
    }
    else if (fifths > 0) {
        keySig->SetSig({ fifths, ACCIDENTAL_WRITTEN_s });
    }
    else {
        keySig->SetSig({ -1, ACCIDENTAL_WRITTEN_NONE });
    }

    // Also convert pname and accid attributes
    if (keySig->HasPname()) {
        TransPitch pitch = TransPitch(keySig->GetPname(), ACCIDENTAL_GESTURAL_NONE, keySig->GetAccid(), 4);
        m_transposer->Transpose(pitch);
        keySig->SetPname(pitch.GetPitchName());
        keySig->SetAccid(pitch.GetAccidW());
    }

    return FUNCTOR_SIBLINGS;
}

FunctorCode TransposeFunctor::VisitMdiv(Mdiv *mdiv)
{
    m_keySigForStaffN.clear();

    return FUNCTOR_CONTINUE;
}

FunctorCode TransposeFunctor::VisitNote(Note *note)
{
    if (!note->HasPname()) return FUNCTOR_SIBLINGS;

    TransPitch pitch = note->GetTransPitch();
    m_transposer->Transpose(pitch);

    const int staffN = note->GetAncestorStaff(RESOLVE_CROSS_STAFF)->GetN();
    const bool hasKeySig = m_keySigForStaffN.contains(staffN) || m_keySigForStaffN.contains(-1);
    note->UpdateFromTransPitch(pitch, hasKeySig);

    return FUNCTOR_SIBLINGS;
}

FunctorCode TransposeFunctor::VisitRest(Rest *rest)
{
    if ((!rest->HasOloc() || !rest->HasPloc()) && !rest->HasLoc()) return FUNCTOR_SIBLINGS;

    // Find whether current layer is top, middle (either one if multiple) or bottom
    Staff *parentStaff = rest->GetAncestorStaff();
    Layer *parentLayer = vrv_cast<Layer *>(rest->GetFirstAncestor(LAYER));
    assert(parentLayer);

    ListOfObjects objects = parentStaff->FindAllDescendantsByType(LAYER, false);
    const int layerCount = (int)objects.size();

    Layer *firstLayer = vrv_cast<Layer *>(objects.front());
    Layer *lastLayer = vrv_cast<Layer *>(objects.back());

    const bool isTopLayer = (firstLayer->GetN() == parentLayer->GetN());
    const bool isBottomLayer = (lastLayer->GetN() == parentLayer->GetN());

    // transpose based on @oloc and @ploc
    if (rest->HasOloc() && rest->HasPloc()) {
        const TransPitch centralLocation(6, 0, 4); // middle location of the staff
        TransPitch restLoc(rest->GetPloc() - PITCHNAME_c, 0, rest->GetOloc());
        m_transposer->Transpose(restLoc);
        const bool isRestOnSpace = static_cast<bool>((restLoc.m_oct * 7 + restLoc.m_pname) % 2);
        // on outer layers move rest on odd locations one line further
        // in middle layers tolerate even locations to not risk collisions
        if (layerCount > 1) {
            if (isTopLayer && isRestOnSpace) {
                ++restLoc;
            }
            else if (isBottomLayer && isRestOnSpace) {
                --restLoc;
            }
            if ((isTopLayer && (restLoc < centralLocation)) || (isBottomLayer && (restLoc > centralLocation))) {
                restLoc = centralLocation;
            }
        }

        rest->UpdateFromTransLoc(restLoc);
    }
    // transpose based on @loc
    else if (rest->HasLoc()) {
        constexpr int centralLocation = 4;
        int transval = m_transposer->GetTranspositionIntervalClass();
        int diatonic;
        int chromatic;
        m_transposer->IntervalToDiatonicChromatic(diatonic, chromatic, transval);
        int transposedLoc = rest->GetLoc() + diatonic;
        // on outer layers move rest on odd locations one line further
        // in middle layers tolerate even locations to not risk collisions
        if (layerCount > 1) {
            if (isTopLayer)
                transposedLoc += abs(transposedLoc % 2);
            else if (isBottomLayer)
                transposedLoc -= abs(transposedLoc % 2);
            if ((isTopLayer && (transposedLoc < centralLocation))
                || (isBottomLayer && (transposedLoc > centralLocation))) {
                transposedLoc = centralLocation;
            }
        }
        rest->SetLoc(transposedLoc);
    }

    return FUNCTOR_SIBLINGS;
}

FunctorCode TransposeFunctor::VisitScore(Score *score)
{
    ScoreDef *scoreDef = score->GetScoreDef();
    assert(scoreDef);

    if (m_transposer->IsValidIntervalName(m_transposition)) {
        m_transposer->SetTransposition(m_transposition);
    }
    else if (m_transposer->IsValidKeyTonic(m_transposition)) {
        // Find the starting key tonic of the data to use in calculating the tranposition interval:
        // Set transposition by key tonic.
        // Detect the current key from the keysignature.
        KeySig *keySig = vrv_cast<KeySig *>(scoreDef->FindDescendantByType(KEYSIG));
        // If there is no keysignature, assume it is C.
        TransPitch currentKey = TransPitch(0, 0, 0);
        if (keySig && keySig->HasPname()) {
            currentKey = TransPitch(keySig->GetPname(), ACCIDENTAL_GESTURAL_NONE, keySig->GetAccid(), 0);
        }
        else if (keySig) {
            // No tonic pitch in key signature, so infer from key signature.
            int fifthsInt = keySig->GetFifthsInt();
            // Check the keySig@mode is present (currently assuming major):
            currentKey = m_transposer->CircleOfFifthsToMajorTonic(fifthsInt);
            // need to add a dummy "0" key signature in score (staffDefs of staffDef).
        }
        m_transposer->SetTransposition(currentKey, m_transposition);
    }
    else if (m_transposer->IsValidSemitones(m_transposition)) {
        KeySig *keySig = vrv_cast<KeySig *>(scoreDef->FindDescendantByType(KEYSIG));
        int fifths = 0;
        if (keySig) {
            fifths = keySig->GetFifthsInt();
        }
        else {
            LogWarning("No key signature in data, assuming no key signature with no sharps/flats.");
            // need to add a dummy "0" key signature in score (staffDefs of staffDef).
        }
        m_transposer->SetTransposition(fifths, m_transposition);
    }
    else {
        LogWarning("Transposition is invalid: %s", m_transposition.c_str());
        // there is no transposition that can be done so do not try
        // to transpose any further (if continuing in this function,
        // there will not be an error, just that the transposition
        // will be at the unison, so no notes should change.
        return FUNCTOR_STOP;
    }

    // Evaluate functor on scoreDef
    scoreDef->Process(*this);

    return FUNCTOR_CONTINUE;
}

FunctorCode TransposeFunctor::VisitStaffDef(StaffDef *staffDef)
{
    if (!this->GetKeySigForStaffDef(staffDef)) {
        KeySig *keySig = new KeySig();
        staffDef->AddChild(keySig);
        LogWarning("Adding auxiliary KeySig for transposition");
    }

    return FUNCTOR_CONTINUE;
}

const KeySig *TransposeFunctor::GetKeySigForStaffDef(const StaffDef *staffDef) const
{
    const KeySig *keySig = vrv_cast<const KeySig *>(staffDef->FindDescendantByType(KEYSIG));
    if (!keySig) {
        const ScoreDef *scoreDef = vrv_cast<const ScoreDef *>(staffDef->GetFirstAncestor(SCOREDEF));
        keySig = vrv_cast<const KeySig *>(scoreDef->FindDescendantByType(KEYSIG, 1));
    }
    return keySig;
}

int TransposeFunctor::GetStaffNForKeySig(const KeySig *keySig) const
{
    int staffN = -1;
    const StaffDef *staffDef = vrv_cast<const StaffDef *>(keySig->GetFirstAncestor(STAFFDEF));
    if (staffDef) {
        staffN = staffDef->GetN();
    }
    else {
        const Staff *staff = keySig->GetAncestorStaff(ANCESTOR_ONLY, false);
        if (staff) staffN = staff->GetN();
    }
    return staffN;
}

//----------------------------------------------------------------------------
// TransposeSelectedMdivFunctor
//----------------------------------------------------------------------------

TransposeSelectedMdivFunctor::TransposeSelectedMdivFunctor(Doc *doc, Transposer *transposer)
    : TransposeFunctor(doc, transposer)
{
}

FunctorCode TransposeSelectedMdivFunctor::VisitMdiv(Mdiv *mdiv)
{
    TransposeFunctor::VisitMdiv(mdiv);

    m_currentMdivIDs.push_back(mdiv->GetID());

    return FUNCTOR_CONTINUE;
}

FunctorCode TransposeSelectedMdivFunctor::VisitPageMilestone(PageMilestoneEnd *pageMilestoneEnd)
{
    if (pageMilestoneEnd->GetStart() && pageMilestoneEnd->GetStart()->Is(MDIV)) {
        m_currentMdivIDs.pop_back();
    }
    return FUNCTOR_CONTINUE;
}

FunctorCode TransposeSelectedMdivFunctor::VisitScore(Score *score)
{
    // Check whether we are in the selected mdiv
    if (!m_selectedMdivID.empty()
        && (std::find(m_currentMdivIDs.begin(), m_currentMdivIDs.end(), m_selectedMdivID) == m_currentMdivIDs.end())) {
        return FUNCTOR_CONTINUE;
    }

    return TransposeFunctor::VisitScore(score);
}

FunctorCode TransposeSelectedMdivFunctor::VisitSystem(System *system)
{
    // Check whether we are in the selected mdiv
    if (!m_selectedMdivID.empty()
        && (std::find(m_currentMdivIDs.begin(), m_currentMdivIDs.end(), m_selectedMdivID) == m_currentMdivIDs.end())) {
        return FUNCTOR_SIBLINGS;
    }

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// TransposeToSoundingPitchFunctor
//----------------------------------------------------------------------------

TransposeToSoundingPitchFunctor::TransposeToSoundingPitchFunctor(Doc *doc, Transposer *transposer)
    : TransposeFunctor(doc, transposer)
{
}

FunctorCode TransposeToSoundingPitchFunctor::VisitMdiv(Mdiv *mdiv)
{
    TransposeFunctor::VisitMdiv(mdiv);

    m_transposeIntervalForStaffN.clear();

    return FUNCTOR_CONTINUE;
}

FunctorCode TransposeToSoundingPitchFunctor::VisitScore(Score *score)
{
    // Evaluate functor on scoreDef
    score->GetScoreDef()->Process(*this);

    return FUNCTOR_CONTINUE;
}

FunctorCode TransposeToSoundingPitchFunctor::VisitScoreDef(ScoreDef *scoreDef)
{
    // Set the transposition in order to transpose common key signatures
    // (i.e. encoded as ScoreDef attributes or direct KeySig children)
    const std::vector<int> staffNs = scoreDef->GetStaffNs();
    if (staffNs.empty()) {
        int transposeInterval = 0;
        if (!m_transposeIntervalForStaffN.empty()) {
            transposeInterval = m_transposeIntervalForStaffN.begin()->second;
        }
        m_transposer->SetTransposition(transposeInterval);
    }
    else {
        this->VisitStaffDef(scoreDef->GetStaffDef(staffNs.front()));
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode TransposeToSoundingPitchFunctor::VisitScoreDefEnd(ScoreDef *scoreDef)
{
    const bool hasScoreDefKeySig = m_keySigForStaffN.contains(-1);
    if (hasScoreDefKeySig) {
        bool showWarning = false;
        // Check if some staves are untransposed
        const int mapEntryCount = static_cast<int>(m_transposeIntervalForStaffN.size());
        if ((mapEntryCount > 0) && (mapEntryCount < (int)scoreDef->GetStaffNs().size())) {
            showWarning = true;
        }
        // Check if there are different transpositions
        auto iter = std::adjacent_find(m_transposeIntervalForStaffN.begin(), m_transposeIntervalForStaffN.end(),
            [](const auto &mapEntry1, const auto &mapEntry2) { return (mapEntry1.second != mapEntry2.second); });
        if (iter != m_transposeIntervalForStaffN.end()) {
            showWarning = true;
        }
        // Display warning
        if (showWarning) {
            LogWarning("Transpose to sounding pitch cannot handle different transpositions for ScoreDef key "
                       "signatures. Please encode KeySig as StaffDef attribute or child.");
        }
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode TransposeToSoundingPitchFunctor::VisitStaff(Staff *staff)
{
    this->UpdateTranspositionFromStaffN(staff);

    return FUNCTOR_CONTINUE;
}

FunctorCode TransposeToSoundingPitchFunctor::VisitStaffDef(StaffDef *staffDef)
{
    // Call base method (creates KeySig if missing)
    TransposeFunctor::VisitStaffDef(staffDef);

    const KeySig *keySig = this->GetKeySigForStaffDef(staffDef);

    // Determine and store the transposition interval (based on keySig)
    if (keySig && staffDef->HasTransSemi() && staffDef->HasN()) {
        const int fifths = keySig->GetFifthsInt();
        int semitones = staffDef->GetTransSemi();
        // Factor out octave transpositions
        const int sign = (semitones >= 0) ? +1 : -1;
        semitones = sign * (std::abs(semitones) % 24);
        m_transposer->SetTransposition(fifths, std::to_string(semitones));
        m_transposeIntervalForStaffN[staffDef->GetN()] = m_transposer->GetTranspositionIntervalClass();
        staffDef->ResetTransposition();
    }
    else {
        this->UpdateTranspositionFromStaffN(staffDef);
    }

    return FUNCTOR_CONTINUE;
}

void TransposeToSoundingPitchFunctor::UpdateTranspositionFromStaffN(const AttNInteger *staffN)
{
    int transposeInterval = 0;
    if (staffN->HasN() && m_transposeIntervalForStaffN.contains(staffN->GetN())) {
        transposeInterval = m_transposeIntervalForStaffN.at(staffN->GetN());
    }
    m_transposer->SetTransposition(transposeInterval);
}

} // namespace vrv

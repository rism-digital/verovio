/////////////////////////////////////////////////////////////////////////////
// Name:        convertfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "convertfunctor.h"

//----------------------------------------------------------------------------

#include "div.h"
#include "doc.h"
#include "ending.h"
#include "fermata.h"
#include "layer.h"
#include "mdiv.h"
#include "mrest.h"
#include "page.h"
#include "rest.h"
#include "score.h"
#include "section.h"
#include "staff.h"
#include "syllable.h"
#include "system.h"
#include "tie.h"
#include "vrv.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// ConvertToPageBasedFunctor
//----------------------------------------------------------------------------

ConvertToPageBasedFunctor::ConvertToPageBasedFunctor(Page *page) : Functor()
{
    m_currentSystem = NULL;
    m_page = page;
}

FunctorCode ConvertToPageBasedFunctor::VisitDiv(Div *div)
{
    assert(m_currentSystem);
    div->MoveItselfTo(m_currentSystem);

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToPageBasedFunctor::VisitEditorialElement(EditorialElement *editorialElement)
{
    assert(m_currentSystem);
    editorialElement->MoveItselfTo(m_currentSystem);

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToPageBasedFunctor::VisitEditorialElementEnd(EditorialElement *editorialElement)
{
    if (editorialElement->m_visibility == Visible) {
        editorialElement->ConvertToPageBasedMilestone(editorialElement, m_currentSystem);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToPageBasedFunctor::VisitEnding(Ending *ending)
{
    assert(m_currentSystem);
    ending->MoveItselfTo(m_currentSystem);

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToPageBasedFunctor::VisitEndingEnd(Ending *ending)
{
    ending->ConvertToPageBasedMilestone(ending, m_currentSystem);

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToPageBasedFunctor::VisitMeasure(Measure *measure)
{
    // Move itself to the pageBasedSystem - do not process children
    assert(m_currentSystem);
    measure->MoveItselfTo(m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

FunctorCode ConvertToPageBasedFunctor::VisitMdiv(Mdiv *mdiv)
{
    mdiv->MoveItselfTo(m_page);

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToPageBasedFunctor::VisitMdivEnd(Mdiv *mdiv)
{
    if (mdiv->m_visibility == Visible) {
        mdiv->ConvertToPageBasedMilestone(mdiv, m_page);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToPageBasedFunctor::VisitScore(Score *score)
{
    assert(!m_currentSystem);
    score->MoveItselfTo(m_page);
    System *system = new System();
    m_currentSystem = system;
    m_page->AddChild(system);

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToPageBasedFunctor::VisitScoreEnd(Score *score)
{
    score->ConvertToPageBasedMilestone(score, m_page);
    m_currentSystem = NULL;

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToPageBasedFunctor::VisitScoreDef(ScoreDef *scoreDef)
{
    // Move itself to the pageBasedSystem - do not process children
    assert(m_currentSystem);
    scoreDef->MoveItselfTo(m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

FunctorCode ConvertToPageBasedFunctor::VisitSection(Section *section)
{
    assert(m_currentSystem);
    section->MoveItselfTo(m_currentSystem);

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToPageBasedFunctor::VisitSectionEnd(Section *section)
{
    section->ConvertToPageBasedMilestone(section, m_currentSystem);

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToPageBasedFunctor::VisitSystemElement(SystemElement *systemElement)
{
    assert(m_currentSystem);
    systemElement->MoveItselfTo(m_currentSystem);

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// ConvertToCastOffMensuralFunctor
//----------------------------------------------------------------------------

ConvertToCastOffMensuralFunctor::ConvertToCastOffMensuralFunctor(
    Doc *doc, System *targetSystem, const IntTree *layerTree)
    : DocFunctor(doc)
{
    m_contentLayer = NULL;
    m_targetSystem = targetSystem;
    m_targetSubSystem = NULL;
    m_targetMeasure = NULL;
    m_targetStaff = NULL;
    m_targetLayer = NULL;
    m_segmentIdx = 0;
    m_segmentTotal = 0;
    m_layerTree = layerTree;
}

FunctorCode ConvertToCastOffMensuralFunctor::VisitBarLine(BarLine *barLine)
{
    assert(barLine->GetAlignment());
    assert(m_targetSubSystem);
    assert(m_targetLayer);

    // If this is the last barline of the layer, we will just move it and do not create a new segment
    bool isLast = (m_contentLayer->GetLast() == barLine);
    Object *next = m_contentLayer->GetNext(barLine);
    bool nextIsBarline = (next && next->Is(BARLINE));

    // See if we create proper measures and what to do with the barLine
    bool convertToMeasured = m_doc->GetOptions()->m_mensuralToMeasure.GetValue();

    if (convertToMeasured) {
        // barLine object will be deleted
        m_targetMeasure->SetRight(barLine->GetForm());
    }
    else {
        barLine->MoveItselfTo(m_targetLayer);
    }

    // Now we can return if this is the end barLine
    if (isLast || nextIsBarline) return FUNCTOR_SIBLINGS;

    for (int staffN : m_staffNs) {
        // The barline is missing in at least one of the staves - do not break here
        if (!barLine->GetAlignment()->HasAlignmentReference(staffN)) {
            // LogDebug("BarLine not on all staves %d %s", m_targetStaff->GetN(), barLine->GetClassName().c_str());
            return FUNCTOR_SIBLINGS;
        }
    }

    // Make a segment break
    // First case: add a new measure segment (e.g., first pass)
    if (m_targetSubSystem->GetChildCount() <= m_segmentIdx) {
        m_targetMeasure = new Measure(convertToMeasured);
        if (convertToMeasured) {
            m_targetMeasure->SetN(StringFormat("%d", m_segmentTotal + 1 + m_segmentIdx));
        }
        m_targetSubSystem->AddChild(m_targetMeasure);
        // Add a staff with same attributes as in the previous segment
        m_targetStaff = new Staff(*m_targetStaff);
        m_targetStaff->ClearChildren();
        m_targetStaff->CloneReset();
        m_targetMeasure->AddChild(m_targetStaff);
        // Add a layer also with the same attributes as in the previous segment
        m_targetLayer = new Layer(*m_targetLayer);
        m_targetLayer->ClearChildren();
        m_targetLayer->CloneReset();
        m_targetStaff->AddChild(m_targetLayer);
    }
    // Second case: retrieve the appropriate segment
    else {
        m_targetMeasure = dynamic_cast<Measure *>(m_targetSubSystem->GetChild(m_segmentIdx));
        // It must be there
        assert(m_targetMeasure);

        // Look if we already have the staff (e.g., with more than one layer)
        AttNIntegerComparison comparisonStaffN(STAFF, m_targetStaff->GetN());
        Staff *staff = vrv_cast<Staff *>(m_targetMeasure->FindDescendantByComparison(&comparisonStaffN));
        if (!staff) {
            staff = new Staff(*m_targetStaff);
            staff->ClearChildren();
            staff->CloneReset();
            m_targetMeasure->AddChild(staff);
        }
        m_targetStaff = staff;

        // Add a new layer as the new target
        m_targetLayer = new Layer(*m_targetLayer);
        m_targetLayer->ClearChildren();
        m_targetLayer->CloneReset();
        m_targetStaff->AddChild(m_targetLayer);
    }
    ++m_segmentIdx;

    return FUNCTOR_SIBLINGS;
}

FunctorCode ConvertToCastOffMensuralFunctor::VisitLayer(Layer *layer)
{
    m_contentLayer = layer;

    m_targetLayer = new Layer(*layer);
    m_targetLayer->ClearChildren();
    m_targetLayer->CloneReset();
    // Keep the xml:id of the layer in the first segment
    m_targetLayer->SwapID(layer);
    assert(m_targetStaff);
    m_targetStaff->AddChild(m_targetLayer);

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToCastOffMensuralFunctor::VisitMeasure(Measure *measure)
{
    // We are processing by staff/layer from the call below - we obviously do not want to loop...
    if (m_targetMeasure) {
        return FUNCTOR_CONTINUE;
    }

    bool convertToMeasured = m_doc->GetOptions()->m_mensuralToMeasure.GetValue();

    assert(m_targetSystem);
    assert(m_layerTree);

    // Create a temporary subsystem for receiving the measure segments
    System targetSubSystem;
    m_targetSubSystem = &targetSubSystem;

    // Create the first measure segment - problem: we are dropping the section element - we should create a score-based
    // MEI file instead
    Measure *targetMeasure = new Measure(convertToMeasured);
    if (convertToMeasured) {
        targetMeasure->SetN(StringFormat("%d", m_segmentTotal + 1));
    }
    m_targetSubSystem->AddChild(targetMeasure);

    Filters filters;
    Filters *previousFilters = this->SetFilters(&filters);

    // Now we can process by layer and move their content to (measure) segments
    for (const auto &staves : m_layerTree->child) {
        for (const auto &layers : staves.second.child) {
            // Create ad comparison object for each type / @n
            AttNIntegerComparison matchStaff(STAFF, staves.first);
            AttNIntegerComparison matchLayer(LAYER, layers.first);
            filters = { &matchStaff, &matchLayer };

            m_segmentIdx = 1;
            m_targetMeasure = targetMeasure;

            measure->Process(*this);
        }
    }

    this->SetFilters(previousFilters);

    m_targetMeasure = NULL;
    m_targetSubSystem = NULL;
    m_segmentTotal = targetSubSystem.GetChildCount();
    // Copy the measure segments to the final target segment
    m_targetSystem->MoveChildrenFrom(&targetSubSystem);

    return FUNCTOR_SIBLINGS;
}

FunctorCode ConvertToCastOffMensuralFunctor::VisitObject(Object *object)
{
    assert(object->GetParent());
    // We want to move only the children of the layer of any type (notes, editorial elements, etc)
    if (object->GetParent()->Is(LAYER)) {
        assert(m_targetLayer);
        object->MoveItselfTo(m_targetLayer);
        // Do not process children because we move the full sub-tree
        return FUNCTOR_SIBLINGS;
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToCastOffMensuralFunctor::VisitScoreDef(ScoreDef *scoreDef)
{
    assert(m_targetSystem);
    scoreDef->MoveItselfTo(m_targetSystem);

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToCastOffMensuralFunctor::VisitStaff(Staff *staff)
{
    m_targetStaff = new Staff(*staff);
    m_targetStaff->ClearChildren();
    m_targetStaff->CloneReset();
    // Keep the xml:id of the staff in the first staff segment
    m_targetStaff->SwapID(staff);
    assert(m_targetMeasure);
    m_targetMeasure->AddChild(m_targetStaff);

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToCastOffMensuralFunctor::VisitSyllable(Syllable *syllable)
{
    assert(m_targetSubSystem);
    assert(m_targetLayer);
    assert(syllable->GetParent());

    // If this is the first syllable of the layer, we do not create a new segment
    if (m_contentLayer->GetFirst() == syllable) {
        syllable->MoveItselfTo(m_targetLayer);
        return FUNCTOR_SIBLINGS;
    }
    // If the syllable is in editorial markup, do not create a new segment
    if (!syllable->GetParent()->Is(LAYER)) return FUNCTOR_SIBLINGS;

    // Make a segment break
    // First case: add a new measure segment (e.g., first pass)
    if (m_targetSubSystem->GetChildCount() <= m_segmentIdx) {
        m_targetMeasure = new Measure(false);
        m_targetSubSystem->AddChild(m_targetMeasure);
        // Add a staff with same attributes as in the previous segment
        m_targetStaff = new Staff(*m_targetStaff);
        m_targetStaff->ClearChildren();
        m_targetStaff->CloneReset();
        m_targetMeasure->AddChild(m_targetStaff);
        // Add a layer also with the same attributes as in the previous segment
        m_targetLayer = new Layer(*m_targetLayer);
        m_targetLayer->ClearChildren();
        m_targetLayer->CloneReset();
        m_targetStaff->AddChild(m_targetLayer);
    }
    // Second case: retrieve the appropriate segment
    else {
        m_targetMeasure = dynamic_cast<Measure *>(m_targetSubSystem->GetChild(m_segmentIdx));
        // It must be there
        assert(m_targetMeasure);

        // Look if we already have the staff (e.g., with more than one layer)
        AttNIntegerComparison comparisonStaffN(STAFF, m_targetStaff->GetN());
        Staff *staff = vrv_cast<Staff *>(m_targetMeasure->FindDescendantByComparison(&comparisonStaffN));
        if (!staff) {
            staff = new Staff(*m_targetStaff);
            staff->ClearChildren();
            staff->CloneReset();
            m_targetMeasure->AddChild(staff);
        }
        m_targetStaff = staff;

        // Add a new layer as the new target
        m_targetLayer = new Layer(*m_targetLayer);
        m_targetLayer->ClearChildren();
        m_targetLayer->CloneReset();
        m_targetStaff->AddChild(m_targetLayer);
    }
    syllable->MoveItselfTo(m_targetLayer);
    ++m_segmentIdx;

    return FUNCTOR_SIBLINGS;
}

FunctorCode ConvertToCastOffMensuralFunctor::VisitSystemElement(SystemElement *systemElement)
{
    assert(m_targetSystem);
    systemElement->MoveItselfTo(m_targetSystem);

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// ConvertToUnCastOffMensuralFunctor
//----------------------------------------------------------------------------

ConvertToUnCastOffMensuralFunctor::ConvertToUnCastOffMensuralFunctor() : Functor()
{
    this->ResetContent();
    m_trackSegmentsToDelete = true;
}

void ConvertToUnCastOffMensuralFunctor::ResetContent()
{
    m_contentMeasure = NULL;
    m_contentLayer = NULL;
}

FunctorCode ConvertToUnCastOffMensuralFunctor::VisitLayer(Layer *layer)
{
    if (!m_contentLayer) {
        m_contentLayer = layer;
    }
    else {
        m_contentLayer->MoveChildrenFrom(layer);
    }

    return FUNCTOR_SIBLINGS;
}

FunctorCode ConvertToUnCastOffMensuralFunctor::VisitMeasure(Measure *measure)
{
    if (!m_contentMeasure) {
        m_contentMeasure = measure;
    }
    else if (m_trackSegmentsToDelete) {
        m_segmentsToDelete.push_back(measure);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToUnCastOffMensuralFunctor::VisitSection(Section *section)
{
    this->ResetContent();

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// ConvertMarkupAnalyticalFunctor
//----------------------------------------------------------------------------

ConvertMarkupAnalyticalFunctor::ConvertMarkupAnalyticalFunctor(bool permanent) : Functor()
{
    m_currentChord = NULL;
    m_permanent = permanent;
}

FunctorCode ConvertMarkupAnalyticalFunctor::VisitChord(Chord *chord)
{
    assert(!m_currentChord);
    m_currentChord = chord;

    /****** fermata ******/

    if (chord->HasFermata()) {
        Fermata *fermata = new Fermata();
        this->ConvertToFermata(fermata, chord, chord->GetID());
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertMarkupAnalyticalFunctor::VisitChordEnd(Chord *chord)
{
    if (m_permanent) {
        chord->ResetTiePresent();
    }

    assert(m_currentChord);
    m_currentChord = NULL;

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertMarkupAnalyticalFunctor::VisitMeasureEnd(Measure *measure)
{
    for (Object *object : m_controlEvents) {
        measure->AddChild(object);
    }

    m_controlEvents.clear();

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertMarkupAnalyticalFunctor::VisitMRest(MRest *mRest)
{
    if (mRest->HasFermata()) {
        Fermata *fermata = new Fermata();
        this->ConvertToFermata(fermata, mRest, mRest->GetID());
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertMarkupAnalyticalFunctor::VisitNote(Note *note)
{
    /****** ties ******/

    AttTiePresent *check = note;
    // Use the parent chord if there is no @tie on the note
    if (!note->HasTie() && m_currentChord) {
        check = m_currentChord;
    }
    assert(check);

    Object *currentMeasure = note->GetFirstAncestor(MEASURE);
    assert(currentMeasure);

    std::vector<Note *>::iterator iter = m_currentNotes.begin();
    while (iter != m_currentNotes.end()) {
        // same octave and same pitch - this is the one!
        if ((note->GetOct() == (*iter)->GetOct()) && (note->GetPname() == (*iter)->GetPname())) {
            // right flag
            if ((check->GetTie() == TIE_m) || (check->GetTie() == TIE_t)) {
                Tie *tie = new Tie();
                if (!m_permanent) {
                    tie->IsAttribute(true);
                }
                tie->SetStartid("#" + (*iter)->GetID());
                tie->SetEndid("#" + note->GetID());
                // Add it to the starting measure when we are already in the next one
                Object *startMeasure = (*iter)->GetFirstAncestor(MEASURE);
                if (startMeasure && (startMeasure != currentMeasure)) {
                    startMeasure->AddChild(tie);
                }
                else {
                    m_controlEvents.push_back(tie);
                }
            }
            else {
                LogWarning("Expected @tie median or terminal in note '%s', skipping it", note->GetID().c_str());
            }
            iter = m_currentNotes.erase(iter);
            // we are done for this note
            break;
        }
        ++iter;
    }

    if ((check->GetTie() == TIE_m) || (check->GetTie() == TIE_i)) {
        m_currentNotes.push_back(note);
    }

    if (m_permanent) {
        note->ResetTiePresent();
    }

    /****** fermata ******/

    if (note->HasFermata()) {
        Fermata *fermata = new Fermata();
        this->ConvertToFermata(fermata, note, note->GetID());
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertMarkupAnalyticalFunctor::VisitRest(Rest *rest)
{
    if (rest->HasFermata()) {
        Fermata *fermata = new Fermata();
        this->ConvertToFermata(fermata, rest, rest->GetID());
    }

    return FUNCTOR_CONTINUE;
}

void ConvertMarkupAnalyticalFunctor::ConvertToFermata(
    Fermata *fermata, AttFermataPresent *fermataPresent, const std::string &id)
{
    fermata->SetPlace(Att::StaffrelBasicToStaffrel(fermataPresent->GetFermata()));
    if (m_permanent) {
        fermataPresent->ResetFermataPresent();
    }
    else {
        fermata->IsAttribute(true);
    }
    fermata->SetStartid("#" + id);
    m_controlEvents.push_back(fermata);
}

//----------------------------------------------------------------------------
// ConvertMarkupArticFunctor
//----------------------------------------------------------------------------

ConvertMarkupArticFunctor::ConvertMarkupArticFunctor() : Functor() {}

FunctorCode ConvertMarkupArticFunctor::VisitArtic(Artic *artic)
{
    if (artic->GetArtic().size() > 1) m_articsToConvert.emplace_back(artic);

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertMarkupArticFunctor::VisitLayerEnd(Layer *layer)
{
    for (Artic *artic : m_articsToConvert) {
        this->SplitMultival(artic);
    }
    m_articsToConvert.clear();

    return FUNCTOR_CONTINUE;
}

void ConvertMarkupArticFunctor::SplitMultival(Artic *artic) const
{
    Object *parent = artic->GetParent();
    assert(parent);

    std::vector<data_ARTICULATION> articList = artic->GetArtic();
    if (articList.empty()) return;

    int idx = artic->GetIdx() + 1;
    std::vector<data_ARTICULATION>::iterator iter;
    for (iter = articList.begin() + 1; iter != articList.end(); ++iter) {
        Artic *articChild = new Artic();
        articChild->SetArtic({ *iter });
        articChild->AttColor::operator=(*artic);
        articChild->AttEnclosingChars::operator=(*artic);
        articChild->AttExtSymAuth::operator=(*artic);
        articChild->AttExtSymNames::operator=(*artic);
        articChild->AttPlacementRelEvent::operator=(*artic);
        parent->InsertChild(articChild, idx);
        ++idx;
    }

    // Only keep the first value in the original element
    artic->SetArtic({ articList.at(0) });

    // Multiple valued attributes cannot be preserved as such
    if (artic->IsAttribute()) {
        artic->IsAttribute(false);
        LogInfo("Multiple valued attribute @artic on '%s' permanently converted to <artic> elements",
            parent->GetID().c_str());
    }
}

//----------------------------------------------------------------------------
// ConvertMarkupScoreDefFunctor
//----------------------------------------------------------------------------

ConvertMarkupScoreDefFunctor::ConvertMarkupScoreDefFunctor(Doc *doc) : DocFunctor(doc)
{
    m_currentScoreDef = NULL;
}

FunctorCode ConvertMarkupScoreDefFunctor::VisitScore(Score *score)
{
    // Evaluate functor on scoreDef
    score->GetScoreDef()->Process(*this);

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertMarkupScoreDefFunctor::VisitScoreDefElement(ScoreDefElement *scoreDefElement)
{
    if (scoreDefElement->Is(SCOREDEF)) {
        m_currentScoreDef = scoreDefElement;
        return FUNCTOR_CONTINUE;
    }

    // This should never be the case
    if (!scoreDefElement->Is(STAFFDEF) || !m_currentScoreDef) return FUNCTOR_CONTINUE;

    // Copy score definition elements to the staffDef but only if they are not given at the staffDef
    // This might require more refined merging because we can lose data if some staffDef values are defined
    // but do not contain all the ones given in the scoreDef (e.g. @key.mode in scoreDef but not in a staffDef with
    // @key.sig)
    if (m_currentScoreDef->HasClefInfo() && !scoreDefElement->HasClefInfo()) {
        scoreDefElement->AddChild(m_currentScoreDef->GetClefCopy());
    }
    if (m_currentScoreDef->HasKeySigInfo() && !scoreDefElement->HasKeySigInfo()) {
        scoreDefElement->AddChild(m_currentScoreDef->GetKeySigCopy());
    }
    if (m_currentScoreDef->HasMeterSigGrpInfo() && !scoreDefElement->HasMeterSigGrpInfo()) {
        scoreDefElement->AddChild(m_currentScoreDef->GetMeterSigGrpCopy());
    }
    if (m_currentScoreDef->HasMeterSigInfo() && !scoreDefElement->HasMeterSigInfo()) {
        scoreDefElement->AddChild(m_currentScoreDef->GetMeterSigCopy());
    }
    if (m_currentScoreDef->HasMensurInfo() && !scoreDefElement->HasMensurInfo()) {
        scoreDefElement->AddChild(m_currentScoreDef->GetMensurCopy());
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertMarkupScoreDefFunctor::VisitScoreDefElementEnd(ScoreDefElement *scoreDefElement)
{
    if (!scoreDefElement->Is(SCOREDEF)) return FUNCTOR_CONTINUE;

    // At the end of the scoreDef, remove all score definition elements
    if (scoreDefElement->HasClefInfo()) {
        Object *clef = scoreDefElement->FindDescendantByType(CLEF, 1);
        if (clef) scoreDefElement->DeleteChild(clef);
    }
    if (scoreDefElement->HasKeySigInfo()) {
        Object *keySig = scoreDefElement->FindDescendantByType(KEYSIG, 1);
        if (keySig) scoreDefElement->DeleteChild(keySig);
    }
    if (scoreDefElement->HasMeterSigGrpInfo()) {
        Object *meterSigGrp = scoreDefElement->FindDescendantByType(METERSIGGRP, 1);
        if (meterSigGrp) scoreDefElement->DeleteChild(meterSigGrp);
    }
    if (scoreDefElement->HasMeterSigInfo()) {
        Object *meterSig = scoreDefElement->FindDescendantByType(METERSIG, 1);
        if (meterSig) scoreDefElement->DeleteChild(meterSig);
    }
    if (scoreDefElement->HasMensurInfo()) {
        Object *mensur = scoreDefElement->FindDescendantByType(MENSUR, 1);
        if (mensur) scoreDefElement->DeleteChild(mensur);
    }

    m_currentScoreDef = NULL;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

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

ConvertToCastOffMensuralFunctor::ConvertToCastOffMensuralFunctor(Doc *doc, System *targetSystem) : DocFunctor(doc)
{
    m_contentStaff = NULL;
    m_contentLayer = NULL;
    m_targetSystem = targetSystem;
    m_targetStaff = NULL;
    m_targetLayer = NULL;
}

FunctorCode ConvertToCastOffMensuralFunctor::VisitLayer(Layer *layer)
{
    m_contentLayer = layer;
    m_targetLayer = NULL;

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToCastOffMensuralFunctor::VisitMeasure(Measure *measure)
{
    m_measures.clear();
    m_breakPoints.clear();

    const int nbLayers = measure->GetDescendantCount(LAYER);
    bool isFirst = true;

    // Create at least one measure to copy stuff to
    Measure *segment = new Measure(UNMEASURED);
    m_targetSystem->AddChild(segment);
    m_measures.push_back(segment);

    for (const Object *child : measure->m_measureAligner.GetChildren()) {
        const Alignment *alignment = vrv_cast<const Alignment *>(child);
        assert(alignment);
        // We use the alignments with an element at all layer as a breakpoint
        if (!this->IsValidBreakPoint(alignment, nbLayers)) continue;
        // Do not break at the first one
        if (isFirst) {
            isFirst = false;
            continue;
        }
        Measure *segment = new Measure(UNMEASURED);
        m_targetSystem->AddChild(segment);
        m_measures.push_back(segment);
        m_breakPoints.push_back(alignment);
    }

    // Now we are ready to process staves/layers and to move content to m_measures
    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToCastOffMensuralFunctor::VisitObject(Object *object)
{
    assert(object->GetParent());
    // We want to move only the children of the layer of any type (notes, editorial elements, etc)
    if (object->GetParent()->Is(LAYER)) {
        this->InitSegment(object);
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
    m_currentMeasure = m_measures.begin();
    m_currentBreakPoint = m_breakPoints.begin();

    m_contentStaff = staff;
    m_targetStaff = NULL;

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToCastOffMensuralFunctor::VisitSystemElement(SystemElement *systemElement)
{
    assert(m_targetSystem);
    systemElement->MoveItselfTo(m_targetSystem);

    return FUNCTOR_CONTINUE;
}

bool ConvertToCastOffMensuralFunctor::IsValidBreakPoint(const Alignment *alignment, const int nbLayers)
{
    if (alignment->GetType() != ALIGNMENT_DEFAULT) return false;

    // Not all layers have an alignment and we cannot break here
    if (alignment->GetChildCount() != nbLayers) return false;

    for (const Object *child : alignment->GetChildren()) {
        for (const Object *refChild : child->GetChildren()) {
            // Do not break within editorial markup
            if (refChild->GetFirstAncestorInRange(EDITORIAL_ELEMENT, EDITORIAL_ELEMENT_max)) return false;
            // Do not break within a ligature
            if (refChild->GetFirstAncestor(LIGATURE)) return false;
        }
        // When we have more than one neume in a syllable, every neume has its own alignment.
        // Only the first one, which is shared with the syllable, is a valid break point
        if (child->FindDescendantByType(NEUME) && !child->FindDescendantByType(SYLLABLE)) {
            return false;
        }
    }

    return true;
}

void ConvertToCastOffMensuralFunctor::InitSegment(Object *object)
{
    assert(m_contentStaff);
    assert(m_contentLayer);

    LayerElement *element = NULL;
    if (object->IsLayerElement()) element = vrv_cast<LayerElement *>(object);

    if (element && (element->GetAlignment() == *m_currentBreakPoint)) {
        m_targetStaff = NULL;
        m_targetLayer = NULL;
        std::advance(m_currentBreakPoint, 1);
        std::advance(m_currentMeasure, 1);
    }

    if (m_targetStaff && m_targetLayer) return;

    m_targetStaff = new Staff();
    m_contentStaff->CopyAttributesTo(m_targetStaff);
    // Keep the xml:id of the staff in the first staff segment
    m_targetStaff->SwapID(m_contentStaff);
    assert(*m_currentMeasure);
    (*m_currentMeasure)->AddChild(m_targetStaff);

    m_targetLayer = new Layer();
    m_contentLayer->CopyAttributesTo(m_targetLayer);
    // Keep the xml:id of the layer in the first segment
    m_targetLayer->SwapID(m_contentLayer);
    assert(m_targetStaff);
    m_targetStaff->AddChild(m_targetLayer);
}

//----------------------------------------------------------------------------
// ConvertToUnCastOffMensuralFunctor
//----------------------------------------------------------------------------

ConvertToUnCastOffMensuralFunctor::ConvertToUnCastOffMensuralFunctor() : Functor()
{
    this->ResetContent();
    // We process layer by layer, keep a list of segments to be deleted the first time we go through
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
    // First measure of the section, move all content to it and keep it
    if (!m_contentMeasure) {
        m_contentMeasure = measure;
    }
    // First pass, mark the measure to be deleted once finished
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

//----------------------------------------------------------------------------
// ConvertToMensuralViewFunctor
//----------------------------------------------------------------------------

ConvertToMensuralViewFunctor::ConvertToMensuralViewFunctor(Doc *doc) : DocFunctor(doc)
{
    m_viewLayer = NULL;
}

FunctorCode ConvertToMensuralViewFunctor::VisitEditorialElement(EditorialElement *editorialElement)
{
    // We are not in a layer yet
    if (!m_viewLayer) return FUNCTOR_CONTINUE;

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToMensuralViewFunctor::VisitLayer(Layer *layer)
{
    assert(!m_viewLayer);
    assert(m_stack.empty());

    m_viewLayer = new Layer();
    layer->CopyAttributesTo(m_viewLayer);
    m_stack.push_back(m_viewLayer);

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToMensuralViewFunctor::VisitLayerEnd(Layer *layer)
{
    layer->GetParent()->ReplaceChild(layer, m_viewLayer);
    // This will delete all editorial markup and ligatures in the old layer
    delete layer;

    // We should have only the m_layerView left
    assert(m_stack.size() == 1);
    m_viewLayer = NULL;
    m_stack.clear();

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToMensuralViewFunctor::VisitLayerElement(LayerElement *layerElement)
{
    // The stack top it either the new layer or the parent element within editorial markup
    if (m_stack.back() != layerElement->GetParent()) {
        layerElement->MoveItselfTo(m_stack.back());
    }

    m_stack.push_back(layerElement);

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToMensuralViewFunctor::VisitLayerElementEnd(LayerElement *layerElement)
{
    m_stack.pop_back();

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToMensuralViewFunctor::VisitLigature(Ligature *ligature)
{
    // For now simply skip them - eventually we might want to add brackets
    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToMensuralViewFunctor::VisitLigatureEnd(Ligature *ligature)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv

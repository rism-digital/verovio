/////////////////////////////////////////////////////////////////////////////
// Name:        convertfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "convertfunctor.h"

//----------------------------------------------------------------------------

#include "bracketspan.h"
#include "dir.h"
#include "div.h"
#include "doc.h"
#include "ending.h"
#include "fermata.h"
#include "layer.h"
#include "ligature.h"
#include "mdiv.h"
#include "mrest.h"
#include "page.h"
#include "rest.h"
#include "score.h"
#include "section.h"
#include "staff.h"
#include "syllable.h"
#include "system.h"
#include "text.h"
#include "tie.h"
#include "tuplet.h"
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
    if (!editorialElement->IsHidden()) {
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
    if (!mdiv->IsHidden()) {
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
    m_segments.clear();
    m_breakPoints.clear();

    const int nbLayers = measure->GetDescendantCount(LAYER);
    bool isFirst = true;

    // Create at least one segment to copy stuff to
    Measure *segment = new Measure(UNMEASURED);
    m_targetSystem->AddChild(segment);
    m_segments.push_back(segment);

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
        m_segments.push_back(segment);
        m_breakPoints.push_back(alignment);
    }

    // Now we are ready to process staves/layers and to move content to m_segments
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
    m_currentSegment = m_segments.begin();
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

    const bool ligatureAsBracket = m_doc->GetOptions()->m_ligatureAsBracket.GetValue();

    for (const Object *child : alignment->GetChildren()) {
        for (const Object *refChild : child->GetChildren()) {
            // Do not break within editorial markup
            if (refChild->GetFirstAncestorInRange(EDITORIAL_ELEMENT, EDITORIAL_ELEMENT_max)) return false;
            // Do not break within a ligature when rendered as bracket - (notes in it will have a different aligner
            // execpt for the first one)
            if (ligatureAsBracket && refChild->GetFirstAncestor(LIGATURE)) {
                const Ligature *ligature = vrv_cast<const Ligature *>(refChild->GetFirstAncestor(LIGATURE));
                assert(ligature);
                if (ligature->GetAlignment() != alignment) return false;
            }
        }
        // When we have more than one neume in a syllable, every neume has its own alignment.
        // Only the first one, which is shared with the syllable, is a valid break point
        if (child->FindDescendantByType(NC) && !child->FindDescendantByType(SYLLABLE)) {
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

    if (element && element->GetAlignment() && (element->GetAlignment() == *m_currentBreakPoint)) {
        m_targetStaff = NULL;
        m_targetLayer = NULL;
        std::advance(m_currentBreakPoint, 1);
        std::advance(m_currentSegment, 1);
    }

    if (m_targetStaff && m_targetLayer) return;

    m_targetStaff = new Staff();
    m_contentStaff->CopyAttributesTo(m_targetStaff);
    // Keep the xml:id of the staff in the first staff segment
    m_targetStaff->SwapID(m_contentStaff);
    assert(*m_currentSegment);
    (*m_currentSegment)->AddChild(m_targetStaff);

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
// ConvertToCmnFunctor
//----------------------------------------------------------------------------

ConvertToCmnFunctor::ConvertToCmnFunctor(Doc *doc, System *targetSystem, Score *score) : DocFunctor(doc)
{
    m_targetSystem = targetSystem;
    m_score = score;
    m_currentStaff = NULL;
    m_n = 0;
}

FunctorCode ConvertToCmnFunctor::VisitChord(Chord *chord)
{
    // Mensural chords are aligned looking at the longest duration in the notes
    Note *longestNote = NULL;
    ListOfObjects notes = chord->FindAllDescendantsByType(NOTE);
    for (Object *object : notes) {
        Note *note = vrv_cast<Note *>(object);
        if (!longestNote || (note->GetScoreTimeDuration() > longestNote->GetScoreTimeDuration())) {
            longestNote = note;
        }
    }

    // Use the longest note duration but create Chords
    // That way the (longest) duration it move up from the note elements to the chord
    // As it stands we ignore notes with shorter duration in the chords
    // We also ignore coloration
    this->ConvertDurationInterface(longestNote, CHORD);

    // This should not happen, just check it once for all
    if (m_durationElements.empty()) return FUNCTOR_SIBLINGS;

    // Copy the `@pname` and `@oct` from the mensural note
    for (Object *noteObject : notes) {
        Object *tieStart = NULL;
        const Accid *accid = vrv_cast<const Accid *>(noteObject->FindDescendantByType(ACCID));
        bool isFirstNote = true;
        for (Object *object : m_durationElements) {
            Note *note = vrv_cast<Note *>(noteObject);
            Note *cmnNote = new Note();
            cmnNote->SetPname(note->GetPname());
            cmnNote->SetOct(note->GetOct());
            this->ConvertAccid(cmnNote, accid, isFirstNote);
            object->AddChild(cmnNote);

            // Also create the ties for notes in the chord
            if (tieStart) {
                Object *measure = tieStart->GetFirstAncestor(MEASURE);
                assert(measure);
                Tie *tie = new Tie();
                tie->SetStartid("#" + tieStart->GetID());
                tie->SetEndid("#" + cmnNote->GetID());
                measure->AddChild(tie);
            }
            tieStart = cmnNote;
        }
    }

    // Move the verse
    Object *verse = chord->FindDescendantByType(VERSE);
    if (verse) {
        verse->MoveItselfTo(m_durationElements.front());
    }

    return FUNCTOR_SIBLINGS;
}

FunctorCode ConvertToCmnFunctor::VisitLayer(Layer *layer)
{
    m_currentParams.mensur = layer->GetCurrentMensur();
    m_currentParams.meterSig = layer->GetCurrentMeterSig();
    m_currentParams.proport = layer->GetCurrentProport();

    m_ligature = NULL;
    m_coloration = NULL;
    m_proportTuplet = NULL;

    m_startid = "";

    m_currentLayer = m_layers.begin();
    m_currentMeasure = m_measures.begin();

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToCmnFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (layerElement->IsSystemElement()) return FUNCTOR_CONTINUE;

    if (layerElement->Is(CLEF)) {
        if (!m_layerClef) m_layerClef = vrv_cast<Clef *>(layerElement);
    }
    else if (layerElement->Is(MENSUR)) {
        // replace the current mensur
        m_currentParams.mensur = vrv_cast<Mensur *>(layerElement);
        assert(m_currentParams.mensur);
        this->ConvertMensur(m_currentParams.mensur);
    }
    else if (layerElement->Is(PROPORT)) {
        if (layerElement->GetType() == "cmme_tempo_change") return FUNCTOR_SIBLINGS;
        // replace the current proport
        const Proport *previous = (m_currentParams.proport) ? (m_currentParams.proport) : NULL;
        m_currentParams.proport = vrv_cast<Proport *>(layerElement);
        assert(m_currentParams.proport);
        if (previous) {
            m_currentParams.proport->Cumulate(previous);
        }
        // Reset the tuplet since we expect the num / numbase to be different
        m_proportTuplet = NULL;
    }
    else if (layerElement->Is({ ACCID, BARLINE, DOT })) {
        // can be ignored
    }
    else {
        LogDebug(layerElement->GetClassName().c_str());
    }

    return FUNCTOR_SIBLINGS;
}

FunctorCode ConvertToCmnFunctor::VisitLigature(Ligature *ligature)
{
    assert(!m_ligature);

    m_ligature = new BracketSpan();
    m_ligature->SetFunc(bracketSpanLog_FUNC_ligature);
    m_ligature->SetLform(LINEFORM_solid);
    (*m_currentMeasure).m_measure->AddChild(m_ligature);

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToCmnFunctor::VisitLigatureEnd(Ligature *ligature)
{
    m_ligature = NULL;

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToCmnFunctor::VisitMeasure(Measure *measure)
{
    m_measures.clear();

    // measure->m_measureAligner.LogDebugTree();

    const int nbLayers = measure->GetDescendantCount(LAYER);

    // First build an array of all mensuration signs that correspond to a time signature change
    // Mensur sign should appear at all voices to be considered global
    std::vector<MensurInfo> mensurs;
    MensurInfo mensur; // Will be copied when added to the array, so we can re-use it.

    for (const Object *child : measure->m_measureAligner.GetChildren()) {
        const Alignment *alignment = vrv_cast<const Alignment *>(child);
        assert(alignment);
        // We use the alignments with an element at all layer as a breakpoint
        if (!this->IsGlobalMensur(alignment, nbLayers, mensur.m_mensur)) continue;
        //
        mensur.m_time = alignment->GetTime();
        mensurs.push_back(mensur);
    }
    // We need to have at least one and at least one at beginning (time 0)
    if (mensurs.empty()) {
        mensur.m_time = 0;
        mensurs.push_back(mensur);
        LogWarning("No mensur detected");
    }
    else if (mensurs.front().m_time != 0) {
        mensur.m_time = 0;
        mensur.m_mensur.Reset();
        mensurs.insert(mensurs.begin(), mensur);
        LogWarning("No mensur detected at the beginning");
    }

    // Now we can create measure object based on the mensur and the time of each MensurInfo section
    // The total time of the piece
    Fraction totalTime = measure->m_measureAligner.GetMaxTime();
    Fraction time(0);
    // We know that we have at least one MensurInfo
    std::vector<MensurInfo>::iterator mensurIter = mensurs.begin();
    Fraction measureDuration = this->CalcMeasureDuration(mensurs.front().m_mensur);
    Mensur currentMensur = mensurs.front().m_mensur;
    std::advance(mensurIter, 1);
    // The next is either the next MensurInfo or the end of the piece
    Fraction next = (mensurIter == mensurs.end()) ? totalTime : (*mensurIter).m_time;

    while (time < next) {
        MeasureInfo measureInfo(time, measureDuration);
        Measure *cmnMeasure = new Measure();
        cmnMeasure->SetN(std::to_string(++m_n));
        measureInfo.m_measure = cmnMeasure;
        if ((time + measureInfo.m_duration) > next) {
            measureInfo.m_duration = next - time;
            cmnMeasure->SetMetcon(BOOLEAN_false);
        }
        m_targetSystem->AddChild(cmnMeasure);
        m_measures.push_back(measureInfo);

        time = time + measureDuration;
        if ((time >= next) && (mensurIter != mensurs.end())) {
            time = next;
            currentMensur = (*mensurIter).m_mensur;
            Fraction duration = this->CalcMeasureDuration(currentMensur);
            // The new measure does not have the same duration - add a scoreDef with a meterSig change
            if (duration != measureDuration) {
                ScoreDef *scoreDef = new ScoreDef();
                MeterSig *meterSig = new MeterSig();
                meterSig->IsAttribute(true);
                meterSig->SetUnit(2);
                Fraction count = duration / Fraction(DURATION_2);
                meterSig->SetCount({ { count.GetNumerator() }, MeterCountSign::None });
                scoreDef->AddChild(meterSig);
                m_targetSystem->AddChild(scoreDef);
            }
            measureDuration = duration;
            std::advance(mensurIter, 1);
            next = (mensurIter == mensurs.end()) ? totalTime : (*mensurIter).m_time;
        }
    }
    // This will happen with empty CMME files
    if (m_measures.empty()) {
        LogWarning("The content of the measure seems empty, creating one for the process not to fail.");
        MeasureInfo measureInfo(time, measureDuration);
        Measure *cmnMeasure = new Measure();
        measureInfo.m_measure = cmnMeasure;
        // Arbitrary duration
        measureInfo.m_duration = 2;
        cmnMeasure->SetMetcon(BOOLEAN_false);
        m_targetSystem->AddChild(cmnMeasure);
        m_measures.push_back(measureInfo);
    }

    m_measures.back().m_measure->SetRight(BARRENDITION_dbl);

    // Now we are ready to process layers and to move content to m_measures
    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToCmnFunctor::VisitMeasureEnd(Measure *measure)
{
    // This is the first measure in the system - we need to update the scoreDef
    if (m_score && m_score->GetScoreDef()) {
        for (Object *child : m_score->GetScoreDef()->GetList()) {
            StaffDef *staffDef = vrv_cast<StaffDef *>(child);
            assert(staffDef);
            // Remove the notation type (CMN is normally the default)
            staffDef->SetNotationtype(NOTATIONTYPE_NONE);
            // Remove the mensur
            Object *mensur = staffDef->GetFirst(MENSUR);
            if (mensur) staffDef->DeleteChild(mensur);
            // Add the meterSig
            MeterSig *meterSig = new MeterSig();
            meterSig->SetUnit(2);
            Fraction count = m_measures.front().m_duration / Fraction(DURATION_2);
            meterSig->SetCount({ { count.GetNumerator() }, MeterCountSign::None });
            staffDef->AddChild(meterSig);
            // Add or replace the clef
            Object *clef = staffDef->GetFirst(CLEF);
            // We must have m_clefs filled with clefs converted when processing layers
            // However, if we have a clef in the staffDef, convert and use that one
            if (clef) {
                this->ConvertClef(m_clefs.back(), vrv_cast<Clef *>(clef));
                staffDef->DeleteChild(clef);
            }
            staffDef->AddChild(m_clefs.back());
            // Pop it from the list
            m_clefs.pop_back();
        }
        // Adjust the scoreDef only for the first measure, set it to NULL;
        m_score = NULL;
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToCmnFunctor::VisitNote(Note *note)
{
    this->ConvertDurationInterface(note, NOTE);

    // This should not happen, just check it once for all
    if (m_durationElements.empty()) return FUNCTOR_SIBLINGS;

    const Accid *accid = vrv_cast<const Accid *>(note->FindDescendantByType(ACCID));
    bool isFirstNote = true;

    // Copy the `@pname` and `@oct` from the mensural note
    for (Object *object : m_durationElements) {
        Note *cmnNote = vrv_cast<Note *>(object);
        assert(cmnNote);
        cmnNote->SetPname(note->GetPname());
        cmnNote->SetOct(note->GetOct());
        if (note->HasColor()) cmnNote->SetColor(note->GetColor());
        this->ConvertAccid(cmnNote, accid, isFirstNote);
    }

    // Check if we are in a ligature and set `@startid` and `@endid` as needed
    if (m_ligature) {
        // The ligature has just been created, add the first note as `@startid`
        if (!m_ligature->HasStartid()) m_ligature->SetStartid("#" + m_durationElements.front()->GetID());
        // Set the last as `@endid` - will be updated until the ligature is closed
        m_ligature->SetEndid("#" + m_durationElements.back()->GetID());
    }

    // Check if the note has coloration - create or add it to the coloration bracket as appropriate
    if (note->HasColored()) {
        if (!m_coloration) {
            m_coloration = new BracketSpan();
            m_coloration->SetFunc(bracketSpanLog_FUNC_coloration);
            Object *measure = m_durationElements.front()->GetFirstAncestor(MEASURE);
            assert(measure);
            measure->AddChild(m_coloration);
            m_coloration->SetStartid("#" + m_durationElements.front()->GetID());
        }
        // Set the last as `@endid` - will be updated if next note is also colored
        m_coloration->SetEndid("#" + m_durationElements.back()->GetID());
    }
    // End the coloration bracket if any
    else if (m_coloration) {
        m_coloration = NULL;
    }

    // Move the verse
    Object *verse = note->FindDescendantByType(VERSE);
    if (verse) {
        verse->MoveItselfTo(m_durationElements.front());
    }

    // Add the tie
    Object *tieStart = m_durationElements.front();
    for (Object *tieEnd : m_durationElements) {
        if (tieStart == tieEnd) continue;
        Object *measure = tieStart->GetFirstAncestor(MEASURE);
        assert(measure);
        Tie *tie = new Tie();
        tie->SetStartid("#" + tieStart->GetID());
        tie->SetEndid("#" + tieEnd->GetID());
        measure->AddChild(tie);
        if (note->HasColor()) tie->SetColor(note->GetColor());
        tieStart = tieEnd;
    }

    return FUNCTOR_SIBLINGS;
}

FunctorCode ConvertToCmnFunctor::VisitRest(Rest *rest)
{
    this->ConvertDurationInterface(rest, REST);

    // Copy the `@color` from the mensural rest
    for (Object *object : m_durationElements) {
        Rest *cmnRest = vrv_cast<Rest *>(object);
        assert(cmnRest);
        if (rest->HasColor()) cmnRest->SetColor(rest->GetColor());
    }

    return FUNCTOR_SIBLINGS;
}

FunctorCode ConvertToCmnFunctor::VisitScoreDef(ScoreDef *scoreDef)
{
    assert(m_targetSystem);
    scoreDef->MoveItselfTo(m_targetSystem);

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToCmnFunctor::VisitStaff(Staff *staff)
{
    m_currentStaff = staff;
    m_layerClef = NULL;
    m_currentMeasure = m_measures.begin();

    m_layers.clear();
    m_layers.reserve(m_measures.size());
    for (int i = 0; i < (int)m_measures.size(); ++i) {
        Layer *layer = new Layer();
        m_layers.push_back(layer);
    }
    m_currentLayer = m_layers.begin();

    for (int i = 0; i < (int)m_measures.size(); ++i) {
        Staff *cmnStaff = new Staff();
        cmnStaff->SetN(staff->GetN());
        cmnStaff->AddChild(m_layers.at(i));
        m_measures.at(i).m_measure->AddChildBack(cmnStaff);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToCmnFunctor::VisitStaffEnd(Staff *staff)
{
    // Add mRest to empty layers
    for (auto *layer : m_layers) {
        if (layer->GetChildCount() == 0) {
            layer->AddChild(new MRest());
        }
    }

    Clef *clef = new Clef();
    this->ConvertClef(clef, m_layerClef);
    // Add to the list of clef (one per staff)
    m_clefs.push_front(clef);

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToCmnFunctor::VisitSystemElement(SystemElement *systemElement)
{
    assert(m_targetSystem);
    systemElement->MoveItselfTo(m_targetSystem);

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToCmnFunctor::VisitSystemEnd(System *system)
{
    assert(m_targetSystem);
    Measure *last = vrv_cast<Measure *>(m_targetSystem->GetLast(MEASURE));
    assert(last);
    last->SetRight(BARRENDITION_end);

    return FUNCTOR_CONTINUE;
}

bool ConvertToCmnFunctor::IsGlobalMensur(const Alignment *alignment, const int nbLayers, Mensur &mensur)
{
    if (alignment->GetType() != ALIGNMENT_MENSUR) return false;

    // Not all layers have an alignment and we cannot break here
    if (alignment->GetChildCount() != nbLayers) return false;

    for (const Object *child : alignment->GetChildren()) {
        // Just pick the one of the first layer - quite arbitrary and we should check if they are all the same
        // and decide what to do if they are not
        const Mensur *mensurRef = vrv_cast<const Mensur *>(child->FindDescendantByType(MENSUR));
        if (mensurRef) {
            mensur = *mensurRef;
            return true;
        }
    }

    return true;
}

Fraction ConvertToCmnFunctor::CalcMeasureDuration(const Mensur &mensur)
{
    Fraction duration(DURATION_2);
    duration = duration * abs(mensur.GetProlatio());
    if (abs(mensur.GetProlatio()) == 2) duration = duration * abs(mensur.GetTempus());
    return duration;
}

void ConvertToCmnFunctor::ConvertDurationInterface(DurationInterface *interface, ClassId classId)
{
    assert(interface);

    m_durationElements.clear();

    data_DURATION noteDur = interface->GetActualDur();
    // Longa and maxima are converted into repeated breves since breve is the longest duration we can fit in a measure
    if (noteDur < DURATION_breve) noteDur = DURATION_breve;

    this->SplitDurationInterface(classId, noteDur, interface->GetScoreTimeOnset() / SCORE_TIME_UNIT,
        interface->GetScoreTimeDuration() / SCORE_TIME_UNIT);
}

void ConvertToCmnFunctor::SplitDurationInterface(
    ClassId classId, data_DURATION noteDur, Fraction time, Fraction duration)
{
    if (m_currentLayer == m_layers.end()) {
        LogDebug("Extraneous content not processed");
        return;
    }

    const Fraction measureEnd = (*m_currentMeasure).m_time + (*m_currentMeasure).m_duration;
    const Fraction noteEnd = time + duration;

    std::list<CmnDuration> cmnDurations;

    Fraction processed = duration;
    // If we go beyond the end of the measure, first process only what fits
    if (noteEnd > measureEnd) {
        processed = measureEnd - time;
    }

    // The alignement duration features in the proportion - we can revert it back and make tuplets
    Fraction durationWithoutProport = processed;
    // Invert apply num and numbase
    if (m_currentParams.proport->HasNum())
        durationWithoutProport = durationWithoutProport * m_currentParams.proport->GetCumulatedNum();
    if (m_currentParams.proport->HasNumbase())
        durationWithoutProport = durationWithoutProport / m_currentParams.proport->GetCumulatedNumbase();
    // We have a proportion applied, so the duration is different
    bool hasProportion = (durationWithoutProport != processed);
    // A flag indicating if the tuplet is a temporary one not corresponding to the proportion when notes are split
    // across measures
    bool nonProportTuplet = false;

    // Check if the value can be represented by value up to 16th (arbitrary)
    // This is not always the case when notes are split across measures
    // It is most likely to happen with proportions, but not only and it can also happen with coloration and notes
    // having `@num` and `@numbase` One possible improvement would be to regroup sequences of notes with the same `@num`
    // and `@numbase` under the same tuplet
    if ((durationWithoutProport % Fraction(DURATION_16)) != 0) {
        // If not, do not use the proportion tuplet an revert the duration back - however, that happens only when we
        // have a proportion
        m_proportTuplet = NULL;
        durationWithoutProport = processed;
        // If the remaining duration cannot be represented as well, last resort option with a specific tuplet
        if ((durationWithoutProport % Fraction(DURATION_16)) != 0) {
            m_proportTuplet = new Tuplet();
            // Use a note with half of the noteDur expected when in a proportion since the note is being split across
            // measures - create a tuplet with according ratio
            Fraction tupletRatio = (hasProportion) ? durationWithoutProport / Fraction(noteDur) * 2
                                                   : durationWithoutProport / Fraction(noteDur);
            LogWarning("The tuplet corresponding to proportion is not appropriate and must be changed to %s",
                tupletRatio.ToString().c_str());
            m_proportTuplet->SetNum(tupletRatio.GetDenominator());
            m_proportTuplet->SetNumbase(tupletRatio.GetNumerator());
            m_proportTuplet->SetNumFormat(tupletVis_NUMFORMAT_ratio);
            m_proportTuplet->SetBracketVisible(BOOLEAN_false);
            // Same here : use a note with half of the noteDur expected when in a proportion since the note is being
            // split across measures
            durationWithoutProport = (hasProportion) ? Fraction(noteDur) / 2 : Fraction(noteDur);
            (*m_currentLayer)->AddChild(m_proportTuplet);
            // Mark the tuplet as temporary
            nonProportTuplet = true;
        }
    }
    // Create a new tuplet but only if we have not already created one for the previous note
    else if (hasProportion && !m_proportTuplet) {
        m_proportTuplet = new Tuplet();
        m_proportTuplet->SetNum(m_currentParams.proport->GetCumulatedNum());
        m_proportTuplet->SetNumbase(m_currentParams.proport->GetCumulatedNumbase());
        m_proportTuplet->SetNumFormat(tupletVis_NUMFORMAT_ratio);
        m_proportTuplet->SetBracketVisible(BOOLEAN_false);
        (*m_currentLayer)->AddChild(m_proportTuplet);
    }

    // Split what we can fit within a measure into cmn duration (e.g., B => B. Sb. in tempus perfectum and prolatio
    // major) Fill a list of CMN duration with dots (and num / numbase, unused for now)
    this->SplitDurationIntoCmn(noteDur, durationWithoutProport, m_currentParams.mensur, cmnDurations);

    // Add them to the layer using the ObjectFactory (create notes or rests)
    for (const CmnDuration &cmnDuration : cmnDurations) {
        ObjectFactory *instance = ObjectFactory::GetInstance();
        Object *layerElement = instance->Create(classId);
        assert(layerElement);
        // Add it to the durationElement for post processing (e.g., setting note attributes or adding ties)
        m_durationElements.push_back(layerElement);
        DurationInterface *interface = layerElement->GetDurationInterface();
        assert(interface);
        if (m_proportTuplet) {
            // We already have note in the tuplet so we should show the bracket
            if (m_proportTuplet->GetChildCount() > 0) m_proportTuplet->SetBracketVisible(BOOLEAN_true);
            m_proportTuplet->AddChild(layerElement);
        }
        else {
            (*m_currentLayer)->AddChild(layerElement);
        }
        interface->SetDur(cmnDuration.m_duration);
        // Add a `@dots` only if not 0
        if (cmnDuration.m_dots != 0) interface->SetDots(cmnDuration.m_dots);
    }

    // The tuplet is specific to the partial duration, reset it (e.g., at the beginning of a measure with a note split
    // from the previous measure)
    if (nonProportTuplet) m_proportTuplet = NULL;

    // Increase the tstamp by the value processed
    m_startid = m_durationElements.front()->GetID();

    // If we have reach the end of the measure, go to the next one
    if (time + processed == measureEnd) {
        ++m_currentMeasure;
        ++m_currentLayer;
        // End of the measure, close the tuplet
        m_proportTuplet = NULL;
        // Reset the tstamp
        m_startid = "";
    }
    // Also check if we have more to process for that note or rest - if yes, call it recursively
    if (duration - processed != 0) {
        this->SplitDurationInterface(classId, noteDur, time = (*m_currentMeasure).m_time, duration - processed);
    }
}

void ConvertToCmnFunctor::SplitDurationIntoCmn(
    data_DURATION elementDur, Fraction duration, const Mensur *mensur, std::list<CmnDuration> &cmnDurations)
{
    const bool prolatioMajor = (abs(mensur->GetProlatio()) == 3);
    const bool tempusPerfectum = (abs(mensur->GetTempus()) == 3);
    const int semiBrevisDots = (prolatioMajor) ? 1 : 0;
    const int brevisDots = (tempusPerfectum) ? 1 : 0;

    const Fraction semiBrevis = Fraction(1) * abs(mensur->GetProlatio()) / 2;
    const Fraction brevis = Fraction(1) * abs(mensur->GetTempus());

    // First see if we are expecting a breve and if the duration is long enough
    if (elementDur == DURATION_breve) {
        // First check if this in an imperfecta breve, which can be converted as a non dotted breve
        if (tempusPerfectum && (duration == brevis * 2 / 3)) {
            cmnDurations.push_back(CmnDuration(DURATION_breve, 0));
            duration = 0;
        }
        while (duration >= brevis) {
            cmnDurations.push_back(CmnDuration(DURATION_breve, brevisDots));
            duration = duration - brevis;
            // Check if we can use a dotted breve
            if ((duration == brevis / 2) && (brevisDots == 0)) {
                cmnDurations.back().m_dots = 1;
                duration = 0;
            }
            if ((duration == brevis / 2) && (brevisDots == 0)) {
                cmnDurations.back().m_dots = 1;
                duration = 0;
            }
        }
        // If we have not processed everything, go down to the level of semibrevis
        if (duration != 0) elementDur = DURATION_1;
    }
    // See now if the duration is long enough for a semibrevis
    if (elementDur == DURATION_1) {
        // First check if this in an imperfecta semibreve, which can be converted as a non dotted semibreve
        if (prolatioMajor && (duration == semiBrevis * 2 / 3)) {
            cmnDurations.push_back(CmnDuration(DURATION_1, 0));
            duration = 0;
        }
        while (duration >= semiBrevis) {
            cmnDurations.push_back(CmnDuration(DURATION_1, semiBrevisDots));
            duration = duration - semiBrevis;
            // Check if we can use a dotted whole note
            if ((duration == semiBrevis / 2) && (semiBrevisDots == 0)) {
                cmnDurations.back().m_dots = 1;
                duration = 0;
            }
        }
    }
    // Then process the rest until everything is processed
    while (duration != 0) {
        auto [durPart, remainder] = duration.ToDur();
        Fraction durPartDuration = duration - remainder;
        cmnDurations.push_back(CmnDuration(durPart, 0));
        duration = remainder;
        // Check if we can use a dotted value
        if (duration == durPartDuration / 2) {
            cmnDurations.back().m_dots = 1;
            duration = 0;
        }
    }
}

void ConvertToCmnFunctor::ConvertAccid(Note *cmnNote, const Accid *accid, bool &isFirstNote)
{
    if (!accid) return;

    Accid *cmnAccid = new Accid();
    if (isFirstNote) {
        accid->CopyAttributesTo(cmnAccid);
        isFirstNote = false;
    }
    else {
        cmnAccid->SetAccidGes(Att::AccidentalWrittenToGestural(accid->GetAccid()));
    }
    cmnNote->AddChild(cmnAccid);
}

void ConvertToCmnFunctor::ConvertClef(Clef *cmnClef, const Clef *clef)
{
    assert(cmnClef);

    if (!clef) {
        // Default to C3 clef since this is the most neutral one ambitus-wise
        cmnClef->SetLine(3);
        cmnClef->SetShape(CLEFSHAPE_C);
    }
    else if (clef->GetShape() == CLEFSHAPE_F) {
        cmnClef->SetShape(CLEFSHAPE_F);
        cmnClef->SetLine(4);
    }
    else if (clef->GetShape() == CLEFSHAPE_G) {
        cmnClef->SetShape(CLEFSHAPE_G);
        cmnClef->SetLine(2);
    }
    // Assuming to be C
    else {
        if (clef->GetLine() > 4) {
            cmnClef->SetShape(CLEFSHAPE_F);
            cmnClef->SetLine(4);
        }
        else if (clef->GetLine() > 2) {
            cmnClef->SetShape(CLEFSHAPE_G);
            cmnClef->SetLine(2);
            cmnClef->SetDis(OCTAVE_DIS_8);
            cmnClef->SetDisPlace(STAFFREL_basic_below);
        }
        else {
            cmnClef->SetShape(CLEFSHAPE_G);
            cmnClef->SetLine(2);
        }
    }
}

void ConvertToCmnFunctor::ConvertMensur(const Mensur *mensur)
{
    // We need as least a sign or a num
    if (!mensur->HasSign() && !mensur->HasNum()) return;

    Dir *dir = new Dir();
    dir->SetStaff(m_currentStaff->AttNInteger::StrToXsdPositiveIntegerList(std::to_string(m_currentStaff->GetN())));
    if (m_startid.empty()) {
        dir->SetTstamp(0.0);
    }
    else {
        dir->SetStartid("#" + m_startid);
    }
    dir->SetType("mscore-staff-text");
    dir->SetPlace(STAFFREL_above);
    Text *text = new Text();
    std::string str;
    if (mensur->HasSign()) {
        str += (mensur->GetSign() == MENSURATIONSIGN_C) ? "C" : "O";
        if (mensur->GetOrient() == ORIENTATION_reversed) str += "r";
        if (mensur->HasSlash()) str += "|";
        if (mensur->HasDot()) str += ".";
    }
    if (mensur->HasNum()) {
        str += std::to_string(mensur->GetNum());
        if (mensur->HasNumbase()) str += "/" + std::to_string(mensur->GetNumbase());
    }
    text->SetText(UTF8to32(str));
    dir->AddChild(text);
    (*m_currentMeasure).m_measure->AddChild(dir);
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

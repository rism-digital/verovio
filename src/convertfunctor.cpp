/////////////////////////////////////////////////////////////////////////////
// Name:        convertfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "convertfunctor.h"

//----------------------------------------------------------------------------

#include <ranges>

//----------------------------------------------------------------------------

#include "bracketspan.h"
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

ConvertToCmnFunctor::ConvertToCmnFunctor(Doc *doc, System *targetSystem) : DocFunctor(doc)
{
    m_targetSystem = targetSystem;
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
        for (Object *object : m_durationElements) {
            Note *note = vrv_cast<Note *>(noteObject);
            Note *cmnNote = new Note();
            cmnNote->SetPname(note->GetPname());
            cmnNote->SetOct(note->GetOct());
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

    m_currentLayer = m_layers.begin();
    m_currentMeasure = m_measures.begin();

    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToCmnFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (layerElement->IsSystemElement()) return FUNCTOR_CONTINUE;

    LogDebug(layerElement->GetClassName().c_str());

    if (layerElement->Is(MENSUR)) {
        // replace the current mensur
        m_currentParams.mensur = vrv_cast<Mensur *>(layerElement);
        assert(m_currentParams.mensur);
    }
    else if (layerElement->Is(PROPORT)) {
        // replace the current proport
        const Proport *previous = (m_currentParams.proport) ? (m_currentParams.proport) : NULL;
        m_currentParams.proport = vrv_cast<Proport *>(layerElement);
        assert(m_currentParams.proport);
        if (previous) {
            m_currentParams.proport->Cumulate(previous);
        }
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

    measure->m_measureAligner.LogDebugTree();

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
    if (mensurs.empty() || (mensurs.front().m_time != 0)) {
        mensur.m_time = 0;
        mensurs.push_back(mensur);
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
            measureDuration = this->CalcMeasureDuration((*mensurIter).m_mensur);
            currentMensur = (*mensurIter).m_mensur;
            std::advance(mensurIter, 1);
            next = (mensurIter == mensurs.end()) ? totalTime : (*mensurIter).m_time;
        }
    }

    // Now we are ready to process layers and to move content to m_measures
    return FUNCTOR_CONTINUE;
}

FunctorCode ConvertToCmnFunctor::VisitNote(Note *note)
{
    this->ConvertDurationInterface(note, NOTE);

    // This should not happen, just check it once for all
    if (m_durationElements.empty()) return FUNCTOR_SIBLINGS;

    // Copy the `@pname` and `@oct` from the mensural note
    for (Object *object : m_durationElements) {
        Note *cmnNote = vrv_cast<Note *>(object);
        assert(cmnNote);
        cmnNote->SetPname(note->GetPname());
        cmnNote->SetOct(note->GetOct());
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
            // m_colored->SetLform(LINEFORM_);
            (*m_currentMeasure).m_measure->AddChild(m_coloration);
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
    for (Object *tieEnd : m_durationElements | std::views::drop(1)) {
        Object *measure = tieStart->GetFirstAncestor(MEASURE);
        assert(measure);
        Tie *tie = new Tie();
        tie->SetStartid("#" + tieStart->GetID());
        tie->SetEndid("#" + tieEnd->GetID());
        measure->AddChild(tie);
        tieStart = tieEnd;
    }

    return FUNCTOR_SIBLINGS;
}

FunctorCode ConvertToCmnFunctor::VisitRest(Rest *rest)
{
    this->ConvertDurationInterface(rest, REST);

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

FunctorCode ConvertToCmnFunctor::VisitSystemElement(SystemElement *systemElement)
{
    assert(m_targetSystem);
    systemElement->MoveItselfTo(m_targetSystem);

    return FUNCTOR_CONTINUE;
}

bool ConvertToCmnFunctor::IsGlobalMensur(const Alignment *alignment, const int nbLayers, Mensur &mensur)
{
    if (alignment->GetType() != ALIGNMENT_MENSUR) return false;

    // Not all layers have an alignment and we cannot break here
    if (alignment->GetChildCount() != nbLayers) return false;

    for (const Object *child : alignment->GetChildren()) {
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
    duration = duration * abs(mensur.GetTempus());
    return duration;
}

void ConvertToCmnFunctor::ConvertDurationInterface(DurationInterface *interface, ClassId classId)
{
    m_durationElements.clear();

    data_DURATION noteDur = interface->GetActualDur();
    // Longa and maxima are converted into repeated breves since breve is the longest duration we can fit in a measure
    if (noteDur < DURATION_breve) noteDur = DURATION_breve;

    this->SplitDurationInterface(classId, noteDur, interface->GetScoreTimeOnset() / SCORE_TIME_UNIT,
        interface->GetScoreTimeDuration() / SCORE_TIME_UNIT);
}

#define PROPORT_AS_TUPLET true

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
    // If we go beyon the end of the measure, first processed only what fits
    if (noteEnd > measureEnd) {
        processed = measureEnd - time;
    }

    // The aligne duration features in the proportion - we can apply it back and make tuplets
    Fraction durationWithoutProport = processed;
    if (PROPORT_AS_TUPLET) {
        // Invert apply num and numbase
        if (m_currentParams.proport->HasNum())
            durationWithoutProport = durationWithoutProport * m_currentParams.proport->GetCumulatedNum();
        if (m_currentParams.proport->HasNumbase())
            durationWithoutProport = durationWithoutProport / m_currentParams.proport->GetCumulatedNumbase();
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
        (*m_currentLayer)->AddChild(layerElement);
        interface->SetDur(cmnDuration.m_duration);
        // Add a `@dots` only if not 0
        if (cmnDuration.m_dots != 0) interface->SetDots(cmnDuration.m_dots);

        if (PROPORT_AS_TUPLET) {
            // Reapply num and numbase - should be made a tuplet
            if (m_currentParams.proport->HasNum()) interface->SetNum(m_currentParams.proport->GetCumulatedNum());
            if (m_currentParams.proport->HasNumbase())
                interface->SetNumbase(m_currentParams.proport->GetCumulatedNumbase());
        }
    }

    // If we have reach the end of the measure, go to the next one
    if (time + processed == measureEnd) {
        ++m_currentMeasure;
        ++m_currentLayer;
    }
    // Also check if we have more to process for that note or rest - if yes, call it recursively
    if (duration - processed != 0) {
        this->SplitDurationInterface(classId, noteDur, time = (*m_currentMeasure).m_time, duration - processed);
    }
}

void ConvertToCmnFunctor::SplitDurationIntoCmn(
    data_DURATION elementDur, Fraction duration, const Mensur *mensur, std::list<CmnDuration> &cmnDurations)
{
    bool prolatioMajor = (abs(mensur->GetProlatio()) == 3);
    bool tempusPerfectum = (abs(mensur->GetTempus()) == 3);
    const int semiBrevisDots = (prolatioMajor) ? 1 : 0;
    const int brevisDots = (tempusPerfectum) ? 1 : 0;

    Fraction semiBrevis = Fraction(1, 1) * abs(mensur->GetProlatio()) / 2;
    Fraction brevis = Fraction(1, 1) * abs(mensur->GetTempus());

    // First see if we are expecting a breve and if the duration is long enough
    if (elementDur == DURATION_breve) {
        while (duration >= brevis) {
            cmnDurations.push_back(CmnDuration(DURATION_breve, brevisDots));
            duration = duration - brevis;
            // Check if we can use a dotted breve
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

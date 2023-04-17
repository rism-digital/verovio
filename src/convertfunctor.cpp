/////////////////////////////////////////////////////////////////////////////
// Name:        convertfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "convertfunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "ending.h"
#include "layer.h"
#include "mdiv.h"
#include "page.h"
#include "score.h"
#include "section.h"
#include "staff.h"
#include "system.h"
#include "vrv.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// ConvertToPageBasedFunctor
//----------------------------------------------------------------------------

ConvertToPageBasedFunctor::ConvertToPageBasedFunctor(Page *page)
{
    m_currentSystem = NULL;
    m_page = page;
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

ConvertToCastOffMensuralFunctor::ConvertToCastOffMensuralFunctor(Doc *doc, System *targetSystem, IntTree *layerTree)
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

    Filters *previousFilters = this->GetFilters();
    Filters filters;
    this->SetFilters(&filters);

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

FunctorCode ConvertToCastOffMensuralFunctor::VisitSystemElement(SystemElement *systemElement)
{
    assert(m_targetSystem);
    systemElement->MoveItselfTo(m_targetSystem);

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// ConvertToUnCastOffMensuralFunctor
//----------------------------------------------------------------------------

ConvertToUnCastOffMensuralFunctor::ConvertToUnCastOffMensuralFunctor()
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

} // namespace vrv

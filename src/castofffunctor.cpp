/////////////////////////////////////////////////////////////////////////////
// Name:        castofffunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "castofffunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "editorial.h"
#include "ending.h"
#include "page.h"
#include "pageelement.h"
#include "pagemilestone.h"
#include "sb.h"
#include "system.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// CastOffSystemsFunctor
//----------------------------------------------------------------------------

CastOffSystemsFunctor::CastOffSystemsFunctor(Page *page, Doc *doc, bool smart) : DocFunctor(doc)
{
    m_page = page;
    m_contentSystem = NULL;
    m_currentSystem = NULL;
    m_leftoverSystem = NULL;
    m_shift = 0;
    m_systemWidth = 0;
    m_currentScoreDefWidth = 0;
    m_smart = smart;
}

FunctorCode CastOffSystemsFunctor::VisitEditorialElement(EditorialElement *editorialElement)
{
    // Since the functor returns FUNCTOR_SIBLINGS we should never go lower than the system children
    assert(dynamic_cast<System *>(editorialElement->GetParent()));

    // Special case where we use the Relinquish method.
    // We want to move the editorial element to the currentSystem. However, we cannot use DetachChild
    // from the contentSystem because this screws up the iterator. Relinquish gives up
    // the ownership of the element - the contentSystem will be deleted afterwards.
    editorialElement = vrv_cast<EditorialElement *>(m_contentSystem->Relinquish(editorialElement->GetIdx()));
    assert(editorialElement);
    // move as pending since we want it at the beginning of the system in case of system break coming
    m_pendingElements.push_back(editorialElement);

    return FUNCTOR_SIBLINGS;
}

FunctorCode CastOffSystemsFunctor::VisitEnding(Ending *ending)
{
    // Since the functor returns FUNCTOR_SIBLINGS we should never go lower than the system children
    assert(dynamic_cast<System *>(ending->GetParent()));

    // Special case where we use the Relinquish method.
    // We want to move the ending to the currentSystem. However, we cannot use DetachChild
    // from the contentSystem because this screws up the iterator. Relinquish gives up
    // the ownership of the ending - the contentSystem will be deleted afterwards.
    ending = dynamic_cast<Ending *>(m_contentSystem->Relinquish(ending->GetIdx()));
    // move as pending since we want it at the beginning of the system in case of system break coming
    m_pendingElements.push_back(ending);

    return FUNCTOR_SIBLINGS;
}

FunctorCode CastOffSystemsFunctor::VisitMeasure(Measure *measure)
{
    const bool hasCache = measure->HasCachedHorizontalLayout();
    int overflow = hasCache ? measure->GetCachedOverflow() : measure->GetDrawingOverflow();
    int width = hasCache ? measure->GetCachedWidth() : measure->GetWidth();
    int drawingXRel = measure->GetDrawingXRel();

    Object *nextMeasure = m_contentSystem->GetNext(measure, MEASURE);
    const bool isLeftoverMeasure = ((NULL == nextMeasure) && m_doc->GetOptions()->m_breaksNoWidow.GetValue()
        && (m_doc->GetOptions()->m_breaks.GetValue() != BREAKS_encoded));
    if (m_currentSystem->GetChildCount() > 0) {
        // We have overflowing content (dir, dynam, tempo) larger than 5 units, keep it as pending
        if (overflow > (m_doc->GetDrawingUnit(100) * 5)) {
            measure = dynamic_cast<Measure *>(m_contentSystem->Relinquish(measure->GetIdx()));
            assert(measure);
            // move as pending since we want it not to be broken with the next measure
            m_pendingElements.push_back(measure);
            // continue
            return FUNCTOR_SIBLINGS;
        }
        // Break it if necessary
        else if (drawingXRel + width + m_currentScoreDefWidth - m_shift > m_systemWidth) {
            m_currentSystem = new System();
            m_page->AddChild(m_currentSystem);
            m_shift = drawingXRel;
            // If last measure requires separate system - mark that system as leftover for the future CastOffPages call
            if (isLeftoverMeasure) {
                m_leftoverSystem = m_currentSystem;
            }
            for (Object *oneOfPendingObjects : m_pendingElements) {
                if (oneOfPendingObjects->Is(MEASURE)) {
                    Measure *firstPendingMeasure = vrv_cast<Measure *>(oneOfPendingObjects);
                    assert(firstPendingMeasure);
                    m_shift = firstPendingMeasure->GetCachedXRel();
                    m_leftoverSystem = NULL;
                    // it has to be first measure
                    break;
                }
            }
        }
    }

    // First add all pending objects
    for (Object *element : m_pendingElements) {
        m_currentSystem->AddChild(element);
    }
    m_pendingElements.clear();

    // Special case where we use the Relinquish method.
    measure = dynamic_cast<Measure *>(m_contentSystem->Relinquish(measure->GetIdx()));
    assert(measure);
    m_currentSystem->AddChild(measure);

    return FUNCTOR_SIBLINGS;
}

FunctorCode CastOffSystemsFunctor::VisitPageElement(PageElement *pageElement)
{
    assert(m_page);
    pageElement->MoveItselfTo(m_page);

    return FUNCTOR_CONTINUE;
}

FunctorCode CastOffSystemsFunctor::VisitPageMilestone(PageMilestoneEnd *pageMilestoneEnd)
{
    assert(m_page);
    pageMilestoneEnd->MoveItselfTo(m_page);

    return FUNCTOR_SIBLINGS;
}

FunctorCode CastOffSystemsFunctor::VisitSb(Sb *sb)
{
    if (m_smart) {
        // Get the last measure of the currentSystem
        Measure *measure = dynamic_cast<Measure *>(m_currentSystem->GetChild(m_currentSystem->GetChildCount() - 1));
        if (measure != NULL) {
            int measureRightX = measure->GetDrawingX() + measure->GetWidth() - m_shift;
            double smartSbThresh = m_doc->GetOptions()->m_breaksSmartSb.GetValue();
            if (measureRightX > m_systemWidth * smartSbThresh) {
                // Use this system break.
                m_currentSystem = new System();
                m_page->AddChild(m_currentSystem);
                m_shift += measureRightX;
            }
        }
    }
    // Keep the <sb> in the internal MEI, even if we're not using it to break the system.
    sb->MoveItselfTo(m_currentSystem);
    return FUNCTOR_SIBLINGS;
}

FunctorCode CastOffSystemsFunctor::VisitScoreDef(ScoreDef *scoreDef)
{
    // Since the functor returns FUNCTOR_SIBLINGS we should never go lower than the system children
    assert(dynamic_cast<System *>(scoreDef->GetParent()));

    // Special case where we use the Relinquish method.
    // We want to move the scoreDef to the currentSystem. However, we cannot use DetachChild
    // from the contentSystem because this screws up the iterator. Relinquish gives up
    // the ownership of the scoreDef - the contentSystem will be deleted afterwards.
    scoreDef = dynamic_cast<ScoreDef *>(m_contentSystem->Relinquish(scoreDef->GetIdx()));
    // move as pending since we want it at the beginning of the system in case of system break coming
    m_pendingElements.push_back(scoreDef);
    // This is not perfect since now the scoreDefWidth is the one of the intermediate scoreDefs (and not
    // the initial one - for this to be corrected, we would need two parameters, one for the current initial
    // scoreDef and one for the current that will be the initial one at the next system
    // Also, the abbr label (width) changes would not be taken into account
    m_currentScoreDefWidth = scoreDef->GetDrawingWidth() + m_contentSystem->GetDrawingAbbrLabelsWidth();

    return FUNCTOR_SIBLINGS;
}

FunctorCode CastOffSystemsFunctor::VisitSystem(System *system)
{
    // We are starting a new system we need to cast off
    m_contentSystem = system;
    // We also need to create a new target system and add it to the page
    System *targetSystem = new System();
    m_page->AddChild(targetSystem);
    m_currentSystem = targetSystem;

    m_shift = -system->GetDrawingLabelsWidth();
    m_currentScoreDefWidth = m_page->m_drawingScoreDef.GetDrawingWidth() + system->GetDrawingAbbrLabelsWidth();

    return FUNCTOR_CONTINUE;
}

FunctorCode CastOffSystemsFunctor::VisitSystemEnd(System *system)
{
    if (m_pendingElements.empty()) return FUNCTOR_CONTINUE;

    // Otherwise add all pendings objects
    for (Object *element : m_pendingElements) {
        m_currentSystem->AddChild(element);
    }
    m_pendingElements.clear();

    return FUNCTOR_CONTINUE;
}

FunctorCode CastOffSystemsFunctor::VisitSystemElement(SystemElement *systemElement)
{
    // Since the functor returns FUNCTOR_SIBLINGS we should never go lower than the system children
    assert(dynamic_cast<System *>(systemElement->GetParent()));

    // Special case where we use the Relinquish method.
    systemElement = dynamic_cast<SystemElement *>(m_contentSystem->Relinquish(systemElement->GetIdx()));
    // move as pending since we want it at the beginning of the system in case of system break coming
    m_pendingElements.push_back(systemElement);

    return FUNCTOR_SIBLINGS;
}

FunctorCode CastOffSystemsFunctor::VisitSystemMilestone(SystemMilestoneEnd *systemMilestoneEnd)
{
    // Since the functor returns FUNCTOR_SIBLINGS we should never go lower than the system children
    assert(dynamic_cast<System *>(systemMilestoneEnd->GetParent()));

    // Special case where we use the Relinquish method.
    // We want to move the milestone to the currentSystem. However, we cannot use DetachChild
    // from the contentSystem because this screws up the iterator. Relinquish gives up
    // the ownership of the milestone - the contentSystem will be deleted afterwards.
    systemMilestoneEnd = dynamic_cast<SystemMilestoneEnd *>(m_contentSystem->Relinquish(systemMilestoneEnd->GetIdx()));
    // End milestones are not added to the pending objects because we do not want them to be placed at the beginning of
    // the next system but only if the pending object array is empty (otherwise it will mess up the MEI tree)
    if (m_pendingElements.empty()) {
        m_currentSystem->AddChild(systemMilestoneEnd);
    }
    else {
        m_pendingElements.push_back(systemMilestoneEnd);
    }

    return FUNCTOR_SIBLINGS;
}

//----------------------------------------------------------------------------
// CastOffPagesFunctor
//----------------------------------------------------------------------------

CastOffPagesFunctor::CastOffPagesFunctor(Page *contentPage, Doc *doc, Page *currentPage) : DocFunctor(doc)
{
    m_contentPage = contentPage;
    m_currentPage = currentPage;
    m_shift = 0;
    m_pageHeight = 0;
    m_pgHeadHeight = 0;
    m_pgFootHeight = 0;
    m_pgHead2Height = 0;
    m_pgFoot2Height = 0;
    m_leftoverSystem = NULL;
}

FunctorCode CastOffPagesFunctor::VisitPageEnd(Page *page)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CastOffPagesFunctor::VisitPageElement(PageElement *pageElement)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CastOffPagesFunctor::VisitPageMilestone(PageMilestoneEnd *pageMilestoneEnd)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CastOffPagesFunctor::VisitScore(Score *score)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CastOffPagesFunctor::VisitSystem(System *system)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv

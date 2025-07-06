/////////////////////////////////////////////////////////////////////////////
// Name:        castofffunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "castofffunctor.h"

//----------------------------------------------------------------------------

#include "div.h"
#include "doc.h"
#include "editorial.h"
#include "ending.h"
#include "page.h"
#include "pageelement.h"
#include "pagemilestone.h"
#include "pages.h"
#include "pb.h"
#include "sb.h"
#include "score.h"
#include "staff.h"
#include "system.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// CastOffSystemsFunctor
//----------------------------------------------------------------------------

FunctorCode CastOffSystemsFunctor::VisitDiv(Div *div)
{
    // If we have a previous a Measure or a Div in the System, add a new one
    if ((m_currentSystem->GetChildCount(MEASURE) > 0) || (m_currentSystem->GetChildCount(DIV)) > 0) {
        m_currentSystem = new System();
        m_page->AddChild(m_currentSystem);
    }

    div->MoveItselfTo(m_currentSystem);

    // Always add a System after an Div
    m_currentSystem = new System();
    m_page->AddChild(m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

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
            for (Object *pendingElement : m_pendingElements) {
                if (pendingElement->Is(MEASURE)) {
                    Measure *firstPendingMeasure = vrv_cast<Measure *>(pendingElement);
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
    for (Object *pendingElement : m_pendingElements) {
        m_currentSystem->AddChild(pendingElement);
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

    return FUNCTOR_SIBLINGS;
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
    for (Object *pendingElement : m_pendingElements) {
        m_currentSystem->AddChild(pendingElement);
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
    m_firstCastOffPage = true;
    m_shift = VRV_UNSET;
    m_pageHeight = 0;
    m_pgHeadHeight = 0;
    m_pgFootHeight = 0;
    m_pgHead2Height = 0;
    m_pgFoot2Height = 0;
    m_leftoverSystem = NULL;
}

FunctorCode CastOffPagesFunctor::VisitPageEnd(Page *page)
{
    if (m_pendingPageElements.empty()) return FUNCTOR_CONTINUE;

    // Otherwise add all pendings objects
    for (Object *pendingElement : m_pendingPageElements) {
        m_currentPage->AddChild(pendingElement);
    }
    m_pendingPageElements.clear();

    return FUNCTOR_CONTINUE;
}

FunctorCode CastOffPagesFunctor::VisitPageElement(PageElement *pageElement)
{
    pageElement = dynamic_cast<PageElement *>(m_contentPage->Relinquish(pageElement->GetIdx()));
    assert(pageElement);
    // move as pending since we want it at the beginning of the page in case of system break coming
    m_pendingPageElements.push_back(pageElement);

    return FUNCTOR_SIBLINGS;
}

FunctorCode CastOffPagesFunctor::VisitPageMilestone(PageMilestoneEnd *pageMilestoneEnd)
{
    assert(m_currentPage);

    pageMilestoneEnd = dynamic_cast<PageMilestoneEnd *>(m_contentPage->Relinquish(pageMilestoneEnd->GetIdx()));
    // End milestones can be added to the page only if the pending list is empty
    // Otherwise we are going to mess up the order
    if (m_pendingPageElements.empty()) {
        m_currentPage->AddChild(pageMilestoneEnd);
    }
    else {
        m_pendingPageElements.push_back(pageMilestoneEnd);
    }

    return FUNCTOR_SIBLINGS;
}

FunctorCode CastOffPagesFunctor::VisitScore(Score *score)
{
    this->VisitPageElement(score);

    m_pgHeadHeight = score->m_drawingPgHeadHeight;
    m_pgFootHeight = score->m_drawingPgFootHeight;
    m_pgHead2Height = score->m_drawingPgHead2Height;
    m_pgFoot2Height = score->m_drawingPgFoot2Height;

    return FUNCTOR_SIBLINGS;
}

FunctorCode CastOffPagesFunctor::VisitSystem(System *system)
{
    // Check if this is the first system
    if (m_shift == VRV_UNSET) {
        m_shift = system->GetDrawingYRel();
    }

    const int systemMaxPerPage = m_doc->GetOptions()->m_systemMaxPerPage.GetValue();
    const int systemChildCount = m_currentPage->GetChildCount(SYSTEM);
    if ((systemMaxPerPage && (systemMaxPerPage == systemChildCount))
        || ((systemChildCount > 0)
            && (m_shift - system->GetDrawingYRel() + system->GetHeight() > this->GetAvailableDrawingHeight()))) {
        // If this is the last system in the list, it doesn't fit the page and it's a leftover system (has just one
        // measure) => add the system content to the previous system
        Object *nextSystem = m_contentPage->GetNext(system, SYSTEM);
        Object *lastSystem = m_currentPage->GetLast(SYSTEM);
        if (!nextSystem && lastSystem && (system == m_leftoverSystem)) {
            ArrayOfObjects &children = system->GetChildrenForModification();
            for (Object *child : children) {
                child->MoveItselfTo(lastSystem);
            }
            return FUNCTOR_SIBLINGS;
        }

        m_currentPage = new Page();
        assert(m_doc->GetPages());
        m_doc->GetPages()->AddChild(m_currentPage);
        m_shift = system->GetDrawingYRel();
        m_firstCastOffPage = false;
    }

    // First add all pending objects
    for (Object *pendingElement : m_pendingPageElements) {
        m_currentPage->AddChild(pendingElement);
    }
    m_pendingPageElements.clear();

    // Special case where we use the Relinquish method.
    // We want to move the system to the currentPage. However, we cannot use DetachChild
    // from the contentPage because this screws up the iterator. Relinquish gives up
    // the ownership of the system - the contentPage itself will be deleted afterwards.
    system = vrv_cast<System *>(m_contentPage->Relinquish(system->GetIdx()));
    assert(system);
    m_currentPage->AddChild(system);

    return FUNCTOR_SIBLINGS;
}

int CastOffPagesFunctor::GetAvailableDrawingHeight() const
{
    const int pageHeadAndFootHeight
        = m_firstCastOffPage ? (m_pgHeadHeight + m_pgFootHeight) : (m_pgHead2Height + m_pgFoot2Height);
    return m_pageHeight - pageHeadAndFootHeight;
}

//----------------------------------------------------------------------------
// CastOffEncodingFunctor
//----------------------------------------------------------------------------

CastOffEncodingFunctor::CastOffEncodingFunctor(Doc *doc, Page *currentPage, bool usePages) : DocFunctor(doc)
{
    m_currentPage = currentPage;
    m_currentSystem = NULL;
    m_contentSystem = NULL;
    m_usePages = usePages;
}

FunctorCode CastOffEncodingFunctor::VisitDiv(Div *div)
{
    assert(m_currentSystem);
    div->MoveItselfTo(m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

FunctorCode CastOffEncodingFunctor::VisitEditorialElement(EditorialElement *editorialElement)
{
    // Only move editorial elements that are a child of the system
    if (editorialElement->GetParent() && editorialElement->GetParent()->Is(SYSTEM)) {
        assert(m_currentSystem);
        editorialElement->MoveItselfTo(m_currentSystem);
    }

    return FUNCTOR_SIBLINGS;
}

FunctorCode CastOffEncodingFunctor::VisitEnding(Ending *ending)
{
    assert(m_currentSystem);
    ending->MoveItselfTo(m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

FunctorCode CastOffEncodingFunctor::VisitMeasure(Measure *measure)
{
    assert(m_currentSystem);
    measure->MoveItselfTo(m_currentSystem);

    return FUNCTOR_CONTINUE;
}

FunctorCode CastOffEncodingFunctor::VisitPageElement(PageElement *pageElement)
{
    assert(m_currentPage);
    pageElement->MoveItselfTo(m_currentPage);

    return FUNCTOR_SIBLINGS;
}

FunctorCode CastOffEncodingFunctor::VisitPageMilestone(PageMilestoneEnd *pageMilestoneEnd)
{
    if (pageMilestoneEnd->GetStart() && pageMilestoneEnd->GetStart()->Is(SCORE)) {
        // This is the end of a score, which means that the current system has to be added
        // to the current page
        assert(m_currentSystem);
        m_currentPage->AddChild(m_currentSystem);
        m_currentSystem = NULL;
    }

    assert(m_currentPage);
    pageMilestoneEnd->MoveItselfTo(m_currentPage);

    return FUNCTOR_SIBLINGS;
}

FunctorCode CastOffEncodingFunctor::VisitPb(Pb *pb)
{
    // We look if the current system has a pb or at least one measure, or the current page has at least a system
    // if yes, we assume that the <pb> is not the one at the beginning of the content.
    // This is not very robust but at least make it work when rendering a <mdiv> that does not start with a <pb> (which
    // we cannot force)
    if ((m_currentSystem->GetChildCount(PB) > 0) || (m_currentSystem->GetChildCount(MEASURE) > 0)
        || (m_currentPage->GetChildCount(SYSTEM) > 0)) {
        m_currentPage->AddChild(m_currentSystem);
        m_currentSystem = new System();
        if (m_usePages) {
            m_currentPage = new Page();
            assert(m_doc->GetPages());
            m_doc->GetPages()->AddChild(m_currentPage);
        }
    }

    assert(m_currentSystem);
    pb->MoveItselfTo(m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

FunctorCode CastOffEncodingFunctor::VisitSb(Sb *sb)
{
    // We look if the current system has at least one measure - if yes, we assume that the <sb>
    // is not the one at the beginning of the content (<mdiv>). This is not very robust but at least make it
    // work when rendering a <mdiv> that does not start with a <pb> or a <sb> (which we cannot enforce)
    if (m_currentSystem->GetChildCount(MEASURE) > 0 || m_currentSystem->GetChildCount(DIV) > 0) {
        m_currentPage->AddChild(m_currentSystem);
        m_currentSystem = new System();
    }

    assert(m_currentSystem);
    sb->MoveItselfTo(m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

FunctorCode CastOffEncodingFunctor::VisitScoreDef(ScoreDef *scoreDef)
{
    assert(m_currentSystem);
    scoreDef->MoveItselfTo(m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

FunctorCode CastOffEncodingFunctor::VisitStaff(Staff *staff)
{
    // Staff alignments must be reset, otherwise they would dangle whenever they belong to a deleted system
    staff->SetAlignment(NULL);
    return FUNCTOR_SIBLINGS;
}

FunctorCode CastOffEncodingFunctor::VisitSystem(System *system)
{
    // We are starting a new system we need to cast off
    m_contentSystem = system;
    // Create the new system but do not add it to the page yet.
    // It will be added when reaching a pb / sb or at the end of the score in PageMilestoneEnd::CastOffEncoding
    assert(!m_currentSystem);
    m_currentSystem = new System();

    return FUNCTOR_CONTINUE;
}

FunctorCode CastOffEncodingFunctor::VisitSystemElement(SystemElement *systemElement)
{
    assert(m_currentSystem);
    systemElement->MoveItselfTo(m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

//----------------------------------------------------------------------------
// UnCastOffFunctor
//----------------------------------------------------------------------------

UnCastOffFunctor::UnCastOffFunctor(Page *page) : Functor()
{
    m_page = page;
    m_currentSystem = NULL;
    m_resetCache = true;
}

FunctorCode UnCastOffFunctor::VisitFloatingObject(FloatingObject *floatingObject)
{
    floatingObject->SetCurrentFloatingPositioner(NULL);

    return FUNCTOR_SIBLINGS;
}

FunctorCode UnCastOffFunctor::VisitMeasure(Measure *measure)
{
    if (m_resetCache) {
        measure->ResetCachedXRel();
        measure->ResetCachedWidth();
        measure->ResetCachedOverflow();
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode UnCastOffFunctor::VisitPageElement(PageElement *pageElement)
{
    pageElement->MoveItselfTo(m_page);

    return FUNCTOR_SIBLINGS;
}

FunctorCode UnCastOffFunctor::VisitPageMilestone(PageMilestoneEnd *pageMilestoneEnd)
{
    if (pageMilestoneEnd->GetStart() && pageMilestoneEnd->GetStart()->Is(SCORE)) {
        // This is the end of a score, which means that nothing else should be added to
        // the current system and we set it to NULL;
        assert(m_currentSystem);
        m_currentSystem = NULL;
    }

    pageMilestoneEnd->MoveItselfTo(m_page);

    return FUNCTOR_SIBLINGS;
}

FunctorCode UnCastOffFunctor::VisitScore(Score *score)
{
    this->VisitPageElement(score);

    assert(!m_currentSystem);
    System *system = new System();
    m_currentSystem = system;
    m_page->AddChild(system);

    return FUNCTOR_SIBLINGS;
}

FunctorCode UnCastOffFunctor::VisitSystem(System *system)
{
    // Just move all the content of the system to the continuous one
    // Use the MoveChildrenFrom method that moves and relinquishes them
    // See Object::Relinquish
    m_currentSystem->MoveChildrenFrom(system);

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// CastOffToSelectionFunctor
//----------------------------------------------------------------------------

CastOffToSelectionFunctor::CastOffToSelectionFunctor(
    Page *page, Doc *doc, const std::string &start, const std::string &end)
    : DocFunctor(doc)
{
    m_page = page;
    m_contentSystem = NULL;
    m_currentSystem = NULL;
    m_start = start;
    m_end = end;
    m_isSelection = false;
}

FunctorCode CastOffToSelectionFunctor::VisitDiv(Div *div)
{
    assert(m_currentSystem);
    div->MoveItselfTo(m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

FunctorCode CastOffToSelectionFunctor::VisitEditorialElement(EditorialElement *editorialElement)
{
    assert(m_currentSystem);
    editorialElement->MoveItselfTo(m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

FunctorCode CastOffToSelectionFunctor::VisitMeasure(Measure *measure)
{
    const bool startSelection = !m_isSelection && (measure->GetID() == m_start);

    if (startSelection) {
        m_page = new Page();
        m_doc->GetPages()->AddChild(m_page);
        m_currentSystem = new System();
        m_page->AddChild(m_currentSystem);
        m_isSelection = true;
    }

    const bool endSelection = m_isSelection && (measure->GetID() == m_end);

    assert(m_currentSystem);
    measure->MoveItselfTo(m_currentSystem);

    if (endSelection) {
        m_page = new Page();
        m_doc->GetPages()->AddChild(m_page);
        m_currentSystem = new System();
        m_page->AddChild(m_currentSystem);
        m_isSelection = false;
    }

    return FUNCTOR_SIBLINGS;
}

FunctorCode CastOffToSelectionFunctor::VisitPageElement(PageElement *pageElement)
{
    assert(m_page);
    pageElement->MoveItselfTo(m_page);

    return FUNCTOR_SIBLINGS;
}

FunctorCode CastOffToSelectionFunctor::VisitPageMilestone(PageMilestoneEnd *pageMilestoneEnd)
{
    assert(m_page);
    pageMilestoneEnd->MoveItselfTo(m_page);

    return FUNCTOR_SIBLINGS;
}

FunctorCode CastOffToSelectionFunctor::VisitScoreDef(ScoreDef *scoreDef)
{
    assert(m_currentSystem);
    scoreDef->MoveItselfTo(m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

FunctorCode CastOffToSelectionFunctor::VisitSystem(System *system)
{
    // We are starting a new system we need to cast off
    m_contentSystem = system;
    // We also need to create a new target system and add it to the page
    System *targetSystem = new System();
    m_page->AddChild(targetSystem);
    m_currentSystem = targetSystem;

    return FUNCTOR_CONTINUE;
}

FunctorCode CastOffToSelectionFunctor::VisitSystemElement(SystemElement *systemElement)
{
    assert(m_currentSystem);
    systemElement->MoveItselfTo(m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

FunctorCode CastOffToSelectionFunctor::VisitSystemMilestone(SystemMilestoneEnd *systemMilestoneEnd)
{
    assert(m_currentSystem);
    systemMilestoneEnd->MoveItselfTo(m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv

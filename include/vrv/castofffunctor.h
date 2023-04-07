/////////////////////////////////////////////////////////////////////////////
// Name:        castofffunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CASTOFFFUNCTOR_H__
#define __VRV_CASTOFFFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// CastOffSystemsFunctor
//----------------------------------------------------------------------------

/**
 * This class fills a page by adding systems with the appropriate length.
 * Adds all the pending objects when reaching the end.
 */
class CastOffSystemsFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    CastOffSystemsFunctor(Page *page, Doc *doc, bool smart);
    virtual ~CastOffSystemsFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Retrieve the leftover system
     */
    System *GetLeftoverSystem() const { return m_leftoverSystem; }

    /*
     * Set the system width
     */
    void SetSystemWidth(int width) { m_systemWidth = width; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitEditorialElement(EditorialElement *editorialElement) override;
    FunctorCode VisitEnding(Ending *ending) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitPageElement(PageElement *pageElement) override;
    FunctorCode VisitPageMilestone(PageMilestoneEnd *pageMilestoneEnd) override;
    FunctorCode VisitSb(Sb *sb) override;
    FunctorCode VisitScoreDef(ScoreDef *scoreDef) override;
    FunctorCode VisitSystem(System *system) override;
    FunctorCode VisitSystemEnd(System *system) override;
    FunctorCode VisitSystemElement(SystemElement *systemElement) override;
    FunctorCode VisitSystemMilestone(SystemMilestoneEnd *systemMilestoneEnd) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The system we are taking the content from
    System *m_contentSystem;
    // The page we are adding the system to
    Page *m_page;
    // The current system
    System *m_currentSystem;
    // The cumulated shift (m_drawingXRel of the first measure of the current system)
    int m_shift;
    // The system width
    int m_systemWidth;
    // The current scoreDef width
    int m_currentScoreDefWidth;
    // The pending elements (ScoreDef, Endings, etc.) to be placed at the beginning of a system
    ArrayOfObjects m_pendingElements;
    // Indicates to smartly use encoded system breaks
    bool m_smart;
    // The leftover system (last system with only one measure)
    System *m_leftoverSystem;
};

//----------------------------------------------------------------------------
// CastOffPagesFunctor
//----------------------------------------------------------------------------

/**
 * This class fills a document by adding pages with the appropriate length.
 */
class CastOffPagesFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    CastOffPagesFunctor(Page *contentPage, Doc *doc, Page *currentPage);
    virtual ~CastOffPagesFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Set the leftover system
     */
    void SetLeftoverSystem(System *system) { m_leftoverSystem = system; }

    /*
     * Set the page height
     */
    void SetPageHeight(int height) { m_pageHeight = height; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitPageEnd(Page *page) override;
    FunctorCode VisitPageElement(PageElement *pageElement) override;
    FunctorCode VisitPageMilestone(PageMilestoneEnd *pageMilestoneEnd) override;
    FunctorCode VisitScore(Score *score) override;
    FunctorCode VisitSystem(System *system) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The page we are taking the content from
    Page *m_contentPage;
    // The current page
    Page *m_currentPage;
    // The cumulated shift (m_drawingYRel of the first system of the current page)
    int m_shift;
    // The page heights
    int m_pageHeight;
    int m_pgHeadHeight;
    int m_pgFootHeight;
    int m_pgHead2Height;
    int m_pgFoot2Height;
    // The leftover system (last system with only one measure)
    System *m_leftoverSystem;
    // The pending elements (Mdiv, Score) to be placed at the beginning of a page
    ArrayOfObjects m_pendingPageElements;
};

} // namespace vrv

#endif // __VRV_CASTOFFFUNCTOR_H__

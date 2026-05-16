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
    FunctorCode VisitDiv(Div *div) override;
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
    /*
     * Returns the available height for system drawing on the current page
     */
    int GetAvailableDrawingHeight() const;

public:
    //
private:
    // The page we are taking the content from
    Page *m_contentPage;
    // The current page
    Page *m_currentPage;
    // Indicates whether the current page is the first
    bool m_firstCastOffPage;
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

//----------------------------------------------------------------------------
// CastOffEncodingFunctor
//----------------------------------------------------------------------------

/**
 * This class casts off the document according to the encoding provided (pb and sb).
 */
class CastOffEncodingFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    CastOffEncodingFunctor(Doc *doc, Page *currentPage, bool usePages = true);
    virtual ~CastOffEncodingFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitDiv(Div *div) override;
    FunctorCode VisitEditorialElement(EditorialElement *editorialElement) override;
    FunctorCode VisitEnding(Ending *ending) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitPageElement(PageElement *pageElement) override;
    FunctorCode VisitPageMilestone(PageMilestoneEnd *pageMilestoneEnd) override;
    FunctorCode VisitPb(Pb *pb) override;
    FunctorCode VisitSb(Sb *sb) override;
    FunctorCode VisitScoreDef(ScoreDef *scoreDef) override;
    FunctorCode VisitStaff(Staff *staff) override;
    FunctorCode VisitSystem(System *system) override;
    FunctorCode VisitSystemElement(SystemElement *systemElement) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The current page
    Page *m_currentPage;
    // The system we are taking the content from
    System *m_contentSystem;
    // The current system
    System *m_currentSystem;
    // Indicates if we want to use the pageBreaks from the document
    bool m_usePages;
};

//----------------------------------------------------------------------------
// UnCastOffFunctor
//----------------------------------------------------------------------------

/**
 * This class undoes the cast off for both pages and systems.
 * This is used by Doc::UnCastOffDoc for putting all pages / systems continously.
 */
class UnCastOffFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    UnCastOffFunctor(Page *page);
    virtual ~UnCastOffFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Set the reset cache flag
     */
    void SetResetCache(bool resetCache) { m_resetCache = resetCache; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitFloatingObject(FloatingObject *floatingObject) override;
    FunctorCode VisitMeasure(Measure *measure) override;
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
    // The page we are adding systems to
    Page *m_page;
    // The system we are adding content to
    System *m_currentSystem;
    // Indicates if we need to reset the horizontal layout cache
    bool m_resetCache;
};

//----------------------------------------------------------------------------
// CastOffToSelectionFunctor
//----------------------------------------------------------------------------

/**
 * This class casts off a document to selection.
 * Moves everything before the selection to the first page, the selection to a second page,
 * and everything after the selection to a third page.
 */
class CastOffToSelectionFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    CastOffToSelectionFunctor(Page *page, Doc *doc, const std::string &start, const std::string &end);
    virtual ~CastOffToSelectionFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitDiv(Div *div) override;
    FunctorCode VisitEditorialElement(EditorialElement *editorialElement) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitPageElement(PageElement *pageElement) override;
    FunctorCode VisitPageMilestone(PageMilestoneEnd *pageMilestoneEnd) override;
    FunctorCode VisitScoreDef(ScoreDef *scoreDef) override;
    FunctorCode VisitSystem(System *system) override;
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
    // The page we are adding systems to
    Page *m_page;
    // The current system
    System *m_currentSystem;
    // The start measure ID of the selection
    std::string m_start;
    // The end measure ID of the selection
    std::string m_end;
    // Indicates whether we are currently in the selection range
    bool m_isSelection;
};

} // namespace vrv

#endif // __VRV_CASTOFFFUNCTOR_H__

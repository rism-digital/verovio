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

} // namespace vrv

#endif // __VRV_CASTOFFFUNCTOR_H__

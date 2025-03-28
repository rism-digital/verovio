/////////////////////////////////////////////////////////////////////////////
// Name:        pages.h
// Author:      Laurent Pugin
// Created:     2018/02/15
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PAGES_H__
#define __VRV_PAGES_H__

#include "atts_shared.h"
#include "object.h"

namespace vrv {

class Page;
class Score;
class System;

//----------------------------------------------------------------------------
// Pages
//----------------------------------------------------------------------------

/**
 * This class represent a <pages> in page-based MEI.
 */
class Pages : public Object, public AttLabelled, public AttNNumberLike {

public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Pages();
    virtual ~Pages();
    void Reset() override;
    std::string GetClassName() const override { return "pages"; }
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    bool IsSupportedChild(ClassId classId) override;
    ///@}

    /**
     *
     */
    void ConvertFrom(Score *score);

    /**
     * Refresh the layout of all the child pages.
     */
    void LayOutAll();

    //----------//
    // Functors //
    //----------//

    /**
     * Interface for class functor visitation
     */
    ///@{
    FunctorCode Accept(Functor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(Functor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
    ///@}

private:
    //
public:
    //
private:
    //
};

//----------------------------------------------------------------------------
// PageRange
//----------------------------------------------------------------------------

/**
 * This class represent a page range not owning child pages.
 */
class PageRange : public Pages {

public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    PageRange(Doc *doc);
    virtual ~PageRange();
    void Reset() override;
    ///@}

    /**
     * The the specified page as focus page in the range.
     */
    void SetAsFocus(Page *page);

private:
    void Evaluate(const Object *object);

    void EvaluateSpanningElementsIn(const Object *measure);

public:
    //
private:
    /** A pointer to the doc owning the page range */
    Doc *m_doc;
    /** A pointer to the page with the focus in the range */
    Page *m_focusPage;

    std::list<Page *> m_pageBefore;
    std::list<Page *> m_pageAfter;
};

} // namespace vrv

#endif

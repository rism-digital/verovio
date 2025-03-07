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
    std::string GetClassName() const override { return "Pages"; }
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    bool IsSupportedChild(Object *object) override;
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
// FocusSet
//----------------------------------------------------------------------------

/**
 * This class represent a <pages> in page-based MEI.
 */
class FocusSet : public Pages {

public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    FocusSet(Doc *doc);
    virtual ~FocusSet();
    void Reset() override;
    ///@}

    /**
     * The the specified page as focus page in the set.
     */
    void SetAsFocus(Page *page);

private:
    //
public:
    //
private:
    /** A pointer to the doc owning the focus set */
    Doc *m_doc;
    /** A pointer to the page with the focus */
    Page *m_focus;
};

} // namespace vrv

#endif

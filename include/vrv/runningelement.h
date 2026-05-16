/////////////////////////////////////////////////////////////////////////////
// Name:        runningelement.h
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_RUNNING_ELEMENT_H__
#define __VRV_RUNNING_ELEMENT_H__

#include "atts_shared.h"
#include "textlayoutelement.h"

namespace vrv {

class Page;
class Staff;
class TextElement;

//----------------------------------------------------------------------------
// RunningElement
//----------------------------------------------------------------------------

/**
 * This class represents running elements (headers and footers).
 * It is not an abstract class but should not be instanciated directly.
 */
class RunningElement : public TextLayoutElement, public AttFormework {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method resets all attribute classes
     */
    ///@{
    // RunningElement();
    RunningElement(ClassId classId);
    virtual ~RunningElement();
    void Reset() override;
    ///@}

    /**
     * Disable cloning of the running elements (for now?).
     * It does not make sense you carry copying the running element across the systems.
     */
    Object *Clone() const override { return NULL; }

    /**
     * @name Setter and getter of the generated flag
     */
    ///@{
    bool IsGenerated() const { return m_isGenerated; }
    void IsGenerated(bool isGenerated) { m_isGenerated = isGenerated; }
    ///@}

    /**
     * @name Get and set the X and Y drawing position
     */
    ///@{
    int GetDrawingX() const override;
    int GetDrawingY() const override;
    ///@}

    int GetTotalWidth(const Doc *doc) const override;

    /*
     * @name Setter and getter for the current drawing page
     */
    ///@{
    void SetDrawingPage(Page *page);
    Page *GetDrawingPage() { return m_drawingPage; }
    const Page *GetDrawingPage() const { return m_drawingPage; }
    ///@}

    /**
     * @name Get and set the X and Y drawing relative positions
     */
    ///@{
    int GetDrawingYRel() const { return m_drawingYRel; }
    virtual void SetDrawingYRel(int drawingYRel);
    ///@}

    /**
     * Set the current page number by looking for a <num label="page">#</num> element.
     */
    void SetCurrentPageNum(const Page *currentPage);

    /**
     * Load the footer from the resources (footer.svg)
     */
    void LoadFooter(const Doc *doc);

    /**
     * Add page numbering to the running element.
     */
    void AddPageNum(data_HORIZONTALALIGNMENT halign, data_VERTICALALIGNMENT valign);

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
    /**
     * The page we are drawing (for the x position)
     */
    Page *m_drawingPage;

    /**
     * The y position of the running element
     */
    int m_drawingYRel;

    /**
     * Flag indicating whether or not the element was generated
     */
    bool m_isGenerated;
};

} // namespace vrv

#endif

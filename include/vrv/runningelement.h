/////////////////////////////////////////////////////////////////////////////
// Name:        runningelement.h
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_RUNNING_ELEMENT_H__
#define __VRV_RUNNING_ELEMENT_H__

#include "atts_shared.h"
#include "object.h"

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
class RunningElement : public Object, public ObjectListInterface, public AttHorizontalAlign, public AttTyped {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method resets all attribute classes
     */
    ///@{
    RunningElement();
    RunningElement(ClassId classId);
    RunningElement(ClassId classId, const std::string &classIdStr);
    virtual ~RunningElement();
    void Reset() override;
    ///@}

    /**
     * Disable cloning of the running elements (for now?).
     * It does not make sense you carry copying the running element across the systems.
     */
    Object *Clone() const override { return NULL; }

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    bool IsSupportedChild(Object *object) override;
    ///@}

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

    int GetWidth() const;

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
     * @name Get the size of row, cols or cells
     */
    ///@{
    /** Height including margins */
    virtual int GetTotalHeight(const Doc *doc) const = 0;
    /** Content height */
    int GetContentHeight() const;
    /** Row from 0 to 2 */
    int GetRowHeight(int row) const;
    /** Col from 0 to 2 */
    int GetColHeight(int col) const;
    /** Row from 0 to 8 */
    int GetCellHeight(int cell) const;
    ///@}

    /**
     * Scale the content of the running element.
     * Currently unused.
     */
    bool AdjustDrawingScaling(int width);

    /**
     * Adjust the postition of the content of the running element.
     * First adjust the content of each cell, and then the cells themselves.
     */
    bool AdjustRunningElementYPos();

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
     * See Object::PrepareDataInitialization
     */
    int PrepareDataInitialization(FunctorParams *) override;

    /**
     * See Object::Save
     */
    ///@{
    int Save(FunctorParams *functorParams) override;
    int SaveEnd(FunctorParams *functorParams) override;
    ///@}

    /**
     * See Object::AlignVertically
     */
    ///@{
    int AlignVertically(FunctorParams *functorParams) override;
    ///@}

protected:
    /**
     * Filter the list for a specific class.
     * Keep only the top <rend> and <fig>
     */
    void FilterList(ListOfConstObjects &childList) const override;

private:
    /**
     *
     */
    int GetAlignmentPos(data_HORIZONTALALIGNMENT h, data_VERTICALALIGNMENT v) const;

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
     * Stored the top <rend> or <fig> with the 9 possible positioning combinations, from
     * top-left to bottom-right (going left to right first)
     */
    ArrayOfTextElements m_cells[9];

    /**
     * Flag indicating whether or not the element was generated
     */
    bool m_isGenerated;

    /**
     *
     */
    int m_drawingScalingPercent[3];
};

} // namespace vrv

#endif

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
    RunningElement(const std::string &classid);
    virtual ~RunningElement();
    virtual void Reset();
    virtual ClassId GetClassId() const { return RUNNING_ELEMENT; }
    ///@}

    /**
     * Disable cloning of the running elements (for now?).
     * It does not make sense you carry copying the running element across the systems.
     */
    virtual Object *Clone() const { return NULL; }

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    virtual bool IsSupportedChild(Object *object);
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
    virtual int GetDrawingX() const;
    virtual int GetDrawingY() const;
    ///@}

    int GetWidth() const;

    /*
     * @name Setter and getter for the current drawing page
     */
    ///@{
    void SetDrawingPage(Page *page);
    Page *GetDrawingPage() { return m_drawingPage; }
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
    int GetTotalHeight();
    /** Row from 0 to 2 */
    int GetRowHeight(int row);
    /** Col from 0 to 2 */
    int GetColHeight(int col);
    /** Row from 0 to 8 */
    int GetCellHeight(int cell);
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
    void SetCurrentPageNum(Page *currentPage);

    /**
     * Load the footer from the resources (footer.svg)
     */
    void LoadFooter();

    /**
     * Add page numbering to the running element.
     */
    void AddPageNum(data_HORIZONTALALIGNMENT halign, data_VERTICALALIGNMENT valign);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::Save
     */
    ///@{
    virtual int Save(FunctorParams *functorParams);
    virtual int SaveEnd(FunctorParams *functorParams);
    ///@}

    /**
     * See Object::AlignVertically
     */
    ///@{
    virtual int AlignVertically(FunctorParams *functorParams);
    ///@}

protected:
    /**
     * Filter the list for a specific class.
     * Keep only the top <rend> and <fig>
     */
    virtual void FilterList(ArrayOfObjects *childList);

private:
    /**
     *
     */
    int GetAlignmentPos(data_HORIZONTALALIGNMENT h, data_VERTICALALIGNMENT v);

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

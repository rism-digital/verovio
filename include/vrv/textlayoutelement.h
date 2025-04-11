/////////////////////////////////////////////////////////////////////////////
// Name:        textlayoutelement.h
// Author:      Laurent Pugin
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TEXT_LAYOUT_ELEMENT_H__
#define __VRV_TEXT_LAYOUT_ELEMENT_H__

#include "atts_shared.h"
#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// TextLayoutElement
//----------------------------------------------------------------------------

/**
 * This class represents a text element featuring horizontal and vertical layout.
 * It is not an abstract class but should not be instanciated directly.
 */
class TextLayoutElement : public Object, public ObjectListInterface, public AttTyped {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    TextLayoutElement(ClassId classId);
    virtual ~TextLayoutElement();
    void Reset() override;
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    bool IsSupportedChild(ClassId classId) override;
    ///@}

    /**
     * @name Setter and getter for the text element cells
     */
    ///@{
    void ResetCells();
    void AppendTextToCell(int index, TextElement *text);
    ///@}

    /**
     * @name Setter and getter for the drawing scaling
     */
    ///@{
    void ResetDrawingScaling();
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
    // Width including margins */
    virtual int GetTotalWidth(const Doc *doc) const = 0;
    /** Content width */
    int GetContentWidth() const;
    /** Row from 0 to 2 */
    int GetRowWidth(int col) const;
    /** Col from 0 to 2 */
    int GetColWidth(int col) const;
    /** Row from 0 to 8 */
    int GetCellWidth(int cell) const;

    /**
     * Calculate the cell position
     */
    int GetAlignmentPos(data_HORIZONTALALIGNMENT h, data_VERTICALALIGNMENT v) const;

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

protected:
    /**
     * Filter the list for a specific class.
     * Keep only the top <rend> and <fig>
     */
    void FilterList(ListOfConstObjects &childList) const override;

private:
    //
public:
    //
private:
    /**
     * Stored the top <rend> or <fig> with the 9 possible positioning combinations, from
     * top-left to bottom-right (going left to right first)
     */
    ArrayOfTextElements m_cells[9];

    /**
     *
     */
    int m_drawingScalingPercent[3];
};

} // namespace vrv

#endif

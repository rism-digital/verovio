/////////////////////////////////////////////////////////////////////////////
// Name:        textlayoutelement.cpp
// Author:      Laurent Pugin
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "textlayoutelement.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "doc.h"
#include "editorial.h"
#include "fig.h"
#include "functor.h"
#include "rend.h"

namespace vrv {

//----------------------------------------------------------------------------
// TextLayoutElement
//----------------------------------------------------------------------------

TextLayoutElement::TextLayoutElement(ClassId classId, const std::string &classIdStr)
    : Object(classId, classIdStr), ObjectListInterface(), AttTyped()
{
    this->RegisterAttClass(ATT_TYPED);

    this->Reset();
}

TextLayoutElement::~TextLayoutElement() {}

void TextLayoutElement::Reset()
{
    Object::Reset();
    this->ResetTyped();
}

bool TextLayoutElement::IsSupportedChild(Object *child)
{
    if (child->IsTextElement()) {
        assert(dynamic_cast<TextElement *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

void TextLayoutElement::FilterList(ListOfConstObjects &childList) const
{
    ListOfConstObjects::iterator iter = childList.begin();

    while (iter != childList.end()) {
        // remove nested rend elements
        if ((*iter)->Is(REND)) {
            if ((*iter)->GetFirstAncestor(REND)) {
                iter = childList.erase(iter);
                continue;
            }
        }
        // Also remove anything that is not a fig
        else if (!(*iter)->Is(FIG)) {
            iter = childList.erase(iter);
            continue;
        }
        ++iter;
    }
}

void TextLayoutElement::ResetCells()
{
    for (int i = 0; i < 9; ++i) {
        m_cells[i].clear();
    }
}

void TextLayoutElement::AppendTextToCell(int index, TextElement *text)
{
    assert((index >= 0) && (index < 9));
    m_cells[index].push_back(text);
}

int TextLayoutElement::GetContentHeight() const
{
    int height = 0;
    for (int i = 0; i < 3; ++i) {
        height += this->GetRowHeight(i);
    }
    return height;
}

int TextLayoutElement::GetRowHeight(int row) const
{
    assert((row >= 0) && (row < 3));

    int height = 0;
    for (int i = 0; i < 3; ++i) {
        height = std::max(height, this->GetCellHeight(row * 3 + i));
    }
    return height;
}

int TextLayoutElement::GetColHeight(int col) const
{
    assert((col >= 0) && (col < 3));

    int height = 0;
    for (int i = 0; i < 3; ++i) {
        height += this->GetCellHeight(i * 3 + col);
    }
    return height;
}

int TextLayoutElement::GetCellHeight(int cell) const
{
    assert((cell >= 0) && (cell < 9));

    int columnHeight = 0;
    const ArrayOfTextElements *textElements = &m_cells[cell];
    ArrayOfTextElements::const_iterator iter;
    for (iter = textElements->begin(); iter != textElements->end(); ++iter) {
        if ((*iter)->HasContentBB()) {
            columnHeight += (*iter)->GetContentY2() - (*iter)->GetContentY1();
        }
    }
    return columnHeight;
}

bool TextLayoutElement::AdjustDrawingScaling(int width)
{
    bool scale = false;
    // For each row
    for (int i = 0; i < 3; ++i) {
        int rowWidth = 0;
        // For each column
        for (int j = 0; j < 3; ++j) {
            ArrayOfTextElements *textElements = &m_cells[i * 3 + j];
            int columnWidth = 0;
            // For each object
            for (TextElement *element : *textElements) {
                if (element->HasContentBB()) {
                    int iterWidth = element->GetContentX2() - element->GetContentX1();
                    columnWidth = std::max(columnWidth, iterWidth);
                }
            }
            rowWidth += columnWidth;
        }
        if (rowWidth && (rowWidth > width)) {
            m_drawingScalingPercent[i] = width * 100 / rowWidth;
            scale = true;
        }
    }
    return scale;
}

void TextLayoutElement::ResetDrawingScaling()
{
    for (int i = 0; i < 3; ++i) {
        m_drawingScalingPercent[i] = 100;
    }
}

bool TextLayoutElement::AdjustRunningElementYPos()
{
    // First adjust the content of each cell
    for (int i = 0; i < 9; ++i) {
        int cumulatedYRel = 0;
        ArrayOfTextElements *textElements = &m_cells[i];
        // For each object
        for (TextElement *element : *textElements) {
            if (!element->HasContentBB()) {
                continue;
            }
            int yShift = element->GetContentY2();
            element->SetDrawingYRel(cumulatedYRel - yShift);
            cumulatedYRel += (element->GetContentY1() - element->GetContentY2());
        }
    }

    int rowYRel = 0;
    // For each row
    for (int i = 0; i < 3; ++i) {
        int currentRowHeigt = this->GetRowHeight(i);
        // For each column
        for (int j = 0; j < 3; ++j) {
            int cell = i * 3 + j;
            int colYShift = 0;
            // middle row - it needs to be middle-aligned so calculate the colYShift accordingly
            if (i == 1) {
                colYShift = (currentRowHeigt - this->GetCellHeight(cell)) / 2;
            }
            // bottom row - it needs to be bottom-aligned so calculate the colYShift accordingly
            else if (i == 2) {
                colYShift = (currentRowHeigt - this->GetCellHeight(cell));
            }

            ArrayOfTextElements *textElements = &m_cells[cell];
            // For each object - adjust the yRel according to the rowYRel and the colYshift
            for (TextElement *element : *textElements) {
                if (!element->HasContentBB()) {
                    continue;
                }
                element->SetDrawingYRel(element->GetDrawingYRel() + rowYRel - colYShift);
            }
        }
        rowYRel -= currentRowHeigt;
    }

    return true;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

FunctorCode TextLayoutElement::Accept(Functor &functor)
{
    return functor.VisitTextLayoutElement(this);
}

FunctorCode TextLayoutElement::Accept(ConstFunctor &functor) const
{
    return functor.VisitTextLayoutElement(this);
}

FunctorCode TextLayoutElement::AcceptEnd(Functor &functor)
{
    return functor.VisitTextLayoutElementEnd(this);
}

FunctorCode TextLayoutElement::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitTextLayoutElementEnd(this);
}

} // namespace vrv

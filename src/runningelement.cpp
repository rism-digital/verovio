/////////////////////////////////////////////////////////////////////////////
// Name:        runningelement.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "runningelement.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "fig.h"
#include "functorparams.h"
#include "page.h"
#include "rend.h"
#include "staff.h"
#include "text.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// RunningElement
//----------------------------------------------------------------------------

RunningElement::RunningElement() : Object("re"), ObjectListInterface(), AttHorizontalAlign(), AttTyped()
{
    RegisterAttClass(ATT_HORIZONTALALIGN);
    RegisterAttClass(ATT_TYPED);

    Reset();
}

RunningElement::RunningElement(std::string classid) : Object(classid), AttHorizontalAlign(), AttTyped()
{
    RegisterAttClass(ATT_HORIZONTALALIGN);
    RegisterAttClass(ATT_TYPED);

    Reset();
}

RunningElement::~RunningElement()
{
}

void RunningElement::Reset()
{
    Object::Reset();
    ResetHorizontalAlign();
    ResetTyped();
    
    m_drawingPage = NULL;
    m_drawingYRel = 0;
    
    int i;
    for (i = 0; i < 3; i++) {
        m_drawingScalingPercent[i] = 100;
    }
}

void RunningElement::AddChild(Object *child)
{
    if (child->IsTextElement()) {
        assert(dynamic_cast<TextElement *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }
    
    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}
    
void RunningElement::FilterList(ListOfObjects *childList)
{
    ListOfObjects::iterator iter = childList->begin();

    while (iter != childList->end()) {
        // remove nested rend elements
        if ((*iter)->Is(REND)) {
            if ((*iter)->GetFirstParent(REND)) {
                iter = childList->erase(iter);
                continue;
            }
        }
        // Also remove anything that is not a fig
        else if (!(*iter)->Is(FIG)) {
            iter = childList->erase(iter);
            continue;
        }
        iter++;
    }
    
    int i;
    for (i = 0; i < 9; i++) {
        m_positionnedObjects[i].clear();
    }
    for (i = 0; i < 3; i++) {
        m_drawingScalingPercent[i] = 100;
    }
    
    for (iter = childList->begin(); iter != childList->end(); iter++) {
        int pos = 0;
        AreaPosInterface *interface = dynamic_cast<AreaPosInterface *>(*iter);
        assert(interface);
        pos = this->GetAlignmentPos(interface->GetHalign(), interface->GetValign());
        TextElement *text = dynamic_cast<TextElement *>(*iter);
        assert(text);
        m_positionnedObjects[pos].push_back(text);
    }
}
    
int RunningElement::GetDrawingX() const
{
    if (!m_drawingPage) return 0;
    
    /*
    if (this->GetHalign() == HORIZONTALALIGNMENT_left) {
        return 0;
    }
    else if (this->GetHalign() == HORIZONTALALIGNMENT_center) {
        return m_drawingPage->GetContentWidth() / 2;
    }
    else if (this->GetHalign() == HORIZONTALALIGNMENT_right) {
        return m_drawingPage->GetContentWidth();
    }
    */
    
    return 0;
}

int RunningElement::GetDrawingY() const
{
    m_cachedDrawingY = 0;
    return m_drawingYRel;
}

void RunningElement::SetDrawingYRel(int drawingYRel)
{
    ResetCachedDrawingY();
    m_drawingYRel = drawingYRel;
}
    
int RunningElement::GetWidth() const
{
    if (!m_drawingPage) return 0;
    
    return m_drawingPage->GetContentWidth();
}
    
void RunningElement::SetDrawingPage(Page *page)
{
    ResetList(this);
    
    ResetCachedDrawingX();
    m_drawingPage = page;
}

int RunningElement::CalcTotalHeight()
{
    int height = 0;
    int i, j;
    for (i = 0; i < 3; i++) {
        int columnHeight = 0;
        for (j = 0; j < 3; j++) {
            ArrayOfTextElements *textElements = &m_positionnedObjects[i + j * 3];
            ArrayOfTextElements::iterator iter;
            for (iter = textElements->begin(); iter != textElements->end(); iter++) {
                if ((*iter)->HasContentBB()) {
                    columnHeight += (*iter)->GetContentY2() - (*iter)->GetContentY1();
                }
            }
        }
        height = std::max(height, columnHeight);
    }
    return height;
}
    
bool RunningElement::AdjustDrawingScaling(int width)
{
    int i, j;
    bool scale = false;
    // For each row
    for (i = 0; i < 3; i++) {
        int rowWidth = 0;
        // For each column
        for (j = 0; j < 3; j++) {
            ArrayOfTextElements *textElements = &m_positionnedObjects[i * 3 + j ];
            ArrayOfTextElements::iterator iter;
            int columnWidth = 0;
            // For each object
            for (iter = textElements->begin(); iter != textElements->end(); iter++) {
                if ((*iter)->HasContentBB()) {
                    int iterWidth = (*iter)->GetContentX2() - (*iter)->GetContentX1();
                    columnWidth = std::max(columnWidth, iterWidth);
                }
            }
            rowWidth += columnWidth;
        }
        if (rowWidth > width) {
            m_drawingScalingPercent[i] = width * 100 / rowWidth;
            scale = true;
        }
    }
    return scale;
}
    
bool RunningElement::AdjustYPos()
{
    int i;
    for (i = 0; i < 9; i++) {
        ArrayOfTextElements *textElements = &m_positionnedObjects[i];
        ArrayOfTextElements::iterator iter;
        int cumulatedYRel = 0;
        for (iter = textElements->begin(); iter != textElements->end(); iter++) {
            if ((*iter)->HasContentBB()) {
                int yShift = (*iter)->GetContentY2();
                if ((*iter)->Is(REND)) {
                    Rend *rend = dynamic_cast<Rend *>(*iter);
                    assert(rend);
                    rend->SetDrawingYRel(cumulatedYRel - yShift);

                }
                else {
                    Fig *fig = dynamic_cast<Fig *>(*iter);
                    assert(fig);
                    fig->SetDrawingYRel(cumulatedYRel - yShift);
                }
                cumulatedYRel += ((*iter)->GetContentY1() - (*iter)->GetContentY2());
            }
        }
    }
    return true;
}
        
int RunningElement::GetAlignmentPos(data_HORIZONTALALIGNMENT h, data_VERTICALALIGNMENT v)
{
    int pos = 0;
    switch (h) {
        case (HORIZONTALALIGNMENT_left) : break;
        case (HORIZONTALALIGNMENT_center) : pos += POSITION_CENTER; break;
        case (HORIZONTALALIGNMENT_right) : pos += POSITION_RIGHT; break;
        default:
            pos += POSITION_LEFT; break;
    }
    switch (v) {
        case (VERTICALALIGNMENT_top) : break;
        case (VERTICALALIGNMENT_middle) : pos += POSITION_MIDDLE; break;
        case (VERTICALALIGNMENT_bottom) : pos += POSITION_BOTTOM; break;
        default:
            pos += POSITION_MIDDLE; break;
    }
    return pos;
}
    
//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

int RunningElement::AlignVertically(FunctorParams *functorParams)
{
    AlignVerticallyParams *params = dynamic_cast<AlignVerticallyParams *>(functorParams);
    assert(params);
    
    params->m_pageWidth = this->GetWidth();

    return FUNCTOR_CONTINUE;
}
    
} // namespace vrv

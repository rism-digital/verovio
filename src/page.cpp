/////////////////////////////////////////////////////////////////////////////
// Name:        page.cpp
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "page.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "attcomparison.h"
#include "bboxdevicecontext.h"
#include "doc.h"
#include "functorparams.h"
#include "system.h"
#include "view.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Page
//----------------------------------------------------------------------------

Page::Page() : Object("page-")
{
    Reset();
}

Page::~Page()
{
}

void Page::Reset()
{
    Object::Reset();

    m_drawingScoreDef.Reset();
    m_layoutDone = false;
    this->ResetUuid();

    // by default we have no values and use the document ones
    m_pageHeight = -1;
    m_pageWidth = -1;
    m_pageLeftMar = 0;
    m_pageRightMar = 0;
    m_pageTopMar = 0;
}

void Page::AddChild(Object *child)
{
    if (child->Is() == SYSTEM) {
        assert(dynamic_cast<System *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

void Page::LayOut(bool force)
{
    if (m_layoutDone && !force) {
        return;
    }

    this->LayOutHorizontally();
    this->JustifyHorizontally();
    this->LayOutVertically();

    m_layoutDone = true;
}

void Page::LayOutHorizontally()
{
    Doc *doc = dynamic_cast<Doc *>(m_parent);
    assert(doc);

    // Doc::SetDrawingPage should have been called before
    // Make sure we have the correct page
    assert(this == doc->GetDrawingPage());

    // Reset the horizontal alignment
    Functor resetHorizontalAlignment(&Object::ResetHorizontalAlignment);
    this->Process(&resetHorizontalAlignment, NULL);

    // Reset the vertical alignment
    Functor resetVerticalAlignment(&Object::ResetVerticalAlignment);
    this->Process(&resetVerticalAlignment, NULL);

    // Align the content of the page using measure aligners
    // After this:
    // - each LayerElement object will have its Alignment pointer initialized
    Functor alignHorizontally(&Object::AlignHorizontally);
    AlignHorizontallyParams alignHorizontallyParams(&alignHorizontally);
    Functor alignHorizontallyEnd(&Object::AlignHorizontallyEnd);
    this->Process(&alignHorizontally, &alignHorizontallyParams, &alignHorizontallyEnd);

    // Unless duration-based spacing is disabled, set the X position of each Alignment.
    // Does non-linear spacing based on the duration space between two Alignment objects.
    if (!doc->GetEvenSpacing()) {
        int longestActualDur = DUR_4;
        // Get the longest duration in the piece
        AttDurExtreme durExtremeComparison(LONGEST);
        Object *longestDur = this->FindChildExtremeByAttComparison(&durExtremeComparison);
        if (longestDur) {
            DurationInterface *interface = longestDur->GetDurationInterface();
            assert(interface);
            longestActualDur = interface->GetActualDur();
            // LogDebug("Longest duration is DUR_* code %d", longestActualDur);
        }

        Functor setAlignmentX(&Object::SetAlignmentXPos);
        SetAlignmentXPosParams setAlignmentXPosParams(doc, &setAlignmentX);
        setAlignmentXPosParams.m_longestActualDur = longestActualDur;
        this->Process(&setAlignmentX, &setAlignmentXPosParams);
    }

    // Render it for filling the bounding box
    View view;
    BBoxDeviceContext bBoxDC(&view, 0, 0, BBOX_HORIZONTAL_ONLY);
    view.SetDoc(doc);
    // Do not do the layout in this view - otherwise we will loop...
    view.SetPage(this->GetIdx(), false);
    view.DrawCurrentPage(&bBoxDC, false);

    // Adjust the X shift of the Alignment looking at the bounding boxes
    // Look at each LayerElement and change the m_xShift if the bounding box is overlapping
    SetBoundingBoxGraceXShiftParams setBoundingBoxGraceXShiftParams(doc);
    Functor setBoundingBoxGraceXShift(&Object::SetBoundingBoxGraceXShift);
    this->Process(&setBoundingBoxGraceXShift, &setBoundingBoxGraceXShiftParams);

    // Integrate the X bounding box shift of the elements
    // Once the m_xShift have been calculated, move all positions accordingly
    Functor integrateBoundingBoxGraceXShift(&Object::IntegrateBoundingBoxGraceXShift);
    IntegrateBoundingBoxGraceXShiftParams integrateBoundingBoxGraceXShiftParams(&integrateBoundingBoxGraceXShift);
    this->Process(&integrateBoundingBoxGraceXShift, &integrateBoundingBoxGraceXShiftParams);

    // Adjust the X shift of the Alignment looking at the bounding boxes
    // Look at each LayerElement and change the m_xShift if the bounding box is overlapping
    Functor setBoundingBoxXShift(&Object::SetBoundingBoxXShift);
    Functor setBoundingBoxXShiftEnd(&Object::SetBoundingBoxXShiftEnd);
    SetBoundingBoxXShiftParams setBoundingBoxXShiftParams(doc, &setBoundingBoxXShift, &setBoundingBoxXShiftEnd);
    this->Process(&setBoundingBoxXShift, &setBoundingBoxXShiftParams, &setBoundingBoxXShiftEnd);

    // Integrate the X bounding box shift of the elements
    // Once the m_xShift have been calculated, move all positions accordingly
    Functor integrateBoundingBoxXShift(&Object::IntegrateBoundingBoxXShift);
    IntegrateBoundingBoxXShiftParams integrateBoundingBoxXShiftParams(doc, &integrateBoundingBoxXShift);
    this->Process(&integrateBoundingBoxXShift, &integrateBoundingBoxXShiftParams);

    // Adjust measure X position
    AlignMeasuresParams alignMeasuresParams;
    Functor alignMeasures(&Object::AlignMeasures);
    Functor alignMeasuresEnd(&Object::AlignMeasuresEnd);
    this->Process(&alignMeasures, &alignMeasuresParams, &alignMeasuresEnd);
}

void Page::LayOutVertically()
{
    Doc *doc = dynamic_cast<Doc *>(m_parent);
    assert(doc);

    // Doc::SetDrawingPage should have been called before
    // Make sure we have the correct page
    assert(this == doc->GetDrawingPage());

    // Reset the vertical alignment
    Functor resetVerticalAlignment(&Object::ResetVerticalAlignment);
    this->Process(&resetVerticalAlignment, NULL);

    // Align the content of the page using system aligners
    // After this:
    // - each Staff object will then have its StaffAlignment pointer initialized
    AlignVerticallyParams alignVerticallyParams(doc);
    Functor alignVertically(&Object::AlignVertically);
    this->Process(&alignVertically, &alignVerticallyParams);

    // Render it for filling the bounding box
    View view;
    BBoxDeviceContext bBoxDC(&view, 0, 0);
    view.SetDoc(doc);
    // Do not do the layout in this view - otherwise we will loop...
    view.SetPage(this->GetIdx(), false);
    view.DrawCurrentPage(&bBoxDC, false);

    // Adjust the position of outside articulations
    AdjustArticParams adjustArticParams(doc);
    Functor adjustArtic(&Object::AdjustArtic);
    this->Process(&adjustArtic, &adjustArticParams);

    // Adjust the position of outside articulations with slurs end and start positions
    AdjustArticWithSlursParams adjustArticWithSlursParams(doc);
    Functor adjustArticWithSlurs(&Object::AdjustArticWithSlurs);
    this->Process(&adjustArticWithSlurs, &adjustArticWithSlursParams);

    // Fill the arrays of bounding boxes (above and below) for each staff alignment for which the box overflows.
    SetOverflowBBoxesParams setOverflowBBoxesParams(doc);
    Functor setOverflowBBoxes(&Object::SetOverflowBBoxes);
    Functor setOverflowBBoxesEnd(&Object::SetOverflowBBoxesEnd);
    this->Process(&setOverflowBBoxes, &setOverflowBBoxesParams, &setOverflowBBoxesEnd);

    // Adjust the positioners of floationg elements (slurs, hairpin, dynam, etc)
    Functor adjustFloatingPostioners(&Object::AdjustFloatingPostioners);
    AdjustFloatingPostionersParams adjustFloatingPostionersParams(doc, &adjustFloatingPostioners);
    this->Process(&adjustFloatingPostioners, &adjustFloatingPostionersParams);

    // Calculate the overlap of the staff aligmnents by looking at the overflow bounding boxes params.clear();
    Functor calcStaffOverlap(&Object::CalcStaffOverlap);
    CalcStaffOverlapParams calcStaffOverlapParams(&calcStaffOverlap);
    this->Process(&calcStaffOverlap, &calcStaffOverlapParams);

    // Set the Y position of each StaffAlignment
    // Adjust the Y shift to make sure there is a minimal space (staffMargin) between each staff
    Functor setAlignmentY(&Object::SetAligmentYPos);
    SetAligmentYPosParams setAligmentYPosParams(doc, &setAlignmentY);
    this->Process(&setAlignmentY, &setAligmentYPosParams);

    // Integrate the Y shift of the staves
    // Once the m_yShift have been calculated, move all positions accordingly
    Functor integrateBoundingBoxYShift(&Object::IntegrateBoundingBoxYShift);
    IntegrateBoundingBoxYShiftParams integrateBoundingBoxYShiftParams(&integrateBoundingBoxYShift);
    this->Process(&integrateBoundingBoxYShift, &integrateBoundingBoxYShiftParams);

    // Adjust system Y position
    AlignSystemsParams alignSystemsParams;
    alignSystemsParams.m_shift = doc->m_drawingPageHeight - doc->m_drawingPageTopMar;
    alignSystemsParams.m_systemMargin = (doc->GetSpacingSystem()) * doc->GetDrawingUnit(100);
    Functor alignSystems(&Object::AlignSystems);
    this->Process(&alignSystems, &alignSystemsParams);
}

void Page::JustifyHorizontally()
{
    Doc *doc = dynamic_cast<Doc *>(m_parent);
    assert(doc);

    if (!doc->GetJustificationX()) {
        return;
    }

    // Doc::SetDrawingPage should have been called before
    // Make sure we have the correct page
    assert(this == doc->GetDrawingPage());

    // Justify X position
    Functor justifyX(&Object::JustifyX);
    JustifyXParams justifyXParams(&justifyX);
    justifyXParams.m_systemFullWidth = doc->m_drawingPageWidth - doc->m_drawingPageLeftMar - doc->m_drawingPageRightMar;
    this->Process(&justifyX, &justifyXParams);
}

int Page::GetContentHeight() const
{
    Doc *doc = dynamic_cast<Doc *>(m_parent);
    assert(doc);

    // Doc::SetDrawingPage should have been called before
    // Make sure we have the correct page
    assert(this == doc->GetDrawingPage());

    System *last = dynamic_cast<System *>(m_children.back());
    assert(last);
    return doc->m_drawingPageHeight - doc->m_drawingPageTopMar - last->m_drawingYRel + last->GetHeight();
}

int Page::GetContentWidth() const
{
    Doc *doc = dynamic_cast<Doc *>(m_parent);
    assert(doc);
    // in non debug
    if (!doc) return 0;

    // Doc::SetDrawingPage should have been called before
    // Make sure we have the correct page
    assert(this == doc->GetDrawingPage());

    System *first = dynamic_cast<System *>(m_children.front());
    assert(first);

    // For avoiding unused variable warning in non debug mode
    doc = NULL;

    // we include the left margin and the right margin
    return first->m_drawingTotalWidth + first->m_systemLeftMar + first->m_systemRightMar;
}

} // namespace vrv

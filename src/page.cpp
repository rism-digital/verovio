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

void Page::AddSystem(System *system)
{
    system->SetParent(this);
    m_children.push_back(system);
    Modify();
}

void Page::LayOut(bool force)
{
    if (m_layoutDone && !force) {
        return;
    }

    this->LayOutHorizontally();
    this->LayOutVertically();
    this->JustifyHorizontally();

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
    AlignHorizontallyParams alignHorizontallyParams;
    Functor alignHorizontally(&Object::AlignHorizontally);
    Functor alignHorizontallyEnd(&Object::AlignHorizontallyEnd);
    // Pass it for redirection
    alignHorizontallyParams.m_functor = &alignHorizontally;
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

        SetAlignmentXPosParams setAlignmentXPosParams;
        setAlignmentXPosParams.m_longestActualDur = longestActualDur;
        setAlignmentXPosParams.m_doc = doc;
        Functor setAlignmentX(&Object::SetAlignmentXPos);
        // Pass it for redirection
        setAlignmentXPosParams.m_functor = &setAlignmentX;
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
    SetBoundingBoxGraceXShiftParams setBoundingBoxGraceXShiftParams;
    setBoundingBoxGraceXShiftParams.m_doc = doc;
    Functor setBoundingBoxGraceXShift(&Object::SetBoundingBoxGraceXShift);
    this->Process(&setBoundingBoxGraceXShift, &setBoundingBoxGraceXShiftParams);

    // Integrate the X bounding box shift of the elements
    // Once the m_xShift have been calculated, move all positions accordingly
    IntegrateBoundingBoxGraceXShiftParams integrateBoundingBoxGraceXShiftParams;
    Functor integrateBoundingBoxGraceXShift(&Object::IntegrateBoundingBoxGraceXShift);
    // Pass it for redirection
    integrateBoundingBoxGraceXShiftParams.m_functor = &integrateBoundingBoxGraceXShift;
    this->Process(&integrateBoundingBoxGraceXShift, &integrateBoundingBoxGraceXShiftParams);

    // Adjust the X shift of the Alignment looking at the bounding boxes
    // Look at each LayerElement and change the m_xShift if the bounding box is overlapping
    SetBoundingBoxXShiftParams setBoundingBoxXShiftParams;
    setBoundingBoxXShiftParams.m_doc = doc;
    Functor setBoundingBoxXShift(&Object::SetBoundingBoxXShift);
    Functor setBoundingBoxXShiftEnd(&Object::SetBoundingBoxXShiftEnd);
    // Pass them for redirection
    setBoundingBoxXShiftParams.m_functor = &setBoundingBoxXShift;
    setBoundingBoxXShiftParams.m_functorEnd = &setBoundingBoxXShiftEnd;
    this->Process(&setBoundingBoxXShift, &setBoundingBoxXShiftParams, &setBoundingBoxXShiftEnd);

    // Integrate the X bounding box shift of the elements
    // Once the m_xShift have been calculated, move all positions accordingly
    IntegrateBoundingBoxXShiftParams integrateBoundingBoxXShiftParams;
    integrateBoundingBoxXShiftParams.m_doc = doc;
    Functor integrateBoundingBoxXShift(&Object::IntegrateBoundingBoxXShift);
    // Pass it for redirection
    integrateBoundingBoxXShiftParams.m_functor = &integrateBoundingBoxXShift;
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
    AlignVerticallyParams alignVerticallyParams;
    alignVerticallyParams.m_doc = doc;
    Functor alignVertically(&Object::AlignVertically);
    this->Process(&alignVertically, &alignVerticallyParams);

    // Render it for filling the bounding box
    View view;
    BBoxDeviceContext bBoxDC(&view, 0, 0);
    view.SetDoc(doc);
    // Do not do the layout in this view - otherwise we will loop...
    view.SetPage(this->GetIdx(), false);
    view.DrawCurrentPage(&bBoxDC, false);

    // Fill the arrays of bounding boxes (above and below) for each staff alignment for which the box overflows.
    SetOverflowBBoxesParams setOverflowBBoxesParams;
    setOverflowBBoxesParams.m_doc = doc;
    Functor setOverflowBBoxes(&Object::SetOverflowBBoxes);
    this->Process(&setOverflowBBoxes, &setOverflowBBoxesParams);

    // Adjust the positioners of floationg elements (slurs, hairpin, dynam, etc)
    AdjustFloatingPostionersParams adjustFloatingPostionersParams;
    adjustFloatingPostionersParams.m_doc = doc;
    Functor adjustFloatingPostioners(&Object::AdjustFloatingPostioners);
    // Pass it for redirection
    adjustFloatingPostionersParams.m_functor = &adjustFloatingPostioners;
    this->Process(&adjustFloatingPostioners, &adjustFloatingPostionersParams);

    // Calculate the overlap of the staff aligmnents by looking at the overflow bounding boxes params.clear();
    CalcStaffOverlapParams calcStaffOverlapParams;
    Functor calcStaffOverlap(&Object::CalcStaffOverlap);
    // Pass it for redirection
    calcStaffOverlapParams.m_functor = &calcStaffOverlap;
    this->Process(&calcStaffOverlap, &calcStaffOverlapParams);

    // Set the Y position of each StaffAlignment
    // Adjust the Y shift to make sure there is a minimal space (staffMargin) between each staff
    SetAligmentYPosParams setAligmentYPosParams;
    setAligmentYPosParams.m_doc = doc;
    Functor setAlignmentY(&Object::SetAligmentYPos);
    // Pass it for redirection
    setAligmentYPosParams.m_functor = &setAlignmentY;
    this->Process(&setAlignmentY, &setAligmentYPosParams);

    // Integrate the Y shift of the staves
    // Once the m_yShift have been calculated, move all positions accordingly
    IntegrateBoundingBoxYShiftParams integrateBoundingBoxYShiftParams;
    Functor integrateBoundingBoxYShift(&Object::IntegrateBoundingBoxYShift);
    // Pass it for redirection
    integrateBoundingBoxYShiftParams.m_functor = &integrateBoundingBoxYShift;
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
    JustifyXParams justifyXParams;
    justifyXParams.m_systemFullWidth = doc->m_drawingPageWidth - doc->m_drawingPageLeftMar - doc->m_drawingPageRightMar;
    Functor justifyX(&Object::JustifyX);
    // Pass it for redirection
    justifyXParams.m_functor = &justifyX;
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

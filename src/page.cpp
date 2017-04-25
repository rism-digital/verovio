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
    if (child->Is(SYSTEM)) {
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

void Page::LayOutTranscription(bool force)
{
    if (m_layoutDone && !force) {
        return;
    }
    
    Doc *doc = dynamic_cast<Doc *>(GetParent());
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
    Functor alignHorizontallyEnd(&Object::AlignHorizontallyEnd);
    AlignHorizontallyParams alignHorizontallyParams(&alignHorizontally);
    this->Process(&alignHorizontally, &alignHorizontallyParams, &alignHorizontallyEnd);
    
    // Align the content of the page using system aligners
    // After this:
    // - each Staff object will then have its StaffAlignment pointer initialized
    Functor alignVertically(&Object::AlignVertically);
    Functor alignVerticallyEnd(&Object::AlignVerticallyEnd);
    AlignVerticallyParams alignVerticallyParams(doc, &alignVerticallyEnd);
    this->Process(&alignVertically, &alignVerticallyParams, &alignVerticallyEnd);
    
    // Set the pitch / pos alignement
    // Once View::CalculateRestPosY will be move to Staff we will not need to pass a view anymore
    View view;
    view.SetDoc(doc);
    SetAlignmentPitchPosParams setAlignmentPitchPosParams(doc, &view);
    Functor setAlignmentPitchPos(&Object::SetAlignmentPitchPos);
    this->Process(&setAlignmentPitchPos, &setAlignmentPitchPosParams);
    
    CalcStemParams calcStemParams(doc);
    Functor calcStem(&Object::CalcStem);
    this->Process(&calcStem, &calcStemParams);
    
    FunctorDocParams calcChordNoteHeadsParams(doc);
    Functor calcChordNoteHeads(&Object::CalcChordNoteHeads);
    this->Process(&calcChordNoteHeads, &calcChordNoteHeadsParams);
    
    CalcDotsParams calcDotsParams(doc);
    Functor calcDots(&Object::CalcDots);
    this->Process(&calcDots, &calcDotsParams);
    
    FunctorDocParams calcLegerLinesParams(doc);
    Functor calcLedgerLines(&Object::CalcLedgerLines);
    this->Process(&calcLedgerLines, &calcLegerLinesParams);
    
    m_layoutDone = true;
}
    
void Page::LayOutHorizontally()
{
    Doc *doc = dynamic_cast<Doc *>(GetParent());
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
    Functor alignHorizontallyEnd(&Object::AlignHorizontallyEnd);
    AlignHorizontallyParams alignHorizontallyParams(&alignHorizontally);
    this->Process(&alignHorizontally, &alignHorizontallyParams, &alignHorizontallyEnd);

    // Align the content of the page using system aligners
    // After this:
    // - each Staff object will then have its StaffAlignment pointer initialized
    Functor alignVertically(&Object::AlignVertically);
    Functor alignVerticallyEnd(&Object::AlignVerticallyEnd);
    AlignVerticallyParams alignVerticallyParams(doc, &alignVerticallyEnd);
    this->Process(&alignVertically, &alignVerticallyParams, &alignVerticallyEnd);

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

    // Set the pitch / pos alignement
    // Once View::CalculateRestPosY will be move to Staff we will not need to pass a view anymore
    View view;
    view.SetDoc(doc);
    SetAlignmentPitchPosParams setAlignmentPitchPosParams(doc, &view);
    Functor setAlignmentPitchPos(&Object::SetAlignmentPitchPos);
    this->Process(&setAlignmentPitchPos, &setAlignmentPitchPosParams);

    CalcStemParams calcStemParams(doc);
    Functor calcStem(&Object::CalcStem);
    this->Process(&calcStem, &calcStemParams);

    FunctorDocParams calcChordNoteHeadsParams(doc);
    Functor calcChordNoteHeads(&Object::CalcChordNoteHeads);
    this->Process(&calcChordNoteHeads, &calcChordNoteHeadsParams);

    CalcDotsParams calcDotsParams(doc);
    Functor calcDots(&Object::CalcDots);
    this->Process(&calcDots, &calcDotsParams);

    // Render it for filling the bounding box
    BBoxDeviceContext bBoxDC(&view, 0, 0, BBOX_HORIZONTAL_ONLY);
    // Do not do the layout in this view - otherwise we will loop...
    view.SetPage(this->GetIdx(), false);
    view.DrawCurrentPage(&bBoxDC, false);

    // Adjust the X position of the accidentals, including in chords
    Functor adjustAccidX(&Object::AdjustAccidX);
    AdjustAccidXParams adjustAccidXParams(doc, &adjustAccidX);
    this->Process(&adjustAccidX, &adjustAccidXParams);

    // Adjust the X shift of the Alignment looking at the bounding boxes
    // Look at each LayerElement and change the m_xShift if the bounding box is overlapping
    Functor adjustXPos(&Object::AdjustXPos);
    Functor adjustXPosEnd(&Object::AdjustXPosEnd);
    AdjustXPosParams adjustXPosParams(doc, &adjustXPos, &adjustXPosEnd, doc->m_scoreDef.GetStaffNs());
    this->Process(&adjustXPos, &adjustXPosParams, &adjustXPosEnd);

    // Adjust the X shift of the Alignment looking at the bounding boxes
    // Look at each LayerElement and change the m_xShift if the bounding box is overlapping
    Functor adjustGraceXPos(&Object::AdjustGraceXPos);
    Functor adjustGraceXPosEnd(&Object::AdjustGraceXPosEnd);
    AdjustGraceXPosParams adjustGraceXPosParams(
        doc, &adjustGraceXPos, &adjustGraceXPosEnd, doc->m_scoreDef.GetStaffNs());
    this->Process(&adjustGraceXPos, &adjustGraceXPosParams, &adjustGraceXPosEnd);

    // We need to populate processing lists for processing the document by Layer (for matching @tie) and
    // by Verse (for matching syllable connectors)
    PrepareProcessingListsParams prepareProcessingListsParams;
    Functor prepareProcessingLists(&Object::PrepareProcessingLists);
    this->Process(&prepareProcessingLists, &prepareProcessingListsParams);

    this->AdjustSylSpacingByVerse(prepareProcessingListsParams, doc);

    // Adjust measure X position
    AlignMeasuresParams alignMeasuresParams;
    Functor alignMeasures(&Object::AlignMeasures);
    Functor alignMeasuresEnd(&Object::AlignMeasuresEnd);
    this->Process(&alignMeasures, &alignMeasuresParams, &alignMeasuresEnd);
}

void Page::LayOutVertically()
{
    Doc *doc = dynamic_cast<Doc *>(GetParent());
    assert(doc);

    // Doc::SetDrawingPage should have been called before
    // Make sure we have the correct page
    assert(this == doc->GetDrawingPage());

    // Reset the vertical alignment
    Functor resetVerticalAlignment(&Object::ResetVerticalAlignment);
    this->Process(&resetVerticalAlignment, NULL);

    FunctorDocParams calcLegerLinesParams(doc);
    Functor calcLedgerLines(&Object::CalcLedgerLines);
    this->Process(&calcLedgerLines, &calcLegerLinesParams);

    // Align the content of the page using system aligners
    // After this:
    // - each Staff object will then have its StaffAlignment pointer initialized
    Functor alignVertically(&Object::AlignVertically);
    Functor alignVerticallyEnd(&Object::AlignVerticallyEnd);
    AlignVerticallyParams alignVerticallyParams(doc, &alignVerticallyEnd);
    this->Process(&alignVertically, &alignVerticallyParams, &alignVerticallyEnd);

    // Adjust the position of outside articulations
    FunctorDocParams adjustArticParams(doc);
    Functor adjustArtic(&Object::AdjustArtic);
    this->Process(&adjustArtic, &adjustArticParams);

    // Render it for filling the bounding box
    View view;
    BBoxDeviceContext bBoxDC(&view, 0, 0);
    view.SetDoc(doc);
    // Do not do the layout in this view - otherwise we will loop...
    view.SetPage(this->GetIdx(), false);
    view.DrawCurrentPage(&bBoxDC, false);

    // Adjust the position of outside articulations with slurs end and start positions
    FunctorDocParams adjustArticWithSlursParams(doc);
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
    Functor adjustYPos(&Object::AdjustYPos);
    AdjustYPosParams adjustYPosParams(doc, &adjustYPos);
    this->Process(&adjustYPos, &adjustYPosParams);

    // Adjust system Y position
    AlignSystemsParams alignSystemsParams;
    alignSystemsParams.m_shift = doc->m_drawingPageHeight - doc->m_drawingPageTopMar;
    alignSystemsParams.m_systemMargin = (doc->GetSpacingSystem()) * doc->GetDrawingUnit(100);
    Functor alignSystems(&Object::AlignSystems);
    this->Process(&alignSystems, &alignSystemsParams);
}

void Page::JustifyHorizontally()
{
    Doc *doc = dynamic_cast<Doc *>(GetParent());
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

void Page::LayOutPitchPos()
{
    Doc *doc = dynamic_cast<Doc *>(GetParent());
    assert(doc);

    // Doc::SetDrawingPage should have been called before
    // Make sure we have the correct page
    assert(this == doc->GetDrawingPage());

    // Set the pitch / pos alignement
    // Once View::CalculateRestPosY will be move to Staff we will not need to pass a view anymore
    View view;
    view.SetDoc(doc);
    SetAlignmentPitchPosParams setAlignmentPitchPosParams(doc, &view);
    Functor setAlignmentPitchPos(&Object::SetAlignmentPitchPos);
    this->Process(&setAlignmentPitchPos, &setAlignmentPitchPosParams);

    CalcStemParams calcStemParams(doc);
    Functor calcStem(&Object::CalcStem);
    this->Process(&calcStem, &calcStemParams);
}

int Page::GetContentHeight() const
{
    Doc *doc = dynamic_cast<Doc *>(GetParent());
    assert(doc);

    // Doc::SetDrawingPage should have been called before
    // Make sure we have the correct page
    assert(this == doc->GetDrawingPage());

    System *last = dynamic_cast<System *>(m_children.back());
    assert(last);
    return doc->m_drawingPageHeight - doc->m_drawingPageTopMar - last->GetDrawingYRel() + last->GetHeight();
}

int Page::GetContentWidth() const
{
    Doc *doc = dynamic_cast<Doc *>(GetParent());
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

void Page::AdjustSylSpacingByVerse(PrepareProcessingListsParams &listsParams, Doc *doc)
{
    IntTree_t::iterator staves;
    IntTree_t::iterator layers;
    IntTree_t::iterator verses;

    if (listsParams.m_verseTree.child.empty()) return;

    std::vector<AttComparison *> filters;

    // Same for the lyrics, but Verse by Verse since Syl are TimeSpanningInterface elements for handling connectors
    for (staves = listsParams.m_verseTree.child.begin(); staves != listsParams.m_verseTree.child.end(); ++staves) {
        for (layers = staves->second.child.begin(); layers != staves->second.child.end(); ++layers) {
            for (verses = layers->second.child.begin(); verses != layers->second.child.end(); ++verses) {
                // Create ad comparison object for each type / @n
                AttCommonNComparison matchStaff(STAFF, staves->first);
                AttCommonNComparison matchLayer(LAYER, layers->first);
                AttCommonNComparison matchVerse(VERSE, verses->first);
                filters = { &matchStaff, &matchLayer, &matchVerse };

                // The first pass sets m_drawingFirstNote and m_drawingLastNote for each syl
                // m_drawingLastNote is set only if the syl has a forward connector
                AdjustSylSpacingParams adjustSylSpacingParams(doc);
                Functor adjustSylSpacing(&Object::AdjustSylSpacing);
                Functor adjustSylSpacingEnd(&Object::AdjustSylSpacingEnd);
                this->Process(&adjustSylSpacing, &adjustSylSpacingParams, &adjustSylSpacingEnd, &filters);
            }
        }
    }
}

} // namespace vrv

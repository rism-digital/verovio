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

#include "bboxdevicecontext.h"
#include "comparison.h"
#include "doc.h"
#include "functorparams.h"
#include "pages.h"
#include "pgfoot.h"
#include "pgfoot2.h"
#include "pghead.h"
#include "pghead2.h"
#include "staff.h"
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

Page::~Page() {}

void Page::Reset()
{
    Object::Reset();

    m_drawingScoreDef.Reset();
    m_layoutDone = false;
    this->ResetUuid();

    // by default we have no values and use the document ones
    m_pageHeight = -1;
    m_pageWidth = -1;
    m_pageMarginBottom = 0;
    m_pageMarginLeft = 0;
    m_pageMarginRight = 0;
    m_pageMarginTop = 0;
    m_PPUFactor = 1.0;

    m_drawingJustifiableHeight = 0;
    m_justificationSum = 0.;
}

bool Page::IsSupportedChild(Object *child)
{
    if (child->Is(SYSTEM)) {
        assert(dynamic_cast<System *>(child));
    }
    else {
        return false;
    }
    return true;
}

RunningElement *Page::GetHeader() const
{
    Doc *doc = dynamic_cast<Doc *>(this->GetFirstAncestor(DOC));
    if (!doc || (doc->GetOptions()->m_header.GetValue() == HEADER_none)) {
        return NULL;
    }

    Pages *pages = doc->GetPages();
    assert(pages);

    // first page or use the pgHeader for all pages?
    if ((pages->GetFirst() == this) || (doc->GetOptions()->m_usePgHeaderForAll.GetValue())) {
        return doc->m_mdivScoreDef.GetPgHead();
    }
    else {
        return doc->m_mdivScoreDef.GetPgHead2();
    }
}

RunningElement *Page::GetFooter() const
{
    Doc *doc = dynamic_cast<Doc *>(this->GetFirstAncestor(DOC));
    if (!doc || (doc->GetOptions()->m_footer.GetValue() == FOOTER_none)) {
        return NULL;
    }

    Pages *pages = doc->GetPages();
    assert(pages);

    // first page or use the pgFooter for all pages?
    if ((pages->GetFirst() == this) || (doc->GetOptions()->m_usePgFooterForAll.GetValue())) {
        return doc->m_mdivScoreDef.GetPgFoot();
    }
    else {
        return doc->m_mdivScoreDef.GetPgFoot2();
    }
}

void Page::LayOut(bool force)
{
    if (m_layoutDone && !force) {
        // We only need to reset the header - this will adjust the page number if necessary
        if (this->GetHeader()) this->GetHeader()->SetDrawingPage(this);
        if (this->GetFooter()) this->GetFooter()->SetDrawingPage(this);
        return;
    }

    this->LayOutHorizontally();
    this->JustifyHorizontally();
    this->LayOutVertically();
    this->JustifyVertically();

    Doc *doc = vrv_cast<Doc *>(GetFirstAncestor(DOC));
    assert(doc);
    if (doc->GetOptions()->m_svgBoundingBoxes.GetValue()) {
        View view;
        view.SetDoc(doc);
        BBoxDeviceContext bBoxDC(&view, 0, 0);
        // Do not do the layout in this view - otherwise we will loop...
        view.SetPage(this->GetIdx(), false);
        view.DrawCurrentPage(&bBoxDC, false);
    }

    m_layoutDone = true;
}

void Page::LayOutTranscription(bool force)
{
    if (m_layoutDone && !force) {
        return;
    }

    Doc *doc = vrv_cast<Doc *>(GetFirstAncestor(DOC));
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
    AlignHorizontallyParams alignHorizontallyParams(&alignHorizontally, doc);
    this->Process(&alignHorizontally, &alignHorizontallyParams, &alignHorizontallyEnd);

    // Align the content of the page using system aligners
    // After this:
    // - each Staff object will then have its StaffAlignment pointer initialized
    Functor alignVertically(&Object::AlignVertically);
    Functor alignVerticallyEnd(&Object::AlignVerticallyEnd);
    AlignVerticallyParams alignVerticallyParams(doc, &alignVertically, &alignVerticallyEnd);
    this->Process(&alignVertically, &alignVerticallyParams, &alignVerticallyEnd);

    // Set the pitch / pos alignement
    SetAlignmentPitchPosParams setAlignmentPitchPosParams(doc);
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
    View view;
    view.SetDoc(doc);
    BBoxDeviceContext bBoxDC(&view, 0, 0, BBOX_HORIZONTAL_ONLY);
    // Do not do the layout in this view - otherwise we will loop...
    view.SetPage(this->GetIdx(), false);
    view.DrawCurrentPage(&bBoxDC, false);

    Functor adjustXRelForTranscription(&Object::AdjustXRelForTranscription);
    this->Process(&adjustXRelForTranscription, NULL);

    FunctorDocParams calcLedgerLinesParams(doc);
    Functor calcLedgerLines(&Object::CalcLedgerLines);
    this->Process(&calcLedgerLines, &calcLedgerLinesParams);

    m_layoutDone = true;
}

void Page::LayOutHorizontally()
{
    Doc *doc = vrv_cast<Doc *>(GetFirstAncestor(DOC));
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
    AlignHorizontallyParams alignHorizontallyParams(&alignHorizontally, doc);
    this->Process(&alignHorizontally, &alignHorizontallyParams, &alignHorizontallyEnd);

    // Align the content of the page using system aligners
    // After this:
    // - each Staff object will then have its StaffAlignment pointer initialized
    Functor alignVertically(&Object::AlignVertically);
    Functor alignVerticallyEnd(&Object::AlignVerticallyEnd);
    AlignVerticallyParams alignVerticallyParams(doc, &alignVertically, &alignVerticallyEnd);
    this->Process(&alignVertically, &alignVerticallyParams, &alignVerticallyEnd);

    // Unless duration-based spacing is disabled, set the X position of each Alignment.
    // Does non-linear spacing based on the duration space between two Alignment objects.
    if (!doc->GetOptions()->m_evenNoteSpacing.GetValue()) {
        int longestActualDur = DUR_4;

        // Detect the longest duration in order to adjust the spacing (false by default)
        if (doc->GetOptions()->m_spacingDurDetection.GetValue()) {
            // Get the longest duration in the piece
            AttDurExtremeComparison durExtremeComparison(LONGEST);
            Object *longestDur = this->FindDescendantExtremeByComparison(&durExtremeComparison);
            if (longestDur) {
                DurationInterface *interface = longestDur->GetDurationInterface();
                assert(interface);
                longestActualDur = interface->GetActualDur();
                // LogDebug("Longest duration is DUR_* code %d", longestActualDur);
            }
        }

        Functor setAlignmentX(&Object::SetAlignmentXPos);
        SetAlignmentXPosParams setAlignmentXPosParams(doc, &setAlignmentX);
        setAlignmentXPosParams.m_longestActualDur = longestActualDur;
        this->Process(&setAlignmentX, &setAlignmentXPosParams);
    }

    // Set the pitch / pos alignement
    SetAlignmentPitchPosParams setAlignmentPitchPosParams(doc);
    Functor setAlignmentPitchPos(&Object::SetAlignmentPitchPos);
    this->Process(&setAlignmentPitchPos, &setAlignmentPitchPosParams);

    if (Att::IsMensuralType(doc->m_notationType)) {
        FunctorDocParams calcLigatureNotePosParams(doc);
        Functor calcLigatureNotePos(&Object::CalcLigatureNotePos);
        this->Process(&calcLigatureNotePos, &calcLigatureNotePosParams);
    }

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
    View view;
    view.SetDoc(doc);
    BBoxDeviceContext bBoxDC(&view, 0, 0, BBOX_HORIZONTAL_ONLY);
    // Do not do the layout in this view - otherwise we will loop...
    view.SetPage(this->GetIdx(), false);
    view.DrawCurrentPage(&bBoxDC, false);

    // Adjust the x position of the LayerElement where multiple layer collide
    // Look at each LayerElement and change the m_xShift if the bounding box is overlapping
    Functor adjustLayers(&Object::AdjustLayers);
    AdjustLayersParams adjustLayersParams(doc, &adjustLayers, doc->m_mdivScoreDef.GetStaffNs());
    this->Process(&adjustLayers, &adjustLayersParams);

    // Adjust the X position of the accidentals, including in chords
    Functor adjustAccidX(&Object::AdjustAccidX);
    AdjustAccidXParams adjustAccidXParams(doc, &adjustAccidX);
    this->Process(&adjustAccidX, &adjustAccidXParams);

    // Adjust the X shift of the Alignment looking at the bounding boxes
    // Look at each LayerElement and change the m_xShift if the bounding box is overlapping
    Functor adjustXPos(&Object::AdjustXPos);
    Functor adjustXPosEnd(&Object::AdjustXPosEnd);
    AdjustXPosParams adjustXPosParams(doc, &adjustXPos, &adjustXPosEnd, doc->m_mdivScoreDef.GetStaffNs());
    this->Process(&adjustXPos, &adjustXPosParams, &adjustXPosEnd);

    // Adjust the X shift of the Alignment looking at the bounding boxes
    // Look at each LayerElement and change the m_xShift if the bounding box is overlapping
    Functor adjustGraceXPos(&Object::AdjustGraceXPos);
    Functor adjustGraceXPosEnd(&Object::AdjustGraceXPosEnd);
    AdjustGraceXPosParams adjustGraceXPosParams(
        doc, &adjustGraceXPos, &adjustGraceXPosEnd, doc->m_mdivScoreDef.GetStaffNs());
    this->Process(&adjustGraceXPos, &adjustGraceXPosParams, &adjustGraceXPosEnd);

    // We need to populate processing lists for processing the document by Layer (for matching @tie) and
    // by Verse (for matching syllable connectors)
    PrepareProcessingListsParams prepareProcessingListsParams;
    Functor prepareProcessingLists(&Object::PrepareProcessingLists);
    this->Process(&prepareProcessingLists, &prepareProcessingListsParams);

    this->AdjustSylSpacingByVerse(prepareProcessingListsParams, doc);

    Functor adjustHarmGrpsSpacing(&Object::AdjustHarmGrpsSpacing);
    Functor adjustHarmGrpsSpacingEnd(&Object::AdjustHarmGrpsSpacingEnd);
    AdjustHarmGrpsSpacingParams adjustHarmGrpsSpacingParams(doc, &adjustHarmGrpsSpacing, &adjustHarmGrpsSpacingEnd);
    this->Process(&adjustHarmGrpsSpacing, &adjustHarmGrpsSpacingParams, &adjustHarmGrpsSpacingEnd);

    // Adjust the arpeg
    Functor adjustArpeg(&Object::AdjustArpeg);
    Functor adjustArpegEnd(&Object::AdjustArpegEnd);
    AdjustArpegParams adjustArpegParams(doc, &adjustArpeg);
    this->Process(&adjustArpeg, &adjustArpegParams, &adjustArpegEnd);

    // Adjust the position of the tuplets
    FunctorDocParams adjustTupletsXParams(doc);
    Functor adjustTupletsX(&Object::AdjustTupletsX);
    this->Process(&adjustTupletsX, &adjustTupletsXParams);

    // Prevent a margin overflow
    Functor adjustXOverlfow(&Object::AdjustXOverflow);
    Functor adjustXOverlfowEnd(&Object::AdjustXOverflowEnd);
    AdjustXOverflowParams adjustXOverflowParams(doc->GetDrawingUnit(100));
    this->Process(&adjustXOverlfow, &adjustXOverflowParams, &adjustXOverlfowEnd);

    // Adjust measure X position
    AlignMeasuresParams alignMeasuresParams;
    Functor alignMeasures(&Object::AlignMeasures);
    Functor alignMeasuresEnd(&Object::AlignMeasuresEnd);
    this->Process(&alignMeasures, &alignMeasuresParams, &alignMeasuresEnd);
}

void Page::LayOutVertically()
{
    Doc *doc = vrv_cast<Doc *>(GetFirstAncestor(DOC));
    assert(doc);

    // Doc::SetDrawingPage should have been called before
    // Make sure we have the correct page
    assert(this == doc->GetDrawingPage());

    // Reset the vertical alignment
    Functor resetVerticalAlignment(&Object::ResetVerticalAlignment);
    this->Process(&resetVerticalAlignment, NULL);

    FunctorDocParams calcLedgerLinesParams(doc);
    Functor calcLedgerLines(&Object::CalcLedgerLines);
    this->Process(&calcLedgerLines, &calcLedgerLinesParams);

    // Align the content of the page using system aligners
    // After this:
    // - each Staff object will then have its StaffAlignment pointer initialized
    Functor alignVertically(&Object::AlignVertically);
    Functor alignVerticallyEnd(&Object::AlignVerticallyEnd);
    AlignVerticallyParams alignVerticallyParams(doc, &alignVertically, &alignVerticallyEnd);
    this->Process(&alignVertically, &alignVerticallyParams, &alignVerticallyEnd);

    // Adjust the position of outside articulations
    FunctorDocParams calcArticParams(doc);
    Functor calcArtic(&Object::CalcArtic);
    this->Process(&calcArtic, &calcArticParams);

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

    // Adjust the position of the beams in regards of layer elements
    AdjustBeamParams adjustBeamParams(doc);
    Functor adjustBeams(&Object::AdjustBeams);
    Functor adjustBeamsEnd(&Object::AdjustBeamsEnd);
    this->Process(&adjustBeams, &adjustBeamParams, &adjustBeamsEnd);

    // Adjust the position of the tuplets
    FunctorDocParams adjustTupletsYParams(doc);
    Functor adjustTupletsY(&Object::AdjustTupletsY);
    this->Process(&adjustTupletsY, &adjustTupletsYParams);

    // Adjust the position of the slurs
    Functor adjustSlurs(&Object::AdjustSlurs);
    AdjustSlursParams adjustSlursParams(doc, &adjustSlurs);
    this->Process(&adjustSlurs, &adjustSlursParams);

    // If slurs were adjusted we need to redraw to adjust the bounding boxes
    if (adjustSlursParams.m_adjusted) {
        view.SetPage(this->GetIdx(), false);
        view.DrawCurrentPage(&bBoxDC, false);
    }

    // Fill the arrays of bounding boxes (above and below) for each staff alignment for which the box overflows.
    SetOverflowBBoxesParams setOverflowBBoxesParams(doc);
    Functor setOverflowBBoxes(&Object::SetOverflowBBoxes);
    Functor setOverflowBBoxesEnd(&Object::SetOverflowBBoxesEnd);
    this->Process(&setOverflowBBoxes, &setOverflowBBoxesParams, &setOverflowBBoxesEnd);

    // Adjust the positioners of floationg elements (slurs, hairpin, dynam, etc)
    Functor adjustFloatingPositioners(&Object::AdjustFloatingPositioners);
    AdjustFloatingPositionersParams adjustFloatingPositionersParams(doc, &adjustFloatingPositioners);
    this->Process(&adjustFloatingPositioners, &adjustFloatingPositionersParams);

    // Adjust the overlap of the staff aligmnents by looking at the overflow bounding boxes params.clear();
    Functor adjustStaffOverlap(&Object::AdjustStaffOverlap);
    AdjustStaffOverlapParams adjustStaffOverlapParams(&adjustStaffOverlap);
    this->Process(&adjustStaffOverlap, &adjustStaffOverlapParams);

    // Set the Y position of each StaffAlignment
    // Adjust the Y shift to make sure there is a minimal space (staffMargin) between each staff
    Functor adjustYPos(&Object::AdjustYPos);
    AdjustYPosParams adjustYPosParams(doc, &adjustYPos);
    this->Process(&adjustYPos, &adjustYPosParams);

    // Adjust the positioners of floationg elements placed between staves
    Functor adjustFloatingPositionersBetween(&Object::AdjustFloatingPositionersBetween);
    AdjustFloatingPositionersBetweenParams adjustFloatingPositionersBetweenParams(
        doc, &adjustFloatingPositionersBetween);
    this->Process(&adjustFloatingPositionersBetween, &adjustFloatingPositionersBetweenParams);

    Functor adjustCrossStaffYPos(&Object::AdjustCrossStaffYPos);
    Functor adjustCrossStaffYPosEnd(&Object::AdjustCrossStaffYPosEnd);
    FunctorDocParams adjustCrossStaffYPosParams(doc);
    this->Process(&adjustCrossStaffYPos, &adjustCrossStaffYPosParams, &adjustCrossStaffYPosEnd);

    if (this->GetHeader()) {
        this->GetHeader()->AdjustRunningElementYPos();
    }

    if (this->GetFooter()) {
        this->GetFooter()->AdjustRunningElementYPos();
    }

    // Adjust system Y position
    AlignSystemsParams alignSystemsParams(doc);
    alignSystemsParams.m_shift = doc->m_drawingPageContentHeight;
    alignSystemsParams.m_systemMargin = (doc->GetOptions()->m_spacingSystem.GetValue()) * doc->GetDrawingUnit(100);
    Functor alignSystems(&Object::AlignSystems);
    Functor alignSystemsEnd(&Object::AlignSystemsEnd);
    this->Process(&alignSystems, &alignSystemsParams, &alignSystemsEnd);
}

void Page::JustifyHorizontally()
{
    Doc *doc = vrv_cast<Doc *>(GetFirstAncestor(DOC));
    assert(doc);

    if ((doc->GetOptions()->m_breaks.GetValue() == BREAKS_none) || doc->GetOptions()->m_noJustification.GetValue()) {
        return;
    }

    // Doc::SetDrawingPage should have been called before
    // Make sure we have the correct page
    assert(this == doc->GetDrawingPage());

    if ((doc->GetOptions()->m_adjustPageWidth.GetValue())) {
        doc->m_drawingPageContentWidth = this->GetContentWidth();
        doc->m_drawingPageWidth
            = doc->m_drawingPageContentWidth + doc->m_drawingPageMarginLeft + doc->m_drawingPageMarginRight;
    }
    else {
        // Justify X position
        Functor justifyX(&Object::JustifyX);
        JustifyXParams justifyXParams(&justifyX, doc);
        justifyXParams.m_systemFullWidth = doc->m_drawingPageContentWidth;
        this->Process(&justifyX, &justifyXParams);
    }
}

void Page::JustifyVertically()
{
    Doc *doc = vrv_cast<Doc *>(GetFirstAncestor(DOC));
    assert(doc);

    // Doc::SetDrawingPage should have been called before
    // Make sure we have the correct page
    assert(this == doc->GetDrawingPage());

    // Nothing to justify
    if (m_drawingJustifiableHeight <= 0 || m_justificationSum <= 0) {
        return;
    }

    // Vertical justificaiton is not enabled
    if (!doc->GetOptions()->m_justifyVertically.GetValue()) {
        return;
    }

    // Last page and justification of last page is not enabled
    Pages *pages = doc->GetPages();
    assert(pages);
    if (pages->GetLast() == this) {
        int idx = this->GetIdx();
        const int childSystems = GetChildCount(SYSTEM);
        if (idx > 0) {
            Page *penultimatePage = dynamic_cast<Page *>(pages->GetPrevious(this));
            assert(penultimatePage);

            if (penultimatePage->m_drawingJustifiableHeight < this->m_drawingJustifiableHeight) {
                this->m_drawingJustifiableHeight = penultimatePage->m_drawingJustifiableHeight;
            }

            const int maxSystemsPerPage = doc->GetOptions()->m_systemMaxPerPage.GetValue();
            if ((childSystems <= 2) || (childSystems < maxSystemsPerPage)) {
                m_justificationSum = penultimatePage->m_justificationSum;
            }
        }
        else {
            const int stavesPerSystem = m_drawingScoreDef.GetChildCount(STAFFDEF, UNLIMITED_DEPTH);
            if (childSystems * stavesPerSystem < 8) return;
        }
    }

    // Justify Y position
    Functor justifyY(&Object::JustifyY);
    JustifyYParams justifyYParams(&justifyY, doc);
    justifyYParams.m_justificationSum = this->m_justificationSum;
    justifyYParams.m_spaceToDistribute = this->m_drawingJustifiableHeight;
    this->Process(&justifyY, &justifyYParams);
}

void Page::LayOutPitchPos()
{
    Doc *doc = vrv_cast<Doc *>(GetFirstAncestor(DOC));
    assert(doc);

    // Doc::SetDrawingPage should have been called before
    // Make sure we have the correct page
    assert(this == doc->GetDrawingPage());

    // Set the pitch / pos alignement
    SetAlignmentPitchPosParams setAlignmentPitchPosParams(doc);
    Functor setAlignmentPitchPos(&Object::SetAlignmentPitchPos);
    this->Process(&setAlignmentPitchPos, &setAlignmentPitchPosParams);

    CalcStemParams calcStemParams(doc);
    Functor calcStem(&Object::CalcStem);
    this->Process(&calcStem, &calcStemParams);
}

int Page::GetContentHeight() const
{
    Doc *doc = vrv_cast<Doc *>(GetFirstAncestor(DOC));
    assert(doc);

    // Doc::SetDrawingPage should have been called before
    // Make sure we have the correct page
    assert(this == doc->GetDrawingPage());

    if (!GetChildCount()) {
        return 0;
    }

    System *last = dynamic_cast<System *>(GetChildren()->back());
    assert(last);
    int height = doc->m_drawingPageContentHeight - last->GetDrawingYRel() + last->GetHeight();

    if (this->GetFooter()) {
        height += this->GetFooter()->GetTotalHeight();
    }

    return height;
}

int Page::GetContentWidth() const
{
    Doc *doc = vrv_cast<Doc *>(GetFirstAncestor(DOC));
    assert(doc);
    // in non debug
    if (!doc) return 0;

    // Doc::SetDrawingPage should have been called before
    // Make sure we have the correct page
    assert(this == doc->GetDrawingPage());

    int maxWidth = 0;
    for (auto &child : *this->GetChildren()) {
        System *system = dynamic_cast<System *>(child);
        if (system) {
            // we include the left margin and the right margin
            int systemWidth = system->m_drawingTotalWidth + system->m_systemLeftMar + system->m_systemRightMar;
            maxWidth = std::max(systemWidth, maxWidth);
        }
    }
    // For avoiding unused variable warning in non debug mode
    doc = NULL;

    return maxWidth;
}

void Page::AdjustSylSpacingByVerse(PrepareProcessingListsParams &listsParams, Doc *doc)
{
    IntTree_t::iterator staves;
    IntTree_t::iterator layers;
    IntTree_t::iterator verses;

    if (listsParams.m_verseTree.child.empty()) return;

    ArrayOfComparisons filters;

    // Same for the lyrics, but Verse by Verse since Syl are TimeSpanningInterface elements for handling connectors
    for (staves = listsParams.m_verseTree.child.begin(); staves != listsParams.m_verseTree.child.end(); ++staves) {
        for (layers = staves->second.child.begin(); layers != staves->second.child.end(); ++layers) {
            for (verses = layers->second.child.begin(); verses != layers->second.child.end(); ++verses) {
                // Create ad comparison object for each type / @n
                AttNIntegerComparison matchStaff(STAFF, staves->first);
                AttNIntegerComparison matchLayer(LAYER, layers->first);
                AttNIntegerComparison matchVerse(VERSE, verses->first);
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

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

int Page::ApplyPPUFactor(FunctorParams *functorParams)
{
    ApplyPPUFactorParams *params = vrv_params_cast<ApplyPPUFactorParams *>(functorParams);
    assert(params);

    params->m_page = this;
    this->m_pageWidth /= params->m_page->GetPPUFactor();
    this->m_pageHeight /= params->m_page->GetPPUFactor();
    this->m_pageMarginBottom /= params->m_page->GetPPUFactor();
    this->m_pageMarginLeft /= params->m_page->GetPPUFactor();
    this->m_pageMarginRight /= params->m_page->GetPPUFactor();
    this->m_pageMarginTop /= params->m_page->GetPPUFactor();

    return FUNCTOR_CONTINUE;
}

int Page::ResetVerticalAlignment(FunctorParams *functorParams)
{
    // Same functor, but we have not FunctorParams so we just re-instanciate it
    Functor resetVerticalAlignment(&Object::ResetVerticalAlignment);

    RunningElement *header = this->GetHeader();
    if (header) {
        header->Process(&resetVerticalAlignment, NULL);
        header->SetDrawingPage(NULL);
        header->SetDrawingYRel(0);
    }
    RunningElement *footer = this->GetFooter();
    if (footer) {
        footer->Process(&resetVerticalAlignment, NULL);
        footer->SetDrawingPage(NULL);
        footer->SetDrawingYRel(0);
    }

    return FUNCTOR_CONTINUE;
}

int Page::AlignVerticallyEnd(FunctorParams *functorParams)
{
    AlignVerticallyParams *params = vrv_params_cast<AlignVerticallyParams *>(functorParams);
    assert(params);

    params->m_cumulatedShift = 0;

    // Also align the header and footer

    RunningElement *header = this->GetHeader();
    if (header) {
        header->SetDrawingPage(this);
        header->SetDrawingYRel(0);
        header->Process(params->m_functor, params, params->m_functorEnd);
    }
    RunningElement *footer = this->GetFooter();
    if (footer) {
        footer->SetDrawingPage(this);
        footer->SetDrawingYRel(0);
        footer->Process(params->m_functor, params, params->m_functorEnd);
    }

    return FUNCTOR_CONTINUE;
}

int Page::AlignSystems(FunctorParams *functorParams)
{
    AlignSystemsParams *params = vrv_params_cast<AlignSystemsParams *>(functorParams);
    assert(params);

    params->m_justificationSum = 0;

    RunningElement *header = this->GetHeader();
    if (header) {
        const int bottomMarginPgHead
            = params->m_doc->GetOptions()->m_bottomMarginPgHead.GetValue() * params->m_doc->GetDrawingUnit(100);

        header->SetDrawingYRel(params->m_shift);
        params->m_shift -= header->GetTotalHeight() + bottomMarginPgHead;
    }
    return FUNCTOR_CONTINUE;
}

int Page::AlignSystemsEnd(FunctorParams *functorParams)
{
    AlignSystemsParams *params = vrv_params_cast<AlignSystemsParams *>(functorParams);
    assert(params);

    this->m_drawingJustifiableHeight = params->m_shift;
    this->m_justificationSum = params->m_justificationSum;

    RunningElement *footer = this->GetFooter();
    if (footer) {
        this->m_drawingJustifiableHeight -= footer->GetTotalHeight();

        // Move it up below the last system
        if (params->m_doc->GetOptions()->m_adjustPageHeight.GetValue()) {
            if (GetChildCount()) {
                System *last = dynamic_cast<System *>(GetChildren()->back());
                assert(last);
                footer->SetDrawingYRel(last->GetDrawingYRel() - last->GetHeight());
            }
        }
        else {
            footer->SetDrawingYRel(footer->GetTotalHeight());
        }
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

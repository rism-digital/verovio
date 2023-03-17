/////////////////////////////////////////////////////////////////////////////
// Name:        page.cpp
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "page.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "adjustaccidxfunctor.h"
#include "adjustarpegfunctor.h"
#include "adjustclefchangesfunctor.h"
#include "adjustdotsfunctor.h"
#include "adjustgracexposfunctor.h"
#include "adjustharmgrpsspacingfunctor.h"
#include "adjustlayersfunctor.h"
#include "adjustsylspacingfunctor.h"
#include "adjusttempofunctor.h"
#include "adjusttupletsxfunctor.h"
#include "adjustxoverflowfunctor.h"
#include "adjustxposfunctor.h"
#include "alignfunctor.h"
#include "bboxdevicecontext.h"
#include "cachehorizontallayoutfunctor.h"
#include "calcalignmentpitchposfunctor.h"
#include "calcalignmentxposfunctor.h"
#include "calcchordnoteheadsfunctor.h"
#include "calcdotsfunctor.h"
#include "calcledgerlinesfunctor.h"
#include "calcslurdirectionfunctor.h"
#include "calcspanningbeamspansfunctor.h"
#include "calcstemfunctor.h"
#include "comparison.h"
#include "doc.h"
#include "functor.h"
#include "functorparams.h"
#include "libmei.h"
#include "pageelement.h"
#include "pages.h"
#include "pgfoot.h"
#include "pgfoot2.h"
#include "pghead.h"
#include "pghead2.h"
#include "resetfunctor.h"
#include "score.h"
#include "staff.h"
#include "system.h"
#include "view.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Page
//----------------------------------------------------------------------------

Page::Page() : Object(PAGE, "page-")
{
    this->Reset();
}

Page::~Page() {}

void Page::Reset()
{
    Object::Reset();

    m_drawingScoreDef.Reset();
    m_score = NULL;
    m_scoreEnd = NULL;
    m_layoutDone = false;
    this->ResetID();

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
    if (child->IsPageElement()) {
        assert(dynamic_cast<PageElement *>(child));
    }
    else if (child->Is(SYSTEM)) {
        assert(dynamic_cast<System *>(child));
    }
    else {
        return false;
    }
    return true;
}

bool Page::IsFirstOfSelection() const
{
    const Doc *doc = vrv_cast<const Doc *>(this->GetFirstAncestor(DOC));
    assert(doc);
    if (!doc->HasSelection()) return false;

    assert(this->GetParent());
    return (this->GetParent()->GetFirst() == this);
}

bool Page::IsLastOfSelection() const
{
    const Doc *doc = vrv_cast<const Doc *>(this->GetFirstAncestor(DOC));
    assert(doc);
    if (!doc->HasSelection()) return false;

    assert(this->GetParent());
    return (this->GetParent()->GetLast() == this);
}

RunningElement *Page::GetHeader()
{
    return const_cast<RunningElement *>(std::as_const(*this).GetHeader());
}

const RunningElement *Page::GetHeader() const
{
    assert(m_score);

    const Doc *doc = vrv_cast<const Doc *>(this->GetFirstAncestor(DOC));
    if (!doc || (doc->GetOptions()->m_header.GetValue() == HEADER_none)) {
        return NULL;
    }

    const Pages *pages = doc->GetPages();
    assert(pages);

    // first page or use the pgHeader for all pages?
    if ((pages->GetFirst() == this) || (doc->GetOptions()->m_usePgHeaderForAll.GetValue())) {
        return m_score->GetScoreDef()->GetPgHead();
    }
    else {
        return m_score->GetScoreDef()->GetPgHead2();
    }
}

RunningElement *Page::GetFooter()
{
    return const_cast<RunningElement *>(std::as_const(*this).GetFooter());
}

const RunningElement *Page::GetFooter() const
{
    assert(m_scoreEnd);

    const Doc *doc = vrv_cast<const Doc *>(this->GetFirstAncestor(DOC));
    if (!doc || (doc->GetOptions()->m_footer.GetValue() == FOOTER_none)) {
        return NULL;
    }

    const Pages *pages = doc->GetPages();
    assert(pages);

    // first page or use the pgFooter for all pages?
    if ((pages->GetFirst() == this) || (doc->GetOptions()->m_usePgFooterForAll.GetValue())) {
        return m_scoreEnd->GetScoreDef()->GetPgFoot();
    }
    else {
        return m_scoreEnd->GetScoreDef()->GetPgFoot2();
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

    Doc *doc = vrv_cast<Doc *>(this->GetFirstAncestor(DOC));
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

    Doc *doc = vrv_cast<Doc *>(this->GetFirstAncestor(DOC));
    assert(doc);

    // Doc::SetDrawingPage should have been called before
    // Make sure we have the correct page
    assert(this == doc->GetDrawingPage());

    // Reset the horizontal alignment
    ResetHorizontalAlignmentFunctor resetHorizontalAlignment;
    this->Process(resetHorizontalAlignment);

    // Reset the vertical alignment
    Functor resetVerticalAlignment(&Object::ResetVerticalAlignment);
    this->Process(&resetVerticalAlignment, NULL);

    // Align the content of the page using measure aligners
    // After this:
    // - each LayerElement object will have its Alignment pointer initialized
    AlignHorizontallyFunctor alignHorizontally(doc);
    this->Process(alignHorizontally);

    // Align the content of the page using system aligners
    // After this:
    // - each Staff object will then have its StaffAlignment pointer initialized
    Functor alignVertically(&Object::AlignVertically);
    Functor alignVerticallyEnd(&Object::AlignVerticallyEnd);
    AlignVerticallyParams alignVerticallyParams(doc, &alignVertically, &alignVerticallyEnd);
    this->Process(&alignVertically, &alignVerticallyParams, &alignVerticallyEnd);

    // Set the pitch / pos alignment
    CalcAlignmentPitchPosFunctor calcAlignmentPitchPos(doc);
    this->Process(calcAlignmentPitchPos);

    CalcStemFunctor calcStem(doc);
    this->Process(calcStem);

    CalcChordNoteHeadsFunctor calcChordNoteHeads(doc);
    this->Process(calcChordNoteHeads);

    CalcDotsFunctor calcDots(doc);
    this->Process(calcDots);

    // Render it for filling the bounding box
    View view;
    view.SetDoc(doc);
    BBoxDeviceContext bBoxDC(&view, 0, 0, BBOX_HORIZONTAL_ONLY);
    // Do not do the layout in this view - otherwise we will loop...
    view.SetPage(this->GetIdx(), false);
    view.DrawCurrentPage(&bBoxDC, false);

    Functor adjustXRelForTranscription(&Object::AdjustXRelForTranscription);
    this->Process(&adjustXRelForTranscription, NULL);

    CalcLedgerLinesFunctor calcLedgerLines(doc);
    this->Process(calcLedgerLines);

    m_layoutDone = true;
}

void Page::ResetAligners()
{
    Doc *doc = vrv_cast<Doc *>(this->GetFirstAncestor(DOC));
    assert(doc);

    // Doc::SetDrawingPage should have been called before
    // Make sure we have the correct page
    assert(this == doc->GetDrawingPage());

    // Reset the horizontal alignment
    ResetHorizontalAlignmentFunctor resetHorizontalAlignment;
    this->Process(resetHorizontalAlignment);

    // Reset the vertical alignment
    Functor resetVerticalAlignment(&Object::ResetVerticalAlignment);
    this->Process(&resetVerticalAlignment, NULL);

    // Align the content of the page using measure aligners
    // After this:
    // - each LayerElement object will have its Alignment pointer initialized
    AlignHorizontallyFunctor alignHorizontally(doc);
    this->Process(alignHorizontally);

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

        CalcAlignmentXPosFunctor calcAlignmentXPos(doc);
        calcAlignmentXPos.SetLongestActualDur(longestActualDur);
        this->Process(calcAlignmentXPos);
    }

    // Set the pitch / pos alignment
    CalcAlignmentPitchPosFunctor calcAlignmentPitchPos(doc);
    this->Process(calcAlignmentPitchPos);

    if (IsMensuralType(doc->m_notationType)) {
        FunctorDocParams calcLigatureNotePosParams(doc);
        Functor calcLigatureNotePos(&Object::CalcLigatureNotePos);
        this->Process(&calcLigatureNotePos, &calcLigatureNotePosParams);
    }

    CalcStemFunctor calcStem(doc);
    this->Process(calcStem);

    CalcChordNoteHeadsFunctor calcChordNoteHeads(doc);
    this->Process(calcChordNoteHeads);

    CalcDotsFunctor calcDots(doc);
    this->Process(calcDots);

    // Adjust the position of outside articulations
    CalcArticParams calcArticParams(doc);
    Functor calcArtic(&Object::CalcArtic);
    this->Process(&calcArtic, &calcArticParams);

    CalcSlurDirectionFunctor calcSlurDirection(doc);
    this->Process(calcSlurDirection);

    CalcSpanningBeamSpansFunctor calcSpanningBeamSpans(doc);
    this->Process(calcSpanningBeamSpans);
}

void Page::LayOutHorizontally()
{
    Doc *doc = vrv_cast<Doc *>(this->GetFirstAncestor(DOC));
    assert(doc);

    // Doc::SetDrawingPage should have been called before
    // Make sure we have the correct page
    assert(this == doc->GetDrawingPage());

    this->ResetAligners();

    // Render it for filling the bounding box
    View view;
    view.SetDoc(doc);
    view.SetSlurHandling(SlurHandling::Ignore);
    BBoxDeviceContext bBoxDC(&view, 0, 0, BBOX_HORIZONTAL_ONLY);
    // Do not do the layout in this view - otherwise we will loop...
    view.SetPage(this->GetIdx(), false);
    view.DrawCurrentPage(&bBoxDC, false);

    // Adjust the position of outside articulations
    AdjustArticParams adjustArticParams(doc);
    Functor adjustArtic(&Object::AdjustArtic);
    this->Process(&adjustArtic, &adjustArticParams);

    // Adjust the x position of the LayerElement where multiple layers collide
    // Look at each LayerElement and change the m_xShift if the bounding box is overlapping
    // For the first iteration align elements without taking dots into consideration
    AdjustLayersFunctor adjustLayers(doc, doc->GetCurrentScoreDef()->GetStaffNs());
    this->Process(adjustLayers);

    // Adjust dots for the multiple layers. Try to align dots that can be grouped together when layers collide,
    // otherwise keep their relative positioning
    AdjustDotsFunctor adjustDots(doc, doc->GetCurrentScoreDef()->GetStaffNs());
    this->Process(adjustDots);

    // Adjust layers again, this time including dots positioning
    AdjustLayersFunctor adjustLayersWithDots(doc, doc->GetCurrentScoreDef()->GetStaffNs());
    adjustLayersWithDots.IgnoreDots(false);
    this->Process(adjustLayersWithDots);

    // Adjust the X position of the accidentals, including in chords
    AdjustAccidXFunctor adjustAccidX(doc);
    this->Process(adjustAccidX);

    // Adjust the X shift of the Alignment looking at the bounding boxes
    // Look at each LayerElement and change the m_xShift if the bounding box is overlapping
    AdjustXPosFunctor adjustXPos(doc, doc->GetCurrentScoreDef()->GetStaffNs());
    adjustXPos.SetExcluded({ TABDURSYM });
    this->Process(adjustXPos);

    // Adjust tabRhythm separately
    adjustXPos.ClearExcluded();
    adjustXPos.SetIncluded({ BARLINE, KEYSIG, METERSIG, TABDURSYM });
    adjustXPos.SetRightBarLinesOnly(true);
    this->Process(adjustXPos);

    // Adjust the X shift of the Alignment looking at the bounding boxes
    // Look at each LayerElement and change the m_xShift if the bounding box is overlapping
    AdjustGraceXPosFunctor adjustGraceXPos(doc, doc->GetCurrentScoreDef()->GetStaffNs());
    this->Process(adjustGraceXPos);

    // Adjust the spacing of clef changes since they are skipped in AdjustXPos
    // Look at each clef change and  move them to the left and add space if necessary
    AdjustClefChangesFunctor adjustClefChanges(doc);
    this->Process(adjustClefChanges);

    // We need to populate processing lists for processing the document by Layer (for matching @tie) and
    // by Verse (for matching syllable connectors)
    InitProcessingListsParams initProcessingListsParams;
    Functor initProcessingLists(&Object::InitProcessingLists);
    this->Process(&initProcessingLists, &initProcessingListsParams);

    this->AdjustSylSpacingByVerse(initProcessingListsParams, doc);

    AdjustHarmGrpsSpacingFunctor adjustHarmGrpsSpacing(doc);
    this->Process(adjustHarmGrpsSpacing);

    // Adjust the arpeg
    AdjustArpegFunctor adjustArpeg(doc);
    this->Process(adjustArpeg);

    // Adjust the tempo
    AdjustTempoFunctor adjustTempo(doc);
    this->Process(adjustTempo);

    // Adjust the position of the tuplets
    AdjustTupletsXFunctor adjustTupletsX(doc);
    this->Process(adjustTupletsX);

    // Prevent a margin overflow
    AdjustXOverflowFunctor adjustXOverflow(doc->GetDrawingUnit(100));
    this->Process(adjustXOverflow);

    // Adjust measure X position
    AlignMeasuresFunctor alignMeasures(doc);
    this->Process(alignMeasures);
}

void Page::LayOutHorizontallyWithCache(bool restore)
{
    Doc *doc = vrv_cast<Doc *>(this->GetFirstAncestor(DOC));
    assert(doc);

    CacheHorizontalLayoutFunctor cacheHorizontalLayout(doc);
    cacheHorizontalLayout.SetRestore(restore);
    this->Process(cacheHorizontalLayout);
}

void Page::LayOutVertically()
{
    Doc *doc = vrv_cast<Doc *>(this->GetFirstAncestor(DOC));
    assert(doc);

    // Doc::SetDrawingPage should have been called before
    // Make sure we have the correct page
    assert(this == doc->GetDrawingPage());

    // Reset the vertical alignment
    Functor resetVerticalAlignment(&Object::ResetVerticalAlignment);
    this->Process(&resetVerticalAlignment, NULL);

    CalcLedgerLinesFunctor calcLedgerLines(doc);
    this->Process(calcLedgerLines);

    // Align the content of the page using system aligners
    // After this:
    // - each Staff object will then have its StaffAlignment pointer initialized
    Functor alignVertically(&Object::AlignVertically);
    Functor alignVerticallyEnd(&Object::AlignVerticallyEnd);
    AlignVerticallyParams alignVerticallyParams(doc, &alignVertically, &alignVerticallyEnd);
    this->Process(&alignVertically, &alignVerticallyParams, &alignVerticallyEnd);

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

    // At this point slurs must not be reinitialized, otherwise the adjustment we just did was in vain
    view.SetSlurHandling(SlurHandling::Drawing);
    view.SetPage(this->GetIdx(), false);
    view.DrawCurrentPage(&bBoxDC, false);

    // Adjust the position of tuplets by slurs
    FunctorDocParams adjustTupletWithSlursParams(doc);
    Functor adjustTupletWithSlurs(&Object::AdjustTupletWithSlurs);
    this->Process(&adjustTupletWithSlurs, &adjustTupletWithSlursParams);

    // Fill the arrays of bounding boxes (above and below) for each staff alignment for which the box overflows.
    CalcBBoxOverflowsParams calcBBoxOverflowsParams(doc);
    Functor calcBBoxOverflows(&Object::CalcBBoxOverflows);
    Functor calcBBoxOverflowsEnd(&Object::CalcBBoxOverflowsEnd);
    this->Process(&calcBBoxOverflows, &calcBBoxOverflowsParams, &calcBBoxOverflowsEnd);

    // Adjust the positioners of floating elements (slurs, hairpin, dynam, etc)
    Functor adjustFloatingPositioners(&Object::AdjustFloatingPositioners);
    AdjustFloatingPositionersParams adjustFloatingPositionersParams(doc, &adjustFloatingPositioners);
    this->Process(&adjustFloatingPositioners, &adjustFloatingPositionersParams);

    // Adjust the overlap of the staff alignments by looking at the overflow bounding boxes params.clear();
    Functor adjustStaffOverlap(&Object::AdjustStaffOverlap);
    AdjustStaffOverlapParams adjustStaffOverlapParams(doc, &adjustStaffOverlap);
    this->Process(&adjustStaffOverlap, &adjustStaffOverlapParams);

    // Set the Y position of each StaffAlignment
    // Adjust the Y shift to make sure there is a minimal space (staffMargin) between each staff
    Functor adjustYPos(&Object::AdjustYPos);
    AdjustYPosParams adjustYPosParams(doc, &adjustYPos);
    this->Process(&adjustYPos, &adjustYPosParams);

    // Adjust the positioners of floating elements placed between staves
    Functor adjustFloatingPositionersBetween(&Object::AdjustFloatingPositionersBetween);
    AdjustFloatingPositionersBetweenParams adjustFloatingPositionersBetweenParams(
        doc, &adjustFloatingPositionersBetween);
    this->Process(&adjustFloatingPositionersBetween, &adjustFloatingPositionersBetweenParams);

    Functor adjustCrossStaffYPos(&Object::AdjustCrossStaffYPos);
    FunctorDocParams adjustCrossStaffYPosParams(doc);
    this->Process(&adjustCrossStaffYPos, &adjustCrossStaffYPosParams);

    // Redraw are re-adjust the position of the slurs when we have cross-staff ones
    if (adjustSlursParams.m_crossStaffSlurs) {
        view.SetSlurHandling(SlurHandling::Initialize);
        view.SetPage(this->GetIdx(), false);
        view.DrawCurrentPage(&bBoxDC, false);
        this->Process(&adjustSlurs, &adjustSlursParams);
    }

    doc->SetCurrentScore(this->m_score);

    if (this->GetHeader()) {
        this->GetHeader()->AdjustRunningElementYPos();
    }

    if (this->GetFooter()) {
        this->GetFooter()->AdjustRunningElementYPos();
    }

    // Adjust system Y position
    AlignSystemsParams alignSystemsParams(doc);
    alignSystemsParams.m_shift = doc->m_drawingPageContentHeight;
    alignSystemsParams.m_systemSpacing = (doc->GetOptions()->m_spacingSystem.GetValue()) * doc->GetDrawingUnit(100);
    Functor alignSystems(&Object::AlignSystems);
    Functor alignSystemsEnd(&Object::AlignSystemsEnd);
    this->Process(&alignSystems, &alignSystemsParams, &alignSystemsEnd);
}

void Page::JustifyHorizontally()
{
    Doc *doc = vrv_cast<Doc *>(this->GetFirstAncestor(DOC));
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
    Doc *doc = vrv_cast<Doc *>(this->GetFirstAncestor(DOC));
    assert(doc);

    // Doc::SetDrawingPage should have been called before
    // Make sure we have the correct page
    assert(this == doc->GetDrawingPage());

    // Nothing to justify
    if (m_drawingJustifiableHeight <= 0 || m_justificationSum <= 0) {
        return;
    }

    // Vertical justification is not enabled
    if (!doc->GetOptions()->m_justifyVertically.GetValue()) {
        return;
    }

    // Ignore vertical justification if it's not required
    if (!this->IsJustificationRequired(doc)) return;

    // Justify Y position
    Functor justifyY(&Object::JustifyY);
    JustifyYParams justifyYParams(&justifyY, doc);
    justifyYParams.m_justificationSum = m_justificationSum;
    justifyYParams.m_spaceToDistribute = m_drawingJustifiableHeight;
    this->Process(&justifyY, &justifyYParams);

    if (!justifyYParams.m_shiftForStaff.empty()) {
        // Adjust cross staff content which is displaced through vertical justification
        Functor justifyYAdjustCrossStaff(&Object::JustifyYAdjustCrossStaff);
        JustifyYAdjustCrossStaffParams justifyYAdjustCrossStaffParams(doc);
        justifyYAdjustCrossStaffParams.m_shiftForStaff = justifyYParams.m_shiftForStaff;
        this->Process(&justifyYAdjustCrossStaff, &justifyYAdjustCrossStaffParams);
    }
}

bool Page::IsJustificationRequired(const Doc *doc)
{
    const Pages *pages = doc->GetPages();
    assert(pages);

    const int childSystems = this->GetChildCount(SYSTEM);
    // Last page and justification of last page is not enabled
    if (pages->GetLast() == this) {
        const int idx = this->GetIdx();
        if (idx > 0) {
            const Page *previousPage = dynamic_cast<const Page *>(pages->GetPrevious(this));
            assert(previousPage);
            const int previousJustifiableHeight = previousPage->m_drawingJustifiableHeight;
            const int previousJustificationSum = previousPage->m_justificationSum;

            if (previousJustifiableHeight < m_drawingJustifiableHeight) {
                m_drawingJustifiableHeight = previousJustifiableHeight;
            }

            const int maxSystemsPerPage = doc->GetOptions()->m_systemMaxPerPage.GetValue();
            if ((childSystems <= 2) || (childSystems < maxSystemsPerPage)) {
                m_justificationSum = previousJustificationSum;
            }
        }
        else {
            const int stavesPerSystem = m_drawingScoreDef.GetDescendantCount(STAFFDEF);
            if (childSystems * stavesPerSystem < 8) return false;
        }
    }
    const double ratio = (double)m_drawingJustifiableHeight / (double)doc->m_drawingPageHeight;
    if (ratio > doc->GetOptions()->m_justificationMaxVertical.GetValue()) {
        m_drawingJustifiableHeight
            = doc->m_drawingPageHeight * doc->GetOptions()->m_justificationMaxVertical.GetValue();
    }

    return true;
}

void Page::LayOutPitchPos()
{
    Doc *doc = vrv_cast<Doc *>(this->GetFirstAncestor(DOC));
    assert(doc);

    // Doc::SetDrawingPage should have been called before
    // Make sure we have the correct page
    assert(this == doc->GetDrawingPage());

    // Set the pitch / pos alignment
    CalcAlignmentPitchPosFunctor calcAlignmentPitchPos(doc);
    this->Process(calcAlignmentPitchPos);

    CalcStemFunctor calcStem(doc);
    this->Process(calcStem);
}

int Page::GetContentHeight() const
{
    const Doc *doc = vrv_cast<const Doc *>(this->GetFirstAncestor(DOC));
    assert(doc);

    // Doc::SetDrawingPage should have been called before
    // Make sure we have the correct page
    assert(this == doc->GetDrawingPage());

    if (!this->GetChildCount()) {
        return 0;
    }

    const System *last = vrv_cast<const System *>(this->GetLast(SYSTEM));
    assert(last);
    int height = doc->m_drawingPageContentHeight - last->GetDrawingYRel() + last->GetHeight();

    if (this->GetFooter()) {
        height += this->GetFooter()->GetTotalHeight(doc);
    }

    return height;
}

int Page::GetContentWidth() const
{
    const Doc *doc = vrv_cast<const Doc *>(this->GetFirstAncestor(DOC));
    assert(doc);
    // in non debug
    if (!doc) return 0;

    // Doc::SetDrawingPage should have been called before
    // Make sure we have the correct page
    assert(this == doc->GetDrawingPage());

    int maxWidth = 0;
    for (const Object *child : this->GetChildren()) {
        const System *system = dynamic_cast<const System *>(child);
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

void Page::AdjustSylSpacingByVerse(InitProcessingListsParams &listsParams, Doc *doc)
{
    IntTree_t::iterator staves;
    IntTree_t::iterator layers;
    IntTree_t::iterator verses;

    if (listsParams.m_verseTree.child.empty()) return;

    Filters filters;

    // Same for the lyrics, but Verse by Verse since Syl are TimeSpanningInterface elements for handling connectors
    for (staves = listsParams.m_verseTree.child.begin(); staves != listsParams.m_verseTree.child.end(); ++staves) {
        for (layers = staves->second.child.begin(); layers != staves->second.child.end(); ++layers) {
            for (verses = layers->second.child.begin(); verses != layers->second.child.end(); ++verses) {
                // Create ad comparison object for each type / @n
                AttNIntegerComparison matchStaff(STAFF, staves->first);
                AttNIntegerComparison matchLayer(LAYER, layers->first);
                AttNIntegerComparison matchVerse(VERSE, verses->first);
                filters = { &matchStaff, &matchLayer, &matchVerse };

                AdjustSylSpacingFunctor adjustSylSpacing(doc);
                adjustSylSpacing.SetFilters(&filters);
                this->Process(adjustSylSpacing);
            }
        }
    }
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

FunctorCode Page::Accept(MutableFunctor &functor)
{
    return functor.VisitPage(this);
}

FunctorCode Page::Accept(ConstFunctor &functor) const
{
    return functor.VisitPage(this);
}

FunctorCode Page::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitPageEnd(this);
}

FunctorCode Page::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitPageEnd(this);
}

int Page::ApplyPPUFactor(FunctorParams *functorParams)
{
    ApplyPPUFactorParams *params = vrv_params_cast<ApplyPPUFactorParams *>(functorParams);
    assert(params);

    params->m_page = this;
    m_pageWidth /= params->m_page->GetPPUFactor();
    m_pageHeight /= params->m_page->GetPPUFactor();
    m_pageMarginBottom /= params->m_page->GetPPUFactor();
    m_pageMarginLeft /= params->m_page->GetPPUFactor();
    m_pageMarginRight /= params->m_page->GetPPUFactor();
    m_pageMarginTop /= params->m_page->GetPPUFactor();

    return FUNCTOR_CONTINUE;
}

int Page::ResetVerticalAlignment(FunctorParams *functorParams)
{
    // Same functor, but we have not FunctorParams so we just re-instantiate it
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
        header->SetDrawingYRel(params->m_shift);
        const int headerHeight = header->GetTotalHeight(params->m_doc);
        if (headerHeight > 0) {
            params->m_shift -= headerHeight;
        }
    }
    return FUNCTOR_CONTINUE;
}

int Page::AlignSystemsEnd(FunctorParams *functorParams)
{
    AlignSystemsParams *params = vrv_params_cast<AlignSystemsParams *>(functorParams);
    assert(params);

    m_drawingJustifiableHeight = params->m_shift;
    m_justificationSum = params->m_justificationSum;

    RunningElement *footer = this->GetFooter();
    if (footer) {
        m_drawingJustifiableHeight -= footer->GetTotalHeight(params->m_doc);

        // Move it up below the last system
        if (params->m_doc->GetOptions()->m_adjustPageHeight.GetValue()) {
            if (this->GetChildCount()) {
                System *last = vrv_cast<System *>(this->GetLast(SYSTEM));
                assert(last);
                const int unit = params->m_doc->GetDrawingUnit(100);
                const int topMargin = params->m_doc->GetOptions()->m_topMarginPgFooter.GetValue() * unit;
                footer->SetDrawingYRel(last->GetDrawingYRel() - last->GetHeight() - topMargin);
            }
        }
        else {
            footer->SetDrawingYRel(footer->GetContentHeight());
        }
    }

    return FUNCTOR_CONTINUE;
}

int Page::CastOffPagesEnd(FunctorParams *functorParams)
{
    CastOffPagesParams *params = vrv_params_cast<CastOffPagesParams *>(functorParams);
    assert(params);

    if (params->m_pendingPageElements.empty()) return FUNCTOR_CONTINUE;

    // Otherwise add all pendings objects
    ArrayOfObjects::iterator iter;
    for (iter = params->m_pendingPageElements.begin(); iter != params->m_pendingPageElements.end(); ++iter) {
        params->m_currentPage->AddChild(*iter);
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

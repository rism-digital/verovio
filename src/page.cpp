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
#include "adjustarticfunctor.h"
#include "adjustbeamsfunctor.h"
#include "adjustclefchangesfunctor.h"
#include "adjustdotsfunctor.h"
#include "adjustfloatingpositionerfunctor.h"
#include "adjustgracexposfunctor.h"
#include "adjustharmgrpsspacingfunctor.h"
#include "adjustlayersfunctor.h"
#include "adjustslursfunctor.h"
#include "adjuststaffoverlapfunctor.h"
#include "adjustsylspacingfunctor.h"
#include "adjusttempofunctor.h"
#include "adjusttupletsxfunctor.h"
#include "adjusttupletsyfunctor.h"
#include "adjustxoverflowfunctor.h"
#include "adjustxposfunctor.h"
#include "adjustxrelfortranscriptionfunctor.h"
#include "adjustyposfunctor.h"
#include "alignfunctor.h"
#include "bboxdevicecontext.h"
#include "cachehorizontallayoutfunctor.h"
#include "calcalignmentpitchposfunctor.h"
#include "calcalignmentxposfunctor.h"
#include "calcarticfunctor.h"
#include "calcbboxoverflowsfunctor.h"
#include "calcchordnoteheadsfunctor.h"
#include "calcdotsfunctor.h"
#include "calcledgerlinesfunctor.h"
#include "calcligaturenoteposfunctor.h"
#include "calcslurdirectionfunctor.h"
#include "calcspanningbeamspansfunctor.h"
#include "calcstemfunctor.h"
#include "comparison.h"
#include "doc.h"
#include "functor.h"
#include "justifyfunctor.h"
#include "libmei.h"
#include "miscfunctor.h"
#include "pageelement.h"
#include "pages.h"
#include "pgfoot.h"
#include "pghead.h"
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
        return m_score->GetScoreDef()->GetPgHead(PGFUNC_first);
    }
    else {
        return m_score->GetScoreDef()->GetPgHead(PGFUNC_all);
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
        return m_scoreEnd->GetScoreDef()->GetPgFoot(PGFUNC_first);
    }
    else {
        return m_scoreEnd->GetScoreDef()->GetPgFoot(PGFUNC_all);
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
    ResetVerticalAlignmentFunctor resetVerticalAlignment;
    this->Process(resetVerticalAlignment);

    // Align the content of the page using measure aligners
    // After this:
    // - each LayerElement object will have its Alignment pointer initialized
    AlignHorizontallyFunctor alignHorizontally(doc);
    this->Process(alignHorizontally);

    // Align the content of the page using system aligners
    // After this:
    // - each Staff object will then have its StaffAlignment pointer initialized
    AlignVerticallyFunctor alignVertically(doc);
    this->Process(alignVertically);

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

    AdjustXRelForTranscriptionFunctor adjustXRelForTranscription;
    this->Process(adjustXRelForTranscription);

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
    ResetVerticalAlignmentFunctor resetVerticalAlignment;
    this->Process(resetVerticalAlignment);

    // Align the content of the page using measure aligners
    // After this:
    // - each LayerElement object will have its Alignment pointer initialized
    AlignHorizontallyFunctor alignHorizontally(doc);
    this->Process(alignHorizontally);

    // Align the content of the page using system aligners
    // After this:
    // - each Staff object will then have its StaffAlignment pointer initialized
    AlignVerticallyFunctor alignVertically(doc);
    this->Process(alignVertically);

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
        CalcLigatureNotePosFunctor calcLigatureNotePos(doc);
        this->Process(calcLigatureNotePos);
    }

    CalcStemFunctor calcStem(doc);
    this->Process(calcStem);

    CalcChordNoteHeadsFunctor calcChordNoteHeads(doc);
    this->Process(calcChordNoteHeads);

    CalcDotsFunctor calcDots(doc);
    this->Process(calcDots);

    // Adjust the position of outside articulations
    CalcArticFunctor calcArtic(doc);
    this->Process(calcArtic);

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

    // Get the scoreDef at the beginning of the page
    ScoreDef *scoreDef = m_score->GetScoreDef();

    // Adjust the position of outside articulations
    AdjustArticFunctor adjustArtic(doc);
    this->Process(adjustArtic);

    // Adjust the x position of the LayerElement where multiple layers collide
    // Look at each LayerElement and change the m_xShift if the bounding box is overlapping
    // For the first iteration align elements without taking dots into consideration
    AdjustLayersFunctor adjustLayers(doc, scoreDef->GetStaffNs());
    this->Process(adjustLayers);

    // Adjust dots for the multiple layers. Try to align dots that can be grouped together when layers collide,
    // otherwise keep their relative positioning
    AdjustDotsFunctor adjustDots(doc, scoreDef->GetStaffNs());
    this->Process(adjustDots);

    // Adjust layers again, this time including dots positioning
    AdjustLayersFunctor adjustLayersWithDots(doc, scoreDef->GetStaffNs());
    adjustLayersWithDots.IgnoreDots(false);
    this->Process(adjustLayersWithDots);

    // Adjust the X position of the accidentals, including in chords
    AdjustAccidXFunctor adjustAccidX(doc);
    this->Process(adjustAccidX);

    // Adjust the X shift of the Alignment looking at the bounding boxes
    // Look at each LayerElement and change the m_xShift if the bounding box is overlapping
    AdjustXPosFunctor adjustXPos(doc, scoreDef->GetStaffNs());
    adjustXPos.SetExcluded({ TABDURSYM });
    this->Process(adjustXPos);

    // Adjust tabRhythm separately
    adjustXPos.ClearExcluded();
    adjustXPos.SetIncluded({ BARLINE, KEYSIG, METERSIG, TABDURSYM });
    adjustXPos.SetRightBarLinesOnly(true);
    this->Process(adjustXPos);

    // Adjust the X shift of the Alignment looking at the bounding boxes
    // Look at each LayerElement and change the m_xShift if the bounding box is overlapping
    AdjustGraceXPosFunctor adjustGraceXPos(doc, scoreDef->GetStaffNs());
    this->Process(adjustGraceXPos);

    // Adjust the spacing of clef changes since they are skipped in AdjustXPos
    // Look at each clef change and  move them to the left and add space if necessary
    AdjustClefChangesFunctor adjustClefChanges(doc);
    this->Process(adjustClefChanges);

    // We need to populate processing lists for processing the document by Layer (for matching @tie) and
    // by Verse (for matching syllable connectors)
    InitProcessingListsFunctor initProcessingLists;
    this->Process(initProcessingLists);

    this->AdjustSylSpacingByVerse(initProcessingLists.GetVerseTree(), doc);

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
    ResetVerticalAlignmentFunctor resetVerticalAlignment;
    this->Process(resetVerticalAlignment);

    CalcLedgerLinesFunctor calcLedgerLines(doc);
    this->Process(calcLedgerLines);

    // Align the content of the page using system aligners
    // After this:
    // - each Staff object will then have its StaffAlignment pointer initialized
    AlignVerticallyFunctor alignVertically(doc);
    this->Process(alignVertically);

    // Render it for filling the bounding box
    View view;
    BBoxDeviceContext bBoxDC(&view, 0, 0);
    view.SetDoc(doc);
    // Do not do the layout in this view - otherwise we will loop...
    view.SetPage(this->GetIdx(), false);
    view.DrawCurrentPage(&bBoxDC, false);

    // Adjust the position of outside articulations with slurs end and start positions
    AdjustArticWithSlursFunctor adjustArticWithSlurs(doc);
    this->Process(adjustArticWithSlurs);

    // Adjust the position of the beams in regards of layer elements
    AdjustBeamsFunctor adjustBeams(doc);
    this->Process(adjustBeams);

    // Adjust the position of the tuplets
    AdjustTupletsYFunctor adjustTupletsY(doc);
    this->Process(adjustTupletsY);

    // Adjust the position of the slurs
    AdjustSlursFunctor adjustSlurs(doc);
    this->Process(adjustSlurs);

    // At this point slurs must not be reinitialized, otherwise the adjustment we just did was in vain
    view.SetSlurHandling(SlurHandling::Drawing);
    view.SetPage(this->GetIdx(), false);
    view.DrawCurrentPage(&bBoxDC, false);

    // Adjust the position of tuplets by slurs
    AdjustTupletWithSlursFunctor adjustTupletWithSlurs(doc);
    this->Process(adjustTupletWithSlurs);

    // Fill the arrays of bounding boxes (above and below) for each staff alignment for which the box overflows.
    CalcBBoxOverflowsFunctor calcBBoxOverflows(doc);
    this->Process(calcBBoxOverflows);

    // Adjust the positioners of floating elements (slurs, hairpin, dynam, etc)
    AdjustFloatingPositionersFunctor adjustFloatingPositioners(doc);
    this->Process(adjustFloatingPositioners);

    // Adjust the overlap of the staff alignments by looking at the overflow bounding boxes
    AdjustStaffOverlapFunctor adjustStaffOverlap(doc);
    this->Process(adjustStaffOverlap);

    // Set the Y position of each StaffAlignment
    // Adjust the Y shift to make sure there is a minimal space (staffMargin) between each staff
    AdjustYPosFunctor adjustYPos(doc);
    this->Process(adjustYPos);

    // Adjust the positioners of floating elements placed between staves
    AdjustFloatingPositionersBetweenFunctor adjustFloatingPositionersBetween(doc);
    this->Process(adjustFloatingPositionersBetween);

    AdjustCrossStaffYPosFunctor adjustCrossStaffYPos(doc);
    this->Process(adjustCrossStaffYPos);

    // Redraw are re-adjust the position of the slurs when we have cross-staff ones
    if (adjustSlurs.HasCrossStaffSlurs()) {
        view.SetSlurHandling(SlurHandling::Initialize);
        view.SetPage(this->GetIdx(), false);
        view.DrawCurrentPage(&bBoxDC, false);
        this->Process(adjustSlurs);
    }

    if (this->GetHeader()) {
        this->GetHeader()->AdjustRunningElementYPos();
    }

    if (this->GetFooter()) {
        this->GetFooter()->AdjustRunningElementYPos();
    }

    // Adjust system Y position
    AlignSystemsFunctor alignSystems(doc);
    alignSystems.SetShift(doc->m_drawingPageContentHeight);
    alignSystems.SetSystemSpacing(doc->GetOptions()->m_spacingSystem.GetValue() * doc->GetDrawingUnit(100));
    this->Process(alignSystems);
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
        JustifyXFunctor justifyX(doc);
        justifyX.SetSystemFullWidth(doc->m_drawingPageContentWidth);
        this->Process(justifyX);
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
    JustifyYFunctor justifyY(doc);
    justifyY.SetJustificationSum(m_justificationSum);
    justifyY.SetSpaceToDistribute(m_drawingJustifiableHeight);
    this->Process(justifyY);

    if (!justifyY.GetShiftForStaff().empty()) {
        // Adjust cross staff content which is displaced through vertical justification
        JustifyYAdjustCrossStaffFunctor justifyYAdjustCrossStaff(doc);
        justifyYAdjustCrossStaff.SetShiftForStaff(justifyY.GetShiftForStaff());
        this->Process(justifyYAdjustCrossStaff);
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

void Page::AdjustSylSpacingByVerse(const IntTree &verseTree, Doc *doc)
{
    IntTree_t::const_iterator staves;
    IntTree_t::const_iterator layers;
    IntTree_t::const_iterator verses;

    if (verseTree.child.empty()) return;

    Filters filters;

    // Same for the lyrics, but Verse by Verse since Syl are TimeSpanningInterface elements for handling connectors
    for (staves = verseTree.child.begin(); staves != verseTree.child.end(); ++staves) {
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

FunctorCode Page::Accept(Functor &functor)
{
    return functor.VisitPage(this);
}

FunctorCode Page::Accept(ConstFunctor &functor) const
{
    return functor.VisitPage(this);
}

FunctorCode Page::AcceptEnd(Functor &functor)
{
    return functor.VisitPageEnd(this);
}

FunctorCode Page::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitPageEnd(this);
}

} // namespace vrv

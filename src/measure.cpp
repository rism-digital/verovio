/////////////////////////////////////////////////////////////////////////////
// Name:        measure.cpp
// Author:      Laurent Pugin
// Created:     2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "measure.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "controlelement.h"
#include "doc.h"
#include "editorial.h"
#include "ending.h"
#include "f.h"
#include "functorparams.h"
#include "hairpin.h"
#include "harm.h"
#include "multirest.h"
#include "page.h"
#include "pages.h"
#include "pedal.h"
#include "section.h"
#include "staff.h"
#include "staffdef.h"
#include "syl.h"
#include "system.h"
#include "systemmilestone.h"
#include "tempo.h"
#include "tie.h"
#include "timeinterface.h"
#include "timemap.h"
#include "timestamp.h"
#include "vrv.h"

//----------------------------------------------------------------------------

#include "MidiFile.h"

namespace vrv {

//----------------------------------------------------------------------------
// Measure
//----------------------------------------------------------------------------

static const ClassRegistrar<Measure> s_factory("measure", MEASURE);

Measure::Measure(bool measureMusic, int logMeasureNb)
    : Object(MEASURE, "measure-")
    , AttBarring()
    , AttCoordX1()
    , AttCoordX2()
    , AttMeasureLog()
    , AttMeterConformanceBar()
    , AttNNumberLike()
    , AttPointing()
    , AttTyped()
{
    this->RegisterAttClass(ATT_BARRING);
    this->RegisterAttClass(ATT_COORDX1);
    this->RegisterAttClass(ATT_COORDX2);
    this->RegisterAttClass(ATT_MEASURELOG);
    this->RegisterAttClass(ATT_METERCONFORMANCEBAR);
    this->RegisterAttClass(ATT_NNUMBERLIKE);
    this->RegisterAttClass(ATT_POINTING);
    this->RegisterAttClass(ATT_TYPED);

    m_measuredMusic = measureMusic;
    // We set parent to it because we want to access the parent doc from the aligners
    m_measureAligner.SetParent(this);
    // Idem for timestamps
    m_timestampAligner.SetParent(this);
    // Idem for barlines
    m_leftBarLine.SetParent(this);
    m_rightBarLine.SetParent(this);

    // owned pointers need to be set to NULL;
    m_drawingScoreDef = NULL;

    // Set the barline positions
    m_leftBarLine.SetPosition(BarLinePosition::Left);
    m_rightBarLine.SetPosition(BarLinePosition::Right);

    this->Reset();

    if (!measureMusic) this->SetRight(BARRENDITION_invis);
}

Measure::~Measure()
{
    // We need to delete own objects
    this->Reset();
}

void Measure::CloneReset()
{
    Object::CloneReset();

    m_measureAligner.Reset();
    m_measureAligner.SetParent(this);
    // Idem for timestamps
    m_timestampAligner.SetParent(this);
    // Idem for barlines
    m_leftBarLine.SetParent(this);
    m_rightBarLine.SetParent(this);

    // owned pointers need to be set to NULL;
    m_drawingScoreDef = NULL;
}

void Measure::Reset()
{
    Object::Reset();
    this->ResetCoordX1();
    this->ResetCoordX2();
    this->ResetMeasureLog();
    this->ResetMeterConformanceBar();
    this->ResetNNumberLike();
    this->ResetPointing();
    this->ResetTyped();

    if (m_drawingScoreDef) {
        delete m_drawingScoreDef;
        m_drawingScoreDef = NULL;
    }

    m_timestampAligner.Reset();
    m_xAbs = VRV_UNSET;
    m_xAbs2 = VRV_UNSET;
    m_drawingXRel = 0;

    m_cachedXRel = VRV_UNSET;
    m_cachedOverflow = VRV_UNSET;
    m_cachedWidth = VRV_UNSET;

    // by default, we have a single barLine on the right (none on the left)
    m_rightBarLine.SetForm(this->GetRight());
    m_leftBarLine.SetForm(this->GetLeft());

    if (!m_measuredMusic) {
        m_xAbs = VRV_UNSET;
        m_xAbs2 = VRV_UNSET;
    }

    m_drawingEnding = NULL;
    m_hasAlignmentRefWithMultipleLayers = false;

    m_scoreTimeOffset.clear();
    m_realTimeOffsetMilliseconds.clear();
    m_currentTempo = MIDI_TEMPO;
}

bool Measure::IsSupportedChild(Object *child)
{
    if (child->IsControlElement()) {
        assert(dynamic_cast<ControlElement *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else if (child->Is(STAFF)) {
        Staff *staff = vrv_cast<Staff *>(child);
        assert(staff);
        if (staff && (staff->GetN() < 1)) {
            // This is not 100% safe if we have a <app> and <rdg> with more than
            // one staff as a previous child.
            staff->SetN(this->GetChildCount());
        }
    }
    else {
        return false;
    }
    return true;
}

void Measure::AddChildBack(Object *child)
{
    if (!this->IsSupportedChild(child)) {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        return;
    }

    child->SetParent(this);
    ArrayOfObjects &children = this->GetChildrenForModification();
    if (children.empty()) {
        children.push_back(child);
    }
    else if (children.back()->Is(STAFF)) {
        children.push_back(child);
    }
    else {
        for (auto it = children.begin(); it != children.end(); ++it) {
            if (!(*it)->Is(STAFF)) {
                children.insert(it, child);
                break;
            }
        }
    }
    Modify();
}

int Measure::GetDrawingX() const
{
    if (!this->IsMeasuredMusic()) {
        const System *system = vrv_cast<const System *>(this->GetFirstAncestor(SYSTEM));
        assert(system);
        if (system->m_yAbs != VRV_UNSET) {
            return (system->m_systemLeftMar);
        }
    }

    if (m_xAbs != VRV_UNSET) return m_xAbs;

    if (m_cachedDrawingX != VRV_UNSET) return m_cachedDrawingX;

    const System *system = vrv_cast<const System *>(this->GetFirstAncestor(SYSTEM));
    assert(system);
    m_cachedDrawingX = system->GetDrawingX() + this->GetDrawingXRel();
    return m_cachedDrawingX;
}

void Measure::ResetCachedDrawingX() const
{
    Object::ResetCachedDrawingX();
    m_timestampAligner.ResetCachedDrawingX();
}

void Measure::SetDrawingXRel(int drawingXRel)
{
    this->ResetCachedDrawingX();
    m_drawingXRel = drawingXRel;
}

bool Measure::IsFirstInSystem() const
{
    assert(this->GetParent());
    return (this->GetParent()->GetFirst(MEASURE) == this);
}

bool Measure::IsLastInSystem() const
{
    assert(this->GetParent());
    return (this->GetParent()->GetLast(MEASURE) == this);
}

int Measure::GetLeftBarLineXRel() const
{
    if (m_measureAligner.GetLeftBarLineAlignment()) {
        return m_measureAligner.GetLeftBarLineAlignment()->GetXRel();
    }
    return 0;
}

int Measure::GetLeftBarLineLeft() const
{
    int x = this->GetLeftBarLineXRel();
    if (m_leftBarLine.HasSelfBB()) {
        x += m_leftBarLine.GetContentX1();
    }
    return x;
}

int Measure::GetLeftBarLineRight() const
{
    int x = this->GetLeftBarLineXRel();
    if (m_leftBarLine.HasSelfBB()) {
        x += m_leftBarLine.GetContentX2();
    }
    return x;
}

int Measure::GetRightBarLineXRel() const
{
    if (m_measureAligner.GetRightBarLineAlignment()) {
        return m_measureAligner.GetRightBarLineAlignment()->GetXRel();
    }
    return 0;
}

int Measure::GetRightBarLineWidth(Doc *doc)
{
    const BarLine *barline = this->GetRightBarLine();
    if (!barline) return 0;

    const int staffSize = 100;
    const int barLineWidth = doc->GetDrawingBarLineWidth(staffSize);
    const int barLineThickWidth
        = doc->GetDrawingUnit(staffSize) * doc->GetOptions()->m_thickBarlineThickness.GetValue();
    const int barLineSeparation = doc->GetDrawingUnit(staffSize) * doc->GetOptions()->m_barLineSeparation.GetValue();

    int width = 0;
    switch (barline->GetForm()) {
        case BARRENDITION_dbl:
        case BARRENDITION_dbldashed: {
            width = barLineSeparation + barLineWidth / 2;
            break;
        }
        case BARRENDITION_rptend:
        case BARRENDITION_end: {
            width = barLineSeparation + barLineWidth + barLineThickWidth / 2;
            break;
        }
        case BARRENDITION_rptboth: {
            width = 2 * barLineSeparation + barLineWidth / 2 + barLineThickWidth;
            break;
        }
        default: break;
    }
    return width;
}

int Measure::GetRightBarLineLeft() const
{
    int x = this->GetRightBarLineXRel();
    if (m_rightBarLine.HasSelfBB()) {
        x += m_rightBarLine.GetContentX1();
    }
    return x;
}

int Measure::GetRightBarLineRight() const
{
    int x = this->GetRightBarLineXRel();
    if (m_rightBarLine.HasSelfBB()) {
        x += m_rightBarLine.GetContentX2();
    }
    return x;
}

int Measure::GetWidth() const
{
    if (!this->IsMeasuredMusic()) {
        const System *system = vrv_cast<const System *>(this->GetFirstAncestor(SYSTEM));
        assert(system);
        if (system->m_yAbs != VRV_UNSET) {
            const Page *page = vrv_cast<const Page *>(system->GetFirstAncestor(PAGE));
            assert(page);
            // xAbs2 =  page->m_pageWidth - system->m_systemRightMar;
            return page->m_pageWidth - system->m_systemLeftMar - system->m_systemRightMar;
        }
    }

    if (m_xAbs2 != VRV_UNSET) return (m_xAbs2 - m_xAbs);

    assert(m_measureAligner.GetRightAlignment());
    return m_measureAligner.GetRightAlignment()->GetXRel();
}

int Measure::GetInnerWidth() const
{
    return (this->GetRightBarLineLeft() - this->GetLeftBarLineRight());
}

int Measure::GetInnerCenterX() const
{
    return (this->GetDrawingX() + this->GetLeftBarLineRight() + this->GetInnerWidth() / 2);
}

int Measure::GetDrawingOverflow()
{
    Functor adjustXOverlfow(&Object::AdjustXOverflow);
    Functor adjustXOverlfowEnd(&Object::AdjustXOverflowEnd);
    AdjustXOverflowParams adjustXOverflowParams(0);
    adjustXOverflowParams.m_currentSystem = vrv_cast<System *>(this->GetFirstAncestor(SYSTEM));
    assert(adjustXOverflowParams.m_currentSystem);
    adjustXOverflowParams.m_lastMeasure = this;
    this->Process(&adjustXOverlfow, &adjustXOverflowParams, &adjustXOverlfowEnd);
    if (!adjustXOverflowParams.m_currentWidest) return 0;

    int measureRightX = this->GetDrawingX() + this->GetWidth();
    int overflow = adjustXOverflowParams.m_currentWidest->GetContentRight() - measureRightX;
    return std::max(0, overflow);
}

int Measure::GetSectionRestartShift(Doc *doc) const
{
    if (this->IsFirstInSystem()) {
        return 0;
    }
    else {
        return 5 * doc->GetDrawingDoubleUnit(100);
    }
}

void Measure::SetDrawingScoreDef(ScoreDef *drawingScoreDef)
{
    assert(!m_drawingScoreDef); // We should always call UnscoreDefSetCurrent before

    m_drawingScoreDef = new ScoreDef();
    *m_drawingScoreDef = *drawingScoreDef;
}

std::vector<Staff *> Measure::GetFirstStaffGrpStaves(ScoreDef *scoreDef)
{
    assert(scoreDef);

    std::vector<Staff *> staves;
    std::vector<int>::iterator iter;
    std::vector<int> staffList;

    // First get all the staffGrps
    ListOfObjects staffGrps = scoreDef->FindAllDescendantsByType(STAFFGRP);

    // Then the @n of each first staffDef
    for (auto &staffGrp : staffGrps) {
        StaffDef *staffDef = dynamic_cast<StaffDef *>((staffGrp)->GetFirst(STAFFDEF));
        if (staffDef) staffList.push_back(staffDef->GetN());
    }

    // Get the corresponding staves in the measure
    for (iter = staffList.begin(); iter != staffList.end(); ++iter) {
        AttNIntegerComparison matchN(STAFF, *iter);
        Staff *staff = dynamic_cast<Staff *>(this->FindDescendantByComparison(&matchN, 1));
        if (!staff) {
            // LogDebug("Staff with @n '%d' not found in measure '%s'", *iter, measure->GetUuid().c_str());
            continue;
        }
        staves.push_back(staff);
    }
    if (staves.empty()) LogDebug("Empty @staff array");
    return staves;
}

Staff *Measure::GetTopVisibleStaff()
{
    Staff *staff = NULL;
    ListOfObjects staves = this->FindAllDescendantsByType(STAFF, false);
    for (auto &child : staves) {
        staff = vrv_cast<Staff *>(child);
        assert(staff);
        if (staff->DrawingIsVisible()) {
            break;
        }
        staff = NULL;
    }
    return staff;
}

Staff *Measure::GetBottomVisibleStaff()
{
    Staff *bottomStaff = NULL;
    ListOfObjects staves = this->FindAllDescendantsByType(STAFF, false);
    for (const auto child : staves) {
        Staff *staff = vrv_cast<Staff *>(child);
        assert(staff);
        if (!staff->DrawingIsVisible()) {
            continue;
        }
        bottomStaff = staff;
    }
    return bottomStaff;
}

int Measure::EnclosesTime(int time) const
{
    int repeat = 1;
    double timeDuration
        = m_measureAligner.GetRightAlignment()->GetTime() * DURATION_4 / DUR_MAX * 60.0 / m_currentTempo * 1000.0 + 0.5;
    std::vector<double>::const_iterator iter;
    for (iter = m_realTimeOffsetMilliseconds.begin(); iter != m_realTimeOffsetMilliseconds.end(); ++iter) {
        if ((time >= *iter) && (time <= *iter + timeDuration)) return repeat;
        repeat++;
    }
    return 0;
}

double Measure::GetRealTimeOffsetMilliseconds(int repeat) const
{
    if ((repeat < 1) || repeat > (int)m_realTimeOffsetMilliseconds.size()) return 0;
    return m_realTimeOffsetMilliseconds.at(repeat - 1);
}

data_BARRENDITION Measure::GetDrawingLeftBarLineByStaffN(int staffN) const
{
    auto elementIter = m_invisibleStaffBarlines.find(staffN);
    if (elementIter != m_invisibleStaffBarlines.end()) return elementIter->second.first;
    return this->GetDrawingLeftBarLine();
}

data_BARRENDITION Measure::GetDrawingRightBarLineByStaffN(int staffN) const
{
    auto elementIter = m_invisibleStaffBarlines.find(staffN);
    if (elementIter != m_invisibleStaffBarlines.end()) return elementIter->second.second;
    return this->GetDrawingRightBarLine();
}

Measure::BarlineRenditionPair Measure::SelectDrawingBarLines(Measure *previous)
{
    // Barlines are stored in the map in the following format:
    // previous measure right -> current measure left -> expected barlines (previous, current)
    static std::map<data_BARRENDITION, std::map<data_BARRENDITION, BarlineRenditionPair>> drawingLines = {
        // previous right barline is dotted
        { BARRENDITION_dotted,
            { { BARRENDITION_dotted, { BARRENDITION_dotted, BARRENDITION_NONE } },
                { BARRENDITION_dashed, { BARRENDITION_dashed, BARRENDITION_NONE } },
                { BARRENDITION_single, { BARRENDITION_single, BARRENDITION_NONE } },
                { BARRENDITION_dbldotted, { BARRENDITION_dbldotted, BARRENDITION_NONE } },
                { BARRENDITION_dbldashed, { BARRENDITION_dbldashed, BARRENDITION_NONE } },
                { BARRENDITION_dbl, { BARRENDITION_dbl, BARRENDITION_NONE } } } },
        // previous right barline is dashed
        { BARRENDITION_dashed,
            { { BARRENDITION_dotted, { BARRENDITION_dashed, BARRENDITION_NONE } },
                { BARRENDITION_dashed, { BARRENDITION_dashed, BARRENDITION_NONE } },
                { BARRENDITION_single, { BARRENDITION_single, BARRENDITION_NONE } },
                { BARRENDITION_dbldotted, { BARRENDITION_dashed, BARRENDITION_dotted } },
                { BARRENDITION_dbldashed, { BARRENDITION_dbldashed, BARRENDITION_NONE } },
                { BARRENDITION_dbl, { BARRENDITION_dbl, BARRENDITION_NONE } } } },
        // previous right barline is single
        { BARRENDITION_single,
            { { BARRENDITION_dotted, { BARRENDITION_single, BARRENDITION_NONE } },
                { BARRENDITION_dashed, { BARRENDITION_single, BARRENDITION_NONE } },
                { BARRENDITION_single, { BARRENDITION_single, BARRENDITION_NONE } },
                { BARRENDITION_dbldotted, { BARRENDITION_single, BARRENDITION_dotted } },
                { BARRENDITION_dbldashed, { BARRENDITION_single, BARRENDITION_dashed } },
                { BARRENDITION_dbl, { BARRENDITION_dbl, BARRENDITION_NONE } } } },
        // previous right barline is double dotted
        { BARRENDITION_dbldotted,
            { { BARRENDITION_dotted, { BARRENDITION_dbldotted, BARRENDITION_NONE } },
                { BARRENDITION_dashed, { BARRENDITION_dotted, BARRENDITION_dashed } },
                { BARRENDITION_single, { BARRENDITION_dotted, BARRENDITION_single } },
                { BARRENDITION_dbldotted, { BARRENDITION_dbldotted, BARRENDITION_NONE } },
                { BARRENDITION_dbldashed, { BARRENDITION_dbldashed, BARRENDITION_NONE } },
                { BARRENDITION_dbl, { BARRENDITION_dbl, BARRENDITION_NONE } } } },
        // previous right barline is double dashed
        { BARRENDITION_dbldashed,
            { { BARRENDITION_dotted, { BARRENDITION_dbldashed, BARRENDITION_NONE } },
                { BARRENDITION_dashed, { BARRENDITION_dbldashed, BARRENDITION_NONE } },
                { BARRENDITION_single, { BARRENDITION_dashed, BARRENDITION_single } },
                { BARRENDITION_dbldotted, { BARRENDITION_dbldashed, BARRENDITION_NONE } },
                { BARRENDITION_dbldashed, { BARRENDITION_dbldashed, BARRENDITION_NONE } },
                { BARRENDITION_dbl, { BARRENDITION_dbl, BARRENDITION_NONE } } } },
        // previous right barline is double
        { BARRENDITION_dbl,
            { { BARRENDITION_dotted, { BARRENDITION_dbl, BARRENDITION_NONE } },
                { BARRENDITION_dashed, { BARRENDITION_dbl, BARRENDITION_NONE } },
                { BARRENDITION_single, { BARRENDITION_dbl, BARRENDITION_NONE } },
                { BARRENDITION_dbldotted, { BARRENDITION_dbl, BARRENDITION_NONE } },
                { BARRENDITION_dbldashed, { BARRENDITION_dbl, BARRENDITION_NONE } },
                { BARRENDITION_dbl, { BARRENDITION_dbl, BARRENDITION_NONE } } } },
    };

    const BarlineRenditionPair defaultValue = { previous->GetRight(), this->GetLeft() };
    auto previousRight = drawingLines.find(previous->GetRight());
    if (previousRight == drawingLines.end()) return defaultValue;

    auto currentLeft = previousRight->second.find(this->GetLeft());
    if (currentLeft == previousRight->second.end()) return defaultValue;

    return currentLeft->second;
}

void Measure::SetDrawingBarLines(Measure *previous, int barlineDrawingFlags)
{
    // First set the right barline. If none then set a single one.
    data_BARRENDITION rightBarline = (this->HasRight()) ? this->GetRight() : BARRENDITION_single;
    this->SetDrawingRightBarLine(rightBarline);

    // Now adjust the right barline of the previous measure (if any) and the left one
    if (!previous) {
        this->SetDrawingLeftBarLine(this->GetLeft());
    }
    else if (barlineDrawingFlags & BarlineDrawingFlags::SYSTEM_BREAK) {
        // we have rptboth on one of the two sides, split them (ignore any other value)
        if ((previous->GetRight() == BARRENDITION_rptboth) || (this->GetLeft() == BARRENDITION_rptboth)) {
            previous->SetDrawingRightBarLine(BARRENDITION_rptend);
            this->SetDrawingLeftBarLine(BARRENDITION_rptstart);
        }
        // nothing to do with any other value?
        else {
            this->SetDrawingLeftBarLine(this->GetLeft());
        }
    }
    else if (!((barlineDrawingFlags & BarlineDrawingFlags::SCORE_DEF_INSERT)
                 || (barlineDrawingFlags & BarlineDrawingFlags::INVISIBLE_MEASURE_CURRENT)
                 || (barlineDrawingFlags & BarlineDrawingFlags::INVISIBLE_MEASURE_PREVIOUS))) {
        // we have rptboth split in the two measures, make them one rptboth
        if ((previous->GetRight() == BARRENDITION_rptend) && (this->GetLeft() == BARRENDITION_rptstart)) {
            previous->SetDrawingRightBarLine(BARRENDITION_rptboth);
            this->SetDrawingLeftBarLine(BARRENDITION_NONE);
        }
        // we have an rptend before, make sure there in none on the left (ignore any other value)
        else if (previous->GetRight() == BARRENDITION_rptend) {
            this->SetDrawingLeftBarLine(BARRENDITION_NONE);
        }
        // we have an rptstart coming, make sure there is none on the right before (ignore any other value)
        else if (this->GetLeft() == BARRENDITION_rptstart) {
            // always set the right barline to invis for spacing
            previous->SetDrawingRightBarLine(BARRENDITION_invis);
            this->SetDrawingLeftBarLine(BARRENDITION_rptstart);
        }
        // we have an rptboth coming, make sure there is none on the right before (ignore any other value)
        else if (this->GetLeft() == BARRENDITION_rptboth) {
            // always set the right barline to invis for spacing
            previous->SetDrawingRightBarLine(BARRENDITION_invis);
            this->SetDrawingLeftBarLine(BARRENDITION_rptboth);
        }
        // handle other possible barline interactions
        else {
            auto [right, left] = SelectDrawingBarLines(previous);
            if (right != left) {
                previous->SetDrawingRightBarLine(right);
                this->SetDrawingLeftBarLine(left);
                if (this->HasInvisibleStaffBarlines()) this->GetLeftBarLine()->SetPosition(BarLinePosition::None);
            }
        }
    }
    else {
        if ((barlineDrawingFlags & BarlineDrawingFlags::INVISIBLE_MEASURE_PREVIOUS)
            && !(barlineDrawingFlags & BarlineDrawingFlags::INVISIBLE_MEASURE_CURRENT)
            && !(barlineDrawingFlags & BarlineDrawingFlags::SCORE_DEF_INSERT)) {
            if (this->GetLeft() == BARRENDITION_NONE) {
                this->SetLeft(BARRENDITION_single);
            }
            this->GetLeftBarLine()->SetPosition(BarLinePosition::None);
        }
        // with a scoredef inbetween always set it to what we have in the encoding
        this->SetDrawingLeftBarLine(this->GetLeft());
    }
}

void Measure::SetInvisibleStaffBarlines(
    Measure *previous, ListOfObjects &currentInvisible, ListOfObjects &previousInvisible, int barlineDrawingFlags)
{
    if (!previous) return;

    // Process invisible staves in the current measure and set right barline values for previous measure
    for (const auto object : currentInvisible) {
        Staff *staff = vrv_cast<Staff *>(object);
        assert(staff);
        data_BARRENDITION right = previous->GetRight();
        if (right == BARRENDITION_NONE) right = BARRENDITION_single;
        auto [iter, result]
            = previous->m_invisibleStaffBarlines.insert({ staff->GetN(), { BARRENDITION_NONE, right } });
        if (!result) iter->second.second = right;
    }
    // Then process invisible staves in the previous measure and set left barline values in the current measure
    for (const auto object : previousInvisible) {
        Staff *staff = vrv_cast<Staff *>(object);
        assert(staff);
        data_BARRENDITION left = this->GetLeft();
        if ((left == BARRENDITION_NONE) && !(barlineDrawingFlags & BarlineDrawingFlags::SCORE_DEF_INSERT))
            left = BARRENDITION_single;
        auto [iter, result] = m_invisibleStaffBarlines.insert({ staff->GetN(), { left, BARRENDITION_NONE } });
        if (!result) iter->second.first = left;
    }
}

std::vector<std::pair<LayerElement *, LayerElement *>> Measure::GetInternalTieEndpoints()
{
    ListOfObjects children = this->FindAllDescendantsByType(TIE);

    std::vector<std::pair<LayerElement *, LayerElement *>> endpoints;
    for (Object *object : children) {
        Tie *tie = vrv_cast<Tie *>(object);
        // If both start and end points of the tie are not within current measure - skip it
        LayerElement *start = tie->GetStart();
        if (!start || (start->GetFirstAncestor(MEASURE) != this)) continue;
        LayerElement *end = tie->GetEnd();
        if (!end || (end->GetFirstAncestor(MEASURE) != this)) continue;
        endpoints.emplace_back(start, end);
    }

    return endpoints;
}

//----------------------------------------------------------------------------
// Measure functor methods
//----------------------------------------------------------------------------

int Measure::FindSpannedLayerElements(FunctorParams *functorParams)
{
    FindSpannedLayerElementsParams *params = vrv_params_cast<FindSpannedLayerElementsParams *>(functorParams);
    assert(params);

    if (params->m_interface->GetStartMeasure() == this) {
        params->m_inMeasureRange = true;
    }

    if (!params->m_inMeasureRange) {
        return FUNCTOR_SIBLINGS;
    }

    return FUNCTOR_CONTINUE;
}

int Measure::FindSpannedLayerElementsEnd(FunctorParams *functorParams)
{
    FindSpannedLayerElementsParams *params = vrv_params_cast<FindSpannedLayerElementsParams *>(functorParams);
    assert(params);

    if (params->m_interface->GetEndMeasure() == this) {
        params->m_inMeasureRange = false;
    }

    return FUNCTOR_CONTINUE;
}

int Measure::ConvertMarkupAnalyticalEnd(FunctorParams *functorParams)
{
    ConvertMarkupAnalyticalParams *params = vrv_params_cast<ConvertMarkupAnalyticalParams *>(functorParams);
    assert(params);

    ArrayOfObjects::iterator iter;
    for (iter = params->m_controlEvents.begin(); iter != params->m_controlEvents.end(); ++iter) {
        this->AddChild(*iter);
    }

    params->m_controlEvents.clear();

    return FUNCTOR_CONTINUE;
}

int Measure::ConvertToPageBased(FunctorParams *functorParams)
{
    ConvertToPageBasedParams *params = vrv_params_cast<ConvertToPageBasedParams *>(functorParams);
    assert(params);

    // Move itself to the pageBasedSystem - do not process children
    assert(params->m_currentSystem);
    this->MoveItselfTo(params->m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

int Measure::ConvertToCastOffMensural(FunctorParams *functorParams)
{
    ConvertToCastOffMensuralParams *params = vrv_params_cast<ConvertToCastOffMensuralParams *>(functorParams);
    assert(params);

    // We are processing by staff/layer from the call below - we obviously do not want to loop...
    if (params->m_targetMeasure) {
        return FUNCTOR_CONTINUE;
    }

    bool convertToMeasured = params->m_doc->GetOptions()->m_mensuralToMeasure.GetValue();

    assert(params->m_targetSystem);
    assert(params->m_layerTree);

    // Create a temporary subsystem for receiving the measure segments
    System targetSubSystem;
    params->m_targetSubSystem = &targetSubSystem;

    // Create the first measure segment - problem: we are dropping the section element - we should create a score-based
    // MEI file instead
    Measure *measure = new Measure(convertToMeasured);
    if (convertToMeasured) {
        measure->SetN(StringFormat("%d", params->m_segmentTotal + 1));
    }
    params->m_targetSubSystem->AddChild(measure);

    ArrayOfComparisons filters;
    // Now we can process by layer and move their content to (measure) segments
    for (auto const &staves : params->m_layerTree->child) {
        for (auto const &layers : staves.second.child) {
            // Create ad comparison object for each type / @n
            AttNIntegerComparison matchStaff(STAFF, staves.first);
            AttNIntegerComparison matchLayer(LAYER, layers.first);
            filters = { &matchStaff, &matchLayer };

            params->m_segmentIdx = 1;
            params->m_targetMeasure = measure;

            Functor convertToCastOffMensural(&Object::ConvertToCastOffMensural);
            this->Process(&convertToCastOffMensural, params, NULL, &filters);
        }
    }

    params->m_targetMeasure = NULL;
    params->m_targetSubSystem = NULL;
    params->m_segmentTotal = targetSubSystem.GetChildCount();
    // Copy the measure segments to the final target segment
    params->m_targetSystem->MoveChildrenFrom(&targetSubSystem);

    return FUNCTOR_SIBLINGS;
}

int Measure::ConvertToUnCastOffMensural(FunctorParams *functorParams)
{
    ConvertToUnCastOffMensuralParams *params = vrv_params_cast<ConvertToUnCastOffMensuralParams *>(functorParams);
    assert(params);

    if (params->m_contentMeasure == NULL) {
        params->m_contentMeasure = this;
    }
    else if (params->m_addSegmentsToDelete) {
        params->m_segmentsToDelete.push_back(this);
    }

    return FUNCTOR_CONTINUE;
}

int Measure::Save(FunctorParams *functorParams)
{
    if (this->IsMeasuredMusic())
        return Object::Save(functorParams);
    else
        return FUNCTOR_CONTINUE;
}

int Measure::SaveEnd(FunctorParams *functorParams)
{
    if (this->IsMeasuredMusic())
        return Object::SaveEnd(functorParams);
    else
        return FUNCTOR_CONTINUE;
}

int Measure::ScoreDefUnsetCurrent(FunctorParams *functorParams)
{
    ScoreDefUnsetCurrentParams *params = vrv_params_cast<ScoreDefUnsetCurrentParams *>(functorParams);
    assert(params);

    if (m_drawingScoreDef) {
        delete m_drawingScoreDef;
        m_drawingScoreDef = NULL;
    }

    // We also need to remove scoreDef elements in the AlignmentReference objects
    m_measureAligner.Process(params->m_functor, params);

    return FUNCTOR_CONTINUE;
}

int Measure::ScoreDefOptimize(FunctorParams *functorParams)
{
    ScoreDefOptimizeParams *params = vrv_params_cast<ScoreDefOptimizeParams *>(functorParams);
    assert(params);

    if (!params->m_doc->GetOptions()->m_condenseTempoPages.GetValue()) {
        return FUNCTOR_CONTINUE;
    }

    params->m_hasFermata = (this->FindDescendantByType(FERMATA));
    params->m_hasTempo = (this->FindDescendantByType(TEMPO));

    return FUNCTOR_CONTINUE;
}

int Measure::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    this->SetDrawingXRel(0);
    if (m_measureAligner.GetLeftAlignment()) {
        m_measureAligner.GetLeftAlignment()->SetXRel(0);
    }
    if (m_measureAligner.GetRightAlignment()) {
        m_measureAligner.GetRightAlignment()->SetXRel(0);
    }

    Functor resetHorizontalAlignment(&Object::ResetHorizontalAlignment);
    m_timestampAligner.Process(&resetHorizontalAlignment, NULL);

    m_hasAlignmentRefWithMultipleLayers = false;

    return FUNCTOR_CONTINUE;
}

int Measure::ApplyPPUFactor(FunctorParams *functorParams)
{
    ApplyPPUFactorParams *params = vrv_params_cast<ApplyPPUFactorParams *>(functorParams);
    assert(params);

    if (m_xAbs != VRV_UNSET) m_xAbs /= params->m_page->GetPPUFactor();
    if (m_xAbs2 != VRV_UNSET) m_xAbs2 /= params->m_page->GetPPUFactor();

    return FUNCTOR_CONTINUE;
}

int Measure::AlignHorizontally(FunctorParams *functorParams)
{
    AlignHorizontallyParams *params = vrv_params_cast<AlignHorizontallyParams *>(functorParams);
    assert(params);

    // clear the content of the measureAligner
    m_measureAligner.Reset();

    // point to it
    params->m_measureAligner = &m_measureAligner;
    params->m_hasMultipleLayer = false;

    if (m_leftBarLine.SetAlignment(m_measureAligner.GetLeftBarLineAlignment())) params->m_hasMultipleLayer = true;
    if (m_rightBarLine.SetAlignment(m_measureAligner.GetRightBarLineAlignment())) params->m_hasMultipleLayer = true;

    assert(params->m_measureAligner);

    return FUNCTOR_CONTINUE;
}

int Measure::AlignHorizontallyEnd(FunctorParams *functorParams)
{
    AlignHorizontallyParams *params = vrv_params_cast<AlignHorizontallyParams *>(functorParams);
    assert(params);

    int meterUnit = (params->m_currentMeterSig) ? params->m_currentMeterSig->GetUnit() : 4;
    m_measureAligner.SetInitialTstamp(meterUnit);

    // We also need to align the timestamps - we do it at the end since we need the *meterSig to be initialized by a
    // Layer. Obviously this will not work with different time signature. However, I am not sure how this would work
    // in MEI anyway.
    m_timestampAligner.Process(params->m_functor, params);

    // Next scoreDef will be INTERMEDIATE_SCOREDEF (See Layer::AlignHorizontally)
    params->m_isFirstMeasure = false;

    if (params->m_hasMultipleLayer) m_hasAlignmentRefWithMultipleLayers = true;

    return FUNCTOR_CONTINUE;
}

int Measure::AlignVertically(FunctorParams *functorParams)
{
    AlignVerticallyParams *params = vrv_params_cast<AlignVerticallyParams *>(functorParams);
    assert(params);

    // we also need to reset the staffNb
    params->m_staffIdx = 0;

    return FUNCTOR_CONTINUE;
}

int Measure::AdjustArpegEnd(FunctorParams *functorParams)
{
    AdjustArpegParams *params = vrv_params_cast<AdjustArpegParams *>(functorParams);
    assert(params);

    if (!params->m_alignmentArpegTuples.empty()) {
        params->m_measureAligner = &m_measureAligner;
        m_measureAligner.Process(params->m_functor, params, NULL, NULL, UNLIMITED_DEPTH, BACKWARD);
        params->m_alignmentArpegTuples.clear();
    }

    return FUNCTOR_CONTINUE;
}

int Measure::AdjustClefChanges(FunctorParams *functorParams)
{
    AdjustClefsParams *params = vrv_params_cast<AdjustClefsParams *>(functorParams);
    assert(params);

    params->m_aligner = &m_measureAligner;

    return FUNCTOR_CONTINUE;
}

int Measure::AdjustLayers(FunctorParams *functorParams)
{
    AdjustLayersParams *params = vrv_params_cast<AdjustLayersParams *>(functorParams);
    assert(params);

    if (!m_hasAlignmentRefWithMultipleLayers) return FUNCTOR_SIBLINGS;

    std::vector<int>::iterator iter;
    ArrayOfComparisons filters;
    for (iter = params->m_staffNs.begin(); iter != params->m_staffNs.end(); ++iter) {
        filters.clear();
        // Create ad comparison object for each type / @n
        std::vector<int> ns;
        // -1 for barline attributes that need to be taken into account each time
        ns.push_back(BARLINE_REFERENCES);
        ns.push_back(*iter);
        AttNIntegerAnyComparison matchStaff(ALIGNMENT_REFERENCE, ns);
        filters.push_back(&matchStaff);

        m_measureAligner.Process(params->m_functor, params, params->m_functorEnd, &filters);
    }

    return FUNCTOR_SIBLINGS;
}

int Measure::AdjustDots(FunctorParams *functorParams)
{
    AdjustDotsParams *params = vrv_params_cast<AdjustDotsParams *>(functorParams);
    assert(params);

    if (!m_hasAlignmentRefWithMultipleLayers) return FUNCTOR_SIBLINGS;

    std::vector<int>::iterator iter;
    ArrayOfComparisons filters;
    for (iter = params->m_staffNs.begin(); iter != params->m_staffNs.end(); ++iter) {
        filters.clear();
        // Create ad comparison object for each type / @n
        std::vector<int> ns;
        // -1 for barline attributes that need to be taken into account each time
        ns.push_back(BARLINE_REFERENCES);
        ns.push_back(*iter);
        AttNIntegerAnyComparison matchStaff(ALIGNMENT_REFERENCE, ns);
        filters.push_back(&matchStaff);

        m_measureAligner.Process(params->m_functor, params, params->m_functorEnd, &filters);
    }

    return FUNCTOR_SIBLINGS;
}

int Measure::AdjustAccidX(FunctorParams *functorParams)
{
    AdjustAccidXParams *params = vrv_params_cast<AdjustAccidXParams *>(functorParams);
    assert(params);

    params->m_currentMeasure = this;

    m_measureAligner.Process(params->m_functor, params);

    return FUNCTOR_CONTINUE;
}

int Measure::AdjustGraceXPos(FunctorParams *functorParams)
{
    AdjustGraceXPosParams *params = vrv_params_cast<AdjustGraceXPosParams *>(functorParams);
    assert(params);

    m_measureAligner.PushAlignmentsRight();
    params->m_rightDefaultAlignment = NULL;

    // We process it backward because we want to get the rightDefaultAlignment
    m_measureAligner.Process(params->m_functor, params, params->m_functorEnd, NULL, UNLIMITED_DEPTH, BACKWARD);

    // We need to process the staves in the reverse order
    std::vector<int> staffNs = params->m_staffNs;
    std::vector<int> staffNsReversed;
    staffNsReversed.resize(staffNs.size());
    std::reverse_copy(staffNs.begin(), staffNs.end(), staffNsReversed.begin());

    m_measureAligner.PushAlignmentsRight();
    params->m_rightDefaultAlignment = NULL;

    params->m_staffNs = staffNsReversed;
    m_measureAligner.Process(params->m_functor, params, params->m_functorEnd, NULL, UNLIMITED_DEPTH, BACKWARD);

    // Put params back
    params->m_staffNs = staffNs;

    return FUNCTOR_SIBLINGS;
}

int Measure::AdjustXPos(FunctorParams *functorParams)
{
    AdjustXPosParams *params = vrv_params_cast<AdjustXPosParams *>(functorParams);
    assert(params);

    params->m_minPos = 0;
    params->m_upcomingMinPos = VRV_UNSET;
    params->m_cumulatedXShift = 0;

    System *system = vrv_cast<System *>(this->GetFirstAncestor(SYSTEM));
    assert(system);

    const bool hasSystemStartLine = this->IsFirstInSystem() && system->GetDrawingScoreDef()->HasSystemStartLine();

    ArrayOfComparisons filters;
    for (auto staffN : params->m_staffNs) {
        params->m_minPos = 0;
        params->m_upcomingMinPos = VRV_UNSET;
        params->m_cumulatedXShift = 0;
        params->m_staffN = (staffN);
        params->m_boundingBoxes.clear();
        params->m_previousAlignment.Reset();
        params->m_currentAlignment.Reset();
        StaffAlignment *staffAlignment = system->m_systemAligner.GetStaffAlignmentForStaffN(staffN);
        params->m_staffSize = (staffAlignment) ? staffAlignment->GetStaffSize() : 100;

        // Prevent collisions of scoredef clefs with thick barlines
        if (hasSystemStartLine) {
            params->m_upcomingMinPos = params->m_doc->GetDrawingBarLineWidth(params->m_staffSize);
        }

        filters.clear();
        // Create ad comparison object for each type / @n
        std::vector<int> ns;
        // -1 for barline attributes that need to be taken into account each time
        ns.push_back(-1);
        ns.push_back(staffN);
        AttNIntegerAnyComparison matchStaff(ALIGNMENT_REFERENCE, ns);
        filters.push_back(&matchStaff);

        params->m_measureTieEndpoints = this->GetInternalTieEndpoints();
        m_measureAligner.Process(params->m_functor, params, params->m_functorEnd, &filters);
    }

    // m_measureAligner.Process(params->m_functor, params, params->m_functorEnd);

    int minMeasureWidth
        = params->m_doc->GetOptions()->m_unit.GetValue() * params->m_doc->GetOptions()->m_measureMinWidth.GetValue();
    // First try to see if we have a double measure length element
    MeasureAlignerTypeComparison alignmentComparison(ALIGNMENT_FULLMEASURE2);
    Alignment *fullMeasure2
        = dynamic_cast<Alignment *>(m_measureAligner.FindDescendantByComparison(&alignmentComparison, 1));

    // With a double measure with element (mRpt2, multiRpt)
    if (fullMeasure2 != NULL) {
        minMeasureWidth *= 2;
    }
    // Nothing if the measure has at least one note or @metcon="false"
    else if ((this->FindDescendantByType(NOTE) != NULL) || (this->GetMetcon() == BOOLEAN_false)) {
        minMeasureWidth = 0;
    }
    // Adjust min width based on multirest attributes (@num and @width), but only if these values are larger than
    // current min width
    else if (this->FindDescendantByType(MULTIREST) != NULL) {
        MultiRest *multiRest = vrv_cast<MultiRest *>(this->FindDescendantByType(MULTIREST));
        const int num = multiRest->GetNum();
        if (multiRest->HasWidth()) {
            const int fixedWidth
                = multiRest->AttWidth::GetWidth() * (params->m_doc->GetDrawingUnit(params->m_staffSize) + 4);
            if (minMeasureWidth < fixedWidth) minMeasureWidth = fixedWidth;
        }
        else if (num > 10) {
            minMeasureWidth *= log1p(num) / 2;
        }
    }

    int currentMeasureWidth = this->GetRightBarLineLeft() - this->GetLeftBarLineRight();
    if (currentMeasureWidth < minMeasureWidth) {
        ArrayOfAdjustmentTuples boundaries{ std::make_tuple(this->GetLeftBarLine()->GetAlignment(),
            this->GetRightBarLine()->GetAlignment(), minMeasureWidth - currentMeasureWidth) };
        m_measureAligner.AdjustProportionally(boundaries);
    }

    return FUNCTOR_SIBLINGS;
}

int Measure::AdjustHarmGrpsSpacingEnd(FunctorParams *functorParams)
{
    AdjustHarmGrpsSpacingParams *params = vrv_params_cast<AdjustHarmGrpsSpacingParams *>(functorParams);
    assert(params);

    // At the end of the measure - pass it along for overlapping verses
    params->m_previousMeasure = this;

    // Adjust the postion of the alignment according to what we have collected for this harm gpr
    m_measureAligner.AdjustProportionally(params->m_overlapingHarm);
    params->m_overlapingHarm.clear();

    return FUNCTOR_CONTINUE;
}

int Measure::AdjustSylSpacingEnd(FunctorParams *functorParams)
{
    AdjustSylSpacingParams *params = vrv_params_cast<AdjustSylSpacingParams *>(functorParams);
    assert(params);

    // At the end of the measure - pass it along for overlapping verses
    params->m_previousMeasure = this;

    // Adjust the postion of the alignment according to what we have collected for this verse
    m_measureAligner.AdjustProportionally(params->m_overlapingSyl);
    params->m_overlapingSyl.clear();

    return FUNCTOR_CONTINUE;
}

int Measure::AdjustXOverflow(FunctorParams *functorParams)
{
    AdjustXOverflowParams *params = vrv_params_cast<AdjustXOverflowParams *>(functorParams);
    assert(params);

    params->m_lastMeasure = this;
    // For now look only at the content of the last measure, so discard any previous control event.
    // We need to do this because AdjustXOverflow is run before measures are aligned, so the right
    // position comparison do not actually tell us which one is the longest. This is not optimal
    // and can be improved.
    params->m_currentWidest = NULL;

    return FUNCTOR_CONTINUE;
}

int Measure::SetAlignmentXPos(FunctorParams *functorParams)
{
    SetAlignmentXPosParams *params = vrv_params_cast<SetAlignmentXPosParams *>(functorParams);
    assert(params);

    m_measureAligner.Process(params->m_functor, params);

    return FUNCTOR_SIBLINGS;
}

int Measure::JustifyX(FunctorParams *functorParams)
{
    JustifyXParams *params = vrv_params_cast<JustifyXParams *>(functorParams);
    assert(params);

    if (params->m_applySectionRestartShift) {
        params->m_measureXRel += this->GetSectionRestartShift(params->m_doc);
        params->m_applySectionRestartShift = false;
    }

    if (params->m_measureXRel > 0) {
        this->SetDrawingXRel(params->m_measureXRel);
    }
    else {
        params->m_measureXRel = this->GetDrawingXRel();
    }

    m_measureAligner.Process(params->m_functor, params);

    return FUNCTOR_SIBLINGS;
}

int Measure::AlignMeasures(FunctorParams *functorParams)
{
    AlignMeasuresParams *params = vrv_params_cast<AlignMeasuresParams *>(functorParams);
    assert(params);

    if (params->m_applySectionRestartShift) {
        params->m_shift += this->GetSectionRestartShift(params->m_doc);
        params->m_applySectionRestartShift = false;
    }

    this->SetDrawingXRel(params->m_shift);

    params->m_shift += this->GetWidth();
    params->m_justifiableWidth += this->GetRightBarLineXRel() - this->GetLeftBarLineXRel();

    return FUNCTOR_SIBLINGS;
}

int Measure::ResetDrawing(FunctorParams *functorParams)
{
    m_timestampAligner.Reset();
    m_drawingEnding = NULL;
    return FUNCTOR_CONTINUE;
}

int Measure::CastOffSystems(FunctorParams *functorParams)
{
    CastOffSystemsParams *params = vrv_params_cast<CastOffSystemsParams *>(functorParams);
    assert(params);

    const bool hasCache = this->HasCachedHorizontalLayout();
    int overflow = hasCache ? this->m_cachedOverflow : this->GetDrawingOverflow();
    int width = hasCache ? this->m_cachedWidth : this->GetWidth();
    int drawingXRel = this->m_drawingXRel;

    Object *nextMeasure = params->m_contentSystem->GetNext(this, MEASURE);
    const bool isLeftoverMeasure = ((NULL == nextMeasure) && params->m_doc->GetOptions()->m_breaksNoWidow.GetValue()
        && (params->m_doc->GetOptions()->m_breaks.GetValue() != BREAKS_encoded));
    if (params->m_currentSystem->GetChildCount() > 0) {
        // We have overflowing content (dir, dynam, tempo) larger than 5 units, keep it as pending
        if (overflow > (params->m_doc->GetDrawingUnit(100) * 5)) {
            Measure *measure = dynamic_cast<Measure *>(params->m_contentSystem->Relinquish(this->GetIdx()));
            assert(measure);
            // move as pending since we want it not to be broken with the next measure
            params->m_pendingElements.push_back(measure);
            // continue
            return FUNCTOR_SIBLINGS;
        }
        // Break it if necessary
        else if (drawingXRel + width + params->m_currentScoreDefWidth - params->m_shift > params->m_systemWidth) {
            params->m_currentSystem = new System();
            params->m_page->AddChild(params->m_currentSystem);
            params->m_shift = drawingXRel;
            // If last measure requires separate system - mark that system as leftover for the future CastOffPages call
            if (isLeftoverMeasure) {
                params->m_leftoverSystem = params->m_currentSystem;
            }
            for (Object *oneOfPendingObjects : params->m_pendingElements) {
                if (oneOfPendingObjects->Is(MEASURE)) {
                    Measure *firstPendingMesure = vrv_cast<Measure *>(oneOfPendingObjects);
                    assert(firstPendingMesure);
                    params->m_shift = firstPendingMesure->m_cachedXRel;
                    params->m_leftoverSystem = NULL;
                    // it has to be first measure
                    break;
                }
            }
        }
    }

    // First add all pendings objects
    ArrayOfObjects::iterator iter;
    for (iter = params->m_pendingElements.begin(); iter != params->m_pendingElements.end(); ++iter) {
        params->m_currentSystem->AddChild(*iter);
    }
    params->m_pendingElements.clear();

    // Special case where we use the Relinquish method.
    Measure *measure = dynamic_cast<Measure *>(params->m_contentSystem->Relinquish(this->GetIdx()));
    assert(measure);
    params->m_currentSystem->AddChild(measure);

    return FUNCTOR_SIBLINGS;
}

int Measure::CastOffEncoding(FunctorParams *functorParams)
{
    CastOffEncodingParams *params = vrv_params_cast<CastOffEncodingParams *>(functorParams);
    assert(params);

    MoveItselfTo(params->m_currentSystem);

    return FUNCTOR_CONTINUE;
}

int Measure::InitSelection(FunctorParams *functorParams)
{
    InitSelectionParams *params = vrv_params_cast<InitSelectionParams *>(functorParams);
    assert(params);

    const bool startSelection = (!params->m_isSelection && this->GetUuid() == params->m_start);
    const bool endSelection = (params->m_isSelection && this->GetUuid() == params->m_end);

    if (!startSelection) MoveItselfTo(params->m_currentSystem);

    if (startSelection || endSelection) {
        Page *page = new Page();
        params->m_doc->GetPages()->AddChild(page);
        System *system = new System();
        page->AddChild(system);
        params->m_currentSystem = system;
        params->m_isSelection = !params->m_isSelection;
    }

    if (startSelection) MoveItselfTo(params->m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

int Measure::FillStaffCurrentTimeSpanningEnd(FunctorParams *functorParams)
{
    FillStaffCurrentTimeSpanningParams *params = vrv_params_cast<FillStaffCurrentTimeSpanningParams *>(functorParams);
    assert(params);

    std::vector<Object *>::iterator iter = params->m_timeSpanningElements.begin();
    while (iter != params->m_timeSpanningElements.end()) {
        Measure *endParent = NULL;
        if ((*iter)->HasInterface(INTERFACE_TIME_SPANNING)) {
            TimeSpanningInterface *interface = (*iter)->GetTimeSpanningInterface();
            assert(interface);
            if (interface->GetEnd()) {
                endParent = dynamic_cast<Measure *>(interface->GetEnd()->GetFirstAncestor(MEASURE));
            }
        }
        if (!endParent && (*iter)->HasInterface(INTERFACE_LINKING)) {
            LinkingInterface *interface = (*iter)->GetLinkingInterface();
            assert(interface);
            if (interface->GetNextLink()) {
                // We should have one because we allow only control Event (dir and dynam) to be linked as target
                TimePointInterface *nextInterface = interface->GetNextLink()->GetTimePointInterface();
                assert(nextInterface);
                endParent = dynamic_cast<Measure *>(nextInterface->GetStart()->GetFirstAncestor(MEASURE));
            }
        }
        assert(endParent);
        // We have reached the end of the spanning - remove it from the list of running elements
        if (endParent == this) {
            iter = params->m_timeSpanningElements.erase(iter);
        }
        else {
            ++iter;
        }
    }
    return FUNCTOR_CONTINUE;
}

int Measure::PrepareMilestones(FunctorParams *functorParams)
{
    PrepareMilestonesParams *params = vrv_params_cast<PrepareMilestonesParams *>(functorParams);
    assert(params);

    std::vector<SystemMilestoneInterface *>::iterator iter;
    for (iter = params->m_startMilestones.begin(); iter != params->m_startMilestones.end(); ++iter) {
        (*iter)->SetMeasure(this);
    }
    params->m_startMilestones.clear();

    if (params->m_currentEnding) {
        // Set the ending to each measure in between
        m_drawingEnding = params->m_currentEnding;
    }

    // Keep a pointer to the measure for when we are reaching the end (see SystemMilestoneEnd::PrepareMilestones)
    params->m_lastMeasure = this;

    return FUNCTOR_CONTINUE;
}

int Measure::PrepareCrossStaff(FunctorParams *functorParams)
{
    PrepareCrossStaffParams *params = vrv_params_cast<PrepareCrossStaffParams *>(functorParams);
    assert(params);

    params->m_currentMeasure = this;

    return FUNCTOR_CONTINUE;
}

int Measure::PrepareFloatingGrps(FunctorParams *functorParams)
{
    PrepareFloatingGrpsParams *params = vrv_params_cast<PrepareFloatingGrpsParams *>(functorParams);
    assert(params);

    if (params->m_previousEnding) {
        // We have a measure in between endings and the previous one was group, just reset pointer to NULL
        params->m_previousEnding = NULL;
    }

    return FUNCTOR_CONTINUE;
}

int Measure::PrepareFloatingGrpsEnd(FunctorParams *functorParams)
{
    PrepareFloatingGrpsParams *params = vrv_params_cast<PrepareFloatingGrpsParams *>(functorParams);
    assert(params);

    params->m_dynams.clear();

    std::vector<Hairpin *>::iterator iter = params->m_hairpins.begin();
    while (iter != params->m_hairpins.end()) {
        assert((*iter)->GetEnd());
        Measure *measureEnd = dynamic_cast<Measure *>((*iter)->GetEnd()->GetFirstAncestor(MEASURE));
        if (measureEnd == this) {
            iter = params->m_hairpins.erase(iter);
        }
        else {
            ++iter;
        }
    }

    // Match down and up pedal lines
    using pedalIter = std::list<Pedal *>::iterator;
    pedalIter pIter = params->m_pedalLines.begin();
    while (pIter != params->m_pedalLines.end()) {
        if ((*pIter)->GetDir() != pedalLog_DIR_down) {
            ++pIter;
            continue;
        }
        pedalIter up = std::find_if(params->m_pedalLines.begin(), params->m_pedalLines.end(), [&pIter](Pedal *pedal) {
            if (((*pIter)->GetStaff() == pedal->GetStaff()) && (pedal->GetDir() != pedalLog_DIR_down)) {
                return true;
            }
            return false;
        });
        if (up != params->m_pedalLines.end()) {
            (*pIter)->SetEnd((*up)->GetStart());
            if ((*up)->GetDir() == pedalLog_DIR_bounce) {
                (*pIter)->EndsWithBounce(true);
            }
            params->m_pedalLines.erase(up);
            pIter = params->m_pedalLines.erase(pIter);
        }
        else {
            ++pIter;
        }
    }

    return FUNCTOR_CONTINUE;
}

int Measure::PrepareTimePointingEnd(FunctorParams *functorParams)
{
    PrepareTimePointingParams *params = vrv_params_cast<PrepareTimePointingParams *>(functorParams);
    assert(params);

    if (!params->m_timePointingInterfaces.empty()) {
        LogWarning("%d time pointing element(s) could not be matched in measure %s",
            params->m_timePointingInterfaces.size(), this->GetUuid().c_str());
    }

    ListOfPointingInterClassIdPairs::iterator iter = params->m_timePointingInterfaces.begin();
    while (iter != params->m_timePointingInterfaces.end()) {
        iter = params->m_timePointingInterfaces.erase(iter);
    }

    return FUNCTOR_CONTINUE;
}

int Measure::PrepareTimeSpanningEnd(FunctorParams *functorParams)
{
    PrepareTimeSpanningParams *params = vrv_params_cast<PrepareTimeSpanningParams *>(functorParams);
    assert(params);

    ListOfSpanningInterOwnerPairs::iterator iter = params->m_timeSpanningInterfaces.begin();
    while (iter != params->m_timeSpanningInterfaces.end()) {
        // At the end of the measure (going backward) we remove element for which we do not need to match the end (for
        // now). Eventually, we could consider them, for example if we want to display their spanning or for improved
        // midi output
        if (iter->second->GetClassId() == HARM) {
            iter = params->m_timeSpanningInterfaces.erase(iter);
        }
        else {
            ++iter;
        }
    }

    return FUNCTOR_CONTINUE;
}

int Measure::PrepareTimestampsEnd(FunctorParams *functorParams)
{
    PrepareTimestampsParams *params = vrv_params_cast<PrepareTimestampsParams *>(functorParams);
    assert(params);

    ListOfObjectBeatPairs::iterator iter = params->m_tstamps.begin();
    // Loop throught the object/beat pairs and create the TimestampAttr when necessary
    while (iter != params->m_tstamps.end()) {
        // -1 means that we have a @tstamp (start) to add to the current measure
        if ((*iter).second.first == -1) {
            TimePointInterface *interface = ((*iter).first)->GetTimePointInterface();
            assert(interface);
            TimestampAttr *timestampAttr = m_timestampAligner.GetTimestampAtTime((*iter).second.second);
            interface->SetStart(timestampAttr);
            // purge the list of unmatched element is this is a TimeSpanningInterface element
            if ((*iter).first->HasInterface(INTERFACE_TIME_SPANNING)) {
                TimeSpanningInterface *tsInterface = ((*iter).first)->GetTimeSpanningInterface();
                assert(tsInterface);
                if (tsInterface->HasStartAndEnd()) {
                    auto item
                        = std::find_if(params->m_timeSpanningInterfaces.begin(), params->m_timeSpanningInterfaces.end(),
                            [tsInterface](std::pair<TimeSpanningInterface *, ClassId> pair) {
                                return (pair.first == tsInterface);
                            });
                    if (item != params->m_timeSpanningInterfaces.end()) {
                        // LogDebug("Found it!");
                        params->m_timeSpanningInterfaces.erase(item);
                    }
                }
            }
            // remove it
            iter = params->m_tstamps.erase(iter);
        }
        // 0 means that we have a @tstamp2 (end) to add to the current measure
        else if ((*iter).second.first == 0) {
            TimeSpanningInterface *interface = ((*iter).first)->GetTimeSpanningInterface();
            assert(interface);
            TimestampAttr *timestampAttr = m_timestampAligner.GetTimestampAtTime((*iter).second.second);
            interface->SetEnd(timestampAttr);
            // We can check if the interface is now fully mapped (start / end) and purge the list of unmatched
            // elements
            if (interface->HasStartAndEnd()) {
                auto item
                    = std::find_if(params->m_timeSpanningInterfaces.begin(), params->m_timeSpanningInterfaces.end(),
                        [interface](
                            std::pair<TimeSpanningInterface *, ClassId> pair) { return (pair.first == interface); });
                if (item != params->m_timeSpanningInterfaces.end()) {
                    // LogDebug("Found it!");
                    params->m_timeSpanningInterfaces.erase(item);
                }
            }
            iter = params->m_tstamps.erase(iter);
        }
        // we have not reached the correct end measure yet
        else {
            (*iter).second.first--;
            ++iter;
        }
    }

    // Here we can also set the start for F within Harm that have no @startid or @tstamp but might have an extender
    // In the future, we can do something similar to handle Dir within other types of control events
    // Basically, a child control event should use the start (and end) of its parent.
    // In the case of F, we still expect the @tstamp2 to be given in F, but this could be changed
    // Eventually, this could be done in another functor if it becomes a more common way to set start / end because it
    // is a bit weird to iterate over F objects here.
    ListOfObjects fs = this->FindAllDescendantsByType(FIGURE);
    for (auto &object : fs) {
        F *f = vrv_cast<F *>(object);
        assert(f);
        // Nothing to do if the f has as start or has not end
        if (f->GetStart() || !f->GetEnd()) continue;

        Harm *harm = vrv_cast<Harm *>(f->GetFirstAncestor(HARM));
        if (harm) {
            f->SetStart(harm->GetStart());
            // We should also remove the f from the list because we can consider it as being mapped now
            auto item = std::find_if(params->m_timeSpanningInterfaces.begin(), params->m_timeSpanningInterfaces.end(),
                [f](std::pair<TimeSpanningInterface *, ClassId> pair) { return (pair.first == f); });
            if (item != params->m_timeSpanningInterfaces.end()) {
                // LogDebug("Found it!");
                params->m_timeSpanningInterfaces.erase(item);
            }
        }
    }

    return FUNCTOR_CONTINUE;
}

int Measure::PrepareMIDI(FunctorParams *functorParams)
{
    PrepareMIDIParams *params = vrv_params_cast<PrepareMIDIParams *>(functorParams);
    assert(params);

    params->m_currentTempo = m_currentTempo;

    return FUNCTOR_CONTINUE;
}

int Measure::GenerateMIDI(FunctorParams *functorParams)
{
    GenerateMIDIParams *params = vrv_params_cast<GenerateMIDIParams *>(functorParams);
    assert(params);

    // Here we need to update the m_totalTime from the starting time of the measure.
    params->m_totalTime = m_scoreTimeOffset.back();

    if (m_currentTempo != params->m_currentTempo) {
        params->m_midiFile->addTempo(0, m_scoreTimeOffset.back() * params->m_midiFile->getTPQ(), m_currentTempo);
        params->m_currentTempo = m_currentTempo;
    }

    return FUNCTOR_CONTINUE;
}

int Measure::GenerateTimemap(FunctorParams *functorParams)
{
    GenerateTimemapParams *params = vrv_params_cast<GenerateTimemapParams *>(functorParams);
    assert(params);

    params->m_scoreTimeOffset = this->m_scoreTimeOffset.back();
    params->m_realTimeOffsetMilliseconds = this->m_realTimeOffsetMilliseconds.back();
    params->m_currentTempo = this->m_currentTempo;

    params->m_timemap->AddEntry(this, params);

    return FUNCTOR_CONTINUE;
}

int Measure::CalcMaxMeasureDuration(FunctorParams *functorParams)
{
    CalcMaxMeasureDurationParams *params = vrv_params_cast<CalcMaxMeasureDurationParams *>(functorParams);
    assert(params);

    m_scoreTimeOffset.clear();
    m_scoreTimeOffset.push_back(params->m_currentScoreTime);

    m_realTimeOffsetMilliseconds.clear();
    // m_realTimeOffsetMilliseconds.push_back(int(params->m_maxCurrentRealTimeSeconds * 1000.0 + 0.5));
    m_realTimeOffsetMilliseconds.push_back(params->m_currentRealTimeSeconds * 1000.0);

    return FUNCTOR_CONTINUE;
}

int Measure::CalcMaxMeasureDurationEnd(FunctorParams *functorParams)
{
    CalcMaxMeasureDurationParams *params = vrv_params_cast<CalcMaxMeasureDurationParams *>(functorParams);
    assert(params);

    const double scoreTimeIncrement
        = m_measureAligner.GetRightAlignment()->GetTime() * params->m_multiRestFactor * DURATION_4 / DUR_MAX;
    m_currentTempo = params->m_currentTempo * params->m_tempoAdjustment;
    params->m_currentScoreTime += scoreTimeIncrement;
    params->m_currentRealTimeSeconds += scoreTimeIncrement * 60.0 / m_currentTempo;
    params->m_multiRestFactor = 1;

    return FUNCTOR_CONTINUE;
}

int Measure::CalcOnsetOffset(FunctorParams *functorParams)
{
    CalcOnsetOffsetParams *params = vrv_params_cast<CalcOnsetOffsetParams *>(functorParams);
    assert(params);

    params->m_currentTempo = m_currentTempo;

    return FUNCTOR_CONTINUE;
}

int Measure::UnCastOff(FunctorParams *functorParams)
{
    UnCastOffParams *params = vrv_params_cast<UnCastOffParams *>(functorParams);
    assert(params);

    if (params->m_resetCache) {
        m_cachedXRel = VRV_UNSET;
        m_cachedWidth = VRV_UNSET;
        m_cachedOverflow = VRV_UNSET;
    }

    return FUNCTOR_CONTINUE;
}

int Measure::HorizontalLayoutCache(FunctorParams *functorParams)
{
    HorizontalLayoutCacheParams *params = vrv_params_cast<HorizontalLayoutCacheParams *>(functorParams);
    assert(params);

    if (params->m_restore) {
        m_drawingXRel = m_cachedXRel;
    }
    else {
        m_cachedWidth = this->GetWidth();
        m_cachedOverflow = this->GetDrawingOverflow();
        m_cachedXRel = m_drawingXRel;
    }
    if (this->GetLeftBarLine()) this->GetLeftBarLine()->HorizontalLayoutCache(functorParams);
    if (this->GetRightBarLine()) this->GetRightBarLine()->HorizontalLayoutCache(functorParams);

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

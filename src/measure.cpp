/////////////////////////////////////////////////////////////////////////////
// Name:        measure.cpp
// Author:      Laurent Pugin
// Created:     2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "measure.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <cassert>
#include <math.h>

//----------------------------------------------------------------------------

#include "adjustxoverflowfunctor.h"
#include "comparison.h"
#include "controlelement.h"
#include "doc.h"
#include "dynam.h"
#include "editorial.h"
#include "ending.h"
#include "f.h"
#include "functor.h"
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

    m_index = VRV_UNSET;

    this->ResetDrawingScoreDef();

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

void Measure::CacheXRel(bool restore)
{
    if (restore) {
        m_drawingXRel = m_cachedXRel;
    }
    else {
        m_cachedWidth = this->GetWidth();
        m_cachedOverflow = this->GetDrawingOverflow();
        m_cachedXRel = m_drawingXRel;
    }
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

int Measure::CalculateRightBarLineWidth(const Doc *doc, int staffSize) const
{
    const BarLine *barline = this->GetRightBarLine();
    if (!barline) return 0;

    const int barLineWidth = doc->GetDrawingBarLineWidth(staffSize);
    const int barLineThickWidth
        = doc->GetDrawingUnit(staffSize) * doc->GetOptions()->m_thickBarlineThickness.GetValue();
    const int barLineSeparation = doc->GetDrawingUnit(staffSize) * doc->GetOptions()->m_barLineSeparation.GetValue();

    int width = 0;
    switch (barline->GetForm()) {
        case BARRENDITION_dbl:
        case BARRENDITION_dbldashed:
        case BARRENDITION_dbldotted: {
            width = barLineSeparation + barLineWidth;
            break;
        }
        case BARRENDITION_rptend:
        case BARRENDITION_end: {
            width = barLineSeparation + barLineWidth + barLineThickWidth;
            break;
        }
        case BARRENDITION_rptboth: {
            width = 2 * barLineSeparation + barLineWidth + barLineThickWidth;
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
    AdjustXOverflowFunctor adjustXOverflow(0);
    System *system = vrv_cast<System *>(this->GetFirstAncestor(SYSTEM));
    assert(system);
    adjustXOverflow.SetCurrentSystem(system);
    adjustXOverflow.SetLastMeasure(this);
    this->Process(adjustXOverflow);

    FloatingPositioner *widestPositioner = adjustXOverflow.GetWidestPositioner();
    if (!widestPositioner) return 0;

    int measureRightX = this->GetDrawingX() + this->GetWidth();
    int overflow = widestPositioner->GetContentRight() - measureRightX;
    return std::max(0, overflow);
}

int Measure::GetSectionRestartShift(const Doc *doc) const
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
    assert(!m_drawingScoreDef); // We should always call ResetDrawingScoreDef before

    m_drawingScoreDef = new ScoreDef();
    *m_drawingScoreDef = *drawingScoreDef;
}

void Measure::ResetDrawingScoreDef()
{
    if (m_drawingScoreDef) {
        delete m_drawingScoreDef;
        m_drawingScoreDef = NULL;
    }
}

std::vector<Staff *> Measure::GetFirstStaffGrpStaves(ScoreDef *scoreDef)
{
    assert(scoreDef);

    std::vector<Staff *> staves;
    std::set<int> staffList;

    // First get all the staffGrps
    ListOfObjects staffGrps = scoreDef->FindAllDescendantsByType(STAFFGRP);

    // Then the @n of each first staffDef
    for (Object *staffGrp : staffGrps) {
        StaffDef *staffDef = vrv_cast<StaffDef *>((staffGrp)->FindDescendantByType(STAFFDEF));
        if (staffDef && (staffDef->GetDrawingVisibility() != OPTIMIZATION_HIDDEN)) staffList.insert(staffDef->GetN());
    }

    // Get the corresponding staves in the measure
    for (int staffN : staffList) {
        AttNIntegerComparison matchN(STAFF, staffN);
        Staff *staff = vrv_cast<Staff *>(this->FindDescendantByComparison(&matchN, 1));
        if (!staff) {
            // LogDebug("Staff with @n '%d' not found in measure '%s'", *iter, measure->GetID().c_str());
            continue;
        }
        staves.push_back(staff);
    }
    if (staves.empty()) LogDebug("Empty @staff array");
    return staves;
}

Staff *Measure::GetTopVisibleStaff()
{
    return const_cast<Staff *>(std::as_const(*this).GetTopVisibleStaff());
}

const Staff *Measure::GetTopVisibleStaff() const
{
    const Staff *staff = NULL;
    ListOfConstObjects staves = this->FindAllDescendantsByType(STAFF, false);
    for (const Object *child : staves) {
        staff = vrv_cast<const Staff *>(child);
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
    return const_cast<Staff *>(std::as_const(*this).GetBottomVisibleStaff());
}

const Staff *Measure::GetBottomVisibleStaff() const
{
    const Staff *bottomStaff = NULL;
    ListOfConstObjects staves = this->FindAllDescendantsByType(STAFF, false);
    for (const auto child : staves) {
        const Staff *staff = vrv_cast<const Staff *>(child);
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

Measure::BarlineRenditionPair Measure::SelectDrawingBarLines(const Measure *previous) const
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

FunctorCode Measure::Accept(MutableFunctor &functor)
{
    return functor.VisitMeasure(this);
}

FunctorCode Measure::Accept(ConstFunctor &functor) const
{
    return functor.VisitMeasure(this);
}

FunctorCode Measure::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitMeasureEnd(this);
}

FunctorCode Measure::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitMeasureEnd(this);
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

    Filters filters;
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
    return (this->IsMeasuredMusic()) ? Object::Save(functorParams) : FUNCTOR_CONTINUE;
}

int Measure::SaveEnd(FunctorParams *functorParams)
{
    return (this->IsMeasuredMusic()) ? Object::SaveEnd(functorParams) : FUNCTOR_CONTINUE;
}

int Measure::ApplyPPUFactor(FunctorParams *functorParams)
{
    ApplyPPUFactorParams *params = vrv_params_cast<ApplyPPUFactorParams *>(functorParams);
    assert(params);

    if (m_xAbs != VRV_UNSET) m_xAbs /= params->m_page->GetPPUFactor();
    if (m_xAbs2 != VRV_UNSET) m_xAbs2 /= params->m_page->GetPPUFactor();

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

int Measure::CastOffToSelection(FunctorParams *functorParams)
{
    CastOffToSelectionParams *params = vrv_params_cast<CastOffToSelectionParams *>(functorParams);
    assert(params);

    const bool startSelection = (!params->m_isSelection && this->GetID() == params->m_start);

    if (startSelection) {
        params->m_page = new Page();
        params->m_doc->GetPages()->AddChild(params->m_page);
        params->m_currentSystem = new System();
        params->m_page->AddChild(params->m_currentSystem);
        params->m_isSelection = true;
    }

    const bool endSelection = (params->m_isSelection && this->GetID() == params->m_end);

    MoveItselfTo(params->m_currentSystem);

    if (endSelection) {
        params->m_page = new Page();
        params->m_doc->GetPages()->AddChild(params->m_page);
        params->m_currentSystem = new System();
        params->m_page->AddChild(params->m_currentSystem);
        params->m_isSelection = false;
    }

    return FUNCTOR_SIBLINGS;
}

int Measure::InitMIDI(FunctorParams *functorParams)
{
    InitMIDIParams *params = vrv_params_cast<InitMIDIParams *>(functorParams);
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

int Measure::InitMaxMeasureDuration(FunctorParams *functorParams)
{
    InitMaxMeasureDurationParams *params = vrv_params_cast<InitMaxMeasureDurationParams *>(functorParams);
    assert(params);

    m_scoreTimeOffset.clear();
    m_scoreTimeOffset.push_back(params->m_currentScoreTime);

    m_realTimeOffsetMilliseconds.clear();
    // m_realTimeOffsetMilliseconds.push_back(int(params->m_maxCurrentRealTimeSeconds * 1000.0 + 0.5));
    m_realTimeOffsetMilliseconds.push_back(params->m_currentRealTimeSeconds * 1000.0);

    return FUNCTOR_CONTINUE;
}

int Measure::InitMaxMeasureDurationEnd(FunctorParams *functorParams)
{
    InitMaxMeasureDurationParams *params = vrv_params_cast<InitMaxMeasureDurationParams *>(functorParams);
    assert(params);

    const double scoreTimeIncrement
        = m_measureAligner.GetRightAlignment()->GetTime() * params->m_multiRestFactor * DURATION_4 / DUR_MAX;
    m_currentTempo = params->m_currentTempo * params->m_tempoAdjustment;
    params->m_currentScoreTime += scoreTimeIncrement;
    params->m_currentRealTimeSeconds += scoreTimeIncrement * 60.0 / m_currentTempo;
    params->m_multiRestFactor = 1;

    return FUNCTOR_CONTINUE;
}

int Measure::InitOnsetOffset(FunctorParams *functorParams)
{
    InitOnsetOffsetParams *params = vrv_params_cast<InitOnsetOffsetParams *>(functorParams);
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

} // namespace vrv

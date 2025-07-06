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
#include "timestamp.h"
#include "vrv.h"

//----------------------------------------------------------------------------

#include "MidiFile.h"

namespace vrv {

//----------------------------------------------------------------------------
// Measure
//----------------------------------------------------------------------------

static const ClassRegistrar<Measure> s_factory("measure", MEASURE);

Measure::Measure(MeasureType measureMusic, int logMeasureNb)
    : Object(MEASURE)
    , FacsimileInterface()
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
    this->RegisterInterface(FacsimileInterface::GetAttClasses(), FacsimileInterface::IsInterface());

    m_measureType = measureMusic;

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

    if (!this->IsMeasuredMusic() && !this->IsNeumeLine()) this->SetRight(BARRENDITION_invis);
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
    FacsimileInterface::Reset();
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
    m_drawingFacsX1 = VRV_UNSET;
    m_drawingFacsX2 = VRV_UNSET;
    m_drawingXRel = 0;

    m_cachedXRel = VRV_UNSET;
    m_cachedOverflow = VRV_UNSET;
    m_cachedWidth = VRV_UNSET;

    // by default, we have a single barLine on the right (none on the left)
    m_rightBarLine.SetForm(this->GetRight());
    m_leftBarLine.SetForm(this->GetLeft());

    m_drawingFacsX1 = VRV_UNSET;
    m_drawingFacsX2 = VRV_UNSET;

    m_drawingEnding = NULL;
    m_hasAlignmentRefWithMultipleLayers = false;

    m_scoreTimeOnset.clear();
    m_realTimeOnsetMilliseconds.clear();
    m_scoreTimeOffset.clear();
    m_realTimeOffsetMilliseconds.clear();
    m_currentTempo = MIDI_TEMPO;
}

bool Measure::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ STAFF };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else if (Object::IsControlElement(classId)) {
        return true;
    }
    else if (Object::IsEditorialElement(classId)) {
        return true;
    }
    else {
        return false;
    }
}

void Measure::AddChildBack(Object *child)
{
    if (!this->IsSupportedChild(child->GetClassId()) || !this->AddChildAdditionalCheck(child)) {
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
    this->Modify();
}

bool Measure::AddChildAdditionalCheck(Object *child)
{
    if (child->Is(STAFF)) {
        Staff *staff = vrv_cast<Staff *>(child);
        assert(staff);
        if (staff && (staff->GetN() < 1)) {
            // This is not 100% safe if we have a <app> and <rdg> with more than
            // one staff as a previous child.
            staff->SetN(this->GetChildCount());
        }
    }
    return (Object::AddChildAdditionalCheck(child));
}

int Measure::GetDrawingX() const
{
    if (m_drawingFacsX1 != VRV_UNSET) return m_drawingFacsX1;

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
    if (m_drawingFacsX2 != VRV_UNSET) return (m_drawingFacsX2 - m_drawingFacsX1);

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
    m_drawingScoreDef->ReplaceWithCopyOf(drawingScoreDef);
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
        = m_measureAligner.GetRightAlignment()->GetTime().ToDouble() * SCORE_TIME_UNIT * 60.0 / m_currentTempo * 1000.0
        + 0.5;
    std::vector<double>::const_iterator iter;
    for (iter = m_realTimeOnsetMilliseconds.begin(); iter != m_realTimeOnsetMilliseconds.end(); ++iter) {
        if ((time >= *iter) && (time <= *iter + timeDuration)) return repeat;
        repeat++;
    }
    return VRV_UNSET;
}

Fraction Measure::GetScoreTimeOnset(int repeat) const
{
    if (m_scoreTimeOnset.empty() || (repeat > (int)m_scoreTimeOnset.size())) return 0;
    if (repeat == VRV_UNSET) return m_scoreTimeOnset.back();
    assert(repeat > 0);
    return m_scoreTimeOnset.at(repeat - 1);
}

double Measure::GetRealTimeOnsetMilliseconds(int repeat) const
{
    if (m_realTimeOnsetMilliseconds.empty() || (repeat > (int)m_realTimeOnsetMilliseconds.size())) return 0;
    if (repeat == VRV_UNSET) return m_realTimeOnsetMilliseconds.back();
    assert(repeat > 0);
    return m_realTimeOnsetMilliseconds.at(repeat - 1);
}

Fraction Measure::GetScoreTimeOffset(int repeat) const
{
    if (m_scoreTimeOffset.empty() || (repeat > (int)m_scoreTimeOffset.size())) return 0;
    if (repeat == VRV_UNSET) return m_scoreTimeOffset.back();
    assert(repeat > 0);
    return m_scoreTimeOffset.at(repeat - 1);
}

double Measure::GetRealTimeOffsetMilliseconds(int repeat) const
{
    if (m_realTimeOffsetMilliseconds.empty() || (repeat > (int)m_realTimeOffsetMilliseconds.size())) return 0;
    if (repeat == VRV_UNSET) return m_realTimeOffsetMilliseconds.back();
    assert(repeat > 0);
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

FunctorCode Measure::Accept(Functor &functor)
{
    return functor.VisitMeasure(this);
}

FunctorCode Measure::Accept(ConstFunctor &functor) const
{
    return functor.VisitMeasure(this);
}

FunctorCode Measure::AcceptEnd(Functor &functor)
{
    return functor.VisitMeasureEnd(this);
}

FunctorCode Measure::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitMeasureEnd(this);
}

} // namespace vrv

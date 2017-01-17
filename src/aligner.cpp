/////////////////////////////////////////////////////////////////////////////
// Name:        aligner.cpp
// Author:      Laurent Pugin
// Created:     2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "aligner.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "doc.h"
#include "floatingobject.h"
#include "functorparams.h"
#include "note.h"
#include "staff.h"
#include "style.h"
#include "timestamp.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// SystemAligner
//----------------------------------------------------------------------------

SystemAligner::SystemAligner() : Object()
{
    Reset();
}

SystemAligner::~SystemAligner()
{
}

void SystemAligner::Reset()
{
    Object::Reset();
    m_bottomAlignment = NULL;
    m_bottomAlignment = GetStaffAlignment(0, NULL, NULL);
}

StaffAlignment *SystemAligner::GetStaffAlignment(int idx, Staff *staff, Doc *doc)
{
    // The last one is always the bottomAlignment (unless if not created)
    if (m_bottomAlignment) {
        // remove it temporarily
        this->m_children.pop_back();
    }

    if (idx < GetStaffAlignmentCount()) {
        this->m_children.push_back(m_bottomAlignment);
        return dynamic_cast<StaffAlignment *>(m_children.at(idx));
    }
    // check that we are searching for the next one (not a gap)
    assert(idx == GetStaffAlignmentCount());
    // LogDebug("Creating staff alignment");

    // This is the first time we are looking for it (e.g., first staff)
    // We create the StaffAlignment
    StaffAlignment *alignment = new StaffAlignment();
    alignment->SetStaff(staff, doc);
    alignment->SetParent(this);
    m_children.push_back(alignment);

    if (m_bottomAlignment) {
        this->m_children.push_back(m_bottomAlignment);
    }

    return alignment;
}

StaffAlignment *SystemAligner::GetStaffAlignmentForStaffN(int staffN) const
{
    StaffAlignment *alignment = NULL;
    int i;
    for (i = 0; i < this->GetStaffAlignmentCount(); i++) {
        alignment = dynamic_cast<StaffAlignment *>(m_children.at(i));
        assert(alignment);

        if ((alignment->GetStaff()) && (alignment->GetStaff()->GetN() == staffN)) return alignment;
    }
    LogDebug("Staff alignment for staff %d not found", staffN);
    return NULL;
}

//----------------------------------------------------------------------------
// StaffAlignment
//----------------------------------------------------------------------------

StaffAlignment::StaffAlignment() : Object()
{
    m_yRel = 0;
    m_yShift = 0;
    m_verseCount = 0;
    m_staff = NULL;

    m_overflowAbove = 0;
    m_overflowBelow = 0;
    m_staffHeight = 0;
    m_overlap = 0;
}

StaffAlignment::~StaffAlignment()
{
    ClearPositioners();
}

void StaffAlignment::ClearPositioners()
{
    ArrayOfFloatingPositioners::iterator iter;
    for (iter = m_floatingPositioners.begin(); iter != m_floatingPositioners.end(); ++iter) {
        delete *iter;
    }
    m_floatingPositioners.clear();
}

void StaffAlignment::SetStaff(Staff *staff, Doc *doc)
{
    m_staff = staff;
    if (staff && doc) {
        m_staffHeight = (staff->m_drawingLines - 1) * doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize);
    }
}

int StaffAlignment::GetStaffSize() const
{
    return m_staff ? m_staff->m_drawingStaffSize : 100;
}

void StaffAlignment::SetYShift(int yShift)
{
    if (yShift < m_yShift) {
        m_yShift = yShift;
    }
}

void StaffAlignment::SetOverflowAbove(int overflowAbove)
{
    if (overflowAbove > m_overflowAbove) {
        m_overflowAbove = overflowAbove;
    }
}

void StaffAlignment::SetOverlap(int overlap)
{
    if (overlap > m_overlap) {
        m_overlap = overlap;
    }
}

void StaffAlignment::SetOverflowBelow(int overflowBottom)
{
    if (overflowBottom > m_overflowBelow) {
        m_overflowBelow = overflowBottom;
    }
}

void StaffAlignment::SetVerseCount(int verse_count)
{
    // if 0, then assume 1;
    verse_count = std::max(verse_count, 1);
    if (verse_count > m_verseCount) {
        m_verseCount = verse_count;
    }
}

int StaffAlignment::CalcOverflowAbove(BoundingBox *box)
{
    if (box->Is() == FLOATING_POSITIONER) {
        FloatingPositioner *positioner = dynamic_cast<FloatingPositioner *>(box);
        assert(positioner);
        return positioner->GetDrawingY() + positioner->m_contentBB_y2;
    }
    return box->GetDrawingY() + box->m_contentBB_y2;
}

int StaffAlignment::CalcOverflowBelow(BoundingBox *box)
{
    if (box->Is() == FLOATING_POSITIONER) {
        FloatingPositioner *positioner = dynamic_cast<FloatingPositioner *>(box);
        assert(positioner);
        return -(positioner->GetDrawingY() + positioner->m_contentBB_y1 + m_staffHeight);
    }
    return -(box->GetDrawingY() + box->m_contentBB_y1 + m_staffHeight);
}

void StaffAlignment::SetCurrentFloatingPositioner(FloatingObject *object, int x, int y)
{
    auto item = std::find_if(m_floatingPositioners.begin(), m_floatingPositioners.end(),
        [object](FloatingPositioner *positioner) { return positioner->GetObject() == object; });
    if (item != m_floatingPositioners.end()) {
        // LogDebug("Found it!");
    }
    else {
        FloatingPositioner *box = new FloatingPositioner(object);
        m_floatingPositioners.push_back(box);
        item = m_floatingPositioners.end() - 1;
    }
    (*item)->SetDrawingX(x);
    (*item)->SetDrawingY(y);
    // LogDebug("BB %d", item->second.m_contentBB_x1);
    object->SetCurrentFloatingPositioner((*item));
}

//----------------------------------------------------------------------------
// MeasureAligner
//----------------------------------------------------------------------------

MeasureAligner::MeasureAligner() : Object()
{
    m_leftAlignment = NULL;
    m_leftBarLineAlignment = NULL;
    m_rightAlignment = NULL;
    m_rightBarLineAlignment = NULL;
}

MeasureAligner::~MeasureAligner()
{
}

void MeasureAligner::Reset()
{
    Object::Reset();
    m_nonJustifiableLeftMargin = 0;
    m_leftAlignment = new Alignment(-1.0 * DUR_MAX, ALIGNMENT_MEASURE_START);
    AddAlignment(m_leftAlignment);
    m_leftBarLineAlignment = new Alignment(-1.0 * DUR_MAX, ALIGNMENT_MEASURE_LEFT_BARLINE);
    AddAlignment(m_leftBarLineAlignment);
    m_rightBarLineAlignment = new Alignment(0.0 * DUR_MAX, ALIGNMENT_MEASURE_RIGHT_BARLINE);
    AddAlignment(m_rightBarLineAlignment);
    m_rightAlignment = new Alignment(0.0 * DUR_MAX, ALIGNMENT_MEASURE_END);
    AddAlignment(m_rightAlignment);
}

void MeasureAligner::AddAlignment(Alignment *alignment, int idx)
{
    alignment->SetParent(this);
    if (idx == -1) {
        m_children.push_back(alignment);
    }
    else {
        InsertChild(alignment, idx);
    }
}

Alignment *MeasureAligner::GetAlignmentAtTime(double time, AlignmentType type)
{
    time = round(time * (pow(10, 10)) / pow(10, 10));
    int i;
    int idx = -1; // the index if we reach the end.
    Alignment *alignment = NULL;
    // First try to see if we already have something at the time position
    for (i = 0; i < GetAlignmentCount(); i++) {
        alignment = dynamic_cast<Alignment *>(m_children.at(i));
        assert(alignment);

        double alignment_time = alignment->GetTime();
        if (vrv::AreEqual(alignment_time, time)) {
            if (alignment->GetType() == type) {
                return alignment;
            }
            else if (alignment->GetType() > type) {
                idx = i;
                break;
            }
        }
        // nothing found, do not go any further but keep the index
        if (alignment->GetTime() > time) {
            idx = i;
            break;
        }
    }
    // nothing found
    if (idx == -1) {
        if ((type != ALIGNMENT_MEASURE_END) && (this->Is() != GRACE_ALIGNER)) {
            // This typically occurs when a tstamp event occurs after the last note of a measure
            int rightBarlineIdx = m_rightBarLineAlignment->GetIdx();
            assert(rightBarlineIdx != -1);
            idx = rightBarlineIdx - 1;
            this->SetMaxTime(time);
        }
        else {
            idx = GetAlignmentCount();
        }
    }
    Alignment *newAlignment = new Alignment(time, type);
    AddAlignment(newAlignment, idx);
    return newAlignment;
}

void MeasureAligner::SetMaxTime(double time)
{
    // we have to have a m_rightBarLineAlignment
    assert(m_rightBarLineAlignment);

    // it must be found in the aligner
    int idx = m_rightBarLineAlignment->GetIdx();
    assert(idx != -1);

    int i;
    Alignment *alignment = NULL;
    // Increase the time position for all alignment from the right barline
    for (i = idx; i < GetAlignmentCount(); i++) {
        alignment = dynamic_cast<Alignment *>(m_children.at(i));
        assert(alignment);
        // Change it only if higher than before
        if (time > alignment->GetTime()) alignment->SetTime(time);
    }
}

//----------------------------------------------------------------------------
// GraceAligner
//----------------------------------------------------------------------------

GraceAligner::GraceAligner() : MeasureAligner()
{
    m_totalWidth = 0;
}

GraceAligner::~GraceAligner()
{
}

void GraceAligner::StackNote(Note *note)
{
    m_noteStack.push_back(note);
}

void GraceAligner::AlignStack()
{
    int i;
    double time = 0.0;
    for (i = (int)m_noteStack.size(); i > 0; i--) {
        Note *note = dynamic_cast<Note *>(m_noteStack.at(i - 1));
        assert(note);
        // get the duration of the event
        double duration = note->LayerElement::GetAlignmentDuration(NULL, NULL, false);
        // Time goes backward with grace notes
        time -= duration;
        note->SetGraceAlignment(this->GetAlignmentAtTime(time, ALIGNMENT_DEFAULT));
    }
    m_noteStack.clear();
}

//----------------------------------------------------------------------------
// Alignment
//----------------------------------------------------------------------------

Alignment::Alignment() : Object()
{
    Reset();
}

Alignment::Alignment(double time, AlignmentType type) : Object()
{
    Reset();
    m_time = time;
    m_type = type;
}

void Alignment::Reset()
{
    Object::Reset();

    m_layerElementsRef.clear();
    m_xRel = 0;
    m_xShift = 0;
    m_maxWidth = 0;
    m_time = 0.0;
    m_type = ALIGNMENT_DEFAULT;
    m_graceAligner = NULL;
}

Alignment::~Alignment()
{
    if (m_graceAligner) {
        delete m_graceAligner;
    }
}

void Alignment::AddLayerElementRef(LayerElement *element)
{
    assert(element->IsLayerElement());
    m_layerElementsRef.push_back(element);
}

void Alignment::SetXRel(int x_rel)
{
    m_xRel = x_rel;
}

void Alignment::SetXShift(int xShift)
{
    if (xShift > m_xShift) {
        m_xShift = xShift;
    }
}

void Alignment::SetMaxWidth(int maxWidth)
{
    if (maxWidth > m_maxWidth) {
        m_maxWidth = maxWidth;
    }
}

GraceAligner *Alignment::GetGraceAligner()
{
    if (!m_graceAligner) {
        m_graceAligner = new GraceAligner();
    }
    return m_graceAligner;
}

//----------------------------------------------------------------------------
// TimestampAligner
//----------------------------------------------------------------------------

TimestampAligner::TimestampAligner() : Object()
{
    Reset();
}

TimestampAligner::~TimestampAligner()
{
}

void TimestampAligner::Reset()
{
    Object::Reset();
}

TimestampAttr *TimestampAligner::GetTimestampAtTime(double time)
{
    int i;
    int idx = -1; // the index if we reach the end.
    // We need to adjust the position since timestamp 0 to 1.0 are before 0 musical time
    time = time - 1.0;
    TimestampAttr *timestampAttr = NULL;
    // First try to see if we already have something at the time position
    for (i = 0; i < GetChildCount(); i++) {
        timestampAttr = dynamic_cast<TimestampAttr *>(m_children.at(i));
        assert(timestampAttr);

        double alignmentTime = timestampAttr->GetActualDurPos();
        if (vrv::AreEqual(alignmentTime, time)) {
            return timestampAttr;
        }
        // nothing found, do not go any further but keep the index
        if (alignmentTime > time) {
            idx = i;
            break;
        }
    }
    // nothing found
    timestampAttr = new TimestampAttr();
    timestampAttr->SetDrawingPos(time);
    timestampAttr->SetParent(this);
    if (idx == -1) {
        m_children.push_back(timestampAttr);
    }
    else {
        InsertChild(timestampAttr, idx);
    }
    return timestampAttr;
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int StaffAlignment::CalcStaffOverlap(FunctorParams *functorParams)
{
    CalcStaffOverlapParams *params = dynamic_cast<CalcStaffOverlapParams *>(functorParams);
    assert(params);

    // This is the bottom alignment (or something is wrong)
    if (!this->m_staff) return FUNCTOR_STOP;

    if (params->m_previous == NULL) {
        params->m_previous = this;
        return FUNCTOR_SIBLINGS;
    }

    ArrayOfBoundingBoxes::iterator iter;
    // go through all the elements of the top staff that have an overflow below
    for (iter = params->m_previous->m_overflowBelowBBoxes.begin();
         iter != params->m_previous->m_overflowBelowBBoxes.end(); iter++) {
        auto i = m_overflowAboveBBoxes.begin();
        auto end = m_overflowAboveBBoxes.end();
        while (i != end) {
            // find all the elements from the bottom staff that have an overflow at the top with an horizontal overap
            i = std::find_if(i, end, [iter](BoundingBox *elem) { return (*iter)->HorizontalOverlap(elem); });
            if (i != end) {
                // calculate the vertical overlap and see if this is more than the expected space
                int overflowBelow = params->m_previous->CalcOverflowBelow(*iter);
                int overflowAbove = this->CalcOverflowAbove(*i);
                int spacing = std::max(params->m_previous->m_overflowBelow, this->m_overflowAbove);
                if (spacing < (overflowBelow + overflowAbove)) {
                    // LogDebug("Overlap %d", (overflowBelow + overflowAbove) - spacing);
                    this->SetOverlap((overflowBelow + overflowAbove) - spacing);
                }
                i++;
            }
        }
    }

    params->m_previous = this;

    return FUNCTOR_SIBLINGS;
}

int StaffAlignment::AdjustFloatingPostioners(FunctorParams *functorParams)
{
    AdjustFloatingPostionersParams *params = dynamic_cast<AdjustFloatingPostionersParams *>(functorParams);
    assert(params);

    int staffSize = this->GetStaffSize();

    if (params->m_classId == SYL) {
        if (this->GetVerseCount() > 0) {
            FontInfo *lyricFont = params->m_doc->GetDrawingLyricFont(m_staff->m_drawingStaffSize);
            int descender = params->m_doc->GetTextGlyphDescender(L'q', lyricFont, false);
            int height = params->m_doc->GetTextGlyphHeight(L'I', lyricFont, false);
            int margin
                = params->m_doc->GetBottomMargin(SYL) * params->m_doc->GetDrawingUnit(staffSize) / PARAM_DENOMINATOR;
            this->SetOverflowBelow(this->m_overflowBelow + this->GetVerseCount() * (height - descender + margin));
            // For now just clear the overflowBelow, which avoids the overlap to be calculated. We could also keep them
            // and check if they are some lyrics in order to know if the overlap needs to be calculated or not.
            m_overflowBelowBBoxes.clear();
        }
        return FUNCTOR_SIBLINGS;
    }

    ArrayOfFloatingPositioners::iterator iter;
    for (iter = m_floatingPositioners.begin(); iter != m_floatingPositioners.end(); ++iter) {
        assert((*iter)->GetObject());
        if ((*iter)->GetObject()->Is() != params->m_classId) continue;

        // for slurs and ties we do not need to adjust them, only add them to the overflow boxes if required
        if ((params->m_classId == SLUR) || (params->m_classId == TIE)) {

            int overflowAbove = this->CalcOverflowAbove((*iter));
            if (overflowAbove > params->m_doc->GetDrawingStaffLineWidth(staffSize) / 2) {
                // LogMessage("%sparams->m_doctop overflow: %d", current->GetUuid().c_str(), overflowAbove);
                this->SetOverflowAbove(overflowAbove);
                this->m_overflowAboveBBoxes.push_back((*iter));
            }

            int overflowBelow = this->CalcOverflowBelow((*iter));
            if (overflowBelow > params->m_doc->GetDrawingStaffLineWidth(staffSize) / 2) {
                // LogMessage("%s bottom overflow: %d", current->GetUuid().c_str(), overflowBelow);
                this->SetOverflowBelow(overflowBelow);
                this->m_overflowBelowBBoxes.push_back((*iter));
            }
            continue;
        }

        // This sets the default position (without considering any overflowing box)
        (*iter)->CalcDrawingYRel(params->m_doc, this, NULL);

        ArrayOfBoundingBoxes *overflowBoxes = &m_overflowBelowBBoxes;
        // above?
        if ((*iter)->GetDrawingPlace() == STAFFREL_above) {
            overflowBoxes = &m_overflowAboveBBoxes;
        }
        auto i = overflowBoxes->begin();
        auto end = overflowBoxes->end();
        while (i != end) {
            // find all the overflowing elements from the staff that overlap horizonatally
            i = std::find_if(i, end, [iter](BoundingBox *elem) { return (*iter)->HorizontalOverlap(elem); });
            if (i != end) {
                // update the yRel accordingly
                (*iter)->CalcDrawingYRel(params->m_doc, this, *i);
                i++;
            }
        }
        //  Now update the staffAlignment max overflow (above or below) and add the positioner to the list of
        //  overflowing elements
        if ((*iter)->GetDrawingPlace() == STAFFREL_above) {
            int overflowAbove = this->CalcOverflowAbove((*iter));
            overflowBoxes->push_back((*iter));
            this->SetOverflowAbove(overflowAbove);
        }
        else {
            int overflowBelow = this->CalcOverflowBelow((*iter));
            overflowBoxes->push_back((*iter));
            this->SetOverflowBelow(overflowBelow);
        }
    }

    return FUNCTOR_SIBLINGS;
}

int StaffAlignment::AdjustFloatingPostionerGrps(FunctorParams *functorParams)
{
    AdjustFloatingPostionerGrpsParams *params = dynamic_cast<AdjustFloatingPostionerGrpsParams *>(functorParams);
    assert(params);

    ArrayOfFloatingPositioners positioners;
    // make a temporary copy of positionners with a classId desired and that have a drawing grpId
    std::copy_if(m_floatingPositioners.begin(), m_floatingPositioners.end(), std::back_inserter(positioners),
        [params](FloatingPositioner *positioner) {
            assert(positioner->GetObject());
            // search in the desired classIds
            return ((std::find(params->m_classIds.begin(), params->m_classIds.end(), positioner->GetObject()->Is())
                        != params->m_classIds.end())
                && (positioner->GetObject()->GetDrawingGrpId() != 0));
        });

    // A vector for storing a pair with the grpId and the min or max YRel
    std::vector<std::pair<int, int> > grpIdYRel;

    ArrayOfFloatingPositioners::iterator iter;
    for (iter = positioners.begin(); iter != positioners.end(); ++iter) {
        int currentGrpId = (*iter)->GetObject()->GetDrawingGrpId();
        // Look if we already have a pair for this grpId
        auto i = std::find_if(grpIdYRel.begin(), grpIdYRel.end(),
            [currentGrpId](std::pair<int, int> &pair) { return (pair.first == currentGrpId); });
        // if not, then just add a new pair with the YRel of the current positioner
        if (i == grpIdYRel.end()) {
            grpIdYRel.push_back(std::make_pair(currentGrpId, (*iter)->GetDrawingYRel()));
        }
        // else, adjust the min or max YRel of the pair if necessary
        else {
            if ((*iter)->GetDrawingPlace() == STAFFREL_above) {
                if ((*iter)->GetDrawingYRel() < (*i).second) (*i).second = (*iter)->GetDrawingYRel();
            }
            else {
                if ((*iter)->GetDrawingYRel() > (*i).second) (*i).second = (*iter)->GetDrawingYRel();
            }
        }
    }

    // Now go through all the positioners again and ajust the YRel with the value of the pair
    for (iter = positioners.begin(); iter != positioners.end(); ++iter) {
        int currentGrpId = (*iter)->GetObject()->GetDrawingGrpId();
        auto i = std::find_if(grpIdYRel.begin(), grpIdYRel.end(),
            [currentGrpId](std::pair<int, int> &pair) { return (pair.first == currentGrpId); });
        // We must have find it
        assert(i != grpIdYRel.end());
        (*iter)->SetDrawingYRel((*i).second);
    }

    return FUNCTOR_SIBLINGS;
}

int StaffAlignment::SetAligmentYPos(FunctorParams *functorParams)
{
    SetAligmentYPosParams *params = dynamic_cast<SetAligmentYPosParams *>(functorParams);
    assert(params);

    int maxOverlfowAbove;
    if (params->m_previousVerseCount > 0) {
        maxOverlfowAbove = params->m_previousOverflowBelow + m_overflowAbove;
    }
    else {
        // The maximum between the overflow below of the previous staff and the overflow above of the current
        maxOverlfowAbove = std::max(params->m_previousOverflowBelow, m_overflowAbove);

        // If we have some overlap, add it
        if (m_overlap) maxOverlfowAbove += m_overlap;
    }

    // Is the maximum the overflow (+ overlap) shift, or the default ?
    int shift = std::max(maxOverlfowAbove, params->m_doc->GetSpacingStaff() * params->m_doc->GetDrawingUnit(100));

    // Add a margin
    shift += params->m_doc->GetBottomMargin(STAFF) * params->m_doc->GetDrawingUnit(this->GetStaffSize())
        / PARAM_DENOMINATOR;

    // Shift, including the previous staff height
    SetYShift(-shift - params->m_previousStaffHeight);

    params->m_previousStaffHeight = m_staffHeight;
    params->m_previousOverflowBelow = m_overflowBelow;
    params->m_previousVerseCount = this->GetVerseCount();

    return FUNCTOR_CONTINUE;
}

int StaffAlignment::IntegrateBoundingBoxYShift(FunctorParams *functorParams)
{
    IntegrateBoundingBoxYShiftParams *params = dynamic_cast<IntegrateBoundingBoxYShiftParams *>(functorParams);
    assert(params);

    // integrates the m_yShift into the m_yRel
    m_yRel += m_yShift + params->m_shift;

    // cumulate the shift value
    params->m_shift += m_yShift;
    m_yShift = 0;

    return FUNCTOR_CONTINUE;
}

int MeasureAligner::IntegrateBoundingBoxXShift(FunctorParams *functorParams)
{
    IntegrateBoundingBoxXShiftParams *params = dynamic_cast<IntegrateBoundingBoxXShiftParams *>(functorParams);
    assert(params);

    params->m_shift = 0;

    return FUNCTOR_CONTINUE;
}

int Alignment::IntegrateBoundingBoxGraceXShift(FunctorParams *functorParams)
{
    if (!m_graceAligner) {
        return FUNCTOR_CONTINUE;
    }

    int i;
    int shift = 0;
    for (i = 0; i < m_graceAligner->GetChildCount(); i++) {
        Alignment *alignment = dynamic_cast<Alignment *>(m_graceAligner->GetChild(i));
        assert(alignment);
        alignment->SetXRel(alignment->GetXShift() + shift);
        shift += alignment->GetXShift();
    }

    // Set the total width by looking at the position and maximum width of the last alignment
    if (m_graceAligner->GetChildCount() == 0) {
        return FUNCTOR_CONTINUE;
    }
    Alignment *alignment = dynamic_cast<Alignment *>(m_graceAligner->GetLast());
    assert(alignment);
    m_graceAligner->SetWidth(alignment->GetXRel() + alignment->GetMaxWidth());

    return FUNCTOR_CONTINUE;
}

int Alignment::IntegrateBoundingBoxXShift(FunctorParams *functorParams)
{
    IntegrateBoundingBoxXShiftParams *params = dynamic_cast<IntegrateBoundingBoxXShiftParams *>(functorParams);
    assert(params);

    // We move the first left position according to style but not for aligners that are empty and not
    // for the left barline because we want it to be at the 0 pos if nothing before it.
    if ((params->m_shift == 0) && (m_type != ALIGNMENT_MEASURE_LEFT_BARLINE) && !m_layerElementsRef.empty()) {
        params->m_shift = params->m_doc->GetLeftPosition() * params->m_doc->GetDrawingUnit(100) / PARAM_DENOMINATOR;
    }

    // integrates the m_xShift into the m_xRel
    m_xRel += m_xShift + params->m_shift;
    // cumulate the shift value and the width
    params->m_shift += m_xShift;

    // reset member to 0
    m_xShift = 0;

    return FUNCTOR_CONTINUE;
}

int Alignment::SetBoundingBoxXShift(FunctorParams *functorParams)
{
    SetBoundingBoxXShiftParams *params = dynamic_cast<SetBoundingBoxXShiftParams *>(functorParams);
    assert(params);

    // Here we want to process only the left scoreDef up to the left barline
    if (this->m_type > ALIGNMENT_MEASURE_LEFT_BARLINE) return FUNCTOR_CONTINUE;

    ArrayOfObjects::iterator iter;

    // Because we are processing the elements vertically we need to reset minPos for each element
    int previousMinPos = params->m_minPos;
    for (iter = m_layerElementsRef.begin(); iter != m_layerElementsRef.end(); iter++) {
        params->m_minPos = previousMinPos;
        (*iter)->Process(params->m_functor, params);
    }

    // If we have elements for this alignment, then adjust the minPos
    if (!m_layerElementsRef.empty()) {
        params->m_minPos = this->GetXRel() + this->GetMaxWidth();
    }
    // Otherwise, just set its xShift because this was not done by the functor
    else {
        this->SetXShift(params->m_minPos - this->GetXRel());
        params->m_minPos = this->GetXRel();
    }

    return FUNCTOR_CONTINUE;
}

int Alignment::SetBoundingBoxXShiftEnd(FunctorParams *functorParams)
{
    SetBoundingBoxXShiftParams *params = dynamic_cast<SetBoundingBoxXShiftParams *>(functorParams);
    assert(params);

    // Because these do not get shifted with their bounding box because their bounding box is calculated
    // according to
    // the width of the measure, their xShift has to be set 'by hand'
    if (GetType() == ALIGNMENT_FULLMEASURE) {
        params->m_minPos = std::max(this->GetXRel() + params->m_doc->m_drawingMinMeasureWidth, params->m_minPos);
    }
    else if (GetType() == ALIGNMENT_FULLMEASURE2) {
        params->m_minPos = std::max(this->GetXRel() + 2 * params->m_doc->m_drawingMinMeasureWidth, params->m_minPos);
    }

    // Here we want to process only the alignments from the right barline to the end - this includes the right
    // scoreDef
    // if any
    if (this->m_type < ALIGNMENT_MEASURE_RIGHT_BARLINE) return FUNCTOR_CONTINUE;

    ArrayOfObjects::iterator iter;

    // Because we are processing the elements vertically we need to reset minPos for each element
    int previousMinPos = params->m_minPos;
    for (iter = m_layerElementsRef.begin(); iter != m_layerElementsRef.end(); iter++) {
        params->m_minPos = previousMinPos;
        (*iter)->Process(params->m_functor, params);
    }

    // If we have elements for this alignment, then adjust the minPos
    if (!m_layerElementsRef.empty()) {
        params->m_minPos = this->GetXRel() + this->GetMaxWidth();
    }
    // Otherwise, just set its xShift because this was not done by the functor
    // This includes ALIGNMENT_MEASURE_END alignments
    else {
        this->SetXShift(params->m_minPos - this->GetXRel());
    }

    return FUNCTOR_CONTINUE;
}

int MeasureAligner::SetAlignmentXPos(FunctorParams *functorParams)
{
    SetAlignmentXPosParams *params = dynamic_cast<SetAlignmentXPosParams *>(functorParams);
    assert(params);

    // We start a new MeasureAligner
    // Reset the previous time position and x_rel to 0;
    params->m_previousTime = 0.0;
    params->m_previousXRel = 0;

    return FUNCTOR_CONTINUE;
}

/* Compute "ideal" horizontal space to allow for a given time interval, ignoring the need
to keep consecutive symbols from overlapping or nearly overlapping: we assume spacing
will be increased as necessary later to avoid that. For modern notation (CMN), ideal space
is a function of time interval.

For a discussion of the way engravers determine spacing, see Elaine Gould, _Behind Bars_,
p. 39. But we need something more flexible, because, for example: (1) We're interested in
music with notes of very long duration: say, music in mensural notation containing longas
or maximas; such music is usually not spaced by duration, but we support spacing by
duration if the user wishes, and standard engravers' rules would waste a lot of space.
(2) For some purposes, spacing strictly proportional to duration is desirable. The most
flexible solution might be to get ideal spacing from a user-definable table, but using a
formula with parameters can come close and has other advantages. */

int Alignment::HorizontalSpaceForDuration(
    double intervalTime, int maxActualDur, double spacingLinear, double spacingNonLinear)
{
    /* If the longest duration interval in the score is longer than semibreve, adjust spacing so
       that interval gets the space a semibreve would ordinarily get. */
    if (maxActualDur < DUR_1) intervalTime /= pow(2.0, DUR_1 - maxActualDur);
    int intervalXRel;
    intervalXRel = pow(intervalTime, spacingNonLinear) * spacingLinear * 10.0; // numbers are experimental constants
    return intervalXRel;
}

int Alignment::SetAlignmentXPos(FunctorParams *functorParams)
{
    SetAlignmentXPosParams *params = dynamic_cast<SetAlignmentXPosParams *>(functorParams);
    assert(params);

    // Do not set an x pos for anything before the barline (including it)
    if (this->m_type <= ALIGNMENT_MEASURE_LEFT_BARLINE) return FUNCTOR_CONTINUE;
    // Do not set an x pos for anything after the barline (but still for it)
    if (this->m_type > ALIGNMENT_MEASURE_RIGHT_BARLINE) return FUNCTOR_CONTINUE;

    int intervalXRel = 0;
    double intervalTime = (m_time - params->m_previousTime);

    // For clef changes, do not take into account the interval so we keep them left aligned
    // This is not perfect because the previous time is the one of the previous aligner and
    // there is maybe space between the last note and the clef on their layer
    if (this->m_type == ALIGNMENT_CLEF) intervalTime = 0.0;

    if (intervalTime > 0.0) {
        intervalXRel = HorizontalSpaceForDuration(intervalTime, params->m_longestActualDur,
            params->m_doc->GetSpacingLinear(), params->m_doc->GetSpacingNonLinear());
        // LogDebug("SetAlignmentXPos: intervalTime=%.2f intervalXRel=%d", intervalTime, intervalXRel);
    }
    m_xRel = params->m_previousXRel + intervalXRel * DEFINITION_FACTOR;
    params->m_previousTime = m_time;
    params->m_previousXRel = m_xRel;

    return FUNCTOR_CONTINUE;
}

int MeasureAligner::JustifyX(FunctorParams *functorParams)
{
    JustifyXParams *params = dynamic_cast<JustifyXParams *>(functorParams);
    assert(params);

    params->m_leftBarLineX = GetLeftBarLineAlignment()->GetXRel();
    params->m_rightBarLineX = GetRightBarLineAlignment()->GetXRel();

    return FUNCTOR_CONTINUE;
}

int Alignment::JustifyX(FunctorParams *functorParams)
{
    JustifyXParams *params = dynamic_cast<JustifyXParams *>(functorParams);
    assert(params);

    if (m_type <= ALIGNMENT_MEASURE_LEFT_BARLINE) {
        // Nothing to do for all left scoreDef elements and the left barline
    }
    else if (m_type < ALIGNMENT_MEASURE_RIGHT_BARLINE) {
        // All elements up to the next barline, move them but also take into account the leftBarlineX
        this->m_xRel = ceil((((double)this->m_xRel - (double)params->m_leftBarLineX) * params->m_justifiableRatio)
            + params->m_leftBarLineX);
    }
    else {
        //  Now more the right barline and all right scoreDef elements
        int shift = this->m_xRel - params->m_rightBarLineX;
        this->m_xRel
            = ceil(((double)params->m_rightBarLineX - (double)params->m_leftBarLineX) * params->m_justifiableRatio)
            + params->m_leftBarLineX + shift;
    }

    // Finally, when reaching the end of the measure, update the measureXRel for the next measure
    if (m_type == ALIGNMENT_MEASURE_END) {
        params->m_measureXRel += this->m_xRel;
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

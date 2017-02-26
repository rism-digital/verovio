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

#include "attcomparison.h"
#include "doc.h"
#include "floatingobject.h"
#include "functorparams.h"
#include "measure.h"
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
    if (box->Is(FLOATING_POSITIONER)) {
        FloatingPositioner *positioner = dynamic_cast<FloatingPositioner *>(box);
        assert(positioner);
        return positioner->GetContentTop();
    }
    return box->GetContentTop();
}

int StaffAlignment::CalcOverflowBelow(BoundingBox *box)
{
    if (box->Is(FLOATING_POSITIONER)) {
        FloatingPositioner *positioner = dynamic_cast<FloatingPositioner *>(box);
        assert(positioner);
        return -(positioner->GetContentBottom() + m_staffHeight);
    }
    return -(box->GetContentBottom() + m_staffHeight);
}

void StaffAlignment::SetCurrentFloatingPositioner(FloatingObject *object, Object *objectX, Object *objectY)
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
    (*item)->SetObjectXY(objectX, objectY);
    // LogDebug("BB %d", item->second.m_contentBB_x1);
    object->SetCurrentFloatingPositioner((*item));
}

//----------------------------------------------------------------------------
// HorizontalAligner
//----------------------------------------------------------------------------

HorizontalAligner::HorizontalAligner() : Object()
{
    Reset();
}

HorizontalAligner::~HorizontalAligner()
{
}

void HorizontalAligner::Reset()
{
    Object::Reset();
}

Alignment *HorizontalAligner::SearchAlignmentAtTime(double time, AlignmentType type, int &idx)
{
    int i;
    idx = -1; // the index if we reach the end.
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
    return NULL;
}

void HorizontalAligner::AddAlignment(Alignment *alignment, int idx)
{
    alignment->SetParent(this);
    if (idx == -1) {
        m_children.push_back(alignment);
    }
    else {
        InsertChild(alignment, idx);
    }
}

//----------------------------------------------------------------------------
// MeasureAligner
//----------------------------------------------------------------------------

MeasureAligner::MeasureAligner() : HorizontalAligner()
{
    m_leftAlignment = NULL;
    m_leftBarLineAlignment = NULL;
    m_rightAlignment = NULL;
    m_rightBarLineAlignment = NULL;

    Reset();
}

MeasureAligner::~MeasureAligner()
{
}

void MeasureAligner::Reset()
{
    HorizontalAligner::Reset();
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

Alignment *MeasureAligner::GetAlignmentAtTime(double time, AlignmentType type)
{
    int idx; // the index if we reach the end.
    time = round(time * (pow(10, 10)) / pow(10, 10));
    Alignment *alignment = this->SearchAlignmentAtTime(time, type, idx);
    // we already have a alignment of the type at that time
    if (alignment != NULL) return alignment;
    // nothing found to the end
    if (idx == -1) {
        if (type != ALIGNMENT_MEASURE_END) {
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

double MeasureAligner::GetMaxTime() const
{
    // we have to have a m_rightBarLineAlignment
    assert(m_rightBarLineAlignment);

    return m_rightAlignment->GetTime();
}

void MeasureAligner::AdjustProportionally(const ArrayOfAdjustmentTuples &adjustments)
{
    ArrayOfAdjustmentTuples::const_iterator iter;
    for (iter = adjustments.begin(); iter != adjustments.end(); iter++) {
        Alignment *start = std::get<0>(*iter);
        assert(start);
        Alignment *end = std::get<1>(*iter);
        assert(end);
        int dist = std::get<2>(*iter);
        if ((start->GetXRel() >= end->GetXRel()) || (dist == 0)) {
            LogDebug("Trying to ajdust alignment at the same position or with a distance of 0;");
            continue;
        }
        // We need to store them because they are going to be changed in the loop below
        int startX = start->GetXRel();
        int endX = end->GetXRel();
        // We use a reverse iterator
        ArrayOfObjects::iterator alignIter;
        for (alignIter = m_children.begin(); alignIter != m_children.end(); alignIter++) {
            Alignment *current = dynamic_cast<Alignment *>(*alignIter);
            assert(current);
            // Nothing to do once we passed the start aligment
            if (current->GetXRel() <= startX)
                continue;
            else if (current->GetXRel() >= endX) {
                current->SetXRel(current->GetXRel() + dist);
                continue;
            }
            else {
                int ratio = (current->GetXRel() - startX) * 100 / (endX - startX);
                int shift = dist * ratio / 100;
                current->SetXRel(current->GetXRel() + shift);
            }
        }
    }
}

void MeasureAligner::PushAlignmentsRight()
{
    Alignment *previous = NULL;
    ArrayOfObjects::reverse_iterator riter;
    for (riter = m_children.rbegin(); riter != m_children.rend(); riter++) {
        Alignment *current = dynamic_cast<Alignment *>(*riter);
        assert(current);

        if ((current->GetType() == ALIGNMENT_GRACENOTE) || (current->GetType() == ALIGNMENT_CONTAINER)) {
            if (previous) current->SetXRel(previous->GetXRel());
        }
        else {
            previous = current;
        }
    }
}

void MeasureAligner::AdjustGraceNoteSpacing(Doc *doc, Alignment *alignment, int staffN)
{
    assert(doc);
    assert(alignment);
    assert(alignment->GetType() == ALIGNMENT_GRACENOTE);
    assert(alignment->GetGraceAligner());

    Measure *measure = dynamic_cast<Measure *>(this->m_parent);
    assert(measure);

    int maxRight = VRV_UNSET;
    Alignment *rightAlignment = NULL;

    // We can set staffN as VRV_UNSET to align all staves (this should be an option)
    // We can also define somewhere vector of staffDef@n to be aligned together
    // For this we would need an alternate version GetLeftRight that can take a vector of staffNs
    // staffN = VRV_UNSET;

    bool found = false;
    ArrayOfObjects::reverse_iterator riter;
    for (riter = m_children.rbegin(); riter != m_children.rend(); riter++) {
        if (!found) {
            if ((*riter) == alignment) found = true;
            continue;
        }

        rightAlignment = dynamic_cast<Alignment *>(*riter);
        assert(rightAlignment);

        // Do not go beyond the left bar line
        if (rightAlignment->GetType() == ALIGNMENT_MEASURE_LEFT_BARLINE) {
            maxRight = measure->GetLeftBarLineRight();
            break;
        }

        int minLeft;
        rightAlignment->GetLeftRight(staffN, minLeft, maxRight);

        if (maxRight != VRV_UNSET) break;
    }

    // This should never happen because we must have hit the left barline in the loop above
    if (!rightAlignment || (maxRight == VRV_UNSET)) return;

    // Check if the left position of the group is on the right of the previous maxRight
    // If not, move the aligments accordingly
    int left = alignment->GetGraceAligner()->GetGraceGroupLeft(staffN);
    // We also set artificially the margin with the previous note
    if (left != -VRV_UNSET) left -= doc->GetLeftMargin(NOTE) * doc->GetDrawingUnit(100) / PARAM_DENOMINATOR;
    if (left < maxRight) {
        int spacing = (maxRight - left);
        ArrayOfAdjustmentTuples boundaries{ std::make_tuple(rightAlignment, alignment, spacing) };
        this->AdjustProportionally(boundaries);
    }
}

//----------------------------------------------------------------------------
// GraceAligner
//----------------------------------------------------------------------------

GraceAligner::GraceAligner() : HorizontalAligner()
{
    Reset();
}

GraceAligner::~GraceAligner()
{
}

void GraceAligner::Reset()
{
    HorizontalAligner::Reset();
    m_totalWidth = 0;
}

Alignment *GraceAligner::GetAlignmentAtTime(double time, AlignmentType type)
{
    int idx; // the index if we reach the end.
    time = round(time * (pow(10, 10)) / pow(10, 10));
    Alignment *alignment = this->SearchAlignmentAtTime(time, type, idx);
    // we already have a alignment of the type at that time
    if (alignment != NULL) return alignment;
    // nothing found until the end
    if (idx == -1) {
        idx = GetAlignmentCount();
    }
    Alignment *newAlignment = new Alignment(time, type);
    AddAlignment(newAlignment, idx);
    return newAlignment;
}

void GraceAligner::StackGraceElement(LayerElement *element)
{
    // Nespresso: What else?
    assert(element->Is(NOTE) || (element->Is(CHORD)));

    if (element->Is(NOTE)) {
        Note *note = dynamic_cast<Note *>(element);
        assert(note);
        if (note->IsChordTone()) return;
    }

    m_graceStack.push_back(element);
}

void GraceAligner::AlignStack()
{
    int i;
    double time = 0.0;
    for (i = (int)m_graceStack.size(); i > 0; i--) {
        LayerElement *element = dynamic_cast<LayerElement *>(m_graceStack.at(i - 1));
        assert(element);
        // get the duration of the event
        double duration = element->GetAlignmentDuration(NULL, NULL, false);
        // Time goes backward with grace notes
        time -= duration;
        Alignment *alignment = this->GetAlignmentAtTime(time, ALIGNMENT_DEFAULT);
        element->SetGraceAlignment(alignment);
        alignment->AddLayerElementRef(element);

        AttComparisonAny matchType({ NOTE, ACCID });
        ArrayOfObjects children;
        ArrayOfObjects::iterator childrenIter;
        element->FindAllChildByAttComparison(&children, &matchType);

        // Then the @n of each first staffDef
        for (childrenIter = children.begin(); childrenIter != children.end(); childrenIter++) {
            // Trick : FindAllChildByAttComparison include the element, which is probably a problem.
            // With note, we want to set only accid, so make sure we do not set it twice
            if (*childrenIter == element) continue;
            LayerElement *childElement = dynamic_cast<LayerElement *>(*childrenIter);
            assert(childElement);
            childElement->SetGraceAlignment(alignment);
        }
    }
    m_graceStack.clear();
}

int GraceAligner::GetGraceGroupLeft(int staffN)
{
    Alignment *leftAlignment = dynamic_cast<Alignment *>(this->GetFirst());
    if (!leftAlignment) return -VRV_UNSET;

    int minLeft, maxRight;
    leftAlignment->GetLeftRight(staffN, minLeft, maxRight);

    return minLeft;
}

int GraceAligner::GetGraceGroupRight(int staffN)
{
    Alignment *rightAlignment = dynamic_cast<Alignment *>(this->GetLast());
    if (!rightAlignment) return VRV_UNSET;

    int minLeft, maxRight;
    rightAlignment->GetLeftRight(staffN, minLeft, maxRight);

    return maxRight;
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

    m_xRel = 0;
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

void Alignment::AddChild(Object *child)
{
    assert(dynamic_cast<AlignmentReference *>(child));

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

void Alignment::AddLayerElementRef(LayerElement *element)
{
    assert(element->IsLayerElement());

    // -1 will be used for barlines attributes
    int n = -1;
    Staff *staffRef = element->m_crossStaff;
    if (!staffRef) staffRef = dynamic_cast<Staff *>(element->GetFirstParent(STAFF));
    if (staffRef) n = staffRef->GetN();
    AlignmentReference *alignmentRef = new AlignmentReference(n, element);
    this->AddChild(alignmentRef);
}

void Alignment::SetXRel(int xRel)
{
    m_xRel = xRel;
}

void Alignment::GetLeftRight(int staffN, int &minLeft, int &maxRight)
{
    Functor getAlignmentLeftRight(&Object::GetAlignmentLeftRight);
    GetAlignmentLeftRightParams getAlignmentLeftRightParams(&getAlignmentLeftRight);

    if (staffN != VRV_UNSET) {
        std::vector<AttComparison *> filters;
        AttCommonNComparison matchStaff(ALIGNMENT_REFERENCE, staffN);
        filters.push_back(&matchStaff);
        this->Process(&getAlignmentLeftRight, &getAlignmentLeftRightParams, NULL, &filters);
    }
    else
        this->Process(&getAlignmentLeftRight, &getAlignmentLeftRightParams);

    minLeft = getAlignmentLeftRightParams.m_minLeft;
    maxRight = getAlignmentLeftRightParams.m_maxRight;
}

GraceAligner *Alignment::GetGraceAligner()
{
    if (!m_graceAligner) {
        m_graceAligner = new GraceAligner();
    }
    return m_graceAligner;
}

//----------------------------------------------------------------------------
// AlignmentReference
//----------------------------------------------------------------------------

AlignmentReference::AlignmentReference() : Object(), AttCommon()
{
    RegisterAttClass(ATT_COMMON);

    Reset();
}

AlignmentReference::AlignmentReference(int n, Object *elementRef) : Object(), AttCommon()
{
    RegisterAttClass(ATT_COMMON);

    Reset();
    this->SetN(n);
    m_elementRef = elementRef;
}

void AlignmentReference::Reset()
{
    Object::Reset();
    ResetCommon();

    m_elementRef = NULL;
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
        if (!(*iter)->GetObject()->Is(params->m_classId)) continue;

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
            return (
                (std::find(params->m_classIds.begin(), params->m_classIds.end(), positioner->GetObject()->GetClassId())
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

int Alignment::AdjustGraceXPos(FunctorParams *functorParams)
{
    AdjustGraceXPosParams *params = dynamic_cast<AdjustGraceXPosParams *>(functorParams);
    assert(params);

    // We are in a Measure aligner - redirect to the GraceAligner when it is a ALIGNMENT_GRACENOTE
    if (!params->m_isGraceAlignment) {
        // Do not process AlignmentReference children if no GraceAligner
        if (!m_graceAligner) {
            // We store the default alignment before we hit the grace alignment
            if (m_type == ALIGNMENT_DEFAULT) params->m_rightDefaultAlignment = this;
            return FUNCTOR_SIBLINGS;
        }
        assert(m_type == ALIGNMENT_GRACENOTE);

        // Change the flag for indicating that the alignment is child of a GraceAligner
        params->m_isGraceAlignment = true;

        // Get the parent measure Aligner
        MeasureAligner *measureAligner = dynamic_cast<MeasureAligner *>(this->GetFirstParent(MEASURE_ALIGNER));
        assert(measureAligner);

        std::vector<int>::iterator iter;
        std::vector<AttComparison *> filters;
        for (iter = params->m_staffNs.begin(); iter != params->m_staffNs.end(); iter++) {

            int graceMaxPos = this->GetXRel();
            // If we have a rightDefault, then this is (quite likely) the next note / chord
            // Get its minimum left and make it the max right position of the grace group
            if (params->m_rightDefaultAlignment) {
                int minLeft, maxRight;
                params->m_rightDefaultAlignment->GetLeftRight(*iter, minLeft, maxRight);
                if (minLeft != -VRV_UNSET)
                    graceMaxPos = minLeft
                        - params->m_doc->GetLeftMargin(NOTE) * params->m_doc->GetDrawingUnit(75) / PARAM_DENOMINATOR;
            }
            else {
                // This happens when grace notes are at the end of a measure before a barline
                graceMaxPos -= params->m_doc->GetDrawingUnit(100);
            }

            params->m_graceMaxPos = graceMaxPos;
            params->m_graceUpcomingMaxPos = -VRV_UNSET;
            params->m_graceCumulatedXShift = 0;
            filters.clear();
            // Create ad comparison object for each type / @n
            AttCommonNComparison matchStaff(ALIGNMENT_REFERENCE, (*iter));
            filters.push_back(&matchStaff);

            m_graceAligner->Process(
                params->m_functor, params, params->m_functorEnd, &filters, UNLIMITED_DEPTH, BACKWARD);

            // There was not grace notes for that staff
            if (params->m_graceCumulatedXShift == 0) continue;

            // Now we need to adjust the space for the grace not group
            measureAligner->AdjustGraceNoteSpacing(params->m_doc, this, (*iter));
        }

        // Change the flag back
        params->m_isGraceAlignment = false;

        return FUNCTOR_CONTINUE;
    }

    // This is happening when aligning the grace aligner itself
    this->SetXRel(this->GetXRel() + params->m_graceCumulatedXShift);

    return FUNCTOR_CONTINUE;
}

int Alignment::AdjustGraceXPosEnd(FunctorParams *functorParams)
{
    AdjustGraceXPosParams *params = dynamic_cast<AdjustGraceXPosParams *>(functorParams);
    assert(params);

    if (params->m_graceUpcomingMaxPos != -VRV_UNSET) {
        params->m_graceMaxPos = params->m_graceUpcomingMaxPos;
        // We reset it for the next aligner
        params->m_graceUpcomingMaxPos = -VRV_UNSET;
    }

    return FUNCTOR_CONTINUE;
}

int Alignment::AdjustXPos(FunctorParams *functorParams)
{
    AdjustXPosParams *params = dynamic_cast<AdjustXPosParams *>(functorParams);
    assert(params);

    // LogDebug("Alignment type %d", m_type);

    this->SetXRel(this->GetXRel() + params->m_cumulatedXShift);

    if (GetType() == ALIGNMENT_CONTAINER) {
        return FUNCTOR_SIBLINGS;
    }
    else if (m_type == ALIGNMENT_MEASURE_END) {
        this->SetXRel(params->m_minPos);
    }

    return FUNCTOR_CONTINUE;
}

int Alignment::AdjustXPosEnd(FunctorParams *functorParams)
{
    AdjustXPosParams *params = dynamic_cast<AdjustXPosParams *>(functorParams);
    assert(params);

    if (params->m_upcomingMinPos != VRV_UNSET) {
        params->m_minPos = params->m_upcomingMinPos;
        // We reset it for the next aligner
        params->m_upcomingMinPos = VRV_UNSET;
    }

    // No upcoming bounding boxes, we keep the previous ones (e.g., the alignment has nothing for this staff)
    // Eventually we might want to have a more sophisticated pruning algorithm
    if (params->m_upcomingBoundingBoxes.empty()) return FUNCTOR_CONTINUE;

    params->m_boundingBoxes = params->m_upcomingBoundingBoxes;
    params->m_upcomingBoundingBoxes.clear();

    return FUNCTOR_CONTINUE;
}

int AlignmentReference::GetAlignmentLeftRight(FunctorParams *functorParams)
{
    GetAlignmentLeftRightParams *params = dynamic_cast<GetAlignmentLeftRightParams *>(functorParams);
    assert(params);

    this->GetObject()->Process(params->m_functor, params);

    return FUNCTOR_CONTINUE;
}

int AlignmentReference::AdjustGraceXPos(FunctorParams *functorParams)
{
    AdjustGraceXPosParams *params = dynamic_cast<AdjustGraceXPosParams *>(functorParams);
    assert(params);

    // LogDebug("AlignmentRef staff %d", GetN());
    this->m_elementRef->Process(params->m_functor, params);

    return FUNCTOR_CONTINUE;
}

int AlignmentReference::AdjustXPos(FunctorParams *functorParams)
{
    AdjustXPosParams *params = dynamic_cast<AdjustXPosParams *>(functorParams);
    assert(params);

    // LogDebug("AlignmentRef staff %d", GetN());
    this->m_elementRef->Process(params->m_functor, params, params->m_functorEnd);

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

    int intervalXRel = 0;
    double intervalTime = (m_time - params->m_previousTime);

    if (this->m_type > ALIGNMENT_MEASURE_RIGHT_BARLINE) {
        intervalTime = 0.0;
    }

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

/////////////////////////////////////////////////////////////////////////////
// Name:        horizontalaligner.cpp
// Author:      Laurent Pugin
// Created:     2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "horizontalaligner.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <math.h>
#include <numeric>

//----------------------------------------------------------------------------

#include "arpeg.h"
#include "comparison.h"
#include "doc.h"
#include "elementpart.h"
#include "floatingobject.h"
#include "functor.h"
#include "layer.h"
#include "measure.h"
#include "miscfunctor.h"
#include "note.h"
#include "options.h"
#include "smufl.h"
#include "staff.h"
#include "staffdef.h"
#include "timestamp.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// HorizontalAligner
//----------------------------------------------------------------------------

HorizontalAligner::HorizontalAligner(ClassId classId) : Object(classId)
{
    this->Reset();
}

HorizontalAligner::~HorizontalAligner() {}

void HorizontalAligner::Reset()
{
    Object::Reset();
}

Alignment *HorizontalAligner::SearchAlignmentAtTime(const Fraction &time, AlignmentType type, int &idx)
{
    return const_cast<Alignment *>(std::as_const(*this).SearchAlignmentAtTime(time, type, idx));
}

const Alignment *HorizontalAligner::SearchAlignmentAtTime(const Fraction &time, AlignmentType type, int &idx) const
{
    idx = -1; // the index if we reach the end.
    const Alignment *alignment = NULL;
    // First try to see if we already have something at the time position
    for (int i = 0; i < this->GetAlignmentCount(); ++i) {
        alignment = vrv_cast<const Alignment *>(this->GetChild(i));
        assert(alignment);

        Fraction alignment_time = alignment->GetTime();
        if (alignment_time == time) {
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
    if (idx == -1) {
        this->AddChild(alignment);
    }
    else {
        this->InsertChild(alignment, idx);
    }
}

FunctorCode HorizontalAligner::Accept(Functor &functor)
{
    return functor.VisitHorizontalAligner(this);
}

FunctorCode HorizontalAligner::Accept(ConstFunctor &functor) const
{
    return functor.VisitHorizontalAligner(this);
}

FunctorCode HorizontalAligner::AcceptEnd(Functor &functor)
{
    return functor.VisitHorizontalAlignerEnd(this);
}

FunctorCode HorizontalAligner::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitHorizontalAlignerEnd(this);
}

//----------------------------------------------------------------------------
// MeasureAligner
//----------------------------------------------------------------------------

MeasureAligner::MeasureAligner() : HorizontalAligner(MEASURE_ALIGNER)
{
    m_leftAlignment = NULL;
    m_leftBarLineAlignment = NULL;
    m_rightAlignment = NULL;
    m_rightBarLineAlignment = NULL;

    this->Reset();
}

MeasureAligner::~MeasureAligner() {}

void MeasureAligner::Reset()
{
    HorizontalAligner::Reset();
    m_nonJustifiableLeftMargin = 0;
    m_leftAlignment = new Alignment(-1, ALIGNMENT_MEASURE_START);
    this->AddAlignment(m_leftAlignment);
    m_leftBarLineAlignment = new Alignment(-1, ALIGNMENT_MEASURE_LEFT_BARLINE);
    this->AddAlignment(m_leftBarLineAlignment);
    m_rightBarLineAlignment = new Alignment(0, ALIGNMENT_MEASURE_RIGHT_BARLINE);
    this->AddAlignment(m_rightBarLineAlignment);
    m_rightAlignment = new Alignment(0, ALIGNMENT_MEASURE_END);
    this->AddAlignment(m_rightAlignment);

    m_initialTstampDur = -1;
}

bool MeasureAligner::IsSupportedChild(ClassId classId)
{
    // Nothing to check here
    return true;
}

Alignment *MeasureAligner::GetAlignmentAtTime(const Fraction &time, AlignmentType type)
{
    int idx; // the index if we reach the end.
    Alignment *alignment = this->SearchAlignmentAtTime(time, type, idx);
    // we already have a alignment of the type at that time
    if (alignment != NULL) return alignment;
    // nothing found to the end
    if (idx == -1) {
        if (type != ALIGNMENT_MEASURE_END) {
            // This typically occurs when a tstamp event occurs after the last note of a measure
            int rightBarlineIdx = m_rightBarLineAlignment->GetIdx();
            assert(rightBarlineIdx != -1);
            idx = rightBarlineIdx;
            this->SetMaxTime(time);
        }
        else {
            idx = this->GetAlignmentCount();
        }
    }
    Alignment *newAlignment = new Alignment(time, type);
    this->AddAlignment(newAlignment, idx);
    return newAlignment;
}

void MeasureAligner::SetMaxTime(const Fraction &time)
{
    // we have to have a m_rightBarLineAlignment
    assert(m_rightBarLineAlignment);

    // it must be found in the aligner
    int idx = m_rightBarLineAlignment->GetIdx();
    assert(idx != -1);

    Alignment *alignment = NULL;
    // Increase the time position for all alignment from the right barline
    for (int i = idx; i < this->GetAlignmentCount(); ++i) {
        alignment = vrv_cast<Alignment *>(this->GetChild(i));
        assert(alignment);
        // Change it only if higher than before
        if (time > alignment->GetTime()) alignment->SetTime(time);
    }
}

Fraction MeasureAligner::GetMaxTime() const
{
    // we have to have a m_rightBarLineAlignment
    assert(m_rightBarLineAlignment);

    return m_rightAlignment->GetTime();
}

void MeasureAligner::SetInitialTstamp(data_DURATION meterUnit)
{
    m_initialTstampDur = Fraction(meterUnit) * -1;
}

void MeasureAligner::AdjustProportionally(const ArrayOfAdjustmentTuples &adjustments)
{
    ArrayOfAdjustmentTuples::const_iterator iter;
    for (iter = adjustments.begin(); iter != adjustments.end(); ++iter) {
        Alignment *start = std::get<0>(*iter);
        assert(start);
        Alignment *end = std::get<1>(*iter);
        assert(end);
        int dist = std::get<2>(*iter);
        if (dist == 0) {
            LogDebug("Trying to adjust alignment with a distance of 0;");
            continue;
        }
        // We need to store them because they are going to be changed in the loop below
        int startX = start->GetXRel();
        int endX = end->GetXRel();
        // We use a reverse iterator
        for (Object *child : this->GetChildren()) {
            Alignment *current = vrv_cast<Alignment *>(child);
            assert(current);
            // Nothing to do once we passed the start alignment
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
    ArrayOfObjects::const_reverse_iterator riter;
    for (riter = this->GetChildren().rbegin(); riter != this->GetChildren().rend(); ++riter) {
        Alignment *current = vrv_cast<Alignment *>(*riter);
        assert(current);
        if (current->IsOfType({ ALIGNMENT_GRACENOTE })) {
            if (previous) current->SetXRel(previous->GetXRel());
        }
        else {
            previous = current;
        }
    }
}

void MeasureAligner::AdjustGraceNoteSpacing(const Doc *doc, Alignment *alignment, int staffN)
{
    assert(doc);
    assert(alignment);
    assert(alignment->GetType() == ALIGNMENT_GRACENOTE);

    int graceAlignerId = doc->GetOptions()->m_graceRhythmAlign.GetValue() ? 0 : staffN;
    assert(alignment->HasGraceAligner(graceAlignerId));

    Measure *measure = vrv_cast<Measure *>(this->GetParent());
    assert(measure);

    int maxRight = VRV_UNSET;
    Alignment *rightAlignment = NULL;

    // Set staffNGrp as VRV_UNSET to align all staves
    // We could eventually also define somewhere vector of staffDef@n to be aligned together
    // For this we would need an alternate version GetLeftRight that can take a vector of staffNs
    int staffNGrp = doc->GetOptions()->m_graceRightAlign.GetValue() ? VRV_UNSET : staffN;

    bool found = false;
    ArrayOfObjects::const_reverse_iterator riter;
    for (riter = this->GetChildren().rbegin(); riter != this->GetChildren().rend(); ++riter) {
        if (!found) {
            if ((*riter) == alignment) found = true;
            continue;
        }

        rightAlignment = vrv_cast<Alignment *>(*riter);
        assert(rightAlignment);

        if (rightAlignment->IsOfType({ ALIGNMENT_FULLMEASURE, ALIGNMENT_FULLMEASURE2 })) {
            continue;
        }

        // Do not go beyond the left bar line
        if (rightAlignment->GetType() == ALIGNMENT_MEASURE_LEFT_BARLINE) {
            maxRight = measure->GetLeftBarLineRight();
            break;
        }

        int minLeft;
        rightAlignment->GetLeftRight(staffNGrp, minLeft, maxRight, { CLEF });

        if (maxRight != VRV_UNSET) break;
    }

    // This should never happen because we must have hit the left barline in the loop above
    if (!rightAlignment || (maxRight == VRV_UNSET)) return;

    // Check if the left position of the group is on the right of the previous maxRight
    // If not, move the alignments accordingly
    int left = alignment->GetGraceAligner(graceAlignerId)->GetGraceGroupLeft(staffN);
    // We also set artificially the margin with the previous note
    if (left != -VRV_UNSET) left -= doc->GetLeftMargin(NOTE) * doc->GetDrawingUnit(100);
    if (left < maxRight) {
        int spacing = (maxRight - left);
        ArrayOfAdjustmentTuples boundaries{ std::make_tuple(rightAlignment, alignment, spacing) };
        this->AdjustProportionally(boundaries);
    }
}

FunctorCode MeasureAligner::Accept(Functor &functor)
{
    return functor.VisitMeasureAligner(this);
}

FunctorCode MeasureAligner::Accept(ConstFunctor &functor) const
{
    return functor.VisitMeasureAligner(this);
}

FunctorCode MeasureAligner::AcceptEnd(Functor &functor)
{
    return functor.VisitMeasureAlignerEnd(this);
}

FunctorCode MeasureAligner::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitMeasureAlignerEnd(this);
}

//----------------------------------------------------------------------------
// GraceAligner
//----------------------------------------------------------------------------

GraceAligner::GraceAligner() : HorizontalAligner(GRACE_ALIGNER)
{
    this->Reset();
}

GraceAligner::~GraceAligner() {}

void GraceAligner::Reset()
{
    HorizontalAligner::Reset();
    m_totalWidth = 0;
}

Alignment *GraceAligner::GetAlignmentAtTime(const Fraction &time, AlignmentType type)
{
    int idx; // the index if we reach the end.
    // time = round(time);
    Alignment *alignment = this->SearchAlignmentAtTime(time, type, idx);
    // we already have a alignment of the type at that time
    if (alignment != NULL) return alignment;
    // nothing found until the end
    if (idx == -1) {
        idx = this->GetAlignmentCount();
    }
    Alignment *newAlignment = new Alignment(time, type);
    this->AddAlignment(newAlignment, idx);
    return newAlignment;
}

void GraceAligner::StackGraceElement(LayerElement *element)
{
    // Nespresso: What else?
    assert(element->Is({ NOTE, CHORD }));

    if (element->Is(NOTE)) {
        Note *note = vrv_cast<Note *>(element);
        assert(note);
        if (note->IsChordTone()) return;
    }

    m_graceStack.push_back(element);
}

void GraceAligner::AlignStack()
{
    Fraction time;
    for (int i = (int)m_graceStack.size(); i > 0; --i) {
        LayerElement *element = vrv_cast<LayerElement *>(m_graceStack.at(i - 1));
        assert(element);
        // get the duration of the event
        Fraction duration = element->GetAlignmentDuration(false);
        // Time goes backward with grace notes
        time = time - duration;
        Alignment *alignment = this->GetAlignmentAtTime(time, ALIGNMENT_DEFAULT);
        element->SetGraceAlignment(alignment);

        ClassIdsComparison matchType({ ACCID, FLAG, NOTE, STEM });
        ListOfObjects children;
        element->FindAllDescendantsByComparison(&children, &matchType);
        alignment->AddLayerElementRef(element);

        // Set the grace alignment to all children
        for (auto &child : children) {
            // Trick : FindAllDescendantsByComparison include the element, which is probably a problem.
            // With note, we want to set only accid, so make sure we do not set it twice
            if (child == element) continue;
            LayerElement *childElement = vrv_cast<LayerElement *>(child);
            assert(childElement);
            childElement->SetGraceAlignment(alignment);
            alignment->AddLayerElementRef(childElement);
        }
    }
    m_graceStack.clear();
}

int GraceAligner::GetGraceGroupLeft(int staffN) const
{
    // First we need to get the left alignment with an alignment reference with staffN
    const Alignment *leftAlignment = NULL;
    if (staffN != VRV_UNSET) {
        AttNIntegerComparison matchStaff(ALIGNMENT_REFERENCE, staffN);
        const Object *reference = this->FindDescendantByComparison(&matchStaff);
        if (!reference) return -VRV_UNSET;
        // The alignment is its parent
        leftAlignment = dynamic_cast<const Alignment *>(reference->GetParent());
    }
    else {
        leftAlignment = dynamic_cast<const Alignment *>(this->GetFirst());
    }
    // Return if nothing found
    if (!leftAlignment) return -VRV_UNSET;

    int minLeft, maxRight;
    leftAlignment->GetLeftRight(staffN, minLeft, maxRight);

    return minLeft;
}

int GraceAligner::GetGraceGroupRight(int staffN) const
{
    // See GraceAligner::GetGraceGroupLeft
    // We do not need to search of the alignment with staffN here because all grace note groups
    // Have their right note aligned, so getting the last is fine
    const Alignment *rightAlignment = dynamic_cast<const Alignment *>(this->GetLast());
    if (!rightAlignment) return VRV_UNSET;

    int minLeft, maxRight;
    rightAlignment->GetLeftRight(staffN, minLeft, maxRight);

    return maxRight;
}

void GraceAligner::SetGraceAlignmentXPos(const Doc *doc)
{
    assert(doc);

    ArrayOfObjects::const_reverse_iterator childrenIter;

    int i = 0;
    // Then the @n of each first staffDef
    for (childrenIter = this->GetChildren().rbegin(); childrenIter != this->GetChildren().rend(); ++childrenIter) {
        Alignment *alignment = vrv_cast<Alignment *>(*childrenIter);
        assert(alignment);
        // We space with a notehead (non grace size) which seems to be a reasonable default spacing with margin
        // Ideally we should look at the duration in that alignment and also the maximum staff scaling for this aligner
        alignment->SetXRel(-i * doc->GetGlyphWidth(SMUFL_E0A4_noteheadBlack, 100, false));
        ++i;
    }
}

FunctorCode GraceAligner::Accept(Functor &functor)
{
    return functor.VisitGraceAligner(this);
}

FunctorCode GraceAligner::Accept(ConstFunctor &functor) const
{
    return functor.VisitGraceAligner(this);
}

FunctorCode GraceAligner::AcceptEnd(Functor &functor)
{
    return functor.VisitGraceAlignerEnd(this);
}

FunctorCode GraceAligner::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitGraceAlignerEnd(this);
}

//----------------------------------------------------------------------------
// Alignment
//----------------------------------------------------------------------------

Alignment::Alignment() : Object(ALIGNMENT)
{
    this->Reset();
}

Alignment::Alignment(const Fraction &time, AlignmentType type) : Object(ALIGNMENT)
{
    this->Reset();
    m_time = time;
    m_type = type;
}

void Alignment::Reset()
{
    Object::Reset();

    m_xRel = 0;
    m_time = Fraction(0);
    m_type = ALIGNMENT_DEFAULT;

    this->ClearGraceAligners();
}

Alignment::~Alignment()
{
    this->ClearGraceAligners();
}

void Alignment::ClearGraceAligners()
{
    MapOfIntGraceAligners::const_iterator iter;
    for (iter = m_graceAligners.begin(); iter != m_graceAligners.end(); ++iter) {
        delete iter->second;
    }
    m_graceAligners.clear();
}

bool Alignment::IsSupportedChild(ClassId classId)
{
    // Nothing to check here
    return true;
}

bool Alignment::operator==(const Alignment &other) const
{
    const Measure *measure = vrv_cast<const Measure *>(this->GetFirstAncestor(MEASURE));
    const Measure *otherMeasure = vrv_cast<const Measure *>(other.GetFirstAncestor(MEASURE));
    assert(measure && otherMeasure);

    return (measure == otherMeasure) && (this->GetTime() == other.GetTime());
}

std::weak_ordering Alignment::operator<=>(const Alignment &other) const
{
    const Measure *measure = vrv_cast<const Measure *>(this->GetFirstAncestor(MEASURE));
    const Measure *otherMeasure = vrv_cast<const Measure *>(other.GetFirstAncestor(MEASURE));
    assert(measure && otherMeasure);

    if (measure == otherMeasure) {
        return this->GetTime() <=> other.GetTime();
    }
    else {
        return Object::IsPreOrdered(measure, otherMeasure) ? std::weak_ordering::less : std::weak_ordering::greater;
    }
}

bool Alignment::HasAccidVerticalOverlap(const Alignment *otherAlignment, int staffN) const
{
    if (!otherAlignment) return false;

    AttNIntegerComparison matchStaff(ALIGNMENT_REFERENCE, staffN);
    // get alignment references for both alignments
    const AlignmentReference *currentRef
        = vrv_cast<const AlignmentReference *>(this->FindDescendantByComparison(&matchStaff, 1));
    const AlignmentReference *otherRef
        = vrv_cast<const AlignmentReference *>(otherAlignment->FindDescendantByComparison(&matchStaff, 1));
    if (!currentRef || !otherRef) return false;

    return otherRef->HasAccidVerticalOverlap(currentRef->GetChildren());
}

bool Alignment::HasAlignmentReference(int staffN) const
{
    AttNIntegerComparison matchStaff(ALIGNMENT_REFERENCE, staffN);
    return (this->FindDescendantByComparison(&matchStaff, 1) != NULL);
}

bool Alignment::HasTimestampOnly() const
{
    // If no child, then not timestamp
    if (!this->GetChildCount()) return false;
    // Look for everything that is not a timestamp
    ClassIdsComparison notTimestamp({ ALIGNMENT, ALIGNMENT_REFERENCE, TIMESTAMP_ATTR });
    notTimestamp.ReverseComparison();
    return (this->FindDescendantByComparison(&notTimestamp, 2) == NULL);
}

AlignmentReference *Alignment::GetAlignmentReference(int staffN)
{
    AttNIntegerComparison matchStaff(ALIGNMENT_REFERENCE, staffN);
    AlignmentReference *alignmentRef = vrv_cast<AlignmentReference *>(this->FindDescendantByComparison(&matchStaff, 1));
    if (!alignmentRef) {
        alignmentRef = new AlignmentReference(staffN);
        this->AddChild(alignmentRef);
    }
    return alignmentRef;
}

void Alignment::SetXRel(int xRel)
{
    this->ResetCachedDrawingX();
    m_xRel = xRel;
}

bool Alignment::AddLayerElementRef(LayerElement *element)
{
    assert(element->IsLayerElement());

    // 0 will be used for barlines attributes or timestamps
    int layerN = 0;

    // -1 will be used for barlines attributes
    int staffN = BARLINE_REFERENCES;
    // -2 will be used for timestamps
    if (element->Is(TIMESTAMP_ATTR))
        staffN = TSTAMP_REFERENCES;
    else {
        Layer *layerRef = NULL;
        Staff *staffRef = element->GetCrossStaff(layerRef);
        // We have a cross-staff situation
        // For grace notes, we want to keep the original staffN because they need to be aligned together
        if (staffRef && !element->IsGraceNote()) {
            assert(layerRef);
            // We set cross-staff layers to the negative value in the alignment references in order to distinct them
            layerN = -layerRef->GetN();
            staffN = staffRef->GetN();
        }
        // Non cross staff normal case
        else {
            layerRef = vrv_cast<Layer *>(element->GetFirstAncestor(LAYER));
            if (layerRef) staffRef = vrv_cast<Staff *>(layerRef->GetFirstAncestor(STAFF));
            if (staffRef && layerRef) {
                layerN = layerRef->GetN();
                staffN = staffRef->GetN();
            }
            // staffN and layerN remain unused for barLine attributes and timestamps
            else {
                assert(element->Is({ BARLINE, TIMESTAMP_ATTR }));
            }
        }
    }
    AlignmentReference *alignmentRef = this->GetAlignmentReference(staffN);
    element->SetAlignmentLayerN(layerN);
    alignmentRef->AddChild(element);

    return alignmentRef->HasMultipleLayer();
}

bool Alignment::IsOfType(const std::vector<AlignmentType> &types) const
{
    return (std::find(types.begin(), types.end(), m_type) != types.end());
}

void Alignment::GetLeftRight(
    const std::vector<int> &staffNs, int &minLeft, int &maxRight, const std::vector<ClassId> &excludes) const
{
    minLeft = -VRV_UNSET;
    maxRight = VRV_UNSET;

    for (int staffN : staffNs) {
        int staffMinLeft, staffMaxRight;
        this->GetLeftRight(staffN, staffMinLeft, staffMaxRight, excludes);
        minLeft = std::min(minLeft, staffMinLeft);
        maxRight = std::max(maxRight, staffMaxRight);
    }
}

void Alignment::GetLeftRight(int staffN, int &minLeft, int &maxRight, const std::vector<ClassId> &excludes) const
{
    GetAlignmentLeftRightFunctor getAlignmentLeftRight;
    getAlignmentLeftRight.ExcludeClasses(excludes);

    if (staffN != VRV_UNSET) {
        Filters filters;
        AttNIntegerComparison matchStaff(ALIGNMENT_REFERENCE, staffN);
        filters.Add(&matchStaff);
        getAlignmentLeftRight.SetFilters(&filters);
        this->Process(getAlignmentLeftRight);
    }
    else {
        this->Process(getAlignmentLeftRight);
    }

    minLeft = getAlignmentLeftRight.GetMinLeft();
    maxRight = getAlignmentLeftRight.GetMaxRight();
}

GraceAligner *Alignment::GetGraceAligner(int id)
{
    if (!m_graceAligners.contains(id)) {
        m_graceAligners[id] = new GraceAligner();
    }
    return m_graceAligners[id];
}

bool Alignment::HasGraceAligner(int id) const
{
    return (m_graceAligners.count(id) == 1);
}

bool Alignment::PerformBoundingBoxAlignment() const
{
    return this->IsOfType({ ALIGNMENT_ACCID, ALIGNMENT_DOT, ALIGNMENT_DEFAULT });
}

AlignmentReference *Alignment::GetReferenceWithElement(const LayerElement *element, int staffN)
{
    return const_cast<AlignmentReference *>(std::as_const(*this).GetReferenceWithElement(element, staffN));
}

const AlignmentReference *Alignment::GetReferenceWithElement(const LayerElement *element, int staffN) const
{
    const AlignmentReference *reference = NULL;

    for (const Object *child : this->GetChildren()) {
        reference = dynamic_cast<const AlignmentReference *>(child);
        if (reference->GetN() == staffN) {
            return reference;
        }
        else if (staffN == VRV_UNSET) {
            if (child->HasDescendant(element, 1)) return reference;
        }
    }
    return reference;
}

std::pair<int, int> Alignment::GetAlignmentTopBottom() const
{
    int max = VRV_UNSET, min = VRV_UNSET;
    // Iterate over each element in each alignment reference and find max/min Y value - these will serve as top/bottom
    // values for the Alignment
    for (const Object *child : this->GetChildren()) {
        const AlignmentReference *reference = dynamic_cast<const AlignmentReference *>(child);
        for (const Object *element : reference->GetChildren()) {
            const int top = element->GetSelfTop();
            if ((VRV_UNSET == max) || (top > max)) {
                max = top;
            }
            const int bottom = element->GetSelfBottom();
            if ((VRV_UNSET == min) || (bottom < min)) {
                min = bottom;
            }
        }
    }
    return { min, max };
}

int Alignment::HorizontalSpaceForDuration(
    const Fraction &intervalTime, data_DURATION maxActualDur, double spacingLinear, double spacingNonLinear)
{
    double intervalTimeDbl = intervalTime.ToDouble();
    /* If the longest duration interval in the score is longer than semibreve, adjust spacing so
     that interval gets the space a semibreve would ordinarily get. */
    if (maxActualDur < DURATION_1) intervalTimeDbl /= pow(2.0, DURATION_1 - maxActualDur);

    return pow(intervalTimeDbl * 1024, spacingNonLinear) * spacingLinear * 10.0; // numbers are experimental constants
}

FunctorCode Alignment::Accept(Functor &functor)
{
    return functor.VisitAlignment(this);
}

FunctorCode Alignment::Accept(ConstFunctor &functor) const
{
    return functor.VisitAlignment(this);
}

FunctorCode Alignment::AcceptEnd(Functor &functor)
{
    return functor.VisitAlignmentEnd(this);
}

FunctorCode Alignment::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitAlignmentEnd(this);
}

//----------------------------------------------------------------------------
// AlignmentReference
//----------------------------------------------------------------------------

AlignmentReference::AlignmentReference() : Object(ALIGNMENT_REFERENCE), AttNInteger()
{
    this->RegisterAttClass(ATT_NINTEGER);

    this->Reset();

    this->SetAsReferenceObject();
}

AlignmentReference::AlignmentReference(int staffN) : Object(ALIGNMENT_REFERENCE), AttNInteger()
{
    this->RegisterAttClass(ATT_NINTEGER);

    this->Reset();

    this->SetAsReferenceObject();
    this->SetN(staffN);
}

AlignmentReference::~AlignmentReference() {}

void AlignmentReference::Reset()
{
    Object::Reset();
    this->ResetNInteger();

    m_layerCount = 0;
}

bool AlignmentReference::IsSupportedChild(ClassId classId)
{
    // Nothing to check here
    return true;
}

void AlignmentReference::AddChild(Object *child)
{
    LayerElement *childElement = vrv_cast<LayerElement *>(child);
    assert(childElement);

    ArrayOfObjects &children = this->GetChildrenForModification();

    if (!childElement->HasSameas()) {
        ArrayOfObjects::iterator childrenIter;
        // Check if the we will have a reference with multiple layers
        for (childrenIter = children.begin(); childrenIter != children.end(); ++childrenIter) {
            LayerElement *element = dynamic_cast<LayerElement *>(*childrenIter);
            if (childElement->GetAlignmentLayerN() == element->GetAlignmentLayerN()) {
                break;
            }
        }
        if (childrenIter == children.end()) m_layerCount++;
    }

    // Special case where we do not set the parent because the reference will not have ownership
    // Children will be treated as relinquished objects in the desctructor
    // However, we need to make sure the child has a parent (somewhere else)
    assert(child->GetParent() && this->IsReferenceObject());
    children.push_back(child);
    this->Modify();
}

bool AlignmentReference::HasAccidVerticalOverlap(const ArrayOfConstObjects &objects) const
{
    for (const auto child : this->GetChildren()) {
        if (!child->Is(ACCID)) continue;
        const Accid *accid = vrv_cast<const Accid *>(child);
        if (!accid->HasAccid()) continue;
        for (const auto object : objects) {
            if (accid->VerticalContentOverlap(object)) return true;
        }
    }
    return false;
}

bool AlignmentReference::HasCrossStaffElements() const
{
    ListOfConstObjects children;
    ClassIdsComparison classId({ NOTE, CHORD });
    this->FindAllDescendantsByComparison(&children, &classId);

    for (const auto child : children) {
        const LayerElement *layerElement = vrv_cast<const LayerElement *>(child);
        if (layerElement->m_crossStaff) return true;
    }

    return false;
}

FunctorCode AlignmentReference::Accept(Functor &functor)
{
    return functor.VisitAlignmentReference(this);
}

FunctorCode AlignmentReference::Accept(ConstFunctor &functor) const
{
    return functor.VisitAlignmentReference(this);
}

FunctorCode AlignmentReference::AcceptEnd(Functor &functor)
{
    return functor.VisitAlignmentReferenceEnd(this);
}

FunctorCode AlignmentReference::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitAlignmentReferenceEnd(this);
}

//----------------------------------------------------------------------------
// TimestampAligner
//----------------------------------------------------------------------------

TimestampAligner::TimestampAligner() : Object(TIMESTAMP_ALIGNER)
{
    this->Reset();
}

TimestampAligner::~TimestampAligner() {}

void TimestampAligner::Reset()
{
    Object::Reset();
}

bool TimestampAligner::IsSupportedChild(ClassId classId)
{
    // Nothing to check here
    return true;
}

TimestampAttr *TimestampAligner::GetTimestampAtTime(double time)
{
    int idx = -1; // the index if we reach the end.
    // We need to adjust the position since timestamp 0 to 1.0 are before 0 musical time
    time = time - 1.0;
    TimestampAttr *timestampAttr = NULL;

    ArrayOfObjects &children = this->GetChildrenForModification();

    // First try to see if we already have something at the time position
    int i = 0;
    for (Object *child : children) {
        timestampAttr = vrv_cast<TimestampAttr *>(child);
        assert(timestampAttr);

        double alignmentTime = timestampAttr->GetActualDurPos();
        if (ApproximatelyEqual(alignmentTime, time)) {
            return timestampAttr;
        }
        // nothing found, do not go any further but keep the index
        if (alignmentTime > time) {
            idx = i;
            break;
        }
        ++i;
    }
    // nothing found
    timestampAttr = new TimestampAttr();
    timestampAttr->SetDrawingPos(time);
    if (idx == -1) {
        this->AddChild(timestampAttr);
    }
    else {
        this->InsertChild(timestampAttr, idx);
    }
    return timestampAttr;
}

FunctorCode TimestampAligner::Accept(Functor &functor)
{
    return functor.VisitTimestampAligner(this);
}

FunctorCode TimestampAligner::Accept(ConstFunctor &functor) const
{
    return functor.VisitTimestampAligner(this);
}

FunctorCode TimestampAligner::AcceptEnd(Functor &functor)
{
    return functor.VisitTimestampAlignerEnd(this);
}

FunctorCode TimestampAligner::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitTimestampAlignerEnd(this);
}

} // namespace vrv

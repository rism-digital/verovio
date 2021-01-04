/////////////////////////////////////////////////////////////////////////////
// Name:        horizontalaligner.cpp
// Author:      Laurent Pugin
// Created:     2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "horizontalaligner.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "arpeg.h"
#include "comparison.h"
#include "doc.h"
#include "floatingobject.h"
#include "functorparams.h"
#include "layer.h"
#include "measure.h"
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

HorizontalAligner::HorizontalAligner() : Object()
{
    Reset();
}

HorizontalAligner::~HorizontalAligner() {}

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
    for (i = 0; i < GetAlignmentCount(); ++i) {
        alignment = vrv_cast<Alignment *>(GetChildren()->at(i));
        assert(alignment);

        double alignment_time = alignment->GetTime();
        if (AreEqual(alignment_time, time)) {
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
    ArrayOfObjects *children = this->GetChildrenForModification();
    if (idx == -1) {
        children->push_back(alignment);
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

MeasureAligner::~MeasureAligner() {}

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
    time = durRound(time);
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
    for (i = idx; i < GetAlignmentCount(); ++i) {
        alignment = vrv_cast<Alignment *>(this->GetChildren()->at(i));
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
        for (auto child : *this->GetChildren()) {
            Alignment *current = vrv_cast<Alignment *>(child);
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
    ArrayOfObjects::const_reverse_iterator riter;
    for (riter = this->GetChildren()->rbegin(); riter != this->GetChildren()->rend(); ++riter) {
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

void MeasureAligner::AdjustGraceNoteSpacing(Doc *doc, Alignment *alignment, int staffN)
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
    for (riter = this->GetChildren()->rbegin(); riter != this->GetChildren()->rend(); ++riter) {
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
        rightAlignment->GetLeftRight(staffNGrp, minLeft, maxRight);

        if (maxRight != VRV_UNSET) break;
    }

    // This should never happen because we must have hit the left barline in the loop above
    if (!rightAlignment || (maxRight == VRV_UNSET)) return;

    // Check if the left position of the group is on the right of the previous maxRight
    // If not, move the aligments accordingly
    int left = alignment->GetGraceAligner(graceAlignerId)->GetGraceGroupLeft(staffN);
    // We also set artificially the margin with the previous note
    if (left != -VRV_UNSET) left -= doc->GetLeftMargin(NOTE) * doc->GetDrawingUnit(100);
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

GraceAligner::~GraceAligner() {}

void GraceAligner::Reset()
{
    HorizontalAligner::Reset();
    m_totalWidth = 0;
}

Alignment *GraceAligner::GetAlignmentAtTime(double time, AlignmentType type)
{
    int idx; // the index if we reach the end.
    time = round(time);
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
    int i;
    double time = 0.0;
    for (i = (int)m_graceStack.size(); i > 0; i--) {
        LayerElement *element = vrv_cast<LayerElement *>(m_graceStack.at(i - 1));
        assert(element);
        // get the duration of the event
        double duration = element->GetAlignmentDuration(NULL, NULL, false);
        // Time goes backward with grace notes
        time -= duration;
        Alignment *alignment = this->GetAlignmentAtTime(time, ALIGNMENT_DEFAULT);
        element->SetGraceAlignment(alignment);

        ClassIdsComparison matchType({ ACCID, FLAG, NOTE, STEM });
        ListOfObjects children;
        element->FindAllDescendantByComparison(&children, &matchType);
        alignment->AddLayerElementRef(element);

        // Set the grace alignmnet to all children
        for (auto &child : children) {
            // Trick : FindAllDescendantByComparison include the element, which is probably a problem.
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

int GraceAligner::GetGraceGroupLeft(int staffN)
{
    // First we need to get the left alignment with an alignment reference with staffN
    Alignment *leftAlignment = NULL;
    if (staffN != VRV_UNSET) {
        AttNIntegerComparison matchStaff(ALIGNMENT_REFERENCE, staffN);
        Object *reference = this->FindDescendantByComparison(&matchStaff);
        if (!reference) return -VRV_UNSET;
        // The alignment is its parent
        leftAlignment = dynamic_cast<Alignment *>(reference->GetParent());
    }
    else
        leftAlignment = dynamic_cast<Alignment *>(this->GetFirst());
    // Return if nothing found
    if (!leftAlignment) return -VRV_UNSET;

    int minLeft, maxRight;
    leftAlignment->GetLeftRight(staffN, minLeft, maxRight);

    return minLeft;
}

int GraceAligner::GetGraceGroupRight(int staffN)
{
    // See GraceAligner::GetGraceGroupLeft
    // We do not need to search of the alignment with staffN here because all grace note groups
    // Have their right note aligned, so getting the last is fine
    Alignment *rightAlignment = dynamic_cast<Alignment *>(this->GetLast());
    if (!rightAlignment) return VRV_UNSET;

    int minLeft, maxRight;
    rightAlignment->GetLeftRight(staffN, minLeft, maxRight);

    return maxRight;
}

void GraceAligner::SetGraceAligmentXPos(Doc *doc)
{
    assert(doc);

    ArrayOfObjects::const_reverse_iterator childrenIter;

    int i = 0;
    // Then the @n of each first staffDef
    for (childrenIter = this->GetChildren()->rbegin(); childrenIter != this->GetChildren()->rend(); ++childrenIter) {
        Alignment *alignment = vrv_cast<Alignment *>(*childrenIter);
        assert(alignment);
        // We space with a notehead (non grace size) which seems to be a reasonable default spacing with margin
        // Ideally we should look at the duration in that alignmment and also the maximum staff scaling for this aligner
        alignment->SetXRel(-i * doc->GetGlyphWidth(SMUFL_E0A4_noteheadBlack, 100, false));
        i++;
    }
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

    ClearGraceAligners();
}

Alignment::~Alignment()
{
    ClearGraceAligners();
}

void Alignment::ClearGraceAligners()
{
    MapOfIntGraceAligners::const_iterator iter;
    for (iter = m_graceAligners.begin(); iter != m_graceAligners.end(); ++iter) {
        delete iter->second;
    }
    m_graceAligners.clear();
}

bool Alignment::IsSupportedChild(Object *child)
{
    assert(dynamic_cast<AlignmentReference *>(child));
    return true;
}

bool Alignment::HasAlignmentReference(int staffN)
{
    AttNIntegerComparison matchStaff(ALIGNMENT_REFERENCE, staffN);
    return (this->FindDescendantByComparison(&matchStaff, 1) != NULL);
}

AlignmentReference *Alignment::GetAlignmentReference(int staffN)
{
    AttNIntegerComparison matchStaff(ALIGNMENT_REFERENCE, staffN);
    AlignmentReference *alignmentRef
        = dynamic_cast<AlignmentReference *>(this->FindDescendantByComparison(&matchStaff, 1));
    if (!alignmentRef) {
        alignmentRef = new AlignmentReference(staffN);
        this->AddChild(alignmentRef);
    }
    return alignmentRef;
}

void Alignment::SetXRel(int xRel)
{
    ResetCachedDrawingX();
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
            layerRef = dynamic_cast<Layer *>(element->GetFirstAncestor(LAYER));
            if (layerRef) staffRef = dynamic_cast<Staff *>(layerRef->GetFirstAncestor(STAFF));
            if (staffRef) {
                layerN = layerRef->GetN();
                staffN = staffRef->GetN();
            }
            // staffN and layerN remain unsed for barLine attributes and timestamps
            else {
                assert(element->Is({ BARLINE_ATTR_LEFT, BARLINE_ATTR_RIGHT, TIMESTAMP_ATTR }));
            }
        }
    }
    AlignmentReference *alignmentRef = GetAlignmentReference(staffN);
    element->SetAlignmentLayerN(layerN);
    alignmentRef->AddChild(element);

    return alignmentRef->HasMultipleLayer();
}

bool Alignment::IsOfType(const std::vector<AlignmentType> &types)
{
    return (std::find(types.begin(), types.end(), m_type) != types.end());
}

void Alignment::GetLeftRight(int staffN, int &minLeft, int &maxRight)
{
    Functor getAlignmentLeftRight(&Object::GetAlignmentLeftRight);
    GetAlignmentLeftRightParams getAlignmentLeftRightParams(&getAlignmentLeftRight);

    if (staffN != VRV_UNSET) {
        ArrayOfComparisons filters;
        AttNIntegerComparison matchStaff(ALIGNMENT_REFERENCE, staffN);
        filters.push_back(&matchStaff);
        this->Process(&getAlignmentLeftRight, &getAlignmentLeftRightParams, NULL, &filters);
    }
    else
        this->Process(&getAlignmentLeftRight, &getAlignmentLeftRightParams);

    minLeft = getAlignmentLeftRightParams.m_minLeft;
    maxRight = getAlignmentLeftRightParams.m_maxRight;
}

GraceAligner *Alignment::GetGraceAligner(int id)
{
    if (m_graceAligners.count(id) == 0) {
        m_graceAligners[id] = new GraceAligner();
    }
    return m_graceAligners[id];
}

bool Alignment::HasGraceAligner(int id) const
{
    return (m_graceAligners.count(id) == 1);
}

AlignmentReference *Alignment::GetReferenceWithElement(LayerElement *element, int staffN)
{
    AlignmentReference *reference = NULL;

    for (auto child : *this->GetChildren()) {
        reference = dynamic_cast<AlignmentReference *>(child);
        if (reference->GetN() == staffN) {
            return reference;
        }
        else if (staffN == VRV_UNSET) {
            if (child->HasDescendant(element, 1)) return reference;
        }
    }
    return reference;
}

void Alignment::AddToAccidSpace(Accid *accid)
{
    assert(accid);

    // Do not added them if no @accid (e.g., @accid.ges only)
    if (!accid->HasAccid()) return;

    AlignmentReference *reference = this->GetReferenceWithElement(accid);
    assert(reference);
    reference->AddToAccidSpace(accid);
}

//----------------------------------------------------------------------------
// AlignmentReference
//----------------------------------------------------------------------------

AlignmentReference::AlignmentReference() : Object(), AttNInteger()
{
    RegisterAttClass(ATT_NINTEGER);

    Reset();

    this->SetAsReferenceObject();
}

AlignmentReference::AlignmentReference(int staffN) : Object(), AttNInteger()
{
    RegisterAttClass(ATT_NINTEGER);

    Reset();

    this->SetAsReferenceObject();
    this->SetN(staffN);
}

AlignmentReference::~AlignmentReference() {}

void AlignmentReference::Reset()
{
    Object::Reset();
    ResetNInteger();

    m_accidSpace.clear();
    m_layerCount = 0;
}

bool AlignmentReference::IsSupportedChild(Object *child)
{
    assert(dynamic_cast<LayerElement *>(child));
    return true;
}

void AlignmentReference::AddChild(Object *child)
{
    LayerElement *childElement = vrv_cast<LayerElement *>(child);
    assert(childElement);

    ArrayOfObjects *children = this->GetChildrenForModification();

    if (!childElement->HasSameas()) {
        ArrayOfObjects::iterator childrenIter;
        // Check if the we will have a reference with multiple layers
        for (childrenIter = children->begin(); childrenIter != children->end(); ++childrenIter) {
            LayerElement *element = dynamic_cast<LayerElement *>(*childrenIter);
            if (childElement->GetAlignmentLayerN() == element->GetAlignmentLayerN()) {
                break;
            }
        }
        if (childrenIter == children->end()) m_layerCount++;
    }

    // Specical case where we do not set the parent because the reference will not have ownership
    // Children will be treated as relinquished objects in the desctructor
    // However, we need to make sure the child has a parent (somewhere else)
    assert(child->GetParent() && this->IsReferenceObject());
    children->push_back(child);
    Modify();
}

void AlignmentReference::AddToAccidSpace(Accid *accid)
{
    assert(accid);

    if (std::find(m_accidSpace.begin(), m_accidSpace.end(), accid) != m_accidSpace.end()) return;

    m_accidSpace.push_back(accid);
}

void AlignmentReference::AdjustAccidWithAccidSpace(Accid *accid, Doc *doc, int staffSize)
{
    std::vector<Accid *> leftAccids;

    // bottom one
    for (auto child : *this->GetChildren()) {
        accid->AdjustX(dynamic_cast<LayerElement *>(child), doc, staffSize, leftAccids);
    }
}

//----------------------------------------------------------------------------
// TimestampAligner
//----------------------------------------------------------------------------

TimestampAligner::TimestampAligner() : Object()
{
    Reset();
}

TimestampAligner::~TimestampAligner() {}

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

    ArrayOfObjects *children = this->GetChildrenForModification();

    // First try to see if we already have something at the time position
    for (i = 0; i < GetChildCount(); ++i) {
        timestampAttr = vrv_cast<TimestampAttr *>(children->at(i));
        assert(timestampAttr);

        double alignmentTime = timestampAttr->GetActualDurPos();
        if (AreEqual(alignmentTime, time)) {
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
        children->push_back(timestampAttr);
    }
    else {
        InsertChild(timestampAttr, idx);
    }
    return timestampAttr;
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int MeasureAligner::SetAlignmentXPos(FunctorParams *functorParams)
{
    SetAlignmentXPosParams *params = vrv_params_cast<SetAlignmentXPosParams *>(functorParams);
    assert(params);

    // We start a new MeasureAligner
    // Reset the previous time position and x_rel to 0;
    params->m_previousTime = 0.0;
    params->m_previousXRel = 0;

    return FUNCTOR_CONTINUE;
}

int MeasureAligner::JustifyX(FunctorParams *functorParams)
{
    JustifyXParams *params = vrv_params_cast<JustifyXParams *>(functorParams);
    assert(params);

    params->m_leftBarLineX = GetLeftBarLineAlignment()->GetXRel();
    params->m_rightBarLineX = GetRightBarLineAlignment()->GetXRel();

    return FUNCTOR_CONTINUE;
}

int Alignment::AdjustArpeg(FunctorParams *functorParams)
{
    AdjustArpegParams *params = vrv_params_cast<AdjustArpegParams *>(functorParams);
    assert(params);

    // An array of Alignment / Arpeg / staffN / bool (for indicating if we have reached the aligment yet)
    ArrayOfAligmentArpegTuples::iterator iter = params->m_alignmentArpegTuples.begin();

    while (iter != params->m_alignmentArpegTuples.end()) {
        // We are reaching the alignment to which an arpeg points to (i.e, the topNote one)
        if (std::get<0>(*iter) == this) {
            std::get<3>(*iter) = true;
            ++iter;
            continue;
        }
        // We have not reached the alignment of the arpeg, just continue (backwards)
        else if (std::get<3>(*iter) == false) {
            ++iter;
            continue;
        }
        // We are now in an alignment preceeding an arpeg - check for overlap
        int minLeft, maxRight;
        this->GetLeftRight(std::get<2>(*iter), minLeft, maxRight);

        // Nothing for the staff we are looking at, we also need to check with barlines
        if (maxRight == VRV_UNSET) {
            this->GetLeftRight(-1, minLeft, maxRight);
        }

        // Make sure that there is no overlap with right barline of the previous measure
        if ((maxRight == VRV_UNSET) && (m_type == ALIGNMENT_MEASURE_LEFT_BARLINE)) {
            Measure *measure = vrv_cast<Measure *>(this->GetFirstAncestor(MEASURE));
            auto parent = measure->GetParent();
            Measure *previous = vrv_cast<Measure *>(parent->GetPrevious(measure, MEASURE));
            if (previous) {
                Alignment *alignment = previous->m_measureAligner.GetRightBarLineAlignment();
                alignment->GetLeftRight(-1, minLeft, maxRight);
                if (maxRight != VRV_UNSET) {
                    const int previousWidth = previous->GetWidth();
                    minLeft -= previousWidth;
                    maxRight -= previousWidth;
                }
            }
        }

        // Nothing, just continue
        if (maxRight == VRV_UNSET) {
            ++iter;
            continue;
        }

        int overlap = maxRight - std::get<1>(*iter)->GetCurrentFloatingPositioner()->GetSelfLeft();
        // HARDCODED
        overlap += params->m_doc->GetDrawingUnit(100) / 2 * 3;
        // LogDebug("maxRight %d, %d %d", maxRight, std::get<2>(*iter), overlap);
        if (overlap > 0) {
            ArrayOfAdjustmentTuples boundaries{ std::make_tuple(this, std::get<0>(*iter), overlap) };
            params->m_measureAligner->AdjustProportionally(boundaries);
        }

        // We can remove it from the list
        iter = params->m_alignmentArpegTuples.erase(iter);
    }

    return FUNCTOR_CONTINUE;
}

int Alignment::AdjustGraceXPos(FunctorParams *functorParams)
{
    AdjustGraceXPosParams *params = vrv_params_cast<AdjustGraceXPosParams *>(functorParams);
    assert(params);

    // We are in a Measure aligner - redirect to the GraceAligner when it is a ALIGNMENT_GRACENOTE
    if (!params->m_isGraceAlignment) {
        // Do not process AlignmentReference children if no GraceAligner
        if (m_graceAligners.empty()) {
            // We store the default alignment before we hit the grace alignment
            if (m_type == ALIGNMENT_DEFAULT) params->m_rightDefaultAlignment = this;
            return FUNCTOR_SIBLINGS;
        }
        assert(m_type == ALIGNMENT_GRACENOTE);

        // Change the flag for indicating that the alignment is child of a GraceAligner
        params->m_isGraceAlignment = true;

        // Get the parent measure Aligner
        MeasureAligner *measureAligner = vrv_cast<MeasureAligner *>(this->GetFirstAncestor(MEASURE_ALIGNER));
        assert(measureAligner);

        std::vector<int>::iterator iter;
        ArrayOfComparisons filters;
        for (iter = params->m_staffNs.begin(); iter != params->m_staffNs.end(); ++iter) {

            // Rescue value, used at the end of a measure without a barline
            int graceMaxPos = this->GetXRel() - params->m_doc->GetDrawingUnit(100);
            // If we have a rightDefault, then this is (quite likely) the next note / chord
            // Get its minimum left and make it the max right position of the grace group
            if (params->m_rightDefaultAlignment) {
                int minLeft, maxRight;
                params->m_rightDefaultAlignment->GetLeftRight(*iter, minLeft, maxRight);
                if (minLeft != -VRV_UNSET)
                    graceMaxPos = minLeft - params->m_doc->GetLeftMargin(NOTE) * params->m_doc->GetDrawingUnit(75);
            }
            // This happens when grace notes are at the end of a measure before a barline
            else {
                int minLeft, maxRight;
                assert(measureAligner->GetRightBarLineAlignment());
                // staffN -1 is barline
                measureAligner->GetRightBarLineAlignment()->GetLeftRight(BARLINE_REFERENCES, minLeft, maxRight);
                if (minLeft != -VRV_UNSET)
                    graceMaxPos = minLeft - params->m_doc->GetLeftMargin(NOTE) * params->m_doc->GetDrawingUnit(75);
            }

            params->m_graceMaxPos = graceMaxPos;
            params->m_graceUpcomingMaxPos = -VRV_UNSET;
            params->m_graceCumulatedXShift = VRV_UNSET;
            filters.clear();
            // Create ad comparison object for each type / @n
            AttNIntegerComparison matchStaff(ALIGNMENT_REFERENCE, (*iter));
            filters.push_back(&matchStaff);

            int graceAlignerId = params->m_doc->GetOptions()->m_graceRhythmAlign.GetValue() ? 0 : *iter;

            if (HasGraceAligner(graceAlignerId)) {
                GetGraceAligner(graceAlignerId)
                    ->Process(params->m_functor, params, params->m_functorEnd, &filters, UNLIMITED_DEPTH, BACKWARD);

                // There was not grace notes for that staff
                if (params->m_graceCumulatedXShift == VRV_UNSET) continue;

                // Now we need to adjust the space for the grace not group
                measureAligner->AdjustGraceNoteSpacing(params->m_doc, this, (*iter));
            }
        }

        // Change the flag back
        params->m_isGraceAlignment = false;

        return FUNCTOR_CONTINUE;
    }

    if (params->m_graceCumulatedXShift != VRV_UNSET) {
        // This is happening when aligning the grace aligner itself
        this->SetXRel(this->GetXRel() + params->m_graceCumulatedXShift);
    }

    return FUNCTOR_CONTINUE;
}

int Alignment::AdjustGraceXPosEnd(FunctorParams *functorParams)
{
    AdjustGraceXPosParams *params = vrv_params_cast<AdjustGraceXPosParams *>(functorParams);
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
    AdjustXPosParams *params = vrv_params_cast<AdjustXPosParams *>(functorParams);
    assert(params);

    // LogDebug("Alignment type %d", m_type);

    this->SetXRel(this->GetXRel() + params->m_cumulatedXShift);

    if (m_type == ALIGNMENT_MEASURE_END) {
        this->SetXRel(params->m_minPos);
    }

    return FUNCTOR_CONTINUE;
}

int Alignment::AdjustXPosEnd(FunctorParams *functorParams)
{
    AdjustXPosParams *params = vrv_params_cast<AdjustXPosParams *>(functorParams);
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

int Alignment::AdjustAccidX(FunctorParams *functorParams)
{
    AdjustAccidXParams *params = vrv_params_cast<AdjustAccidXParams *>(functorParams);
    assert(params);

    MapOfIntGraceAligners::const_iterator iter;
    for (iter = m_graceAligners.begin(); iter != m_graceAligners.end(); ++iter) {
        iter->second->Process(params->m_functor, functorParams);
    }

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
    SetAlignmentXPosParams *params = vrv_params_cast<SetAlignmentXPosParams *>(functorParams);
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
            params->m_doc->GetOptions()->m_spacingLinear.GetValue(),
            params->m_doc->GetOptions()->m_spacingNonLinear.GetValue());
        // LogDebug("SetAlignmentXPos: intervalTime=%.2f intervalXRel=%d", intervalTime, intervalXRel);
    }

    MapOfIntGraceAligners::const_iterator iter;
    for (iter = m_graceAligners.begin(); iter != m_graceAligners.end(); ++iter) {
        iter->second->SetGraceAligmentXPos(params->m_doc);
    }

    SetXRel(params->m_previousXRel + intervalXRel * DEFINITION_FACTOR);
    params->m_previousTime = m_time;
    params->m_previousXRel = m_xRel;

    return FUNCTOR_CONTINUE;
}

int Alignment::JustifyX(FunctorParams *functorParams)
{
    JustifyXParams *params = vrv_params_cast<JustifyXParams *>(functorParams);
    assert(params);

    if (m_type <= ALIGNMENT_MEASURE_LEFT_BARLINE) {
        // Nothing to do for all left scoreDef elements and the left barline
    }
    else if (m_type < ALIGNMENT_MEASURE_RIGHT_BARLINE) {
        // All elements up to the next barline, move them but also take into account the leftBarlineX
        SetXRel(ceil((((double)this->m_xRel - (double)params->m_leftBarLineX) * params->m_justifiableRatio)
            + params->m_leftBarLineX));
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

int AlignmentReference::AdjustLayers(FunctorParams *functorParams)
{
    AdjustLayersParams *params = vrv_params_cast<AdjustLayersParams *>(functorParams);
    assert(params);

    if (!this->HasMultipleLayer()) return FUNCTOR_SIBLINGS;

    params->m_currentLayerN = VRV_UNSET;
    params->m_currentNote = NULL;
    params->m_currentChord = NULL;
    params->m_current.clear();
    params->m_previous.clear();

    return FUNCTOR_CONTINUE;
}

int AlignmentReference::AdjustGraceXPos(FunctorParams *functorParams)
{
    AdjustGraceXPosParams *params = vrv_params_cast<AdjustGraceXPosParams *>(functorParams);
    assert(params);

    // Because we are processing grace notes aligment backward (see Alignment::AdjustGraceXPos) we need
    // to process the children (LayerElement) "by hand" in FORWARD manner
    // (filters can be NULL because filtering was already applied in the parent)
    for (auto child : *this->GetChildren()) {
        child->Process(params->m_functor, params, params->m_functorEnd, NULL, UNLIMITED_DEPTH, FORWARD);
    }

    return FUNCTOR_SIBLINGS;
}

int AlignmentReference::AdjustAccidX(FunctorParams *functorParams)
{
    AdjustAccidXParams *params = vrv_params_cast<AdjustAccidXParams *>(functorParams);
    assert(params);

    if (m_accidSpace.empty()) return FUNCTOR_SIBLINGS;

    assert(params->m_doc);
    StaffDef *staffDef = params->m_doc->m_mdivScoreDef.GetStaffDef(this->GetN());
    int staffSize = (staffDef && staffDef->HasScale()) ? staffDef->GetScale() : 100;

    std::sort(m_accidSpace.begin(), m_accidSpace.end(), AccidSpaceSort());

    // Detect the octave and mark them
    std::vector<Accid *>::iterator iter, octaveIter;
    for (iter = m_accidSpace.begin(); iter != m_accidSpace.end() - 1; ++iter) {
        Note *note = vrv_cast<Note *>((*iter)->GetFirstAncestor(NOTE));
        assert(note);
        if (!note) continue;
        for (octaveIter = iter + 1; octaveIter != m_accidSpace.end(); ++octaveIter) {
            Note *octave = vrv_cast<Note *>((*octaveIter)->GetFirstAncestor(NOTE));
            assert(octave);
            if (!octave) continue;
            // Same pitch, different octave, same accid - for now?
            if ((note->GetPname() == octave->GetPname()) && (note->GetOct() != octave->GetOct())
                && ((*iter)->GetAccid() == (*octaveIter)->GetAccid())) {
                (*iter)->SetDrawingOctaveAccid(*octaveIter);
                (*octaveIter)->SetDrawingOctave(true);
            }
        }
    }

    int count = (int)m_accidSpace.size();
    int i, j;

    // Align the octaves
    for (i = 0; i < count - 1; ++i) {
        if (m_accidSpace.at(i)->GetDrawingOctaveAccid() != NULL) {
            this->AdjustAccidWithAccidSpace(m_accidSpace.at(i), params->m_doc, staffSize);
            this->AdjustAccidWithAccidSpace(m_accidSpace.at(i)->GetDrawingOctaveAccid(), params->m_doc, staffSize);
            int dist = m_accidSpace.at(i)->GetDrawingX() - m_accidSpace.at(i)->GetDrawingOctaveAccid()->GetDrawingX();
            if (dist > 0)
                m_accidSpace.at(i)->SetDrawingXRel(m_accidSpace.at(i)->GetDrawingXRel() - dist);
            else if (dist < 0)
                m_accidSpace.at(i)->GetDrawingOctaveAccid()->SetDrawingXRel(
                    m_accidSpace.at(i)->GetDrawingOctaveAccid()->GetDrawingXRel() + dist);
        }
    }

    int middle = (count % 2) ? (count / 2) + 1 : (count / 2);
    // Zig-zag processing
    for (i = 0, j = count - 1; i < middle; i++, j--) {
        // top one - but skip octaves
        if (!m_accidSpace.at(j)->GetDrawingOctaveAccid() && !m_accidSpace.at(j)->GetDrawingOctave())
            this->AdjustAccidWithAccidSpace(m_accidSpace.at(j), params->m_doc, staffSize);

        // Break with odd number of elements once the middle is reached
        if (i == j) break;

        // bottom one - but skip octaves
        if (!m_accidSpace.at(i)->GetDrawingOctaveAccid() && !m_accidSpace.at(i)->GetDrawingOctave())
            this->AdjustAccidWithAccidSpace(m_accidSpace.at(i), params->m_doc, staffSize);
    }

    return FUNCTOR_SIBLINGS;
}

int AlignmentReference::UnsetCurrentScoreDef(FunctorParams *functorParams)
{
    Alignment *alignment = vrv_cast<Alignment *>(this->GetParent());
    assert(alignment);

    switch (alignment->GetType()) {
        case ALIGNMENT_SCOREDEF_CLEF:
        case ALIGNMENT_SCOREDEF_KEYSIG:
        case ALIGNMENT_SCOREDEF_MENSUR:
        case ALIGNMENT_SCOREDEF_METERSIG:
        case ALIGNMENT_SCOREDEF_CAUTION_CLEF:
        case ALIGNMENT_SCOREDEF_CAUTION_KEYSIG:
        case ALIGNMENT_SCOREDEF_CAUTION_MENSUR:
        case ALIGNMENT_SCOREDEF_CAUTION_METERSIG: this->ClearChildren(); break;
        default: break;
    }

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv

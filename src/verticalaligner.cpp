/////////////////////////////////////////////////////////////////////////////
// Name:        verticalaligner.cpp
// Author:      Laurent Pugin
// Created:     2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "verticalaligner.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "doc.h"
#include "floatingobject.h"
#include "functorparams.h"
#include "slur.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// SystemAligner
//----------------------------------------------------------------------------

SystemAligner::SystemAligner() : Object()
{
    Reset();
}

SystemAligner::~SystemAligner() {}

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

    if (idx < GetChildCount()) {
        this->m_children.push_back(m_bottomAlignment);
        return dynamic_cast<StaffAlignment *>(m_children.at(idx));
    }
    // check that we are searching for the next one (not a gap)
    assert(idx == GetChildCount());
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
    for (i = 0; i < this->GetChildCount(); ++i) {
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

void StaffAlignment::SetYRel(int yRel)
{
    if (yRel < m_yRel) {
        m_yRel = yRel;
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
        return positioner->GetContentTop() - this->GetYRel();
    }
    return box->GetSelfTop() - this->GetYRel();
}

int StaffAlignment::CalcOverflowBelow(BoundingBox *box)
{
    if (box->Is(FLOATING_POSITIONER)) {
        FloatingPositioner *positioner = dynamic_cast<FloatingPositioner *>(box);
        assert(positioner);
        return -(positioner->GetContentBottom() + m_staffHeight - this->GetYRel());
    }
    return -(box->GetSelfBottom() + m_staffHeight - this->GetYRel());
}

void StaffAlignment::SetCurrentFloatingPositioner(
    FloatingObject *object, Object *objectX, Object *objectY, char spanningType)
{
    FloatingPositioner *positioner = this->GetCorrespFloatingPositioner(object);
    if (positioner == NULL) {
        positioner = new FloatingPositioner(object, this, spanningType);
        m_floatingPositioners.push_back(positioner);
    }
    positioner->SetObjectXY(objectX, objectY);
    // LogDebug("BB %d", item->second.m_contentBB_x1);
    object->SetCurrentFloatingPositioner(positioner);
}

FloatingPositioner *StaffAlignment::GetCorrespFloatingPositioner(FloatingObject *object)
{
    auto item = std::find_if(m_floatingPositioners.begin(), m_floatingPositioners.end(),
        [object](FloatingPositioner *positioner) { return positioner->GetObject() == object; });
    if (item != m_floatingPositioners.end()) {
        return *item;
    }
    return NULL;
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

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
            int margin = params->m_doc->GetBottomMargin(SYL) * params->m_doc->GetDrawingUnit(staffSize);
            int minMargin = std::max((int)(params->m_doc->GetOptions()->m_lyricTopMinMargin.GetValue()
                                         * params->m_doc->GetDrawingUnit(staffSize)),
                this->GetOverflowBelow());
            this->SetOverflowBelow(minMargin + this->GetVerseCount() * (height - descender + margin));
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

        // Skip if no content bounding box is available
        if (!(*iter)->HasContentBB()) continue;

        // for slurs and ties we do not need to adjust them, only add them to the overflow boxes if required
        if ((params->m_classId == SLUR) || (params->m_classId == TIE)) {

            bool skipAbove = false;
            bool skipBelow = false;

            if ((*iter)->GetObject()->Is(SLUR)) {
                Slur *slur = dynamic_cast<Slur *>((*iter)->GetObject());
                assert(slur);
                slur->GetCrossStaffOverflows(this, (*iter)->m_cuvreDir, skipAbove, skipBelow);
            }

            int overflowAbove = 0;
            if (!skipAbove) overflowAbove = this->CalcOverflowAbove((*iter));
            if (overflowAbove > params->m_doc->GetDrawingStaffLineWidth(staffSize) / 2) {
                // LogMessage("%sparams->m_doctop overflow: %d", current->GetUuid().c_str(), overflowAbove);
                this->SetOverflowAbove(overflowAbove);
                this->m_overflowAboveBBoxes.push_back((*iter));
            }

            int overflowBelow = 0;
            if (!skipBelow) overflowBelow = this->CalcOverflowBelow((*iter));
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
        data_STAFFREL_basic place = (*iter)->GetDrawingPlace();
        if (place == STAFFREL_basic_above) {
            overflowBoxes = &m_overflowAboveBBoxes;
        }
        auto i = overflowBoxes->begin();
        auto end = overflowBoxes->end();
        while (i != end) {
            // find all the overflowing elements from the staff that overlap horizonatally
            i = std::find_if(i, end, [iter](BoundingBox *elem) { return (*iter)->HorizontalContentOverlap(elem); });
            if (i != end) {
                // update the yRel accordingly
                (*iter)->CalcDrawingYRel(params->m_doc, this, *i);
                i++;
            }
        }
        //  Now update the staffAlignment max overflow (above or below) and add the positioner to the list of
        //  overflowing elements
        if (place == STAFFREL_basic_above) {
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
            if ((*iter)->GetDrawingPlace() == STAFFREL_basic_above) {
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
        // We must have found it
        assert(i != grpIdYRel.end());
        (*iter)->SetDrawingYRel((*i).second);
    }

    return FUNCTOR_SIBLINGS;
}

int StaffAlignment::AdjustStaffOverlap(FunctorParams *functorParams)
{
    AdjustStaffOverlapParams *params = dynamic_cast<AdjustStaffOverlapParams *>(functorParams);
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
         iter != params->m_previous->m_overflowBelowBBoxes.end(); ++iter) {
        auto i = m_overflowAboveBBoxes.begin();
        auto end = m_overflowAboveBBoxes.end();
        while (i != end) {
            // find all the elements from the bottom staff that have an overflow at the top with an horizontal overlap
            i = std::find_if(i, end, [iter](BoundingBox *elem) { return (*iter)->HorizontalContentOverlap(elem); });
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

int StaffAlignment::AlignVerticallyEnd(FunctorParams *functorParams)
{
    AlignVerticallyParams *params = dynamic_cast<AlignVerticallyParams *>(functorParams);
    assert(params);

    SetYRel(-params->m_cumulatedShift);

    params->m_cumulatedShift
        += m_staffHeight + params->m_doc->GetOptions()->m_spacingStaff.GetValue() * params->m_doc->GetDrawingUnit(100);

    return FUNCTOR_CONTINUE;
}

int StaffAlignment::AdjustYPos(FunctorParams *functorParams)
{
    AdjustYPosParams *params = dynamic_cast<AdjustYPosParams *>(functorParams);
    assert(params);

    int maxOverflowAbove;
    if (params->m_previousVerseCount > 0) {
        maxOverflowAbove = params->m_previousOverflowBelow + m_overflowAbove;
    }
    else {
        // The maximum between the overflow below of the previous staff and the overflow above of the current
        maxOverflowAbove = std::max(params->m_previousOverflowBelow, m_overflowAbove);

        // If we have some overlap, add it
        if (m_overlap) maxOverflowAbove += m_overlap;
    }

    // Add a margin
    maxOverflowAbove += params->m_doc->GetBottomMargin(STAFF) * params->m_doc->GetDrawingUnit(this->GetStaffSize());

    // Is the maximum the overflow (+ overlap) shift, or the default ?
    maxOverflowAbove -= params->m_doc->GetOptions()->m_spacingStaff.GetValue() * params->m_doc->GetDrawingUnit(100);
    // Is the maximum the overflow (+ overlap) shift, or the default ?
    int shift = std::max(0, maxOverflowAbove);

    params->m_cumulatedShift += shift;

    SetYRel(GetYRel() - params->m_cumulatedShift);

    params->m_previousOverflowBelow = m_overflowBelow;
    params->m_previousVerseCount = this->GetVerseCount();

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

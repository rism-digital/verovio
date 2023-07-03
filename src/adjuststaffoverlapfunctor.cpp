/////////////////////////////////////////////////////////////////////////////
// Name:        adjuststaffoverlapfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjuststaffoverlapfunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "floatingobject.h"
#include "system.h"
#include "verticalaligner.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustStaffOverlapFunctor
//----------------------------------------------------------------------------

AdjustStaffOverlapFunctor::AdjustStaffOverlapFunctor(Doc *doc) : DocFunctor(doc)
{
    m_previous = NULL;
}

FunctorCode AdjustStaffOverlapFunctor::VisitStaffAlignment(StaffAlignment *staffAlignment)
{
    // This is the first alignment
    if (m_previous == NULL) {
        m_previous = staffAlignment;
        return FUNCTOR_SIBLINGS;
    }

    const int spacing = std::max(m_previous->GetOverflowBelow(), staffAlignment->GetOverflowAbove());

    // Calculate the overlap for scoreDef clefs
    int overflowBelow = m_previous->GetScoreDefClefOverflowBelow();
    int overflowAbove = staffAlignment->GetScoreDefClefOverflowAbove();
    if (spacing < (overflowBelow + overflowAbove)) {
        staffAlignment->SetOverlap((overflowBelow + overflowAbove) - spacing);
    }

    staffAlignment->AdjustBracketGroupSpacing(m_doc, m_previous, spacing);

    // Calculate the requested spacing
    const int currentStaffDistance = m_previous->GetYRel() - m_previous->GetStaffHeight() - staffAlignment->GetYRel();
    const int requestedSpace = std::max(staffAlignment->GetRequestedSpaceAbove(), m_previous->GetRequestedSpaceBelow());
    if (requestedSpace > 0) {
        staffAlignment->SetRequestedSpacing(currentStaffDistance + requestedSpace);
    }

    // This is the bottom alignment (or something is wrong) - this is all we need to do
    if (!staffAlignment->GetStaff()) {
        return FUNCTOR_STOP;
    }

    const int staffSize = staffAlignment->GetStaffSize();
    const int drawingUnit = m_doc->GetDrawingUnit(staffSize);

    // go through all the elements of the top staff that have an overflow below
    for (BoundingBox *bboxBelow : m_previous->GetBBoxesBelow()) {
        const ArrayOfBoundingBoxes &bboxesAbove = staffAlignment->GetBBoxesAbove();
        auto iter = bboxesAbove.begin();
        auto end = bboxesAbove.end();
        while (iter != end) {
            // find all the elements from the bottom staff that have an overflow at the top with an horizontal overlap
            iter = std::find_if(iter, end, [bboxBelow, drawingUnit](BoundingBox *elem) {
                if (bboxBelow->Is(FLOATING_POSITIONER)) {
                    FloatingPositioner *fp = vrv_cast<FloatingPositioner *>(bboxBelow);
                    if (fp->GetObject()->Is({ DIR, DYNAM, TEMPO }) && fp->GetObject()->IsExtenderElement()) {
                        return bboxBelow->HorizontalContentOverlap(elem, drawingUnit * 4)
                            || bboxBelow->VerticalContentOverlap(elem);
                    }
                }
                return bboxBelow->HorizontalContentOverlap(elem);
            });
            if (iter != end) {
                // calculate the vertical overlap and see if this is more than the expected space
                int overflowBelow = m_previous->CalcOverflowBelow(bboxBelow);
                int overflowAbove = staffAlignment->CalcOverflowAbove(*iter);
                int minSpaceBetween = 0;
                if ((bboxBelow->Is(ARTIC) && ((*iter)->Is({ ARTIC, NOTE })))
                    || (bboxBelow->Is(NOTE) && ((*iter)->Is(ARTIC)))) {
                    minSpaceBetween = drawingUnit;
                }
                if (spacing < (overflowBelow + overflowAbove + minSpaceBetween)) {
                    staffAlignment->SetOverlap((overflowBelow + overflowAbove + minSpaceBetween) - spacing);
                }
                ++iter;
            }
        }
    }

    m_previous = staffAlignment;

    return FUNCTOR_SIBLINGS;
}

FunctorCode AdjustStaffOverlapFunctor::VisitSystem(System *system)
{
    m_previous = NULL;
    system->m_systemAligner.Process(*this);

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv

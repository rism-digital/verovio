/////////////////////////////////////////////////////////////////////////////
// Name:        adjustdotsfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustdotsfunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "elementpart.h"
#include "score.h"
#include "staff.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustDotsFunctor
//----------------------------------------------------------------------------

AdjustDotsFunctor::AdjustDotsFunctor(Doc *doc, const std::vector<int> &staffNs) : DocFunctor(doc)
{
    m_staffNs = staffNs;
}

FunctorCode AdjustDotsFunctor::VisitAlignmentEnd(Alignment *alignment)
{
    // process dots only if there is at least 1 dot (vertical group) in the alignment
    if (!m_elements.empty() && !m_dots.empty()) {
        // multimap of overlapping dots with other elements
        std::multimap<LayerElement *, LayerElement *> overlapElements;

        // Try to find which dots can be grouped together. To achieve this, find layer elements that collide with these
        // dots. Then find if their parents (note/chord) have dots - if they do then we can group these dots together,
        // otherwise they should be kept separate
        for (Dots *dots : m_dots) {
            // A third staff size will be used as required margin
            const Staff *staff = dots->GetAncestorStaff(RESOLVE_CROSS_STAFF);
            const int staffSize = staff->m_drawingStaffSize;
            const int thirdUnit = m_doc->GetDrawingUnit(staffSize) / 3;

            for (LayerElement *element : m_elements) {
                if (dots->HorizontalSelfOverlap(element, thirdUnit)
                    && dots->VerticalSelfOverlap(element, 2 * thirdUnit)) {
                    if (element->Is({ CHORD, NOTE })) {
                        if (dynamic_cast<AttAugmentDots *>(element)->GetDots() < 1) continue;
                        overlapElements.emplace(dots, element);
                    }
                    else if (Object *chord = element->GetFirstAncestor(CHORD); chord) {
                        if (vrv_cast<Chord *>(chord)->GetDots() < 1) continue;
                        overlapElements.emplace(dots, vrv_cast<LayerElement *>(chord));
                    }
                    else if (Object *note = element->GetFirstAncestor(NOTE); note) {
                        if (vrv_cast<Note *>(note)->GetDots() < 1) continue;
                        overlapElements.emplace(dots, vrv_cast<LayerElement *>(note));
                    }
                }
            }
        }

        // if at least one overlapping element has been found, make sure to adjust relative positioning of the dots in
        // the group to the rightmost one
        if (!overlapElements.empty()) {
            for (Dots *dots : m_dots) {
                auto pair = overlapElements.equal_range(dots);
                int max = 0;
                for (auto it = pair.first; it != pair.second; ++it) {
                    const int diff = it->second->GetDrawingX() + it->first->GetDrawingXRel() - it->first->GetDrawingX();
                    if (diff > max) max = diff;
                }
                if (max) dots->SetDrawingXRel(dots->GetDrawingXRel() + max);
                dots->IsAdjusted(true);
            }
        }
    }

    m_elements.clear();
    m_dots.clear();

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustDotsFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (layerElement->Is(NOTE) && layerElement->GetParent()->Is(CHORD)) return FUNCTOR_SIBLINGS;
    if (layerElement->Is(DOTS)) {
        m_dots.push_back(vrv_cast<Dots *>(layerElement));
    }
    else {
        m_elements.push_back(layerElement);
    }

    return FUNCTOR_SIBLINGS;
}

FunctorCode AdjustDotsFunctor::VisitMeasure(Measure *measure)
{
    if (!measure->HasAlignmentRefWithMultipleLayers()) return FUNCTOR_SIBLINGS;

    Filters filters;
    Filters *previousFilters = this->SetFilters(&filters);

    for (int &n : m_staffNs) {
        filters.Clear();
        // Create ad comparison object for each type / @n
        std::vector<int> ns;
        // -1 for barline attributes that need to be taken into account each time
        ns.push_back(BARLINE_REFERENCES);
        ns.push_back(n);
        AttNIntegerAnyComparison matchStaff(ALIGNMENT_REFERENCE, ns);
        filters.Add(&matchStaff);

        // Process on measure aligner with filters
        measure->m_measureAligner.Process(*this);
    }

    this->SetFilters(previousFilters);

    return FUNCTOR_SIBLINGS;
}

FunctorCode AdjustDotsFunctor::VisitScore(Score *score)
{
    assert(score->GetScoreDef());

    m_staffNs = score->GetScoreDef()->GetStaffNs();

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

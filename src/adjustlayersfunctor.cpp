/////////////////////////////////////////////////////////////////////////////
// Name:        adjustlayersfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustlayersfunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "score.h"
#include "staff.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustLayersFunctor
//----------------------------------------------------------------------------

AdjustLayersFunctor::AdjustLayersFunctor(Doc *doc, const std::vector<int> &staffNs) : DocFunctor(doc)
{
    m_staffNs = staffNs;
    m_currentLayerN = VRV_UNSET;
    m_unison = false;
    m_ignoreDots = true;
    m_stemSameas = false;
    m_accumulatedShift = 0;
}

FunctorCode AdjustLayersFunctor::VisitAlignmentReference(AlignmentReference *alignmentReference)
{
    if (!alignmentReference->HasMultipleLayer()) return FUNCTOR_SIBLINGS;

    m_currentLayerN = VRV_UNSET;
    m_current.clear();
    m_previous.clear();
    m_accumulatedShift = 0;

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustLayersFunctor::VisitAlignmentReferenceEnd(AlignmentReference *alignmentReference)
{
    // Determine staff
    if (m_current.empty()) return FUNCTOR_CONTINUE;
    LayerElement *firstElem = m_current.at(0);
    Staff *staff = firstElem->GetAncestorStaff(RESOLVE_CROSS_STAFF);

    const int extension
        = m_doc->GetDrawingLedgerLineExtension(staff->m_drawingStaffSize, firstElem->GetDrawingCueSize());

    if ((abs(m_accumulatedShift) < 2 * extension) && m_ignoreDots) {
        // Check each pair of notes from different layers for possible collisions of ledger lines with note stems
        const bool handleLedgerLineStemCollision
            = std::any_of(m_current.begin(), m_current.end(), [this, staff](LayerElement *currentElem) {
                  if (!currentElem->Is(NOTE)) return false;
                  Note *currentNote = vrv_cast<Note *>(currentElem);
                  assert(currentNote);

                  return std::any_of(
                      m_previous.begin(), m_previous.end(), [this, staff, currentNote](LayerElement *previousElem) {
                          if (!previousElem->Is(NOTE)) return false;
                          Note *previousNote = vrv_cast<Note *>(previousElem);
                          assert(previousNote);

                          return Note::HandleLedgerLineStemCollision(m_doc, staff, currentNote, previousNote);
                      });
              });

        // To avoid collisions shift the chord or note to the left
        if (handleLedgerLineStemCollision) {
            auto itElem
                = std::find_if(m_current.begin(), m_current.end(), [](LayerElement *elem) { return elem->Is(NOTE); });
            assert(itElem != m_current.end());

            LayerElement *chord = vrv_cast<Note *>(*itElem)->IsChordTone();
            LayerElement *element = chord ? chord : (*itElem);

            const int shift = 2 * extension - abs(m_accumulatedShift);
            element->SetDrawingXRel(element->GetDrawingXRel() - shift);
        }
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustLayersFunctor::VisitLayerElement(LayerElement *layerElement)
{
    if (layerElement->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    // Check if we are starting a new layer - if yes copy the current elements to previous
    if (!m_current.empty() && (layerElement->GetAlignmentLayerN() != m_currentLayerN)) {
        m_previous.reserve(m_previous.size() + m_current.size());
        m_previous.insert(m_previous.end(), m_current.begin(), m_current.end());
        m_current.clear();
    }

    m_currentLayerN = layerElement->GetAlignmentLayerN();

    // These are the only ones we want to keep for further collision detection
    if (layerElement->HasSelfBB()) {
        if (layerElement->Is({ NOTE, STEM })) {
            m_current.push_back(layerElement);
        }
        else if (!m_ignoreDots && layerElement->Is(DOTS)) {
            m_current.push_back(layerElement);
        }
    }

    // We are processing the first layer, nothing to do yet
    if (m_previous.empty()) return FUNCTOR_SIBLINGS;

    const int shift = layerElement->AdjustOverlappingLayers(m_doc, m_previous, !m_ignoreDots, m_unison, m_stemSameas);
    m_accumulatedShift += shift;

    return FUNCTOR_SIBLINGS;
}

FunctorCode AdjustLayersFunctor::VisitMeasure(Measure *measure)
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

FunctorCode AdjustLayersFunctor::VisitScore(Score *score)
{
    assert(score->GetScoreDef());

    m_staffNs = score->GetScoreDef()->GetStaffNs();

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

/////////////////////////////////////////////////////////////////////////////
// Name:        adjustharmgrpsspacingfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustharmgrpsspacingfunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "harm.h"
#include "syl.h"
#include "system.h"
#include "vrv.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustHarmGrpsSpacingFunctor
//----------------------------------------------------------------------------

AdjustHarmGrpsSpacingFunctor::AdjustHarmGrpsSpacingFunctor(Doc *doc) : DocFunctor(doc)
{
    m_currentGrp = 0;
    m_previousHarmPositioner = NULL;
    m_previousHarmStart = NULL;
    m_previousMeasure = NULL;
    m_currentSystem = NULL;
}

FunctorCode AdjustHarmGrpsSpacingFunctor::VisitHarm(Harm *harm)
{
    int currentGrpId = harm->GetDrawingGrpId();

    // No group ID, nothing to do - should probably never happen
    if (currentGrpId == 0) {
        return FUNCTOR_SIBLINGS;
    }

    // We are filling the array of grp ids for the system
    if (m_currentGrp == 0) {
        // Look if we already have this grpId
        if (std::find(m_grpIds.begin(), m_grpIds.end(), currentGrpId) == m_grpIds.end()) {
            // if not, then just add to the list of grpIds for the system
            m_grpIds.push_back(currentGrpId);
        }
        // This is it for this pass
        return FUNCTOR_SIBLINGS;
    }
    // We are processing harm for a grp Id which is not the current one, skip it
    else if (currentGrpId != m_currentGrp) {
        return FUNCTOR_SIBLINGS;
    }

    /************** Find the widest positioner **************/

    // Get all the positioners for this object - all of them (all staves) because we can have different staff sizes
    ArrayOfFloatingPositioners positioners;
    m_currentSystem->m_systemAligner.FindAllPositionerPointingTo(&positioners, harm);

    FloatingPositioner *harmPositioner = NULL;
    // Something is probably not right if nothing found - maybe no @staff
    if (positioners.empty()) {
        LogDebug("Something was wrong when searching positioners for %s '%s'", harm->GetClassName().c_str(),
            harm->GetID().c_str());
        return FUNCTOR_SIBLINGS;
    }

    // Keep the one with the lowest left position (this will also be the widest)
    for (auto const &positioner : positioners) {
        if (!harmPositioner || (harmPositioner->GetContentLeft() > positioner->GetContentLeft())) {
            harmPositioner = positioner;
        }
    }

    // If the harm positioner is missing or is empty, do not adjust spacing
    if (!harmPositioner || !harmPositioner->HasContentBB()) {
        return FUNCTOR_SIBLINGS;
    }

    // If we have more than one harm at the same position, do not adjust them
    // This situation makes sense when the first of them is right aligned
    if (m_previousHarmStart && m_previousHarmStart == harm->GetStart()) {
        m_previousHarmPositioner = harmPositioner;
        return FUNCTOR_SIBLINGS;
    }

    /************** Calculate the adjustment **************/

    assert(harm->GetStart());
    assert(harmPositioner);

    // First harm in the system
    if (!m_previousMeasure && !m_previousHarmPositioner) {
        // Check that is it not overflowing the beginning of the measure
        int overflow = harm->GetStart()->GetDrawingX() + harmPositioner->GetContentX1();
        Measure *measure = vrv_cast<Measure *>(harm->GetFirstAncestor(MEASURE));
        if ((overflow < 0) && measure && measure->GetLeftBarLine()) {
            m_overlappingHarm.push_back(std::make_tuple(
                measure->GetLeftBarLine()->GetAlignment(), harm->GetStart()->GetAlignment(), -overflow));
        }
    }

    // Not much to do when we hit the first harm of the system
    if (m_previousHarmPositioner == NULL) {
        m_previousHarmStart = harm->GetStart();
        m_previousHarmPositioner = harmPositioner;
        m_previousMeasure = NULL;
        return FUNCTOR_SIBLINGS;
    }

    int xShift = 0;

    // We have a previous harm from the previous measure - we need to add the measure width because the measures are
    // not aligned yet
    if (m_previousMeasure) {
        xShift = m_previousMeasure->GetWidth();
    }

    assert(m_previousHarmStart);
    assert(m_previousHarmPositioner);

    int overlap = m_previousHarmPositioner->GetContentRight() - (harmPositioner->GetContentLeft() + xShift);
    // Two units as default spacing
    int wordSpace = 2 * m_doc->GetDrawingUnit(100);
    Syl::AdjustToLyricSize(m_doc, wordSpace);
    overlap += wordSpace;

    if (overlap > 0) {
        // We are adjusting harms in two different measures - move only the right barline of the first measure
        if (m_previousMeasure) {
            m_overlappingHarm.push_back(std::make_tuple(
                m_previousHarmStart->GetAlignment(), m_previousMeasure->GetRightBarLine()->GetAlignment(), overlap));
            // Do it now
            m_previousMeasure->m_measureAligner.AdjustProportionally(m_overlappingHarm);
            m_overlappingHarm.clear();
        }
        else {
            // Normal case, both in the same measure
            m_overlappingHarm.push_back(
                std::make_tuple(m_previousHarmStart->GetAlignment(), harm->GetStart()->GetAlignment(), overlap));
        }
    }

    m_previousHarmStart = harm->GetStart();
    m_previousHarmPositioner = harmPositioner;
    m_previousMeasure = NULL;

    return FUNCTOR_SIBLINGS;
}

FunctorCode AdjustHarmGrpsSpacingFunctor::VisitMeasureEnd(Measure *measure)
{
    // At the end of the measure - pass it along for overlapping verses
    m_previousMeasure = measure;

    // Adjust the position of the alignment according to what we have collected for this harm grp
    measure->m_measureAligner.AdjustProportionally(m_overlappingHarm);
    m_overlappingHarm.clear();

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustHarmGrpsSpacingFunctor::VisitSystem(System *system)
{
    // reset it, but not the current grpId!
    m_currentSystem = system;
    m_overlappingHarm.clear();
    m_previousHarmPositioner = NULL;
    m_previousHarmStart = NULL;
    m_previousMeasure = NULL;

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustHarmGrpsSpacingFunctor::VisitSystemEnd(System *system)
{
    // End of the first pass - loop over for each group id
    if (m_currentGrp == 0) {
        for (auto grpId : m_grpIds) {
            m_currentGrp = grpId;
            system->Process(*this);
        }
        // Make sure we reset it for the next system
        m_currentGrp = 0;
        return FUNCTOR_CONTINUE;
    }

    /************** End of a system when actually adjusting **************/

    if (!m_previousMeasure) {
        return FUNCTOR_CONTINUE;
    }

    // Here we also need to handle the last harm of the measure - we check the alignment with the right barline
    if (m_previousHarmPositioner) {
        const Object *positioner = m_previousHarmPositioner->GetObject();
        assert(positioner);
        // We do this only if the harm is in the last measure
        if (m_previousMeasure == positioner->GetFirstAncestor(MEASURE)) {
            int overlap = m_previousHarmPositioner->GetContentRight()
                - m_previousMeasure->GetRightBarLine()->GetAlignment()->GetXRel();

            if (overlap > 0) {
                m_overlappingHarm.push_back(std::make_tuple(m_previousHarmStart->GetAlignment(),
                    m_previousMeasure->GetRightBarLine()->GetAlignment(), overlap));
            }
        }
    }

    // Adjust the position of the alignment according to what we have collected for this harm group id
    m_previousMeasure->m_measureAligner.AdjustProportionally(m_overlappingHarm);
    m_overlappingHarm.clear();

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

/////////////////////////////////////////////////////////////////////////////
// Name:        adjustxoverflowfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustxoverflowfunctor.h"

//----------------------------------------------------------------------------

#include "controlelement.h"
#include "floatingobject.h"
#include "system.h"
#include "vrv.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustXOverflowFunctor
//----------------------------------------------------------------------------

AdjustXOverflowFunctor::AdjustXOverflowFunctor(int margin) : Functor()
{
    m_currentSystem = NULL;
    m_lastMeasure = NULL;
    m_currentWidest = NULL;
    m_margin = margin;
}

FunctorCode AdjustXOverflowFunctor::VisitControlElement(ControlElement *controlElement)
{
    if (!controlElement->Is({ CPMARK, DIR, DYNAM, ORNAM, REPEATMARK, TEMPO })) {
        return FUNCTOR_SIBLINGS;
    }

    // Right aligned cannot overflow
    if (controlElement->GetChildRendAlignment() == HORIZONTALALIGNMENT_right) {
        return FUNCTOR_SIBLINGS;
    }

    assert(m_currentSystem);

    // Get all the positioners for this object - all of them (all staves) because we can have different staff sizes
    ArrayOfFloatingPositioners positioners;
    m_currentSystem->m_systemAligner.FindAllPositionerPointingTo(&positioners, controlElement);

    // Something is probably not right if nothing found - maybe no @staff
    if (positioners.empty()) {
        LogDebug("Something was wrong when searching positioners for %s '%s'", controlElement->GetClassName().c_str(),
            controlElement->GetID().c_str());
        return FUNCTOR_SIBLINGS;
    }

    // Keep the one with the highest right position
    for (auto const &positioner : positioners) {
        if (!m_currentWidest || (m_currentWidest->GetContentRight() < positioner->GetContentRight())) {
            m_currentWidest = positioner;
        }
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustXOverflowFunctor::VisitMeasure(Measure *measure)
{
    m_lastMeasure = measure;
    // For now look only at the content of the last measure, so discard any previous control event.
    // We need to do this because AdjustXOverflow is run before measures are aligned, so the right
    // position comparison do not actually tell us which one is the longest. This is not optimal
    // and can be improved.
    m_currentWidest = NULL;

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustXOverflowFunctor::VisitSystem(System *system)
{
    m_currentSystem = system;
    m_lastMeasure = NULL;
    m_currentWidest = NULL;

    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustXOverflowFunctor::VisitSystemEnd(System *system)
{
    // Continue if no measure or no widest element
    if (!m_lastMeasure || !m_currentWidest) {
        return FUNCTOR_CONTINUE;
    }

    // Continue if the right position of the measure is larger than the widest element right
    int measureRightX = m_lastMeasure->GetDrawingX() + m_lastMeasure->GetRightBarLineLeft() - m_margin;
    if (measureRightX > m_currentWidest->GetContentRight()) {
        return FUNCTOR_CONTINUE;
    }

    LayerElement *objectX = dynamic_cast<LayerElement *>(m_currentWidest->GetObjectX());
    if (!objectX) {
        return FUNCTOR_CONTINUE;
    }
    Alignment *left = objectX->GetAlignment();
    Measure *objectXMeasure = vrv_cast<Measure *>(objectX->GetFirstAncestor(MEASURE));
    if (objectXMeasure != m_lastMeasure) {
        left = m_lastMeasure->GetLeftBarLine()->GetAlignment();
    }

    int overflow = m_currentWidest->GetContentRight() - measureRightX;
    if (overflow > 0) {
        ArrayOfAdjustmentTuples boundaries{ std::make_tuple(
            left, m_lastMeasure->GetRightBarLine()->GetAlignment(), overflow) };
        m_lastMeasure->m_measureAligner.AdjustProportionally(boundaries);
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

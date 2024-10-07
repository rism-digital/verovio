/////////////////////////////////////////////////////////////////////////////
// Name:        findlayerelementsfunctor.cpp
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "findlayerelementsfunctor.h"

//----------------------------------------------------------------------------

#include "alignfunctor.h"
#include "layer.h"
#include "layerelement.h"
#include "staff.h"

namespace vrv {

//----------------------------------------------------------------------------
// LayersInTimeSpanFunctor
//----------------------------------------------------------------------------

LayersInTimeSpanFunctor::LayersInTimeSpanFunctor(const MeterSig *meterSig, const Mensur *mensur) : ConstFunctor()
{
    m_meterParams.meterSig = meterSig;
    m_meterParams.mensur = mensur;
}

void LayersInTimeSpanFunctor::SetEvent(const Fraction &time, const Fraction &duration)
{
    m_time = time;
    m_duration = duration;
}

FunctorCode LayersInTimeSpanFunctor::VisitLayerElement(const LayerElement *layerElement)
{
    if (layerElement->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    // For mRest we do not look at the time span
    if (layerElement->Is(MREST)) {
        // Add the layerN to the list of layers occuring in this time frame
        m_layers.insert(layerElement->GetAlignmentLayerN());

        return FUNCTOR_SIBLINGS;
    }

    if (!layerElement->GetDurationInterface() || layerElement->Is(MSPACE) || layerElement->Is(SPACE)
        || layerElement->HasSameasLink())
        return FUNCTOR_CONTINUE;
    if (layerElement->Is(NOTE) && layerElement->GetParent()->Is(CHORD)) return FUNCTOR_CONTINUE;

    Fraction duration = layerElement->GetAlignmentDuration(m_meterParams);
    Fraction time = layerElement->GetAlignment()->GetTime();

    // The event is starting after the end of the element
    if ((time + duration) <= m_time) {
        return FUNCTOR_CONTINUE;
    }
    // The element is starting after the event end - we can stop here
    else if (time >= (m_time + m_duration)) {
        return FUNCTOR_STOP;
    }

    // Add the layerN to the list of layers occuring in this time frame
    m_layers.insert(layerElement->GetAlignmentLayerN());

    // Not need to recurse for chords? Not quite sure about it.
    return (layerElement->Is(CHORD)) ? FUNCTOR_SIBLINGS : FUNCTOR_CONTINUE;
}

FunctorCode LayersInTimeSpanFunctor::VisitMensur(const Mensur *mensur)
{
    m_meterParams.mensur = mensur;

    return FUNCTOR_CONTINUE;
}

FunctorCode LayersInTimeSpanFunctor::VisitMeterSig(const MeterSig *meterSig)
{
    m_meterParams.meterSig = meterSig;

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// LayerElementsInTimeSpanFunctor
//----------------------------------------------------------------------------

LayerElementsInTimeSpanFunctor::LayerElementsInTimeSpanFunctor(
    const MeterSig *meterSig, const Mensur *mensur, const Layer *layer)
    : ConstFunctor()
{
    m_time = 0;
    m_duration = 0;
    m_meterParams.meterSig = meterSig;
    m_meterParams.mensur = mensur;
    m_layer = layer;
    m_allLayersButCurrent = false;
}

void LayerElementsInTimeSpanFunctor::SetEvent(const Fraction &time, const Fraction &duration)
{
    m_time = time;
    m_duration = duration;
}

FunctorCode LayerElementsInTimeSpanFunctor::VisitLayerElement(const LayerElement *layerElement)
{
    const Layer *currentLayer = vrv_cast<const Layer *>(layerElement->GetFirstAncestor(LAYER));
    // Either get layer refernced by @m_layer or all layers but it, depending on the @m_allLayersButCurrent flag
    if ((!m_allLayersButCurrent && (currentLayer != m_layer)) || (m_allLayersButCurrent && (currentLayer == m_layer))) {
        return FUNCTOR_SIBLINGS;
    }
    if (!currentLayer || layerElement->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    if (layerElement->HasSameasLink()) return FUNCTOR_CONTINUE;

    if (layerElement->Is(MREST)) {
        m_elements.push_back(layerElement);
        return FUNCTOR_CONTINUE;
    }

    if (!layerElement->GetDurationInterface() || layerElement->Is({ MSPACE, SPACE })) return FUNCTOR_CONTINUE;

    Fraction duration = !layerElement->GetFirstAncestor(CHORD)
        ? layerElement->GetAlignmentDuration(m_meterParams)
        : vrv_cast<const Chord *>(layerElement->GetFirstAncestor(CHORD))->GetAlignmentDuration(m_meterParams);

    Fraction time = layerElement->GetAlignment()->GetTime();

    // The event is starting after the end of the element
    if ((time + duration) <= m_time) return FUNCTOR_CONTINUE;
    // The element is starting after the event end - we can stop here
    if (time >= (m_time + m_duration)) return FUNCTOR_STOP;

    if (layerElement->Is(NOTE)) {
        const Note *note = vrv_cast<const Note *>(layerElement);
        const Chord *chord = note->IsChordTone();
        if (chord && (std::find(m_elements.begin(), m_elements.end(), chord) != m_elements.end())) {
            return FUNCTOR_CONTINUE;
        }
    }
    m_elements.push_back(layerElement);

    // Not need to recurse for chords
    return layerElement->Is(CHORD) ? FUNCTOR_SIBLINGS : FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// FindSpannedLayerElementsFunctor
//----------------------------------------------------------------------------

FindSpannedLayerElementsFunctor::FindSpannedLayerElementsFunctor(const TimeSpanningInterface *interface)
    : ConstFunctor()
{
    m_interface = interface;
    m_minPos = 0;
    m_maxPos = 0;
    m_minLayerN = 0;
    m_maxLayerN = 0;
}

void FindSpannedLayerElementsFunctor::SetMinMaxPos(int minPos, int maxPos)
{
    m_minPos = minPos;
    m_maxPos = maxPos;
}

void FindSpannedLayerElementsFunctor::SetMinMaxLayerN(int minLayerN, int maxLayerN)
{
    m_minLayerN = minLayerN;
    m_maxLayerN = maxLayerN;
}

FunctorCode FindSpannedLayerElementsFunctor::VisitLayerElement(const LayerElement *layerElement)
{
    if (layerElement->IsScoreDefElement()) return FUNCTOR_SIBLINGS;

    if (!layerElement->Is(m_classIds)) {
        return FUNCTOR_CONTINUE;
    }

    if (layerElement->HasContentBB() && !layerElement->HasEmptyBB() && (layerElement->GetContentRight() > m_minPos)
        && (layerElement->GetContentLeft() < m_maxPos)) {

        // We skip the start or end of the slur
        const LayerElement *start = m_interface->GetStart();
        const LayerElement *end = m_interface->GetEnd();
        if ((layerElement == start) || (layerElement == end)) {
            return FUNCTOR_CONTINUE;
        }

        // Skip if neither parent staff nor cross staff matches the given staff number
        if (!m_staffNs.empty()) {
            const Staff *staff = layerElement->GetAncestorStaff();
            if (m_staffNs.find(staff->GetN()) == m_staffNs.end()) {
                const Layer *layer = NULL;
                staff = layerElement->GetCrossStaff(layer);
                if (!staff || (m_staffNs.find(staff->GetN()) == m_staffNs.end())) {
                    return FUNCTOR_CONTINUE;
                }
            }
        }

        // Skip if layer number is outside given bounds
        const int layerN = layerElement->GetOriginalLayerN();
        if (m_minLayerN && (m_minLayerN > layerN)) {
            return FUNCTOR_CONTINUE;
        }
        if (m_maxLayerN && (m_maxLayerN < layerN)) {
            return FUNCTOR_CONTINUE;
        }

        // Skip elements aligned at start/end, but on a different staff
        if ((layerElement->GetAlignment() == start->GetAlignment()) && !start->Is(TIMESTAMP_ATTR)) {
            const Staff *staff = layerElement->GetAncestorStaff(RESOLVE_CROSS_STAFF);
            const Staff *startStaff = start->GetAncestorStaff(RESOLVE_CROSS_STAFF);
            if (staff->GetN() != startStaff->GetN()) {
                return FUNCTOR_CONTINUE;
            }
        }
        if ((layerElement->GetAlignment() == end->GetAlignment()) && !end->Is(TIMESTAMP_ATTR)) {
            const Staff *staff = layerElement->GetAncestorStaff(RESOLVE_CROSS_STAFF);
            const Staff *endStaff = end->GetAncestorStaff(RESOLVE_CROSS_STAFF);
            if (staff->GetN() != endStaff->GetN()) {
                return FUNCTOR_CONTINUE;
            }
        }

        m_elements.push_back(layerElement);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode FindSpannedLayerElementsFunctor::VisitMeasure(const Measure *measure)
{
    if (Object::IsPreOrdered(measure, m_interface->GetStartMeasure())) return FUNCTOR_SIBLINGS;

    if (Object::IsPreOrdered(m_interface->GetEndMeasure(), measure)) return FUNCTOR_SIBLINGS;

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// GetRelativeLayerElementFunctor
//----------------------------------------------------------------------------

GetRelativeLayerElementFunctor::GetRelativeLayerElementFunctor(int elementIndex, bool anotherLayer) : ConstFunctor()
{
    m_relativeElement = NULL;
    m_initialElementIndex = elementIndex;
    m_isInNeighboringLayer = anotherLayer;
}

FunctorCode GetRelativeLayerElementFunctor::VisitLayerElement(const LayerElement *layerElement)
{
    // Do not check for index of the element if we're looking into neighboring layer or if nested element is being
    // processed (e.g. ignore index children of beams, since they have their own indices irrelevant to the one that
    // has been passed inside this functor)
    if (!m_isInNeighboringLayer && layerElement->GetParent()->Is(LAYER)) {
        if ((this->GetDirection() == FORWARD) && (layerElement->GetIdx() < m_initialElementIndex)) {
            return FUNCTOR_SIBLINGS;
        }
        if ((this->GetDirection() == BACKWARD) && (layerElement->GetIdx() > m_initialElementIndex)) {
            return FUNCTOR_SIBLINGS;
        }
    }

    if (layerElement->Is({ NOTE, CHORD, FTREM })) {
        m_relativeElement = layerElement;
        return FUNCTOR_STOP;
    }

    if (layerElement->Is(REST)) return m_isInNeighboringLayer ? FUNCTOR_STOP : FUNCTOR_SIBLINGS;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

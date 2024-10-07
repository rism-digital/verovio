/////////////////////////////////////////////////////////////////////////////
// Name:        findfunctor.cpp
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "findfunctor.h"

//----------------------------------------------------------------------------

#include "comparison.h"
#include "layer.h"
#include "object.h"
#include "plistinterface.h"
#include "score.h"

namespace vrv {

//----------------------------------------------------------------------------
// FindAllByComparisonFunctor
//----------------------------------------------------------------------------

FindAllByComparisonFunctor::FindAllByComparisonFunctor(Comparison *comparison, ListOfObjects *elements) : Functor()
{
    m_comparison = comparison;
    m_elements = elements;
    m_continueDepthSearchForMatches = true;
}

void FindAllByComparisonFunctor::SetContinueDepthSearchForMatches(bool continueDepthSearchForMatches)
{
    m_continueDepthSearchForMatches = continueDepthSearchForMatches;
}

FunctorCode FindAllByComparisonFunctor::VisitObject(Object *object)
{
    // evaluate by applying the Comparison operator()
    if ((*m_comparison)(object)) {
        m_elements->push_back(object);
        if (!m_continueDepthSearchForMatches) {
            return FUNCTOR_SIBLINGS;
        }
    }
    // continue until the end
    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// FindAllConstByComparisonFunctor
//----------------------------------------------------------------------------

FindAllConstByComparisonFunctor::FindAllConstByComparisonFunctor(Comparison *comparison, ListOfConstObjects *elements)
    : ConstFunctor()
{
    m_comparison = comparison;
    m_elements = elements;
    m_continueDepthSearchForMatches = true;
}

void FindAllConstByComparisonFunctor::SetContinueDepthSearchForMatches(bool continueDepthSearchForMatches)
{
    m_continueDepthSearchForMatches = continueDepthSearchForMatches;
}

FunctorCode FindAllConstByComparisonFunctor::VisitObject(const Object *object)
{
    // evaluate by applying the Comparison operator()
    if ((*m_comparison)(object)) {
        m_elements->push_back(object);
        if (!m_continueDepthSearchForMatches) {
            return FUNCTOR_SIBLINGS;
        }
    }
    // continue until the end
    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// FindAllBetweenFunctor
//----------------------------------------------------------------------------

FindAllBetweenFunctor::FindAllBetweenFunctor(
    Comparison *comparison, ListOfConstObjects *elements, const Object *start, const Object *end)
    : ConstFunctor()
{
    m_comparison = comparison;
    m_elements = elements;
    m_start = start;
    m_end = end;
}

FunctorCode FindAllBetweenFunctor::VisitObject(const Object *object)
{
    // We are reaching the start of the range
    if (m_start == object) {
        // Setting the start to NULL indicates that we are in the range
        m_start = NULL;
    }
    // We have not reached the start yet
    else if (m_start) {
        return FUNCTOR_CONTINUE;
    }

    // evaluate by applying the Comparison operator()
    if ((*m_comparison)(object)) {
        m_elements->push_back(object);
    }

    // We have reached the end of the range
    if (m_end == object) {
        return FUNCTOR_STOP;
    }

    // continue until the end
    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// FindByComparisonFunctor
//----------------------------------------------------------------------------

FindByComparisonFunctor::FindByComparisonFunctor(Comparison *comparison) : ConstFunctor()
{
    m_comparison = comparison;
    m_element = NULL;
}

FunctorCode FindByComparisonFunctor::VisitObject(const Object *object)
{
    if (m_element) {
        // this should not happen, but just in case
        return FUNCTOR_STOP;
    }

    // evaluate by applying the Comparison operator()
    if ((*m_comparison)(object)) {
        m_element = object;
        return FUNCTOR_STOP;
    }

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// FindByIDFunctor
//----------------------------------------------------------------------------

FindByIDFunctor::FindByIDFunctor(const std::string &id) : ConstFunctor()
{
    m_id = id;
    m_element = NULL;
}

FunctorCode FindByIDFunctor::VisitObject(const Object *object)
{
    if (m_element) {
        // this should not happen, but just in case
        return FUNCTOR_STOP;
    }

    if (m_id == object->GetID()) {
        m_element = object;
        return FUNCTOR_STOP;
    }

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// FindNextChildByComparisonFunctor
//----------------------------------------------------------------------------

FindNextChildByComparisonFunctor::FindNextChildByComparisonFunctor(Comparison *comparison, const Object *start)
    : ConstFunctor()
{
    m_comparison = comparison;
    m_start = start;
    m_element = NULL;
}

FunctorCode FindNextChildByComparisonFunctor::VisitObject(const Object *object)
{
    // we are reaching the start of the range
    if (m_start == object) {
        // setting m_start to be null tells us that we're in the range
        m_start = NULL;
        return FUNCTOR_CONTINUE;
    }

    else if (m_start) {
        // we're not yet in the range
        return FUNCTOR_CONTINUE;
    }

    if ((*m_comparison)(object)) {
        m_element = object;
        return FUNCTOR_STOP;
    }

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// FindPreviousChildByComparisonFunctor
//----------------------------------------------------------------------------

FindPreviousChildByComparisonFunctor::FindPreviousChildByComparisonFunctor(Comparison *comparison, const Object *start)
    : ConstFunctor()
{
    m_comparison = comparison;
    m_start = start;
    m_element = NULL;
}

FunctorCode FindPreviousChildByComparisonFunctor::VisitObject(const Object *object)
{
    // this guy works by going from the start and replacing the return element with every nearer element
    // until you get to the 'start' element
    if (m_start == object) {
        // we've reached the end element, so stop
        return FUNCTOR_STOP;
    }

    if ((*m_comparison)(object)) {
        m_element = object;
    }

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// FindExtremeByComparisonFunctor
//----------------------------------------------------------------------------

FindExtremeByComparisonFunctor::FindExtremeByComparisonFunctor(Comparison *comparison) : ConstFunctor()
{
    m_comparison = comparison;
    m_element = NULL;
}

FunctorCode FindExtremeByComparisonFunctor::VisitObject(const Object *object)
{
    // evaluate by applying the Comparison operator()
    if ((*m_comparison)(object)) {
        m_element = object;
    }
    // continue until the end
    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// FindAllReferencedObjectsFunctor
//----------------------------------------------------------------------------

FindAllReferencedObjectsFunctor::FindAllReferencedObjectsFunctor(ListOfObjects *elements) : Functor()
{
    m_elements = elements;
    m_milestoneReferences = false;
}

FunctorCode FindAllReferencedObjectsFunctor::VisitObject(Object *object)
{
    if (object->HasInterface(INTERFACE_LINKING)) {
        LinkingInterface *interface = object->GetLinkingInterface();
        assert(interface);
        if (interface->GetNextLink()) m_elements->push_back(interface->GetNextLink());
        if (interface->GetSameasLink()) m_elements->push_back(interface->GetSameasLink());
    }
    if (object->HasInterface(INTERFACE_PLIST)) {
        PlistInterface *interface = object->GetPlistInterface();
        assert(interface);
        for (Object *object : interface->GetRefs()) {
            m_elements->push_back(object);
        }
    }
    if (object->HasInterface(INTERFACE_TIME_POINT) || object->HasInterface(INTERFACE_TIME_SPANNING)) {
        TimePointInterface *interface = object->GetTimePointInterface();
        assert(interface);
        if (interface->GetStart() && !interface->GetStart()->Is(TIMESTAMP_ATTR))
            m_elements->push_back(interface->GetStart());
    }
    if (object->HasInterface(INTERFACE_TIME_SPANNING)) {
        TimeSpanningInterface *interface = object->GetTimeSpanningInterface();
        assert(interface);
        if (interface->GetEnd() && !interface->GetEnd()->Is(TIMESTAMP_ATTR)) m_elements->push_back(interface->GetEnd());
    }
    if (object->Is(NOTE)) {
        Note *note = vrv_cast<Note *>(object);
        assert(note);
        // The note has a stem.sameas that was resolved the a note, then that one is referenced
        if (note->HasStemSameas() && note->HasStemSameasNote()) {
            m_elements->push_back(note->GetStemSameasNote());
        }
    }
    // These will also be referred to as milestones in page-based MEI
    if (m_milestoneReferences && object->IsMilestoneElement()) {
        m_elements->push_back(object);
    }

    // continue until the end
    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// FindElementInLayerStaffDefFunctor
//----------------------------------------------------------------------------

FindElementInLayerStaffDefFunctor::FindElementInLayerStaffDefFunctor(const std::string &xmlId) : ConstFunctor()
{
    m_id = xmlId;
    m_element = NULL;
}

FunctorCode FindElementInLayerStaffDefFunctor::VisitLayer(const Layer *layer)
{
    if (!layer->HasStaffDef()) return FUNCTOR_SIBLINGS;
    // Get corresponding elements from the layer
    if (layer->GetStaffDefClef() && (layer->GetStaffDefClef()->GetID() == m_id)) {
        m_element = layer->GetStaffDefClef();
    }
    else if (layer->GetStaffDefKeySig() && (layer->GetStaffDefKeySig()->GetID() == m_id)) {
        m_element = layer->GetStaffDefKeySig();
    }
    else if (layer->GetStaffDefMensur() && (layer->GetStaffDefMensur()->GetID() == m_id)) {
        m_element = layer->GetStaffDefMensur();
    }
    else if (layer->GetStaffDefMeterSig() && (layer->GetStaffDefMeterSig()->GetID() == m_id)) {
        m_element = layer->GetStaffDefMeterSig();
    }
    else if (layer->GetStaffDefMeterSigGrp() && (layer->GetStaffDefMeterSigGrp()->GetID() == m_id)) {
        m_element = layer->GetStaffDefMeterSigGrp();
    }

    return m_element ? FUNCTOR_STOP : FUNCTOR_SIBLINGS;
}

FunctorCode FindElementInLayerStaffDefFunctor::VisitScore(const Score *score)
{
    if (score->GetScoreDef()->GetID() == m_id) {
        m_element = score->GetScoreDef();
    }
    else {
        m_element = score->GetScoreDef()->FindDescendantByID(m_id);
    }

    return (m_element) ? FUNCTOR_STOP : FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// AddToFlatListFunctor
//----------------------------------------------------------------------------

AddToFlatListFunctor::AddToFlatListFunctor(ListOfConstObjects *flatList) : ConstFunctor()
{
    m_flatList = flatList;
}

FunctorCode AddToFlatListFunctor::VisitObject(const Object *object)
{
    m_flatList->push_back(object);
    // LogDebug("List %d", m_flatList->size());

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

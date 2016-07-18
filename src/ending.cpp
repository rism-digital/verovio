/////////////////////////////////////////////////////////////////////////////
// Name:        ending.h
// Author:      Laurent Pugin
// Created:     14/07/2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "ending.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "system.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// EndingBoundary
//----------------------------------------------------------------------------

EndingBoundary::EndingBoundary(EndingBoundary *startBoundary) : FloatingElement("ending-"), AttCommon()
{
    RegisterAttClass(ATT_COMMON);

    m_startBoundary = startBoundary;
    if (m_startBoundary) {
        m_startBoundary->SetEndBoundary(this);
    }
    else {
        m_endBoundary = NULL;
    }

    Reset();
}

EndingBoundary::~EndingBoundary()
{
}

void EndingBoundary::Reset()
{
    FloatingElement::Reset();
    ResetCommon();

    m_drawingMeasure = NULL;
}

//----------------------------------------------------------------------------
// EndingBoundary functor methods
//----------------------------------------------------------------------------

int EndingBoundary::PrepareEndings(ArrayPtrVoid *params)
{
    // param 0: Measure **lastMeasure
    // param 1: EndingBoundary **currentEnding
    Measure **lastMeasure = static_cast<Measure **>((*params).at(0));
    EndingBoundary **currentEnding = static_cast<EndingBoundary **>((*params).at(1));

    // This is an end boundary - we need to its pointer to the last measure we have encountered
    if (this->IsEndBoundary()) {
        if ((*lastMeasure) == NULL) {
            LogWarning("A measure cannot be set to the end ending boundary");
        }
        this->SetMeasure((*lastMeasure));
        // We are done
        (*lastMeasure) = NULL;
        (*currentEnding) = NULL;
    }
    else if (this->IsStartBoundary()) {
        (*currentEnding) = this;
    }

    return FUNCTOR_CONTINUE;
}

int EndingBoundary::ResetDrawing(ArrayPtrVoid *params)
{
    m_drawingMeasure = NULL;

    return FUNCTOR_CONTINUE;
};

int EndingBoundary::CastOffSystems(ArrayPtrVoid *params)
{
    // param 0: a pointer to the system we are taking the content from
    // param 1: a pointer the page we are adding system to (unused)
    // param 2: a pointer to the current system
    // param 3: the cummulated shift (m_drawingXRel of the first measure of the current system) (unused)
    // param 4: the system width (unused)
    // param 5: the current scoreDef width (unused)
    System *contentSystem = static_cast<System *>((*params).at(0));
    System **currentSystem = static_cast<System **>((*params).at(2));

    // Since the functor returns FUNCTOR_SIBLINGS we should never go lower than the system children
    assert(dynamic_cast<System *>(this->m_parent));

    // Special case where we use the Relinquish method.
    // We want to move the measure to the currentSystem. However, we cannot use DetachChild
    // from the content System because this screws up the iterator. Relinquish gives up
    // the ownership of the Measure - the contentSystem will be deleted afterwards.
    EndingBoundary *ending = dynamic_cast<EndingBoundary *>(contentSystem->Relinquish(this->GetIdx()));
    (*currentSystem)->AddEnding(ending);

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
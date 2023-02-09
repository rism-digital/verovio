/////////////////////////////////////////////////////////////////////////////
// Name:        calcspanningbeamspansfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "calcspanningbeamspansfunctor.h"

//----------------------------------------------------------------------------

#include "beamspan.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// CalcSpanningBeamSpansFunctor
//----------------------------------------------------------------------------

CalcSpanningBeamSpansFunctor::CalcSpanningBeamSpansFunctor(Doc *doc) : DocFunctor(doc) {}

FunctorCode CalcSpanningBeamSpansFunctor::VisitBeamSpan(BeamSpan *beamSpan)
{
    const ArrayOfObjects &beamedElements = beamSpan->GetBeamedElements();

    if (beamedElements.empty() || !beamSpan->GetStart() || !beamSpan->GetEnd()) return FUNCTOR_CONTINUE;

    Object *startSystem = beamSpan->GetStart()->GetFirstAncestor(SYSTEM);
    Object *endSystem = beamSpan->GetEnd()->GetFirstAncestor(SYSTEM);
    assert(startSystem && endSystem);
    if (startSystem == endSystem) return FUNCTOR_CONTINUE;

    // Find layerElements that belong to another system and store them in the vector alongside
    // the system they belong to. This will allow us to break down beamSpan based on the systems.
    auto iter = beamedElements.begin();
    BeamSpan::SpanIndexVector elements;
    Object *firstSystem = startSystem;
    while (iter != beamedElements.end()) {
        elements.push_back({ iter, firstSystem });
        iter = std::find_if(iter, beamedElements.end(), [&firstSystem](Object *element) {
            Object *parentSystem = element->GetFirstAncestor(SYSTEM);
            if (firstSystem == parentSystem) return false;
            firstSystem = parentSystem;
            return true;
        });
    }
    elements.push_back({ beamedElements.end(), NULL });

    // Iterator for the elements are based on the initial order of the elements, so skip current system when
    // found and process it separately in the end
    Object *currentSystem = beamSpan->GetFirstAncestor(SYSTEM);
    int currentSystemIndex = 0;
    for (int i = 0; i < (int)elements.size() - 1; ++i) {
        if (elements.at(i).second == currentSystem) {
            currentSystemIndex = i;
            continue;
        }
        beamSpan->AddSpanningSegment(m_doc, elements, i);
    }
    beamSpan->AddSpanningSegment(m_doc, elements, currentSystemIndex, false);

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

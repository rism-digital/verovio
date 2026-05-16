/////////////////////////////////////////////////////////////////////////////
// Name:        genericlayerelement.cpp
// Author:      Laurent Pugin
// Created:     2024
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "genericlayerelement.h"
//----------------------------------------------------------------------------

#include <cassert>
#include <math.h>

//----------------------------------------------------------------------------

#include "functor.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Space
//----------------------------------------------------------------------------

// static const ClassRegistrar<GenericLayerElement> s_factory("generic", GENERIC_ELEMENT);

GenericLayerElement::GenericLayerElement() : LayerElement(GENERIC_ELEMENT)
{
    LogError("Creating generic element without name");
    m_className = "[unspecified]";
    this->Reset();
}

GenericLayerElement::GenericLayerElement(const std::string &name) : LayerElement(GENERIC_ELEMENT)
{
    m_meiName = name;
    m_className = name;
    std::transform(m_className.begin(), m_className.begin() + 1, m_className.begin(), ::toupper);
    this->Reset();
}

GenericLayerElement::~GenericLayerElement() {}

void GenericLayerElement::Reset()
{
    LayerElement::Reset();
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

FunctorCode GenericLayerElement::Accept(Functor &functor)
{
    return functor.VisitGenericLayerElement(this);
}

FunctorCode GenericLayerElement::Accept(ConstFunctor &functor) const
{
    return functor.VisitGenericLayerElement(this);
}

FunctorCode GenericLayerElement::AcceptEnd(Functor &functor)
{
    return functor.VisitGenericLayerElementEnd(this);
}

FunctorCode GenericLayerElement::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitGenericLayerElementEnd(this);
}

} // namespace vrv

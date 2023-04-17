/////////////////////////////////////////////////////////////////////////////
// Name:        svg.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "svg.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "fig.h"
#include "functor.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Svg
//----------------------------------------------------------------------------

static const ClassRegistrar<Svg> s_factory("svg", SVG);

Svg::Svg() : Object(SVG, "svg-")
{
    this->Reset();
}

Svg::~Svg() {}

void Svg::Reset()
{
    Object::Reset();
}

void Svg::Set(pugi::xml_node svg)
{
    m_svg.reset();
    m_svg.append_copy(svg);
}

int Svg::GetWidth() const
{
    if (m_svg && m_svg.first_child() && m_svg.first_child().attribute("width")) {
        return atoi(m_svg.first_child().attribute("width").value()) * DEFINITION_FACTOR;
    }
    return 0;
}

int Svg::GetHeight() const
{
    if (m_svg && m_svg.first_child() && m_svg.first_child().attribute("height")) {
        return atoi(m_svg.first_child().attribute("height").value()) * DEFINITION_FACTOR;
    }
    return 0;
}

FunctorCode Svg::Accept(MutableFunctor &functor)
{
    return functor.VisitSvg(this);
}

FunctorCode Svg::Accept(ConstFunctor &functor) const
{
    return functor.VisitSvg(this);
}

FunctorCode Svg::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitSvgEnd(this);
}

FunctorCode Svg::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitSvgEnd(this);
}

} // namespace vrv

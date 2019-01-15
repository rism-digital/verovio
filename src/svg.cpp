/////////////////////////////////////////////////////////////////////////////
// Name:        svg.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "fig.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "svg.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Svg
//----------------------------------------------------------------------------

Svg::Svg() : Object("fig-")
{
    Reset();
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

} // namespace vrv

/////////////////////////////////////////////////////////////////////////////
// Name:        glyph.cpp
// Author:      Rodolfo Zitellini
// Created:     30/07/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "glyph.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Glyph
//----------------------------------------------------------------------------

Glyph::Glyph()
{
    m_x = 0;
    m_y = 0;
    m_width = 0;
    m_height = 0;
    m_horizAdvX = 0;
    m_unitsPerEm = 20480;
    m_codeStr = "[unset]";
    m_faceIdentity = 0;
    m_glyphId = 0;
}

Glyph::Glyph(int unitsPerEm)
{
    m_x = 0;
    m_y = 0;
    m_width = 0;
    m_height = 0;
    m_horizAdvX = 0;
    m_unitsPerEm = unitsPerEm * 10;
    m_codeStr = "[unset]";
    m_faceIdentity = 0;
    m_glyphId = 0;
}

Glyph::~Glyph() {}

void Glyph::SetBoundingBox(double x, double y, double w, double h)
{
    m_x = (int)(10.0 * x);
    m_y = (int)(10.0 * y);
    m_width = (int)(10.0 * w);
    m_height = (int)(10.0 * h);
}

void Glyph::GetBoundingBox(int &x, int &y, int &w, int &h) const
{
    x = m_x;
    y = m_y;
    w = m_width;
    h = m_height;
}

void Glyph::SetAnchor(std::string anchorStr, double x, double y)
{
    SMuFLGlyphAnchor anchorId;
    if (anchorStr == "stemDownNW") {
        anchorId = SMUFL_stemDownNW;
    }
    else if (anchorStr == "stemUpSE") {
        anchorId = SMUFL_stemUpSE;
    }
    else if (anchorStr == "cutOutNE") {
        anchorId = SMUFL_cutOutNE;
    }
    else if (anchorStr == "cutOutNW") {
        anchorId = SMUFL_cutOutNW;
    }
    else if (anchorStr == "cutOutSE") {
        anchorId = SMUFL_cutOutSE;
    }
    else if (anchorStr == "cutOutSW") {
        anchorId = SMUFL_cutOutSW;
    }
    // Silently ignore unused anchors
    else {
        return;
    }
    // Anchor points are given as staff spaces (upm / 4)
    m_anchors[anchorId] = Point(x * this->GetUnitsPerEm() / 4, y * this->GetUnitsPerEm() / 4);
}

bool Glyph::HasAnchor(SMuFLGlyphAnchor anchor) const
{
    return (m_anchors.count(anchor) == 1);
}

const Point *Glyph::GetAnchor(SMuFLGlyphAnchor anchor) const
{
    return &m_anchors.at(anchor);
}

} // namespace vrv

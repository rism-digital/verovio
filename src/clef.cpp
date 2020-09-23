/////////////////////////////////////////////////////////////////////////////
// Name:        clef.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "clef.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "doc.h"
#include "functorparams.h"
#include "scoredefinterface.h"
#include "staff.h"

namespace vrv {

//----------------------------------------------------------------------------
// Clef
//----------------------------------------------------------------------------

Clef::Clef() : LayerElement("clef-"), AttClefShape(), AttColor(), AttLineLoc(), AttOctaveDisplacement()
{
    RegisterAttClass(ATT_CLEFSHAPE);
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_LINELOC);
    RegisterAttClass(ATT_OCTAVEDISPLACEMENT);

    Reset();
}

Clef::~Clef() {}

void Clef::Reset()
{
    LayerElement::Reset();
    ResetClefShape();
    ResetColor();
    ResetLineLoc();
    ResetOctaveDisplacement();
}

int Clef::GetClefLocOffset() const
{
    if (this->HasSameasLink() && this->GetSameasLink()->Is(CLEF)) {
        Clef *sameas = vrv_cast<Clef *>(this->GetSameasLink());
        assert(sameas);
        return sameas->GetClefLocOffset();
    }

    int offset = 0;
    if (GetShape() == CLEFSHAPE_G)
        offset = -4;
    else if (GetShape() == CLEFSHAPE_F)
        offset = 4;

    offset += (GetLine() - 1) * 2;

    int disPlace = 0;
    if (GetDisPlace() == STAFFREL_basic_above)
        disPlace = -1;
    else if (GetDisPlace() == STAFFREL_basic_below)
        disPlace = 1;

    if ((disPlace != 0) && (GetDis() != OCTAVE_DIS_NONE)) offset += (disPlace * (GetDis() - 1));

    return offset;
}

int Clef::ClefId(data_CLEFSHAPE shape, char line, data_OCTAVE_DIS octaveDis, data_STAFFREL_basic place)
{
    return place << 24 | octaveDis << 16 | line << 8 | shape;
}

//----------------------------------------------------------------------------
// Clef functors methods
//----------------------------------------------------------------------------

int Clef::AdjustBeams(FunctorParams* functorParams)
{
    const std::map<data_CLEFSHAPE, std::pair<wchar_t, double> > topToMiddleProportions
        = { { CLEFSHAPE_G, { SMUFL_E050_gClef, 0.6 } }, 
            { CLEFSHAPE_C, { SMUFL_E05C_cClef, 0.5 } },
            { CLEFSHAPE_F, { SMUFL_E062_fClef, 0.35 } }
    };

    AdjustBeamParams *params = vrv_params_cast<AdjustBeamParams *>(functorParams);
    assert(params);

    Staff *staff = vrv_cast<Staff *>(GetFirstAncestor(STAFF));
    assert(staff);

    auto currentShapeIter = topToMiddleProportions.find(GetShape());
    if (currentShapeIter == topToMiddleProportions.end()) return FUNCTOR_CONTINUE;

    // Y position differes for clef shapes, so we need to take into account only a part of the glyph height
    // Proportion of the glyph about Y point is defined in the topToMiddleProportions map and used when
    // left and right margins are calculated
    const int clefPosition = staff->GetDrawingY()
        - params->m_doc->GetDrawingDoubleUnit(staff->m_drawingStaffSize) * (staff->m_drawingLines - GetLine());
    const int clefGlyphHeight
        = params->m_doc->GetGlyphHeight(currentShapeIter->second.first, staff->m_drawingStaffSize, false);
    const int leftMargin = clefPosition + clefGlyphHeight * currentShapeIter->second.second - params->m_y1;
    const int rightMargin = clefPosition + clefGlyphHeight * currentShapeIter->second.second - params->m_y2;

    const int overlapMargin = std::max(leftMargin, rightMargin);
    if (overlapMargin >= params->m_overlapMargin) {
        const int staffOffset = params->m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        params->m_overlapMargin = (overlapMargin/staffOffset + 1) * staffOffset;
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

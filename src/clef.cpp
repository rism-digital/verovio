/////////////////////////////////////////////////////////////////////////////
// Name:        clef.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "clef.h"
#include "scoredefinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Clef
//----------------------------------------------------------------------------


Clef::Clef() : LayerElement("clef-"), AttClefShape(), AttColor(), AttLabelled(), AttLineLoc(), AttOctaveDisplacement()
{
    Init();
}

Clef::Clef(const ScoreDefInterface *clefAttr) : LayerElement("clef-")
{
    Init();

    this->SetShape(clefAttr->GetClefShape());
    this->SetLine(clefAttr->GetClefLine());
    this->SetDis(clefAttr->GetClefDis());
    this->SetDisPlace(clefAttr->GetClefDisPlace());
    this->SetColor(clefAttr->GetClefColor());
}

void Clef::Init()
{
    RegisterAttClass(ATT_CLEFSHAPE);
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_LABELLED);
    RegisterAttClass(ATT_LINELOC);
    RegisterAttClass(ATT_OCTAVEDISPLACEMENT);

    Reset();
}

Clef::~Clef()
{
}

void Clef::Reset()
{
    LayerElement::Reset();
    ResetClefShape();
    ResetColor();
    ResetLabelled();
    ResetLineLoc();
    ResetOctaveDisplacement();
}

int Clef::GetClefLocOffset() const
{
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

} // namespace vrv

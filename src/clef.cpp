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

Clef::Clef():
    LayerElement("clef-"),
    AttClefshape(),
    AttLineloc(),
    AttOctavedisplacement()
{
    Init();
}
    
Clef::Clef( ScoreDefInterface *clefAttr ):
    LayerElement("clef-")
{
    Init();
    
    this->SetShape(clefAttr->GetClefShape());
    this->SetLine(clefAttr->GetClefLine());
    this->SetDis(clefAttr->GetClefDis());
    this->SetDisPlace(clefAttr->GetClefDisPlace());
}
    
void Clef::Init()
{
    RegisterAttClass(ATT_CLEFSHAPE);
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
    ResetClefshape();
    ResetLineloc();
    ResetOctavedisplacement();
}
    
int Clef::GetClefId()
{
    return GetDisPlace() << 24 | GetDis() << 16 | GetShape() << 8 | GetLine();
}

int Clef::GetClefOffset ()
{
    int clefCode = GetClefId();
    int offset = 0;		// case 5: UT 1e ligne par default, valable pour PERC
    switch(clefCode)
    {
        case G1 : offset = -4; break;
        case G2 : offset = -2; break;
        case G2_8va : offset = -9; break;
        case G2_8vb : offset = 5; break;
        case F3 : offset = 8; break;
        case F4 : offset = 10; break;
        case F5 : offset = 12; break;
        case C1 : offset = 0; break;
        case C2 : offset = 2; break;
        case C3 : offset = 4; break;
        case C4 : offset = 6; break;
        case C5 : offset = 8;
        default: break;
    }
    return offset;
}

} // namespace vrv

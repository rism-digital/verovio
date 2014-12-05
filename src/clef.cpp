/////////////////////////////////////////////////////////////////////////////
// Name:        clef.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "clef.h"

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
    Reset();
}
    
Clef::Clef( ClefAttr *clefAttr ):
    LayerElement("clef-")
{
    Clef();
    this->SetShape(clefAttr->GetClefShape());
    this->SetLine(clefAttr->GetClefLine());
    this->SetDis(clefAttr->GetClefDis());
    this->SetDisPlace(clefAttr->GetClefDisPlace());
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

bool Clef::operator==( Object& other )
{
    Clef *otherClef = dynamic_cast<Clef*>( &other );
    if ( !otherClef ) {
        return false;
    }
    /*
    if ( this->m_clefId != otherClef->m_clefId ) {
        return false;
    }
    */
    return true;
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
    

//----------------------------------------------------------------------------
// ClefAttr
//----------------------------------------------------------------------------

ClefAttr::ClefAttr():
    Object(),
    AttCleffingLog()
{
    Reset();
}


ClefAttr::~ClefAttr()
{
}
    
void ClefAttr::Reset()
{
    Object::Reset();
    ResetCleffingLog();
}

bool ClefAttr::operator==( Object& other )
{
    ClefAttr *otherClefAttr = dynamic_cast<ClefAttr*>( &other );
    if ( !otherClefAttr ) {
        return false;
    }
    /*
    if ( this->m_clefId != otherClef->m_clefId ) {
        return false;
    }
    */
    return true;
}

} // namespace vrv

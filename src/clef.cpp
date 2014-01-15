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
	LayerElement("clef-")
{
    m_clefId = UT1;
}


Clef::~Clef()
{
}

int Clef::GetClefOffset ()
{	
	int offset = 0;		// case 5: UT 1e ligne par default, valable pour PERC
	switch(m_clefId)
	{	case SOL2 : offset = -2; break;
		case SOL1 : offset = -4; break;
		case SOLva : offset = 5; break;
		case FA5 : offset = 12; break;
		case FA4 : offset = 10; break;
		case FA3 : offset = 8; break;
		case UT2 : offset = 2; break;
		case UT3 : offset = 4; break;
		case UT5 : offset = 8; break;
		case UT4 : offset =  6;		
		default: break;
	}
	return offset;
}

bool Clef::operator==( Object& other )
{
    Clef *otherClef = dynamic_cast<Clef*>( &other );
    if ( !otherClef ) {
        return false;
    }
    if ( this->m_clefId != otherClef->m_clefId ) {
        return false;
    }
    return true;
}

void Clef::SetValue( int value, int flag ) 
{
   switch ( value )
   {
       case ('1'): this->m_clefId = SOL1; break;
       case ('2'): this->m_clefId = SOL2; break;
       case ('3'): this->m_clefId = UT1; break;
       case ('4'): this->m_clefId = UT2; break;
       case ('5'): this->m_clefId = UT3; break;
       case ('6'): this->m_clefId = UT4; break;
       case ('7'): this->m_clefId = UT5; break;
       case ('8'): this->m_clefId = FA3; break;
       case ('9'): this->m_clefId = FA4; break;
       case ('0'): this->m_clefId = FA5; break;
   }
}
    
} // namespace vrv

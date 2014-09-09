/////////////////////////////////////////////////////////////////////////////
// Name:        slur.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "slur.h"

//----------------------------------------------------------------------------

#include <assert.h>

namespace vrv {

//----------------------------------------------------------------------------
// Slur
//----------------------------------------------------------------------------

Slur::Slur():
    LayerElement("slur-")
{
    Reset();
}


Slur::~Slur()
{
    if (m_first && m_first->GetSlurAttrTerminal() == this) {
        m_first->ResetSlurAttrInitial();
    }
    if (m_second && m_second->GetSlurAttrTerminal() == this) {
        m_second->ResetSlurAttrTerminal();
    }
}
        
void Slur::Reset()
{
    LayerElement::Reset();
    m_first = NULL;
    m_second = NULL;
}

void Slur::SetFirstNote( Note *note )
{
    assert( !m_first );
    m_first = note;
}

void Slur::SetSecondNote( Note *note )
{
    assert( !m_second );
    m_second = note;
}

} // namespace vrv
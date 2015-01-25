/////////////////////////////////////////////////////////////////////////////
// Name:        tie.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "tie.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "note.h"

namespace vrv {
    
//----------------------------------------------------------------------------
// Tie
//----------------------------------------------------------------------------

Tie::Tie():
LayerElement("tie-")
{
    Reset();
}


Tie::~Tie()
{
    if (m_first && m_first->GetTieAttrTerminal() == this) {
        m_first->ResetTieAttrInitial();
    }
    if (m_second && m_second->GetTieAttrTerminal() == this) {
        m_second->ResetTieAttrTerminal();
    }
}
    
void Tie::Reset()
{
    LayerElement::Reset();
    m_first = NULL;
    m_second = NULL;
}

void Tie::SetStart( Note *note )
{
    assert( !m_first );
    m_first = note;
}

void Tie::SetEnd( Note *note )
{
    assert( !m_second );
    m_second = note;
}

} // namespace vrv
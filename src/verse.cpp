/////////////////////////////////////////////////////////////////////////////
// Name:        verse.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "verse.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <iostream>

//----------------------------------------------------------------------------

#include "aligner.h"
#include "layer.h"
#include "staff.h"
#include "syl.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Verse
//----------------------------------------------------------------------------

Verse::Verse():
    DocObject("verse-")
{
}

Verse::~Verse()
{
}

void Verse::AddSyl(Syl *syl) {
    
    syl->SetParent( this );
    m_children.push_back(syl);
    Modify();
}

int Verse::AlignVertically( ArrayPtrVoid params )
{
    // param 0: the systemAligner
    // param 1: the staffNb
    SystemAligner **systemAligner = static_cast<SystemAligner**>(params[0]);
    int *staffNb = static_cast<int*>(params[1]);
    
    // we need to call it because we are overriding Object::AlignVertically
    this->ResetVerticalAlignment();
    
    // this gets (or creates) the measureAligner for the measure
    StaffAlignment *alignment = (*systemAligner)->GetStaffAlignment( *staffNb );
    
    assert( alignment );
    
    // Add the number count
    alignment->SetVerseCount(this->GetN());
    
    return FUNCTOR_CONTINUE;
}

    
int Verse::PrepareDrawing( ArrayPtrVoid params )
{
    // param 0: the IntTree
    IntTree *tree = static_cast<IntTree*>(params[0]);
    
    Staff *staff = dynamic_cast<Staff*>( this->GetFirstParent( &typeid( Staff ) ) );
    Layer *layer = dynamic_cast<Layer*>( this->GetFirstParent( &typeid( Layer ) ) );
    
    assert( staff && layer );
    
    tree->child[ staff->GetN() ].child[ layer->GetN() ].child[ this->GetN() ];
    
    return FUNCTOR_SIBLINGS;
}
    
int Verse::PrepareLyrics( ArrayPtrVoid params )
{
    
    Syl *syl = dynamic_cast<Syl*>( this->GetFirst( &typeid(Syl) ) );
    if (syl) {
        //std::cout << UTF16to8( syl->GetText().c_str() ) << std::endl;
    }
    return FUNCTOR_CONTINUE;
}
    
} // namespace vrv

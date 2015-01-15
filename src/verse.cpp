/////////////////////////////////////////////////////////////////////////////
// Name:        verse.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "verse.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "aligner.h"
#include "editorial.h"
#include "layer.h"
#include "staff.h"
#include "syl.h"

namespace vrv {

//----------------------------------------------------------------------------
// Verse
//----------------------------------------------------------------------------

Verse::Verse():
    LayerElement("verse-"),
    AttCommon()
{
    Reset();
}

Verse::~Verse()
{
}

void Verse::Reset()
{
    LayerElement::Reset();
    ResetCommon();
}

void Verse::AddElement(vrv::LayerElement *element)
{
    assert( dynamic_cast<Syl*>(element) || dynamic_cast<EditorialElement*>(element) );
    element->SetParent( this );
    m_children.push_back(element);
    Modify();
}
    
//----------------------------------------------------------------------------
// Verse functor methods
//----------------------------------------------------------------------------

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
    // Alternate solution with StaffN_LayerN_VerseN_t
    //StaffN_LayerN_VerseN_t *tree = static_cast<StaffN_LayerN_VerseN_t*>(params[0]);
    
    Staff *staff = dynamic_cast<Staff*>( this->GetFirstParent( &typeid( Staff ) ) );
    Layer *layer = dynamic_cast<Layer*>( this->GetFirstParent( &typeid( Layer ) ) );
    
    assert( staff && layer );
    
    tree->child[ staff->GetN() ].child[ layer->GetN() ].child[ this->GetN() ];
    // Alternate solution with StaffN_LayerN_VerseN_t
    //(*tree)[ staff->GetN() ][ layer->GetN() ][ this->GetN() ] = true;
    
    return FUNCTOR_SIBLINGS;
}
  
} // namespace vrv

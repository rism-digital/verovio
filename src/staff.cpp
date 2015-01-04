/////////////////////////////////////////////////////////////////////////////
// Name:        staff.cpp
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "staff.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "aligner.h"
#include "io.h"
#include "layer.h"
#include "system.h"
#include "verse.h"
#include "vrv.h"
#include "vrvdef.h"

namespace vrv {

//----------------------------------------------------------------------------
// Staff
//----------------------------------------------------------------------------

Staff::Staff( int n ):
	DocObject("staff-"),
    AttCommon()
{
    Reset();
    m_n = n;
}

Staff::~Staff()
{
    
}

void Staff::Reset()
{
    DocObject::Reset();
    ResetCommon();
    notAnc = false; // LP we want modern notation :))
    grise = false;
    invisible = false;
    staffSize = 0;
    m_drawingLines = 5;
    m_yAbs = VRV_UNSET;
    m_drawingY = 0;
    m_staffAlignment = NULL;
}

void Staff::AddLayer( Layer *layer )
{
	layer->SetParent( this );
	m_children.push_back( layer );
    
    if ( layer->GetN() < 1 ) {
        layer->SetN( this->GetLayerCount() );
    }
}

int Staff::GetVerticalSpacing()
{
    return 160; // arbitrary generic value
}
    
void Staff::ResetVerticalAlignment()
{
    m_drawingY = 0;
}

bool Staff::GetPosOnPage( ArrayPtrVoid params )
{
    // param 0: the Staff we are looking for
    // param 1: the position on the page (int)
    // param 2; the success flag (bool)
    Staff *staff = static_cast<Staff*>(params[0]);
	int *position = static_cast<int*>(params[1]);
    bool *success = static_cast<bool*>(params[2]);
    
    if ( (*success) ) {
        return true;
    } 
    (*position)++;
    if ( this == staff ) {
        (*success) = true;
        return true;
    }
    // to be verified
    return false;
}


int Staff::GetYRel()
{
    if (m_staffAlignment) {
        return m_staffAlignment->GetYRel();
    }
    return 0;
}

//----------------------------------------------------------------------------
// Staff functor methods
//----------------------------------------------------------------------------


int Staff::AlignVertically( ArrayPtrVoid params )
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
    
    // Set the pointer of the m_alignment
    m_staffAlignment = alignment;
    
    // for next staff
    (*staffNb)++;
    
    return FUNCTOR_CONTINUE;
}

} // namespace vrv

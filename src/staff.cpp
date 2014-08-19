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

#include "vrv.h"
#include "aligner.h"
#include "vrvdef.h"
#include "io.h"
#include "layer.h"
#include "system.h"

namespace vrv {

//----------------------------------------------------------------------------
// Staff
//----------------------------------------------------------------------------

Staff::Staff( int n ):
	DocObject("staff-")
{
	Clear( );
    //wxASSERT ( n > 0 );
    m_n = n;
}

Staff::Staff( const Staff& staff )
{
    m_parent = NULL;
	notAnc = staff.notAnc;
	grise = staff.grise;
	invisible = staff.invisible;
	staffSize = staff.staffSize;
	m_drawingLines = staff.m_drawingLines;
	m_yAbs = staff.m_yAbs;
	m_drawingY = staff.m_drawingY;
    m_staffAlignment = NULL;

    int i;
	for (i = 0; i < staff.GetLayerCount(); i++)
	{
        Layer *nlayer = new Layer( *dynamic_cast<Layer*>( staff.m_children[i] ) );
        this->AddLayer( nlayer );
	}
    
    this->ResetUuid();
}

Staff::~Staff()
{
    
}

void Staff::Clear()
{
	ClearChildren();
    m_parent = NULL;
	notAnc = false; // LP we want modern notation :))
	grise = false;
	invisible = false;
	staffSize = 0; 
	m_drawingLines = 5;
	m_yAbs = VRV_UNSET;
	m_drawingY = 0;
    m_staffAlignment = NULL;
}

int Staff::Save( ArrayPtrVoid params )
{
    // param 0: output stream
    FileOutputStream *output = static_cast<FileOutputStream*>(params[0]);           
    if (!output->WriteStaff( this )) {
        return FUNCTOR_STOP;
    }
    return FUNCTOR_CONTINUE;

}

void Staff::AddLayer( Layer *layer )
{
	layer->SetParent( this );
	m_children.push_back( layer );
    
    if ( layer->GetLayerNo() == -1 ) {
        layer->SetLayerNo( this->GetLayerCount() );
    }
}

void Staff::CopyAttributes( Staff *nstaff )
{
	if ( !nstaff )
		return;

	nstaff->Clear();
	nstaff->notAnc = notAnc;
	nstaff->grise = grise;
	nstaff->invisible = invisible;
	nstaff->staffSize = staffSize;
	nstaff->m_drawingLines = m_drawingLines;
	nstaff->m_yAbs = m_yAbs;
	nstaff->m_drawingY = m_drawingY;
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

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

#include "layer.h"
#include "note.h"
#include "syl.h"
#include "system.h"

namespace vrv {

//----------------------------------------------------------------------------
// Staff
//----------------------------------------------------------------------------

Staff::Staff( int n ):
	MeasureElement("staff-"),
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
    MeasureElement::Reset();
    ResetCommon();
    notAnc = false; // LP we want modern notation :))
    grise = false;
    invisible = false;
    staffSize = 0;
    m_drawingLines = 5;
    m_yAbs = VRV_UNSET;
    m_drawingY = 0;
    m_staffAlignment = NULL;
    m_currentSyls.clear();
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
    
int Staff::FillStaffCurrentLyrics( ArrayPtrVoid params )
{
    // param 0: the current Syl
    // param 1: the last Note
    Syl **currentSyl = static_cast<Syl**>(params[0]);

    if ((*currentSyl)) {
        // We have a running syl started in a previous measure
        this->m_currentSyls.push_back((*currentSyl));
        if ((*currentSyl)->m_drawingLastNote) {
            // Look if the syl ends in this measure - if yes, set it to NULL for the next measure
            if ((*currentSyl)->m_drawingLastNote->GetFirstParent( &typeid(Staff) ) == this ) {
                (*currentSyl) = NULL;
            }
        }
    }
    
    return FUNCTOR_CONTINUE;
}

int Staff::FillStaffCurrentLyricsEnd( ArrayPtrVoid params )
{
    // param 0: the current Syl
    // param 1: the last Note
    Syl **currentSyl = static_cast<Syl**>(params[0]);
    
    // Here we have a running syl started and ended in this measure but still running (happens with @con="u"
    // At the end of a word (not next syl for detecting the end of the syl)
    if ((*currentSyl) && (*currentSyl)->m_drawingLastNote) {
        // Look if the syl ends in this measure - if yes, set it to NULL for the next measure
        if ((*currentSyl)->m_drawingLastNote->GetFirstParent( &typeid(Staff) ) == this ) {
            (*currentSyl) = NULL;
        }
    }
    
    return FUNCTOR_CONTINUE;
}

} // namespace vrv

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

#include "doc.h"
#include "layer.h"
#include "note.h"
#include "syl.h"
#include "system.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Staff
//----------------------------------------------------------------------------

Staff::Staff( int n ):
	DocObject("staff-"),
    AttCommon()
{
    RegisterAttClass(ATT_COMMON);
    Reset();
    SetN(n);
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
    m_drawingStaffSize = 100;
    m_drawingLines = 5;
    m_yAbs = VRV_UNSET;
    m_drawingY = 0;
    m_staffAlignment = NULL;
    m_timeSpanningElements.clear();
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

bool Staff::GetPosOnPage( ArrayPtrVoid *params )
{
    // param 0: the Staff we are looking for
    // param 1: the position on the page (int)
    // param 2; the success flag (bool)
    Staff *staff = static_cast<Staff*>((*params).at(0));
	int *position = static_cast<int*>((*params).at(1));
    bool *success = static_cast<bool*>((*params).at(2));
    
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


int Staff::AlignVertically( ArrayPtrVoid *params )
{
    // param 0: the systemAligner
    // param 1: the staffNb
    SystemAligner **systemAligner = static_cast<SystemAligner**>((*params).at(0));
	int *staffNb = static_cast<int*>((*params).at(1));
    
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
        
int Staff::FillStaffCurrentTimeSpanning( ArrayPtrVoid *params )
{
    // param 0: the current Syl
    std::vector<DocObject*> *elements = static_cast<std::vector<DocObject*>*>((*params).at(0));
    
    std::vector<DocObject*>::iterator iter = elements->begin();
    while ( iter != elements->end()) {
        TimeSpanningInterface *interface = dynamic_cast<TimeSpanningInterface*>(*iter);
        assert(interface);
        Staff *endParent = dynamic_cast<Staff *>(interface->GetEnd()->GetFirstParent( STAFF ) );
        assert( endParent );
        // Because we are not processing following staff @n, we need to check it here.
        // this might cause problem with cross-staves slurs if the end is on a lower staff than the start:
        // this will be true for the staff below the start in the same measure - a fix would be to check if
        // we are still in the same measure (compare this->m_parent and start->m_parent)
        if ( endParent->GetN() == this->GetN() ) {
            m_timeSpanningElements.push_back(*iter);
        }
        // We have reached the end of the spanning - remove it from the list of running elements
        if ( endParent == this ) {
            iter = elements->erase( iter );
        }
        else {
            iter++;
        }
    }
    return FUNCTOR_CONTINUE;
}
    
int Staff::FillStaffCurrentLyrics( ArrayPtrVoid *params )
{
    // param 0: the current Syl
    // param 1: the last Note
    Syl **currentSyl = static_cast<Syl**>((*params).at(0));
    
    if ((*currentSyl)) {
        // We have a running syl started in a previous measure
        this->m_timeSpanningElements.push_back((*currentSyl));
        if ((*currentSyl)->GetEnd()) {
            // Look if the syl ends in this measure - if not, add it
            if ((*currentSyl)->GetEnd()->GetFirstParent( STAFF ) == this ) {
                (*currentSyl) = NULL;
            }
        }
    }
    
    return FUNCTOR_CONTINUE;
}
    
int Staff::ResetDrawing( ArrayPtrVoid *params )
{
    // Pass it to the pseudo functor of the interface
    this->m_timeSpanningElements.clear();
    return FUNCTOR_CONTINUE;
};
    
    
int Staff::SetDrawingXY( ArrayPtrVoid *params )
{
    // param 0: a pointer doc
    // param 1: a pointer to the current system
    // param 2: a pointer to the current measure (unused)
    // param 3: a pointer to the current staff
    // param 4: a pointer to the current layer (unused)
    // param 5: a pointer to the view (unused)
    // param 6: a bool indicating if we are processing layer elements or not
    Doc *doc = static_cast<Doc*>((*params).at(0));
    System **currentSystem = static_cast<System**>((*params).at(1));
    Staff **currentStaff = static_cast<Staff**>((*params).at(3));
    bool *processLayerElements = static_cast<bool*>((*params).at(6));
    
    (*currentStaff) = this;
    
    // Second pass where we do just process layer elements
    if ((*processLayerElements)) return FUNCTOR_CONTINUE;

    // Here we set the appropriate y value to be used for drawing
    // With Raw documents, we use m_drawingYRel that is calculated by the layout algorithm
    // With Transcription documents, we use the m_yAbs
    if ( this->m_yAbs == VRV_UNSET ) {
        assert( doc->GetType() == Raw );
        this->SetDrawingY( this->GetYRel() + (*currentSystem)->GetDrawingY() );
    }
    else
    {
        assert( doc->GetType() == Transcription );
        this->SetDrawingY( this->m_yAbs );
    }
    
    return FUNCTOR_CONTINUE;
}
    
int Staff::PrepareRpt( ArrayPtrVoid *params )
{
    // param 0: a pointer to the current MRpt pointer (unused)
    // param 1: a pointer to the data_BOOLEAN indicating if multiNumber
    // param 2: a pointer to the doc scoreDef
    data_BOOLEAN *multiNumber = static_cast<data_BOOLEAN*>((*params).at(1));
    ScoreDef *scoreDef = static_cast<ScoreDef*>((*params).at(2));
    
    // If multiNumber is set, we already know that nothing needs to be done
    // Futhermore, if @multi.number is false, the functor should have stopped (see below)
    if ((*multiNumber) != BOOLEAN_NONE) {
        return FUNCTOR_CONTINUE;
    }
    
    // This is happening only for the first staff element of the staff @n
    if (StaffDef *staffDef = scoreDef->GetStaffDef( this->GetN() ) ) {
        if ((staffDef->HasMultiNumber()) && (staffDef->GetMultiNumber() == BOOLEAN_false)) {
            // Set it just in case, but stopping the functor should do it for this staff @n
            (*multiNumber) = BOOLEAN_false;
            return FUNCTOR_STOP;
        }
    }
    (*multiNumber) = BOOLEAN_true;
    return FUNCTOR_CONTINUE;
}

} // namespace vrv

/////////////////////////////////////////////////////////////////////////////
// Name:        system.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "system.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "doc.h"
#include "measure.h"
#include "page.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// System
//----------------------------------------------------------------------------

System::System() :
	DocObject("system-"), DrawingListInterface()
{
    // We set parent to it because we want to access the parent doc from the aligners
    // See Object::SetParentDoc()
    m_systemAligner.SetParent( this );
    Reset();
}

System::~System()
{
}
    
void System::Reset()
{
    DocObject::Reset();
    DrawingListInterface::Reset();
    
    m_systemLeftMar = 0;
    m_systemRightMar = 0;
    m_xAbs = VRV_UNSET;
    m_drawingXRel = 0;
    m_drawingX = 0;
    m_yAbs = VRV_UNSET;
    m_drawingYRel = 0;
    m_drawingY = 0;
    m_drawingTotalWidth = 0;
}

void System::AddMeasure( Measure *measure )
{
	measure->SetParent( this );
	m_children.push_back( measure );
    Modify();
}

void System::AddScoreDef( ScoreDef *scoreDef )
{
    scoreDef->SetParent( this );
    m_children.push_back( scoreDef );
    Modify();
}

int System::GetVerticalSpacing()
{
    return 0; // arbitrary generic value
}

void System::ResetHorizontalAlignment()
{
    m_drawingXRel = 0;
	m_drawingX = 0;
    m_drawingLabelsWidth = 0;
}

    
void System::ResetVerticalAlignment()
{
    m_drawingYRel = 0;
    m_drawingY = 0;
}

Measure *System::GetAtPos( int x )
{
	Measure *measure = dynamic_cast<Measure*>( this->GetFirst( &typeid(Measure) ) );
	if ( !measure )
		return NULL;
    
    Measure *next = NULL;
	while ( (next = dynamic_cast<Measure*>( this->GetNext() ) ) )
	{
		if ( (int)measure->GetDrawingX() < x )
		{
			return measure;
		}
		measure = next;
	}

	return measure;
}
    
int System::GetHeight()
{
    if ( m_systemAligner.GetBottomAlignment() ) {
        return -m_systemAligner.GetBottomAlignment()->GetYRel();
    }
    return 0;
}
    
void System::SetDrawingLabelsWidth( int width )
{
    if ( m_drawingLabelsWidth < width ) {
        m_drawingLabelsWidth = width;
    }
}

//----------------------------------------------------------------------------
// System functor methods
//----------------------------------------------------------------------------

int System::AlignVertically( ArrayPtrVoid params )
{
    // param 0: the systemAligner
    // param 1: the staffNb (unused)
    SystemAligner **systemAligner = static_cast<SystemAligner**>(params[0]);
    
    // we need to call it because we are overriding Object::AlignVertically
    this->ResetVerticalAlignment();
    
    // When calculating the alignment, the position has to be 0
    m_drawingYRel = 0;
    m_systemAligner.Reset();
    (*systemAligner) = &m_systemAligner;
    
    return FUNCTOR_CONTINUE;
}


int System::SetAligmentYPos( ArrayPtrVoid params )
{
    // param 0: the previous staff height
    // param 1: the staff margin (unused)
    // param 2: the staff interline sizes (int[2]) (unused)
    // param 2: the functor to be redirected to SystemAligner
    int *previousStaffHeight = static_cast<int*>(params[0]);
    Functor *setAligmnentPosY = static_cast<Functor*>(params[3]);
    
    (*previousStaffHeight) = 0;
    
    m_systemAligner.Process( setAligmnentPosY, params);
    
    return FUNCTOR_SIBLINGS;
}


int System::IntegrateBoundingBoxYShift( ArrayPtrVoid params )
{
    // param 0: the cumulated shift
    // param 1: the functor to be redirected to SystemAligner
    int *shift = static_cast<int*>(params[0]);
    Functor *integrateBoundingBoxYShift = static_cast<Functor*>(params[1]);
    
    (*shift) = 0;
    m_systemAligner.Process( integrateBoundingBoxYShift, params);
    
    return FUNCTOR_SIBLINGS;
}

int System::AlignMeasures( ArrayPtrVoid params )
{
    // param 0: the cumulated shift
    int *shift = static_cast<int*>(params[0]);
    
    m_drawingXRel = this->m_systemLeftMar + this->GetDrawingLabelsWidth();
    (*shift) = 0;
    
    return FUNCTOR_CONTINUE;
}

int System::AlignMeasuresEnd( ArrayPtrVoid params )
{
    // param 0: the cumulated shift
    int *shift = static_cast<int*>(params[0]);
    
    m_drawingTotalWidth = (*shift) + this->GetDrawingLabelsWidth();
    
    return FUNCTOR_CONTINUE;
}

int System::AlignSystems( ArrayPtrVoid params )
{
    // param 0: the cumulated shift
    // param 1: the system margin
    int *shift = static_cast<int*>(params[0]);
    int *systemMargin = static_cast<int*>(params[1]);
    
    this->m_drawingYRel = (*shift);
    
    assert( m_systemAligner.GetBottomAlignment() );
    
    (*shift) += m_systemAligner.GetBottomAlignment()->GetYRel() - (*systemMargin);
    
    return FUNCTOR_SIBLINGS;
}


int System::JustifyX( ArrayPtrVoid params )
{
    // param 0: the justification ratio
    // param 1: the justification ratio for the measure (depends on the margin) (unused)
    // param 2: the non justifiable margin (unused)
    // param 3: the system full width (without system margins)
    // param 4: the functor to be redirected to the MeasureAligner
    double *ratio = static_cast<double*>(params[0]);
    int *systemFullWidth = static_cast<int*>(params[3]);
    
    assert( m_parent );
    assert( m_parent->m_parent );
    
    (*ratio) = (double)((*systemFullWidth) - this->GetDrawingLabelsWidth() - this->m_systemLeftMar - this->m_systemRightMar) / ((double)m_drawingTotalWidth - this->GetDrawingLabelsWidth());
    
    if ((*ratio) < 0.8 ) {
        // Arbitrary value for avoiding over-compressed justification
        LogWarning("Justification stop because of a ratio smaller the 0.8");
        //return FUNCTOR_SIBLINGS;
    }
    
    // Check if we are on the last page and on the last system - do no justify it if ratio > 1.0
    if ( (m_parent->GetIdx() == m_parent->m_parent->GetChildCount() - 1)
        && (this->GetIdx() == m_parent->GetChildCount() - 1) ) {
        if ( (*ratio) > 1.0 ) {
            return FUNCTOR_STOP;
        }
    }
    
    return FUNCTOR_CONTINUE;
}
    
int System::SetBoundingBoxYShiftEnd( ArrayPtrVoid params )
{
    // param 0: the height of the previous staff
    int *system_height = static_cast<int*>(params[1]);
    
    m_systemAligner.GetBottomAlignment()->SetYShift( (*system_height) );

    return FUNCTOR_CONTINUE;
}

    
int System::CastOffPages( ArrayPtrVoid params )
{
    // param 0: a pointer to the page we are taking the content from
    // param 1: a pointer the document we are adding pages to
    // param 2: a pointer to the current page
    // param 3: the cummulated shift (m_drawingYRel of the first system of the current page)
    // param 4: the page height
    Page *contentPage = static_cast<Page*>(params[0]);
    Doc *doc = static_cast<Doc*>(params[1]);
    Page **currentPage = static_cast<Page**>(params[2]);
    int *shift = static_cast<int*>(params[3]);
    int *pageHeight = static_cast<int*>(params[4]);
    
    if ( ( (*currentPage)->GetChildCount() > 0 ) && ( this->m_drawingYRel - this->GetHeight() - (*shift) < 0 )) { //(*pageHeight) ) ) {
        (*currentPage) = new Page();
        doc->AddPage( *currentPage );
        (*shift) = this->m_drawingYRel - (*pageHeight);
    }
    
    // Special case where we use the Relinquish method.
    // We want to move the system to the currentPage. However, we cannot use DetachChild
    // from the contentPage because this screws up the iterator. Relinquish gives up
    // the ownership of the system - the contentPage itself will be deleted afterwards.
    System *system = dynamic_cast<System*>( contentPage->Relinquish( this->GetIdx()) );
    (*currentPage)->AddSystem( system );
    
    return FUNCTOR_SIBLINGS;
}

int System::UnCastOff( ArrayPtrVoid params )
{
    // param 0: a pointer to the system we are adding system to
    System *currentSystem = static_cast<System*>(params[0]);
    
    // Just move all the content of the system to the continous one (parameter)
    // Use the MoveChildren method that move the and relinquishes them
    // See Object::Relinquish
    currentSystem->MoveChildren( this );
    
    // No need to go deeper
    return FUNCTOR_SIBLINGS;
}
    
int System::SetDrawingXY( ArrayPtrVoid params )
{
    // param 0: a pointer doc
    // param 1: a pointer to the current system
    // param 2: a pointer to the current measure (unused)
    // param 3: a pointer to the current staff (unused)
    // param 4: a pointer to the current layer
    // param 5: a pointer to the view (unused)
    // param 6: a bool indicating if we are processing layer elements or not
    Doc *doc = static_cast<Doc*>(params[0]);
    System **currentSystem = static_cast<System**>(params[1]);
    bool *processLayerElements = static_cast<bool*>(params[6]);
    
    (*currentSystem) = this;
    
    // Second pass where we do just process layer elements
    if ((*processLayerElements)) return FUNCTOR_CONTINUE;
    
    // Here we set the appropriate y value to be used for drawing
    // With Raw documents, we use m_drawingYRel that is calculated by the layout algorithm
    // With Transcription documents, we use the m_yAbs
    if ( this->m_yAbs == VRV_UNSET ) {
        assert( doc->GetType() == Raw );
        this->SetDrawingX( this->m_drawingXRel );
        this->SetDrawingY( this->m_drawingYRel );
    }
    else
    {
        assert( doc->GetType() == Transcription );
        this->SetDrawingX( this->m_xAbs );
        this->SetDrawingY( this->m_yAbs );
    }
    
    return FUNCTOR_CONTINUE;
}
    
} // namespace vrv

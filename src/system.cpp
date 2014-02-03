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

#include "vrv.h"
#include "doc.h"
#include "io.h"
#include "measure.h"
#include "page.h"

namespace vrv {

//----------------------------------------------------------------------------
// System
//----------------------------------------------------------------------------

System::System() :
	DocObject("system-")
{
	Clear( );
}

System::System( const System& system )
{
    int i;

	m_systemLeftMar = system.m_systemLeftMar;
	m_systemRightMar = system.m_systemRightMar;
	m_xAbs = system.m_xAbs;
	m_drawingXRel = system.m_drawingXRel;
	m_drawingX = system.m_drawingX;
	m_yAbs = system.m_yAbs;
	m_drawingYRel = system.m_drawingYRel;
	m_drawingY = system.m_drawingY;
    
	for (i = 0; i < this->GetMeasureCount(); i++)
	{
        Measure *nmeasure = new Measure( *dynamic_cast<Measure*>( system.m_children[i] ) );
        this->AddMeasure( nmeasure );
	}
}

System::~System()
{
}

void System::Clear( )
{
	ClearChildren();
	m_systemLeftMar = 50;
	m_systemRightMar = 50;
	m_xAbs = VRV_UNSET;
    m_drawingXRel = 0;
	m_drawingX = 0;
	m_yAbs = VRV_UNSET;
    m_drawingYRel = 0;
	m_drawingY = 0;
    m_drawingTotalWidth = 0;
}


int System::Save( ArrayPtrVoid params )
{
    // param 0: output stream
    FileOutputStream *output = static_cast<FileOutputStream*>(params[0]);         
    if (!output->WriteSystem( this )) {
        return FUNCTOR_STOP;
    }
    return FUNCTOR_CONTINUE;

}

void System::AddMeasure( Measure *measure )
{
	measure->SetParent( this );
	m_children.push_back( measure );
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
}

    
void System::ResetVerticalAlignment()
{
    m_drawingYRel = 0;
    m_drawingY = 0;
}
    
Measure *System::GetFirst( )
{
	if ( m_children.empty() )
		return NULL;
	return dynamic_cast<Measure*>(m_children[0]);
}

Measure *System::GetLast( )
{
	if ( m_children.empty() )
		return NULL;
	int i = (int)m_children.size() - 1;
	return dynamic_cast<Measure*>(m_children[i]);
}

Measure *System::GetNext( Measure *measure )
{
    if ( !measure || m_children.empty())
        return NULL;
        
	int i = GetChildIndex( measure );
    
	if ((i == -1 ) || ( i >= GetMeasureCount() - 1 ))
		return NULL;
	
	return dynamic_cast<Measure*>(m_children[i + 1]);
	
}

Measure *System::GetPrevious( Measure *measure  )
{
    if ( !measure || m_children.empty() )
        return NULL;
        
	int i = GetChildIndex( measure );

	if ((i == -1 ) || ( i <= 0 ))
        return NULL;
	
    return dynamic_cast<Measure*>(m_children[i - 1]);
}


Measure *System::GetAtPos( int x )
{
	//y += ( STAFF_OFFSET / 2 );
	Measure *measure = this->GetFirst();
	if ( !measure )
		return NULL;
	
    
    Measure *next = NULL;
	while ( (next = this->GetNext(measure) ) )
	{
		if ( (int)measure->m_drawingX < x )
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
    m_drawingXRel = 0;
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
    MusFunctor *setAligmnentPosY = static_cast<MusFunctor*>(params[3]);
    
    (*previousStaffHeight) = 0;
    
    m_systemAligner.Process( setAligmnentPosY, params);
    
    return FUNCTOR_SIBLINGS;
}


int System::IntegrateBoundingBoxYShift( ArrayPtrVoid params )
{
    // param 0: the cumulated shift
    // param 1: the functor to be redirected to SystemAligner
    int *shift = static_cast<int*>(params[0]);
    MusFunctor *integrateBoundingBoxYShift = static_cast<MusFunctor*>(params[1]);
    
    m_drawingXRel = this->m_systemLeftMar;
    (*shift) = 0;
    m_systemAligner.Process( integrateBoundingBoxYShift, params);
    
    return FUNCTOR_SIBLINGS;
}

int System::AlignMeasures( ArrayPtrVoid params )
{
    // param 0: the cumulated shift
    int *shift = static_cast<int*>(params[0]);
    
    (*shift) = 0;
    
    return FUNCTOR_CONTINUE;
}

int System::AlignMeasuresEnd( ArrayPtrVoid params )
{
    // param 0: the cumulated shift
    int *shift = static_cast<int*>(params[0]);
    
    m_drawingTotalWidth = (*shift);
    
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
    // param 0: the justification ratio (unused)
    // param 1: the system full width (without system margins)
    // param 2: the functor to be redirected to the MeasureAligner (unused)
    double *ratio = static_cast<double*>(params[0]);
    int *systemFullWidth = static_cast<int*>(params[1]);
    
    assert( m_parent );
    assert( m_parent->m_parent );
    
    (*ratio) = (double)((*systemFullWidth) - this->m_systemLeftMar - this->m_systemRightMar) / (double)m_drawingTotalWidth;
    
    if ((*ratio) < 0.8 ) {
        // Arbitrary value for avoiding over-compressed justification
        LogWarning("Justification stop because of a ratio smaller the 0.8");
        return FUNCTOR_SIBLINGS;
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

} // namespace vrv

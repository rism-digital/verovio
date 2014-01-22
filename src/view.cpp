/////////////////////////////////////////////////////////////////////////////
// Name:        view.cpp
// Author:      Laurent Pugin
// Created:     2010
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "view.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <typeinfo>

//----------------------------------------------------------------------------

#include "doc.h"
#include "layerelement.h"
#include "note.h"
#include "page.h"

namespace vrv {

//----------------------------------------------------------------------------
// View
//----------------------------------------------------------------------------

View::View( )
{
	m_doc = NULL;
	m_pageIdx = 0;

	m_currentColour = AxBLACK;
	m_currentElement = NULL;
    m_currentLayer = NULL;
    m_currentMeasure = NULL;
	m_currentStaff = NULL;
    m_currentSystem = NULL;

    m_editorMode = EDITOR_EDIT;
	
	m_notationMode = MENSURAL_MODE;
}


View::~View()
{
}

void View::SetDoc( Doc *doc )
{
	if ( doc == NULL ) // unset file
	{
		m_doc = NULL;
        DoReset();
	}
    else {
        m_doc = doc;
    }
    m_currentElement = NULL;
    m_currentLayer = NULL;
    m_currentMeasure = NULL;
    m_currentStaff = NULL;
    m_currentSystem = NULL;
    m_currentPage = NULL;
    m_pageIdx = 0;
}


void View::SetPage( int pageIdx, bool doLayout )
{

	assert( m_doc ); // Page cannot be NULL
    assert( m_doc->HasPage( pageIdx ) );
    
    m_pageIdx = pageIdx;
    m_currentPage = m_doc->SetDrawingPage( pageIdx );
    
    if (doLayout) {
        m_doc->SetCurrentScoreDef();
        // if we once deal with multiple views, it would be better
        // to redo the layout only when necessary?
        m_currentPage->Layout( );
    }

    m_currentElement = NULL;
    m_currentLayer = NULL;
    m_currentMeasure = NULL;
    m_currentStaff = NULL;
    m_currentSystem = NULL;
    
    
    /*
	// selectionne le premier element
	if ( m_page->GetSystemCount() > 0 ) 
    {
		m_currentSystem = m_page->GetFirst();
    	if ( m_currentSystem->GetStaffCount() > 0 ) 
        {
            m_currentStaff = m_currentSystem->GetFirst();
            if (m_currentStaff->GetLayerCount() > 0 ) 
            {
                m_currentLayer = m_currentStaff->GetFirst();
                if ( m_currentLayer->GetElementCount() > 0 ) 
                {
                    m_currentElement = m_currentLayer->GetFirst();
                }
            }
        }
	}
    */

    OnPageChange();
    DoRefresh();
}

bool View::HasNext( bool forward ) 
{ 
	if ( forward )
		return ( m_doc && ( m_doc->HasPage( m_pageIdx + 1 ) ) );
	else
		return ( m_doc && ( m_doc->HasPage( m_pageIdx - 1 ) ) );
    return false;
		
}

void View::Next( bool forward ) 
{ 
	if ( !m_doc )
        return;

	if ( forward && this->HasNext( true ) )
		m_pageIdx++;
	else if ( !forward && this->HasNext( false ) )
		m_pageIdx--;

	SetPage( m_pageIdx );
}

int View::ToDeviceContextX( int i ) { return i; }; // the same

/** x value in the Logical world */
int View::ToLogicalX( int i )  { return i; };

/** y value in the View */
int View::ToDeviceContextY( int i )  
{ 
    if (!m_doc) {
        return 0;
    }
    
    return m_doc->m_drawingPageHeight - i; // flipped
}

/** y value in the Logical world  */
int View::ToLogicalY( int i )  
{ 
    { 
        if (!m_doc) {
            return 0;
        }
        
        return m_doc->m_drawingPageHeight - i; // flipped
    }
}
    
void View::SwapPoints (MusPoint *x1, MusPoint *x2)
{
    MusPoint a;
    a = *x1;
    *x1 = *x2;
    *x2 = a;
}

} // namespace vrv

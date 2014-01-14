/////////////////////////////////////////////////////////////////////////////
// Name:        musrc.cpp
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

namespace vrv {

//----------------------------------------------------------------------------
// View
//----------------------------------------------------------------------------

View::View( )
{
	m_doc = NULL;
    m_page = NULL;
	m_npage = 0;

	m_currentColour = AxBLACK;
	m_currentElement = NULL;
    m_currentLayer = NULL;
    m_currentMeasure = NULL;
	m_currentStaff = NULL;
    m_currentSystem = NULL;

	m_lyricMode = false;
	m_inputLyric = false;
    m_editorMode = MUS_EDITOR_EDIT;
	//m_editorMode = MUS_EDITOR_INSERT;
	
	m_notation_mode = MUS_MENSURAL_MODE;
    //m_notation_mode = MUS_CMN_MODE;
	
	m_str.reserve(1000);
}


View::~View()
{
}

void View::SetDoc( Doc *doc )
{
	if ( doc == NULL ) // unset file
	{
		m_doc = NULL;
		m_page = NULL;
        m_currentElement = NULL;
        m_currentLayer = NULL;
        m_currentMeasure = NULL;
        m_currentStaff = NULL;
        m_currentSystem = NULL;
        DoReset();
		return;
	}
    else {
        m_doc = doc;
        //m_notation_mode = m_layout->m_env.m_notationMode;
        m_npage = 0;
        // for now we just get the first page
        SetPage( (Page*)m_doc->m_children[m_npage] );
        //CheckPoint( UNDO_ALL, MUS_UNDO_FILE ); // ax2
    }
}


void View::SetPage( Page *page )
{
	assert( page ); // Page cannot be NULL
    
    m_doc->SetRendPage( page );

	m_page = page;

	m_currentElement = NULL;
    m_currentLayer = NULL;
    m_currentMeasure = NULL;
	m_currentStaff = NULL;
    
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
		return ( m_doc && ((int)m_doc->GetChildCount() - 1 > m_npage) );
	else
		return ( m_doc && (m_npage > 0) );
    return false;
		
}

void View::Next( bool forward ) 
{ 
	if ( !m_doc )
        return;

	if ( forward && this->HasNext( true ) )
		m_npage++;
	else if ( !forward && this->HasNext( false ) )
		m_npage--;

	SetPage( (Page*)m_doc->m_children[m_npage] );
}

void View::LoadPage( int nopage )
{
    /*
	if ( !m_doc )
		return;

	if ((nopage < 0) || (nopage > m_fh->nbpage - 1))
		return;

	m_npage = nopage;
	SetPage( &m_doc->m_pages[m_npage] );
    */
    LogDebug( "View::LoadPage missing in ax2" );
}

int View::ToRendererX( int i ) { return i; }; // the same

/** x value in the Logical world */
int View::ToLogicalX( int i )  { return i; };

/** y value in the Renderer */
int View::ToRendererY( int i )  
{ 
    if (!m_doc) {
        return 0;
    }
    
    return m_doc->m_rendPageHeight - i; // flipped
}

/** y value in the Logical world  */
int View::ToLogicalY( int i )  
{ 
    { 
        if (!m_doc) {
            return 0;
        }
        
        return m_doc->m_rendPageHeight - i; // flipped
    }
}

bool View::IsNoteSelected() 
{ 
	if (!m_currentElement) 
		return false;
	else
		return m_currentElement->IsNote() || m_currentElement->IsNeume();
}

} // namespace vrv

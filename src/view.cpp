/////////////////////////////////////////////////////////////////////////////
// Name:        view.cpp
// Author:      Laurent Pugin
// Created:     2010
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "view.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <sstream>

//----------------------------------------------------------------------------

#include "doc.h"
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
        m_currentPage->LayOut( );
    }

    m_currentElement = NULL;
    m_currentLayer = NULL;
    m_currentMeasure = NULL;
    m_currentStaff = NULL;
    m_currentSystem = NULL;

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
    
void View::SwapPoints (Point *x1, Point *x2)
{
    Point a;
    a = *x1;
    *x1 = *x2;
    *x2 = a;
}

std::wstring View::IntToTupletFigures(unsigned short number)
{
    return IntToSmuflFigures(number, 0xE880);
}

std::wstring View::IntToTimeSigFigures(unsigned short number)
{
    return IntToSmuflFigures(number, 0xE080);
}

std::wstring View::IntToSmuflFigures(unsigned short number, int offset)
{
    // We do not convert more that FF values
    if (number > 0xFFFF) number = 0xFFFF;
    
    std::wstringstream stream;
    stream << number;
    std::wstring str = stream.str();
    
    int i;
    for (i = 0; i < str.size(); i++) {
        str[i] += offset - 48;
    }
    return str;
}
    

} // namespace vrv

/////////////////////////////////////////////////////////////////////////////
// Name:        doc.cpp
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "doc.h"

//----------------------------------------------------------------------------

#include "vrv.h"
#include "layer.h"
#include "layerelement.h"
#include "page.h"
#include "staff.h"
#include "system.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------
// Doc
//----------------------------------------------------------------------------

namespace vrv {

Doc::Doc() :
    Object("doc-")
{
    Reset( Raw );
}

Doc::~Doc()
{
    
}

void Doc::Reset( DocType type )
{
    UpdateFontValues();
    
    m_type = type;
    
    ClearChildren();
    
    m_pageWidth = -1;
    m_pageHeight = -1;
    m_pageRightMar = 0;
    m_pageLeftMar = 0;
    m_pageTopMar = 0;
    
    m_drawingPage = NULL;
    m_currentScoreDefDone = false;
    
    m_scoreDef.Clear();
}

int Doc::Save( ArrayPtrVoid params )
{  
    // param 0: output stream
    FileOutputStream *output = static_cast<FileOutputStream*>(params[0]);         
    if (!output->WriteDoc( this )) {
        return FUNCTOR_STOP;
    }
    return FUNCTOR_CONTINUE;
}

void Doc::AddPage( Page *page )
{
	page->SetParent( this );
	m_children.push_back( page );
    Modify();
}

void Doc::Refresh()
{
    RefreshViews();
}
    
void Doc::SetCurrentScoreDef( bool force )
{
    if ( m_currentScoreDefDone && ! force ) {
        return;
    }
    
    ScoreDef currentScoreDef;
    currentScoreDef = m_scoreDef;
    StaffDef *staffDef = NULL;
    ArrayPtrVoid params;
    params.push_back( &currentScoreDef );
    params.push_back( &staffDef );
    MusFunctor SetCurrentScoreDef( &Object::SetCurrentScoreDef );
    this->Process( &SetCurrentScoreDef, params );
    
    m_currentScoreDefDone = true;
}

void Doc::Layout( )
{
    this->SetCurrentScoreDef();
    
    Page *contentPage = this->SetDrawingPage( 0 );
    assert( contentPage );
    contentPage->LayOutHorizontally();
    
    System *contentSystem = dynamic_cast<System*>(contentPage->DetachChild( 0 ));
    assert( contentSystem );
    
    System *currentSystem = new System();
    contentPage->AddSystem( currentSystem );
    int shift = 0;
    int systemFullWidth = this->m_drawingPageWidth - this->m_drawingPageLeftMar - this->m_drawingPageRightMar
        - currentSystem->m_systemLeftMar - currentSystem->m_systemRightMar;
    ArrayPtrVoid params;
    params.push_back( contentSystem );
    params.push_back( contentPage );
    params.push_back( &currentSystem );
    params.push_back( &shift );
    params.push_back( &systemFullWidth );
    MusFunctor castOffSystems( &Object::CastOffSystems );
    contentSystem->Process( &castOffSystems, params );
    delete contentSystem;
    
    LogDebug("Layout: %d systems", contentPage->GetSystemCount());
    
    // Reset the scoreDef at the beginning of each system
    this->SetCurrentScoreDef( true );
    contentPage->LayOutVertically( );
    
    // Detach the contentPage
    this->DetachChild( 0 );
    assert( contentPage && !contentPage->m_parent );
    
    Page *currentPage = new Page();
    this->AddPage( currentPage );
    shift = 0;
    int pageFullHeight = this->m_drawingPageHeight - this->m_drawingPageTopMar; // obviously we need a bottom margin
    params.clear();
    params.push_back( contentPage );
    params.push_back( this );
    params.push_back( &currentPage );
    params.push_back( &shift );
    params.push_back( &pageFullHeight );
    MusFunctor castOffPages( &Object::CastOffPages );
    contentPage->Process( &castOffPages, params );
    delete contentPage;
    
    LogDebug("Layout: %d pages", this->GetChildCount());

    // We need to reset the drawing page to NULL
    // because idx will still be 0 but contentPage is dead!
    this->ResetDrawingPage( );
    this->SetCurrentScoreDef( true );
}
    
bool Doc::HasPage( int pageIdx )
{
    return ( (pageIdx >= 0 ) && (pageIdx < GetChildCount() ) );
}

Page *Doc::SetDrawingPage( int pageIdx )
{
    // out of range 
    if ( !HasPage( pageIdx ) ) {
        return NULL;
    }
    // nothing to do
    if ( m_drawingPage && m_drawingPage->GetIdx() == pageIdx ) {
        return m_drawingPage;
    }
    m_drawingPage = dynamic_cast<Page*>(this->GetChild( pageIdx ) );
    
    assert( m_drawingPage );
    
    // we use the page members only if set (!= -1) 
    if ( m_drawingPage->m_pageHeight != -1 )
    {
        m_drawingPageHeight = m_drawingPage->m_pageHeight;
        m_drawingPageWidth = m_drawingPage->m_pageWidth;
        m_drawingPageLeftMar = m_drawingPage->m_pageLeftMar;
        m_drawingPageRightMar = m_drawingPage->m_pageRightMar;
        m_drawingPageTopMar = m_drawingPage->m_pageTopMar;
	}
	else if ( this->m_pageHeight != -1 )
	{
        m_drawingPageHeight = this->m_pageHeight;
        m_drawingPageWidth = this->m_pageWidth;
        m_drawingPageLeftMar = this->m_pageLeftMar;
        m_drawingPageRightMar = this->m_pageRightMar;
        m_drawingPageTopMar = this->m_pageTopMar;
    }
    else
    {
        m_drawingPageHeight = m_env.m_pageHeight;
        m_drawingPageWidth = m_env.m_pageWidth;
        m_drawingPageLeftMar = m_env.m_pageLeftMar;
        m_drawingPageRightMar = m_env.m_pageRightMar;
        m_drawingPageTopMar = m_env.m_pageTopMar;
    }
    
    if (this->m_env.m_landscape)
    {	
        int pageHeight = m_drawingPageWidth;
        m_drawingPageWidth = m_drawingPageHeight;
        m_drawingPageHeight = pageHeight;
        int pageRightMar = m_drawingPageLeftMar;
        m_drawingPageLeftMar = m_drawingPageRightMar;
        m_drawingPageRightMar = pageRightMar;
    }
    
    // From here we could check if values have changed
    // Since  m_env.m_interlDefin stays the same, it useless to do it
    // every time for now.
    
	m_drawingBeamMaxSlope = this->m_env.m_beamMaxSlope;
	m_drawingBeamMinSlope = this->m_env.m_beamMinSlope;
	m_drawingBeamMaxSlope /= 100;
	m_drawingBeamMinSlope /= 100;
    
    m_drawingSmallStaffRatio[0] = this->m_env.m_smallStaffNum;
    m_drawingSmallStaffRatio[1] = this->m_env.m_smallStaffDen;
    m_drawingGraceRatio[0] = this->m_env.m_graceNum;
    m_drawingGraceRatio[1] = this->m_env.m_graceDen;
    
    // half of the space between two lines
    m_drawingHalfInterl[0] = m_env.m_interlDefin/2;
    // same for small staves
    m_drawingHalfInterl[1] = (m_drawingHalfInterl[0] * m_drawingSmallStaffRatio[0]) / m_drawingSmallStaffRatio[1];
    // space between two lines
    m_drawingInterl[0] = m_drawingHalfInterl[0] * 2;
    // same for small staves
    m_drawingInterl[1] = m_drawingHalfInterl[1] * 2;
    // staff (with five lines)
    m_drawingStaffSize[0] = m_drawingInterl[0] * 4;
    m_drawingStaffSize[1] = m_drawingInterl[1] * 4;
    //
    m_drawingOctaveSize[0] = m_drawingHalfInterl[0] * 7;
    m_drawingOctaveSize[1] = m_drawingHalfInterl[1] * 7;
    
    m_drawingStep1 = m_drawingHalfInterl[0];
    m_drawingStep2 = m_drawingStep1 * 3;
    m_drawingStep3 = m_drawingStep1 * 6;
    
    // values for beams
    m_drawingBeamWidth[0] = this->m_env.m_interlDefin / 2;
    m_drawingBeamWhiteWidth[0] = this->m_env.m_interlDefin / 4;
    m_drawingBeamWidth[1] = (m_drawingBeamWidth[0] * m_drawingSmallStaffRatio[0]) / m_drawingSmallStaffRatio[1];
    m_drawingBeamWhiteWidth[1] = (m_drawingBeamWhiteWidth[0] * m_drawingSmallStaffRatio[0]) / m_drawingSmallStaffRatio[1];
    
    m_drawingFontHeight = CalcLeipzigFontSize();
    m_drawingFontHeightAscent[0][0] = floor(LEIPZIG_ASCENT * (double)m_drawingFontHeight / LEIPZIG_UNITS_PER_EM);
	m_drawingFontHeightAscent[0][1] = (m_drawingFontHeightAscent[0][0] * m_drawingGraceRatio[0]) / m_drawingGraceRatio[1];
    m_drawingFontHeightAscent[1][0] = (m_drawingFontHeightAscent[0][0] * m_drawingSmallStaffRatio[0]) / m_drawingSmallStaffRatio[1];
	m_drawingFontHeightAscent[1][1] = (m_drawingFontHeightAscent[1][0] * m_drawingGraceRatio[0]) / m_drawingGraceRatio[1];
    
    m_drawingFontSize[0][0] = m_drawingFontHeight;
    m_drawingFontSize[0][1] = (m_drawingFontSize[0][0] * m_drawingGraceRatio[0]) / m_drawingGraceRatio[1];
    m_drawingFontSize[1][0] = (m_drawingFontSize[0][0] * m_drawingSmallStaffRatio[0]) / m_drawingSmallStaffRatio[1];
    m_drawingFontSize[1][1]= (m_drawingFontSize[1][0] * m_drawingGraceRatio[0])/ m_drawingGraceRatio[1];
    
	m_drawingFonts[0][0].SetPointSize( m_drawingFontSize[0][0] ); //160
    m_drawingFonts[0][1].SetPointSize( m_drawingFontSize[0][1] ); //120
    m_drawingFonts[1][0].SetPointSize( m_drawingFontSize[1][0] ); //128
    m_drawingFonts[1][1].SetPointSize( m_drawingFontSize[1][1] ); //100
    
	m_drawingLyricFonts[0].SetPointSize( m_drawingLyricFont.GetPointSize() );
    m_drawingLyricFonts[1].SetPointSize( m_drawingLyricFont.GetPointSize() );
    
    m_drawingVerticalUnit1[0] = (float)m_drawingInterl[0]/4;
    m_drawingVerticalUnit2[0] = (float)m_drawingInterl[0]/8;
    m_drawingVerticalUnit1[1] = (float)m_drawingInterl[1]/4;
    m_drawingVerticalUnit2[1] = (float)m_drawingInterl[1]/8;
    
    float glyph_size;
    glyph_size = (LEIPZIG_HALF_NOTE_HEAD_WIDTH * (float)m_drawingFontHeight / LEIPZIG_UNITS_PER_EM);
    m_drawingNoteRadius[0][0] = ceil(glyph_size / 2);
    m_drawingNoteRadius[0][1] = (m_drawingNoteRadius[0][0] * m_drawingGraceRatio[0])/m_drawingGraceRatio[1];
    m_drawingNoteRadius[1][0] = (m_drawingNoteRadius[0][0] * m_drawingSmallStaffRatio[0])/m_drawingSmallStaffRatio[1];
    m_drawingNoteRadius[1][1] = (m_drawingNoteRadius[1][0] * m_drawingGraceRatio[0])/m_drawingGraceRatio[1];
    
    m_drawingLedgerLine[0][0] = (int)(glyph_size * .72);
    m_drawingLedgerLine[0][1] = (m_drawingLedgerLine[0][0] * m_drawingGraceRatio[0])/m_drawingGraceRatio[1];
    m_drawingLedgerLine[1][0] = (m_drawingLedgerLine[0][0] * m_drawingSmallStaffRatio[0])/m_drawingSmallStaffRatio[1];
    m_drawingLedgerLine[1][1] = (m_drawingLedgerLine[1][0] * m_drawingGraceRatio[0])/m_drawingGraceRatio[1];
    
    glyph_size = round(LEIPZIG_WHOLE_NOTE_HEAD_WIDTH * (double)m_drawingFontHeight / LEIPZIG_UNITS_PER_EM);
    m_drawingLedgerLine[0][2] = (int)(glyph_size * .66);
    m_drawingLedgerLine[1][2] = (m_drawingLedgerLine[0][2] * m_drawingSmallStaffRatio[0]) /m_drawingSmallStaffRatio[1];
    
    m_drawingBrevisWidth[0] = (int)((glyph_size * 0.8) / 2);
    m_drawingBrevisWidth[1] = (m_drawingBrevisWidth[0] * m_drawingSmallStaffRatio[0]) /m_drawingSmallStaffRatio[1];
    
    glyph_size = round(LEIPZIG_SHARP_WIDTH * (double)m_drawingFontHeight / LEIPZIG_UNITS_PER_EM);
    m_drawingAccidWidth[0][0] = glyph_size;
    m_drawingAccidWidth[0][1] = (m_drawingAccidWidth[0][0] * m_drawingGraceRatio[0])/m_drawingGraceRatio[1];
    m_drawingAccidWidth[1][0] = (m_drawingAccidWidth[0][0] * m_drawingSmallStaffRatio[0]) /m_drawingSmallStaffRatio[1];
    m_drawingAccidWidth[1][1] = (m_drawingAccidWidth[1][0] * m_drawingGraceRatio[0])/m_drawingGraceRatio[1];
    
	return m_drawingPage;
}

int Doc::CalcLeipzigFontSize( )
{
    // We just have the Leipzig font for now
    return round((double)m_env.m_interlDefin * LEIPZIG_UNITS_PER_EM / LEIPZIG_WHOLE_NOTE_HEAD_HEIGHT);
}

void Doc::UpdateFontValues() 
{	
	if ( !m_drawingLeipzigFont.FromString( Resources::GetMusicFontDescStr() ) )
        LogWarning( "Impossible to load font 'Leipzig'" );
	
	//LogMessage( "Size %d, Family %d, Style %d, Weight %d, Underline %d, Face %s, Desc %s",
	//	m_drawingLeipzigFont.GetPointSize(),
	//	m_drawingLeipzigFont.GetFamily(),
	//	m_drawingLeipzigFont.GetStyle(),
	//	m_drawingLeipzigFont.GetWeight(),
	//	m_drawingLeipzigFont.GetUnderlined(),
	//	m_drawingLeipzigFont.GetFaceName().c_str(),
	//	m_drawingLeipzigFont.GetNativeFontInfoDesc().c_str());
    
	m_drawingFonts[0][0] = m_drawingLeipzigFont;
    m_drawingFonts[0][1] = m_drawingLeipzigFont;
    m_drawingFonts[1][0] = m_drawingLeipzigFont;
    m_drawingFonts[1][1] = m_drawingLeipzigFont;
	
	// Lyrics
	if ( !m_drawingLyricFont.FromString( Resources::GetLyricFontDescStr() ) )
		LogWarning( "Impossible to load font for the lyrics" );
    
	m_drawingLyricFonts[0] = m_drawingLyricFont;
    m_drawingLyricFonts[1] = m_drawingLyricFont;
}
    
int Doc::GetAdjustedDrawingPageHeight()
{
    assert( m_drawingPage );
    int contentHeight = m_drawingPage->GetContentHeight();
    return (contentHeight + m_drawingPageTopMar * 2);
}

    
int Doc::GetAdjustedDrawingPageWidth()
{
    assert( m_drawingPage );
    int contentWidth = m_drawingPage->GetContentWidth();
    return (contentWidth + m_drawingPageLeftMar + m_drawingPageRightMar);
}

int Doc::Save( FileOutputStream *output )
{
    ArrayPtrVoid params;
	params.push_back( output );

    MusFunctor save( &Object::Save );
    this->Process( &save, params );
    
    return true;
}

} // namespace vrv
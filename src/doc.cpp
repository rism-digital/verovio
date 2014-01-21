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
    
    m_drawPage = NULL;
    m_currentScoreDefDone = false;
    
    m_scoreDef.Clear();
}

int Doc::Save( ArrayPtrVoid params )
{  
    // param 0: output stream
    FileOutputStream *output = (FileOutputStream*)params[0];       
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
    contentPage->AlignHorizontally();
    
    System *contentSystem = dynamic_cast<System*>(contentPage->DetachChild( 0 ));
    assert( contentSystem );
    
    System *currentSystem = new System();
    contentPage->AddSystem( currentSystem );
    int shift = 0;
    int systemFullWidth = this->m_drawPageWidth - this->m_drawPageLeftMar - this->m_drawPageRightMar
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
    
    //LogMessage("Prout");
    this->SetCurrentScoreDef( true );
    contentPage->Layout( true );
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
    if ( m_drawPage && m_drawPage->GetIdx() == pageIdx ) {
        return m_drawPage;
    }
    m_drawPage = dynamic_cast<Page*>(this->GetChild( pageIdx ) );
    
    assert( m_drawPage );
    
    // we use the page members only if set (!= -1) 
    if ( m_drawPage->m_pageHeight != -1 )
    {
        m_drawPageHeight = m_drawPage->m_pageHeight;
        m_drawPageWidth = m_drawPage->m_pageWidth;
        m_drawPageLeftMar = m_drawPage->m_pageLeftMar;
        m_drawPageRightMar = m_drawPage->m_pageRightMar;
        m_drawPageTopMar = m_drawPage->m_pageTopMar;
	}
	else if ( this->m_pageHeight != -1 )
	{
        m_drawPageHeight = this->m_pageHeight;
        m_drawPageWidth = this->m_pageWidth;
        m_drawPageLeftMar = this->m_pageLeftMar;
        m_drawPageRightMar = this->m_pageRightMar;
        m_drawPageTopMar = this->m_pageTopMar;
    }
    else
    {
        m_drawPageHeight = m_env.m_pageHeight;
        m_drawPageWidth = m_env.m_pageWidth;
        m_drawPageLeftMar = m_env.m_pageLeftMar;
        m_drawPageRightMar = m_env.m_pageRightMar;
        m_drawPageTopMar = m_env.m_pageTopMar;
    }
    
    if (this->m_env.m_landscape)
    {	
        int pageHeight = m_drawPageWidth;
        m_drawPageWidth = m_drawPageHeight;
        m_drawPageHeight = pageHeight;
        int pageRightMar = m_drawPageLeftMar;
        m_drawPageLeftMar = m_drawPageRightMar;
        m_drawPageRightMar = pageRightMar;
    }
    
    // From here we could check if values have changed
    // Since  m_env.m_interlDefin stays the same, it useless to do it
    // every time for now.
    
	m_drawBeamMaxSlope = this->m_env.m_beamMaxSlope;
	m_drawBeamMinSlope = this->m_env.m_beamMinSlope;
	m_drawBeamMaxSlope /= 100;
	m_drawBeamMinSlope /= 100;
    
    m_drawSmallStaffRatio[0] = this->m_env.m_smallStaffNum;
    m_drawSmallStaffRatio[1] = this->m_env.m_smallStaffDen;
    m_drawGraceRatio[0] = this->m_env.m_graceNum;
    m_drawGraceRatio[1] = this->m_env.m_graceDen;
    
    // half of the space between two lines
    m_drawHalfInterl[0] = m_env.m_interlDefin/2;
    // same for small staves
    m_drawHalfInterl[1] = (m_drawHalfInterl[0] * m_drawSmallStaffRatio[0]) / m_drawSmallStaffRatio[1];
    // space between two lines
    m_drawInterl[0] = m_drawHalfInterl[0] * 2;
    // same for small staves
    m_drawInterl[1] = m_drawHalfInterl[1] * 2;
    // staff (with five lines)
    m_drawStaffSize[0] = m_drawInterl[0] * 4;
    m_drawStaffSize[1] = m_drawInterl[1] * 4;
    //
    m_drawOctaveSize[0] = m_drawHalfInterl[0] * 7;
    m_drawOctaveSize[1] = m_drawHalfInterl[1] * 7;
    
    m_drawStep1 = m_drawHalfInterl[0];
    m_drawStep2 = m_drawStep1 * 3;
    m_drawStep3 = m_drawStep1 * 6;
    
    // values for beams
    m_drawBeamWidth[0] = this->m_env.m_interlDefin / 2;
    m_drawBeamWhiteWidth[0] = this->m_env.m_interlDefin / 4;
    m_drawBeamWidth[1] = (m_drawBeamWidth[0] * m_drawSmallStaffRatio[0]) / m_drawSmallStaffRatio[1];
    m_drawBeamWhiteWidth[1] = (m_drawBeamWhiteWidth[0] * m_drawSmallStaffRatio[0]) / m_drawSmallStaffRatio[1];
    
    m_drawFontHeight = CalcLeipzigFontSize();
    m_drawFontHeightAscent[0][0] = floor(LEIPZIG_ASCENT * (double)m_drawFontHeight / LEIPZIG_UNITS_PER_EM);
	m_drawFontHeightAscent[0][1] = (m_drawFontHeightAscent[0][0] * m_drawGraceRatio[0]) / m_drawGraceRatio[1];
    m_drawFontHeightAscent[1][0] = (m_drawFontHeightAscent[0][0] * m_drawSmallStaffRatio[0]) / m_drawSmallStaffRatio[1];
	m_drawFontHeightAscent[1][1] = (m_drawFontHeightAscent[1][0] * m_drawGraceRatio[0]) / m_drawGraceRatio[1];
    
    m_drawFontSize[0][0] = m_drawFontHeight;
    m_drawFontSize[0][1] = (m_drawFontSize[0][0] * m_drawGraceRatio[0]) / m_drawGraceRatio[1];
    m_drawFontSize[1][0] = (m_drawFontSize[0][0] * m_drawSmallStaffRatio[0]) / m_drawSmallStaffRatio[1];
    m_drawFontSize[1][1]= (m_drawFontSize[1][0] * m_drawGraceRatio[0])/ m_drawGraceRatio[1];
    
	m_drawFonts[0][0].SetPointSize( m_drawFontSize[0][0] ); //160
    m_drawFonts[0][1].SetPointSize( m_drawFontSize[0][1] ); //120
    m_drawFonts[1][0].SetPointSize( m_drawFontSize[1][0] ); //128
    m_drawFonts[1][1].SetPointSize( m_drawFontSize[1][1] ); //100
    
	m_drawLyricFonts[0].SetPointSize( m_drawLyricFont.GetPointSize() );
    m_drawLyricFonts[1].SetPointSize( m_drawLyricFont.GetPointSize() );
    
    m_drawVerticalUnit1[0] = (float)m_drawInterl[0]/4;
    m_drawVerticalUnit2[0] = (float)m_drawInterl[0]/8;
    m_drawVerticalUnit1[1] = (float)m_drawInterl[1]/4;
    m_drawVerticalUnit2[1] = (float)m_drawInterl[1]/8;
    
    float glyph_size;
    glyph_size = (LEIPZIG_HALF_NOTE_HEAD_WIDTH * (float)m_drawFontHeight / LEIPZIG_UNITS_PER_EM);
    m_drawNoteRadius[0][0] = ceil(glyph_size / 2);
    m_drawNoteRadius[0][1] = (m_drawNoteRadius[0][0] * m_drawGraceRatio[0])/m_drawGraceRatio[1];
    m_drawNoteRadius[1][0] = (m_drawNoteRadius[0][0] * m_drawSmallStaffRatio[0])/m_drawSmallStaffRatio[1];
    m_drawNoteRadius[1][1] = (m_drawNoteRadius[1][0] * m_drawGraceRatio[0])/m_drawGraceRatio[1];
    
    m_drawLedgerLine[0][0] = (int)(glyph_size * .72);
    m_drawLedgerLine[0][1] = (m_drawLedgerLine[0][0] * m_drawGraceRatio[0])/m_drawGraceRatio[1];
    m_drawLedgerLine[1][0] = (m_drawLedgerLine[0][0] * m_drawSmallStaffRatio[0])/m_drawSmallStaffRatio[1];
    m_drawLedgerLine[1][1] = (m_drawLedgerLine[1][0] * m_drawGraceRatio[0])/m_drawGraceRatio[1];
    
    glyph_size = round(LEIPZIG_WHOLE_NOTE_HEAD_WIDTH * (double)m_drawFontHeight / LEIPZIG_UNITS_PER_EM);
    m_drawLedgerLine[0][2] = (int)(glyph_size * .66);
    m_drawLedgerLine[1][2] = (m_drawLedgerLine[0][2] * m_drawSmallStaffRatio[0]) /m_drawSmallStaffRatio[1];
    
    m_drawBrevisWidth[0] = (int)((glyph_size * 0.8) / 2);
    m_drawBrevisWidth[1] = (m_drawBrevisWidth[0] * m_drawSmallStaffRatio[0]) /m_drawSmallStaffRatio[1];
    
    glyph_size = round(LEIPZIG_SHARP_WIDTH * (double)m_drawFontHeight / LEIPZIG_UNITS_PER_EM);
    m_drawAccidWidth[0][0] = glyph_size;
    m_drawAccidWidth[0][1] = (m_drawAccidWidth[0][0] * m_drawGraceRatio[0])/m_drawGraceRatio[1];
    m_drawAccidWidth[1][0] = (m_drawAccidWidth[0][0] * m_drawSmallStaffRatio[0]) /m_drawSmallStaffRatio[1];
    m_drawAccidWidth[1][1] = (m_drawAccidWidth[1][0] * m_drawGraceRatio[0])/m_drawGraceRatio[1];
    
	return m_drawPage;
}

int Doc::CalcLeipzigFontSize( )
{
    // We just have the Leipzig font for now
    return round((double)m_env.m_interlDefin * LEIPZIG_UNITS_PER_EM / LEIPZIG_WHOLE_NOTE_HEAD_HEIGHT);
}

void Doc::UpdateFontValues() 
{	
	if ( !m_drawLeipzigFont.FromString( Resources::GetMusicFontDescStr() ) )
        LogWarning( "Impossible to load font 'Leipzig'" );
	
	//LogMessage( "Size %d, Family %d, Style %d, Weight %d, Underline %d, Face %s, Desc %s",
	//	m_drawLeipzigFont.GetPointSize(),
	//	m_drawLeipzigFont.GetFamily(),
	//	m_drawLeipzigFont.GetStyle(),
	//	m_drawLeipzigFont.GetWeight(),
	//	m_drawLeipzigFont.GetUnderlined(),
	//	m_drawLeipzigFont.GetFaceName().c_str(),
	//	m_drawLeipzigFont.GetNativeFontInfoDesc().c_str());
    
	m_drawFonts[0][0] = m_drawLeipzigFont;
    m_drawFonts[0][1] = m_drawLeipzigFont;
    m_drawFonts[1][0] = m_drawLeipzigFont;
    m_drawFonts[1][1] = m_drawLeipzigFont;
	
	// Lyrics
	if ( !m_drawLyricFont.FromString( Resources::GetLyricFontDescStr() ) )
		LogWarning( "Impossible to load font for the lyrics" );
    
	m_drawLyricFonts[0] = m_drawLyricFont;
    m_drawLyricFonts[1] = m_drawLyricFont;
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
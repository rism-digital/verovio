/////////////////////////////////////////////////////////////////////////////
// Name:        musdoc.cpp
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
    
    m_rendPage = NULL;
    
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

void Doc::Layout( )
{
    ScoreDef currentScoreDef;
    currentScoreDef = m_scoreDef;
    StaffDef *staffDef = NULL;
    ArrayPtrVoid params;
    params.push_back( &currentScoreDef );
    params.push_back( &staffDef );
    MusFunctor setPageScoreDef( &Object::SetPageScoreDef );
    this->Process( &setPageScoreDef, params );
    
    int i;
	Page *page = NULL;
    for (i = 0; i < this->GetChildCount(); i++)
	{
		page = (Page*)this->m_children[i];
        page->Layout( );
        break;
     }
}

void Doc::SetRendPage( Page *page )
{
    if ( !page || (page == m_rendPage) ) {
        return;
    }
    m_rendPage = page;
    
    // we use the page members only if set (!= -1) 
    if ( page->m_pageHeight != -1 )
    {
        m_rendPageHeight = page->m_pageHeight;
        m_rendPageWidth = page->m_pageWidth;
        m_rendPageLeftMar = page->m_pageLeftMar;
        m_rendPageRightMar = page->m_pageRightMar;
        m_rendPageTopMar = page->m_pageTopMar;
	}
	else if ( this->m_pageHeight != -1 )
	{
        m_rendPageHeight = this->m_pageHeight;
        m_rendPageWidth = this->m_pageWidth;
        m_rendPageLeftMar = this->m_pageLeftMar;
        m_rendPageRightMar = this->m_pageRightMar;
        m_rendPageTopMar = this->m_pageTopMar;
    }
    else
    {
        m_rendPageHeight = m_env.m_pageHeight;
        m_rendPageWidth = m_env.m_pageWidth;
        m_rendPageLeftMar = m_env.m_pageLeftMar;
        m_rendPageRightMar = m_env.m_pageRightMar;
        m_rendPageTopMar = m_env.m_pageTopMar;
    }
    
    if (this->m_env.m_landscape)
    {	
        int pageHeight = m_rendPageWidth;
        m_rendPageWidth = m_rendPageHeight;
        m_rendPageHeight = pageHeight;
        int pageRightMar = m_rendPageLeftMar;
        m_rendPageLeftMar = m_rendPageRightMar;
        m_rendPageRightMar = pageRightMar;
    }
    
    // From here we could check if values have changed
    // Since  m_env.m_interlDefin stays the same, it useless to do it
    // every time for now.
    
	m_rendBeamMaxSlope = this->m_env.m_beamMaxSlope;
	m_rendBeamMinSlope = this->m_env.m_beamMinSlope;
	m_rendBeamMaxSlope /= 100;
	m_rendBeamMinSlope /= 100;
    
    m_rendSmallStaffRatio[0] = this->m_env.m_smallStaffNum;
    m_rendSmallStaffRatio[1] = this->m_env.m_smallStaffDen;
    m_rendGraceRatio[0] = this->m_env.m_graceNum;
    m_rendGraceRatio[1] = this->m_env.m_graceDen;
    
    // half of the space between two lines
    m_rendHalfInterl[0] = m_env.m_interlDefin/2;
    // same for small staves
    m_rendHalfInterl[1] = (m_rendHalfInterl[0] * m_rendSmallStaffRatio[0]) / m_rendSmallStaffRatio[1];
    // space between two lines
    m_rendInterl[0] = m_rendHalfInterl[0] * 2;
    // same for small staves
    m_rendInterl[1] = m_rendHalfInterl[1] * 2;
    // staff (with five lines)
    m_rendStaffSize[0] = m_rendInterl[0] * 4;
    m_rendStaffSize[1] = m_rendInterl[1] * 4;
    //
    m_rendOctaveSize[0] = m_rendHalfInterl[0] * 7;
    m_rendOctaveSize[1] = m_rendHalfInterl[1] * 7;
    
    m_rendStep1 = m_rendHalfInterl[0];
    m_rendStep2 = m_rendStep1 * 3;
    m_rendStep3 = m_rendStep1 * 6;
    
    // values for beams
    m_rendBeamWidth[0] = this->m_env.m_beamWidth;
    m_rendBeamWhiteWidth[0] = this->m_env.m_beamWhiteWidth;
    m_rendBeamWidth[1] = (m_rendBeamWidth[0] * m_rendSmallStaffRatio[0]) / m_rendSmallStaffRatio[1];
    m_rendBeamWhiteWidth[1] = (m_rendBeamWhiteWidth[0] * m_rendSmallStaffRatio[0]) / m_rendSmallStaffRatio[1];
    
    m_rendFontHeight = CalcLeipzigFontSize();
    m_rendFontHeightAscent[0][0] = floor(LEIPZIG_ASCENT * (double)m_rendFontHeight / LEIPZIG_UNITS_PER_EM);
	m_rendFontHeightAscent[0][0] +=  Vrv::GetFontPosCorrection();
	m_rendFontHeightAscent[0][1] = (m_rendFontHeightAscent[0][0] * m_rendGraceRatio[0]) / m_rendGraceRatio[1];
    m_rendFontHeightAscent[1][0] = (m_rendFontHeightAscent[0][0] * m_rendSmallStaffRatio[0]) / m_rendSmallStaffRatio[1];
	m_rendFontHeightAscent[1][1] = (m_rendFontHeightAscent[1][0] * m_rendGraceRatio[0]) / m_rendGraceRatio[1];
    
    m_rendFontSize[0][0] = m_rendFontHeight;
    m_rendFontSize[0][1] = (m_rendFontSize[0][0] * m_rendGraceRatio[0]) / m_rendGraceRatio[1];
    m_rendFontSize[1][0] = (m_rendFontSize[0][0] * m_rendSmallStaffRatio[0]) / m_rendSmallStaffRatio[1];
    m_rendFontSize[1][1]= (m_rendFontSize[1][0] * m_rendGraceRatio[0])/ m_rendGraceRatio[1];
    
	m_rendFonts[0][0].SetPointSize( m_rendFontSize[0][0] ); //160
    m_rendFonts[0][1].SetPointSize( m_rendFontSize[0][1] ); //120
    m_rendFonts[1][0].SetPointSize( m_rendFontSize[1][0] ); //128
    m_rendFonts[1][1].SetPointSize( m_rendFontSize[1][1] ); //100
    
	m_rendLyricFonts[0].SetPointSize( m_rendLyricFont.GetPointSize() );
    m_rendLyricFonts[1].SetPointSize( m_rendLyricFont.GetPointSize() );
    
    m_rendVerticalUnit1[0] = (float)m_rendInterl[0]/4;
    m_rendVerticalUnit2[0] = (float)m_rendInterl[0]/8;
    m_rendVerticalUnit1[1] = (float)m_rendInterl[1]/4;
    m_rendVerticalUnit2[1] = (float)m_rendInterl[1]/8;
    
    float glyph_size;
    glyph_size = (LEIPZIG_HALF_NOTE_HEAD_WIDTH * (float)m_rendFontHeight / LEIPZIG_UNITS_PER_EM);
    m_rendNoteRadius[0][0] = ceil(glyph_size / 2);
    m_rendNoteRadius[0][1] = (m_rendNoteRadius[0][0] * m_rendGraceRatio[0])/m_rendGraceRatio[1];
    m_rendNoteRadius[1][0] = (m_rendNoteRadius[0][0] * m_rendSmallStaffRatio[0])/m_rendSmallStaffRatio[1];
    m_rendNoteRadius[1][1] = (m_rendNoteRadius[1][0] * m_rendGraceRatio[0])/m_rendGraceRatio[1];
    
    m_rendLedgerLine[0][0] = (int)(glyph_size * .72);
    m_rendLedgerLine[0][1] = (m_rendLedgerLine[0][0] * m_rendGraceRatio[0])/m_rendGraceRatio[1];
    m_rendLedgerLine[1][0] = (m_rendLedgerLine[0][0] * m_rendSmallStaffRatio[0])/m_rendSmallStaffRatio[1];
    m_rendLedgerLine[1][1] = (m_rendLedgerLine[1][0] * m_rendGraceRatio[0])/m_rendGraceRatio[1];
    
    glyph_size = round(LEIPZIG_WHOLE_NOTE_HEAD_WIDTH * (double)m_rendFontHeight / LEIPZIG_UNITS_PER_EM);
    m_rendLedgerLine[0][2] = (int)(glyph_size * .66);
    m_rendLedgerLine[1][2] = (m_rendLedgerLine[0][2] * m_rendSmallStaffRatio[0]) /m_rendSmallStaffRatio[1];
    
    m_rendBrevisWidth[0] = (int)((glyph_size * 0.8) / 2);
    m_rendBrevisWidth[1] = (m_rendBrevisWidth[0] * m_rendSmallStaffRatio[0]) /m_rendSmallStaffRatio[1];
    
    glyph_size = round(LEIPZIG_SHARP_WIDTH * (double)m_rendFontHeight / LEIPZIG_UNITS_PER_EM);
    m_rendAccidWidth[0][0] = glyph_size;
    m_rendAccidWidth[0][1] = (m_rendAccidWidth[0][0] * m_rendGraceRatio[0])/m_rendGraceRatio[1];
    m_rendAccidWidth[1][0] = (m_rendAccidWidth[0][0] * m_rendSmallStaffRatio[0]) /m_rendSmallStaffRatio[1];
    m_rendAccidWidth[1][1] = (m_rendAccidWidth[1][0] * m_rendGraceRatio[0])/m_rendGraceRatio[1];
    
	return;
}

int Doc::CalcLeipzigFontSize( )
{
    // We just have the Leipzig font for now
    return round((double)m_env.m_interlDefin * LEIPZIG_UNITS_PER_EM / LEIPZIG_WHOLE_NOTE_HEAD_HEIGHT);
}

void Doc::UpdateFontValues() 
{	
	if ( !m_rendLeipzigFont.FromString( Vrv::GetMusicFontDescStr() ) )
        Vrv::LogWarning( "Impossible to load font 'Leipzig'" );
	
	//Vrv::LogMessage( "Size %d, Family %d, Style %d, Weight %d, Underline %d, Face %s, Desc %s",
	//	m_rendLeipzigFont.GetPointSize(),
	//	m_rendLeipzigFont.GetFamily(),
	//	m_rendLeipzigFont.GetStyle(),
	//	m_rendLeipzigFont.GetWeight(),
	//	m_rendLeipzigFont.GetUnderlined(),
	//	m_rendLeipzigFont.GetFaceName().c_str(),
	//	m_rendLeipzigFont.GetNativeFontInfoDesc().c_str());
    
	m_rendFonts[0][0] = m_rendLeipzigFont;
    m_rendFonts[0][1] = m_rendLeipzigFont;
    m_rendFonts[1][0] = m_rendLeipzigFont;
    m_rendFonts[1][1] = m_rendLeipzigFont;
	
	// Lyrics
	if ( !m_rendLyricFont.FromString( Vrv::GetLyricFontDescStr() ) )
		Vrv::LogWarning( "Impossible to load font for the lyrics" );
    
	m_rendLyricFonts[0] = m_rendLyricFont;
    m_rendLyricFonts[1] = m_rendLyricFont;
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
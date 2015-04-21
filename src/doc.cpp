/////////////////////////////////////////////////////////////////////////////
// Name:        doc.cpp
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "doc.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "att_comparison.h"
#include "barline.h"
#include "chord.h"
#include "glyph.h"
#include "keysig.h"
#include "layer.h"
#include "mensur.h"
#include "metersig.h"
#include "mrest.h"
#include "multirest.h"
#include "note.h"
#include "page.h"
#include "slur.h"
#include "smufl.h"
#include "staff.h"
#include "style.h"
#include "syl.h"
#include "system.h"
#include "verse.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Doc
//----------------------------------------------------------------------------

Doc::Doc() :
    Object("doc-")
{
    m_style = new Style();
    Reset( Raw );
}

Doc::~Doc()
{
    delete m_style;
    
}

void Doc::Reset( DocType type )
{
    Object::Reset();
    
    m_type = type;
    m_pageWidth = -1;
    m_pageHeight = -1;
    m_pageRightMar = 0;
    m_pageLeftMar = 0;
    m_pageTopMar = 0;
    
    m_spacingStaff = m_style->m_spacingStaff;
    m_spacingSystem = m_style->m_spacingSystem;
    
    m_drawingPage = NULL;
    m_drawingJustifyX = true;
    m_currentScoreDefDone = false;
    m_drawingPreparationDone = false;
    
    m_scoreDef.Reset();
    
    m_drawingLyricFont.SetFaceName("Garamond");
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
    
void Doc::PrepareDrawing()
{
    ArrayPtrVoid params;
    
    if (m_drawingPreparationDone) {
        Functor resetDrawing( &Object::ResetDarwing );
        this->Process( &resetDrawing, params );
    }
    
    // Try to match all spanning elements (slur, tie, etc) by processing backward
    std::vector<DocObject*> timeSpanningElements;
    bool fillList = true;
    params.push_back( &timeSpanningElements );
    params.push_back( &fillList );
    Functor prepareTimeSpanning( &Object::PrepareTimeSpanning );
    this->Process( &prepareTimeSpanning, params, NULL, NULL, UNLIMITED_DEPTH, BACKWARD );
    
    // First we tried backward because nomrally the spanning elements are at the end of
    // the measure. However, in some case, one (or both) end points will appear afterwards
    // in the encoding. For these, the previous iteration will not have resolved the link and
    // the spanning elements will remain in the timeSpanningElements array. We try again forward
    // but this time without filling the list (that is only will the remaining elements)
    if ( !timeSpanningElements.empty() ) {
        fillList = false;
        this->Process( &prepareTimeSpanning, params );
    }
    
    // If some are still there, then it is probably an issue in the encoding
    if ( !timeSpanningElements.empty() ) {
        LogWarning("%d time spanning elements could not be matched", timeSpanningElements.size() );
    }
    
    // We need to populate processing lists for processing the document by Layer (for matching @tie) and
    // by Verse (for matching syllable connectors)
    params.clear();
    IntTree verseTree;
    IntTree layerTree;
    params.push_back( &verseTree );
    params.push_back( &layerTree );
    // Alternate solution with StaffN_LayerN_VerseN_t (see also Verse::PrepareDrawing)
    //StaffN_LayerN_VerseN_t staffLayerVerseTree;
    //params.push_back( &staffLayerVerseTree );
    
    // We first fill a tree of int with [staff/layer] and [staff/layer/verse] numbers (@n) to be process
    //LogElapsedTimeStart( );
    Functor prepareProcessingLists( &Object::PrepareProcessingLists );
    this->Process( &prepareProcessingLists, params );
    
    // The tree is used to process each staff/layer/verse separatly
    // For this, we use a array of AttCommmonNComparison that looks for each object if it is of the type
    // and with @n specified
    
    IntTree_t::iterator staves;
    IntTree_t::iterator layers;
    IntTree_t::iterator verses;
    
    // Process by layer for matching @tie attribute - we process notes and chords, looking at
    // GetTie values and pitch and oct for matching notes
    Chord *currentChord;
    std::vector<Note*> currentNotes;
    std::vector<AttComparison*> filters;
    for (staves = layerTree.child.begin(); staves != layerTree.child.end(); ++staves) {
        for (layers = staves->second.child.begin(); layers != staves->second.child.end(); ++layers) {
            filters.clear();
            // Create ad comparison object for each type / @n
            AttCommonNComparison matchStaff( &typeid(Staff), staves->first );
            AttCommonNComparison matchLayer( &typeid(Layer), layers->first );
            filters.push_back( &matchStaff );
            filters.push_back( &matchLayer );
            
            // The first pass set m_drawingFirstNote and m_drawingLastNote for each syl
            // m_drawingLastNote is set only if the syl has a forward connector
            currentChord = NULL;
            currentNotes.clear();
            ArrayPtrVoid paramsTieAttr;
            paramsTieAttr.push_back( &currentNotes );
            paramsTieAttr.push_back( &currentChord );
            Functor prepareTieAttr( &Object::PrepareTieAttr );
            Functor prepareTieAttrEnd( &Object::PrepareTieAttrEnd );
            this->Process( &prepareTieAttr, paramsTieAttr, &prepareTieAttrEnd, &filters );
        
            // After having processed one layer, we check if we have open ties - if yes, we
            // must reset them and they will be ignored.
            if ( !currentNotes.empty() ) {
                std::vector<Note*>::iterator iter;
                for (iter = currentNotes.begin(); iter != currentNotes.end(); iter++) {
                    LogWarning("Unable to match @tie of note '%s', skipping it", (*iter)->GetUuid().c_str());
                    (*iter)->ResetDrawingTieAttr();
                }
            }
        }
    }
    
    Note *currentNote = NULL;
    for (staves = layerTree.child.begin(); staves != layerTree.child.end(); ++staves) {
        for (layers = staves->second.child.begin(); layers != staves->second.child.end(); ++layers) {
            filters.clear();
            // Create ad comparison object for each type / @n
            AttCommonNComparison matchStaff( &typeid(Staff), staves->first );
            AttCommonNComparison matchLayer( &typeid(Layer), layers->first );
            filters.push_back( &matchStaff );
            filters.push_back( &matchLayer );

            ArrayPtrVoid paramsPointers;
            paramsPointers.push_back( &currentNote );
            Functor preparePointersByLayer( &Object::PreparePointersByLayer );
            this->Process( &preparePointersByLayer, paramsPointers, NULL, &filters );
        }
    }
    
    // Same for the lyrics, but Verse by Verse since Syl are TimeSpanningInterface elements for handling connectors
    Syl *currentSyl;
    Note *lastNote;
    Note *lastButOneNote;
    for (staves = verseTree.child.begin(); staves != verseTree.child.end(); ++staves) {
        for (layers = staves->second.child.begin(); layers != staves->second.child.end(); ++layers) {
            for (verses= layers->second.child.begin(); verses != layers->second.child.end(); ++verses) {
                //std::cout << staves->first << " => " << layers->first << " => " << verses->first << '\n';
                filters.clear();
                // Create ad comparison object for each type / @n
                AttCommonNComparison matchStaff( &typeid(Staff), staves->first );
                AttCommonNComparison matchLayer( &typeid(Layer), layers->first );
                AttCommonNComparison matchVerse( &typeid(Verse), verses->first );
                filters.push_back( &matchStaff );
                filters.push_back( &matchLayer );
                filters.push_back( &matchVerse );
                
                // The first pass set m_drawingFirstNote and m_drawingLastNote for each syl
                // m_drawingLastNote is set only if the syl has a forward connector
                currentSyl = NULL;
                lastNote = NULL;
                lastButOneNote = NULL;
                ArrayPtrVoid paramsLyrics;
                paramsLyrics.push_back( &currentSyl );
                paramsLyrics.push_back( &lastNote );
                paramsLyrics.push_back( &lastButOneNote );
                Functor prepareLyrics( &Object::PrepareLyrics );
                Functor prepareLyricsEnd( &Object::PrepareLyricsEnd );
                this->Process( &prepareLyrics, paramsLyrics, &prepareLyricsEnd, &filters );
            }
        }
    }
    
    // Once <slur> , <ties> and @ties are matched but also syl connectors, we need to set them as running TimeSpanningInterface
    // to each staff they are extended. This does not need to be done staff by staff because we can just check the
    // staff->GetN to see where we are (see Staff::FillStaffCurrentTimeSpanning)
    params.clear();
    timeSpanningElements.clear();
    params.push_back( &timeSpanningElements );
    Functor fillStaffCurrentTimeSpanning( &Object::FillStaffCurrentTimeSpanning );
    this->Process( &fillStaffCurrentTimeSpanning, params );
    
    // Something must be wrong in the encoding because a TimeSpanningInterface was left open
    if ( !timeSpanningElements.empty() ) {
        LogDebug("%d time spanning elements could not be set as running", timeSpanningElements.size() );
    }
    
    /*
    // Alternate solution with StaffN_LayerN_VerseN_t
    StaffN_LayerN_VerseN_t::iterator staves;
    LayerN_VerserN_t::iterator layers;
    VerseN_t::iterator verses;
    std::vector<AttComparison*> filters;
    for (staves = staffLayerVerseTree.begin(); staves != staffLayerVerseTree.end(); ++staves) {
        for (layers = staves->second.begin(); layers != staves->second.end(); ++layers) {
            for (verses= layers->second.begin(); verses != layers->second.end(); ++verses) {
                std::cout << staves->first << " => " << layers->first << " => " << verses->first << '\n';
                filters.clear();
                AttCommonNComparison matchStaff( &typeid(Staff), staves->first );
                AttCommonNComparison matchLayer( &typeid(Layer), layers->first );
                AttCommonNComparison matchVerse( &typeid(Verse), verses->first );
                filters.push_back( &matchStaff );
                filters.push_back( &matchLayer );
                filters.push_back( &matchVerse );

                ArrayPtrVoid paramsLyrics;
                Functor prepareLyrics( &Object::PrepareLyrics );
                this->Process( &prepareLyrics, paramsLyrics, NULL, &filters );
            }
        }
    }
    */
    
    //LogElapsedTimeEnd ( "Preparing drawing" );
    
    m_drawingPreparationDone = true;
}
    
void Doc::SetCurrentScoreDef( bool force )
{
    if ( m_currentScoreDefDone && !force ) {
        return;
    }
    
    ScoreDef currentScoreDef;
    currentScoreDef = m_scoreDef;
    StaffDef *staffDef = NULL;
    ArrayPtrVoid params;
    params.push_back( &currentScoreDef );
    params.push_back( &staffDef );
    Functor setCurrentScoreDef( &Object::SetCurrentScoreDef );
    
    //LogElapsedTimeStart( );
    this->Process( &setCurrentScoreDef, params );
    //LogElapsedTimeEnd ( "Setting scoreDefs" );
    
    m_currentScoreDefDone = true;
}

void Doc::CastOff( )
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
    Functor castOffSystems( &Object::CastOffSystems );
    contentSystem->Process( &castOffSystems, params );
    delete contentSystem;
    
    //LogDebug("Layout: %d systems", contentPage->GetSystemCount());
    
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
    Functor castOffPages( &Object::CastOffPages );
    contentPage->Process( &castOffPages, params );
    delete contentPage;
    
    //LogDebug("Layout: %d pages", this->GetChildCount());

    // We need to reset the drawing page to NULL
    // because idx will still be 0 but contentPage is dead!
    this->ResetDrawingPage( );
    this->SetCurrentScoreDef( true );
}
    
void Doc::UnCastOff( )
{  
    Page *contentPage = new Page();
    System *contentSystem = new System();
    contentPage->AddSystem( contentSystem );

    ArrayPtrVoid params;
    params.push_back( contentSystem );

    Functor unCastOff( &Object::UnCastOff );
    this->Process( &unCastOff, params );
    
    this->ClearChildren();
    
    this->AddPage(contentPage);
    
    //LogDebug("ContinousLayout: %d pages", this->GetChildCount());
    
    // We need to reset the drawing page to NULL
    // because idx will still be 0 but contentPage is dead!
    this->ResetDrawingPage( );
    this->SetCurrentScoreDef( true );
}
    
bool Doc::HasPage( int pageIdx )
{
    return ( (pageIdx >= 0 ) && (pageIdx < GetChildCount() ) );
}

int Doc::GetPageCount( )
{
    return GetChildCount() ;
}
    

short Doc::GetLeftMargin( const std::type_info *elementType )
{

    if (typeid(Barline) == *elementType) return 5;
    else if (typeid(Chord) == *elementType) return 10;
    else if (typeid(Clef) == *elementType) return -20;
    else if (typeid(MRest) == *elementType) return 30;
    else if (typeid(Note) == *elementType) return 10;
    return 0;

}
    
short Doc::GetRightMargin( const std::type_info *elementType )
{
    if (typeid(Clef) == *elementType) return 30;
    else if (typeid(KeySig) == *elementType)  return 30;
    else if (typeid(Mensur) == *elementType) return 30;
    else if (typeid(MeterSig) == *elementType) return 30;
    else if (typeid(Barline) == *elementType) return 30;
    else if (typeid(BarlineAttr) == *elementType) return 0;
    else if (typeid(MRest) == *elementType) return 30;
    else if (typeid(MultiRest) == *elementType) return 30;
    //else if (typeid(Note) == *elementType) return 10;
    return 10;
}
    
void Doc:: SetPageHeight( int pageHeight )
{
    m_pageHeight = pageHeight * DEFINITON_FACTOR;
};

void Doc::SetPageWidth( int pageWidth )
{
    m_pageWidth = pageWidth * DEFINITON_FACTOR;
};

void Doc::SetPageLeftMar( short pageLeftMar )
{
    m_pageLeftMar = pageLeftMar * DEFINITON_FACTOR;
};

void Doc::SetPageRightMar( short pageRightMar )
{
    m_pageRightMar = pageRightMar * DEFINITON_FACTOR;
};

void Doc::SetPageTopMar( short pageTopMar )
{
    m_pageTopMar = pageTopMar * DEFINITON_FACTOR;
};

void Doc::SetSpacingStaff( short spacingStaff )
{
    m_spacingStaff = spacingStaff;
};

void Doc::SetSpacingSystem( short spacingSystem )
{
    m_spacingSystem = spacingSystem;
};



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
        m_drawingPageHeight = m_style->m_pageHeight;
        m_drawingPageWidth = m_style->m_pageWidth;
        m_drawingPageLeftMar = m_style->m_pageLeftMar;
        m_drawingPageRightMar = m_style->m_pageRightMar;
        m_drawingPageTopMar = m_style->m_pageTopMar;
    }
    
    if (this->m_style->m_landscape)
    {	
        int pageHeight = m_drawingPageWidth;
        m_drawingPageWidth = m_drawingPageHeight;
        m_drawingPageHeight = pageHeight;
        int pageRightMar = m_drawingPageLeftMar;
        m_drawingPageLeftMar = m_drawingPageRightMar;
        m_drawingPageRightMar = pageRightMar;
    }
    
    // From here we could check if values have changed
    // Since  m_style->m_interlDefin stays the same, it useless to do it
    // every time for now.
    
	m_drawingBeamMaxSlope = this->m_style->m_beamMaxSlope;
	m_drawingBeamMinSlope = this->m_style->m_beamMinSlope;
	m_drawingBeamMaxSlope /= 100;
	m_drawingBeamMinSlope /= 100;
    
    m_drawingSmallStaffRatio[0] = this->m_style->m_smallStaffNum;
    m_drawingSmallStaffRatio[1] = this->m_style->m_smallStaffDen;
    m_drawingGraceRatio[0] = this->m_style->m_graceNum;
    m_drawingGraceRatio[1] = this->m_style->m_graceDen;
    
    // half of the space between two lines
    m_drawingUnit[0] = m_style->m_unit;
    // same for small staves
    m_drawingUnit[1] = (m_drawingUnit[0] * m_drawingSmallStaffRatio[0]) / m_drawingSmallStaffRatio[1];
    // space between two lines
    m_drawingDoubleUnit[0] = m_drawingUnit[0] * 2;
    // same for small staves
    m_drawingDoubleUnit[1] = m_drawingUnit[1] * 2;
    // staff (with five lines)
    m_drawingStaffSize[0] = m_drawingDoubleUnit[0] * 4;
    m_drawingStaffSize[1] = m_drawingDoubleUnit[1] * 4;
    //
    m_drawingOctaveSize[0] = m_drawingUnit[0] * 7;
    m_drawingOctaveSize[1] = m_drawingUnit[1] * 7;
    
    // values for beams
    m_drawingBeamWidth[0] = this->m_style->m_unit;
    m_drawingBeamWhiteWidth[0] = this->m_style->m_unit / 2;
    m_drawingBeamWidth[1] = (m_drawingBeamWidth[0] * m_drawingSmallStaffRatio[0]) / m_drawingSmallStaffRatio[1];
    m_drawingBeamWhiteWidth[1] = (m_drawingBeamWhiteWidth[0] * m_drawingSmallStaffRatio[0]) / m_drawingSmallStaffRatio[1];
    
    m_drawingFontHeight = CalcMusicFontSize();
    
    /*
    m_drawingFontHeightAscent[0][0] = floor(LEIPZIG_ASCENT * (double)m_drawingFontHeight / LEIPZIG_UNITS_PER_EM);
	m_drawingFontHeightAscent[0][1] = (m_drawingFontHeightAscent[0][0] * m_drawingGraceRatio[0]) / m_drawingGraceRatio[1];
    m_drawingFontHeightAscent[1][0] = (m_drawingFontHeightAscent[0][0] * m_drawingSmallStaffRatio[0]) / m_drawingSmallStaffRatio[1];
	m_drawingFontHeightAscent[1][1] = (m_drawingFontHeightAscent[1][0] * m_drawingGraceRatio[0]) / m_drawingGraceRatio[1];
    */
    
	m_drawingSmuflFonts[0][0].SetPointSize( m_drawingFontHeight );
    m_drawingSmuflFonts[0][1].SetPointSize( (m_drawingFontHeight * m_drawingGraceRatio[0]) / m_drawingGraceRatio[1] );
    m_drawingSmuflFonts[1][0].SetPointSize( (m_drawingFontHeight * m_drawingSmallStaffRatio[0]) / m_drawingSmallStaffRatio[1] );
    m_drawingSmuflFonts[1][1].SetPointSize( ( (m_drawingFontHeight * m_drawingSmallStaffRatio[0]) / m_drawingSmallStaffRatio[1] * m_drawingGraceRatio[0]) / m_drawingGraceRatio[1] );
    
    m_drawingLyricFonts[0] = m_drawingLyricFont;
    m_drawingLyricFonts[1] = m_drawingLyricFont;
	m_drawingLyricFonts[0].SetPointSize( m_drawingUnit[0] * m_style->m_lyricSize / PARAM_DENOMINATOR );
    m_drawingLyricFonts[1].SetPointSize( m_drawingUnit[1] * m_style->m_lyricSize / PARAM_DENOMINATOR );
    
    m_drawingMinMeasureWidth = m_drawingUnit[0] * m_style->m_minMeasureWidth / PARAM_DENOMINATOR ;
    
    float glyph_size;
    Glyph *glyph;
    int x, y, w, h;
    glyph = Resources::GetGlyph(SMUFL_E0A3_noteheadHalf);
    assert( glyph );
    glyph->GetBoundingBox( &x, &y, &w, &h );

    glyph_size = round((double)w * (double)m_drawingFontHeight / (double)glyph->GetUnitsPerEm());
    m_drawingNoteRadius[0][0] = ceil(glyph_size / 2);
    m_drawingNoteRadius[0][1] = (m_drawingNoteRadius[0][0] * m_drawingGraceRatio[0])/m_drawingGraceRatio[1];
    m_drawingNoteRadius[1][0] = (m_drawingNoteRadius[0][0] * m_drawingSmallStaffRatio[0])/m_drawingSmallStaffRatio[1];
    m_drawingNoteRadius[1][1] = (m_drawingNoteRadius[1][0] * m_drawingGraceRatio[0])/m_drawingGraceRatio[1];
    
    m_drawingLedgerLine[0][0] = (int)(glyph_size * .72);
    m_drawingLedgerLine[0][1] = (m_drawingLedgerLine[0][0] * m_drawingGraceRatio[0])/m_drawingGraceRatio[1];
    m_drawingLedgerLine[1][0] = (m_drawingLedgerLine[0][0] * m_drawingSmallStaffRatio[0])/m_drawingSmallStaffRatio[1];
    m_drawingLedgerLine[1][1] = (m_drawingLedgerLine[1][0] * m_drawingGraceRatio[0])/m_drawingGraceRatio[1];
    
    glyph = Resources::GetGlyph(SMUFL_E0A2_noteheadWhole);
    assert( glyph );
    glyph->GetBoundingBox( &x, &y, &w, &h );
    glyph_size = round((double)w * (double)m_drawingFontHeight / (double)glyph->GetUnitsPerEm());
    m_drawingLedgerLine[0][2] = (int)(glyph_size * .66);
    m_drawingLedgerLine[1][2] = (m_drawingLedgerLine[0][2] * m_drawingSmallStaffRatio[0]) /m_drawingSmallStaffRatio[1];
    
    m_drawingBrevisWidth[0] = (int)((glyph_size * 0.8) / 2);
    m_drawingBrevisWidth[1] = (m_drawingBrevisWidth[0] * m_drawingSmallStaffRatio[0]) /m_drawingSmallStaffRatio[1];
 
    glyph = Resources::GetGlyph(SMUFL_E262_accidentalSharp);
    assert( glyph );
    glyph->GetBoundingBox( &x, &y, &w, &h );
    glyph_size = round((double)w * (double)m_drawingFontHeight / (double)glyph->GetUnitsPerEm());
    m_drawingAccidWidth[0][0] = glyph_size;
    m_drawingAccidWidth[0][1] = (m_drawingAccidWidth[0][0] * m_drawingGraceRatio[0])/m_drawingGraceRatio[1];
    m_drawingAccidWidth[1][0] = (m_drawingAccidWidth[0][0] * m_drawingSmallStaffRatio[0]) /m_drawingSmallStaffRatio[1];
    m_drawingAccidWidth[1][1] = (m_drawingAccidWidth[1][0] * m_drawingGraceRatio[0])/m_drawingGraceRatio[1];
    glyph_size = round((double)h * (double)m_drawingFontHeight / (double)glyph->GetUnitsPerEm());
    m_drawingAccidHeight[0][0] = glyph_size;
    m_drawingAccidHeight[0][1] = (m_drawingAccidHeight[0][0] * m_drawingGraceRatio[0])/m_drawingGraceRatio[1];
    m_drawingAccidHeight[1][0] = (m_drawingAccidHeight[0][0] * m_drawingSmallStaffRatio[0]) /m_drawingSmallStaffRatio[1];
    m_drawingAccidHeight[1][1] = (m_drawingAccidHeight[1][0] * m_drawingGraceRatio[0])/m_drawingGraceRatio[1];
    
	return m_drawingPage;
}

int Doc::CalcMusicFontSize( )
{
    return m_style->m_unit * 8;
}
    
int Doc::GetAdjustedDrawingPageHeight()
{
    assert( m_drawingPage );
    int contentHeight = m_drawingPage->GetContentHeight();
    return (contentHeight + m_drawingPageTopMar * 2) / DEFINITON_FACTOR;
}

    
int Doc::GetAdjustedDrawingPageWidth()
{
    assert( m_drawingPage );
    int contentWidth = m_drawingPage->GetContentWidth();
    return (contentWidth + m_drawingPageLeftMar + m_drawingPageRightMar) / DEFINITON_FACTOR;;
}
    
//----------------------------------------------------------------------------
// Doc functors methods
//----------------------------------------------------------------------------

int Doc::PrepareLyricsEnd( ArrayPtrVoid params )
{
    // param 0: the current Syl
    // param 1: the last Note
    Syl **currentSyl = static_cast<Syl**>(params[0]);
    Note **lastNote = static_cast<Note**>(params[1]);
    
    if ( (*currentSyl) && (*lastNote) ) {
        (*currentSyl)->SetEnd(*lastNote);
    }
    
    return FUNCTOR_STOP;
}

} // namespace vrv

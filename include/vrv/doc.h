/////////////////////////////////////////////////////////////////////////////
// Name:        doc.h
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_DOC_H__
#define __VRV_DOC_H__

#include "devicecontextbase.h"
#include "scoredef.h"
#include "style.h"

namespace vrv {

class FontInfo;
class Page;
    
enum DocType {
    Raw = 0,
    Rendering,
    Transcription
};


//----------------------------------------------------------------------------
// Doc
//----------------------------------------------------------------------------

/** 
 * This class is a hold the data and corresponds to the model of a MVC design pattern.
 */
class Doc: public Object
{

public:
    // constructors and destructors
    Doc();
    virtual ~Doc();
    virtual ClassId Is() { return DOC; }
	
	void AddPage( Page *page );
    
    /*
     * Clear the content of the document.
     */ 
    void Reset( DocType type );
    
    /**
     * Refreshes the views from Doc.
     */
    virtual void Refresh();
    
    /**
     * Getter for the DocType.
     * The setter is Doc::Reset.
     */
    DocType GetType() { return m_type; };
    
    /**
     * Check if the document has a page with the specified value
     */
    bool HasPage( int pageIdx );
    
	/**
	* Get the total page count
	*/
	int GetPageCount( );
    
    /**
     * @name Get the height or width for a glyph taking into account the staff and grace sizes  
     */
    ///@{
    int GetGlyphHeight( wchar_t smuflCode, int staffSize, bool graceSize );
    int GetGlyphWidth( wchar_t smuflCode, int staffSize, bool graceSize );
    int GetDrawingUnit( int staffSize );
    int GetDrawingDoubleUnit( int staffSize );
    int GetDrawingStaffSize( int staffSize );
    int GetDrawingOctaveSize( int staffSize );
    int GetDrawingBrevisWidth( int staffSize );
    int GetDrawingBarLineWidth( int staffSize );
    int GetDrawingStaffLineWidth( int staffSize );
    int GetDrawingStemWidth( int staffSize );
    int GetDrawingBeamWidth( int staffSize, bool graceSize );
    int GetDrawingBeamWhiteWidth( int staffSize, bool graceSize );
    int GetDrawingLedgerLineLength( int staffSize, bool graceSize );
    int GetGraceSize( int value );
    ///@}
    
    /**
     * @name Get the height or width for a glyph taking into account the staff and grace sizes  
     */
    ///@{
    FontInfo *GetDrawingSmuflFont( int staffSize, bool graceSize );
    FontInfo *GetDrawingLyricFont( int staffSize );
    ///@}

    /**
     * @name Setters for the page dimensions and margins
     */
    ///@{
    void SetPageHeight( int pageHeight );
    void SetPageWidth( int pageWidth );
    void SetPageLeftMar( short pageLeftMar );
    void SetPageRightMar( short pageRightMar );
    void SetPageTopMar( short pageTopMar );
    void SetSpacingStaff( short spacingStaff );
    void SetSpacingSystem( short spacingSystem );
    ///@}
    
    /**
     * @name Getters for tie and slur parameters
     */
    ///@{
    char GetTieThickness() { return m_style->m_tieThickness; };
    char GetSlurMinHeight() { return m_style->m_minSlurHeight; };
    char GetSlurMaxHeight() { return m_style->m_maxSlurHeight; };
    char GetSlurThickness() { return m_style->m_slurThickness; };
    ///@}
     

    /**
     * @name Getters for the page dimensions and margins
     */
    ///@{
    short GetSpacingStaff( ) { return m_spacingStaff; };
    short GetSpacingSystem( ) { return m_spacingSystem; };
    ///@}
    
    /**
     * @name Getters for the object margins (left and right).
     * The margins are given in x / PARAM_DENOMINATOR * UNIT
     * With PARAM_DENOMINATOR == 10, a margin of 25 is 2.5 UNIT.
     * These should eventually be set at parameters.
     */
    ///@{
    char GetLeftMargin( const ClassId classId  );
    char GetRightMargin( const ClassId classId );
    char GetLeftPosition( );
    ///@}

    /*
     * @name Setter and getter for the justification (x-axis) flag.
     * Justification is enabled by default. It needs to be disabled
     * for drawing the entire document on one single system.
     */
    ///@{
    void SetJustificationX( bool drawingJustifyX ) { m_drawingJustifyX = drawingJustifyX; };
    bool GetJustificationX( ) { return m_drawingJustifyX; };
    ///@}

    /*
     * @name Setter and getter for the duration-based-spacing flag.
     * Spacing by duration is always used with CMN, and it's enabled by default.
     * It should be disabled (so we get "even" note spacing) for mensural notation.
     */
    ///@{
    void SetEvenSpacing( bool drawingEvenSpacing ) { m_drawingEvenSpacing = drawingEvenSpacing; };
    bool GetEvenSpacing( ) { return m_drawingEvenSpacing; };
    ///@}
    
    /*
     * @name Setter and getter for linear and non-linear spacing parameters
     */
    ///@{
    void SetSpacingLinear( double drawingSpacingLinear ) { m_drawingSpacingLinear = drawingSpacingLinear; };
    double GetSpacingLinear( ) { return m_drawingSpacingLinear; };
    void SetSpacingNonLinear( double drawingSpacingNonLinear ) { m_drawingSpacingNonLinear = drawingSpacingNonLinear; };
    double GetSpacingNonLinear( ) { return m_drawingSpacingNonLinear; };
    ///@}

    /**
     * Set the initial scoreDef of each page.
     * This is necessary for integrating changes that occur within a page.
     * It uses the MusObject::SetPageScoreDef functor method for parsing the file.
     * This will be done only if m_currentScoreDefDone is false or force is true.
     */
     void SetCurrentScoreDef( bool force = false );
    
    /**
     * Prepare the document for drawing.
     * This sets drawing pointers and value and needs to be done after loading and any editing.
     * For example, it sets the approriate values for the lyrics connectors
     */
    void PrepareDrawing( );
    
    /**
     * Casts off the entire document.
     * Starting from a single system, create and fill pages and systems.
     */
    void CastOff( );
    
    /**
     * Undo the cast off of the entire document.
     * The document will then contain one single page with one single system.
     */
    void UnCastOff( );
    
    /**
     * To be implemented.
     */
    void RefreshViews( ) {};
	
	/**
     * Set drawing values (page size, etc) when drawing a page.
     * By default, the page size of the document is taken.
     * If a page is given, the size of the page is taken.
     * calculFormatPapier() in Wolfgang
     */
	Page *SetDrawingPage( int pageIdx );
    
	/**
     * Reset drawing page to NULL.
     * This might be necessary if we have replaced a page in the document.
     * We need to call this because otherwise looking at the page idx will fail.
     * See Doc::LayOut for an example.
     */
	void ResetDrawingPage( ) { m_drawingPage = NULL; };
    
    /**
     * Getter to the drawPage. Normally, getting the page should 
     * be done with Doc::SetDrawingPage. This is only a method for 
     * asserting that currently have the right page.
     */
    Page *GetDrawingPage( ) { return m_drawingPage; };
    
    /**
     * Return the width adjusted to the content of the current drawing page.
     * This includes the appropriate left and right margins.
     */
    int GetAdjustedDrawingPageWidth( );

    /**
     * Return the height adjusted to the content of the current drawing page.
     * This includes the appropriate top and bottom margin (using top as bottom).
     */
    int GetAdjustedDrawingPageHeight( );
        
    //----------//
    // Functors //
    //----------//
    
    /**
     * Functor for setting wordpos and connector ends
     * The functor is process by doc at the end of a document of closing opened syl.
     */
    virtual int PrepareLyricsEnd( ArrayPtrVoid *params );

private:
    /**
     * Calculates the music font size according to the m_interlDefin reference value.
     */
    int CalcMusicFontSize( );
    
public:
    /**
     * A copy of the header tree stored as pugi::xml_document
     */
    pugi::xml_document m_header;
    
    /**
     * Holds the top scoreDef.
     * In a standard MEI file, this is the <scoreDef> encoded before the first <section>.
     */
    ScoreDef m_scoreDef;
    
    /** The current page height */
    int m_drawingPageHeight;
    /** The current page height */
    int m_drawingPageWidth;
    /** The current page witdth */
    int m_drawingPageLeftMar;
    /** The current page left margin */
    int m_drawingPageRightMar;
    /** The current page right margin */
    int m_drawingPageTopMar;
    /** the current beam minimal slope */
	float m_drawingBeamMinSlope;
    /** the current beam maximal slope */
    float m_drawingBeamMaxSlope;
    /** flag for disabling justification */
    bool m_drawingJustifyX;
    /** flag for disabling spacing by duration */
    bool m_drawingEvenSpacing;
    /** value of the linear spacing factor */
    double m_drawingSpacingLinear;
    /** value of the non linear spacing factor */
    double m_drawingSpacingNonLinear;
    /** minimum measure width */
    int m_drawingMinMeasureWidth;
    
private:
    /**
     * The type of document indicates how to deal with the layout information.
     * A Transcription document type means that the layout information is included
     * and that no layout algorithm should be applied.
     */
    DocType m_type;
    
    /**
     * The object with the default values.
     * This could be saved somewhere as preferences (todo).
     */
    Style *m_style;
    
    /*
     * The following values are set in the Doc::SetDrawingPage.
     * They are all current values to be used when drawing a page in a View and
     * reset for every page. However, most of them are based on the m_staffDefin values
     * and will remain the same. This can be optimized.
     * The pages dimensions and margins are based on the page ones, the document ones or
     * the default in the following order and if available.
     */
    
    /** The page currently being drawn */
    Page *m_drawingPage;
    /** Half a the space between to staff lines */
    int m_drawingUnit;
    /** Space between to staff lines */
    int m_drawingDoubleUnit;
    /** Height of a five line staff */
    int m_drawingStaffSize;
    /** Height of an octave */
    int m_drawingOctaveSize;
    /** Height of a beam (10 and 6 by default) */
    int m_drawingBeamWidth;
    /** Height of a beam spacing (white) (10 and 6 by default) */
    int m_drawingBeamWhiteWidth;
    /** Ledger line length and normal and grace size */
    int m_drawingLedgerLine;
    /** Brevis width */
    int m_drawingBrevisWidth;
    
    /** Smufl font size (100 par defaut) */
    int m_drawingSmuflFontSize;
    /** Lyric font size  */
    int m_drawingLyricFontSize;
    /** Current music font */
    FontInfo m_drawingSmuflFont;
    /** Current lyric font */
    FontInfo m_drawingLyricFont;
    
    /**
     * A flag to indicate whether the currentScoreDef has been set or not.
     * If yes, SetCurrentScoreDef will not parse the document (again) unless
     * the force parameter is set.
     */
    bool m_currentScoreDefDone;
    
    /**
     * A flag to indicate if the drawing preparation has been done. If yes,
     * drawing preparation will be reset before being done again.
     */
    bool m_drawingPreparationDone;
    
    /** Page width (MEI scoredef@page.width) - currently not saved */
    int m_pageWidth;
    /** Page height (MEI scoredef@page.height) - currently not saved */
    int m_pageHeight;
    /** Page left margin (MEI scoredef@page.leftmar) - currently not saved */
    short m_pageLeftMar;
    /** Page right margin (MEI scoredef@page.rightmar) - currently not saved */
    short m_pageRightMar;
    /** Page top margin (MEI scoredef@page.topmar) - currently not saved */
    short m_pageTopMar;
    /** Staff minimal spacing (MEI scoredef@spacing.staff) - currently not saved */
    short m_spacingStaff;
    /** System minimal spacing (MEI scoredef@spacing.system) - currently not saved */
    short m_spacingSystem;
    
	
};

} // namespace vrv

#endif

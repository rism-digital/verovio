/////////////////////////////////////////////////////////////////////////////
// Name:        doc.h
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_DOC_H__
#define __VRV_DOC_H__

#include "vrv.h"
#include "devicecontext.h"
#include "vrvdef.h"
#include "io.h"
#include "object.h"
#include "scoredef.h"

namespace vrv {

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
     * @name Setters for the page dimensions and margins
     */
    ///@{
    void SetPageHeight( int pageHeight ) { m_pageHeight = pageHeight; };
    void SetPageWidth( int pageWidth ) { m_pageWidth = pageWidth; };
    void SetPageLeftMar( short pageLeftMar ) { m_pageLeftMar = pageLeftMar; };
    void SetPageRightMar( short pageRightMar ) { m_pageRightMar = pageRightMar; };
    void SetPageTopMar( short pageTopMar ) { m_pageTopMar = pageTopMar; };
    void SetSpacingStaff( short spacingStaff ) { m_spacingStaff = spacingStaff; };
    void SetSpacingSystem( short spacingSystem ) { m_spacingSystem = spacingSystem; };
    ///@}

    /**
     * @name Getters for the page dimensions and margins
     */
    ///@{
    short GetSpacingStaff( ) { return m_spacingStaff; };
    short GetSpacingSystem( ) { return m_spacingSystem; };
    ///@}
    
    /**
     * Saves the document using the specified output stream.
     * Creates a functors that will parse the full tree.
     */
    virtual int Save( FileOutputStream *output );

    /**
     * Set the initial scoreDef of each page
     * This is necessary for integrating changes that occur within a page.
     * It uses the MusObject::SetPageScoreDef functor method for parsing the file.
     * This will be done only if m_currentScoreDefDone is false or force is true.
     */
     void SetCurrentScoreDef( bool force = false );
    
    /**
     * Performs the layout of the entire document
     */
    void Layout( );
    
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
    
    /**
     * Initializes fonts from the resources (music and lyrics)
     * The method is called when the doc is reset.
     */
	virtual void UpdateFontValues();
        
    //----------//
    // Functors //
    //----------//
    
    /**
     * Functor method for saving the page
     */
    virtual int Save( ArrayPtrVoid params );
    
private:
    /**
     * Calculates the Leipzig font size according to the m_interlDefin reference value.
     * The size is calculated using  LEIPZIG_UNITS_PER_EM and LEIPZIG_WHOLE_NOTE_HEAD_HEIGHT.
     * See vrvdef.h for more information about these values.
     */
    int CalcLeipzigFontSize( );
    
public:
    

    /** 
     * The full name of the file. Exact use to be clarified.
     */
    std::string m_fname;
    
    /**
     * The object with the default values.
     * This could be saved somewhere as preferences (todo).
     */
    DefaultEnv m_env;
    
    /** 
     * The source id. Exact use to be clarified.
     */
    std::string m_source;
    
    /**
     * Holds the top scoreDef.
     * In a standard MEI file, this is the <scoreDef> encoded before the first <section>.
     */
    ScoreDef m_scoreDef;
    
    /*
     * The following values are set in the Doc::SetRenderedPage.
     * They are all current values to be used when drawing a page in a View and
     * reset for every page. However, most of them are based on the m_staffDefin values
     * and will remain the same. This can be optimized.
     * The pages dimensions and margins are based on the page ones, the document ones or 
     * the default in the following order and if available.
     */
  
    /** The page currently being drawn */
    Page *m_drawingPage;
    /** Editor step (10 by default) */
    int m_drawingStep1;
    /** Editor medium step (3 * m_drawingStep1) */
    int m_drawingStep2;
    /** Editor large step (6 * m_drawingStep1) */
    int m_drawingStep3;
    /** Half a the space between to staff lines for normal and small staff (10 and 8 by default) */
    int m_drawingHalfInterl[2];
    /** Space between to staff lines for normal and small staff (20 and 16 by default) */
    int m_drawingInterl[2];
    /** Height of a five line staff for normal and small staff (80 and 64 by default) */
    int m_drawingStaffSize[2];
    /** Height of an octave for normal and small staff (70 and 56 by default) */
    int m_drawingOctaveSize[2];
    /** Font height (100 par defaut) */
    int m_drawingFontHeight;
    /** Font height with ascent for normal and small staff and normal and grace size */
	int m_drawingFontHeightAscent[2][2];
    /** Normal and small staff ration (16 / 20 by default) */
    int m_drawingSmallStaffRatio[2];
    /** Normal and grace size (3 / 4 by default) */
    int m_drawingGraceRatio[2];
    /** Height of a beam for normal and small staff (10 and 6 by default) */
    int m_drawingBeamWidth[2];
    /** Height of a beam spacing (white) for normal and small staff (10 and 6 by default) */
    int m_drawingBeamWhiteWidth[2];
    /** Current font height with ascent for normal and small staff and normal and grace size */
    int m_drawingFontSize[2][2];
    /** Note radius for normal and small staff and normal and grace size */
    int m_drawingNoteRadius[2][2];
    /** Ledger line length for normal and small staff and normal and grace size */
    int m_drawingLedgerLine[2][3];
    /** Brevis width for normal and small staff */
    int m_drawingBrevisWidth[2];
    /** Accident width for normal and small staff and normal and grace size */
    int m_drawingAccidWidth[2][2];
    /** A vertical unit corresponding to the 1/4 of an interline */
    float m_drawingVerticalUnit1[2];
    /** A vertical unit corresponding to the 1/8 of an interline */
    float m_drawingVerticalUnit2[2];
    /** Current Leipzig font for normal and small staff and normal and grace size */
    FontMetricsInfo m_drawingFonts[2][2];				
    /** Leipzig font */
    FontMetricsInfo m_drawingLeipzigFont;
    /** Current lyric font for normal and small staff and normal and grace size */
    FontMetricsInfo m_drawingLyricFonts[2];
    /** Lyric font by default */
    FontMetricsInfo m_drawingLyricFont;
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
    
private:
    /**
     * The type of document indicates how to deal with the layout information.
     * A Transcription document types means that the layout information is included
     * and that no layout algorithm should be applied.
     */
    DocType m_type;
    
    /**
     * A flag for indicating whether the currentScoreDef has been set or not
     * If yes, SetCurrentScoreDef will not parse the document (again) unless
     * the force parameter is set.
     */
    bool m_currentScoreDefDone;
    
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

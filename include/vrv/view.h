/////////////////////////////////////////////////////////////////////////////
// Name:        view.h
// Author:      Laurent Pugin
// Created:     2010
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_RENDERER_H__
#define __VRV_RENDERER_H__

#include <typeinfo>

#include "devicecontext.h"
#include "vrvdef.h"
#include "scoredef.h"

namespace vrv {

class Beam;
class Barline;
class Doc;
class Layer;
class LayerElement;
class Measure;
class Page;
class Staff;
class System;
class Tie;
class Tuplet;

//----------------------------------------------------------------------------
// View
//----------------------------------------------------------------------------

/** 
 * This class is a rendering context and corresponds to the view of a MVC design pattern.
 */
class View
{
public:
    // constructors and destructors
    View();
	virtual ~View();

	// edition
    virtual void OnBeginEdition() {}
    virtual void OnEndEdition() {}
    virtual void OnBeginEditionClef() {}
    virtual void OnEndEditionClef() {}
    virtual void DoRefresh() {}
    virtual void DoResize() {}
    virtual void DoLyricCursor( int x, int y, DeviceContext *dc, std::string lyric ) {}
    virtual void DoReset() {}
    virtual void OnPageChange() {};

	// navigation
	void Next( bool forward );
	bool HasNext( bool forward );
	void LoadPage( int nopage );
	
	// simply returns the value of the last note-type element (mensural or neume)
	bool GetNotationMode();
	
	// convenience method that should be changed after refactoring
	bool IsNoteSelected();
	    
    void SetDoc( Doc *doc );
    
    /** x value in the Renderer */
	int ToRendererX( int i );
	/** x value in the Logical world */
	int ToLogicalX( int i );
	/** y value in the Renderer */
	int ToRendererY( int i );
	/** y value in the Logical world  */
	int ToLogicalY( int i );
	
	static void SwapY( int *y1, int *y2 ) { int tmp = *y1; *y1 = *y2; *y2 = tmp; }
    
    /**
     * Set the current page to pageIdx.
     * If doLayout is true, the layout of the page will be calculated.
     * This is the default behavior, however, in some cases, we do not
     * want it. For example, when drawing the pages for getting the bounding boxes
     */
    void SetPage( int pageIdx, bool doLayout = true );

	/* view_graph.cpp */
	void v_bline ( DeviceContext *dc, int y1, int y2, int x1, int nbr);
	void v_bline2 ( DeviceContext *dc, int y1, int y2, int x1, int nbr);
	void h_bline ( DeviceContext *dc, int x1, int x2, int y1, int nbr);
	void festa_string ( DeviceContext *dc, int x, int y, const std::string str, 
					   Staff *staff, int dimin ); 
	void DrawLeipzigFont ( DeviceContext *dc, int x, int y, unsigned char c, 
				  Staff *staff, bool dimin );
    void DrawTieBezier(DeviceContext *dc, int x, int y, int x1, bool direction);
	//void putfontfast ( DeviceContext *dc, int x, int y, unsigned char c );
	void putstring ( DeviceContext *dc, int x, int y, std::string s, int centrer, int staffSize = 0);
	void putlyric ( DeviceContext *dc, int x, int y, std::string s, int staffSize = 0, bool cursor = false);
	void box( DeviceContext *dc, int x1, int y1, int x2, int y2);
	void rect_plein2( DeviceContext *dc, int x1, int y1, int x2, int y2);
	int hGrosseligne ( DeviceContext *dc, int x1, int y1, int x2, int y2, int decal);
	int DoDrawDot ( DeviceContext *dc, int x, int y );
	
    /* view_bezier.cpp */
	static int CC(int ,int );
	static long BBlend(int ,int ,long );
	static int InitBezier(int );
	static void Bezier(long *,long *,long ,int );
	static void calcBez ( MusPoint *ptcoord, int _nbint );
	static void pntswap (MusPoint *x1, MusPoint *x2);
    
    /* view_page.cpp */
	void DrawCurrentPage( DeviceContext *dc, bool background = true );
    void DrawSystem( DeviceContext *dc, System *system );
	void DrawScoreDef( DeviceContext *dc, ScoreDef *scoreDef, Measure *measure, int x, Barline *barline = NULL );
	void DrawStaffGrp( DeviceContext *dc, Measure *measure, StaffGrp *staffGrp, int x );
	void DrawBracket ( DeviceContext *dc, int x, int y1, int y2, int staffSize);
	void DrawBrace ( DeviceContext *dc, int x, int y1, int y2, int staffSize);
    void DrawBarlines( DeviceContext *dc, Measure *measure, StaffGrp *staffGrp, int x, Barline *barline );
    void DrawBarline( DeviceContext *dc, int x, int y_top, int y_bottom, Barline *barline );
	void DrawBarlineDots ( DeviceContext *dc, int x, Staff *staff, Barline *barline );
	void DrawPartialBarline ( DeviceContext *dc, System *system, int x, Staff *pportee);
    void DrawMeasure( DeviceContext *dc, Measure *measure, System *system );
    void DrawStaff( DeviceContext *dc, Staff *staff, Measure *measure, System *system );
	void DrawStaffLines( DeviceContext *dc, Staff *staff, Measure *measure, System *system );
    int CalculatePitchPosY ( Staff *staff, char pname, int dec_clef, int oct);
	int CalculateNeumePosY ( Staff *staff, char note, int dec_clef, int oct);
    int CalculateRestPosY ( Staff *staff, char duration);
    void DrawLayer( DeviceContext *dc, Layer *layer, Staff *staff,  Measure *measure );
    void DrawLayerList( DeviceContext *dc, Layer *layer, Staff *staff, Measure *measure, const std::type_info *elementType );
	void DrawSlur( DeviceContext *dc, Layer *layer, int x1, int y1, int x2, int y2, bool up, int height = -1);
    int CalculatePitchCode ( Layer *layer, int y_n, int x_pos, int *octave );
    
    /* view_element.cpp */
    /** @name Methods for drawing LayerElement containing other elements */
    ///@{
    void DrawElement( DeviceContext *dc, LayerElement *element, Layer *layer, Measure *measure, Staff *staff );
    void DrawBeamElement(DeviceContext *dc, LayerElement *element, Layer *layer, Measure *measure, Staff *staff);
    void DrawTupletElement( DeviceContext *dc, LayerElement *element, Layer *layer, Measure *measure, Staff *staff );
    void DrawLayerApp( DeviceContext *dc, LayerElement *element, Layer *layer, Measure *measure, Staff *staff );
    ///@}
    
    void DrawDurationElement( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff );
    void DrawBarline( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff );  
    void DrawClef( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff );
    void DrawMensur( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff );
    void DrawFermata(DeviceContext *dc, LayerElement *element, Staff *staff);
    void DrawMensurCircle( DeviceContext *dc, int x, int yy, Staff *staff );
    void DrawMensurDot( DeviceContext *dc, int x, int yy, Staff *staff ); 
    void DrawMensurFigures( DeviceContext *dc, int x, int y, int num, int numBase, Staff *staff); 
    void DrawMensurHalfCircle( DeviceContext *dc, int x, int yy, Staff *staff );
    void DrawMensurReversedHalfCircle( DeviceContext *dc, int x, int yy, Staff *staff ); 
    void DrawMensurSlash( DeviceContext *dc, int x, int yy, Staff *staff );
    void DrawMRest( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure );
    void DrawMultiRest( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure );
    void DrawNote( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff );  
    void DrawRest( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff );
    void DrawSymbol( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, LayerElement *parent = NULL );
    void DrawSymbolAccid( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff );
    void DrawSymbolCustos( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff );
    void DrawSymbolDot( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff );
    void DrawTie( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure );
    void DrawTuplet( DeviceContext *dc, Tuplet *tuplet, Layer *layer, Staff *staff);
    void DrawTrill(DeviceContext *dc, LayerElement *element, Staff *staff);
    void DrawLigature( DeviceContext *dc, int y, LayerElement *element, Layer *layer, Staff *staff );  
    void DrawLedgerLines( DeviceContext *dc, int y_n, int y_p, int xn, unsigned int smaller, int staffSize);
    void DrawLongRest ( DeviceContext *dc, int a, int b, Staff *staff);
    void DrawBreveRest ( DeviceContext *dc, int a, int b, Staff *staff);
    void DrawWholeRest ( DeviceContext *dc, int a, int b, int valeur, unsigned char dots, unsigned int smaller, Staff *staff);
    void DrawQuarterRest ( DeviceContext *dc, int a, int b, int valeur, unsigned char dots, unsigned int smaller, Staff *staff);
    void DrawDots ( DeviceContext *dc, int x1, int y1, int offy, unsigned char dots, Staff *staff );
    void CalculateLigaturePosX ( LayerElement *element, Layer *layer, Staff *staff);
    void DrawAcciaccaturaSlash(DeviceContext *dc, LayerElement *element);
    void DrawKeySig( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff );
    
    /* view_beam.cpp */
    void DrawBeam(  DeviceContext *dc, Layer *layer, Beam *beam, Staff *staff );
    
private:
    
    //Used for calculating tuplets
    bool GetTupletCoordinates(Tuplet* tuplet, Layer *layer, MusPoint* start, MusPoint* end, MusPoint *center);
    std::string IntToObliqueFigures(unsigned int number);
    bool OneBeamInTuplet(Tuplet* tuplet);
    
public:
    /** Document */
    Doc *m_doc;
    /** Index of the current page */
    int m_pageIdx;

	std::string m_str;
    
	// static
	static MusPoint point_[4];
	static MusPoint bcoord[2*(BEZIER_NB_POINTS+1)];

    int m_currentColour;
    
    // element currently selected
	LayerElement *m_currentElement;
    Layer *m_currentLayer;
    Measure *m_currentMeasure;
	Staff *m_currentStaff;
    System *m_currentSystem;
    Page *m_currentPage;
    
	int m_notation_mode; // neumes or mensural notation mode
	bool m_lyricMode;
	bool m_inputLyric;
	EditorMode m_editorMode; // Edit or insert
    
private:

    // static for ligatures
    static int s_drawingLigX[2], s_drawingLigY[2];	// pour garder coord. des ligatures    
    static bool s_drawingLigObliqua;	// marque le 1e passage pour une oblique

    ScoreDef m_drawingScoreDef;
    

};

} // namespace vrv

#endif

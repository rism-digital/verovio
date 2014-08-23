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
#include "scoredef.h"
#include "vrvdef.h"

namespace vrv {

class Beam;
class Barline;
class Doc;
class Layer;
class LayerElement;
class Measure;
class Page;
class Staff;
class Syl;
class System;
class Tie;
class Tuplet;
class Verse;


//----------------------------------------------------------------------------
// View
//----------------------------------------------------------------------------

/** 
 * This class is a drawing context and corresponds to the view of a MVC design pattern.
 */
class View
{
public:
    /** Constructors and destructors */
    ///@{
    View();
	virtual ~View();
    ///@}

    /**
     * @name Virtual methods that are triggered when necessary but the do nothing in 
     * the View class. The can be overriden when necessary in the child classses.
     */
    ///@{
    virtual void OnBeginEdition() {}
    virtual void OnEndEdition() {}
    virtual void OnBeginEditionClef() {}
    virtual void OnEndEditionClef() {}
    virtual void DoRefresh() {}
    virtual void DoResize() {}
    virtual void DoLyricCursor( int x, int y, DeviceContext *dc, std::string lyric ) {}
    virtual void DoReset() {}
    virtual void OnPageChange() {};
    ///@}

    /**
     * @name Navigation methods for changing the page in the view.
     * Navigating will check that the page exists in the document and also set it
     * by calling SetPage (with doLayout = true);
     */
	void Next( bool forward );
	bool HasNext( bool forward );
    ///@}
	
	/**
     * Simply returns the value of the last note-type element (mensural or neume)
     */
	bool GetNotationMode();

    /**
     * Set the document the view is pointing to (mandatory).
     * Several view can point to the same document.
     */
    void SetDoc( Doc *doc );
    
    /** 
     * @name Methods for converting coordinates from the logical world to the device
     * context world.
     * X coordinates are identical in both worlds. Y coordinates are from bottom to top
     * in the logical world and from top to bottom in the device context world.
     */
    ///@{
	int ToDeviceContextX( int i );
	int ToLogicalX( int i );
	int ToDeviceContextY( int i );
	int ToLogicalY( int i );
    ///@}
    
    /**
     * Set the current page to pageIdx.
     * If doLayout is true, the layout of the page will be calculated.
     * This is the default behavior, however, in some cases, we do not
     * want it. For example, when drawing the pages for getting the bounding boxes.
     */
    void SetPage( int pageIdx, bool doLayout = true );
    
    /**  
     * Method that actually draw the current page.
     * This is the only drawing method that is public and that can be called for drawing.
     * The method also takes care of setting the drawing page of the document by calling 
     * Doc::SetDrawingPage. It means that we have different views, each view can have a different
     * current page and it will still work properly.
     * Defined in view_page.cpp
     */
	void DrawCurrentPage( DeviceContext *dc, bool background = true );
    
    
protected:
    /** 
     * @name Methods for drawing System, ScoreDef, StaffDef, Staff, and Layer.
     * Additional methods for drawing braces, barlines, slurs, etc.
     * Defined in view_page.cpp
     */
    ///@{
    void DrawSystem( DeviceContext *dc, System *system );
	void DrawScoreDef( DeviceContext *dc, ScoreDef *scoreDef, Measure *measure, int x, Barline *barline = NULL );
	void DrawStaffGrp( DeviceContext *dc, Measure *measure, StaffGrp *staffGrp, int x );
	void DrawBracket ( DeviceContext *dc, int x, int y1, int y2, int staffSize);
	void DrawBrace ( DeviceContext *dc, int x, int y1, int y2, int staffSize);
    void DrawBarlines( DeviceContext *dc, Measure *measure, StaffGrp *staffGrp, Barline *barline );
    void DrawBarline( DeviceContext *dc, int y_top, int y_bottom, Barline *barline );
	void DrawBarlineDots ( DeviceContext *dc, StaffDef *staffDef, Staff *staff, Barline *barline );
	void DrawPartialBarline ( DeviceContext *dc, System *system, int x, Staff *pportee);
    void DrawMeasure( DeviceContext *dc, Measure *measure, System *system );
    void DrawStaff( DeviceContext *dc, Staff *staff, Measure *measure, System *system );
	void DrawStaffLines( DeviceContext *dc, Staff *staff, Measure *measure, System *system );
    void DrawLayer( DeviceContext *dc, Layer *layer, Staff *staff,  Measure *measure );
    void DrawLayerList( DeviceContext *dc, Layer *layer, Staff *staff, Measure *measure, const std::type_info *elementType );
	void DrawSlur( DeviceContext *dc, Layer *layer, int x1, int y1, int x2, int y2, bool up, int height = -1);
    ///@}
    
    /**
     * @name Methods for calculating drawing positions
     * Defined in view_element.cpp
     */
    ///@{
    int CalculatePitchPosY ( Staff *staff, char pname, int dec_clef, int oct);
    int CalculateRestPosY ( Staff *staff, char duration);
    int CalculatePitchCode ( Layer *layer, int y_n, int x_pos, int *octave );
    ///@}
    
    /** 
     * @name Methods for drawing LayerElement containing other elements.
     * This is the case of Beam, Tuplet, Chords, etc.
     * Defined in view_element.cpp
     */
    ///@{
    void DrawElement( DeviceContext *dc, LayerElement *element, Layer *layer, Measure *measure, Staff *staff );
    void DrawBeamElement(DeviceContext *dc, LayerElement *element, Layer *layer, Measure *measure, Staff *staff);
    void DrawTupletElement( DeviceContext *dc, LayerElement *element, Layer *layer, Measure *measure, Staff *staff );
    void DrawLayerApp( DeviceContext *dc, LayerElement *element, Layer *layer, Measure *measure, Staff *staff );
    ///@}
    
    /**
     * @name Methods for drawing LayerElement child classes.
     * Defined in view_element.cpp
     */
    ///@{
    void DrawAcciaccaturaSlash(DeviceContext *dc, LayerElement *element);
    void DrawBarline( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff );
    void DrawBreveRest ( DeviceContext *dc, int x, int y, Staff *staff);
    void DrawClef( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff );
    void DrawDots ( DeviceContext *dc, int x, int y, unsigned char dots, Staff *staff );
    void DrawDurationElement( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff );
    void DrawFermata(DeviceContext *dc, LayerElement *element, Staff *staff);
    void DrawKeySig( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff );
    void DrawLigature( DeviceContext *dc, int y, LayerElement *element, Layer *layer, Staff *staff );
    void DrawLedgerLines( DeviceContext *dc, int y_n, int y_p, int xn, unsigned int smaller, int staffSize);
    void DrawLongRest ( DeviceContext *dc, int x, int y, Staff *staff);
    void DrawMensur( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff );
    void DrawMensurCircle( DeviceContext *dc, int x, int yy, Staff *staff );
    void DrawMensurDot( DeviceContext *dc, int x, int yy, Staff *staff );
    void DrawMensurFigures( DeviceContext *dc, int x, int y, int num, int numBase, Staff *staff); 
    void DrawMensurHalfCircle( DeviceContext *dc, int x, int yy, Staff *staff );
    void DrawMensurReversedHalfCircle( DeviceContext *dc, int x, int yy, Staff *staff ); 
    void DrawMensurSlash( DeviceContext *dc, int x, int yy, Staff *staff );
    void DrawMeterSig( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff );
    void DrawMRest( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure );
    void DrawMultiRest( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure );
    void DrawNote( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff );  
    void DrawQuarterRest ( DeviceContext *dc, int x, int y, int valeur, unsigned char dots, unsigned int smaller, Staff *staff);
    void DrawRest( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff );
    void DrawSyl( DeviceContext *dc, Syl *syl, int verseNb, LayerElement *element, Layer *layer, Staff *staff);
    void DrawSymbol( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, LayerElement *parent = NULL );
    void DrawSymbolAccid( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff );
    void DrawSymbolCustos( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff );
    void DrawSymbolDot( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff );
    void DrawTie( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure );
    void DrawTrill(DeviceContext *dc, LayerElement *element, Staff *staff);
    void DrawTuplet( DeviceContext *dc, Tuplet *tuplet, Layer *layer, Staff *staff);
    void DrawVerse( DeviceContext *dc, Verse *verse, LayerElement *element, Layer *layer, Staff *staff);
    void DrawWholeRest ( DeviceContext *dc, int x, int y, int valeur, unsigned char dots, unsigned int smaller, Staff *staff);
    void CalculateLigaturePosX ( LayerElement *element, Layer *layer, Staff *staff);
    ///@}
    
    /**
     * @name Method for drawing Beam.
     * Wolfgang legacy code to be redesigned.
     * Defined in view_beam.cpp
     */
    ///@{
    void DrawBeam(  DeviceContext *dc, Layer *layer, Beam *beam, Staff *staff );
    ///@}

    /**
     * @name Low level drawing methods
     * Defined in view_graph.cpp
     */
    ///@{
	void DrawVerticalLine ( DeviceContext *dc, int y1, int y2, int x1, int nbr);
	void DrawHorizontalLine ( DeviceContext *dc, int x1, int x2, int y1, int nbr);
	void DrawLeipzigFont ( DeviceContext *dc, int x, int y, unsigned char c, Staff *staff, bool dimin );
    void DrawTieOrSlurBezier(DeviceContext *dc, int x, int y, int x1, int y1, bool direction);
	void DrawLeipzigString ( DeviceContext *dc, int x, int y, std::string s, int centrer, int staffSize = 0);
	void DrawLyricString ( DeviceContext *dc, int x, int y, std::string s, int staffSize = 0, bool cursor = false);
	void DrawFullRectangle( DeviceContext *dc, int x1, int y1, int x2, int y2);
	void DrawObliqueLine ( DeviceContext *dc, int x1, int y1, int x2, int y2, int decal);
	void DrawDot ( DeviceContext *dc, int x, int y );
    ///@}
    
private:    
    /**
     * @name Internal methods used for calculating tuplets
     */
    ///@{
    bool GetTupletCoordinates(Tuplet* tuplet, Layer *layer, MusPoint* start, MusPoint* end, MusPoint *center);
    std::string IntToObliqueFigures(unsigned short number);
    bool OneBeamInTuplet(Tuplet* tuplet);
    ///@}
    
    /**
     * Swap the to points passed as reference.
     * This is useful for example when calculating bezier positions.
     */
    static void SwapPoints (MusPoint *x1, MusPoint *x2);
    
    /**
     * Swap values passed as reference.
     * This is useful for example when switching to the device context world.
     */
	static void SwapY( int *y1, int *y2 ) { int tmp = *y1; *y1 = *y2; *y2 = tmp; }
    
public:
    /** Document */
    Doc *m_doc;
    /** Index of the current page */
    int m_pageIdx;
    
    /**
     * @name The objects currently selected.
     * To be used in child classes, but defined here because it can be
     * useful for changing the color, for example
     */
    ///@{
	LayerElement *m_currentElement;
    Layer *m_currentLayer;
    Measure *m_currentMeasure;
	Staff *m_currentStaff;
    System *m_currentSystem;
    Page *m_currentPage;
    ///@}
    
    /** The notation mode (CMN, Mensural) */
	int m_notationMode;
    /** The editor mode, which can be Edit or Insert */
	EditorMode m_editorMode;
    
protected:
    /** 
     * The colour currently being used when drawing.
     * It can change when drawing the m_currentElement, for example
     */
    int m_currentColour;
    
    /**
     * The current drawing score def.
     * The is set when starting to draw a page in DrawCurrentPage and then
     * modified appropriately when going through the page.
     */
    ScoreDef m_drawingScoreDef;
    
private:

    /** @name Internal values for storing temporary values for ligatures */
    ///@{
    static int s_drawingLigX[2], s_drawingLigY[2];   
    static bool s_drawingLigObliqua;
    ///@}

};

} // namespace vrv

#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        view.h
// Author:      Laurent Pugin
// Created:     2010
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_RENDERER_H__
#define __VRV_RENDERER_H__

#include <typeinfo>

//----------------------------------------------------------------------------

#include "devicecontextbase.h"
#include "scoredef.h"
#include "vrvdef.h"

namespace vrv {

class Accid;
class Beam;
class Barline;
class Chord;
class DeviceContext;
class Doc;
class EditorialElement;
class Layer;
class LayerElement;
class Measure;
class MeasureElement;
class Page;
class Slur;
class Staff;
class Syl;
class System;
class Tie;
class Tuplet;
class Verse;


enum  {
    SPANNING_START_END = 0,
    SPANNING_START,
    SPANNING_END,
    SPANNING_MIDDLE
};

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
    
    /**
     * @name Methods for calculating drawing positions
     * Defined in view_element.cpp
     */
    ///@{
    int CalculatePitchPosY ( Staff *staff, char pname, int dec_clef, int oct);
    int CalculateRestPosY ( Staff *staff, char duration);
    int CalculatePitchCode ( Layer *layer, int y_n, int x_pos, int *octave );
    ///@}
    
protected:
    /** 
     * @name Methods for drawing System, ScoreDef, StaffDef, Staff, and Layer.
     * Additional methods for drawing braces, barlines, slurs, etc.
     * Defined in view_page.cpp
     */
    ///@{
    void DrawSystem( DeviceContext *dc, System *system );
    void DrawSystemList( DeviceContext *dc, System *system, const std::type_info *elementType );
	void DrawScoreDef( DeviceContext *dc, ScoreDef *scoreDef, Measure *measure, int x, Barline *barLine = NULL );
    void DrawStaffGrp( DeviceContext *dc, Measure *measure, StaffGrp *staffGrp, int x, bool topStaffGrp = false );
    void DrawStaffDefLabels( DeviceContext *dc, Measure *measure, ScoreDef *scoreDef, bool abbreviations = false );
	void DrawBracket ( DeviceContext *dc, int x, int y1, int y2, int staffSize);
	void DrawBrace ( DeviceContext *dc, int x, int y1, int y2, int staffSize);
    void DrawBarlines( DeviceContext *dc, Measure *measure, StaffGrp *staffGrp, Barline *barLine );
    void DrawBarline( DeviceContext *dc, int y_top, int y_bottom, Barline *barLine );
	void DrawBarlineDots ( DeviceContext *dc, StaffDef *staffDef, Staff *staff, Barline *barLine );
	void DrawPartialBarline ( DeviceContext *dc, System *system, int x, Staff *pportee);
    void DrawMeasure( DeviceContext *dc, Measure *measure, System *system );
    void DrawStaff( DeviceContext *dc, Staff *staff, Measure *measure, System *system );
	void DrawStaffLines( DeviceContext *dc, Staff *staff, Measure *measure, System *system );
    void DrawLayer( DeviceContext *dc, Layer *layer, Staff *staff,  Measure *measure );
    void DrawLayerList( DeviceContext *dc, Layer *layer, Staff *staff, Measure *measure, const std::type_info *elementType );
	void DrawSlur( DeviceContext *dc, Layer *layer, int x1, int y1, int x2, int y2, bool up, int height = -1);
    ///@}
    
    /**
     * @name Methods for drawing children
     * Defined in view_page.cpp
     * For each method, we have a parent Object, that can be either the same as the next paremeter,
     * or an intermediate node in the hierarchy. For example, we can draw the system children from the 
     * system itself (in that case, parent will be the same as system) or from an editorial element
     * occuring in between.
     */
    ///@{
    void DrawSystemChildren( DeviceContext *dc, Object *parent, System *system );
    void DrawMeasureChildren( DeviceContext *dc, Object *parent, Measure *measure, System *system );
    void DrawStaffChildren( DeviceContext *dc, Object *parent, Staff *staff,  Measure *measure );
    void DrawLayerChildren( DeviceContext *dc, Object *parent, Layer *layer, Staff *staff, Measure *measure );
    ///@}
    
    /**
     * @name Methods for drawing EditorialElement object at different levels
     * Defined in view_page.cpp
     */
    ///@{
    void DrawSystemEditorialElement( DeviceContext *dc, EditorialElement *element, System *system );
    void DrawMeasureEditorialElement( DeviceContext *dc, EditorialElement *element, Measure *measure, System *system );
    void DrawStaffEditorialElement( DeviceContext *dc, EditorialElement *element, Staff *staff,  Measure *measure );
    void DrawLayerEditorialElement( DeviceContext *dc, EditorialElement *element, Layer *layer, Staff *staff, Measure *measure );
    ///@}
    
    /**
     * @name Top level method for drawing MeasureElement.
     * Call appropriate method of child classes (Staff, Slur, etc).
     * Defined in page_element.cpp
     */
    ///@{
    void DrawMeasureElement( DeviceContext *dc, MeasureElement *element, Measure *measure, System *system );
    ///@}
    
    /**
     * @name Methods for drawing MeasureElement child classes.
     * They are base drawing methods that are called directly from DrawMeasureElement
     * Defined in view_page.cpp
     */
    ///@{
    void DrawTimeSpanningElement( DeviceContext *dc, DocObject *object, System *system );
    void DrawTieOrSlur( DeviceContext *dc, MeasureElement *element, int x1, int x2,
                  Staff *staff, char spanningType, DocObject *graphic = NULL );
    void DrawSylConnector( DeviceContext *dc, Syl *syl, int x1, int x2,
                          Staff *staff, char spanningType, DocObject *graphic = NULL );
    ///@}
    
    /** 
     * @name Top level method for drawing LayerElement.
     * This can be called recursively for elements containing other elements.
     * This the case for Note, Beam, Tuplet, Chords, etc.
     * Defined in view_element.cpp
     */
    ///@{
    void DrawLayerElement( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure );
    ///@}
    
    /**
     * @name Methods for drawing LayerElement child classes.
     * They are base drawing methods that are called directly from DrawLayerElement
     * Because some elements draw their children recursively (e.g., Note) they must all
     * have the same parameters
     * Defined in view_element.cpp
     */
    ///@{
    void DrawAccid( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure, Accid* prevAccid = NULL );
    void DrawBeam(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure );
    void DrawBarline( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure );
    void DrawChord( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure );
    void DrawClef( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure );
    void DrawCustos( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure );
    void DrawDot( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure );
    void DrawDurationElement( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure );
    void DrawKeySig( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure  );
    void DrawMeterSig( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure  );
    void DrawMRest( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure );
    void DrawMultiRest( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure );
    void DrawNote( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure );
    void DrawRest( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure );
    void DrawSpace( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure );
    void DrawSyl( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure );
    void DrawTie( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure );
    void DrawTuplet( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure );
    void DrawVerse( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure );
    ///@}
    
    /**
     * @name Methods for drawing parts of LayerElement child classes.
     * They are sub-drawing methods that are called from the base drawing methods above.
     * The parameter set can be different from a base drawing method since no recursive call is expected
     * Defined in view_element.cpp
     */
    ///@{
    void DrawAcciaccaturaSlash(DeviceContext *dc, LayerElement *element);
    void DrawBreveRest ( DeviceContext *dc, int x, int y, Staff *staff );
    void DrawDots ( DeviceContext *dc, int x, int y, unsigned char dots, Staff *staff );
    void DrawFermata(DeviceContext *dc, LayerElement *element, Staff *staff);
    void DrawLedgerLines ( DeviceContext *dc, LayerElement *element, Staff *staff, bool aboveStaff, bool doubleLength, int skip, int n);
    void DrawLongRest ( DeviceContext *dc, int x, int y, Staff *staff);
    void DrawMeterSigFigures( DeviceContext *dc, int x, int y, int num, int numBase, Staff *staff);
    void DrawQuarterRest ( DeviceContext *dc, int x, int y, int valeur, unsigned char dots, unsigned int smaller, Staff *staff);
    void DrawStem( DeviceContext *dc, LayerElement *object, Staff *staff, data_STEMDIRECTION dir, int radius, int xn, int originY, int heightY = 0);
    void DrawSylConnector( DeviceContext *dc, Syl *syl, System *system );
    void DrawSylConnectorLines( DeviceContext *dc, int x1, int x2, int y, Syl *syl, Staff *staff );
    void DrawTrill(DeviceContext *dc, LayerElement *element, Staff *staff );
    void DrawWholeRest ( DeviceContext *dc, int x, int y, int valeur, unsigned char dots, unsigned int smaller, Staff *staff);
    ///@}
    
    /**
     * @name Methods for drawing mensural LayerElement child classes.
     * They are base drawing methods that are called directly from DrawLayerElement
     * Because some elements draw their children recursively (e.g., Note) they must all
     * have the same parameters
     * Defined in view_mensural.cpp
     */
    ///@{
    void DrawMensur( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure  );
    void DrawMensuralNote ( DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure );
    
    /**
     * @name Methods for drawing parts of mensural LayerElement child classes.
     * Defined in view_mensural.cpp
     */
    ///@{
    void DrawMensurCircle( DeviceContext *dc, int x, int yy, Staff *staff );
    void DrawMensurDot( DeviceContext *dc, int x, int yy, Staff *staff );
    void DrawMensurHalfCircle( DeviceContext *dc, int x, int yy, Staff *staff );
    void DrawMensurReversedHalfCircle( DeviceContext *dc, int x, int yy, Staff *staff );
    void DrawMensurSlash( DeviceContext *dc, int x, int yy, Staff *staff );
    void DrawMaximaToBrevis( DeviceContext *dc, int y, LayerElement *element, Layer *layer, Staff *staff );
    void DrawLigature( DeviceContext *dc, int y, LayerElement *element, Layer *layer, Staff *staff );
    void CalculateLigaturePosX ( LayerElement *element, Layer *layer, Staff *staff);
    ///@}
    
    /**
     * @name Method for drawing Beam.
     * Called from the the layer postponed drawing list.
     * Defined in view_tuplet.cpp
     */
    ///@{
    void DrawTupletPostponed( DeviceContext *dc, Tuplet *tuplet, Layer *layer, Staff *staff );

    /**
     * @name Low level drawing methods
     * Defined in view_graph.cpp
     */
    ///@{
	void DrawVerticalLine ( DeviceContext *dc, int y1, int y2, int x1, int nbr);
	void DrawHorizontalLine ( DeviceContext *dc, int x1, int x2, int y1, int nbr);
	void DrawSmuflCode ( DeviceContext *dc, int x, int y, wchar_t code, int staffSize, bool dimin );
    void DrawTieOrSlurBezier(DeviceContext *dc, int x, int y, int x1, int y1, bool direction);
	void DrawSmuflString ( DeviceContext *dc, int x, int y, std::wstring s, int centrer, int staffSize = 0);
	void DrawLyricString ( DeviceContext *dc, int x, int y, std::wstring s, int staffSize = 0);
	void DrawFullRectangle( DeviceContext *dc, int x1, int y1, int x2, int y2);
	void DrawObliquePolygon ( DeviceContext *dc, int x1, int y1, int x2, int y2, int height);
	void DrawDot ( DeviceContext *dc, int x, int y );
    ///@}
    
private:    
    /**
     * @name Internal methods used for calculating tuplets
     */
    ///@{
    bool GetTupletCoordinates(Tuplet* tuplet, Layer *layer, Point* start, Point* end, Point *center);
    std::wstring IntToTupletFigures(unsigned short number);
    std::wstring IntToTimeSigFigures(unsigned short number);
    std::wstring IntToSmuflFigures(unsigned short number, int offset);
    bool OneBeamInTuplet(Tuplet* tuplet);
    int GetSylY( Syl* syl, Staff *staff );
    ///@}
    
    /**
     * @name Used for calculating clustered information/dot position
     */
    bool IsOnStaffLine ( int y, Staff *staff );
    
    /**
     * Make sure dots on chords are vertically aligned correctly
     */
    void PrepareChordDots ( DeviceContext *dc, Chord *chord, int x, int y, unsigned char dots, Staff *staff );
    
    /**
     * Changes and/or calculates the horizontal alignment of accidentals to prevent overlapping
     */
    bool CalculateAccidX(Staff *staff, Accid *accid, Chord *chord, bool adjustHorizontally);
    
    /**
     * Swap the to points passed as reference.
     * This is useful for example when calculating bezier positions.
     */
    static void SwapPoints (Point *x1, Point *x2);
    
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
    ///@}m
    
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

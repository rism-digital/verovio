/////////////////////////////////////////////////////////////////////////////
// Name:        view.h
// Author:      Laurent Pugin
// Created:     2010
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_RENDERER_H__
#define __VRV_RENDERER_H__

#include "devicecontextbase.h"
#include "scoredef.h"
#include "vrvdef.h"

namespace vrv {

class Accid;
class BarLine;
class Beam;
class BeamParams;
class Chord;
class DeviceContext;
class Dir;
class Doc;
class Dynam;
class EditorialElement;
class Ending;
class Fermata;
class Hairpin;
class Harm;
class Layer;
class LayerElement;
class Measure;
class ControlElement;
class Octave;
class Page;
class Pedal;
class Rend;
class Slur;
class Staff;
class Syl;
class System;
class SystemElement;
class Tempo;
class Text;
class TextElement;
class Tie;
class Trill;
class Tuplet;
class Verse;

enum { SPANNING_START_END = 0, SPANNING_START, SPANNING_END, SPANNING_MIDDLE };

//----------------------------------------------------------------------------
// View
//----------------------------------------------------------------------------

/**
 * This class is a drawing context and corresponds to the view of a MVC design pattern.
 */
class View {
public:
    /** Constructors and destructors */
    ///@{
    View();
    virtual ~View();
    ///@}

    /**
     * @name Virtual methods that are triggered when necessary but they do nothing in
     * the View class. They can be overridden when necessary in the child classses.
     */
    ///@{
    virtual void OnBeginEdition() {}
    virtual void OnEndEdition() {}
    virtual void OnBeginEditionClef() {}
    virtual void OnEndEditionClef() {}
    virtual void DoRefresh() {}
    virtual void DoResize() {}
    virtual void DoReset() {}
    virtual void OnPageChange(){};
    ///@}

    /**
     * @name Navigation methods for changing the page in the view.
     * Navigating will check that the page exists in the document and also set it
     * by calling SetPage (with doLayout = true);
     */
    void Next(bool forward);
    bool HasNext(bool forward);
    ///@}

    /**
     * Simply returns the value of the last note-type element (mensural or neume)
     */
    bool GetNotationMode();

    /**
     * Set the document the view is pointing to (mandatory).
     * Several views can point to the same document.
     */
    void SetDoc(Doc *doc);

    /**
     * @name Methods for converting coordinates from the logical world to the device
     * context world.
     * X coordinates are identical in both worlds. Y coordinates are from bottom to top
     * in the logical world and from top to bottom in the device context world.
     */
    ///@{
    int ToDeviceContextX(int i);
    int ToLogicalX(int i);
    int ToDeviceContextY(int i);
    int ToLogicalY(int i);
    Point ToDeviceContext(Point p);
    Point ToLogical(Point p);
    ///@}

    /**
     * Set the current page to pageIdx.
     * If doLayout is true, the layout of the page will be calculated.
     * This is the default behavior, however, in some cases, we do not
     * want it. For example, when drawing the pages for getting the bounding boxes.
     */
    void SetPage(int pageIdx, bool doLayout = true);

    /**
     * Method that actually draw the current page.
     * This is the only drawing method that is public and that can be called for drawing.
     * The method also takes care of setting the drawing page of the document by calling
     * Doc::SetDrawingPage. It means that we have different views, each view can have a different
     * current page and it will still work properly.
     * Defined in view_page.cpp
     */
    void DrawCurrentPage(DeviceContext *dc, bool background = true);

    /**
     * @name Methods for calculating drawing positions
     * Defined in view_element.cpp
     */
    ///@{
    int CalculatePitchPosY(Staff *staff, data_PITCHNAME pname, int dec_clef, int oct);
    int CalculateRestPosY(Staff *staff, char duration, bool hasMultipleLayer, bool isFirstLayer);
    int CalculatePitchCode(Layer *layer, int y_n, int x_pos, int *octave);
    ///@}

protected:
    /**
     * @name Methods for drawing System, ScoreDef, StaffDef, Staff, and Layer.
     * Additional methods for drawing braces, barlines, slurs, etc.
     * Defined in view_page.cpp
     */
    ///@{
    void DrawSystem(DeviceContext *dc, System *system);
    void DrawSystemList(DeviceContext *dc, System *system, const ClassId classId);
    void DrawScoreDef(DeviceContext *dc, ScoreDef *scoreDef, Measure *measure, int x, BarLine *barLine = NULL);
    void DrawStaffGrp(DeviceContext *dc, Measure *measure, StaffGrp *staffGrp, int x, bool topStaffGrp = false,
        bool abbreviations = false);
    void DrawStaffDef(DeviceContext *dc, Staff *staff, Measure *measure);
    void DrawStaffDefCautionary(DeviceContext *dc, Staff *staff, Measure *measure);
    void DrawStaffDefLabels(DeviceContext *dc, Measure *measure, ScoreDef *scoreDef, bool abbreviations = false);
    void DrawBracket(DeviceContext *dc, int x, int y1, int y2, int staffSize);
    void DrawBrace(DeviceContext *dc, int x, int y1, int y2, int staffSize);
    void DrawBarLines(DeviceContext *dc, Measure *measure, StaffGrp *staffGrp, BarLine *barLine);
    void DrawBarLine(DeviceContext *dc, int y_top, int y_bottom, BarLine *barLine);
    void DrawBarLineDots(DeviceContext *dc, StaffDef *staffDef, Staff *staff, BarLine *barLine);
    void DrawMeasure(DeviceContext *dc, Measure *measure, System *system);
    void DrawStaff(DeviceContext *dc, Staff *staff, Measure *measure, System *system);
    void DrawStaffLines(DeviceContext *dc, Staff *staff, Measure *measure, System *system);
    void DrawLayer(DeviceContext *dc, Layer *layer, Staff *staff, Measure *measure);
    void DrawLayerList(DeviceContext *dc, Layer *layer, Staff *staff, Measure *measure, const ClassId classId);
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
    void DrawSystemChildren(DeviceContext *dc, Object *parent, System *system);
    void DrawMeasureChildren(DeviceContext *dc, Object *parent, Measure *measure, System *system);
    void DrawStaffChildren(DeviceContext *dc, Object *parent, Staff *staff, Measure *measure);
    void DrawLayerChildren(DeviceContext *dc, Object *parent, Layer *layer, Staff *staff, Measure *measure);
    void DrawTextChildren(DeviceContext *dc, Object *parent, int x, int y, bool &setX, bool &setY);
    ///@}

    /**
     * @name Methods for drawing EditorialElement object at different levels
     * Defined in view_page.cpp
     */
    ///@{
    void DrawSystemEditorialElement(DeviceContext *dc, EditorialElement *element, System *system);
    void DrawMeasureEditorialElement(DeviceContext *dc, EditorialElement *element, Measure *measure, System *system);
    void DrawStaffEditorialElement(DeviceContext *dc, EditorialElement *element, Staff *staff, Measure *measure);
    void DrawLayerEditorialElement(
        DeviceContext *dc, EditorialElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawTextEditorialElement(DeviceContext *dc, EditorialElement *element, int x, int y, bool &setX, bool &setY);
    ///@}

    /**
     * @name Top level method for drawing LayerElement.
     * This can be called recursively for elements containing other elements.
     * This the case for Note, Beam, Tuplet, Chords, etc.
     * Defined in view_element.cpp
     */
    ///@{
    void DrawLayerElement(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    ///@}

    /**
     * @name Methods for drawing LayerElement child classes.
     * They are base drawing methods that are called directly from DrawLayerElement
     * Because some elements draw their children recursively (e.g., Note) they must all
     * have the same parameters.
     * Defined in view_element.cpp
     */
    ///@{
    void DrawAccid(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure,
        Accid *prevAccid = NULL);
    void DrawArtic(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure,
        bool drawingList = false);
    void DrawArticPart(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawBarLine(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawBeatRpt(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawBTrem(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawChord(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawClef(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawCustos(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawDot(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawDurationElement(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawKeySig(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawLigature(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawMeterSig(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawMRest(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawMRpt(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawMRpt2(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawMultiRest(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawMultiRpt(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawNote(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawRest(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawSpace(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawSyl(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawTuplet(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawVerse(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    ///@}

    /**
     * @name Methods for drawing parts of LayerElement child classes.
     * They are sub-drawing methods that are called from the base drawing methods above.
     * The parameter set can be different from a base drawing method since no recursive call is expected.
     * Defined in view_element.cpp
     */
    ///@{
    void DrawAcciaccaturaSlash(DeviceContext *dc, LayerElement *element);
    void DrawDots(DeviceContext *dc, int x, int y, unsigned char dots, Staff *staff);
    void DrawFermataAttr(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawLedgerLines(
        DeviceContext *dc, LayerElement *element, Staff *staff, bool aboveStaff, bool doubleLength, int skip, int n);
    void DrawLigatureNote(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff);
    void DrawMeterSigFigures(DeviceContext *dc, int x, int y, int num, int numBase, Staff *staff);
    void DrawMRptPart(DeviceContext *dc, int x, wchar_t smulfCode, int num, bool line, Staff *staff, Measure *measure);
    void DrawRestBreve(DeviceContext *dc, int x, int y, Staff *staff);
    void DrawRestLong(DeviceContext *dc, int x, int y, Staff *staff);
    void DrawRestQuarter(DeviceContext *dc, int x, int y, int valeur, unsigned char dots, bool cueSize, Staff *staff);
    void DrawRestWhole(DeviceContext *dc, int x, int y, int valeur, unsigned char dots, bool cueSize, Staff *staff);
    void DrawStem(DeviceContext *dc, LayerElement *object, Staff *staff, data_STEMDIRECTION dir, int radius, int xn,
        int originY, int heightY = 0);
    void DrawTrillAttr(DeviceContext *dc, LayerElement *element, Staff *staff);
    ///@}

    /**
     * @name Top level method for drawing TextElement.
     * This can be called recursively for text elements containing other text elements.
     * Defined in view_text.cpp
     */
    ///@{
    void DrawTextElement(DeviceContext *dc, TextElement *element, int x, int y, bool &setX, bool &setY);
    ///@}

    /**
     * @name Methods for drawing LayerElement child classes.
     * They are base drawing methods that are called directly from DrawLayerElement
     * Because some elements draw their children recursively (e.g., Note) they must all
     * have the same parameters
     * Defined in view_element.cpp
     */
    ///@{
    void DrawRend(DeviceContext *dc, Rend *rend, int x, int y, bool &setX, bool &setY);
    void DrawText(DeviceContext *dc, Text *text, int x, int y, bool &setX, bool &setY);

    /**
     * @name Method for drawing Beam and FTrem.
     * Defined in view_beam.cpp
     */
    ///@{
    void DrawBeam(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawFTrem(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void CalcBeam(Layer *layer, Staff *staff, const ArrayOfBeamElementCoords *beamElementCoords, int elementCount,
        BeamParams *params);
    ///@}

    /**
     * @name Methods for drawing Floating child classes.
     * They are base drawing methods that are called directly from DrawFloatingElement.
     * Call appropriate method of child classes (Slur, Tempo, Tie, etc).
     * Defined in view_control.cpp
     */
    ///@{
    void DrawControlElement(DeviceContext *dc, ControlElement *element, Measure *measure, System *system);
    void DrawSylConnector(
        DeviceContext *dc, Syl *syl, int x1, int x2, Staff *staff, char spanningType, Object *graphic = NULL);
    void DrawSylConnectorLines(DeviceContext *dc, int x1, int x2, int y, Syl *syl, Staff *staff);
    void DrawTimeSpanningElement(DeviceContext *dc, Object *object, System *system);
    void DrawDir(DeviceContext *dc, Dir *dir, Measure *measure, System *system);
    void DrawDynam(DeviceContext *dc, Dynam *dynam, Measure *measure, System *system);
    void DrawFermata(DeviceContext *dc, Fermata *fermata, Measure *measure, System *system);
    void DrawHairpin(
        DeviceContext *dc, Hairpin *hairpin, int x1, int x2, Staff *staff, char spanningType, Object *graphic = NULL);
    void DrawHarm(DeviceContext *dc, Harm *harm, Measure *measure, System *system);
    void DrawOctave(
        DeviceContext *dc, Octave *octave, int x1, int x2, Staff *staff, char spanningType, Object *graphic = NULL);
    void DrawPedal(DeviceContext *dc, Pedal *pedal, Measure *measure, System *system);
    void DrawSlur(
        DeviceContext *dc, Slur *slur, int x1, int x2, Staff *staff, char spanningType, Object *graphic = NULL);
    void DrawTempo(DeviceContext *dc, Tempo *tempo, Measure *measure, System *system);
    void DrawTie(DeviceContext *dc, Tie *tie, int x1, int x2, Staff *staff, char spanningType, Object *graphic = NULL);
    void DrawTrill(DeviceContext *dc, Trill *trill, Measure *measure, System *system);
    ///@}

    /**
     * @name Methods for drawing SystemElement child classes.
     * They are base drawing methods that are called directly from DrawSystemElement.
     * Defined in view_control.cpp
     */
    ///@{
    void DrawSystemElement(DeviceContext *dc, SystemElement *element, System *system);
    void DrawEnding(DeviceContext *dc, Ending *ending, System *system);
    ///@}

    /**
     * @name Methods for drawing mensural LayerElement child classes.
     * They are base drawing methods that are called directly from DrawLayerElement.
     * Because some elements draw their children recursively (e.g., Note) they must all
     * have the same parameters.
     * Defined in view_mensural.cpp
     */
    ///@{
    void DrawMensur(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawMensuralNote(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawProport(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);

    /**
     * @name Methods for drawing parts of mensural LayerElement child classes.
     * Defined in view_mensural.cpp
     */
    ///@{
    void DrawMensuralStem(DeviceContext *dc, LayerElement *object, Staff *staff, data_STEMDIRECTION dir, int radius,
        int xn, int originY, int heightY = 0);
    void DrawMensurCircle(DeviceContext *dc, int x, int yy, Staff *staff);
    void DrawMensurDot(DeviceContext *dc, int x, int yy, Staff *staff);
    void DrawMensurHalfCircle(DeviceContext *dc, int x, int yy, Staff *staff);
    void DrawMensurReversedHalfCircle(DeviceContext *dc, int x, int yy, Staff *staff);
    void DrawMensurSlash(DeviceContext *dc, int x, int yy, Staff *staff);
    void DrawMaximaToBrevis(DeviceContext *dc, int y, LayerElement *element, Layer *layer, Staff *staff);
    void CalculateLigaturePosX(LayerElement *element, Layer *layer, Staff *staff);
    void DrawProportFigures(DeviceContext *dc, int x, int y, int num, int numBase, Staff *staff);
    ///@}

    /**
     * @name Method for drawing Tuplet.
     * Called from the the layer postponed drawing list.
     * Defined in view_tuplet.cpp
     */
    ///@{
    void DrawTupletPostponed(DeviceContext *dc, Tuplet *tuplet, Layer *layer, Staff *staff);

    /**
     * @name Low level drawing methods
     * Defined in view_graph.cpp
     */
    ///@{
    void DrawVerticalLine(DeviceContext *dc, int y1, int y2, int x1, int nbr);
    void DrawHorizontalLine(DeviceContext *dc, int x1, int x2, int y1, int nbr);
    void DrawSmuflCode(DeviceContext *dc, int x, int y, wchar_t code, int staffSize, bool dimin);
    void DrawThickBezierCurve(DeviceContext *dc, Point bezier[4], int thickness, int staffSize, float angle = 0.0);
    void DrawPartFilledRectangle(DeviceContext *dc, int x1, int y1, int x2, int y2, int fillSection);
    void DrawSmuflString(DeviceContext *dc, int x, int y, std::wstring s, bool center, int staffSize = 100);
    void DrawLyricString(DeviceContext *dc, int x, int y, std::wstring s, int staffSize = 100);
    void DrawFilledRectangle(DeviceContext *dc, int x1, int y1, int x2, int y2);
    void DrawObliquePolygon(DeviceContext *dc, int x1, int y1, int x2, int y2, int height);
    void DrawDiamond(DeviceContext *dc, int x1, int y1, int height, int width, bool fill);
    void DrawDot(DeviceContext *dc, int x, int y, int staffSize);
    ///@}

    /**
     * Calculate the ScoreDef width by taking into account its widest key signature.
     * This is used in justifiation for anticipating the width of initial scoreDefs that are not drawn in the un-casted
     * system.
     */
    void SetScoreDefDrawingWidth(DeviceContext *dc, ScoreDef *scoreDef);

private:
    /**
     * @name Internal methods used for calculating tuplets
     */
    ///@{
    data_STEMDIRECTION GetTupletCoordinates(Tuplet *tuplet, Layer *layer, Point *start, Point *end, Point *center);
    std::wstring IntToTupletFigures(unsigned short number);
    std::wstring IntToTimeSigFigures(unsigned short number);
    std::wstring IntToSmuflFigures(unsigned short number, int offset);
    bool OneBeamInTuplet(Tuplet *tuplet);
    int GetSylY(Syl *syl, Staff *staff);
    ///@}

    /**
     * @name Internal methods used for calculating slurs
     */
    float AdjustSlur(Slur *slur, Staff *staff, int layerN, curvature_CURVEDIR curveDir, Point points[4]);
    int AdjustSlurCurve(Slur *slur, ArrayOfLayerElementPointPairs *spanningPoints, Point *p1, Point *p2, Point *c1,
        Point *c2, curvature_CURVEDIR curveDir, float angle, bool posRatio = true);
    void AdjustSlurPosition(Slur *slur, ArrayOfLayerElementPointPairs *spanningPoints, Point *p1, Point *p2, Point *c1,
        Point *c2, curvature_CURVEDIR curveDir, float *angle, bool forceBothSides);
    float GetAdjustedSlurAngle(Point *p1, Point *p2, curvature_CURVEDIR curveDir);
    void GetControlPoints(
        Point *p1, Point *p2, Point *c1, Point *c2, curvature_CURVEDIR curveDir, int height, int staffSize);
    void GetSpanningPointPositions(ArrayOfLayerElementPointPairs *spanningPoints, Point p1, float angle,
        curvature_CURVEDIR curveDir, int staffSize);
    ///@}

    /**
     * Used for calculating clustered information/dot position
     */
    bool IsOnStaffLine(int y, Staff *staff);

    /**
     * Find the nearest unit position in the direction indicated by place.
     */
    int GetNearestInterStaffPosition(int y, Staff *staff, data_STAFFREL place);

    /**
     * Make sure dots on chords are vertically aligned correctly
     */
    void PrepareChordDots(DeviceContext *dc, Chord *chord, int x, int y, unsigned char dots, Staff *staff);

    /**
     * Changes and/or calculates the horizontal alignment of accidentals to prevent overlapping
     */
    bool CalculateAccidX(Staff *staff, Accid *accid, Chord *chord, bool adjustHorizontally);

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

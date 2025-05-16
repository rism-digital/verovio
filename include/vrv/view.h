/////////////////////////////////////////////////////////////////////////////
// Name:        view.h
// Author:      Laurent Pugin
// Created:     2010
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_RENDERER_H__
#define __VRV_RENDERER_H__

#include <optional>

#include "devicecontextbase.h"
#include "scoredef.h"
#include "textelement.h"
#include "vrvdef.h"

namespace vrv {

class Accid;
class AnnotScore;
class Arpeg;
class BarLine;
class Beam;
class BeamSegment;
class BeamSpan;
class BracketSpan;
class Breath;
class Caesura;
class Chord;
class ControlElement;
class DeviceContext;
class Dir;
class Div;
class DivLine;
class Doc;
class Dynam;
class EditorialElement;
class Ending;
class F;
class Fb;
class Fig;
class Fing;
class FloatingCurvePositioner;
class Fermata;
class Gliss;
class Graphic;
class Hairpin;
class Harm;
class KeyAccid;
class Layer;
class LayerElement;
class Lb;
class Liquescent;
class Measure;
class MNum;
class Mordent;
class Nc;
class Neume;
class Num;
class Octave;
class Options;
class Ornam;
class Page;
class PageElement;
class Pedal;
class PgFoot;
class PgHead;
class PitchInflection;
class Reh;
class Rend;
class RepeatMark;
class RunningElement;
class Slur;
class Staff;
class Svg;
class Syl;
class Syllable;
class Symbol;
class SymbolDef;
class System;
class SystemElement;
class Tempo;
class Text;
class TextDrawingParams;
class TextElement;
class TextLayoutElement;
class Tie;
class Trill;
class Turn;
class Tuplet;
class TupletBracket;
class TupletNum;
class Verse;

// Helper enums
enum class SlurHandling { Ignore, Initialize, Drawing };

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
     * Set the current page.
     * If doLayout is true, the layout of the page will be calculated.
     * This is the default behavior, however, in some cases, we do not
     * want it. For example, when drawing the pages for getting the bounding boxes.
     */
    void SetPage(Page *page, bool doLayout);

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
     * Return the pixel per unit factor of the current page (if any, 1.0 otherwise)
     */
    double GetPPUFactor() const;

    /**
     * @name Methods for calculating drawing positions
     * Defined in view_element.cpp
     */
    ///@{
    int CalculatePitchCode(Layer *layer, int y_n, int x_pos, int *octave);
    ///@}

    /**
     * Control how slurs are handled
     */
    ///@{
    SlurHandling GetSlurHandling() const { return m_slurHandling; }
    void SetSlurHandling(SlurHandling slurHandling) { m_slurHandling = slurHandling; }
    ///@}

protected:
    /**
     * @name Methods for drawing System, ScoreDef, StaffDef, Staff, and Layer.
     * Additional methods for drawing braces, barlines, slurs, etc.
     * Defined in view_page.cpp
     */
    ///@{
    void DrawPageElement(DeviceContext *dc, PageElement *element);
    void DrawSystem(DeviceContext *dc, System *system);
    void DrawSystemList(DeviceContext *dc, System *system, const ClassId classId);
    void DrawScoreDef(DeviceContext *dc, ScoreDef *scoreDef, Measure *measure, int x, BarLine *barLine = NULL,
        bool isLastMeasure = false, bool isLastSystem = false);
    void DrawStaffGrp(DeviceContext *dc, Measure *measure, StaffGrp *staffGrp, int x, bool topStaffGrp = false,
        bool abbreviations = false);
    void DrawStaffDef(DeviceContext *dc, Staff *staff, Measure *measure);
    void DrawStaffDefCautionary(DeviceContext *dc, Staff *staff, Measure *measure);
    void DrawStaffDefLabels(DeviceContext *dc, Measure *measure, StaffGrp *staffGrp, int x, bool abbreviations = false);
    void DrawGrpSym(DeviceContext *dc, Measure *measure, StaffGrp *staffGrp, int &x);
    void DrawLabels(DeviceContext *dc, ScoreDef *scoreDef, Object *object, int x, int y, bool abbreviations,
        int staffSize, int space);
    void DrawBracket(DeviceContext *dc, int x, int y1, int y2, int staffSize);
    void DrawBracketSq(DeviceContext *dc, int x, int y1, int y2, int staffSize);
    void DrawBrace(DeviceContext *dc, int x, int y1, int y2, int staffSize);
    void DrawBarLines(DeviceContext *dc, Measure *measure, StaffGrp *staffGrp, BarLine *barLine, bool isLastMeasure,
        bool isLastSystem, int &yBottomPrevious);
    void DrawBarLine(DeviceContext *dc, int yTop, int yBottom, BarLine *barLine, data_BARRENDITION form,
        bool inStaffSpace = false, bool eraseIntersections = false);
    void DrawBarLineDots(DeviceContext *dc, Staff *staff, BarLine *barLine);
    void DrawLedgerLines(DeviceContext *dc, Staff *staff, const ArrayOfLedgerLines &lines, bool below, bool cueSize);
    void DrawMeasure(DeviceContext *dc, Measure *measure, System *system);
    void DrawMeterSigGrp(DeviceContext *dc, Layer *layer, Staff *staff);
    void DrawMNum(DeviceContext *dc, MNum *mnum, Measure *measure, System *system, int yOffset);
    void DrawStaff(DeviceContext *dc, Staff *staff, Measure *measure, System *system);
    void DrawStaffLines(DeviceContext *dc, Staff *staff, StaffDef *staffDef, Measure *measure, System *system);
    void DrawLayer(DeviceContext *dc, Layer *layer, Staff *staff, Measure *measure);
    void DrawLayerList(DeviceContext *dc, Layer *layer, Staff *staff, Measure *measure, const ClassId classId);
    void DrawLayerDefLabels(
        DeviceContext *dc, ScoreDef *scoreDef, Staff *staff, StaffDef *staffDef, int x, bool abbreviations = false);
    void DrawSystemDivider(DeviceContext *dc, System *system, Measure *firstMeasure);
    ///@}

    /**
     * @name Methods for drawing TextLayoutElement (Div, PgHead, PgFoot, etc.)
     * Defined in view_text.cpp
     */
    ///@{
    void DrawDiv(DeviceContext *dc, Div *div, System *system);
    void DrawRunningElements(DeviceContext *dc, Page *page);
    void DrawTextLayoutElement(DeviceContext *dc, TextLayoutElement *textLayoutElement);
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
    void DrawTextChildren(DeviceContext *dc, Object *parent, TextDrawingParams &params);
    void DrawFbChildren(DeviceContext *dc, Object *parent, TextDrawingParams &params);
    void DrawRunningChildren(DeviceContext *dc, Object *parent, TextDrawingParams &params);
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
    void DrawTextEditorialElement(DeviceContext *dc, EditorialElement *element, TextDrawingParams &params);
    void DrawFbEditorialElement(DeviceContext *dc, EditorialElement *element, TextDrawingParams &params);
    void DrawRunningEditorialElement(DeviceContext *dc, EditorialElement *element, TextDrawingParams &params);
    ///@}

    /**
     * @name Methods for drawing Annotations
     * Defined in view_page.cpp
     */
    ///@{
    void DrawAnnot(DeviceContext *dc, EditorialElement *element, bool isTextElement = false);
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
    void DrawAccid(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawArtic(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawBarLine(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawBeatRpt(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawBTrem(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawChord(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawClef(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawCustos(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawDot(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawDots(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawDurationElement(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawFlag(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawGenericLayerElement(
        DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawGraceGrp(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawHalfmRpt(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawKeySig(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawLigature(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawMeterSig(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawMRest(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawMRpt(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawMRpt2(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawMSpace(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawMultiRest(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawMultiRpt(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawNote(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawRest(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawSpace(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawStem(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawStemMod(DeviceContext *dc, LayerElement *element, Staff *staff);
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
    void DrawAcciaccaturaSlash(DeviceContext *dc, Stem *stem, Staff *staff);
    void DrawChordCluster(DeviceContext *dc, Chord *chord, Layer *layer, Staff *staff, Measure *measure);
    void DrawClefEnclosing(DeviceContext *dc, Clef *clef, Staff *staff, char32_t glyph, int x, int y);
    void DrawDotsPart(DeviceContext *dc, int x, int y, unsigned char dots, const Staff *staff, bool dimin = false);
    void DrawKeySigCancellation(
        DeviceContext *dc, KeySig *keySig, Staff *staff, Clef *clef, int clefLocOffset, int beginCancel, int &x);
    void DrawKeyAccid(DeviceContext *dc, KeyAccid *keyAccid, Staff *staff, Clef *clef, int clefLocOffset, int &x);
    void DrawMeterSig(DeviceContext *dc, MeterSig *meterSig, Staff *staff, int horizOffset);
    /** Returns the width of the drawn figures */
    int DrawMeterSigFigures(DeviceContext *dc, int x, int y, MeterSig *meterSig, int den, Staff *staff);
    void DrawMRptPart(DeviceContext *dc, int xCentered, char32_t smulfCode, int num, bool line, Staff *staff);
    ///@}

    /**
     * @name Top level method for drawing TextElement.
     * This can be called recursively for text elements containing other text elements.
     * Defined in view_text.cpp
     */
    ///@{
    void DrawTextElement(DeviceContext *dc, TextElement *element, TextDrawingParams &params);
    ///@}

    /**
     * @name Container for Figured Bass
     * Defined in view_text.cpp
     */
    ///@{
    void DrawFb(DeviceContext *dc, Staff *staff, Fb *element, TextDrawingParams &params);
    void DrawF(DeviceContext *dc, F *figure, TextDrawingParams &params);

    ///@}

    /**
     * @name Methods for drawing LayerElement child classes.
     * They are base drawing methods that are called directly from DrawLayerElement
     * Because some elements draw their children recursively (e.g., Note) they must all
     * have the same parameters
     * Defined in view_element.cpp
     */
    ///@{
    void DrawFig(DeviceContext *dc, Fig *fig, TextDrawingParams &params);
    void DrawLb(DeviceContext *dc, Lb *lb, TextDrawingParams &params);
    void DrawNum(DeviceContext *dc, Num *num, TextDrawingParams &params);
    void DrawRend(DeviceContext *dc, Rend *rend, TextDrawingParams &params);
    void DrawGraphic(DeviceContext *dc, Graphic *graphic, TextDrawingParams &params, int staffSize, bool dimin);
    void DrawSvg(DeviceContext *dc, Svg *svg, TextDrawingParams &params, int staffSize, bool dimin);
    void DrawSymbol(DeviceContext *dc, Symbol *symbol, TextDrawingParams &params);
    void DrawText(DeviceContext *dc, Text *text, TextDrawingParams &params);
    void DrawTextEnclosure(DeviceContext *dc, const TextDrawingParams &params, int staffSize);

    /**
     * @name Method for drawing Beam and FTrem.
     * Defined in view_beam.cpp
     */
    ///@{
    void DrawBeam(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawFTrem(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawBeamSpan(DeviceContext *dc, BeamSpan *beamSpan, System *system, Object *graphic = NULL);
    ///@}

    /**
     * @name Methods for drawing neumes.
     * Defined in view_neume.cpp
     */
    ///@{
    void DrawDivLine(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawSyllable(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawLiquescent(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawNc(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawNeume(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawOriscus(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawQuilisma(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    ///@}

    /**
     * @name Methods for drawing parts of neume LayerElement child classes.
     * Defined in view_neume.cpp
     */
    ///@{
    void DrawNcAsNotehead(DeviceContext *dc, Nc *nc, Layer *layer, Staff *staff, Measure *measure);
    void DrawNcGlyphs(DeviceContext *dc, Nc *nc, Staff *staff);
    ///@}

    /**
     * @name Methods for drawing Floating child classes.
     * They are base drawing methods that are called directly from DrawFloatingElement.
     * Call appropriate method of child classes (Slur, Tempo, Tie, etc).
     * Defined in view_control.cpp
     */
    ///@{
    void DrawControlElement(DeviceContext *dc, ControlElement *element, Measure *measure, System *system);
    void DrawTimeSpanningElement(DeviceContext *dc, Object *element, System *system);
    void DrawArpeg(DeviceContext *dc, Arpeg *arpeg, Measure *measure, System *system);
    void DrawArpegEnclosing(DeviceContext *dc, Arpeg *arpeg, Staff *staff, char32_t startGlyph, char32_t fillGlyph,
        char32_t endGlyph, int x, int y, int height, bool cueSize);
    void DrawBreath(DeviceContext *dc, Breath *breath, Measure *measure, System *system);
    void DrawCaesura(DeviceContext *dc, Caesura *caesura, Measure *measure, System *system);
    void DrawControlElementText(DeviceContext *dc, ControlElement *element, Measure *measure, System *system);
    void DrawDynam(DeviceContext *dc, Dynam *dynam, Measure *measure, System *system);
    void DrawDynamSymbolOnly(DeviceContext *dc, Staff *staff, Dynam *dynam, const std::u32string &dynamSymbol,
        data_HORIZONTALALIGNMENT alignment, TextDrawingParams &params);
    void DrawFermata(DeviceContext *dc, Fermata *fermata, Measure *measure, System *system);
    void DrawFing(DeviceContext *dc, Fing *fing, Measure *measure, System *system);
    void DrawHarm(DeviceContext *dc, Harm *harm, Measure *measure, System *system);
    void DrawMordent(DeviceContext *dc, Mordent *mordent, Measure *measure, System *system);
    void DrawPedal(DeviceContext *dc, Pedal *pedal, Measure *measure, System *system);
    void DrawReh(DeviceContext *dc, Reh *reh, Measure *measure, System *system);
    void DrawRepeatMark(DeviceContext *dc, RepeatMark *repeatMark, Measure *measure, System *system);
    void DrawTempo(DeviceContext *dc, Tempo *tempo, Measure *measure, System *system);
    void DrawTrill(DeviceContext *dc, Trill *trill, Measure *measure, System *system);
    void DrawTurn(DeviceContext *dc, Turn *turn, Measure *measure, System *system);
    ///@}

    /**
     * @name Methods for drawing time spanning elements, connectors or extensions
     * Called from DrawTimeSpanningElement
     */
    ///@{
    void DrawControlElementConnector(DeviceContext *dc, ControlElement *element, int x1, int x2, Staff *staff,
        char spanningType, Object *graphic = NULL);
    void DrawAnnotScore(DeviceContext *dc, AnnotScore *annotScore, int x1, int x2, Staff *staff, char spanningType,
        Object *graphic = NULL);
    void DrawBracketSpan(DeviceContext *dc, BracketSpan *bracketSpan, int x1, int x2, Staff *staff, char spanningType,
        Object *graphic = NULL);
    void DrawFConnector(
        DeviceContext *dc, F *f, int x1, int x2, Staff *staff, char spanningType, Object *graphic = NULL);
    void DrawGliss(
        DeviceContext *dc, Gliss *gliss, int x1, int x2, Staff *staff, char spanningType, Object *graphic = NULL);
    void DrawHairpin(
        DeviceContext *dc, Hairpin *hairpin, int x1, int x2, Staff *staff, char spanningType, Object *graphic = NULL);
    void DrawOctave(
        DeviceContext *dc, Octave *octave, int x1, int x2, Staff *staff, char spanningType, Object *graphic = NULL);
    void DrawPedalLine(
        DeviceContext *dc, Pedal *pedal, int x1, int x2, Staff *staff, char spanningType, Object *graphic = NULL);
    void DrawPitchInflection(DeviceContext *dc, PitchInflection *pitchInflection, int x1, int x2, Staff *staff,
        char spanningType, Object *graphic = NULL);
    void DrawSlur(
        DeviceContext *dc, Slur *slur, int x1, int x2, Staff *staff, char spanningType, Object *graphic = NULL);
    void DrawTie(DeviceContext *dc, Tie *tie, int x1, int x2, Staff *staff, char spanningType, Object *graphic = NULL);
    void DrawTrillExtension(
        DeviceContext *dc, Trill *trill, int x1, int x2, Staff *staff, char spanningType, Object *graphic = NULL);
    void DrawSylConnector(
        DeviceContext *dc, Syl *syl, int x1, int x2, Staff *staff, char spanningType, Object *graphic = NULL);
    ///@}

    /**
     * @name Methods for drawing connector lines
     */
    ///@{
    void DrawSylConnectorLines(DeviceContext *dc, int x1, int x2, int y, Syl *syl, Staff *staff);
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
    void DrawDotInLigature(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawPlica(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawProport(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    ///@}

    /**
     * @name Methods for drawing parts of mensural LayerElement child classes.
     * Defined in view_mensural.cpp
     */
    ///@{
    void DrawMensuralStem(DeviceContext *dc, Note *note, Staff *staff, data_STEMDIRECTION dir, int xn, int originY);
    void DrawMaximaToBrevis(DeviceContext *dc, int y, LayerElement *element, Layer *layer, Staff *staff);
    void DrawProportFigures(DeviceContext *dc, int x, int y, int num, int numBase, Staff *staff);
    void DrawLigatureNote(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff);
    ///@}

    /**
     * @name Methods for drawing tab LayerElement child classes.
     * They are base drawing methods that are called directly from DrawLayerElement.
     * Because some elements draw their children recursively (e.g., Note) they must all
     * have the same parameters.
     * Defined in view_tab.cpp
     */
    ///@{
    void DrawTabClef(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawTabDurSym(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawTabGrp(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawTabNote(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    ///@}

    /**
     * @name Method for drawing Tuplet.
     * Defined in view_tuplet.cpp
     */
    ///@{
    void DrawTupletBracket(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);
    void DrawTupletNum(DeviceContext *dc, LayerElement *element, Layer *layer, Staff *staff, Measure *measure);

    /**
     * @name Low level drawing methods
     * Defined in view_graph.cpp
     */
    ///@{
    void DrawVerticalLine(DeviceContext *dc, int y1, int y2, int x1, int width, int dashLength = 0, int gapLength = 0);
    void DrawHorizontalLine(
        DeviceContext *dc, int x1, int x2, int y1, int width, int dashLength = 0, int gapLength = 0);
    void DrawObliqueLine(
        DeviceContext *dc, int x1, int x2, int y1, int y2, int width, int dashLength = 0, int gapLength = 0);
    void DrawVerticalSegmentedLine(
        DeviceContext *dc, int x1, SegmentedLine &line, int width, int dashLength = 0, int gapLength = 0);
    void DrawHorizontalSegmentedLine(
        DeviceContext *dc, int y1, SegmentedLine &line, int width, int dashLength = 0, int gapLength = 0);
    void DrawSmuflCode(
        DeviceContext *dc, int x, int y, char32_t code, int staffSize, bool dimin, bool setBBGlyph = false);
    // void DrawSmuflCodeWithCustomFont(DeviceContext *dc, const std::string &customFont, int x, int y, char32_t code,
    //     int staffSize, bool dimin, bool setBBGlyph = false);

    void DrawThickBezierCurve(
        DeviceContext *dc, Point bezier[4], int thickness, int staffSize, int penWidth, PenStyle penStyle = PEN_SOLID);
    void DrawTextString(DeviceContext *dc, const std::u32string &str, TextDrawingParams &params);
    void DrawDirString(DeviceContext *dc, const std::u32string &str, TextDrawingParams &params);
    void DrawDynamString(DeviceContext *dc, const std::u32string &str, TextDrawingParams &params, Rend *rend);
    void DrawHarmString(DeviceContext *dc, const std::u32string &str, TextDrawingParams &params);
    void DrawSmuflLine(DeviceContext *dc, Point orig, int length, int staffSize, bool dimin, char32_t fill,
        char32_t start = 0, char32_t end = 0);
    void DrawSmuflString(DeviceContext *dc, int x, int y, std::u32string s, data_HORIZONTALALIGNMENT alignment,
        int staffSize = 100, bool dimin = false, bool setBBGlyph = false);
    void DrawLyricString(DeviceContext *dc, const std::u32string &str, int staffSize = 100,
        std::optional<TextDrawingParams> params = std::nullopt);
    void DrawNotFilledEllipse(DeviceContext *dc, int x1, int y1, int x2, int y2, int lineThinkness);
    void DrawFilledRectangle(DeviceContext *dc, int x1, int y1, int x2, int y2);
    void DrawNotFilledRectangle(DeviceContext *dc, int x1, int y1, int x2, int y2, int lineThinkness, int radius);
    void DrawFilledRoundedRectangle(DeviceContext *dc, int x1, int y1, int x2, int y2, int radius);
    void DrawObliquePolygon(DeviceContext *dc, int x1, int y1, int x2, int y2, int height);
    void DrawDiamond(DeviceContext *dc, int x1, int y1, int height, int width, bool fill, int linewidth);
    void DrawDot(DeviceContext *dc, int x, int y, int staffSize, bool dimin = false);
    void DrawSquareBracket(DeviceContext *dc, bool leftBracket, int x, int y, int height, int width,
        int horizontalThickness, int verticalThickness);
    void DrawEnclosingBrackets(DeviceContext *dc, int x, int y, int height, int width, int offset, int bracketWidth,
        int horizontalThickness, int verticalThickness);
    void DrawVerticalDots(DeviceContext *dc, int x, const SegmentedLine &line, int barlineWidth, int interval);
    void DrawSymbolDef(DeviceContext *dc, Object *parent, SymbolDef *symbolDef, int x, int y, int staffSize, bool dimin,
        data_HORIZONTALALIGNMENT alignement = HORIZONTALALIGNMENT_left);
    ///@}

    /**
     * Calculate the ScoreDef width by taking into account its widest key signature.
     * This is used in justifiation for anticipating the width of initial scoreDefs that are not drawn in the un-casted
     * system.
     */
    void SetScoreDefDrawingWidth(DeviceContext *dc, ScoreDef *scoreDef);

private:
    /**
     * @name Internal methods
     */
    ///@{
    data_STEMDIRECTION GetTupletCoordinates(Tuplet *tuplet, Layer *layer, Point *start, Point *end, Point *center);
    std::u32string IntToTupletFigures(unsigned short number);
    std::u32string IntToTimeSigFigures(unsigned short number);
    std::u32string IntToSmuflFigures(unsigned short number, int offset);
    int NestedTuplets(Object *object);
    int GetSylYRel(int verseN, Staff *staff, data_STAFFREL place);
    int GetFYRel(F *f, Staff *staff);
    ///@}

    /**
     * @name Internal methods used for calculating slurs
     */
    ///@{
    FloatingCurvePositioner *CalcInitialSlur(
        DeviceContext *dc, Slur *slur, int x1, int x2, Staff *staff, char spanningType);
    ///@}

    /**
     * @name Internal methods for calcultating brevis / longa
     */
    void CalcBrevisPoints(
        Note *note, Staff *staff, Point *topLeft, Point *bottomRight, int sides[4], int shape, bool isMensuralBlack);
    void CalcObliquePoints(Note *note1, Note *note2, Staff *staff, Point points[4], int sides[4], int shape,
        bool isMensuralBlack, bool firstHalf, bool straight);

    /**
     * Internal methods for drawing a BeamSegment
     */
    ///@{
    void DrawBeamSegment(
        DeviceContext *dc, BeamSegment *segment, BeamDrawingInterface *beamInterface, Layer *layer, Staff *staff);
    void DrawFTremSegment(DeviceContext *dc, Staff *staff, FTrem *fTrem);
    ///@}

    /**
     * Internal methods for drawing time spanning elements
     */
    bool HasValidTimeSpanningOrder(DeviceContext *dc, Object *element, LayerElement *start, LayerElement *end) const;

    /**
     * Internal method to find stem direction for notes of mensural notation
     */
    data_STEMDIRECTION GetMensuralStemDir(Layer *layer, Note *note, int verticalCenter);

public:
    /** Document */
    Doc *m_doc;
    /** Options of the document */
    Options *m_options;

    /**
     * @name The objects currently selected.
     * To be used in child classes, but defined here because it can be
     * useful for changing the color, for example
     */
    ///@{
    Page *m_currentPage;
    ///@}

protected:
    /**
     * The color currently being used when drawing.
     * It can change when drawing the m_currentElement, for example
     */
    int m_currentColor;

    /**
     * Control the handling of slurs
     */
    SlurHandling m_slurHandling;

    /**
     * The current drawing score def.
     * The is set when starting to draw a page in DrawCurrentPage and then
     * modified appropriately when going through the page.
     */
    ScoreDef m_drawingScoreDef;

private:
    //----------------//
    // Static members //
    //----------------//

    /** @name Internal values for storing temporary values for ligatures */
    ///@{
    static thread_local int s_drawingLigX[2], s_drawingLigY[2];
    static thread_local bool s_drawingLigObliqua;
    ///@}
};

} // namespace vrv

#endif

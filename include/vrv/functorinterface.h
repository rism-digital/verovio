/////////////////////////////////////////////////////////////////////////////
// Name:        functorinterface.h
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FUNCTOR_INTERFACE_H__
#define __VRV_FUNCTOR_INTERFACE_H__

#include "vrvdef.h"

namespace vrv {

class Accid;
class Alignment;
class AlignmentReference;
class AnchoredText;
class AnnotScore;
class Arpeg;
class Artic;
class BarLine;
class Beam;
class BeamSpan;
class BeatRpt;
class BracketSpan;
class Breath;
class BTrem;
class Caesura;
class Chord;
class Clef;
class ControlElement;
class Course;
class CpMark;
class Custos;
class Dir;
class Div;
class Dot;
class Dots;
class Doc;
class Dynam;
class EditorialElement;
class Ending;
class Expansion;
class F;
class Facsimile;
class Fb;
class Fermata;
class Fing;
class Fig;
class Flag;
class FloatingObject;
class FTrem;
class GenericLayerElement;
class Gliss;
class GraceAligner;
class GraceGrp;
class Graphic;
class GrpSym;
class Hairpin;
class HalfmRpt;
class Harm;
class HorizontalAligner;
class InstrDef;
class KeyAccid;
class KeySig;
class Label;
class LabelAbbr;
class Layer;
class LayerDef;
class LayerElement;
class Lb;
class Ligature;
class Lv;
class Mdiv;
class Measure;
class MeasureAligner;
class Mensur;
class MeterSig;
class MeterSigGrp;
class Mordent;
class MNum;
class MRest;
class MRpt;
class MRpt2;
class MSpace;
class MultiRest;
class MultiRpt;
class Nc;
class Neume;
class Note;
class Num;
class Object;
class Octave;
class Ornam;
class Page;
class PageElement;
class PageMilestoneEnd;
class Pages;
class Pb;
class Pedal;
class PgFoot;
class PgHead;
class Phrase;
class PitchInflection;
class Plica;
class Proport;
class RepeatMark;
class Reh;
class Rend;
class Rest;
class RunningElement;
class Sb;
class Score;
class ScoreDef;
class ScoreDefElement;
class Section;
class Slur;
class Space;
class Staff;
class StaffAlignment;
class StaffDef;
class StaffGrp;
class Stem;
class Surface;
class Svg;
class Syl;
class Syllable;
class Symbol;
class System;
class SystemAligner;
class SystemElement;
class SystemMilestoneEnd;
class TabDurSym;
class TabGrp;
class Tempo;
class Text;
class TextElement;
class TextLayoutElement;
class Tie;
class TimestampAligner;
class TimestampAttr;
class Trill;
class Tuning;
class Tuplet;
class TupletBracket;
class TupletNum;
class Turn;
class Verse;
class Zone;

//----------------------------------------------------------------------------
// FunctorInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for functors based on a visitor pattern.
 * It is not an abstract class but should not be instanciated directly.
 */
class FunctorInterface {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    FunctorInterface() {}
    virtual ~FunctorInterface() = default;
    ///@}

    /**
     * @name Visit object and doc
     */
    ///@{
    virtual FunctorCode VisitObject(Object *) { return FUNCTOR_CONTINUE; }
    virtual FunctorCode VisitObjectEnd(Object *) { return FUNCTOR_CONTINUE; }
    virtual FunctorCode VisitDoc(Doc *doc);
    virtual FunctorCode VisitDocEnd(Doc *doc);
    ///@}

    /**
     * @name Visit container elements
     */
    ///@{
    virtual FunctorCode VisitCourse(Course *course);
    virtual FunctorCode VisitCourseEnd(Course *course);
    virtual FunctorCode VisitGrpSym(GrpSym *grpSym);
    virtual FunctorCode VisitGrpSymEnd(GrpSym *grpSym);
    virtual FunctorCode VisitInstrDef(InstrDef *instrDef);
    virtual FunctorCode VisitInstrDefEnd(InstrDef *instrDef);
    virtual FunctorCode VisitLabel(Label *label);
    virtual FunctorCode VisitLabelEnd(Label *label);
    virtual FunctorCode VisitLabelAbbr(LabelAbbr *labelAbbr);
    virtual FunctorCode VisitLabelAbbrEnd(LabelAbbr *labelAbbr);
    virtual FunctorCode VisitLayer(Layer *layer);
    virtual FunctorCode VisitLayerEnd(Layer *layer);
    virtual FunctorCode VisitLayerDef(LayerDef *layerDef);
    virtual FunctorCode VisitLayerDefEnd(LayerDef *layerDef);
    virtual FunctorCode VisitMeasure(Measure *measure);
    virtual FunctorCode VisitMeasureEnd(Measure *measure);
    virtual FunctorCode VisitPage(Page *page);
    virtual FunctorCode VisitPageEnd(Page *page);
    virtual FunctorCode VisitPages(Pages *pages);
    virtual FunctorCode VisitPagesEnd(Pages *pages);
    virtual FunctorCode VisitPb(Pb *pb);
    virtual FunctorCode VisitPbEnd(Pb *pb);
    virtual FunctorCode VisitSb(Sb *sb);
    virtual FunctorCode VisitSbEnd(Sb *sb);
    virtual FunctorCode VisitScoreDef(ScoreDef *scoreDef);
    virtual FunctorCode VisitScoreDefEnd(ScoreDef *scoreDef);
    virtual FunctorCode VisitScoreDefElement(ScoreDefElement *scoreDefElement);
    virtual FunctorCode VisitScoreDefElementEnd(ScoreDefElement *scoreDefElement);
    virtual FunctorCode VisitStaff(Staff *staff);
    virtual FunctorCode VisitStaffEnd(Staff *staff);
    virtual FunctorCode VisitStaffDef(StaffDef *staffDef);
    virtual FunctorCode VisitStaffDefEnd(StaffDef *staffDef);
    virtual FunctorCode VisitStaffGrp(StaffGrp *staffGrp);
    virtual FunctorCode VisitStaffGrpEnd(StaffGrp *staffGrp);
    virtual FunctorCode VisitSystem(System *system);
    virtual FunctorCode VisitSystemEnd(System *system);
    virtual FunctorCode VisitTuning(Tuning *tuning);
    virtual FunctorCode VisitTuningEnd(Tuning *tuning);
    ///@}

    /**
     * @name Visit editorial elements
     * Limited to EditorialElement for now, can be extended later
     */
    ///@{
    virtual FunctorCode VisitEditorialElement(EditorialElement *editorialElement);
    virtual FunctorCode VisitEditorialElementEnd(EditorialElement *editorialElement);
    ///@}

    /**
     * @name Visit text layout elements
     */
    ///@{
    virtual FunctorCode VisitDiv(Div *div);
    virtual FunctorCode VisitDivEnd(Div *div);
    virtual FunctorCode VisitRunningElement(RunningElement *runningElement);
    virtual FunctorCode VisitRunningElementEnd(RunningElement *runningElement);
    virtual FunctorCode VisitPgFoot(PgFoot *pgFoot);
    virtual FunctorCode VisitPgFootEnd(PgFoot *pgFoot);
    virtual FunctorCode VisitPgHead(PgHead *pgHead);
    virtual FunctorCode VisitPgHeadEnd(PgHead *pgHead);
    virtual FunctorCode VisitTextLayoutElement(TextLayoutElement *textLayoutElement);
    virtual FunctorCode VisitTextLayoutElementEnd(TextLayoutElement *textLayoutElement);
    ///@}

    /**
     * @name Visit system elements
     */
    ///@{
    virtual FunctorCode VisitEnding(Ending *ending);
    virtual FunctorCode VisitEndingEnd(Ending *ending);
    virtual FunctorCode VisitExpansion(Expansion *expansion);
    virtual FunctorCode VisitExpansionEnd(Expansion *expansion);
    virtual FunctorCode VisitSection(Section *section);
    virtual FunctorCode VisitSectionEnd(Section *section);
    virtual FunctorCode VisitSystemElement(SystemElement *systemElement);
    virtual FunctorCode VisitSystemElementEnd(SystemElement *systemElement);
    virtual FunctorCode VisitSystemMilestone(SystemMilestoneEnd *systemMilestoneEnd);
    virtual FunctorCode VisitSystemMilestoneEnd(SystemMilestoneEnd *systemMilestoneEnd);
    ///@}

    /**
     * @name Visit page elements
     */
    ///@{
    virtual FunctorCode VisitMdiv(Mdiv *mdiv);
    virtual FunctorCode VisitMdivEnd(Mdiv *mdiv);
    virtual FunctorCode VisitPageElement(PageElement *pageElement);
    virtual FunctorCode VisitPageElementEnd(PageElement *pageElement);
    virtual FunctorCode VisitPageMilestone(PageMilestoneEnd *pageMilestoneEnd);
    virtual FunctorCode VisitPageMilestoneEnd(PageMilestoneEnd *pageMilestoneEnd);
    virtual FunctorCode VisitScore(Score *score);
    virtual FunctorCode VisitScoreEnd(Score *score);
    ///@}

    /**
     * @name Visit control elements
     */
    ///@{
    virtual FunctorCode VisitAnchoredText(AnchoredText *anchoredText);
    virtual FunctorCode VisitAnchoredTextEnd(AnchoredText *anchoredText);
    virtual FunctorCode VisitAnnotScore(AnnotScore *annotScore);
    virtual FunctorCode VisitAnnotScoreEnd(AnnotScore *annotScore);
    virtual FunctorCode VisitArpeg(Arpeg *arpeg);
    virtual FunctorCode VisitArpegEnd(Arpeg *arpeg);
    virtual FunctorCode VisitBeamSpan(BeamSpan *beamSpan);
    virtual FunctorCode VisitBeamSpanEnd(BeamSpan *beamSpan);
    virtual FunctorCode VisitBracketSpan(BracketSpan *bracketSpan);
    virtual FunctorCode VisitBracketSpanEnd(BracketSpan *bracketSpan);
    virtual FunctorCode VisitBreath(Breath *breath);
    virtual FunctorCode VisitBreathEnd(Breath *breath);
    virtual FunctorCode VisitCaesura(Caesura *caesura);
    virtual FunctorCode VisitCaesuraEnd(Caesura *caesura);
    virtual FunctorCode VisitControlElement(ControlElement *controlElement);
    virtual FunctorCode VisitControlElementEnd(ControlElement *controlElement);
    virtual FunctorCode VisitCpMark(CpMark *cpMark);
    virtual FunctorCode VisitCpMarkEnd(CpMark *cpMark);
    virtual FunctorCode VisitDir(Dir *dir);
    virtual FunctorCode VisitDirEnd(Dir *dir);
    virtual FunctorCode VisitDynam(Dynam *dynam);
    virtual FunctorCode VisitDynamEnd(Dynam *dynam);
    virtual FunctorCode VisitFermata(Fermata *fermata);
    virtual FunctorCode VisitFermataEnd(Fermata *fermata);
    virtual FunctorCode VisitFing(Fing *fing);
    virtual FunctorCode VisitFingEnd(Fing *fing);
    virtual FunctorCode VisitGliss(Gliss *gliss);
    virtual FunctorCode VisitGlissEnd(Gliss *gliss);
    virtual FunctorCode VisitHairpin(Hairpin *hairpin);
    virtual FunctorCode VisitHairpinEnd(Hairpin *hairpin);
    virtual FunctorCode VisitHarm(Harm *harm);
    virtual FunctorCode VisitHarmEnd(Harm *harm);
    virtual FunctorCode VisitLv(Lv *lv);
    virtual FunctorCode VisitLvEnd(Lv *lv);
    virtual FunctorCode VisitMordent(Mordent *mordent);
    virtual FunctorCode VisitMordentEnd(Mordent *mordent);
    virtual FunctorCode VisitOctave(Octave *octave);
    virtual FunctorCode VisitOctaveEnd(Octave *octave);
    virtual FunctorCode VisitOrnam(Ornam *ornam);
    virtual FunctorCode VisitOrnamEnd(Ornam *ornam);
    virtual FunctorCode VisitPedal(Pedal *pedal);
    virtual FunctorCode VisitPedalEnd(Pedal *pedal);
    virtual FunctorCode VisitPhrase(Phrase *phrase);
    virtual FunctorCode VisitPhraseEnd(Phrase *phrase);
    virtual FunctorCode VisitPitchInflection(PitchInflection *pitchInflection);
    virtual FunctorCode VisitPitchInflectionEnd(PitchInflection *pitchInflection);
    virtual FunctorCode VisitReh(Reh *reh);
    virtual FunctorCode VisitRehEnd(Reh *reh);
    virtual FunctorCode VisitRepeatMark(RepeatMark *repeatMark);
    virtual FunctorCode VisitRepeatMarkEnd(RepeatMark *repeatMark);
    virtual FunctorCode VisitSlur(Slur *slur);
    virtual FunctorCode VisitSlurEnd(Slur *slur);
    virtual FunctorCode VisitTempo(Tempo *tempo);
    virtual FunctorCode VisitTempoEnd(Tempo *tempo);
    virtual FunctorCode VisitTie(Tie *tie);
    virtual FunctorCode VisitTieEnd(Tie *tie);
    virtual FunctorCode VisitTrill(Trill *trill);
    virtual FunctorCode VisitTrillEnd(Trill *trill);
    virtual FunctorCode VisitTurn(Turn *turn);
    virtual FunctorCode VisitTurnEnd(Turn *turn);
    ///@}

    /**
     * @name Visit layer elements
     */
    ///@{
    virtual FunctorCode VisitAccid(Accid *accid);
    virtual FunctorCode VisitAccidEnd(Accid *accid);
    virtual FunctorCode VisitArtic(Artic *artic);
    virtual FunctorCode VisitArticEnd(Artic *artic);
    virtual FunctorCode VisitBarLine(BarLine *barLine);
    virtual FunctorCode VisitBarLineEnd(BarLine *barLine);
    virtual FunctorCode VisitBeam(Beam *beam);
    virtual FunctorCode VisitBeamEnd(Beam *beam);
    virtual FunctorCode VisitBeatRpt(BeatRpt *beatRpt);
    virtual FunctorCode VisitBeatRptEnd(BeatRpt *beatRpt);
    virtual FunctorCode VisitBTrem(BTrem *bTrem);
    virtual FunctorCode VisitBTremEnd(BTrem *bTrem);
    virtual FunctorCode VisitChord(Chord *chord);
    virtual FunctorCode VisitChordEnd(Chord *chord);
    virtual FunctorCode VisitClef(Clef *clef);
    virtual FunctorCode VisitClefEnd(Clef *clef);
    virtual FunctorCode VisitCustos(Custos *custos);
    virtual FunctorCode VisitCustosEnd(Custos *custos);
    virtual FunctorCode VisitDot(Dot *dot);
    virtual FunctorCode VisitDotEnd(Dot *dot);
    virtual FunctorCode VisitDots(Dots *dots);
    virtual FunctorCode VisitDotsEnd(Dots *dots);
    virtual FunctorCode VisitFlag(Flag *flag);
    virtual FunctorCode VisitFlagEnd(Flag *flag);
    virtual FunctorCode VisitFTrem(FTrem *fTrem);
    virtual FunctorCode VisitFTremEnd(FTrem *fTrem);
    virtual FunctorCode VisitGenericLayerElement(GenericLayerElement *genericLayerElement);
    virtual FunctorCode VisitGenericLayerElementEnd(GenericLayerElement *genericLayerElement);
    virtual FunctorCode VisitGraceGrp(GraceGrp *graceGrp);
    virtual FunctorCode VisitGraceGrpEnd(GraceGrp *graceGrp);
    virtual FunctorCode VisitHalfmRpt(HalfmRpt *halfmRpt);
    virtual FunctorCode VisitHalfmRptEnd(HalfmRpt *halfmRpt);
    virtual FunctorCode VisitKeyAccid(KeyAccid *keyAccid);
    virtual FunctorCode VisitKeyAccidEnd(KeyAccid *keyAccid);
    virtual FunctorCode VisitKeySig(KeySig *keySig);
    virtual FunctorCode VisitKeySigEnd(KeySig *keySig);
    virtual FunctorCode VisitLayerElement(LayerElement *layerElement);
    virtual FunctorCode VisitLayerElementEnd(LayerElement *layerElement);
    virtual FunctorCode VisitLigature(Ligature *ligature);
    virtual FunctorCode VisitLigatureEnd(Ligature *ligature);
    virtual FunctorCode VisitMensur(Mensur *mensur);
    virtual FunctorCode VisitMensurEnd(Mensur *mensur);
    virtual FunctorCode VisitMeterSig(MeterSig *meterSig);
    virtual FunctorCode VisitMeterSigEnd(MeterSig *meterSig);
    virtual FunctorCode VisitMeterSigGrp(MeterSigGrp *meterSigGrp);
    virtual FunctorCode VisitMeterSigGrpEnd(MeterSigGrp *meterSigGrp);
    virtual FunctorCode VisitMRest(MRest *mRest);
    virtual FunctorCode VisitMRestEnd(MRest *mRest);
    virtual FunctorCode VisitMRpt(MRpt *mRpt);
    virtual FunctorCode VisitMRptEnd(MRpt *mRpt);
    virtual FunctorCode VisitMRpt2(MRpt2 *mRpt2);
    virtual FunctorCode VisitMRpt2End(MRpt2 *mRpt2);
    virtual FunctorCode VisitMSpace(MSpace *mSpace);
    virtual FunctorCode VisitMSpaceEnd(MSpace *mSpace);
    virtual FunctorCode VisitMultiRest(MultiRest *multiRest);
    virtual FunctorCode VisitMultiRestEnd(MultiRest *multiRest);
    virtual FunctorCode VisitMultiRpt(MultiRpt *multiRpt);
    virtual FunctorCode VisitMultiRptEnd(MultiRpt *multiRpt);
    virtual FunctorCode VisitNc(Nc *nc);
    virtual FunctorCode VisitNcEnd(Nc *nc);
    virtual FunctorCode VisitNeume(Neume *neume);
    virtual FunctorCode VisitNeumeEnd(Neume *neume);
    virtual FunctorCode VisitNote(Note *note);
    virtual FunctorCode VisitNoteEnd(Note *note);
    virtual FunctorCode VisitPlica(Plica *plica);
    virtual FunctorCode VisitPlicaEnd(Plica *plica);
    virtual FunctorCode VisitProport(Proport *proport);
    virtual FunctorCode VisitProportEnd(Proport *proport);
    virtual FunctorCode VisitRest(Rest *rest);
    virtual FunctorCode VisitRestEnd(Rest *rest);
    virtual FunctorCode VisitSpace(Space *space);
    virtual FunctorCode VisitSpaceEnd(Space *space);
    virtual FunctorCode VisitStem(Stem *stem);
    virtual FunctorCode VisitStemEnd(Stem *stem);
    virtual FunctorCode VisitSyl(Syl *syl);
    virtual FunctorCode VisitSylEnd(Syl *syl);
    virtual FunctorCode VisitSyllable(Syllable *syllable);
    virtual FunctorCode VisitSyllableEnd(Syllable *syllable);
    virtual FunctorCode VisitTabDurSym(TabDurSym *tabDurSym);
    virtual FunctorCode VisitTabDurSymEnd(TabDurSym *tabDurSym);
    virtual FunctorCode VisitTabGrp(TabGrp *tabGrp);
    virtual FunctorCode VisitTabGrpEnd(TabGrp *tabGrp);
    virtual FunctorCode VisitTimestamp(TimestampAttr *timestamp);
    virtual FunctorCode VisitTimestampEnd(TimestampAttr *timestamp);
    virtual FunctorCode VisitTuplet(Tuplet *tuplet);
    virtual FunctorCode VisitTupletEnd(Tuplet *tuplet);
    virtual FunctorCode VisitTupletBracket(TupletBracket *tupletBracket);
    virtual FunctorCode VisitTupletBracketEnd(TupletBracket *tupletBracket);
    virtual FunctorCode VisitTupletNum(TupletNum *tupletNum);
    virtual FunctorCode VisitTupletNumEnd(TupletNum *tupletNum);
    virtual FunctorCode VisitVerse(Verse *verse);
    virtual FunctorCode VisitVerseEnd(Verse *verse);
    ///@}

    /**
     * @name Visit text elements
     */
    ///@{
    virtual FunctorCode VisitF(F *f);
    virtual FunctorCode VisitFEnd(F *f);
    virtual FunctorCode VisitFb(Fb *fb);
    virtual FunctorCode VisitFbEnd(Fb *fb);
    virtual FunctorCode VisitFig(Fig *fig);
    virtual FunctorCode VisitFigEnd(Fig *fig);
    virtual FunctorCode VisitLb(Lb *lb);
    virtual FunctorCode VisitLbEnd(Lb *lb);
    virtual FunctorCode VisitMNum(MNum *mNum);
    virtual FunctorCode VisitMNumEnd(MNum *mNum);
    virtual FunctorCode VisitNum(Num *num);
    virtual FunctorCode VisitNumEnd(Num *num);
    virtual FunctorCode VisitRend(Rend *rend);
    virtual FunctorCode VisitRendEnd(Rend *rend);
    virtual FunctorCode VisitSvg(Svg *svg);
    virtual FunctorCode VisitSvgEnd(Svg *svg);
    virtual FunctorCode VisitSymbol(Symbol *symbol);
    virtual FunctorCode VisitSymbolEnd(Symbol *symbol);
    virtual FunctorCode VisitText(Text *text);
    virtual FunctorCode VisitTextEnd(Text *text);
    virtual FunctorCode VisitTextElement(TextElement *textElement);
    virtual FunctorCode VisitTextElementEnd(TextElement *textElement);
    ///@}

    /**
     * @name Visit facsimle elements
     */
    ///@{
    virtual FunctorCode VisitFacsimile(Facsimile *facsimile);
    virtual FunctorCode VisitFacsimileEnd(Facsimile *facsimile);
    virtual FunctorCode VisitGraphic(Graphic *graphic);
    virtual FunctorCode VisitGraphicEnd(Graphic *graphic);
    virtual FunctorCode VisitSurface(Surface *surface);
    virtual FunctorCode VisitSurfaceEnd(Surface *surface);
    virtual FunctorCode VisitZone(Zone *zone);
    virtual FunctorCode VisitZoneEnd(Zone *zone);
    ///@}

    /**
     * @name Visit horizontal aligners
     */
    ///@{
    virtual FunctorCode VisitAlignment(Alignment *alignment);
    virtual FunctorCode VisitAlignmentEnd(Alignment *alignment);
    virtual FunctorCode VisitAlignmentReference(AlignmentReference *alignmentReference);
    virtual FunctorCode VisitAlignmentReferenceEnd(AlignmentReference *alignmentReference);
    virtual FunctorCode VisitHorizontalAligner(HorizontalAligner *horizontalAligner);
    virtual FunctorCode VisitHorizontalAlignerEnd(HorizontalAligner *horizontalAligner);
    virtual FunctorCode VisitMeasureAligner(MeasureAligner *measureAligner);
    virtual FunctorCode VisitMeasureAlignerEnd(MeasureAligner *measureAligner);
    virtual FunctorCode VisitGraceAligner(GraceAligner *graceAligner);
    virtual FunctorCode VisitGraceAlignerEnd(GraceAligner *graceAligner);
    virtual FunctorCode VisitTimestampAligner(TimestampAligner *timestampAligner);
    virtual FunctorCode VisitTimestampAlignerEnd(TimestampAligner *timestampAligner);
    ///@}

    /**
     * @name Visit vertical aligners
     */
    ///@{
    virtual FunctorCode VisitSystemAligner(SystemAligner *systemAligner);
    virtual FunctorCode VisitSystemAlignerEnd(SystemAligner *systemAligner);
    virtual FunctorCode VisitStaffAlignment(StaffAlignment *staffAlignment);
    virtual FunctorCode VisitStaffAlignmentEnd(StaffAlignment *staffAlignment);
    ///@}

    /**
     * @name Visit floating objects
     */
    ///@{
    virtual FunctorCode VisitFloatingObject(FloatingObject *floatingObject);
    virtual FunctorCode VisitFloatingObjectEnd(FloatingObject *floatingObject);
    ///@}

private:
    //
public:
    //
private:
    //
};

//----------------------------------------------------------------------------
// ConstFunctorInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for const functors based on a visitor pattern.
 * It is not an abstract class but should not be instanciated directly.
 */
class ConstFunctorInterface {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    ConstFunctorInterface() {}
    virtual ~ConstFunctorInterface() = default;
    ///@}

    /**
     * @name Visit object and doc
     */
    ///@{
    virtual FunctorCode VisitObject(const Object *) { return FUNCTOR_CONTINUE; }
    virtual FunctorCode VisitObjectEnd(const Object *) { return FUNCTOR_CONTINUE; }
    virtual FunctorCode VisitDoc(const Doc *doc);
    virtual FunctorCode VisitDocEnd(const Doc *doc);
    ///@}

    /**
     * @name Visit container elements
     */
    ///@{
    virtual FunctorCode VisitCourse(const Course *course);
    virtual FunctorCode VisitCourseEnd(const Course *course);
    virtual FunctorCode VisitGrpSym(const GrpSym *grpSym);
    virtual FunctorCode VisitGrpSymEnd(const GrpSym *grpSym);
    virtual FunctorCode VisitInstrDef(const InstrDef *instrDef);
    virtual FunctorCode VisitInstrDefEnd(const InstrDef *instrDef);
    virtual FunctorCode VisitLabel(const Label *label);
    virtual FunctorCode VisitLabelEnd(const Label *label);
    virtual FunctorCode VisitLabelAbbr(const LabelAbbr *labelAbbr);
    virtual FunctorCode VisitLabelAbbrEnd(const LabelAbbr *labelAbbr);
    virtual FunctorCode VisitLayer(const Layer *layer);
    virtual FunctorCode VisitLayerEnd(const Layer *layer);
    virtual FunctorCode VisitLayerDef(const LayerDef *layerDef);
    virtual FunctorCode VisitLayerDefEnd(const LayerDef *layerDef);
    virtual FunctorCode VisitMeasure(const Measure *measure);
    virtual FunctorCode VisitMeasureEnd(const Measure *measure);
    virtual FunctorCode VisitPage(const Page *page);
    virtual FunctorCode VisitPageEnd(const Page *page);
    virtual FunctorCode VisitPages(const Pages *pages);
    virtual FunctorCode VisitPagesEnd(const Pages *pages);
    virtual FunctorCode VisitPb(const Pb *pb);
    virtual FunctorCode VisitPbEnd(const Pb *pb);
    virtual FunctorCode VisitSb(const Sb *sb);
    virtual FunctorCode VisitSbEnd(const Sb *sb);
    virtual FunctorCode VisitScoreDef(const ScoreDef *scoreDef);
    virtual FunctorCode VisitScoreDefEnd(const ScoreDef *scoreDef);
    virtual FunctorCode VisitScoreDefElement(const ScoreDefElement *scoreDefElement);
    virtual FunctorCode VisitScoreDefElementEnd(const ScoreDefElement *scoreDefElement);
    virtual FunctorCode VisitStaff(const Staff *staff);
    virtual FunctorCode VisitStaffEnd(const Staff *staff);
    virtual FunctorCode VisitStaffDef(const StaffDef *staffDef);
    virtual FunctorCode VisitStaffDefEnd(const StaffDef *staffDef);
    virtual FunctorCode VisitStaffGrp(const StaffGrp *staffGrp);
    virtual FunctorCode VisitStaffGrpEnd(const StaffGrp *staffGrp);
    virtual FunctorCode VisitSystem(const System *system);
    virtual FunctorCode VisitSystemEnd(const System *system);
    virtual FunctorCode VisitTuning(const Tuning *tuning);
    virtual FunctorCode VisitTuningEnd(const Tuning *tuning);
    ///@}

    /**
     * @name Visit editorial elements
     * Limited to EditorialElement for now, can be extended later
     */
    ///@{
    virtual FunctorCode VisitEditorialElement(const EditorialElement *editorialElement);
    virtual FunctorCode VisitEditorialElementEnd(const EditorialElement *editorialElement);
    ///@}

    /**
     * @name Visit text layout elements
     */
    ///@{
    virtual FunctorCode VisitDiv(const Div *div);
    virtual FunctorCode VisitDivEnd(const Div *div);
    virtual FunctorCode VisitRunningElement(const RunningElement *runningElement);
    virtual FunctorCode VisitRunningElementEnd(const RunningElement *runningElement);
    virtual FunctorCode VisitPgFoot(const PgFoot *pgFoot);
    virtual FunctorCode VisitPgFootEnd(const PgFoot *pgFoot);
    virtual FunctorCode VisitPgHead(const PgHead *pgHead);
    virtual FunctorCode VisitPgHeadEnd(const PgHead *pgHead);
    virtual FunctorCode VisitTextLayoutElement(const TextLayoutElement *textLayoutElement);
    virtual FunctorCode VisitTextLayoutElementEnd(const TextLayoutElement *textLayoutElement);
    ///@}

    /**
     * @name Visit system elements
     */
    ///@{
    virtual FunctorCode VisitEnding(const Ending *ending);
    virtual FunctorCode VisitEndingEnd(const Ending *ending);
    virtual FunctorCode VisitExpansion(const Expansion *expansion);
    virtual FunctorCode VisitExpansionEnd(const Expansion *expansion);
    virtual FunctorCode VisitSection(const Section *section);
    virtual FunctorCode VisitSectionEnd(const Section *section);
    virtual FunctorCode VisitSystemElement(const SystemElement *systemElement);
    virtual FunctorCode VisitSystemElementEnd(const SystemElement *systemElement);
    virtual FunctorCode VisitSystemMilestone(const SystemMilestoneEnd *systemMilestoneEnd);
    virtual FunctorCode VisitSystemMilestoneEnd(const SystemMilestoneEnd *systemMilestoneEnd);
    ///@}

    /**
     * @name Visit page elements
     */
    ///@{
    virtual FunctorCode VisitMdiv(const Mdiv *mdiv);
    virtual FunctorCode VisitMdivEnd(const Mdiv *mdiv);
    virtual FunctorCode VisitPageElement(const PageElement *pageElement);
    virtual FunctorCode VisitPageElementEnd(const PageElement *pageElement);
    virtual FunctorCode VisitPageMilestone(const PageMilestoneEnd *pageMilestoneEnd);
    virtual FunctorCode VisitPageMilestoneEnd(const PageMilestoneEnd *pageMilestoneEnd);
    virtual FunctorCode VisitScore(const Score *score);
    virtual FunctorCode VisitScoreEnd(const Score *score);
    ///@}

    /**
     * @name Visit control elements
     */
    ///@{
    virtual FunctorCode VisitAnchoredText(const AnchoredText *anchoredText);
    virtual FunctorCode VisitAnchoredTextEnd(const AnchoredText *anchoredText);
    virtual FunctorCode VisitAnnotScore(const AnnotScore *annotScore);
    virtual FunctorCode VisitAnnotScoreEnd(const AnnotScore *annotScore);
    virtual FunctorCode VisitArpeg(const Arpeg *arpeg);
    virtual FunctorCode VisitArpegEnd(const Arpeg *arpeg);
    virtual FunctorCode VisitBeamSpan(const BeamSpan *beamSpan);
    virtual FunctorCode VisitBeamSpanEnd(const BeamSpan *beamSpan);
    virtual FunctorCode VisitBracketSpan(const BracketSpan *bracketSpan);
    virtual FunctorCode VisitBracketSpanEnd(const BracketSpan *bracketSpan);
    virtual FunctorCode VisitBreath(const Breath *breath);
    virtual FunctorCode VisitBreathEnd(const Breath *breath);
    virtual FunctorCode VisitCaesura(const Caesura *caesura);
    virtual FunctorCode VisitCaesuraEnd(const Caesura *caesura);
    virtual FunctorCode VisitControlElement(const ControlElement *controlElement);
    virtual FunctorCode VisitControlElementEnd(const ControlElement *controlElement);
    virtual FunctorCode VisitCpMark(const CpMark *cpMark);
    virtual FunctorCode VisitCpMarkEnd(const CpMark *cpMark);
    virtual FunctorCode VisitDir(const Dir *dir);
    virtual FunctorCode VisitDirEnd(const Dir *dir);
    virtual FunctorCode VisitDynam(const Dynam *dynam);
    virtual FunctorCode VisitDynamEnd(const Dynam *dynam);
    virtual FunctorCode VisitFermata(const Fermata *fermata);
    virtual FunctorCode VisitFermataEnd(const Fermata *fermata);
    virtual FunctorCode VisitFing(const Fing *fing);
    virtual FunctorCode VisitFingEnd(const Fing *fing);
    virtual FunctorCode VisitGliss(const Gliss *gliss);
    virtual FunctorCode VisitGlissEnd(const Gliss *gliss);
    virtual FunctorCode VisitHairpin(const Hairpin *hairpin);
    virtual FunctorCode VisitHairpinEnd(const Hairpin *hairpin);
    virtual FunctorCode VisitHarm(const Harm *harm);
    virtual FunctorCode VisitHarmEnd(const Harm *harm);
    virtual FunctorCode VisitLv(const Lv *lv);
    virtual FunctorCode VisitLvEnd(const Lv *lv);
    virtual FunctorCode VisitMordent(const Mordent *mordent);
    virtual FunctorCode VisitMordentEnd(const Mordent *mordent);
    virtual FunctorCode VisitOctave(const Octave *octave);
    virtual FunctorCode VisitOctaveEnd(const Octave *octave);
    virtual FunctorCode VisitOrnam(const Ornam *ornam);
    virtual FunctorCode VisitOrnamEnd(const Ornam *ornam);
    virtual FunctorCode VisitPedal(const Pedal *pedal);
    virtual FunctorCode VisitPedalEnd(const Pedal *pedal);
    virtual FunctorCode VisitPhrase(const Phrase *phrase);
    virtual FunctorCode VisitPhraseEnd(const Phrase *phrase);
    virtual FunctorCode VisitPitchInflection(const PitchInflection *pitchInflection);
    virtual FunctorCode VisitPitchInflectionEnd(const PitchInflection *pitchInflection);
    virtual FunctorCode VisitReh(const Reh *reh);
    virtual FunctorCode VisitRehEnd(const Reh *reh);
    virtual FunctorCode VisitRepeatMark(const RepeatMark *repeatMark);
    virtual FunctorCode VisitRepeatMarkEnd(const RepeatMark *repeatMark);
    virtual FunctorCode VisitSlur(const Slur *slur);
    virtual FunctorCode VisitSlurEnd(const Slur *slur);
    virtual FunctorCode VisitTempo(const Tempo *tempo);
    virtual FunctorCode VisitTempoEnd(const Tempo *tempo);
    virtual FunctorCode VisitTie(const Tie *tie);
    virtual FunctorCode VisitTieEnd(const Tie *tie);
    virtual FunctorCode VisitTrill(const Trill *trill);
    virtual FunctorCode VisitTrillEnd(const Trill *trill);
    virtual FunctorCode VisitTurn(const Turn *turn);
    virtual FunctorCode VisitTurnEnd(const Turn *turn);
    ///@}

    /**
     * @name Visit layer elements
     */
    ///@{
    virtual FunctorCode VisitAccid(const Accid *accid);
    virtual FunctorCode VisitAccidEnd(const Accid *accid);
    virtual FunctorCode VisitArtic(const Artic *artic);
    virtual FunctorCode VisitArticEnd(const Artic *artic);
    virtual FunctorCode VisitBarLine(const BarLine *barLine);
    virtual FunctorCode VisitBarLineEnd(const BarLine *barLine);
    virtual FunctorCode VisitBeam(const Beam *beam);
    virtual FunctorCode VisitBeamEnd(const Beam *beam);
    virtual FunctorCode VisitBeatRpt(const BeatRpt *beatRpt);
    virtual FunctorCode VisitBeatRptEnd(const BeatRpt *beatRpt);
    virtual FunctorCode VisitBTrem(const BTrem *bTrem);
    virtual FunctorCode VisitBTremEnd(const BTrem *bTrem);
    virtual FunctorCode VisitChord(const Chord *chord);
    virtual FunctorCode VisitChordEnd(const Chord *chord);
    virtual FunctorCode VisitClef(const Clef *clef);
    virtual FunctorCode VisitClefEnd(const Clef *clef);
    virtual FunctorCode VisitCustos(const Custos *custos);
    virtual FunctorCode VisitCustosEnd(const Custos *custos);
    virtual FunctorCode VisitDot(const Dot *dot);
    virtual FunctorCode VisitDotEnd(const Dot *dot);
    virtual FunctorCode VisitDots(const Dots *dots);
    virtual FunctorCode VisitDotsEnd(const Dots *dots);
    virtual FunctorCode VisitFlag(const Flag *flag);
    virtual FunctorCode VisitFlagEnd(const Flag *flag);
    virtual FunctorCode VisitFTrem(const FTrem *fTrem);
    virtual FunctorCode VisitFTremEnd(const FTrem *fTrem);
    virtual FunctorCode VisitGenericLayerElement(const GenericLayerElement *genericLayerElement);
    virtual FunctorCode VisitGenericLayerElementEnd(const GenericLayerElement *genericLayerElement);
    virtual FunctorCode VisitGraceGrp(const GraceGrp *graceGrp);
    virtual FunctorCode VisitGraceGrpEnd(const GraceGrp *graceGrp);
    virtual FunctorCode VisitHalfmRpt(const HalfmRpt *halfmRpt);
    virtual FunctorCode VisitHalfmRptEnd(const HalfmRpt *halfmRpt);
    virtual FunctorCode VisitKeyAccid(const KeyAccid *keyAccid);
    virtual FunctorCode VisitKeyAccidEnd(const KeyAccid *keyAccid);
    virtual FunctorCode VisitKeySig(const KeySig *keySig);
    virtual FunctorCode VisitKeySigEnd(const KeySig *keySig);
    virtual FunctorCode VisitLayerElement(const LayerElement *layerElement);
    virtual FunctorCode VisitLayerElementEnd(const LayerElement *layerElement);
    virtual FunctorCode VisitLigature(const Ligature *ligature);
    virtual FunctorCode VisitLigatureEnd(const Ligature *ligature);
    virtual FunctorCode VisitMensur(const Mensur *mensur);
    virtual FunctorCode VisitMensurEnd(const Mensur *mensur);
    virtual FunctorCode VisitMeterSig(const MeterSig *meterSig);
    virtual FunctorCode VisitMeterSigEnd(const MeterSig *meterSig);
    virtual FunctorCode VisitMeterSigGrp(const MeterSigGrp *meterSigGrp);
    virtual FunctorCode VisitMeterSigGrpEnd(const MeterSigGrp *meterSigGrp);
    virtual FunctorCode VisitMRest(const MRest *mRest);
    virtual FunctorCode VisitMRestEnd(const MRest *mRest);
    virtual FunctorCode VisitMRpt(const MRpt *mRpt);
    virtual FunctorCode VisitMRptEnd(const MRpt *mRpt);
    virtual FunctorCode VisitMRpt2(const MRpt2 *mRpt2);
    virtual FunctorCode VisitMRpt2End(const MRpt2 *mRpt2);
    virtual FunctorCode VisitMSpace(const MSpace *mSpace);
    virtual FunctorCode VisitMSpaceEnd(const MSpace *mSpace);
    virtual FunctorCode VisitMultiRest(const MultiRest *multiRest);
    virtual FunctorCode VisitMultiRestEnd(const MultiRest *multiRest);
    virtual FunctorCode VisitMultiRpt(const MultiRpt *multiRpt);
    virtual FunctorCode VisitMultiRptEnd(const MultiRpt *multiRpt);
    virtual FunctorCode VisitNc(const Nc *nc);
    virtual FunctorCode VisitNcEnd(const Nc *nc);
    virtual FunctorCode VisitNeume(const Neume *neume);
    virtual FunctorCode VisitNeumeEnd(const Neume *neume);
    virtual FunctorCode VisitNote(const Note *note);
    virtual FunctorCode VisitNoteEnd(const Note *note);
    virtual FunctorCode VisitPlica(const Plica *plica);
    virtual FunctorCode VisitPlicaEnd(const Plica *plica);
    virtual FunctorCode VisitProport(const Proport *proport);
    virtual FunctorCode VisitProportEnd(const Proport *proport);
    virtual FunctorCode VisitRest(const Rest *rest);
    virtual FunctorCode VisitRestEnd(const Rest *rest);
    virtual FunctorCode VisitSpace(const Space *space);
    virtual FunctorCode VisitSpaceEnd(const Space *space);
    virtual FunctorCode VisitStem(const Stem *stem);
    virtual FunctorCode VisitStemEnd(const Stem *stem);
    virtual FunctorCode VisitSyl(const Syl *syl);
    virtual FunctorCode VisitSylEnd(const Syl *syl);
    virtual FunctorCode VisitSyllable(const Syllable *syllable);
    virtual FunctorCode VisitSyllableEnd(const Syllable *syllable);
    virtual FunctorCode VisitTabDurSym(const TabDurSym *tabDurSym);
    virtual FunctorCode VisitTabDurSymEnd(const TabDurSym *tabDurSym);
    virtual FunctorCode VisitTabGrp(const TabGrp *tabGrp);
    virtual FunctorCode VisitTabGrpEnd(const TabGrp *tabGrp);
    virtual FunctorCode VisitTimestamp(const TimestampAttr *timestamp);
    virtual FunctorCode VisitTimestampEnd(const TimestampAttr *timestamp);
    virtual FunctorCode VisitTuplet(const Tuplet *tuplet);
    virtual FunctorCode VisitTupletEnd(const Tuplet *tuplet);
    virtual FunctorCode VisitTupletBracket(const TupletBracket *tupletBracket);
    virtual FunctorCode VisitTupletBracketEnd(const TupletBracket *tupletBracket);
    virtual FunctorCode VisitTupletNum(const TupletNum *tupletNum);
    virtual FunctorCode VisitTupletNumEnd(const TupletNum *tupletNum);
    virtual FunctorCode VisitVerse(const Verse *verse);
    virtual FunctorCode VisitVerseEnd(const Verse *verse);
    ///@}

    /**
     * @name Visit text elements
     */
    ///@{
    virtual FunctorCode VisitF(const F *f);
    virtual FunctorCode VisitFEnd(const F *f);
    virtual FunctorCode VisitFb(const Fb *fb);
    virtual FunctorCode VisitFbEnd(const Fb *fb);
    virtual FunctorCode VisitFig(const Fig *fig);
    virtual FunctorCode VisitFigEnd(const Fig *fig);
    virtual FunctorCode VisitLb(const Lb *lb);
    virtual FunctorCode VisitLbEnd(const Lb *lb);
    virtual FunctorCode VisitMNum(const MNum *mNum);
    virtual FunctorCode VisitMNumEnd(const MNum *mNum);
    virtual FunctorCode VisitNum(const Num *num);
    virtual FunctorCode VisitNumEnd(const Num *num);
    virtual FunctorCode VisitRend(const Rend *rend);
    virtual FunctorCode VisitRendEnd(const Rend *rend);
    virtual FunctorCode VisitSvg(const Svg *svg);
    virtual FunctorCode VisitSvgEnd(const Svg *svg);
    virtual FunctorCode VisitSymbol(const Symbol *symbol);
    virtual FunctorCode VisitSymbolEnd(const Symbol *symbol);
    virtual FunctorCode VisitText(const Text *text);
    virtual FunctorCode VisitTextEnd(const Text *text);
    virtual FunctorCode VisitTextElement(const TextElement *textElement);
    virtual FunctorCode VisitTextElementEnd(const TextElement *textElement);
    ///@}

    /**
     * @name Visit facsimle elements
     */
    ///@{
    virtual FunctorCode VisitFacsimile(const Facsimile *facsimile);
    virtual FunctorCode VisitFacsimileEnd(const Facsimile *facsimile);
    virtual FunctorCode VisitGraphic(const Graphic *graphic);
    virtual FunctorCode VisitGraphicEnd(const Graphic *graphic);
    virtual FunctorCode VisitSurface(const Surface *surface);
    virtual FunctorCode VisitSurfaceEnd(const Surface *surface);
    virtual FunctorCode VisitZone(const Zone *zone);
    virtual FunctorCode VisitZoneEnd(const Zone *zone);
    ///@}

    /**
     * @name Visit horizontal aligners
     */
    ///@{
    virtual FunctorCode VisitAlignment(const Alignment *alignment);
    virtual FunctorCode VisitAlignmentEnd(const Alignment *alignment);
    virtual FunctorCode VisitAlignmentReference(const AlignmentReference *alignmentReference);
    virtual FunctorCode VisitAlignmentReferenceEnd(const AlignmentReference *alignmentReference);
    virtual FunctorCode VisitHorizontalAligner(const HorizontalAligner *horizontalAligner);
    virtual FunctorCode VisitHorizontalAlignerEnd(const HorizontalAligner *horizontalAligner);
    virtual FunctorCode VisitMeasureAligner(const MeasureAligner *measureAligner);
    virtual FunctorCode VisitMeasureAlignerEnd(const MeasureAligner *measureAligner);
    virtual FunctorCode VisitGraceAligner(const GraceAligner *graceAligner);
    virtual FunctorCode VisitGraceAlignerEnd(const GraceAligner *graceAligner);
    virtual FunctorCode VisitTimestampAligner(const TimestampAligner *timestampAligner);
    virtual FunctorCode VisitTimestampAlignerEnd(const TimestampAligner *timestampAligner);
    ///@}

    /**
     * @name Visit vertical aligners
     */
    ///@{
    virtual FunctorCode VisitSystemAligner(const SystemAligner *systemAligner);
    virtual FunctorCode VisitSystemAlignerEnd(const SystemAligner *systemAligner);
    virtual FunctorCode VisitStaffAlignment(const StaffAlignment *staffAlignment);
    virtual FunctorCode VisitStaffAlignmentEnd(const StaffAlignment *staffAlignment);
    ///@}

    /**
     * @name Visit floating objects
     */
    ///@{
    virtual FunctorCode VisitFloatingObject(const FloatingObject *floatingObject);
    virtual FunctorCode VisitFloatingObjectEnd(const FloatingObject *floatingObject);
    ///@}

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif // __VRV_FUNCTOR_INTERFACE_H__

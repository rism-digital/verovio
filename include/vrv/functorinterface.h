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
class Artic;
class BarLine;
class Beam;
class BeatRpt;
class BTrem;
class Chord;
class Clef;
class Course;
class Custos;
class Dot;
class Dots;
class Doc;
class Flag;
class FTrem;
class GraceGrp;
class GrpSym;
class HalfmRpt;
class InstrDef;
class KeyAccid;
class KeySig;
class Label;
class LabelAbbr;
class Layer;
class LayerDef;
class LayerElement;
class Ligature;
class Mdiv;
class Measure;
class Mensur;
class MeterSig;
class MeterSigGrp;
class MRest;
class MRpt;
class MRpt2;
class MSpace;
class MultiRest;
class MultiRpt;
class Nc;
class Neume;
class Note;
class Object;
class Page;
class PageElement;
class PageMilestoneEnd;
class Pages;
class Pb;
class Plica;
class Proport;
class Rest;
class Sb;
class Score;
class ScoreDef;
class ScoreDefElement;
class Space;
class Staff;
class StaffDef;
class StaffGrp;
class Stem;
class Syl;
class Syllable;
class System;
class TabDurSym;
class TabGrp;
class TimestampAttr;
class Tuning;
class Tuplet;
class TupletBracket;
class TupletNum;
class Verse;

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
    FunctorInterface(){};
    virtual ~FunctorInterface() = default;
    ///@}

    /**
     * @name Visit object and doc
     */
    ///@{
    virtual FunctorCode VisitObject(Object *object) { return FUNCTOR_CONTINUE; }
    virtual FunctorCode VisitObjectEnd(Object *object) { return FUNCTOR_CONTINUE; }
    virtual FunctorCode VisitDoc(Doc *doc);
    virtual FunctorCode VisitDocEnd(Doc *doc);
    ///@}

    /**
     * @name Visit container elements
     */
    ///@{
    // TODO: Add system elements later
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
    // virtual FunctorCode VisitPb(Pb *pb);
    // virtual FunctorCode VisitPbEnd(Pb *pb);
    // virtual FunctorCode VisitSb(Sb *sb);
    // virtual FunctorCode VisitSbEnd(Sb *sb);
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

private:
    //
public:
    //
private:
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
    ConstFunctorInterface(){};
    virtual ~ConstFunctorInterface() = default;
    ///@}

    /**
     * @name Visit object and doc
     */
    ///@{
    virtual FunctorCode VisitObject(const Object *object) { return FUNCTOR_CONTINUE; }
    virtual FunctorCode VisitObjectEnd(const Object *object) { return FUNCTOR_CONTINUE; }
    virtual FunctorCode VisitDoc(const Doc *doc);
    virtual FunctorCode VisitDocEnd(const Doc *doc);
    ///@}

    /**
     * @name Visit container elements
     */
    ///@{
    // TODO: Add system elements later
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
    // virtual FunctorCode VisitPb(const Pb *pb);
    // virtual FunctorCode VisitPbEnd(const Pb *pb);
    // virtual FunctorCode VisitSb(const Sb *sb);
    // virtual FunctorCode VisitSbEnd(const Sb *sb);
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

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif // __VRV_FUNCTOR_INTERFACE_H__

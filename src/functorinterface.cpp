/////////////////////////////////////////////////////////////////////////////
// Name:        functorinterface.cpp
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "functorinterface.h"

//----------------------------------------------------------------------------

#include "accid.h"
#include "artic.h"
#include "barline.h"
#include "beam.h"
#include "beatrpt.h"
#include "btrem.h"
#include "chord.h"
#include "clef.h"
#include "course.h"
#include "custos.h"
#include "doc.h"
#include "dot.h"
#include "elementpart.h"
#include "ftrem.h"
#include "gracegrp.h"
#include "grpsym.h"
#include "halfmrpt.h"
#include "instrdef.h"
#include "keyaccid.h"
#include "keysig.h"
#include "label.h"
#include "labelabbr.h"
#include "layer.h"
#include "layerdef.h"
#include "layerelement.h"
#include "ligature.h"
#include "mdiv.h"
#include "measure.h"
#include "mensur.h"
#include "metersig.h"
#include "metersiggrp.h"
#include "mrest.h"
#include "mrpt.h"
#include "mrpt2.h"
#include "mspace.h"
#include "multirest.h"
#include "multirpt.h"
#include "nc.h"
#include "neume.h"
#include "note.h"
#include "page.h"
#include "pageelement.h"
#include "pagemilestone.h"
#include "pages.h"
#include "plica.h"
#include "proport.h"
#include "rest.h"
#include "score.h"
#include "scoredef.h"
#include "space.h"
#include "staff.h"
#include "staffdef.h"
#include "staffgrp.h"
#include "stem.h"
#include "syl.h"
#include "syllable.h"
#include "system.h"
#include "tabdursym.h"
#include "tabgrp.h"
#include "timestamp.h"
#include "tuning.h"
#include "tuplet.h"
#include "verse.h"

namespace vrv {

//----------------------------------------------------------------------------
// FunctorInterface
//----------------------------------------------------------------------------

FunctorCode FunctorInterface::VisitDoc(Doc *doc)
{
    return this->VisitObject(doc);
}

FunctorCode FunctorInterface::VisitDocEnd(Doc *doc)
{
    return this->VisitObjectEnd(doc);
}

FunctorCode FunctorInterface::VisitCourse(Course *course)
{
    return this->VisitObject(course);
}

FunctorCode FunctorInterface::VisitCourseEnd(Course *course)
{
    return this->VisitObjectEnd(course);
}

FunctorCode FunctorInterface::VisitGrpSym(GrpSym *grpSym)
{
    return this->VisitObject(grpSym);
}

FunctorCode FunctorInterface::VisitGrpSymEnd(GrpSym *grpSym)
{
    return this->VisitObjectEnd(grpSym);
}

FunctorCode FunctorInterface::VisitInstrDef(InstrDef *instrDef)
{
    return this->VisitObject(instrDef);
}

FunctorCode FunctorInterface::VisitInstrDefEnd(InstrDef *instrDef)
{
    return this->VisitObjectEnd(instrDef);
}

FunctorCode FunctorInterface::VisitLabel(Label *label)
{
    return this->VisitObject(label);
}

FunctorCode FunctorInterface::VisitLabelEnd(Label *label)
{
    return this->VisitObjectEnd(label);
}

FunctorCode FunctorInterface::VisitLabelAbbr(LabelAbbr *labelAbbr)
{
    return this->VisitObject(labelAbbr);
}

FunctorCode FunctorInterface::VisitLabelAbbrEnd(LabelAbbr *labelAbbr)
{
    return this->VisitObjectEnd(labelAbbr);
}

FunctorCode FunctorInterface::VisitLayer(Layer *layer)
{
    return this->VisitObject(layer);
}

FunctorCode FunctorInterface::VisitLayerEnd(Layer *layer)
{
    return this->VisitObjectEnd(layer);
}

FunctorCode FunctorInterface::VisitLayerDef(LayerDef *layerDef)
{
    return this->VisitObject(layerDef);
}

FunctorCode FunctorInterface::VisitLayerDefEnd(LayerDef *layerDef)
{
    return this->VisitObjectEnd(layerDef);
}

FunctorCode FunctorInterface::VisitMeasure(Measure *measure)
{
    return this->VisitObject(measure);
}

FunctorCode FunctorInterface::VisitMeasureEnd(Measure *measure)
{
    return this->VisitObjectEnd(measure);
}

FunctorCode FunctorInterface::VisitPage(Page *page)
{
    return this->VisitObject(page);
}

FunctorCode FunctorInterface::VisitPageEnd(Page *page)
{
    return this->VisitObjectEnd(page);
}

FunctorCode FunctorInterface::VisitPages(Pages *pages)
{
    return this->VisitObject(pages);
}

FunctorCode FunctorInterface::VisitPagesEnd(Pages *pages)
{
    return this->VisitObjectEnd(pages);
}

FunctorCode FunctorInterface::VisitScoreDef(ScoreDef *scoreDef)
{
    return this->VisitScoreDefElement(scoreDef);
}

FunctorCode FunctorInterface::VisitScoreDefEnd(ScoreDef *scoreDef)
{
    return this->VisitScoreDefElementEnd(scoreDef);
}

FunctorCode FunctorInterface::VisitScoreDefElement(ScoreDefElement *scoreDefElement)
{
    return this->VisitObject(scoreDefElement);
}

FunctorCode FunctorInterface::VisitScoreDefElementEnd(ScoreDefElement *scoreDefElement)
{
    return this->VisitObjectEnd(scoreDefElement);
}

FunctorCode FunctorInterface::VisitStaff(Staff *staff)
{
    return this->VisitObject(staff);
}

FunctorCode FunctorInterface::VisitStaffEnd(Staff *staff)
{
    return this->VisitObjectEnd(staff);
}

FunctorCode FunctorInterface::VisitStaffDef(StaffDef *staffDef)
{
    return this->VisitScoreDefElement(staffDef);
}

FunctorCode FunctorInterface::VisitStaffDefEnd(StaffDef *staffDef)
{
    return this->VisitScoreDefElementEnd(staffDef);
}

FunctorCode FunctorInterface::VisitStaffGrp(StaffGrp *staffGrp)
{
    return this->VisitObject(staffGrp);
}

FunctorCode FunctorInterface::VisitStaffGrpEnd(StaffGrp *staffGrp)
{
    return this->VisitObjectEnd(staffGrp);
}

FunctorCode FunctorInterface::VisitSystem(System *system)
{
    return this->VisitObject(system);
}

FunctorCode FunctorInterface::VisitSystemEnd(System *system)
{
    return this->VisitObjectEnd(system);
}

FunctorCode FunctorInterface::VisitTuning(Tuning *tuning)
{
    return this->VisitObject(tuning);
}

FunctorCode FunctorInterface::VisitTuningEnd(Tuning *tuning)
{
    return this->VisitObjectEnd(tuning);
}

FunctorCode FunctorInterface::VisitMdiv(Mdiv *mdiv)
{
    return this->VisitPageElement(mdiv);
}

FunctorCode FunctorInterface::VisitMdivEnd(Mdiv *mdiv)
{
    return this->VisitPageElementEnd(mdiv);
}

FunctorCode FunctorInterface::VisitPageElement(PageElement *pageElement)
{
    return this->VisitObject(pageElement);
}

FunctorCode FunctorInterface::VisitPageElementEnd(PageElement *pageElement)
{
    return this->VisitObjectEnd(pageElement);
}

FunctorCode FunctorInterface::VisitPageMilestone(PageMilestoneEnd *pageMilestoneEnd)
{
    return this->VisitPageElement(pageMilestoneEnd);
}

FunctorCode FunctorInterface::VisitPageMilestoneEnd(PageMilestoneEnd *pageMilestoneEnd)
{
    return this->VisitPageElementEnd(pageMilestoneEnd);
}

FunctorCode FunctorInterface::VisitScore(Score *score)
{
    return this->VisitPageElement(score);
}

FunctorCode FunctorInterface::VisitScoreEnd(Score *score)
{
    return this->VisitPageElementEnd(score);
}

FunctorCode FunctorInterface::VisitAccid(Accid *accid)
{
    return this->VisitLayerElement(accid);
}

FunctorCode FunctorInterface::VisitAccidEnd(Accid *accid)
{
    return this->VisitLayerElementEnd(accid);
}

FunctorCode FunctorInterface::VisitArtic(Artic *artic)
{
    return this->VisitLayerElement(artic);
}

FunctorCode FunctorInterface::VisitArticEnd(Artic *artic)
{
    return this->VisitLayerElementEnd(artic);
}

FunctorCode FunctorInterface::VisitBarLine(BarLine *barLine)
{
    return this->VisitLayerElement(barLine);
}

FunctorCode FunctorInterface::VisitBarLineEnd(BarLine *barLine)
{
    return this->VisitLayerElementEnd(barLine);
}

FunctorCode FunctorInterface::VisitBeam(Beam *beam)
{
    return this->VisitLayerElement(beam);
}

FunctorCode FunctorInterface::VisitBeamEnd(Beam *beam)
{
    return this->VisitLayerElementEnd(beam);
}

FunctorCode FunctorInterface::VisitBeatRpt(BeatRpt *beatRpt)
{
    return this->VisitLayerElement(beatRpt);
}

FunctorCode FunctorInterface::VisitBeatRptEnd(BeatRpt *beatRpt)
{
    return this->VisitLayerElementEnd(beatRpt);
}

FunctorCode FunctorInterface::VisitBTrem(BTrem *bTrem)
{
    return this->VisitLayerElement(bTrem);
}

FunctorCode FunctorInterface::VisitBTremEnd(BTrem *bTrem)
{
    return this->VisitLayerElementEnd(bTrem);
}

FunctorCode FunctorInterface::VisitChord(Chord *chord)
{
    return this->VisitLayerElement(chord);
}

FunctorCode FunctorInterface::VisitChordEnd(Chord *chord)
{
    return this->VisitLayerElementEnd(chord);
}

FunctorCode FunctorInterface::VisitClef(Clef *clef)
{
    return this->VisitLayerElement(clef);
}

FunctorCode FunctorInterface::VisitClefEnd(Clef *clef)
{
    return this->VisitLayerElementEnd(clef);
}

FunctorCode FunctorInterface::VisitCustos(Custos *custos)
{
    return this->VisitLayerElement(custos);
}

FunctorCode FunctorInterface::VisitCustosEnd(Custos *custos)
{
    return this->VisitLayerElementEnd(custos);
}

FunctorCode FunctorInterface::VisitDot(Dot *dot)
{
    return this->VisitLayerElement(dot);
}

FunctorCode FunctorInterface::VisitDotEnd(Dot *dot)
{
    return this->VisitLayerElementEnd(dot);
}

FunctorCode FunctorInterface::VisitDots(Dots *dots)
{
    return this->VisitLayerElement(dots);
}

FunctorCode FunctorInterface::VisitDotsEnd(Dots *dots)
{
    return this->VisitLayerElementEnd(dots);
}

FunctorCode FunctorInterface::VisitFlag(Flag *flag)
{
    return this->VisitLayerElement(flag);
}

FunctorCode FunctorInterface::VisitFlagEnd(Flag *flag)
{
    return this->VisitLayerElementEnd(flag);
}

FunctorCode FunctorInterface::VisitFTrem(FTrem *fTrem)
{
    return this->VisitLayerElement(fTrem);
}

FunctorCode FunctorInterface::VisitFTremEnd(FTrem *fTrem)
{
    return this->VisitLayerElementEnd(fTrem);
}

FunctorCode FunctorInterface::VisitGraceGrp(GraceGrp *graceGrp)
{
    return this->VisitLayerElement(graceGrp);
}

FunctorCode FunctorInterface::VisitGraceGrpEnd(GraceGrp *graceGrp)
{
    return this->VisitLayerElementEnd(graceGrp);
}

FunctorCode FunctorInterface::VisitHalfmRpt(HalfmRpt *halfmRpt)
{
    return this->VisitLayerElement(halfmRpt);
}

FunctorCode FunctorInterface::VisitHalfmRptEnd(HalfmRpt *halfmRpt)
{
    return this->VisitLayerElementEnd(halfmRpt);
}

FunctorCode FunctorInterface::VisitKeyAccid(KeyAccid *keyAccid)
{
    return this->VisitLayerElement(keyAccid);
}

FunctorCode FunctorInterface::VisitKeyAccidEnd(KeyAccid *keyAccid)
{
    return this->VisitLayerElementEnd(keyAccid);
}

FunctorCode FunctorInterface::VisitKeySig(KeySig *keySig)
{
    return this->VisitLayerElement(keySig);
}

FunctorCode FunctorInterface::VisitKeySigEnd(KeySig *keySig)
{
    return this->VisitLayerElementEnd(keySig);
}

FunctorCode FunctorInterface::VisitLayerElement(LayerElement *layerElement)
{
    return this->VisitObject(layerElement);
}

FunctorCode FunctorInterface::VisitLayerElementEnd(LayerElement *layerElement)
{
    return this->VisitObjectEnd(layerElement);
}

FunctorCode FunctorInterface::VisitLigature(Ligature *ligature)
{
    return this->VisitLayerElement(ligature);
}

FunctorCode FunctorInterface::VisitLigatureEnd(Ligature *ligature)
{
    return this->VisitLayerElementEnd(ligature);
}

FunctorCode FunctorInterface::VisitMensur(Mensur *mensur)
{
    return this->VisitLayerElement(mensur);
}

FunctorCode FunctorInterface::VisitMensurEnd(Mensur *mensur)
{
    return this->VisitLayerElementEnd(mensur);
}

FunctorCode FunctorInterface::VisitMeterSig(MeterSig *meterSig)
{
    return this->VisitLayerElement(meterSig);
}

FunctorCode FunctorInterface::VisitMeterSigEnd(MeterSig *meterSig)
{
    return this->VisitLayerElementEnd(meterSig);
}

FunctorCode FunctorInterface::VisitMeterSigGrp(MeterSigGrp *meterSigGrp)
{
    return this->VisitObject(meterSigGrp);
}

FunctorCode FunctorInterface::VisitMeterSigGrpEnd(MeterSigGrp *meterSigGrp)
{
    return this->VisitObjectEnd(meterSigGrp);
}

FunctorCode FunctorInterface::VisitMRest(MRest *mRest)
{
    return this->VisitLayerElement(mRest);
}

FunctorCode FunctorInterface::VisitMRestEnd(MRest *mRest)
{
    return this->VisitLayerElementEnd(mRest);
}

FunctorCode FunctorInterface::VisitMRpt(MRpt *mRpt)
{
    return this->VisitLayerElement(mRpt);
}

FunctorCode FunctorInterface::VisitMRptEnd(MRpt *mRpt)
{
    return this->VisitLayerElementEnd(mRpt);
}

FunctorCode FunctorInterface::VisitMRpt2(MRpt2 *mRpt2)
{
    return this->VisitLayerElement(mRpt2);
}

FunctorCode FunctorInterface::VisitMRpt2End(MRpt2 *mRpt2)
{
    return this->VisitLayerElementEnd(mRpt2);
}

FunctorCode FunctorInterface::VisitMSpace(MSpace *mSpace)
{
    return this->VisitLayerElement(mSpace);
}

FunctorCode FunctorInterface::VisitMSpaceEnd(MSpace *mSpace)
{
    return this->VisitLayerElementEnd(mSpace);
}

FunctorCode FunctorInterface::VisitMultiRest(MultiRest *multiRest)
{
    return this->VisitLayerElement(multiRest);
}

FunctorCode FunctorInterface::VisitMultiRestEnd(MultiRest *multiRest)
{
    return this->VisitLayerElementEnd(multiRest);
}

FunctorCode FunctorInterface::VisitMultiRpt(MultiRpt *multiRpt)
{
    return this->VisitLayerElement(multiRpt);
}

FunctorCode FunctorInterface::VisitMultiRptEnd(MultiRpt *multiRpt)
{
    return this->VisitLayerElementEnd(multiRpt);
}

FunctorCode FunctorInterface::VisitNc(Nc *nc)
{
    return this->VisitLayerElement(nc);
}

FunctorCode FunctorInterface::VisitNcEnd(Nc *nc)
{
    return this->VisitLayerElementEnd(nc);
}

FunctorCode FunctorInterface::VisitNeume(Neume *neume)
{
    return this->VisitLayerElement(neume);
}

FunctorCode FunctorInterface::VisitNeumeEnd(Neume *neume)
{
    return this->VisitLayerElementEnd(neume);
}

FunctorCode FunctorInterface::VisitNote(Note *note)
{
    return this->VisitLayerElement(note);
}

FunctorCode FunctorInterface::VisitNoteEnd(Note *note)
{
    return this->VisitLayerElementEnd(note);
}

FunctorCode FunctorInterface::VisitPlica(Plica *plica)
{
    return this->VisitLayerElement(plica);
}

FunctorCode FunctorInterface::VisitPlicaEnd(Plica *plica)
{
    return this->VisitLayerElementEnd(plica);
}

FunctorCode FunctorInterface::VisitProport(Proport *proport)
{
    return this->VisitLayerElement(proport);
}

FunctorCode FunctorInterface::VisitProportEnd(Proport *proport)
{
    return this->VisitLayerElementEnd(proport);
}

FunctorCode FunctorInterface::VisitRest(Rest *rest)
{
    return this->VisitLayerElement(rest);
}

FunctorCode FunctorInterface::VisitRestEnd(Rest *rest)
{
    return this->VisitLayerElementEnd(rest);
}

FunctorCode FunctorInterface::VisitSpace(Space *space)
{
    return this->VisitLayerElement(space);
}

FunctorCode FunctorInterface::VisitSpaceEnd(Space *space)
{
    return this->VisitLayerElementEnd(space);
}

FunctorCode FunctorInterface::VisitStem(Stem *stem)
{
    return this->VisitLayerElement(stem);
}

FunctorCode FunctorInterface::VisitStemEnd(Stem *stem)
{
    return this->VisitLayerElementEnd(stem);
}

FunctorCode FunctorInterface::VisitSyl(Syl *syl)
{
    return this->VisitLayerElement(syl);
}

FunctorCode FunctorInterface::VisitSylEnd(Syl *syl)
{
    return this->VisitLayerElementEnd(syl);
}

FunctorCode FunctorInterface::VisitSyllable(Syllable *syllable)
{
    return this->VisitLayerElement(syllable);
}

FunctorCode FunctorInterface::VisitSyllableEnd(Syllable *syllable)
{
    return this->VisitLayerElementEnd(syllable);
}

FunctorCode FunctorInterface::VisitTabDurSym(TabDurSym *tabDurSym)
{
    return this->VisitLayerElement(tabDurSym);
}

FunctorCode FunctorInterface::VisitTabDurSymEnd(TabDurSym *tabDurSym)
{
    return this->VisitLayerElementEnd(tabDurSym);
}

FunctorCode FunctorInterface::VisitTabGrp(TabGrp *tabGrp)
{
    return this->VisitLayerElement(tabGrp);
}

FunctorCode FunctorInterface::VisitTabGrpEnd(TabGrp *tabGrp)
{
    return this->VisitLayerElementEnd(tabGrp);
}

FunctorCode FunctorInterface::VisitTimestamp(TimestampAttr *timeStamp)
{
    return this->VisitLayerElement(timeStamp);
}

FunctorCode FunctorInterface::VisitTimestampEnd(TimestampAttr *timeStamp)
{
    return this->VisitLayerElementEnd(timeStamp);
}

FunctorCode FunctorInterface::VisitTuplet(Tuplet *tuplet)
{
    return this->VisitLayerElement(tuplet);
}

FunctorCode FunctorInterface::VisitTupletEnd(Tuplet *tuplet)
{
    return this->VisitLayerElementEnd(tuplet);
}

FunctorCode FunctorInterface::VisitTupletBracket(TupletBracket *tupletBracket)
{
    return this->VisitLayerElement(tupletBracket);
}

FunctorCode FunctorInterface::VisitTupletBracketEnd(TupletBracket *tupletBracket)
{
    return this->VisitLayerElementEnd(tupletBracket);
}

FunctorCode FunctorInterface::VisitTupletNum(TupletNum *tupletNum)
{
    return this->VisitLayerElement(tupletNum);
}

FunctorCode FunctorInterface::VisitTupletNumEnd(TupletNum *tupletNum)
{
    return this->VisitLayerElementEnd(tupletNum);
}

FunctorCode FunctorInterface::VisitVerse(Verse *verse)
{
    return this->VisitLayerElement(verse);
}

FunctorCode FunctorInterface::VisitVerseEnd(Verse *verse)
{
    return this->VisitLayerElementEnd(verse);
}

//----------------------------------------------------------------------------
// ConstFunctorInterface
//----------------------------------------------------------------------------

FunctorCode ConstFunctorInterface::VisitDoc(const Doc *doc)
{
    return this->VisitObject(doc);
}

FunctorCode ConstFunctorInterface::VisitDocEnd(const Doc *doc)
{
    return this->VisitObjectEnd(doc);
}

FunctorCode ConstFunctorInterface::VisitCourse(const Course *course)
{
    return this->VisitObject(course);
}

FunctorCode ConstFunctorInterface::VisitCourseEnd(const Course *course)
{
    return this->VisitObjectEnd(course);
}

FunctorCode ConstFunctorInterface::VisitGrpSym(const GrpSym *grpSym)
{
    return this->VisitObject(grpSym);
}

FunctorCode ConstFunctorInterface::VisitGrpSymEnd(const GrpSym *grpSym)
{
    return this->VisitObjectEnd(grpSym);
}

FunctorCode ConstFunctorInterface::VisitInstrDef(const InstrDef *instrDef)
{
    return this->VisitObject(instrDef);
}

FunctorCode ConstFunctorInterface::VisitInstrDefEnd(const InstrDef *instrDef)
{
    return this->VisitObjectEnd(instrDef);
}

FunctorCode ConstFunctorInterface::VisitLabel(const Label *label)
{
    return this->VisitObject(label);
}

FunctorCode ConstFunctorInterface::VisitLabelEnd(const Label *label)
{
    return this->VisitObjectEnd(label);
}

FunctorCode ConstFunctorInterface::VisitLabelAbbr(const LabelAbbr *labelAbbr)
{
    return this->VisitObject(labelAbbr);
}

FunctorCode ConstFunctorInterface::VisitLabelAbbrEnd(const LabelAbbr *labelAbbr)
{
    return this->VisitObjectEnd(labelAbbr);
}

FunctorCode ConstFunctorInterface::VisitLayer(const Layer *layer)
{
    return this->VisitObject(layer);
}

FunctorCode ConstFunctorInterface::VisitLayerEnd(const Layer *layer)
{
    return this->VisitObjectEnd(layer);
}

FunctorCode ConstFunctorInterface::VisitLayerDef(const LayerDef *layerDef)
{
    return this->VisitObject(layerDef);
}

FunctorCode ConstFunctorInterface::VisitLayerDefEnd(const LayerDef *layerDef)
{
    return this->VisitObjectEnd(layerDef);
}

FunctorCode ConstFunctorInterface::VisitMeasure(const Measure *measure)
{
    return this->VisitObject(measure);
}

FunctorCode ConstFunctorInterface::VisitMeasureEnd(const Measure *measure)
{
    return this->VisitObjectEnd(measure);
}

FunctorCode ConstFunctorInterface::VisitPage(const Page *page)
{
    return this->VisitObject(page);
}

FunctorCode ConstFunctorInterface::VisitPageEnd(const Page *page)
{
    return this->VisitObjectEnd(page);
}

FunctorCode ConstFunctorInterface::VisitPages(const Pages *pages)
{
    return this->VisitObject(pages);
}

FunctorCode ConstFunctorInterface::VisitPagesEnd(const Pages *pages)
{
    return this->VisitObjectEnd(pages);
}

FunctorCode ConstFunctorInterface::VisitScoreDef(const ScoreDef *scoreDef)
{
    return this->VisitScoreDefElement(scoreDef);
}

FunctorCode ConstFunctorInterface::VisitScoreDefEnd(const ScoreDef *scoreDef)
{
    return this->VisitScoreDefElementEnd(scoreDef);
}

FunctorCode ConstFunctorInterface::VisitScoreDefElement(const ScoreDefElement *scoreDefElement)
{
    return this->VisitObject(scoreDefElement);
}

FunctorCode ConstFunctorInterface::VisitScoreDefElementEnd(const ScoreDefElement *scoreDefElement)
{
    return this->VisitObjectEnd(scoreDefElement);
}

FunctorCode ConstFunctorInterface::VisitStaff(const Staff *staff)
{
    return this->VisitObject(staff);
}

FunctorCode ConstFunctorInterface::VisitStaffEnd(const Staff *staff)
{
    return this->VisitObjectEnd(staff);
}

FunctorCode ConstFunctorInterface::VisitStaffDef(const StaffDef *staffDef)
{
    return this->VisitScoreDefElement(staffDef);
}

FunctorCode ConstFunctorInterface::VisitStaffDefEnd(const StaffDef *staffDef)
{
    return this->VisitScoreDefElementEnd(staffDef);
}

FunctorCode ConstFunctorInterface::VisitStaffGrp(const StaffGrp *staffGrp)
{
    return this->VisitObject(staffGrp);
}

FunctorCode ConstFunctorInterface::VisitStaffGrpEnd(const StaffGrp *staffGrp)
{
    return this->VisitObjectEnd(staffGrp);
}

FunctorCode ConstFunctorInterface::VisitSystem(const System *system)
{
    return this->VisitObject(system);
}

FunctorCode ConstFunctorInterface::VisitSystemEnd(const System *system)
{
    return this->VisitObjectEnd(system);
}

FunctorCode ConstFunctorInterface::VisitTuning(const Tuning *tuning)
{
    return this->VisitObject(tuning);
}

FunctorCode ConstFunctorInterface::VisitTuningEnd(const Tuning *tuning)
{
    return this->VisitObjectEnd(tuning);
}

FunctorCode ConstFunctorInterface::VisitMdiv(const Mdiv *mdiv)
{
    return this->VisitPageElement(mdiv);
}

FunctorCode ConstFunctorInterface::VisitMdivEnd(const Mdiv *mdiv)
{
    return this->VisitPageElementEnd(mdiv);
}

FunctorCode ConstFunctorInterface::VisitPageElement(const PageElement *pageElement)
{
    return this->VisitObject(pageElement);
}

FunctorCode ConstFunctorInterface::VisitPageElementEnd(const PageElement *pageElement)
{
    return this->VisitObjectEnd(pageElement);
}

FunctorCode ConstFunctorInterface::VisitPageMilestone(const PageMilestoneEnd *pageMilestoneEnd)
{
    return this->VisitPageElement(pageMilestoneEnd);
}

FunctorCode ConstFunctorInterface::VisitPageMilestoneEnd(const PageMilestoneEnd *pageMilestoneEnd)
{
    return this->VisitPageElementEnd(pageMilestoneEnd);
}

FunctorCode ConstFunctorInterface::VisitScore(const Score *score)
{
    return this->VisitPageElement(score);
}

FunctorCode ConstFunctorInterface::VisitScoreEnd(const Score *score)
{
    return this->VisitPageElementEnd(score);
}

FunctorCode ConstFunctorInterface::VisitAccid(const Accid *accid)
{
    return this->VisitLayerElement(accid);
}

FunctorCode ConstFunctorInterface::VisitAccidEnd(const Accid *accid)
{
    return this->VisitLayerElementEnd(accid);
}

FunctorCode ConstFunctorInterface::VisitArtic(const Artic *artic)
{
    return this->VisitLayerElement(artic);
}

FunctorCode ConstFunctorInterface::VisitArticEnd(const Artic *artic)
{
    return this->VisitLayerElementEnd(artic);
}

FunctorCode ConstFunctorInterface::VisitBarLine(const BarLine *barLine)
{
    return this->VisitLayerElement(barLine);
}

FunctorCode ConstFunctorInterface::VisitBarLineEnd(const BarLine *barLine)
{
    return this->VisitLayerElementEnd(barLine);
}

FunctorCode ConstFunctorInterface::VisitBeam(const Beam *beam)
{
    return this->VisitLayerElement(beam);
}

FunctorCode ConstFunctorInterface::VisitBeamEnd(const Beam *beam)
{
    return this->VisitLayerElementEnd(beam);
}

FunctorCode ConstFunctorInterface::VisitBeatRpt(const BeatRpt *beatRpt)
{
    return this->VisitLayerElement(beatRpt);
}

FunctorCode ConstFunctorInterface::VisitBeatRptEnd(const BeatRpt *beatRpt)
{
    return this->VisitLayerElementEnd(beatRpt);
}

FunctorCode ConstFunctorInterface::VisitBTrem(const BTrem *bTrem)
{
    return this->VisitLayerElement(bTrem);
}

FunctorCode ConstFunctorInterface::VisitBTremEnd(const BTrem *bTrem)
{
    return this->VisitLayerElementEnd(bTrem);
}

FunctorCode ConstFunctorInterface::VisitChord(const Chord *chord)
{
    return this->VisitLayerElement(chord);
}

FunctorCode ConstFunctorInterface::VisitChordEnd(const Chord *chord)
{
    return this->VisitLayerElementEnd(chord);
}

FunctorCode ConstFunctorInterface::VisitClef(const Clef *clef)
{
    return this->VisitLayerElement(clef);
}

FunctorCode ConstFunctorInterface::VisitClefEnd(const Clef *clef)
{
    return this->VisitLayerElementEnd(clef);
}

FunctorCode ConstFunctorInterface::VisitCustos(const Custos *custos)
{
    return this->VisitLayerElement(custos);
}

FunctorCode ConstFunctorInterface::VisitCustosEnd(const Custos *custos)
{
    return this->VisitLayerElementEnd(custos);
}

FunctorCode ConstFunctorInterface::VisitDot(const Dot *dot)
{
    return this->VisitLayerElement(dot);
}

FunctorCode ConstFunctorInterface::VisitDotEnd(const Dot *dot)
{
    return this->VisitLayerElementEnd(dot);
}

FunctorCode ConstFunctorInterface::VisitDots(const Dots *dots)
{
    return this->VisitLayerElement(dots);
}

FunctorCode ConstFunctorInterface::VisitDotsEnd(const Dots *dots)
{
    return this->VisitLayerElementEnd(dots);
}

FunctorCode ConstFunctorInterface::VisitFlag(const Flag *flag)
{
    return this->VisitLayerElement(flag);
}

FunctorCode ConstFunctorInterface::VisitFlagEnd(const Flag *flag)
{
    return this->VisitLayerElementEnd(flag);
}

FunctorCode ConstFunctorInterface::VisitFTrem(const FTrem *fTrem)
{
    return this->VisitLayerElement(fTrem);
}

FunctorCode ConstFunctorInterface::VisitFTremEnd(const FTrem *fTrem)
{
    return this->VisitLayerElementEnd(fTrem);
}

FunctorCode ConstFunctorInterface::VisitGraceGrp(const GraceGrp *graceGrp)
{
    return this->VisitLayerElement(graceGrp);
}

FunctorCode ConstFunctorInterface::VisitGraceGrpEnd(const GraceGrp *graceGrp)
{
    return this->VisitLayerElementEnd(graceGrp);
}

FunctorCode ConstFunctorInterface::VisitHalfmRpt(const HalfmRpt *halfmRpt)
{
    return this->VisitLayerElement(halfmRpt);
}

FunctorCode ConstFunctorInterface::VisitHalfmRptEnd(const HalfmRpt *halfmRpt)
{
    return this->VisitLayerElementEnd(halfmRpt);
}

FunctorCode ConstFunctorInterface::VisitKeyAccid(const KeyAccid *keyAccid)
{
    return this->VisitLayerElement(keyAccid);
}

FunctorCode ConstFunctorInterface::VisitKeyAccidEnd(const KeyAccid *keyAccid)
{
    return this->VisitLayerElementEnd(keyAccid);
}

FunctorCode ConstFunctorInterface::VisitKeySig(const KeySig *keySig)
{
    return this->VisitLayerElement(keySig);
}

FunctorCode ConstFunctorInterface::VisitKeySigEnd(const KeySig *keySig)
{
    return this->VisitLayerElementEnd(keySig);
}

FunctorCode ConstFunctorInterface::VisitLayerElement(const LayerElement *layerElement)
{
    return this->VisitObject(layerElement);
}

FunctorCode ConstFunctorInterface::VisitLayerElementEnd(const LayerElement *layerElement)
{
    return this->VisitObjectEnd(layerElement);
}

FunctorCode ConstFunctorInterface::VisitLigature(const Ligature *ligature)
{
    return this->VisitLayerElement(ligature);
}

FunctorCode ConstFunctorInterface::VisitLigatureEnd(const Ligature *ligature)
{
    return this->VisitLayerElementEnd(ligature);
}

FunctorCode ConstFunctorInterface::VisitMensur(const Mensur *mensur)
{
    return this->VisitLayerElement(mensur);
}

FunctorCode ConstFunctorInterface::VisitMensurEnd(const Mensur *mensur)
{
    return this->VisitLayerElementEnd(mensur);
}

FunctorCode ConstFunctorInterface::VisitMeterSig(const MeterSig *meterSig)
{
    return this->VisitLayerElement(meterSig);
}

FunctorCode ConstFunctorInterface::VisitMeterSigEnd(const MeterSig *meterSig)
{
    return this->VisitLayerElementEnd(meterSig);
}

FunctorCode ConstFunctorInterface::VisitMeterSigGrp(const MeterSigGrp *meterSigGrp)
{
    return this->VisitObject(meterSigGrp);
}

FunctorCode ConstFunctorInterface::VisitMeterSigGrpEnd(const MeterSigGrp *meterSigGrp)
{
    return this->VisitObjectEnd(meterSigGrp);
}

FunctorCode ConstFunctorInterface::VisitMRest(const MRest *mRest)
{
    return this->VisitLayerElement(mRest);
}

FunctorCode ConstFunctorInterface::VisitMRestEnd(const MRest *mRest)
{
    return this->VisitLayerElementEnd(mRest);
}

FunctorCode ConstFunctorInterface::VisitMRpt(const MRpt *mRpt)
{
    return this->VisitLayerElement(mRpt);
}

FunctorCode ConstFunctorInterface::VisitMRptEnd(const MRpt *mRpt)
{
    return this->VisitLayerElementEnd(mRpt);
}

FunctorCode ConstFunctorInterface::VisitMRpt2(const MRpt2 *mRpt2)
{
    return this->VisitLayerElement(mRpt2);
}

FunctorCode ConstFunctorInterface::VisitMRpt2End(const MRpt2 *mRpt2)
{
    return this->VisitLayerElementEnd(mRpt2);
}

FunctorCode ConstFunctorInterface::VisitMSpace(const MSpace *mSpace)
{
    return this->VisitLayerElement(mSpace);
}

FunctorCode ConstFunctorInterface::VisitMSpaceEnd(const MSpace *mSpace)
{
    return this->VisitLayerElementEnd(mSpace);
}

FunctorCode ConstFunctorInterface::VisitMultiRest(const MultiRest *multiRest)
{
    return this->VisitLayerElement(multiRest);
}

FunctorCode ConstFunctorInterface::VisitMultiRestEnd(const MultiRest *multiRest)
{
    return this->VisitLayerElementEnd(multiRest);
}

FunctorCode ConstFunctorInterface::VisitMultiRpt(const MultiRpt *multiRpt)
{
    return this->VisitLayerElement(multiRpt);
}

FunctorCode ConstFunctorInterface::VisitMultiRptEnd(const MultiRpt *multiRpt)
{
    return this->VisitLayerElementEnd(multiRpt);
}

FunctorCode ConstFunctorInterface::VisitNc(const Nc *nc)
{
    return this->VisitLayerElement(nc);
}

FunctorCode ConstFunctorInterface::VisitNcEnd(const Nc *nc)
{
    return this->VisitLayerElementEnd(nc);
}

FunctorCode ConstFunctorInterface::VisitNeume(const Neume *neume)
{
    return this->VisitLayerElement(neume);
}

FunctorCode ConstFunctorInterface::VisitNeumeEnd(const Neume *neume)
{
    return this->VisitLayerElementEnd(neume);
}

FunctorCode ConstFunctorInterface::VisitNote(const Note *note)
{
    return this->VisitLayerElement(note);
}

FunctorCode ConstFunctorInterface::VisitNoteEnd(const Note *note)
{
    return this->VisitLayerElementEnd(note);
}

FunctorCode ConstFunctorInterface::VisitPlica(const Plica *plica)
{
    return this->VisitLayerElement(plica);
}

FunctorCode ConstFunctorInterface::VisitPlicaEnd(const Plica *plica)
{
    return this->VisitLayerElementEnd(plica);
}

FunctorCode ConstFunctorInterface::VisitProport(const Proport *proport)
{
    return this->VisitLayerElement(proport);
}

FunctorCode ConstFunctorInterface::VisitProportEnd(const Proport *proport)
{
    return this->VisitLayerElementEnd(proport);
}

FunctorCode ConstFunctorInterface::VisitRest(const Rest *rest)
{
    return this->VisitLayerElement(rest);
}

FunctorCode ConstFunctorInterface::VisitRestEnd(const Rest *rest)
{
    return this->VisitLayerElementEnd(rest);
}

FunctorCode ConstFunctorInterface::VisitSpace(const Space *space)
{
    return this->VisitLayerElement(space);
}

FunctorCode ConstFunctorInterface::VisitSpaceEnd(const Space *space)
{
    return this->VisitLayerElementEnd(space);
}

FunctorCode ConstFunctorInterface::VisitStem(const Stem *stem)
{
    return this->VisitLayerElement(stem);
}

FunctorCode ConstFunctorInterface::VisitStemEnd(const Stem *stem)
{
    return this->VisitLayerElementEnd(stem);
}

FunctorCode ConstFunctorInterface::VisitSyl(const Syl *syl)
{
    return this->VisitLayerElement(syl);
}

FunctorCode ConstFunctorInterface::VisitSylEnd(const Syl *syl)
{
    return this->VisitLayerElementEnd(syl);
}

FunctorCode ConstFunctorInterface::VisitSyllable(const Syllable *syllable)
{
    return this->VisitLayerElement(syllable);
}

FunctorCode ConstFunctorInterface::VisitSyllableEnd(const Syllable *syllable)
{
    return this->VisitLayerElementEnd(syllable);
}

FunctorCode ConstFunctorInterface::VisitTabDurSym(const TabDurSym *tabDurSym)
{
    return this->VisitLayerElement(tabDurSym);
}

FunctorCode ConstFunctorInterface::VisitTabDurSymEnd(const TabDurSym *tabDurSym)
{
    return this->VisitLayerElementEnd(tabDurSym);
}

FunctorCode ConstFunctorInterface::VisitTabGrp(const TabGrp *tabGrp)
{
    return this->VisitLayerElement(tabGrp);
}

FunctorCode ConstFunctorInterface::VisitTabGrpEnd(const TabGrp *tabGrp)
{
    return this->VisitLayerElementEnd(tabGrp);
}

FunctorCode ConstFunctorInterface::VisitTimestamp(const TimestampAttr *timeStamp)
{
    return this->VisitLayerElement(timeStamp);
}

FunctorCode ConstFunctorInterface::VisitTimestampEnd(const TimestampAttr *timeStamp)
{
    return this->VisitLayerElementEnd(timeStamp);
}

FunctorCode ConstFunctorInterface::VisitTuplet(const Tuplet *tuplet)
{
    return this->VisitLayerElement(tuplet);
}

FunctorCode ConstFunctorInterface::VisitTupletEnd(const Tuplet *tuplet)
{
    return this->VisitLayerElementEnd(tuplet);
}

FunctorCode ConstFunctorInterface::VisitTupletBracket(const TupletBracket *tupletBracket)
{
    return this->VisitLayerElement(tupletBracket);
}

FunctorCode ConstFunctorInterface::VisitTupletBracketEnd(const TupletBracket *tupletBracket)
{
    return this->VisitLayerElementEnd(tupletBracket);
}

FunctorCode ConstFunctorInterface::VisitTupletNum(const TupletNum *tupletNum)
{
    return this->VisitLayerElement(tupletNum);
}

FunctorCode ConstFunctorInterface::VisitTupletNumEnd(const TupletNum *tupletNum)
{
    return this->VisitLayerElementEnd(tupletNum);
}

FunctorCode ConstFunctorInterface::VisitVerse(const Verse *verse)
{
    return this->VisitLayerElement(verse);
}

FunctorCode ConstFunctorInterface::VisitVerseEnd(const Verse *verse)
{
    return this->VisitLayerElementEnd(verse);
}

} // namespace vrv

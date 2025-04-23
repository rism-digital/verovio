/////////////////////////////////////////////////////////////////////////////
// Name:        functorinterface.cpp
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "functorinterface.h"

//----------------------------------------------------------------------------

#include "accid.h"
#include "anchoredtext.h"
#include "annotscore.h"
#include "arpeg.h"
#include "artic.h"
#include "barline.h"
#include "beam.h"
#include "beamspan.h"
#include "beatrpt.h"
#include "bracketspan.h"
#include "breath.h"
#include "btrem.h"
#include "caesura.h"
#include "chord.h"
#include "clef.h"
#include "course.h"
#include "cpmark.h"
#include "custos.h"
#include "dir.h"
#include "div.h"
#include "doc.h"
#include "dot.h"
#include "dynam.h"
#include "editorial.h"
#include "elementpart.h"
#include "ending.h"
#include "expansion.h"
#include "f.h"
#include "facsimile.h"
#include "fb.h"
#include "fermata.h"
#include "fig.h"
#include "fing.h"
#include "ftrem.h"
#include "genericlayerelement.h"
#include "gliss.h"
#include "gracegrp.h"
#include "graphic.h"
#include "grpsym.h"
#include "hairpin.h"
#include "halfmrpt.h"
#include "harm.h"
#include "instrdef.h"
#include "keyaccid.h"
#include "keysig.h"
#include "label.h"
#include "labelabbr.h"
#include "layer.h"
#include "layerdef.h"
#include "lb.h"
#include "ligature.h"
#include "lv.h"
#include "mdiv.h"
#include "measure.h"
#include "mensur.h"
#include "metersig.h"
#include "metersiggrp.h"
#include "mnum.h"
#include "mordent.h"
#include "mrest.h"
#include "mrpt.h"
#include "mrpt2.h"
#include "mspace.h"
#include "multirest.h"
#include "multirpt.h"
#include "nc.h"
#include "neume.h"
#include "note.h"
#include "num.h"
#include "octave.h"
#include "ornam.h"
#include "page.h"
#include "pagemilestone.h"
#include "pages.h"
#include "pb.h"
#include "pedal.h"
#include "pgfoot.h"
#include "pghead.h"
#include "phrase.h"
#include "pitchinflection.h"
#include "plica.h"
#include "proport.h"
#include "reh.h"
#include "rend.h"
#include "repeatmark.h"
#include "rest.h"
#include "sb.h"
#include "score.h"
#include "scoredef.h"
#include "section.h"
#include "slur.h"
#include "space.h"
#include "staff.h"
#include "staffdef.h"
#include "staffgrp.h"
#include "stem.h"
#include "surface.h"
#include "svg.h"
#include "syl.h"
#include "syllable.h"
#include "symbol.h"
#include "system.h"
#include "systemmilestone.h"
#include "tabdursym.h"
#include "tabgrp.h"
#include "tempo.h"
#include "text.h"
#include "textlayoutelement.h"
#include "tie.h"
#include "timestamp.h"
#include "trill.h"
#include "tuning.h"
#include "tuplet.h"
#include "turn.h"
#include "verse.h"
#include "zone.h"

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

FunctorCode FunctorInterface::VisitPb(Pb *pb)
{
    return this->VisitSystemElement(pb);
}

FunctorCode FunctorInterface::VisitPbEnd(Pb *pb)
{
    return this->VisitSystemElementEnd(pb);
}

FunctorCode FunctorInterface::VisitSb(Sb *sb)
{
    return this->VisitSystemElement(sb);
}

FunctorCode FunctorInterface::VisitSbEnd(Sb *sb)
{
    return this->VisitSystemElementEnd(sb);
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

FunctorCode FunctorInterface::VisitEditorialElement(EditorialElement *editorialElement)
{
    return this->VisitObject(editorialElement);
}

FunctorCode FunctorInterface::VisitEditorialElementEnd(EditorialElement *editorialElement)
{
    return this->VisitObjectEnd(editorialElement);
}

FunctorCode FunctorInterface::VisitDiv(Div *div)
{
    return this->VisitTextLayoutElement(div);
}

FunctorCode FunctorInterface::VisitDivEnd(Div *div)
{
    return this->VisitTextLayoutElementEnd(div);
}

FunctorCode FunctorInterface::VisitRunningElement(RunningElement *runningElement)
{
    return this->VisitTextLayoutElement(runningElement);
}

FunctorCode FunctorInterface::VisitRunningElementEnd(RunningElement *runningElement)
{
    return this->VisitTextLayoutElementEnd(runningElement);
}

FunctorCode FunctorInterface::VisitPgHead(PgHead *pgHead)
{
    return this->VisitRunningElement(pgHead);
}

FunctorCode FunctorInterface::VisitPgHeadEnd(PgHead *pgHead)
{
    return this->VisitRunningElementEnd(pgHead);
}

FunctorCode FunctorInterface::VisitPgFoot(PgFoot *pgFoot)
{
    return this->VisitRunningElement(pgFoot);
}

FunctorCode FunctorInterface::VisitPgFootEnd(PgFoot *pgFoot)
{
    return this->VisitRunningElementEnd(pgFoot);
}

FunctorCode FunctorInterface::VisitTextLayoutElement(TextLayoutElement *textLayoutElement)
{
    return this->VisitObject(textLayoutElement);
}

FunctorCode FunctorInterface::VisitTextLayoutElementEnd(TextLayoutElement *textLayoutElement)
{
    return this->VisitObjectEnd(textLayoutElement);
}

FunctorCode FunctorInterface::VisitEnding(Ending *ending)
{
    return this->VisitSystemElement(ending);
}

FunctorCode FunctorInterface::VisitEndingEnd(Ending *ending)
{
    return this->VisitSystemElementEnd(ending);
}

FunctorCode FunctorInterface::VisitExpansion(Expansion *expansion)
{
    return this->VisitSystemElement(expansion);
}

FunctorCode FunctorInterface::VisitExpansionEnd(Expansion *expansion)
{
    return this->VisitSystemElementEnd(expansion);
}

FunctorCode FunctorInterface::VisitSection(Section *section)
{
    return this->VisitSystemElement(section);
}

FunctorCode FunctorInterface::VisitSectionEnd(Section *section)
{
    return this->VisitSystemElementEnd(section);
}

FunctorCode FunctorInterface::VisitSystemElement(SystemElement *systemElement)
{
    return this->VisitFloatingObject(systemElement);
}

FunctorCode FunctorInterface::VisitSystemElementEnd(SystemElement *systemElement)
{
    return this->VisitFloatingObjectEnd(systemElement);
}

FunctorCode FunctorInterface::VisitSystemMilestone(SystemMilestoneEnd *systemMilestoneEnd)
{
    return this->VisitSystemElement(systemMilestoneEnd);
}

FunctorCode FunctorInterface::VisitSystemMilestoneEnd(SystemMilestoneEnd *systemMilestoneEnd)
{
    return this->VisitSystemElementEnd(systemMilestoneEnd);
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

FunctorCode FunctorInterface::VisitAnchoredText(AnchoredText *anchoredText)
{
    return this->VisitControlElement(anchoredText);
}

FunctorCode FunctorInterface::VisitAnchoredTextEnd(AnchoredText *anchoredText)
{
    return this->VisitControlElementEnd(anchoredText);
}

FunctorCode FunctorInterface::VisitAnnotScore(AnnotScore *annotScore)
{
    return this->VisitControlElement(annotScore);
}

FunctorCode FunctorInterface::VisitAnnotScoreEnd(AnnotScore *annotScore)
{
    return this->VisitControlElementEnd(annotScore);
}

FunctorCode FunctorInterface::VisitArpeg(Arpeg *arpeg)
{
    return this->VisitControlElement(arpeg);
}

FunctorCode FunctorInterface::VisitArpegEnd(Arpeg *arpeg)
{
    return this->VisitControlElementEnd(arpeg);
}

FunctorCode FunctorInterface::VisitBeamSpan(BeamSpan *beamSpan)
{
    return this->VisitControlElement(beamSpan);
}

FunctorCode FunctorInterface::VisitBeamSpanEnd(BeamSpan *beamSpan)
{
    return this->VisitControlElementEnd(beamSpan);
}

FunctorCode FunctorInterface::VisitBracketSpan(BracketSpan *bracketSpan)
{
    return this->VisitControlElement(bracketSpan);
}

FunctorCode FunctorInterface::VisitBracketSpanEnd(BracketSpan *bracketSpan)
{
    return this->VisitControlElementEnd(bracketSpan);
}

FunctorCode FunctorInterface::VisitBreath(Breath *breath)
{
    return this->VisitControlElement(breath);
}

FunctorCode FunctorInterface::VisitBreathEnd(Breath *breath)
{
    return this->VisitControlElementEnd(breath);
}

FunctorCode FunctorInterface::VisitCaesura(Caesura *caesura)
{
    return this->VisitControlElement(caesura);
}

FunctorCode FunctorInterface::VisitCaesuraEnd(Caesura *caesura)
{
    return this->VisitControlElementEnd(caesura);
}

FunctorCode FunctorInterface::VisitControlElement(ControlElement *controlElement)
{
    return this->VisitFloatingObject(controlElement);
}

FunctorCode FunctorInterface::VisitControlElementEnd(ControlElement *controlElement)
{
    return this->VisitFloatingObjectEnd(controlElement);
}

FunctorCode FunctorInterface::VisitCpMark(CpMark *cpMark)
{
    return this->VisitControlElement(cpMark);
}

FunctorCode FunctorInterface::VisitCpMarkEnd(CpMark *cpMark)
{
    return this->VisitControlElementEnd(cpMark);
}

FunctorCode FunctorInterface::VisitDir(Dir *dir)
{
    return this->VisitControlElement(dir);
}

FunctorCode FunctorInterface::VisitDirEnd(Dir *dir)
{
    return this->VisitControlElementEnd(dir);
}

FunctorCode FunctorInterface::VisitDynam(Dynam *dynam)
{
    return this->VisitControlElement(dynam);
}

FunctorCode FunctorInterface::VisitDynamEnd(Dynam *dynam)
{
    return this->VisitControlElementEnd(dynam);
}

FunctorCode FunctorInterface::VisitFermata(Fermata *fermata)
{
    return this->VisitControlElement(fermata);
}

FunctorCode FunctorInterface::VisitFermataEnd(Fermata *fermata)
{
    return this->VisitControlElementEnd(fermata);
}

FunctorCode FunctorInterface::VisitFing(Fing *fing)
{
    return this->VisitControlElement(fing);
}

FunctorCode FunctorInterface::VisitFingEnd(Fing *fing)
{
    return this->VisitControlElementEnd(fing);
}

FunctorCode FunctorInterface::VisitGliss(Gliss *gliss)
{
    return this->VisitControlElement(gliss);
}

FunctorCode FunctorInterface::VisitGlissEnd(Gliss *gliss)
{
    return this->VisitControlElementEnd(gliss);
}

FunctorCode FunctorInterface::VisitHairpin(Hairpin *hairpin)
{
    return this->VisitControlElement(hairpin);
}

FunctorCode FunctorInterface::VisitHairpinEnd(Hairpin *hairpin)
{
    return this->VisitControlElementEnd(hairpin);
}

FunctorCode FunctorInterface::VisitHarm(Harm *harm)
{
    return this->VisitControlElement(harm);
}

FunctorCode FunctorInterface::VisitHarmEnd(Harm *harm)
{
    return this->VisitControlElementEnd(harm);
}

FunctorCode FunctorInterface::VisitLv(Lv *lv)
{
    return this->VisitTie(lv);
}

FunctorCode FunctorInterface::VisitLvEnd(Lv *lv)
{
    return this->VisitTieEnd(lv);
}

FunctorCode FunctorInterface::VisitMordent(Mordent *mordent)
{
    return this->VisitControlElement(mordent);
}

FunctorCode FunctorInterface::VisitMordentEnd(Mordent *mordent)
{
    return this->VisitControlElementEnd(mordent);
}

FunctorCode FunctorInterface::VisitOctave(Octave *octave)
{
    return this->VisitControlElement(octave);
}

FunctorCode FunctorInterface::VisitOctaveEnd(Octave *octave)
{
    return this->VisitControlElementEnd(octave);
}

FunctorCode FunctorInterface::VisitOrnam(Ornam *ornam)
{
    return this->VisitControlElement(ornam);
}

FunctorCode FunctorInterface::VisitOrnamEnd(Ornam *ornam)
{
    return this->VisitControlElementEnd(ornam);
}

FunctorCode FunctorInterface::VisitPedal(Pedal *pedal)
{
    return this->VisitControlElement(pedal);
}

FunctorCode FunctorInterface::VisitPedalEnd(Pedal *pedal)
{
    return this->VisitControlElementEnd(pedal);
}

FunctorCode FunctorInterface::VisitPhrase(Phrase *phrase)
{
    return this->VisitSlur(phrase);
}

FunctorCode FunctorInterface::VisitPhraseEnd(Phrase *phrase)
{
    return this->VisitSlurEnd(phrase);
}

FunctorCode FunctorInterface::VisitPitchInflection(PitchInflection *pitchInflection)
{
    return this->VisitControlElement(pitchInflection);
}

FunctorCode FunctorInterface::VisitPitchInflectionEnd(PitchInflection *pitchInflection)
{
    return this->VisitControlElementEnd(pitchInflection);
}

FunctorCode FunctorInterface::VisitReh(Reh *reh)
{
    return this->VisitControlElement(reh);
}

FunctorCode FunctorInterface::VisitRehEnd(Reh *reh)
{
    return this->VisitControlElementEnd(reh);
}

FunctorCode FunctorInterface::VisitRepeatMark(RepeatMark *repeatMark)
{
    return this->VisitControlElement(repeatMark);
}

FunctorCode FunctorInterface::VisitRepeatMarkEnd(RepeatMark *repeatMark)
{
    return this->VisitControlElementEnd(repeatMark);
}

FunctorCode FunctorInterface::VisitSlur(Slur *slur)
{
    return this->VisitControlElement(slur);
}

FunctorCode FunctorInterface::VisitSlurEnd(Slur *slur)
{
    return this->VisitControlElementEnd(slur);
}

FunctorCode FunctorInterface::VisitTempo(Tempo *tempo)
{
    return this->VisitControlElement(tempo);
}

FunctorCode FunctorInterface::VisitTempoEnd(Tempo *tempo)
{
    return this->VisitControlElementEnd(tempo);
}

FunctorCode FunctorInterface::VisitTie(Tie *tie)
{
    return this->VisitControlElement(tie);
}

FunctorCode FunctorInterface::VisitTieEnd(Tie *tie)
{
    return this->VisitControlElementEnd(tie);
}

FunctorCode FunctorInterface::VisitTrill(Trill *trill)
{
    return this->VisitControlElement(trill);
}

FunctorCode FunctorInterface::VisitTrillEnd(Trill *trill)
{
    return this->VisitControlElementEnd(trill);
}

FunctorCode FunctorInterface::VisitTurn(Turn *turn)
{
    return this->VisitControlElement(turn);
}

FunctorCode FunctorInterface::VisitTurnEnd(Turn *turn)
{
    return this->VisitControlElementEnd(turn);
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

FunctorCode FunctorInterface::VisitGenericLayerElement(GenericLayerElement *genericLayerElement)
{
    return this->VisitLayerElement(genericLayerElement);
}

FunctorCode FunctorInterface::VisitGenericLayerElementEnd(GenericLayerElement *genericLayerElement)
{
    return this->VisitLayerElementEnd(genericLayerElement);
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
    return this->VisitLayerElement(meterSigGrp);
}

FunctorCode FunctorInterface::VisitMeterSigGrpEnd(MeterSigGrp *meterSigGrp)
{
    return this->VisitLayerElementEnd(meterSigGrp);
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

FunctorCode FunctorInterface::VisitF(F *f)
{
    return this->VisitTextElement(f);
}

FunctorCode FunctorInterface::VisitFEnd(F *f)
{
    return this->VisitTextElementEnd(f);
}

FunctorCode FunctorInterface::VisitFb(Fb *fb)
{
    return this->VisitObject(fb);
}

FunctorCode FunctorInterface::VisitFbEnd(Fb *fb)
{
    return this->VisitObjectEnd(fb);
}

FunctorCode FunctorInterface::VisitFig(Fig *fig)
{
    return this->VisitTextElement(fig);
}

FunctorCode FunctorInterface::VisitFigEnd(Fig *fig)
{
    return this->VisitTextElementEnd(fig);
}

FunctorCode FunctorInterface::VisitLb(Lb *lb)
{
    return this->VisitTextElement(lb);
}

FunctorCode FunctorInterface::VisitLbEnd(Lb *lb)
{
    return this->VisitTextElementEnd(lb);
}

FunctorCode FunctorInterface::VisitMNum(MNum *mNum)
{
    return this->VisitControlElement(mNum);
}

FunctorCode FunctorInterface::VisitMNumEnd(MNum *mNum)
{
    return this->VisitControlElementEnd(mNum);
}

FunctorCode FunctorInterface::VisitNum(Num *num)
{
    return this->VisitTextElement(num);
}

FunctorCode FunctorInterface::VisitNumEnd(Num *num)
{
    return this->VisitTextElementEnd(num);
}

FunctorCode FunctorInterface::VisitRend(Rend *rend)
{
    return this->VisitTextElement(rend);
}

FunctorCode FunctorInterface::VisitRendEnd(Rend *rend)
{
    return this->VisitTextElementEnd(rend);
}

FunctorCode FunctorInterface::VisitSvg(Svg *svg)
{
    return this->VisitObject(svg);
}

FunctorCode FunctorInterface::VisitSvgEnd(Svg *svg)
{
    return this->VisitObjectEnd(svg);
}

FunctorCode FunctorInterface::VisitSymbol(Symbol *symbol)
{
    return this->VisitTextElement(symbol);
}

FunctorCode FunctorInterface::VisitSymbolEnd(Symbol *symbol)
{
    return this->VisitTextElementEnd(symbol);
}

FunctorCode FunctorInterface::VisitText(Text *text)
{
    return this->VisitTextElement(text);
}

FunctorCode FunctorInterface::VisitTextEnd(Text *text)
{
    return this->VisitTextElementEnd(text);
}

FunctorCode FunctorInterface::VisitTextElement(TextElement *textElement)
{
    return this->VisitObject(textElement);
}

FunctorCode FunctorInterface::VisitTextElementEnd(TextElement *textElement)
{
    return this->VisitObjectEnd(textElement);
}

FunctorCode FunctorInterface::VisitFacsimile(Facsimile *facsimile)
{
    return this->VisitObject(facsimile);
}

FunctorCode FunctorInterface::VisitFacsimileEnd(Facsimile *facsimile)
{
    return this->VisitObjectEnd(facsimile);
}

FunctorCode FunctorInterface::VisitGraphic(Graphic *graphic)
{
    return this->VisitObject(graphic);
}

FunctorCode FunctorInterface::VisitGraphicEnd(Graphic *graphic)
{
    return this->VisitObjectEnd(graphic);
}

FunctorCode FunctorInterface::VisitSurface(Surface *surface)
{
    return this->VisitObject(surface);
}

FunctorCode FunctorInterface::VisitSurfaceEnd(Surface *surface)
{
    return this->VisitObjectEnd(surface);
}

FunctorCode FunctorInterface::VisitZone(Zone *zone)
{
    return this->VisitObject(zone);
}

FunctorCode FunctorInterface::VisitZoneEnd(Zone *zone)
{
    return this->VisitObjectEnd(zone);
}

FunctorCode FunctorInterface::VisitAlignment(Alignment *alignment)
{
    return this->VisitObject(alignment);
}

FunctorCode FunctorInterface::VisitAlignmentEnd(Alignment *alignment)
{
    return this->VisitObjectEnd(alignment);
}

FunctorCode FunctorInterface::VisitAlignmentReference(AlignmentReference *alignmentReference)
{
    return this->VisitObject(alignmentReference);
}

FunctorCode FunctorInterface::VisitAlignmentReferenceEnd(AlignmentReference *alignmentReference)
{
    return this->VisitObjectEnd(alignmentReference);
}

FunctorCode FunctorInterface::VisitHorizontalAligner(HorizontalAligner *horizontalAligner)
{
    return this->VisitObject(horizontalAligner);
}

FunctorCode FunctorInterface::VisitHorizontalAlignerEnd(HorizontalAligner *horizontalAligner)
{
    return this->VisitObjectEnd(horizontalAligner);
}

FunctorCode FunctorInterface::VisitMeasureAligner(MeasureAligner *measureAligner)
{
    return this->VisitHorizontalAligner(measureAligner);
}

FunctorCode FunctorInterface::VisitMeasureAlignerEnd(MeasureAligner *measureAligner)
{
    return this->VisitHorizontalAlignerEnd(measureAligner);
}

FunctorCode FunctorInterface::VisitGraceAligner(GraceAligner *graceAligner)
{
    return this->VisitHorizontalAligner(graceAligner);
}

FunctorCode FunctorInterface::VisitGraceAlignerEnd(GraceAligner *graceAligner)
{
    return this->VisitHorizontalAlignerEnd(graceAligner);
}

FunctorCode FunctorInterface::VisitTimestampAligner(TimestampAligner *timestampAligner)
{
    return this->VisitObject(timestampAligner);
}

FunctorCode FunctorInterface::VisitTimestampAlignerEnd(TimestampAligner *timestampAligner)
{
    return this->VisitObjectEnd(timestampAligner);
}

FunctorCode FunctorInterface::VisitSystemAligner(SystemAligner *systemAligner)
{
    return this->VisitObject(systemAligner);
}

FunctorCode FunctorInterface::VisitSystemAlignerEnd(SystemAligner *systemAligner)
{
    return this->VisitObjectEnd(systemAligner);
}

FunctorCode FunctorInterface::VisitStaffAlignment(StaffAlignment *staffAlignment)
{
    return this->VisitObject(staffAlignment);
}

FunctorCode FunctorInterface::VisitStaffAlignmentEnd(StaffAlignment *staffAlignment)
{
    return this->VisitObjectEnd(staffAlignment);
}

FunctorCode FunctorInterface::VisitFloatingObject(FloatingObject *floatingObject)
{
    return this->VisitObject(floatingObject);
}

FunctorCode FunctorInterface::VisitFloatingObjectEnd(FloatingObject *floatingObject)
{
    return this->VisitObjectEnd(floatingObject);
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

FunctorCode ConstFunctorInterface::VisitPb(const Pb *pb)
{
    return this->VisitSystemElement(pb);
}

FunctorCode ConstFunctorInterface::VisitPbEnd(const Pb *pb)
{
    return this->VisitSystemElementEnd(pb);
}

FunctorCode ConstFunctorInterface::VisitSb(const Sb *sb)
{
    return this->VisitSystemElement(sb);
}

FunctorCode ConstFunctorInterface::VisitSbEnd(const Sb *sb)
{
    return this->VisitSystemElementEnd(sb);
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

FunctorCode ConstFunctorInterface::VisitEditorialElement(const EditorialElement *editorialElement)
{
    return this->VisitObject(editorialElement);
}

FunctorCode ConstFunctorInterface::VisitEditorialElementEnd(const EditorialElement *editorialElement)
{
    return this->VisitObjectEnd(editorialElement);
}

FunctorCode ConstFunctorInterface::VisitDiv(const Div *div)
{
    return this->VisitTextLayoutElement(div);
}

FunctorCode ConstFunctorInterface::VisitDivEnd(const Div *div)
{
    return this->VisitTextLayoutElementEnd(div);
}

FunctorCode ConstFunctorInterface::VisitRunningElement(const RunningElement *runningElement)
{
    return this->VisitTextLayoutElement(runningElement);
}

FunctorCode ConstFunctorInterface::VisitRunningElementEnd(const RunningElement *runningElement)
{
    return this->VisitTextLayoutElementEnd(runningElement);
}

FunctorCode ConstFunctorInterface::VisitPgHead(const PgHead *pgHead)
{
    return this->VisitRunningElement(pgHead);
}

FunctorCode ConstFunctorInterface::VisitPgHeadEnd(const PgHead *pgHead)
{
    return this->VisitRunningElementEnd(pgHead);
}

FunctorCode ConstFunctorInterface::VisitPgFoot(const PgFoot *pgFoot)
{
    return this->VisitRunningElement(pgFoot);
}

FunctorCode ConstFunctorInterface::VisitPgFootEnd(const PgFoot *pgFoot)
{
    return this->VisitRunningElementEnd(pgFoot);
}

FunctorCode ConstFunctorInterface::VisitTextLayoutElement(const TextLayoutElement *textLayoutElement)
{
    return this->VisitObject(textLayoutElement);
}

FunctorCode ConstFunctorInterface::VisitTextLayoutElementEnd(const TextLayoutElement *textLayoutElement)
{
    return this->VisitObjectEnd(textLayoutElement);
}

FunctorCode ConstFunctorInterface::VisitEnding(const Ending *ending)
{
    return this->VisitSystemElement(ending);
}

FunctorCode ConstFunctorInterface::VisitEndingEnd(const Ending *ending)
{
    return this->VisitSystemElementEnd(ending);
}

FunctorCode ConstFunctorInterface::VisitExpansion(const Expansion *expansion)
{
    return this->VisitSystemElement(expansion);
}

FunctorCode ConstFunctorInterface::VisitExpansionEnd(const Expansion *expansion)
{
    return this->VisitSystemElementEnd(expansion);
}

FunctorCode ConstFunctorInterface::VisitSection(const Section *section)
{
    return this->VisitSystemElement(section);
}

FunctorCode ConstFunctorInterface::VisitSectionEnd(const Section *section)
{
    return this->VisitSystemElementEnd(section);
}

FunctorCode ConstFunctorInterface::VisitSystemElement(const SystemElement *systemElement)
{
    return this->VisitFloatingObject(systemElement);
}

FunctorCode ConstFunctorInterface::VisitSystemElementEnd(const SystemElement *systemElement)
{
    return this->VisitFloatingObjectEnd(systemElement);
}

FunctorCode ConstFunctorInterface::VisitSystemMilestone(const SystemMilestoneEnd *systemMilestoneEnd)
{
    return this->VisitSystemElement(systemMilestoneEnd);
}

FunctorCode ConstFunctorInterface::VisitSystemMilestoneEnd(const SystemMilestoneEnd *systemMilestoneEnd)
{
    return this->VisitSystemElementEnd(systemMilestoneEnd);
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

FunctorCode ConstFunctorInterface::VisitAnchoredText(const AnchoredText *anchoredText)
{
    return this->VisitControlElement(anchoredText);
}

FunctorCode ConstFunctorInterface::VisitAnchoredTextEnd(const AnchoredText *anchoredText)
{
    return this->VisitControlElementEnd(anchoredText);
}

FunctorCode ConstFunctorInterface::VisitAnnotScore(const AnnotScore *annotScore)
{
    return this->VisitControlElement(annotScore);
}

FunctorCode ConstFunctorInterface::VisitAnnotScoreEnd(const AnnotScore *annotScore)
{
    return this->VisitControlElementEnd(annotScore);
}

FunctorCode ConstFunctorInterface::VisitArpeg(const Arpeg *arpeg)
{
    return this->VisitControlElement(arpeg);
}

FunctorCode ConstFunctorInterface::VisitArpegEnd(const Arpeg *arpeg)
{
    return this->VisitControlElementEnd(arpeg);
}

FunctorCode ConstFunctorInterface::VisitBeamSpan(const BeamSpan *beamSpan)
{
    return this->VisitControlElement(beamSpan);
}

FunctorCode ConstFunctorInterface::VisitBeamSpanEnd(const BeamSpan *beamSpan)
{
    return this->VisitControlElementEnd(beamSpan);
}

FunctorCode ConstFunctorInterface::VisitBracketSpan(const BracketSpan *bracketSpan)
{
    return this->VisitControlElement(bracketSpan);
}

FunctorCode ConstFunctorInterface::VisitBracketSpanEnd(const BracketSpan *bracketSpan)
{
    return this->VisitControlElementEnd(bracketSpan);
}

FunctorCode ConstFunctorInterface::VisitBreath(const Breath *breath)
{
    return this->VisitControlElement(breath);
}

FunctorCode ConstFunctorInterface::VisitBreathEnd(const Breath *breath)
{
    return this->VisitControlElementEnd(breath);
}

FunctorCode ConstFunctorInterface::VisitCaesura(const Caesura *caesura)
{
    return this->VisitControlElement(caesura);
}

FunctorCode ConstFunctorInterface::VisitCaesuraEnd(const Caesura *caesura)
{
    return this->VisitControlElementEnd(caesura);
}

FunctorCode ConstFunctorInterface::VisitControlElement(const ControlElement *controlElement)
{
    return this->VisitFloatingObject(controlElement);
}

FunctorCode ConstFunctorInterface::VisitControlElementEnd(const ControlElement *controlElement)
{
    return this->VisitFloatingObjectEnd(controlElement);
}

FunctorCode ConstFunctorInterface::VisitCpMark(const CpMark *cpMark)
{
    return this->VisitControlElement(cpMark);
}

FunctorCode ConstFunctorInterface::VisitCpMarkEnd(const CpMark *cpMark)
{
    return this->VisitControlElementEnd(cpMark);
}

FunctorCode ConstFunctorInterface::VisitDir(const Dir *dir)
{
    return this->VisitControlElement(dir);
}

FunctorCode ConstFunctorInterface::VisitDirEnd(const Dir *dir)
{
    return this->VisitControlElementEnd(dir);
}

FunctorCode ConstFunctorInterface::VisitDynam(const Dynam *dynam)
{
    return this->VisitControlElement(dynam);
}

FunctorCode ConstFunctorInterface::VisitDynamEnd(const Dynam *dynam)
{
    return this->VisitControlElementEnd(dynam);
}

FunctorCode ConstFunctorInterface::VisitFermata(const Fermata *fermata)
{
    return this->VisitControlElement(fermata);
}

FunctorCode ConstFunctorInterface::VisitFermataEnd(const Fermata *fermata)
{
    return this->VisitControlElementEnd(fermata);
}

FunctorCode ConstFunctorInterface::VisitFing(const Fing *fing)
{
    return this->VisitControlElement(fing);
}

FunctorCode ConstFunctorInterface::VisitFingEnd(const Fing *fing)
{
    return this->VisitControlElementEnd(fing);
}

FunctorCode ConstFunctorInterface::VisitGliss(const Gliss *gliss)
{
    return this->VisitControlElement(gliss);
}

FunctorCode ConstFunctorInterface::VisitGlissEnd(const Gliss *gliss)
{
    return this->VisitControlElementEnd(gliss);
}

FunctorCode ConstFunctorInterface::VisitHairpin(const Hairpin *hairpin)
{
    return this->VisitControlElement(hairpin);
}

FunctorCode ConstFunctorInterface::VisitHairpinEnd(const Hairpin *hairpin)
{
    return this->VisitControlElementEnd(hairpin);
}

FunctorCode ConstFunctorInterface::VisitHarm(const Harm *harm)
{
    return this->VisitControlElement(harm);
}

FunctorCode ConstFunctorInterface::VisitHarmEnd(const Harm *harm)
{
    return this->VisitControlElementEnd(harm);
}

FunctorCode ConstFunctorInterface::VisitLv(const Lv *lv)
{
    return this->VisitTie(lv);
}

FunctorCode ConstFunctorInterface::VisitLvEnd(const Lv *lv)
{
    return this->VisitTieEnd(lv);
}

FunctorCode ConstFunctorInterface::VisitMordent(const Mordent *mordent)
{
    return this->VisitControlElement(mordent);
}

FunctorCode ConstFunctorInterface::VisitMordentEnd(const Mordent *mordent)
{
    return this->VisitControlElementEnd(mordent);
}

FunctorCode ConstFunctorInterface::VisitOctave(const Octave *octave)
{
    return this->VisitControlElement(octave);
}

FunctorCode ConstFunctorInterface::VisitOctaveEnd(const Octave *octave)
{
    return this->VisitControlElementEnd(octave);
}

FunctorCode ConstFunctorInterface::VisitOrnam(const Ornam *ornam)
{
    return this->VisitControlElement(ornam);
}

FunctorCode ConstFunctorInterface::VisitOrnamEnd(const Ornam *ornam)
{
    return this->VisitControlElementEnd(ornam);
}

FunctorCode ConstFunctorInterface::VisitPedal(const Pedal *pedal)
{
    return this->VisitControlElement(pedal);
}

FunctorCode ConstFunctorInterface::VisitPedalEnd(const Pedal *pedal)
{
    return this->VisitControlElementEnd(pedal);
}

FunctorCode ConstFunctorInterface::VisitPhrase(const Phrase *phrase)
{
    return this->VisitSlur(phrase);
}

FunctorCode ConstFunctorInterface::VisitPhraseEnd(const Phrase *phrase)
{
    return this->VisitSlurEnd(phrase);
}

FunctorCode ConstFunctorInterface::VisitPitchInflection(const PitchInflection *pitchInflection)
{
    return this->VisitControlElement(pitchInflection);
}

FunctorCode ConstFunctorInterface::VisitPitchInflectionEnd(const PitchInflection *pitchInflection)
{
    return this->VisitControlElementEnd(pitchInflection);
}

FunctorCode ConstFunctorInterface::VisitReh(const Reh *reh)
{
    return this->VisitControlElement(reh);
}

FunctorCode ConstFunctorInterface::VisitRehEnd(const Reh *reh)
{
    return this->VisitControlElementEnd(reh);
}

FunctorCode ConstFunctorInterface::VisitRepeatMark(const RepeatMark *repeatMark)
{
    return this->VisitControlElement(repeatMark);
}

FunctorCode ConstFunctorInterface::VisitRepeatMarkEnd(const RepeatMark *repeatMark)
{
    return this->VisitControlElementEnd(repeatMark);
}

FunctorCode ConstFunctorInterface::VisitSlur(const Slur *slur)
{
    return this->VisitControlElement(slur);
}

FunctorCode ConstFunctorInterface::VisitSlurEnd(const Slur *slur)
{
    return this->VisitControlElementEnd(slur);
}

FunctorCode ConstFunctorInterface::VisitTempo(const Tempo *tempo)
{
    return this->VisitControlElement(tempo);
}

FunctorCode ConstFunctorInterface::VisitTempoEnd(const Tempo *tempo)
{
    return this->VisitControlElementEnd(tempo);
}

FunctorCode ConstFunctorInterface::VisitTie(const Tie *tie)
{
    return this->VisitControlElement(tie);
}

FunctorCode ConstFunctorInterface::VisitTieEnd(const Tie *tie)
{
    return this->VisitControlElementEnd(tie);
}

FunctorCode ConstFunctorInterface::VisitTrill(const Trill *trill)
{
    return this->VisitControlElement(trill);
}

FunctorCode ConstFunctorInterface::VisitTrillEnd(const Trill *trill)
{
    return this->VisitControlElementEnd(trill);
}

FunctorCode ConstFunctorInterface::VisitTurn(const Turn *turn)
{
    return this->VisitControlElement(turn);
}

FunctorCode ConstFunctorInterface::VisitTurnEnd(const Turn *turn)
{
    return this->VisitControlElementEnd(turn);
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

FunctorCode ConstFunctorInterface::VisitGenericLayerElement(const GenericLayerElement *genericLayerElement)
{
    return this->VisitLayerElement(genericLayerElement);
}

FunctorCode ConstFunctorInterface::VisitGenericLayerElementEnd(const GenericLayerElement *genericLayerElement)
{
    return this->VisitLayerElementEnd(genericLayerElement);
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
    return this->VisitLayerElement(meterSigGrp);
}

FunctorCode ConstFunctorInterface::VisitMeterSigGrpEnd(const MeterSigGrp *meterSigGrp)
{
    return this->VisitLayerElementEnd(meterSigGrp);
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

FunctorCode ConstFunctorInterface::VisitF(const F *f)
{
    return this->VisitTextElement(f);
}

FunctorCode ConstFunctorInterface::VisitFEnd(const F *f)
{
    return this->VisitTextElementEnd(f);
}

FunctorCode ConstFunctorInterface::VisitFb(const Fb *fb)
{
    return this->VisitObject(fb);
}

FunctorCode ConstFunctorInterface::VisitFbEnd(const Fb *fb)
{
    return this->VisitObjectEnd(fb);
}

FunctorCode ConstFunctorInterface::VisitFig(const Fig *fig)
{
    return this->VisitTextElement(fig);
}

FunctorCode ConstFunctorInterface::VisitFigEnd(const Fig *fig)
{
    return this->VisitTextElementEnd(fig);
}

FunctorCode ConstFunctorInterface::VisitLb(const Lb *lb)
{
    return this->VisitTextElement(lb);
}

FunctorCode ConstFunctorInterface::VisitLbEnd(const Lb *lb)
{
    return this->VisitTextElementEnd(lb);
}

FunctorCode ConstFunctorInterface::VisitMNum(const MNum *mNum)
{
    return this->VisitControlElement(mNum);
}

FunctorCode ConstFunctorInterface::VisitMNumEnd(const MNum *mNum)
{
    return this->VisitControlElementEnd(mNum);
}

FunctorCode ConstFunctorInterface::VisitNum(const Num *num)
{
    return this->VisitTextElement(num);
}

FunctorCode ConstFunctorInterface::VisitNumEnd(const Num *num)
{
    return this->VisitTextElementEnd(num);
}

FunctorCode ConstFunctorInterface::VisitRend(const Rend *rend)
{
    return this->VisitTextElement(rend);
}

FunctorCode ConstFunctorInterface::VisitRendEnd(const Rend *rend)
{
    return this->VisitTextElementEnd(rend);
}

FunctorCode ConstFunctorInterface::VisitSvg(const Svg *svg)
{
    return this->VisitObject(svg);
}

FunctorCode ConstFunctorInterface::VisitSvgEnd(const Svg *svg)
{
    return this->VisitObjectEnd(svg);
}

FunctorCode ConstFunctorInterface::VisitSymbol(const Symbol *symbol)
{
    return this->VisitTextElement(symbol);
}

FunctorCode ConstFunctorInterface::VisitSymbolEnd(const Symbol *symbol)
{
    return this->VisitTextElementEnd(symbol);
}

FunctorCode ConstFunctorInterface::VisitText(const Text *text)
{
    return this->VisitTextElement(text);
}

FunctorCode ConstFunctorInterface::VisitTextEnd(const Text *text)
{
    return this->VisitTextElementEnd(text);
}

FunctorCode ConstFunctorInterface::VisitTextElement(const TextElement *textElement)
{
    return this->VisitObject(textElement);
}

FunctorCode ConstFunctorInterface::VisitTextElementEnd(const TextElement *textElement)
{
    return this->VisitObjectEnd(textElement);
}

FunctorCode ConstFunctorInterface::VisitFacsimile(const Facsimile *facsimile)
{
    return this->VisitObject(facsimile);
}

FunctorCode ConstFunctorInterface::VisitFacsimileEnd(const Facsimile *facsimile)
{
    return this->VisitObjectEnd(facsimile);
}

FunctorCode ConstFunctorInterface::VisitGraphic(const Graphic *graphic)
{
    return this->VisitObject(graphic);
}

FunctorCode ConstFunctorInterface::VisitGraphicEnd(const Graphic *graphic)
{
    return this->VisitObjectEnd(graphic);
}

FunctorCode ConstFunctorInterface::VisitSurface(const Surface *surface)
{
    return this->VisitObject(surface);
}

FunctorCode ConstFunctorInterface::VisitSurfaceEnd(const Surface *surface)
{
    return this->VisitObjectEnd(surface);
}

FunctorCode ConstFunctorInterface::VisitZone(const Zone *zone)
{
    return this->VisitObject(zone);
}

FunctorCode ConstFunctorInterface::VisitZoneEnd(const Zone *zone)
{
    return this->VisitObjectEnd(zone);
}

FunctorCode ConstFunctorInterface::VisitAlignment(const Alignment *alignment)
{
    return this->VisitObject(alignment);
}

FunctorCode ConstFunctorInterface::VisitAlignmentEnd(const Alignment *alignment)
{
    return this->VisitObjectEnd(alignment);
}

FunctorCode ConstFunctorInterface::VisitAlignmentReference(const AlignmentReference *alignmentReference)
{
    return this->VisitObject(alignmentReference);
}

FunctorCode ConstFunctorInterface::VisitAlignmentReferenceEnd(const AlignmentReference *alignmentReference)
{
    return this->VisitObjectEnd(alignmentReference);
}

FunctorCode ConstFunctorInterface::VisitHorizontalAligner(const HorizontalAligner *horizontalAligner)
{
    return this->VisitObject(horizontalAligner);
}

FunctorCode ConstFunctorInterface::VisitHorizontalAlignerEnd(const HorizontalAligner *horizontalAligner)
{
    return this->VisitObjectEnd(horizontalAligner);
}

FunctorCode ConstFunctorInterface::VisitMeasureAligner(const MeasureAligner *measureAligner)
{
    return this->VisitHorizontalAligner(measureAligner);
}

FunctorCode ConstFunctorInterface::VisitMeasureAlignerEnd(const MeasureAligner *measureAligner)
{
    return this->VisitHorizontalAlignerEnd(measureAligner);
}

FunctorCode ConstFunctorInterface::VisitGraceAligner(const GraceAligner *graceAligner)
{
    return this->VisitHorizontalAligner(graceAligner);
}

FunctorCode ConstFunctorInterface::VisitGraceAlignerEnd(const GraceAligner *graceAligner)
{
    return this->VisitHorizontalAlignerEnd(graceAligner);
}

FunctorCode ConstFunctorInterface::VisitTimestampAligner(const TimestampAligner *timestampAligner)
{
    return this->VisitObject(timestampAligner);
}

FunctorCode ConstFunctorInterface::VisitTimestampAlignerEnd(const TimestampAligner *timestampAligner)
{
    return this->VisitObjectEnd(timestampAligner);
}

FunctorCode ConstFunctorInterface::VisitSystemAligner(const SystemAligner *systemAligner)
{
    return this->VisitObject(systemAligner);
}

FunctorCode ConstFunctorInterface::VisitSystemAlignerEnd(const SystemAligner *systemAligner)
{
    return this->VisitObjectEnd(systemAligner);
}

FunctorCode ConstFunctorInterface::VisitStaffAlignment(const StaffAlignment *staffAlignment)
{
    return this->VisitObject(staffAlignment);
}

FunctorCode ConstFunctorInterface::VisitStaffAlignmentEnd(const StaffAlignment *staffAlignment)
{
    return this->VisitObjectEnd(staffAlignment);
}

FunctorCode ConstFunctorInterface::VisitFloatingObject(const FloatingObject *floatingObject)
{
    return this->VisitObject(floatingObject);
}

FunctorCode ConstFunctorInterface::VisitFloatingObjectEnd(const FloatingObject *floatingObject)
{
    return this->VisitObjectEnd(floatingObject);
}

} // namespace vrv

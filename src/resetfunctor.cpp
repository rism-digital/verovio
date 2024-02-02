/////////////////////////////////////////////////////////////////////////////
// Name:        resetfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "resetfunctor.h"

//----------------------------------------------------------------------------

#include "arpeg.h"
#include "beamspan.h"
#include "custos.h"
#include "div.h"
#include "dot.h"
#include "editorial.h"
#include "elementpart.h"
#include "ending.h"
#include "f.h"
#include "ftrem.h"
#include "hairpin.h"
#include "layer.h"
#include "ligature.h"
#include "mrest.h"
#include "octave.h"
#include "page.h"
#include "repeatmark.h"
#include "rest.h"
#include "runningelement.h"
#include "section.h"
#include "slur.h"
#include "staff.h"
#include "stem.h"
#include "syl.h"
#include "system.h"
#include "tempo.h"
#include "textlayoutelement.h"
#include "tuplet.h"
#include "turn.h"
#include "verse.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// ResetDataFunctor
//----------------------------------------------------------------------------

ResetDataFunctor::ResetDataFunctor() : Functor() {}

FunctorCode ResetDataFunctor::VisitAccid(Accid *accid)
{
    // Call parent one too
    this->VisitLayerElement(accid);
    accid->PositionInterface::InterfaceResetData(*this, accid);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitArpeg(Arpeg *arpeg)
{
    // Call parent one too
    this->VisitControlElement(arpeg);
    arpeg->PlistInterface::InterfaceResetData(*this, arpeg);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitArtic(Artic *artic)
{
    // Call parent one too
    this->VisitLayerElement(artic);

    artic->SetDrawingPlace(STAFFREL_NONE);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitBeam(Beam *beam)
{
    // Call parent one too
    this->VisitLayerElement(beam);
    beam->BeamDrawingInterface::Reset();

    beam->m_beamSegment.Reset();
    beam->SetStemSameasBeam(NULL);

    // We want the list of the ObjectListInterface to be regenerated
    beam->Modify();

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitBeamSpan(BeamSpan *beamSpan)
{
    // Call parent one too
    this->VisitControlElement(beamSpan);
    beamSpan->BeamDrawingInterface::Reset();
    beamSpan->PlistInterface::InterfaceResetData(*this, beamSpan);

    beamSpan->ResetBeamedElements();
    beamSpan->ClearBeamSegments();
    beamSpan->InitBeamSegments();

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitChord(Chord *chord)
{
    // Call parent one too
    this->VisitLayerElement(chord);

    // We want the list of the ObjectListInterface to be regenerated
    chord->Modify();
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitControlElement(ControlElement *controlElement)
{
    // Call parent one too
    this->VisitFloatingObject(controlElement);

    // Pass it to the pseudo functor of the interface
    if (controlElement->HasInterface(INTERFACE_ALT_SYM)) {
        AltSymInterface *interface = controlElement->GetAltSymInterface();
        assert(interface);
        interface->InterfaceResetData(*this, controlElement);
    }
    if (controlElement->HasInterface(INTERFACE_LINKING)) {
        LinkingInterface *interface = controlElement->GetLinkingInterface();
        assert(interface);
        interface->InterfaceResetData(*this, controlElement);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitCustos(Custos *custos)
{
    // Call parent one too
    this->VisitLayerElement(custos);
    custos->PositionInterface::InterfaceResetData(*this, custos);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitDiv(Div *div)
{
    // Call parent one too
    this->VisitObject(div);

    div->SetDrawingInline(false);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitDot(Dot *dot)
{
    // Call parent one too
    this->VisitLayerElement(dot);
    dot->PositionInterface::InterfaceResetData(*this, dot);

    dot->m_drawingPreviousElement = NULL;
    dot->m_drawingNextElement = NULL;

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitDots(Dots *dots)
{
    // Call parent one too
    this->VisitLayerElement(dots);

    dots->ResetMapOfDotLocs();
    dots->IsAdjusted(false);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitEditorialElement(EditorialElement *editorialElement)
{
    // Call parent one too
    this->VisitObject(editorialElement);

    if (editorialElement->IsSystemMilestone()) {
        editorialElement->SystemMilestoneInterface::InterfaceResetData(*this);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitEnding(Ending *ending)
{
    this->VisitFloatingObject(ending);
    ending->SystemMilestoneInterface::InterfaceResetData(*this);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitF(F *f)
{
    this->VisitTextElement(f);
    f->TimeSpanningInterface::InterfaceResetData(*this, f);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitFlag(Flag *flag)
{
    this->VisitLayerElement(flag);

    flag->m_drawingNbFlags = 0;

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitFloatingObject(FloatingObject *floatingObject)
{
    // Call parent one too
    this->VisitObject(floatingObject);

    floatingObject->ResetDrawing();
    floatingObject->SetDrawingGrpId(0);

    // Pass it to the pseudo functor of the interface
    if (floatingObject->HasInterface(INTERFACE_FACSIMILE)) {
        FacsimileInterface *interface = floatingObject->GetFacsimileInterface();
        assert(interface);
        interface->InterfaceResetData(*this, floatingObject);
    }
    // else / else if because TimpeSpanningInterface::InterfaceResetData resets TimePointingInterface
    if (floatingObject->HasInterface(INTERFACE_TIME_SPANNING)) {
        TimeSpanningInterface *interface = floatingObject->GetTimeSpanningInterface();
        assert(interface);
        interface->InterfaceResetData(*this, floatingObject);
    }
    else if (floatingObject->HasInterface(INTERFACE_TIME_POINT)) {
        TimePointInterface *interface = floatingObject->GetTimePointInterface();
        assert(interface);
        interface->InterfaceResetData(*this, floatingObject);
    }
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitFTrem(FTrem *fTrem)
{
    // Call parent one too
    this->VisitLayerElement(fTrem);

    fTrem->m_beamSegment.Reset();

    // We want the list of the ObjectListInterface to be regenerated
    fTrem->Modify();
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitHairpin(Hairpin *hairpin)
{
    // Call parent one too
    this->VisitControlElement(hairpin);

    hairpin->SetLeftLink(NULL);
    hairpin->SetRightLink(NULL);
    hairpin->SetDrawingLength(0);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitLayer(Layer *layer)
{
    // Call parent one too
    this->VisitObject(layer);

    layer->SetCrossStaffFromAbove(false);
    layer->SetCrossStaffFromBelow(false);
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitLayerElement(LayerElement *layerElement)
{
    // Call parent one too
    this->VisitObject(layerElement);
    layerElement->FacsimileInterface::InterfaceResetData(*this, layerElement);

    layerElement->SetIsInBeamSpan(false);
    layerElement->SetDrawingCueSize(false);
    layerElement->m_crossStaff = NULL;
    layerElement->m_crossLayer = NULL;

    // Pass it to the pseudo functor of the interface
    LinkingInterface *interface = layerElement->GetLinkingInterface();
    assert(interface);
    interface->InterfaceResetData(*this, layerElement);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitLigature(Ligature *ligature)
{
    // Call parent one too
    this->VisitLayerElement(ligature);

    ligature->m_drawingShapes.clear();

    // We want the list of the ObjectListInterface to be regenerated
    ligature->Modify();
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitMeasure(Measure *measure)
{
    // Call parent one too
    this->VisitObject(measure);
    measure->FacsimileInterface::InterfaceResetData(*this, measure);

    measure->m_timestampAligner.Reset();
    measure->SetDrawingEnding(NULL);
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitMRest(MRest *mRest)
{
    // Call parent one too
    this->VisitLayerElement(mRest);
    mRest->PositionInterface::InterfaceResetData(*this, mRest);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitNote(Note *note)
{
    // Call parent one too
    this->VisitLayerElement(note);
    note->PositionInterface::InterfaceResetData(*this, note);

    note->SetDrawingLoc(0);
    note->SetFlippedNotehead(false);
    note->SetStemSameasNote(NULL);
    note->SetStemSameasRole(SAMEAS_NONE);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitObject(Object *object)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitRepeatMark(RepeatMark *repeatMark)
{
    // Call parent one too
    this->VisitControlElement(repeatMark);

    // For now doing nothing, but we should eventually remove generated text when the @func is not 'fine' anymore

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitRest(Rest *rest)
{
    // Call parent one too
    this->VisitLayerElement(rest);
    rest->PositionInterface::InterfaceResetData(*this, rest);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitSection(Section *section)
{
    // Call parent one too
    this->VisitFloatingObject(section);

    if (section->IsSystemMilestone()) {
        section->SystemMilestoneInterface::InterfaceResetData(*this);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitSlur(Slur *slur)
{
    // Call parent one too
    this->VisitControlElement(slur);

    slur->SetDrawingCurveDir(SlurCurveDirection::None);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitStaff(Staff *staff)
{
    // Call parent one too
    this->VisitObject(staff);
    staff->FacsimileInterface::InterfaceResetData(*this, staff);

    staff->m_timeSpanningElements.clear();
    staff->ClearLedgerLines();
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitStem(Stem *stem)
{
    // Call parent one too
    this->VisitLayerElement(stem);

    stem->SetDrawingStemDir(STEMDIRECTION_NONE);
    stem->SetDrawingStemLen(0);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitSyl(Syl *syl)
{
    // Call parent one too
    this->VisitLayerElement(syl);
    syl->TimeSpanningInterface::InterfaceResetData(*this, syl);

    syl->m_nextWordSyl = NULL;

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitSystemMilestone(SystemMilestoneEnd *systemMilestoneEnd)
{
    this->VisitFloatingObject(systemMilestoneEnd);

    systemMilestoneEnd->SetMeasure(NULL);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitTempo(Tempo *tempo)
{
    // Call parent one too
    this->VisitControlElement(tempo);

    tempo->ResetDrawingXRelative();

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitTuplet(Tuplet *tuplet)
{
    // Call parent one too
    this->VisitLayerElement(tuplet);

    // We want the list of the ObjectListInterface to be regenerated
    tuplet->Modify();

    tuplet->SetDrawingLeft(NULL);
    tuplet->SetDrawingRight(NULL);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitTurn(Turn *turn)
{
    // Call parent one too
    this->VisitControlElement(turn);

    turn->m_drawingEndElement = NULL;

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitVerse(Verse *verse)
{
    // Call parent one too
    this->VisitLayerElement(verse);

    verse->SetDrawingLabelAbbr(NULL);

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// ResetHorizontalAlignmentFunctor
//----------------------------------------------------------------------------

ResetHorizontalAlignmentFunctor::ResetHorizontalAlignmentFunctor() : Functor() {}

FunctorCode ResetHorizontalAlignmentFunctor::VisitAccid(Accid *accid)
{
    this->VisitLayerElement(accid);
    accid->PositionInterface::InterfaceResetHorizontalAlignment(*this, accid);

    accid->SetDrawingUnisonAccid(NULL);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitArpeg(Arpeg *arpeg)
{
    arpeg->SetDrawingXRel(0);

    return this->VisitControlElement(arpeg);
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitBeam(Beam *beam)
{
    this->VisitLayerElement(beam);

    beam->m_beamSegment.m_stemSameasRole = SAMEAS_NONE;
    beam->m_beamSegment.m_stemSameasReverseRole = NULL;

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitBeamSpan(BeamSpan *beamSpan)
{
    beamSpan->ClearBeamSegments();
    beamSpan->InitBeamSegments();

    return this->VisitControlElement(beamSpan);
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitCustos(Custos *custos)
{
    this->VisitLayerElement(custos);
    custos->PositionInterface::InterfaceResetHorizontalAlignment(*this, custos);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitDiv(Div *div)
{
    this->VisitTextLayoutElement(div);

    div->SetDrawingXRel(0);
    div->SetDrawingYRel(0);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitDot(Dot *dot)
{
    this->VisitLayerElement(dot);
    dot->PositionInterface::InterfaceResetHorizontalAlignment(*this, dot);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitDots(Dots *dots)
{
    this->VisitLayerElement(dots);

    dots->ResetMapOfDotLocs();

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitFloatingObject(FloatingObject *floatingObject)
{
    floatingObject->SetCurrentFloatingPositioner(NULL);
    floatingObject->ResetMaxDrawingYRel();

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitLayer(Layer *layer)
{
    if (layer->GetStaffDefClef()) {
        this->VisitClef(layer->GetStaffDefClef());
    }
    if (layer->GetStaffDefKeySig()) {
        this->VisitKeySig(layer->GetStaffDefKeySig());
    }
    if (layer->GetStaffDefMensur()) {
        this->VisitMensur(layer->GetStaffDefMensur());
    }
    if (layer->GetStaffDefMeterSig()) {
        this->VisitMeterSig(layer->GetStaffDefMeterSig());
    }
    if (layer->GetStaffDefMeterSigGrp()) {
        layer->GetStaffDefMeterSigGrp()->Process(*this);
    }

    if (layer->GetCautionStaffDefClef()) {
        this->VisitClef(layer->GetCautionStaffDefClef());
    }
    if (layer->GetCautionStaffDefKeySig()) {
        this->VisitKeySig(layer->GetCautionStaffDefKeySig());
    }
    if (layer->GetCautionStaffDefMensur()) {
        this->VisitMensur(layer->GetCautionStaffDefMensur());
    }
    if (layer->GetCautionStaffDefMeterSig()) {
        this->VisitMeterSig(layer->GetCautionStaffDefMeterSig());
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitLayerElement(LayerElement *layerElement)
{
    layerElement->SetDrawingXRel(0);
    // Exception here: the LayerElement::m_drawingYRel position is already set for horizontal alignment
    // See CalcAlignmentPitchPosFunctor - for this reason we need to reset it here and not in ResetVerticalAlignment
    layerElement->SetDrawingYRel(0);

    layerElement->ResetAlignment();
    layerElement->ResetGraceAlignment();
    layerElement->SetAlignmentLayerN(VRV_UNSET);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitMeasure(Measure *measure)
{
    measure->SetDrawingXRel(0);
    if (measure->m_measureAligner.GetLeftAlignment()) {
        measure->m_measureAligner.GetLeftAlignment()->SetXRel(0);
    }
    if (measure->m_measureAligner.GetRightAlignment()) {
        measure->m_measureAligner.GetRightAlignment()->SetXRel(0);
    }

    measure->m_timestampAligner.Process(*this);

    measure->HasAlignmentRefWithMultipleLayers(false);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitMRest(MRest *mRest)
{
    this->VisitLayerElement(mRest);
    mRest->PositionInterface::InterfaceResetHorizontalAlignment(*this, mRest);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitNote(Note *note)
{
    this->VisitLayerElement(note);
    note->PositionInterface::InterfaceResetHorizontalAlignment(*this, note);

    note->SetDrawingLoc(0);
    note->SetFlippedNotehead(false);
    // Re-mark the role as unsed if we have a shared stem
    if (note->HasStemSameasNote()) note->SetStemSameasRole(SAMEAS_UNSET);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitRest(Rest *rest)
{
    this->VisitLayerElement(rest);
    rest->PositionInterface::InterfaceResetHorizontalAlignment(*this, rest);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitScoreDef(ScoreDef *scoreDef)
{
    scoreDef->ResetDrawingLabelsWidth();

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitSystem(System *system)
{
    system->SetDrawingXRel(0);
    system->ResetDrawingAbbrLabelsWidth();

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitTuplet(Tuplet *tuplet)
{
    // Call parent one too
    this->VisitLayerElement(tuplet);

    tuplet->SetDrawingBracketPos(STAFFREL_basic_NONE);
    tuplet->SetBracketAlignedBeam(NULL);
    tuplet->SetNumAlignedBeam(NULL);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitTupletBracket(TupletBracket *tupletBracket)
{
    // Call parent one too
    this->VisitLayerElement(tupletBracket);

    tupletBracket->SetDrawingXRelLeft(0);
    tupletBracket->SetDrawingXRelRight(0);
    tupletBracket->SetAlignedNum(NULL);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitTupletNum(TupletNum *tupletNum)
{
    // Call parent one too
    this->VisitLayerElement(tupletNum);

    tupletNum->SetAlignedBracket(NULL);

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// ResetVerticalAlignmentFunctor
//----------------------------------------------------------------------------

ResetVerticalAlignmentFunctor::ResetVerticalAlignmentFunctor() : Functor() {}

FunctorCode ResetVerticalAlignmentFunctor::VisitArtic(Artic *artic)
{
    // Call parent one too
    this->VisitLayerElement(artic);

    artic->m_startSlurPositioners.clear();
    artic->m_endSlurPositioners.clear();

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetVerticalAlignmentFunctor::VisitFloatingObject(FloatingObject *floatingObject)
{
    floatingObject->SetCurrentFloatingPositioner(NULL);
    floatingObject->ResetMaxDrawingYRel();

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetVerticalAlignmentFunctor::VisitLayerElement(LayerElement *layerElement)
{
    // Nothing to do since m_drawingYRel is reset in ResetHorizontalAlignment and set in CalcAlignmentPitchPos

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetVerticalAlignmentFunctor::VisitOctave(Octave *octave)
{
    this->VisitFloatingObject(octave);

    octave->ResetDrawingExtenderX();

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetVerticalAlignmentFunctor::VisitPage(Page *page)
{
    RunningElement *header = page->GetHeader();
    if (header) {
        header->Process(*this);
        header->SetDrawingPage(NULL);
        header->SetDrawingYRel(0);
    }
    RunningElement *footer = page->GetFooter();
    if (footer) {
        footer->Process(*this);
        footer->SetDrawingPage(NULL);
        footer->SetDrawingYRel(0);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetVerticalAlignmentFunctor::VisitStaff(Staff *staff)
{
    staff->SetAlignment(NULL);
    staff->ClearLedgerLines();

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetVerticalAlignmentFunctor::VisitSystem(System *system)
{
    system->SetDrawingYRel(0);

    system->m_systemAligner.Reset();

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetVerticalAlignmentFunctor::VisitTextElement(TextElement *textElement)
{
    textElement->SetDrawingXRel(0);
    textElement->SetDrawingYRel(0);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetVerticalAlignmentFunctor::VisitTuplet(Tuplet *tuplet)
{
    // Call parent one too
    this->VisitLayerElement(tuplet);

    tuplet->ResetInnerSlurs();

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetVerticalAlignmentFunctor::VisitTupletBracket(TupletBracket *tupletBracket)
{
    // Call parent one too
    this->VisitLayerElement(tupletBracket);

    tupletBracket->SetDrawingYRelLeft(0);
    tupletBracket->SetDrawingYRelRight(0);

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

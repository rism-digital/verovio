/////////////////////////////////////////////////////////////////////////////
// Name:        resetfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "resetfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// ResetDataFunctor
//----------------------------------------------------------------------------

ResetDataFunctor::ResetDataFunctor() {}

FunctorCode ResetDataFunctor::VisitAccid(Accid *accid)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitArpeg(Arpeg *arpeg)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitArtic(Artic *artic)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitBeam(Beam *beam)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitChord(Chord *chord)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitControlElement(ControlElement *controlElement)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitCustos(Custos *custos)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitDot(Dot *dot)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitDots(Dots *dots)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitEditorialElement(EditorialElement *editorialElement)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitEnding(Ending *ending)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitF(F *f)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitFlag(Flag *flag)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitFloatingObject(FloatingObject *floatingObject)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitFTrem(FTrem *fTrem)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitHairpin(Hairpin *hairpin)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitLayer(Layer *layer)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitLayerElement(LayerElement *layerElement)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitLigature(Ligature *ligature)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitMeasure(Measure *measure)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitMRest(MRest *mRest)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitNote(Note *note)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitRest(Rest *rest)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitSection(Section *section)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitSlur(Slur *slur)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitStaff(Staff *staff)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitStem(Stem *stem)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitSyl(Syl *syl)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitSystemMilestone(SystemMilestoneEnd *systemMilestoneEnd)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitTempo(Tempo *tempo)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitTuplet(Tuplet *tuplet)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitTurn(Turn *turn)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitVerse(Verse *verse)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv

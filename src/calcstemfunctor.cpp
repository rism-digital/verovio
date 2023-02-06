/////////////////////////////////////////////////////////////////////////////
// Name:        calcstemfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "calcstemfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// CalcStemFunctor
//----------------------------------------------------------------------------

CalcStemFunctor::CalcStemFunctor(Doc *doc) : DocFunctor(doc)
{
    m_chordStemLength = 0;
    m_verticalCenter = 0;
    m_dur = DUR_1;
    m_isGraceNote = false;
    m_isStemSameasSecondary = false;
    m_tabGrpWithNoNote = false;
    m_staff = NULL;
    m_layer = NULL;
    m_interface = NULL;
}

FunctorCode CalcStemFunctor::VisitBeam(Beam *beam)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CalcStemFunctor::VisitBeamSpan(BeamSpan *beamSpan)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CalcStemFunctor::VisitChord(Chord *chord)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CalcStemFunctor::VisitFTrem(FTrem *fTrem)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CalcStemFunctor::VisitNote(Note *note)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CalcStemFunctor::VisitStaff(Staff *staff)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CalcStemFunctor::VisitStem(Stem *stem)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CalcStemFunctor::VisitTabDurSym(TabDurSym *tabDurSym)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode CalcStemFunctor::VisitTabGrp(TabGrp *tabGrp)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv

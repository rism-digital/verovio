/////////////////////////////////////////////////////////////////////////////
// Name:        mensur.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "mensur.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <math.h>

//----------------------------------------------------------------------------

#include "functor.h"
#include "scoredefinterface.h"
#include "vrv.h"

namespace vrv {

const int Mensur::s_num = 3;
const int Mensur::s_numBase = 2;

//----------------------------------------------------------------------------
// Mensur
//----------------------------------------------------------------------------

static const ClassRegistrar<Mensur> s_factory("mensur", MENSUR);

Mensur::Mensur()
    : LayerElement(MENSUR)
    , AttColor()
    , AttCue()
    , AttDurationRatio()
    , AttMensuralShared()
    , AttMensurVis()
    , AttSlashCount()
    , AttStaffLoc()
{
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_CUE);
    this->RegisterAttClass(ATT_DURATIONRATIO);
    this->RegisterAttClass(ATT_MENSURALSHARED);
    this->RegisterAttClass(ATT_MENSURVIS);
    this->RegisterAttClass(ATT_SLASHCOUNT);
    this->RegisterAttClass(ATT_STAFFLOC);

    this->Reset();
}

Mensur::~Mensur() {}

void Mensur::Reset()
{
    LayerElement::Reset();
    this->ResetColor();
    this->ResetCue();
    this->ResetDurationRatio();
    this->ResetMensuralShared();
    this->ResetMensurVis();
    this->ResetSlashCount();
    this->ResetStaffLoc();
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

FunctorCode Mensur::Accept(Functor &functor)
{
    return functor.VisitMensur(this);
}

FunctorCode Mensur::Accept(ConstFunctor &functor) const
{
    return functor.VisitMensur(this);
}

FunctorCode Mensur::AcceptEnd(Functor &functor)
{
    return functor.VisitMensurEnd(this);
}

FunctorCode Mensur::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitMensurEnd(this);
}

} // namespace vrv

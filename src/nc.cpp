/////////////////////////////////////////////////////////////////////////////
// Name:        nc.cpp
// Author:      Andrew Tran
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "nc.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "doc.h"
#include "elementpart.h"
#include "functor.h"
#include "liquescent.h"
#include "oriscus.h"
#include "quilisma.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Nc
//----------------------------------------------------------------------------

static const ClassRegistrar<Nc> s_factory("nc", NC);

Nc::Nc()
    : LayerElement(NC)
    , DurationInterface()
    , PitchInterface()
    , PositionInterface()
    , AttColor()
    , AttCurvatureDirection()
    , AttIntervalMelodic()
    , AttNcForm()

{
    this->RegisterInterface(DurationInterface::GetAttClasses(), DurationInterface::IsInterface());
    this->RegisterInterface(PitchInterface::GetAttClasses(), PitchInterface::IsInterface());
    this->RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_CURVATUREDIRECTION);
    this->RegisterAttClass(ATT_INTERVALMELODIC);
    this->RegisterAttClass(ATT_NCFORM);

    this->Reset();
}

Nc::~Nc() {}

void Nc::Reset()
{
    LayerElement::Reset();
    DurationInterface::Reset();
    PitchInterface::Reset();
    PositionInterface::Reset();
    this->ResetColor();
    this->ResetCurvatureDirection();
    this->ResetIntervalMelodic();
    this->ResetNcForm();
}

int Nc::PitchOrLocDifferenceTo(const Nc *nc) const
{
    int difference = this->PitchDifferenceTo(nc);
    if ((difference == 0) && this->HasLoc() && nc->HasLoc()) {
        difference = this->GetLoc() - nc->GetLoc();
    }
    return difference;
}

FunctorCode Nc::Accept(Functor &functor)
{
    return functor.VisitNc(this);
}

FunctorCode Nc::Accept(ConstFunctor &functor) const
{
    return functor.VisitNc(this);
}

FunctorCode Nc::AcceptEnd(Functor &functor)
{
    return functor.VisitNcEnd(this);
}

FunctorCode Nc::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitNcEnd(this);
}

bool Nc::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ LIQUESCENT, ORISCUS, QUILISMA };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else {
        return false;
    }
}

} // namespace vrv

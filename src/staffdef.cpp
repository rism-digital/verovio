/////////////////////////////////////////////////////////////////////////////
// Name:        staffdef.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "staffdef.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "functor.h"
#include "instrdef.h"
#include "label.h"
#include "labelabbr.h"
#include "layerdef.h"
#include "metersiggrp.h"
#include "staffgrp.h"
#include "tuning.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// StaffDef
//----------------------------------------------------------------------------

static const ClassRegistrar<StaffDef> s_factory("staffDef", STAFFDEF);

StaffDef::StaffDef()
    : ScoreDefElement(STAFFDEF)
    , AttDistances()
    , AttLabelled()
    , AttNInteger()
    , AttNotationType()
    , AttScalable()
    , AttStaffDefLog()
    , AttStaffDefVis()
    , AttStaffDefVisTablature()
    , AttTimeBase()
    , AttTransposition()
{
    this->RegisterAttClass(ATT_DISTANCES);
    this->RegisterAttClass(ATT_LABELLED);
    this->RegisterAttClass(ATT_NINTEGER);
    this->RegisterAttClass(ATT_NOTATIONTYPE);
    this->RegisterAttClass(ATT_SCALABLE);
    this->RegisterAttClass(ATT_STAFFDEFLOG);
    this->RegisterAttClass(ATT_STAFFDEFVIS);
    this->RegisterAttClass(ATT_STAFFDEFVISTABLATURE);
    this->RegisterAttClass(ATT_TIMEBASE);
    this->RegisterAttClass(ATT_TRANSPOSITION);

    this->Reset();
}

StaffDef::~StaffDef() {}

void StaffDef::Reset()
{
    ScoreDefElement::Reset();
    StaffDefDrawingInterface::Reset();
    this->ResetDistances();
    this->ResetLabelled();
    this->ResetNInteger();
    this->ResetNotationType();
    this->ResetScalable();
    this->ResetStaffDefLog();
    this->ResetStaffDefVis();
    this->ResetStaffDefVisTablature();
    this->ResetTimeBase();
    this->ResetTransposition();

    m_drawingVisibility = OPTIMIZATION_NONE;
}

bool StaffDef::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ CLEF, INSTRDEF, KEYSIG, LABEL, LABELABBR, LAYERDEF, MENSUR, METERSIG,
        METERSIGGRP, TUNING };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else {
        return false;
    }
}

int StaffDef::GetInsertOrderFor(ClassId classId) const
{
    // Anything else goes at the end
    static const std::vector s_order({ LABEL, LABELABBR });
    return this->GetInsertOrderForIn(classId, s_order);
}

bool StaffDef::HasLayerDefWithLabel() const
{
    // First get all the staffGrps
    ListOfConstObjects layerDefs = this->FindAllDescendantsByType(LAYERDEF);

    // Then the @n of each first staffDef
    for (const Object *object : layerDefs) {
        if (object->FindDescendantByType(LABEL)) return true;
    }
    return false;
}

//----------------------------------------------------------------------------
// StaffDef functor methods
//----------------------------------------------------------------------------

FunctorCode StaffDef::Accept(Functor &functor)
{
    return functor.VisitStaffDef(this);
}

FunctorCode StaffDef::Accept(ConstFunctor &functor) const
{
    return functor.VisitStaffDef(this);
}

FunctorCode StaffDef::AcceptEnd(Functor &functor)
{
    return functor.VisitStaffDefEnd(this);
}

FunctorCode StaffDef::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitStaffDefEnd(this);
}

} // namespace vrv

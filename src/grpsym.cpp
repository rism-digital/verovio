/////////////////////////////////////////////////////////////////////////////
// Name:        grpsym.cpp
// Author:      Andriy Makarchuk
// Created:     2020
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "grpsym.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "functor.h"
#include "staffgrp.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// GrpSym
//----------------------------------------------------------------------------

static const ClassRegistrar<GrpSym> s_factory("grpSym", GRPSYM);

GrpSym::GrpSym() : Object(GRPSYM), AttColor(), AttGrpSymLog(), AttStaffGroupingSym(), AttStartId(), AttStartEndId()
{
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_GRPSYMLOG);
    this->RegisterAttClass(ATT_STAFFGROUPINGSYM);
    this->RegisterAttClass(ATT_STARTID);
    this->RegisterAttClass(ATT_STARTENDID);

    this->Reset();
}

GrpSym::~GrpSym() {}

void GrpSym::Reset()
{
    Object::Reset();
    this->ResetColor();
    this->ResetGrpSymLog();
    this->ResetStaffGroupingSym();
    this->ResetStartId();
    this->ResetStartEndId();

    m_startDef = NULL;
    m_endDef = NULL;
}

void GrpSym::SetStartDef(StaffDef *start)
{
    if (start) m_startDef = start;
}

void GrpSym::SetEndDef(StaffDef *end)
{
    if (end) m_endDef = end;
}

int GrpSym::GetDrawingX() const
{
    m_cachedDrawingX = 0;
    return m_cachedDrawingX;
}

int GrpSym::GetDrawingY() const
{
    m_cachedDrawingY = 0;
    return m_cachedDrawingX;
}

//----------------------------------------------------------------------------
// GrpSym functor methods
//----------------------------------------------------------------------------

FunctorCode GrpSym::Accept(Functor &functor)
{
    return functor.VisitGrpSym(this);
}

FunctorCode GrpSym::Accept(ConstFunctor &functor) const
{
    return functor.VisitGrpSym(this);
}

FunctorCode GrpSym::AcceptEnd(Functor &functor)
{
    return functor.VisitGrpSymEnd(this);
}

FunctorCode GrpSym::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitGrpSymEnd(this);
}

} // namespace vrv

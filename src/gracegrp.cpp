/////////////////////////////////////////////////////////////////////////////
// Name:        gracegrp.cpp
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "gracegrp.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "beam.h"
#include "chord.h"
#include "editorial.h"
#include "functor.h"
#include "note.h"
#include "rest.h"
#include "space.h"
#include "vrv.h"

//----------------------------------------------------------------------------

#include "MidiFile.h"

namespace vrv {

//----------------------------------------------------------------------------
// GraceGrp
//----------------------------------------------------------------------------

static const ClassRegistrar<GraceGrp> s_factory("graceGrp", GRACEGRP);

GraceGrp::GraceGrp() : LayerElement(GRACEGRP, "gracegrp-"), AttColor(), AttGraced(), AttGraceGrpLog()
{
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_GRACED);
    this->RegisterAttClass(ATT_GRACEGRPLOG);

    this->Reset();
}

GraceGrp::~GraceGrp() {}

void GraceGrp::Reset()
{
    LayerElement::Reset();
    this->ResetColor();
    this->ResetGraced();
    this->ResetGraceGrpLog();
}

bool GraceGrp::IsSupportedChild(Object *child)
{
    if (child->Is(BEAM)) {
        assert(dynamic_cast<Beam *>(child));
    }
    else if (child->Is(CHORD)) {
        assert(dynamic_cast<Chord *>(child));
    }
    else if (child->Is(NOTE)) {
        assert(dynamic_cast<Note *>(child));
    }
    else if (child->Is(REST)) {
        assert(dynamic_cast<Rest *>(child));
    }
    else if (child->Is(SPACE)) {
        assert(dynamic_cast<Space *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

FunctorCode GraceGrp::Accept(Functor &functor)
{
    return functor.VisitGraceGrp(this);
}

FunctorCode GraceGrp::Accept(ConstFunctor &functor) const
{
    return functor.VisitGraceGrp(this);
}

FunctorCode GraceGrp::AcceptEnd(Functor &functor)
{
    return functor.VisitGraceGrpEnd(this);
}

FunctorCode GraceGrp::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitGraceGrpEnd(this);
}

} // namespace vrv

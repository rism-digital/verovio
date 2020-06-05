/////////////////////////////////////////////////////////////////////////////
// Name:        gracegrp.cpp
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "gracegrp.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "beam.h"
#include "chord.h"
#include "editorial.h"
#include "note.h"
#include "rest.h"
#include "space.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// GraceGrp
//----------------------------------------------------------------------------

GraceGrp::GraceGrp() : LayerElement("gracegrp-"), AttColor(), AttGraced(), AttGraceGrpLog()
{
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_GRACED);
    RegisterAttClass(ATT_GRACEGRPLOG);

    Reset();
}

GraceGrp::~GraceGrp() {}

void GraceGrp::Reset()
{
    LayerElement::Reset();
    ResetColor();
    ResetGraced();
    ResetGraceGrpLog();
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

} // namespace vrv

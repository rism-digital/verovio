/////////////////////////////////////////////////////////////////////////////
// Name:        nc.cpp
// Author:      Andrew Tran
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "nc.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "doc.h"
#include "elementpart.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Nc
//----------------------------------------------------------------------------

Nc::Nc()
    : LayerElement("nc-"), DurationInterface(), FacsimileInterface(), PitchInterface(), PositionInterface(), AttColor(), AttIntervalMelodic(), AttNcForm()

{
    RegisterInterface(DurationInterface::GetAttClasses(), DurationInterface::IsInterface());
    RegisterInterface(FacsimileInterface::GetAttClasses(), FacsimileInterface::IsInterface());
    RegisterInterface(PitchInterface::GetAttClasses(), PitchInterface::IsInterface());
    RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_INTERVALMELODIC);
    RegisterAttClass(ATT_NCFORM);

    Reset();
}

Nc::~Nc() {}

void Nc::Reset()
{
    LayerElement::Reset();
    DurationInterface::Reset();
    FacsimileInterface::Reset();
    PitchInterface::Reset();
    PositionInterface::Reset();
    ResetColor();
    ResetIntervalMelodic();
    ResetNcForm();
}

int Nc::GetDrawingX() const
{
    Doc *doc = dynamic_cast<Doc *>(this->GetFirstParent(DOC));
    assert(doc);
    if (this->HasFacs() && doc->GetType() == Facs) {
        return FacsimileInterface::GetDrawingX();
    }
    else {
        return LayerElement::GetDrawingX();
    }
}

int Nc::GetDrawingY() const
{
    Doc *doc = dynamic_cast<Doc *>(this->GetFirstParent(DOC));
    assert(doc);
    if (this->HasFacs() && doc->GetType() == Facs) {
        return FacsimileInterface::GetDrawingY();
    }
    else {
        return LayerElement::GetDrawingY();
    }
}

} // namespace vrv

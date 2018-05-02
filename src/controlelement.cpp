/////////////////////////////////////////////////////////////////////////////
// Name:        controlelement.cpp
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "controlelement.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "functorparams.h"
#include "rend.h"
#include "system.h"
#include "text.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// ControlElement
//----------------------------------------------------------------------------

ControlElement::ControlElement() : FloatingObject("me"), AttLabelled(), AttTyped()
{
    RegisterAttClass(ATT_LABELLED);
    RegisterAttClass(ATT_TYPED);

    Reset();
}

ControlElement::ControlElement(std::string classid) : FloatingObject(classid), AttLabelled(), AttTyped()
{
    RegisterAttClass(ATT_LABELLED);
    RegisterAttClass(ATT_TYPED);

    Reset();
}

ControlElement::~ControlElement() {}

void ControlElement::Reset()
{
    FloatingObject::Reset();
    ResetLabelled();
    ResetTyped();
}

data_HORIZONTALALIGNMENT ControlElement::GetChildRendAlignment()
{
    Rend *rend = dynamic_cast<Rend *>(this->FindChildByType(REND));
    if (!rend || !rend->HasHalign()) return HORIZONTALALIGNMENT_NONE;

    return rend->GetHalign();
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

int ControlElement::AdjustXOverflow(FunctorParams *functorParams)
{
    AdjustXOverflowParams *params = dynamic_cast<AdjustXOverflowParams *>(functorParams);
    assert(params);

    if (!this->Is({ DIR, DYNAM, TEMPO })) {
        return FUNCTOR_SIBLINGS;
    }

    assert(params->m_currentSystem);

    // Get all the positioners for this object - all of them (all staves) because we can have different staff sizes
    ArrayOfFloatingPositioners positioners;
    params->m_currentSystem->m_systemAligner.FindAllPositionerPointingTo(&positioners, this);

    // Something is probably not right if nothing found - maybe no @staff
    if (positioners.empty()) {
        LogDebug("Something was wrong when searching positioners for %s '%s'", this->GetClassName().c_str(),
            this->GetUuid().c_str());
        return FUNCTOR_SIBLINGS;
    }

    // Keep the one with the highest right position
    for (auto const &positoner : positioners) {
        if (!params->m_currentWidest || (params->m_currentWidest->GetContentRight() < positoner->GetContentRight())) {
            params->m_currentWidest = positoner;
        }
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

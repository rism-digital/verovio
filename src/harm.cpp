/////////////////////////////////////////////////////////////////////////////
// Name:        harm.cpp
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "harm.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "verticalaligner.h"
#include "editorial.h"
#include "functorparams.h"
#include "text.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Harm
//----------------------------------------------------------------------------

Harm::Harm() : ControlElement("harm-"), TextListInterface(), TextDirInterface(), TimeSpanningInterface(), AttLang()
{
    RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_LANG);

    Reset();
}

Harm::~Harm()
{
}

void Harm::Reset()
{
    ControlElement::Reset();
    TextDirInterface::Reset();
    TimeSpanningInterface::Reset();
    ResetLang();
}

void Harm::AddChild(Object *child)
{
    if (child->IsTextElement()) {
        assert(dynamic_cast<TextElement *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

//----------------------------------------------------------------------------
// Harm functor methods
//----------------------------------------------------------------------------

int Harm::PrepareFloatingGrps(FunctorParams *functorParams)
{
    // PrepareFloatingGrpsParams *params = dynamic_cast<PrepareFloatingGrpsParams *>(functorParams);
    // assert(params);

    this->SetDrawingGrpId(DRAWING_GRP_HARM);

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

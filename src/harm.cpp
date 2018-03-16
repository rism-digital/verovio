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

#include "editorial.h"
#include "fb.h"
#include "functorparams.h"
#include "text.h"
#include "verticalaligner.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Harm
//----------------------------------------------------------------------------

Harm::Harm()
    : ControlElement("harm-")
    , TextListInterface()
    , TextDirInterface()
    , TimeSpanningInterface()
    , AttLang()
    , AttNNumberLike()
{
    RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_LANG);
    RegisterAttClass(ATT_NNUMBERLIKE);

    Reset();
}

Harm::~Harm() {}

void Harm::Reset()
{
    ControlElement::Reset();
    TextDirInterface::Reset();
    TimeSpanningInterface::Reset();
    ResetLang();
    ResetNNumberLike();
}

void Harm::AddChild(Object *child)
{
    if (child->Is({ REND, TEXT })) {
        assert(dynamic_cast<TextElement *>(child));
    }
    else if (child->Is(FB)) {
        assert(dynamic_cast<Fb *>(child));
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
    PrepareFloatingGrpsParams *params = dynamic_cast<PrepareFloatingGrpsParams *>(functorParams);
    assert(params);

    std::string n = this->GetN();

    for (auto &kv : params->m_harms) {
        if (kv.first == n) {
            this->SetDrawingGrpId(kv.second->GetDrawingGrpId());
            return FUNCTOR_CONTINUE;
        }
    }

    // first harm@n, create a new group
    this->SetDrawingGrpObject(this);
    params->m_harms.insert(std::make_pair(n, this));

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

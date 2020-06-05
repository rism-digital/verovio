/////////////////////////////////////////////////////////////////////////////
// Name:        fb.cpp
// Author:      Rodolfo Zitellini
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "fb.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "f.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Fb
//----------------------------------------------------------------------------

Fb::Fb() : Object("fb-")
{

    Reset();
}

Fb::~Fb() {}

void Fb::Reset()
{
    Object::Reset();
}

bool Fb::IsSupportedChild(Object *child)
{
    if (child->Is(FIGURE)) {
        assert(dynamic_cast<F *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        return false;
    }
    return true;
}

} // namespace vrv

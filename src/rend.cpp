/////////////////////////////////////////////////////////////////////////////
// Name:        rend.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "rend.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "functorparams.h"
#include "lb.h"
#include "num.h"
#include "text.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Rend
//----------------------------------------------------------------------------

Rend::Rend()
    : TextElement("rend-")
    , AreaPosInterface()
    , AttColor()
    , AttLang()
    , AttTextRendition()
    , AttTypography()
    , AttWhitespace()
{
    RegisterInterface(AreaPosInterface::GetAttClasses(), AreaPosInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_LANG);
    RegisterAttClass(ATT_TEXTRENDITION);
    RegisterAttClass(ATT_TYPOGRAPHY);
    RegisterAttClass(ATT_WHITESPACE);

    Reset();
}

Rend::~Rend() {}

void Rend::Reset()
{
    TextElement::Reset();
    AreaPosInterface::Reset();
    ResetColor();
    ResetLang();
    ResetTextRendition();
    ResetTypography();
    ResetWhitespace();
}

bool Rend::IsSupportedChild(Object *child)
{
    if (child->Is(LB)) {
        assert(dynamic_cast<Lb *>(child));
    }
    else if (child->Is(NUM)) {
        assert(dynamic_cast<Num *>(child));
    }
    else if (child->Is(REND)) {
        assert(dynamic_cast<Rend *>(child));
    }
    else if (child->Is(TEXT)) {
        assert(dynamic_cast<Text *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

int Rend::AlignVertically(FunctorParams *functorParams)
{
    AlignVerticallyParams *params = vrv_params_cast<AlignVerticallyParams *>(functorParams);
    assert(params);

    if (this->GetHalign()) {
        switch (this->GetHalign()) {
            case (HORIZONTALALIGNMENT_right): this->SetDrawingXRel(params->m_pageWidth); break;
            case (HORIZONTALALIGNMENT_center): this->SetDrawingXRel(params->m_pageWidth / 2); break;
            default: break;
        }
    }

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv

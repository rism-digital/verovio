/////////////////////////////////////////////////////////////////////////////
// Name:        mnum.cpp
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "mnum.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "text.h"
#include "verticalaligner.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// MNum
//----------------------------------------------------------------------------

MNum::MNum()
    : ControlElement("mnum-")
    , TextListInterface()
    , TextDirInterface()
    , TimePointInterface()
    , AttColor()
    , AttLang()
    , AttTypography()
{
    RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());
    RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_LANG);
    RegisterAttClass(ATT_TYPOGRAPHY);

    Reset();
}

MNum::~MNum() {}

void MNum::Reset()
{
    ControlElement::Reset();
    TextDirInterface::Reset();
    TimePointInterface::Reset();
    ResetColor();
    ResetLang();
    ResetTypography();

    m_isGenerated = false;
}

bool MNum::IsSupportedChild(Object *child)
{
    if (child->Is({ REND, TEXT })) {
        assert(dynamic_cast<TextElement *>(child));
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
// MNum functor methods
//----------------------------------------------------------------------------

int MNum::Save(FunctorParams *functorParams)
{
    if (this->IsGenerated())
        return FUNCTOR_SIBLINGS;
    else
        return Object::Save(functorParams);
}

int MNum::SaveEnd(FunctorParams *functorParams)
{
    if (this->IsGenerated())
        return FUNCTOR_SIBLINGS;
    else
        return Object::SaveEnd(functorParams);
}

} // namespace vrv

/////////////////////////////////////////////////////////////////////////////
// Name:        num.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "num.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "text.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Num
//----------------------------------------------------------------------------

Num::Num() : TextElement("num-")
{
    Reset();
}

Num::~Num() {}

void Num::Reset()
{
    m_currentText.SetParent(this);
    m_currentText.SetText(L"");
}

bool Num::IsSupportedChild(Object *child)
{
    if (child->Is(TEXT)) {
        assert(dynamic_cast<Text *>(child));
    }
    else {
        return false;
    }
    return true;
}

} // namespace vrv

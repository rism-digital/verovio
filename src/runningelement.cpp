/////////////////////////////////////////////////////////////////////////////
// Name:        runningelement.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "runningelement.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "page.h"
#include "text.h"
#include "verticalaligner.h"

namespace vrv {

//----------------------------------------------------------------------------
// RunningElement
//----------------------------------------------------------------------------

RunningElement::RunningElement() : Object("re"), AttTyped()
{
    RegisterAttClass(ATT_TYPED);

    Reset();
}

RunningElement::RunningElement(std::string classid) : Object(classid), AttTyped()
{
    RegisterAttClass(ATT_TYPED);

    Reset();
}

RunningElement::~RunningElement()
{
}

void RunningElement::Reset()
{
    Object::Reset();
    ResetTyped();
    
    m_drawingPage = NULL;
}
    
int RunningElement::GetDrawingX() const
{
    if (!m_drawingPage) return 0;
    //return m_drawingPage->GetDrawingX();
    return 0;
}

int RunningElement::GetDrawingY() const
{
    if (!m_staffAlignment) return 0;
    return m_staffAlignment->GetYRel();
}


//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

} // namespace vrv

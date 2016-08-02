/////////////////////////////////////////////////////////////////////////////
// Name:        boundary.cpp
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "boundary.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// BoundaryEnd
//----------------------------------------------------------------------------

BoundaryEnd::BoundaryEnd(Object *start)
{
    Reset();
    m_start = start;
    m_startClassName = start->GetClassName();
}

BoundaryEnd::~BoundaryEnd()
{
}

void BoundaryEnd::Reset()
{
    m_start = NULL;
}

std::string BoundaryEnd::GetClassName() const
{
    return m_startClassName + "BoundaryEnd";
}

//----------------------------------------------------------------------------
// BoundaryStartInterface
//----------------------------------------------------------------------------

BoundaryStartInterface::BoundaryStartInterface()
{
    Reset();
}

BoundaryStartInterface::~BoundaryStartInterface()
{
}

void BoundaryStartInterface::Reset()
{
    m_end = NULL;
}

void BoundaryStartInterface::SetEnd(BoundaryEnd *end)
{
    assert(!m_end);
    m_end = end;
}

//----------------------------------------------------------------------------
// Interface pseudo functor (redirected)
//----------------------------------------------------------------------------

} // namespace vrv

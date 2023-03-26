/////////////////////////////////////////////////////////////////////////////
// Name:        adjustslursfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustslursfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustSlursFunctor
//----------------------------------------------------------------------------

AdjustSlursFunctor::AdjustSlursFunctor(Doc *doc) : DocFunctor(doc)
{
    m_crossStaffSlurs = false;
}

FunctorCode AdjustSlursFunctor::VisitStaffAlignment(StaffAlignment *staffAlignment)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustSlursFunctor::VisitSystem(System *system)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv

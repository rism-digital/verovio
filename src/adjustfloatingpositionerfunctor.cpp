/////////////////////////////////////////////////////////////////////////////
// Name:        adjustfloatingpositionerfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustfloatingpositionerfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustFloatingPositionersFunctor
//----------------------------------------------------------------------------

AdjustFloatingPositionersFunctor::AdjustFloatingPositionersFunctor(Doc *doc) : DocFunctor(doc)
{
    m_classId = OBJECT;
    m_inBetween = false;
}

FunctorCode AdjustFloatingPositionersFunctor::VisitStaffAlignment(StaffAlignment *staffAlignment)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustFloatingPositionersFunctor::VisitSystem(System *system)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv

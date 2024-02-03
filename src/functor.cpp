/////////////////////////////////////////////////////////////////////////////
// Name:        functor.cpp
// Author:      David Bauer
// Created:     2024
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "functor.h"

//----------------------------------------------------------------------------

#include "doc.h"

namespace vrv {

//----------------------------------------------------------------------------
// DocFunctor
//----------------------------------------------------------------------------

int DocFunctor::GetMaxNumberOfThreads() const
{
    return m_doc->GetOptions()->m_maxThreads.GetValue();
}

//----------------------------------------------------------------------------
// DocConstFunctor
//----------------------------------------------------------------------------

int DocConstFunctor::GetMaxNumberOfThreads() const
{
    return m_doc->GetOptions()->m_maxThreads.GetValue();
}

} // namespace vrv

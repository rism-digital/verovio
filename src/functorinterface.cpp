/////////////////////////////////////////////////////////////////////////////
// Name:        functorinterface.cpp
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "functorinterface.h"

//----------------------------------------------------------------------------

#include "doc.h"

namespace vrv {

//----------------------------------------------------------------------------
// FunctorInterface
//----------------------------------------------------------------------------

FunctorCode FunctorInterface::VisitDoc(Doc *doc)
{
    return this->VisitObject(doc);
}

FunctorCode FunctorInterface::VisitDocEnd(Doc *doc)
{
    return this->VisitObjectEnd(doc);
}

//----------------------------------------------------------------------------
// ConstFunctorInterface
//----------------------------------------------------------------------------

FunctorCode ConstFunctorInterface::VisitDoc(const Doc *doc)
{
    return this->VisitObject(doc);
}

FunctorCode ConstFunctorInterface::VisitDocEnd(const Doc *doc)
{
    return this->VisitObjectEnd(doc);
}

} // namespace vrv

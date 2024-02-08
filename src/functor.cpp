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
// FunctorBase
//----------------------------------------------------------------------------

std::optional<ClassId> FunctorBase::GetConcurrentClass() const
{
    if (this->GetMaxNumberOfThreads() > 1) {
        switch (this->GetProcessingStrategy()) {
            case ProcessingStrategy::MeasureParallel: return MEASURE;
            case ProcessingStrategy::SystemParallel: return SYSTEM;
            default: break;
        }
    }
    return std::nullopt;
}

//----------------------------------------------------------------------------
// Functor
//----------------------------------------------------------------------------

Functor *Functor::CloneFunctor() const
{
    assert(false);
    return NULL;
}

void Functor::MergeFunctor(const Functor &functor)
{
    if (functor.GetCode() == FUNCTOR_STOP) {
        this->SetCode(FUNCTOR_STOP);
    }
}

//----------------------------------------------------------------------------
// ConstFunctor
//----------------------------------------------------------------------------

ConstFunctor *ConstFunctor::CloneFunctor() const
{
    assert(false);
    return NULL;
}

void ConstFunctor::MergeFunctor(const ConstFunctor &functor)
{
    if (functor.GetCode() == FUNCTOR_STOP) {
        this->SetCode(FUNCTOR_STOP);
    }
}

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

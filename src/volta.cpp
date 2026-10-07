/////////////////////////////////////////////////////////////////////////////
// Name:        volta.cpp
// Author:      Simon Waloschek
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "volta.h"

//----------------------------------------------------------------------------

#include "editorial.h"
#include "functor.h"
#include "syl.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Volta
//----------------------------------------------------------------------------

static const ClassRegistrar<Volta> s_factory("volta", VOLTA);

Volta::Volta() : LayerElement(VOLTA), OffsetInterface(), AttColor(), AttLang(), AttNNumberLike(), AttTypography()
{
    this->RegisterInterface(OffsetInterface::GetAttClasses(), OffsetInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_LANG);
    this->RegisterAttClass(ATT_NNUMBERLIKE);
    this->RegisterAttClass(ATT_TYPOGRAPHY);

    this->Reset();
}

Volta::~Volta() {}

void Volta::Reset()
{
    LayerElement::Reset();
    OffsetInterface::Reset();
    this->ResetColor();
    this->ResetLang();
    this->ResetNNumberLike();
    this->ResetTypography();

    m_drawingVoltaN = 1;
    m_drawingVoltaNSet = false;
}

bool Volta::IsSupportedChild(ClassId classId)
{
    if (classId == SYL) {
        return true;
    }
    else if (Object::IsEditorialElement(classId)) {
        return true;
    }
    else {
        return false;
    }
}

FunctorCode Volta::Accept(Functor &functor)
{
    return functor.VisitVolta(this);
}

FunctorCode Volta::Accept(ConstFunctor &functor) const
{
    return functor.VisitVolta(this);
}

FunctorCode Volta::AcceptEnd(Functor &functor)
{
    return functor.VisitVoltaEnd(this);
}

FunctorCode Volta::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitVoltaEnd(this);
}

} // namespace vrv

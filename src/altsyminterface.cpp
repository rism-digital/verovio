/////////////////////////////////////////////////////////////////////////////
// Name:        altsyminterface.cpp
// Author:      Laurent Pugin
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "altsyminterface.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "functorparams.h"
#include "symboldef.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// AltSymInterface
//----------------------------------------------------------------------------

AltSymInterface::AltSymInterface() : Interface(), AttAltSym()
{
    this->RegisterInterfaceAttClass(ATT_ALTSYM);

    this->Reset();
}

AltSymInterface::~AltSymInterface() {}

void AltSymInterface::Reset()
{
    this->ResetAltSym();

    m_symbolDef = NULL;
    m_symbolDefID = "";
}

void AltSymInterface::SetAltSymbolDef(SymbolDef *symbolDef)
{
    assert(!m_symbolDef);
    m_symbolDef = symbolDef;
}

void AltSymInterface::SetIDStr()
{
    if (this->HasAltsym()) {
        m_symbolDefID = ExtractIDFragment(this->GetAltsym());
    }
}

//----------------------------------------------------------------------------
// Interface pseudo functor (redirected)
//----------------------------------------------------------------------------

int AltSymInterface::InterfacePrepareAltSym(FunctorParams *functorParams, Object *object)
{
    PrepareLinkingParams *params = vrv_params_cast<PrepareLinkingParams *>(functorParams);
    assert(params);

    // This should not happen?
    if (params->m_fillList == false) {
        return FUNCTOR_CONTINUE;
    }

    this->SetIDStr();

    if (!m_symbolDefID.empty()) {
        // TODO
    }

    return FUNCTOR_CONTINUE;
}

int AltSymInterface::InterfaceResetData(FunctorParams *functorParams, Object *object)
{
    m_symbolDef = NULL;
    m_symbolDefID = "";
    return FUNCTOR_CONTINUE;
}

} // namespace vrv

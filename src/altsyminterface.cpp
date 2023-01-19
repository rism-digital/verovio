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

#include "preparedatafunctor.h"
#include "symboldef.h"
#include "symboltable.h"
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

FunctorCode AltSymInterface::InterfacePrepareAltSym(PrepareAltSymFunctor &functor, Object *object)
{
    this->SetIDStr();

    if (!m_symbolDefID.empty()) {
        Object *symbolDef = NULL;
        if (functor.GetSymbolTable()) symbolDef = functor.GetSymbolTable()->FindDescendantByID(m_symbolDefID);

        if (!symbolDef || !symbolDef->Is(SYMBOLDEF)) {
            LogWarning("Reference to the symbolDef `%s` could not be resolved", m_symbolDefID.c_str());
            return FUNCTOR_CONTINUE;
        }
        this->m_symbolDef = vrv_cast<SymbolDef *>(symbolDef);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode AltSymInterface::InterfaceResetData(ResetDataFunctor &functor, Object *object)
{
    m_symbolDef = NULL;
    m_symbolDefID = "";
    return FUNCTOR_CONTINUE;
}

} // namespace vrv

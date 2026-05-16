/////////////////////////////////////////////////////////////////////////////
// Name:        plistinterface.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "plistinterface.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "layerelement.h"
#include "measure.h"
#include "preparedatafunctor.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// PlistInterface
//----------------------------------------------------------------------------

PlistInterface::PlistInterface() : Interface(), AttPlist()
{
    this->RegisterInterfaceAttClass(ATT_PLIST);

    this->Reset();
}

PlistInterface::~PlistInterface() {}

void PlistInterface::Reset()
{
    this->ResetPlist();
}

void PlistInterface::AddRef(const std::string &ref)
{
    xsdAnyURI_List references = this->GetPlist();
    if (std::find(references.begin(), references.end(), ref) == references.end()) {
        references.push_back(ref);
        this->SetPlist(references);
    }
}

void PlistInterface::AddRefAllowDuplicate(const std::string &ref)
{
    xsdAnyURI_List references = this->GetPlist();
    references.push_back(ref);
    this->SetPlist(references);
}

void PlistInterface::SetRef(const Object *ref)
{
    if (!IsValidRef(ref)) {
        return;
    }

    if (std::find(m_references.begin(), m_references.end(), ref) == m_references.end()) {
        m_references.push_back(ref);
    }
}

ArrayOfObjects PlistInterface::GetRefs()
{
    ArrayOfObjects result;
    std::transform(m_references.begin(), m_references.end(), std::back_inserter(result),
        [](const Object *obj) { return const_cast<Object *>(obj); });
    return result;
}

void PlistInterface::SetIDStrs()
{
    assert(m_ids.empty() && m_references.empty());

    xsdAnyURI_List list = this->GetPlist();
    for (const std::string &uri : list) {
        std::string id = ExtractIDFragment(uri);
        if (!id.empty()) {
            m_ids.push_back(id);
        }
        else {
            LogError("Cannot parse the anyURI '%s'", uri.c_str());
        }
    }
}

//----------------------------------------------------------------------------
// Interface pseudo functor (redirected)
//----------------------------------------------------------------------------

FunctorCode PlistInterface::InterfacePreparePlist(PreparePlistFunctor &functor, Object *object)
{
    // This should not happen?
    if (functor.IsProcessingData()) {
        return FUNCTOR_CONTINUE;
    }

    this->SetIDStrs();

    for (const std::string &id : m_ids) {
        functor.InsertInterfaceObjectIDPair(object, id);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode PlistInterface::InterfaceResetData(ResetDataFunctor &functor, Object *object)
{
    m_ids.clear();
    m_references.clear();

    return FUNCTOR_CONTINUE;
}

} // namespace vrv

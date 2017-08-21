/////////////////////////////////////////////////////////////////////////////
// Name:        plistinterface.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "plistinterface.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "attcomparison.h"
#include "functorparams.h"
#include "layerelement.h"
#include "measure.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// PlistInterface
//----------------------------------------------------------------------------

PlistInterface::PlistInterface() : Interface(), AttPlist()
{
    RegisterInterfaceAttClass(ATT_PLIST);

    Reset();
}

PlistInterface::~PlistInterface()
{
}

void PlistInterface::Reset()
{
    ResetPlist();
}

void PlistInterface::AddRef(std::string ref)
{
    /*
    xsdAnyURI_List references = this->GetStaff();
    if (std::find(staves.begin(), staves.end(), n) == staves.end()) {
        staves.push_back(n);
        this->SetStaff(staves);
    }
    */
}

void PlistInterface::SetUuidStrs()
{
    //if (this->HasStartid()) {
    //    m_startUuid = this->ExtractUuidFragment(this->GetStartid());
    //}
}

//----------------------------------------------------------------------------
// Interface pseudo functor (redirected)
//----------------------------------------------------------------------------

int PlistInterface::InterfacePreparePlist(FunctorParams *functorParams, Object *object)
{
    /*
    PrepareTimestampsParams *params = dynamic_cast<PrepareTimestampsParams *>(functorParams);
    assert(params);

    // First we check if the object has already a mapped @startid (it should not)
    if (this->HasStart()) {
        if (this->HasTstamp())
            LogWarning("%s with @xml:id %s has both a @startid and an @tstamp; @tstamp is ignored",
                object->GetClassName().c_str(), object->GetUuid().c_str());
        return FUNCTOR_CONTINUE;
    }
    else if (!HasTstamp()) {
        return FUNCTOR_CONTINUE; // This file is quite likely invalid?
    }

    // We set -1 to the data_MEASUREBEAT for @tstamp
    params->m_tstamps.push_back(std::make_pair(object, data_MEASUREBEAT(-1, this->GetTstamp())));
    */

    return FUNCTOR_CONTINUE;
}

int PlistInterface::InterfaceResetDrawing(FunctorParams *functorParams, Object *object)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv

/////////////////////////////////////////////////////////////////////////////
// Name:        iocmme.h
// Author:      Laurent Pugin
// Created:     2024
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_IOCMME_H__
#define __VRV_IOCMME_H__

#include <string>
#include <vector>

//----------------------------------------------------------------------------

#include "iobase.h"
#include "pugixml.hpp"
#include "vrvdef.h"

namespace vrv {

//----------------------------------------------------------------------------
// CmmeInput
//----------------------------------------------------------------------------

class CmmeInput : public Input {
public:
    // constructors and destructors
    CmmeInput(Doc *doc);
    virtual ~CmmeInput();

    bool Import(const std::string &cmme) override;

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif

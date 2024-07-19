/////////////////////////////////////////////////////////////////////////////
// Name:        iovolpiano.h
// Author:      Laurent Pugin
// Created:     2024
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_IOVOLPIANO_H__
#define __VRV_IOVOLPIANO_H__

#include <string>
#include <vector>

//----------------------------------------------------------------------------

#include "iobase.h"
#include "pugixml.hpp"
#include "vrvdef.h"

namespace vrv {

//----------------------------------------------------------------------------
// VolpianoInput
//----------------------------------------------------------------------------

class VolpianoInput : public Input {
public:
    // constructors and destructors
    VolpianoInput(Doc *doc);
    virtual ~VolpianoInput();

    bool Import(const std::string &volpiano) override;

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif

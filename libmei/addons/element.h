/////////////////////////////////////////////////////////////////////////////
// Name:        element.h
// Author:      Laurent Pugin
// Created:     30/01/2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __LIBMEI_ELEMENT_H__
#define __LIBMEI_ELEMENT_H__

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// Element
//----------------------------------------------------------------------------

/**
 * This class models the MEI element.
 */
class Element {
public:
    Element(std::string name) {}
    virtual ~Element() {}
};

} // namespace vrv

#endif // __LIBMEI_ELEMENT_H__

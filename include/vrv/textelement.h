/////////////////////////////////////////////////////////////////////////////
// Name:        textelement.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TEXT_ELEMENT_H__
#define __VRV_TEXT_ELEMENT_H__

#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// TextElement
//----------------------------------------------------------------------------

class TextElement : public Object {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    TextElement();
    TextElement(std::string classid);
    virtual ~TextElement();
    virtual void Reset();
    virtual std::string GetClassName() const { return "TextElement"; };
    virtual ClassId Is() const { return TEXT_ELEMENT; };
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif

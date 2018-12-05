/////////////////////////////////////////////////////////////////////////////
// Name:        f.h
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_F_H__
#define __VRV_F_H__

#include "atts_shared.h"
#include "textelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// F (figure)
//----------------------------------------------------------------------------

/**
 * This class models the MEI <f> element.
 */
class F : public TextElement {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    F();
    virtual ~F();
    virtual void Reset();
    virtual std::string GetClassName() const { return "F"; }
    virtual ClassId GetClassId() const { return FIGURE; }
    ///@}

    /**
     * Add an element (text, rend. etc.) to a rend.
     * Only supported elements will be actually added to the child list.
     */
    virtual void AddChild(Object *object);

private:
    //
public:
    //
protected:
private:
};

} // namespace vrv

#endif

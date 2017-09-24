/////////////////////////////////////////////////////////////////////////////
// Name:        rend.h
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_REND_H__
#define __VRV_REND_H__

#include "atts_shared.h"
#include "textelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Rend
//----------------------------------------------------------------------------

/**
 * This class models the MEI <rend>
 */
class Rend : public TextElement,
             public AttColor,
             public AttHorizontalAlign,
             public AttLang,
             public AttTypography,
             public AttWhitespace {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    Rend();
    virtual ~Rend();
    virtual Object *Clone() const { return new Rend(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Rend"; }
    virtual ClassId GetClassId() const { return REND; }
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
private:
};

} // namespace vrv

#endif

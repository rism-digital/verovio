/////////////////////////////////////////////////////////////////////////////
// Name:        verse.h
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_VERSE_H__
#define __VRV_VERSE_H__

#include "atts_shared.h"
#include "layerelement.h"

namespace vrv {

class Syl;

//----------------------------------------------------------------------------
// Verse
//----------------------------------------------------------------------------

class Verse : public LayerElement, public AttColor, public AttLang, public AttTypography {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Verse();
    virtual ~Verse();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Verse"; }
    virtual ClassId GetClassId() const { return VERSE; }
    ///@}

    /**
     * Add an element (a syl) to a verse.
     * Only Syl elements will be actually added to the verse.
     */
    virtual void AddChild(Object *object);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::AlignVertically
     */
    virtual int AlignVertically(FunctorParams *functorParams);

    /**
     * See Object::PrepareProcessingLists
     */
    virtual int PrepareProcessingLists(FunctorParams *functorParams);

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif

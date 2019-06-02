/////////////////////////////////////////////////////////////////////////////
// Name:        rhythmglyph.h
// Author:      Laurent Pugin
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_RHYTHMGLYPH_H__
#define __VRV_RHYTHMGLYPH_H__

#include "atts_shared.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// RhythmGlyph
//----------------------------------------------------------------------------

/**
 * This class models the MEI <rhythmGlyph> element.
 */
class RhythmGlyph : public LayerElement, public AttNNumberLike {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    RhythmGlyph();
    virtual ~RhythmGlyph();
    virtual void Reset();
    virtual std::string GetClassName() const { return "RhythmGlyph"; };
    virtual ClassId GetClassId() const { return RHYTHMGLYPH; };
    ///@}

    /**
     * Add an element to a element.
     */
    virtual void AddChild(Object *object);

protected:
    //
private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif

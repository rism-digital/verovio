/////////////////////////////////////////////////////////////////////////////
// Name:        syllable.h
// Author:      Andrew Tran
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SYLLABLE_H__
#define __VRV_SYLLABLE_H__

#include "atts_shared.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Syllable
//----------------------------------------------------------------------------

class ScoreDefInterface;

/**
 * This class models the MEI <mensur> element.
 */
class Syllable : public LayerElement, public ObjectListInterface, public AttColor, public AttSlashCount {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes.
     */
    ///@{
    Syllable();
    void Init();
    virtual ~Syllable();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Syllable"; }
    virtual ClassId GetClassId() const { return SYLLABLE; }
    virtual Object *Clone() const { return new Syllable(*this); }
    ///@}

    /**
     * Add an element (a note or a rest) to a syllable.
     * Only syl or neume will be added.
     */
    virtual void AddChild(Object *object);

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

private:
    //
public:
private:
};

} // namespace vrv

#endif

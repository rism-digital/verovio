/////////////////////////////////////////////////////////////////////////////
// Name:        syllable.h
// Author:      Laurent Pugin
// Created:     2011
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
class Syllable : public LayerElement,
               public AttColor,
               public AttDurationRatio,
               public AttRelativesize,
               public AttSlashcount {
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

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

private:
    //
public:
    /**
     * Static member for setting a value from a controller.
     * Used for example in SetValue
     */
    static int s_num;
    /**
     * Static member for setting a value from a controller.
     * Used for examle in SetValue.
     */
    static int s_numBase;

private:
};

} // namespace vrv

#endif

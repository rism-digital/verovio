/////////////////////////////////////////////////////////////////////////////
// Name:        instrdef.h
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_INSTRDEF_H__
#define __VRV_INSTRDEF_H__

#include "atts_midi.h"
#include "atts_shared.h"
#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// InstrDef
//----------------------------------------------------------------------------

/**
 * This class models the MEI <instrDef> element.
 */
class InstrDef : public Object,
                 public AttChannelized,
                 public AttLabelled,
                 public AttMidiInstrument,
                 public AttNNumberLike {

public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    InstrDef();
    virtual ~InstrDef();
    virtual Object *Clone() const { return new InstrDef(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "InstrDef"; }
    virtual ClassId GetClassId() const { return INSTRDEF; }
    ///@}

    //----------//
    // Functors //
    //----------//

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        phrase.h
// Author:      Laurent Pugin
// Created:     15/06/2020
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PHRASE_H__
#define __VRV_PHRASE_H__

#include "slur.h"

namespace vrv {

//----------------------------------------------------------------------------
// Phrase
//----------------------------------------------------------------------------

class Phrase : public Slur {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Phrase();
    virtual ~Phrase();
    virtual Object *Clone() const { return new Phrase(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Phrase"; }
    virtual ClassId GetClassId() const { return PHRASE; }
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

/////////////////////////////////////////////////////////////////////////////
// Name:        expansion.h
// Author:      Klaus Rettinghaus
// Created:     22/02/2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_EXPANSION_H__
#define __VRV_EXPANSION_H__

#include "atts_shared.h"
#include "systemelement.h"

namespace vrv {

class Expansion;

//----------------------------------------------------------------------------
// Expansion
//----------------------------------------------------------------------------

/**
 * This class represents a MEI expansion.
 */
class Expansion : public SystemElement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Expansion();
    virtual ~Expansion();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Expansion"; }
    virtual ClassId GetClassId() const { return EXPANSION; }
    ///@}

    //----------//
    // Functors //
    //----------//

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

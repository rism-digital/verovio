/////////////////////////////////////////////////////////////////////////////
// Name:        pgfoot.h
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PGFOOT_H__
#define __VRV_PGFOOT_H__

#include "runningelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// PgFoot
//----------------------------------------------------------------------------

/**
 * This class represents an MEI pgFoot.
 */
class PgFoot : public RunningElement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    PgFoot();
    virtual ~PgFoot();
    void Reset() override;
    std::string GetClassName() const override { return "PgFoot"; }
    ///@}

    /**
     * Overriden to get the appropriate margin
     */
    int GetTotalHeight(const Doc *doc) const override;

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

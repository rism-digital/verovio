/////////////////////////////////////////////////////////////////////////////
// Name:        pgfoot2.h
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PGFOOT2_H__
#define __VRV_PGFOOT2_H__

#include "runningelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// PgFoot2
//----------------------------------------------------------------------------

/**
 * This class represents an MEI pgFoot2.
 */
class PgFoot2 : public RunningElement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    PgFoot2();
    virtual ~PgFoot2();
    void Reset() override;
    std::string GetClassName() const override { return "PgFoot2"; }
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

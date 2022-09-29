/////////////////////////////////////////////////////////////////////////////
// Name:        pghead.h
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PGHEAD_H__
#define __VRV_PGHEAD_H__

#include "runningelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// PgHead
//----------------------------------------------------------------------------

/**
 * This class represents an MEI pgHead.
 */
class PgHead : public RunningElement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    PgHead();
    virtual ~PgHead();
    void Reset() override;
    std::string GetClassName() const override { return "PgHead"; }
    ///@}

    /**
     * Overriden to get the appropriate margin
     */
    int GetTotalHeight(const Doc *doc) const override;

    bool GenerateFromMEIHeader(const pugi::xml_document &header);

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

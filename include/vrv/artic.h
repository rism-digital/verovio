/////////////////////////////////////////////////////////////////////////////
// Name:        artic.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ARTIC_H__
#define __VRV_ARTIC_H__

#include "atts_shared.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Artic
//----------------------------------------------------------------------------

class Artic : public LayerElement, public AttArticulation, public AttColor, public AttPlacement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Artic();
    virtual ~Artic();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Artic"; }
    virtual ClassId Is() const { return ARTIC; }
    ///@}

    /**
     * Static method that retrieves the appropriate SMuFL code for a data_ARTICULATION with data_STAFFREL
     */
    static wchar_t GetSmuflCode(data_ARTICULATION artic, data_STAFFREL place);

    /**
     * Static method that retrieves the vertical correctoin for a SMuFL code for with data_STAFFREL.
     * The reason for this is that SMuFL sometimes has the glyph below the line, sometimes above.
     * See bow indications for an example where is is always above
     */
    static bool VerticalCorr(wchar_t code, data_STAFFREL place);

    //----------//
    // Functors //
    //----------//

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        caesura.h
// Author:      Klaus Rettinghaus, Thomas Weber
// Created:     2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CAESURA_H__
#define __VRV_CAESURA_H__

#include "atts_cmn.h"
#include "atts_externalsymbols.h"
#include "controlelement.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Caesura
//----------------------------------------------------------------------------

/**
 * This class models the MEI <caesura> element.
 */
class Caesura : public ControlElement,
                public TimePointInterface,
                public AttColor,
                public AttExtSym,
                public AttPlacementRelStaff {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Caesura();
    virtual ~Caesura();
    Object *Clone() const override { return new Caesura(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Caesura"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    TimePointInterface *GetTimePointInterface() override { return vrv_cast<TimePointInterface *>(this); }
    const TimePointInterface *GetTimePointInterface() const override
    {
        return vrv_cast<const TimePointInterface *>(this);
    }
    ///@}

    /**
     * Get the SMuFL glyph.
     */
    ///@{
    char32_t GetCaesuraGlyph() const;
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

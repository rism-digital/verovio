/////////////////////////////////////////////////////////////////////////////
// Name:        fermata.h
// Author:      Klaus Rettinghaus
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FERMATA_H__
#define __VRV_FERMATA_H__

#include "atts_cmn.h"
#include "atts_visual.h"
#include "controlelement.h"
#include "timeinterface.h"

namespace vrv {

class ConvertAnalyticalMarkupParams;

//----------------------------------------------------------------------------
// Fermata
//----------------------------------------------------------------------------

/**
 * This class models the MEI <fermata> element.
 */
class Fermata : public ControlElement,
                public TimePointInterface,
                public AttColor,
                public AttFermataVis,
                public AttPlacement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Fermata();
    virtual ~Fermata();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Fermata"; }
    virtual ClassId GetClassId() const { return FERMATA; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    ///@}

    /**
     * Helpler for converting markup (from Note, Chord, Rest, MRest)
     */
    void ConvertFromAnalyticalMarkup(
        AttFermataPresent *fermataPresent, const std::string &uuid, ConvertAnalyticalMarkupParams *params);

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

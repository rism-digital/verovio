/////////////////////////////////////////////////////////////////////////////
// Name:        octave.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_OCTAVE_H__
#define __VRV_OCTAVE_H__

#include "atts_cmn.h"
#include "atts_shared.h"
#include "controlelement.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Octave
//----------------------------------------------------------------------------

/**
 * This class models the MEI <octave> element.
 */
class Octave : public ControlElement,
               public TimeSpanningInterface,
               public AttColor,
               public AttExtender,
               public AttLineRend,
               public AttLineRendBase,
               public AttNNumberLike,
               public AttOctaveDisplacement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Octave();
    virtual ~Octave();
    Object *Clone() const override { return new Octave(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Octave"; }
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
    TimeSpanningInterface *GetTimeSpanningInterface() override { return vrv_cast<TimeSpanningInterface *>(this); }
    const TimeSpanningInterface *GetTimeSpanningInterface() const override
    {
        return vrv_cast<const TimeSpanningInterface *>(this);
    }
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

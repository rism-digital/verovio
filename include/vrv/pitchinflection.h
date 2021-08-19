/////////////////////////////////////////////////////////////////////////////
// Name:        pitchinfection.h
// Author:      Laurent Pugin
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PITCH_INFLECTION_H__
#define __VRV_PITCH_INFLECTION_H__

#include "controlelement.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// PitchInflection
//----------------------------------------------------------------------------

/**
 * This class models the MEI <dir> element.
 */
class PitchInflection : public ControlElement, public TimeSpanningInterface {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    PitchInflection();
    virtual ~PitchInflection();
    virtual Object *Clone() const { return new PitchInflection(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "PitchInflection"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    virtual TimeSpanningInterface *GetTimeSpanningInterface() { return dynamic_cast<TimeSpanningInterface *>(this); }
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

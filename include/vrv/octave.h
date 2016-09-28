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

class Octave : public ControlElement, public TimeSpanningInterface, public AttColor, public AttOctavedisplacement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Octave();
    virtual ~Octave();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Octave"; }
    virtual ClassId Is() const { return OCTAVE; }
    ///@}

    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    virtual TimeSpanningInterface *GetTimeSpanningInterface() { return dynamic_cast<TimeSpanningInterface *>(this); }

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

/////////////////////////////////////////////////////////////////////////////
// Name:        arpeg.h
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ARPEG_H__
#define __VRV_ARPEG_H__

#include "atts_cmn.h"
#include "atts_visual.h"
#include "controlelement.h"
#include "plistinterface.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Arpeg
//----------------------------------------------------------------------------

class Arpeg : public ControlElement,
              public PlistInterface,
              public TimePointInterface,
              public AttArpegLog,
              public AttArpegVis,
              public AttColor {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Arpeg();
    virtual ~Arpeg();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Arpeg"; }
    virtual ClassId GetClassId() const { return ARPEG; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual PlistInterface *GetPlistInterface() { return dynamic_cast<PlistInterface *>(this); }
    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    ////@}

    /**
     * Custom method for @plist validation
     */
    virtual bool IsValidRef(Object *ref);

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

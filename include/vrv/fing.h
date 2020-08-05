/////////////////////////////////////////////////////////////////////////////
// Name:        fing.h
// Author:      Andriy Makarchuk
// Created:     2020
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FING_H__
#define __VRV_FING_H__

#include "atts_cmn.h"
#include "controlelement.h"
#include "textdirinterface.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Fing (fingering)
//----------------------------------------------------------------------------

/**
 * This class models the MEI <fing> element.
 */
class Fing : public ControlElement, public TimePointInterface, public TextDirInterface, public AttNNumberLike {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Fing();
    virtual ~Fing();
    virtual Object *Clone() const { return new Fing(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Fing"; }
    virtual ClassId GetClassId() const { return FING; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual TextDirInterface *GetTextDirInterface() { return dynamic_cast<TextDirInterface *>(this); }
    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    ///@}

    /**
     * Add an element (text, rend) to a fing.
     * Only supported elements will be actually added to the child list.
     */
    virtual bool IsSupportedChild(Object *object);

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

#endif // __VRV_FING_H__

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
    Object *Clone() const override { return new Fing(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Fing"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    TextDirInterface *GetTextDirInterface() override { return dynamic_cast<TextDirInterface *>(this); }
    const TextDirInterface *GetTextDirInterface() const override
    {
        return dynamic_cast<const TextDirInterface *>(this);
    }
    TimePointInterface *GetTimePointInterface() override { return dynamic_cast<TimePointInterface *>(this); }
    const TimePointInterface *GetTimePointInterface() const override
    {
        return dynamic_cast<const TimePointInterface *>(this);
    }
    ///@}

    /**
     * Add an element (text, rend) to a fing.
     * Only supported elements will be actually added to the child list.
     */
    bool IsSupportedChild(Object *object) override;

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

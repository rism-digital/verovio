/////////////////////////////////////////////////////////////////////////////
// Name:        ornam.h
// Author:      Laurent Pugin
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ORNAM_H__
#define __VRV_ORNAM_H__

#include "atts_cmn.h"
#include "atts_cmnornaments.h"
#include "atts_externalsymbols.h"
#include "controlelement.h"
#include "textdirinterface.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Ornam
//----------------------------------------------------------------------------

/**
 * This class models the MEI <ornam> element.
 */
class Ornam : public ControlElement,
              public TextListInterface,
              public TextDirInterface,
              public TimePointInterface,
              public AttOrnamentAccid {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Ornam();
    virtual ~Ornam();
    Object *Clone() const override { return new Ornam(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "ornam"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    TextDirInterface *GetTextDirInterface() override { return vrv_cast<TextDirInterface *>(this); }
    const TextDirInterface *GetTextDirInterface() const override { return vrv_cast<const TextDirInterface *>(this); }
    TimePointInterface *GetTimePointInterface() override { return vrv_cast<TimePointInterface *>(this); }
    const TimePointInterface *GetTimePointInterface() const override
    {
        return vrv_cast<const TimePointInterface *>(this);
    }
    ///@}

    /**
     * Add an element (text, rend. etc.) to a ornam.
     * Only supported elements will be actually added to the child list.
     */
    bool IsSupportedChild(ClassId classId) override;

    //----------//
    // Functors //
    //----------//

    /**
     * Interface for class functor visitation
     */
    ///@{
    FunctorCode Accept(Functor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(Functor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
    ///@}

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

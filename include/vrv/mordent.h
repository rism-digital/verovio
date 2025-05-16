/////////////////////////////////////////////////////////////////////////////
// Name:        mordent.h
// Author:      Klaus Rettinghaus
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_MORDENT_H__
#define __VRV_MORDENT_H__

#include "atts_cmn.h"
#include "atts_cmnornaments.h"
#include "atts_externalsymbols.h"
#include "controlelement.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Mordent
//----------------------------------------------------------------------------

/**
 * This class models the MEI <mordent> element.
 */
class Mordent : public ControlElement,
                public TimePointInterface,
                public AttExtSymAuth,
                public AttExtSymNames,
                public AttOrnamentAccid,
                public AttPlacementRelStaff,
                public AttMordentLog {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Mordent();
    virtual ~Mordent();
    Object *Clone() const override { return new Mordent(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "mordent"; }
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
     * Get the SMuFL glyph for the mordent based on type, long attribute or glyph.num
     */
    char32_t GetMordentGlyph() const;

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

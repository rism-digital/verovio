/////////////////////////////////////////////////////////////////////////////
// Name:        bracketspan.h
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_BRACKETSPAN_H__
#define __VRV_BRACKETSPAN_H__

#include "atts_cmn.h"
#include "controlelement.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// BracketSpan
//----------------------------------------------------------------------------

/**
 * This class models the MEI <bracketSpan> element.
 */
class BracketSpan : public ControlElement,
                    public TimeSpanningInterface,
                    public AttBracketSpanLog,
                    public AttLineRend,
                    public AttLineRendBase {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    BracketSpan();
    virtual ~BracketSpan();
    Object *Clone() const override { return new BracketSpan(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "bracketSpan"; }
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

    /**
     * Calculate the bracket line width.
     */
    int GetLineWidth(const Doc *doc, int unit) const;

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
};

} // namespace vrv

#endif

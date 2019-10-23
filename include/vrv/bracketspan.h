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
                    public AttColor,
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
    virtual Object *Clone() const { return new BracketSpan(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "BracketSpan"; }
    virtual ClassId GetClassId() const { return BRACKETSPAN; }
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
};

} // namespace vrv

#endif

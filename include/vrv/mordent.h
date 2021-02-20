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
                public AttColor,
                public AttExtSym,
                public AttOrnamentAccid,
                public AttPlacement,
                public AttMordentLog {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Mordent();
    virtual ~Mordent();
    virtual Object *Clone() const { return new Mordent(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Mordent"; }
    virtual ClassId GetClassId() const { return MORDENT; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    ///@}

    /**
     * Get the SMuFL glyph for the mordent based on type, long attribute or glyph.num
     */
    wchar_t GetMordentGlyph() const;

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

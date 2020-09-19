/////////////////////////////////////////////////////////////////////////////
// Name:        turn.h
// Author:      Klaus Rettinghaus
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TURN_H__
#define __VRV_TURN_H__

#include "atts_cmn.h"
#include "atts_cmnornaments.h"
#include "atts_externalsymbols.h"
#include "controlelement.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Turn
//----------------------------------------------------------------------------

/**
 * This class models the MEI <turn> element.
 */
class Turn : public ControlElement,
             public TimePointInterface,
             public AttColor,
             public AttExtSym,
             public AttOrnamentAccid,
             public AttPlacement,
             public AttTurnLog {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Turn();
    virtual ~Turn();
    virtual Object *Clone() const { return new Turn(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Turn"; }
    virtual ClassId GetClassId() const { return TURN; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    ///@}

    /**
     * Get the SMuFL glyph for the turn based on form or glyph.num
     */
    wchar_t GetTurnGlyph() const;

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

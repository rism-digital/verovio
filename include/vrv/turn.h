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
             public AttExtSymAuth,
             public AttExtSymNames,
             public AttOrnamentAccid,
             public AttPlacementRelStaff,
             public AttTurnLog {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Turn();
    virtual ~Turn();
    Object *Clone() const override { return new Turn(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "turn"; }
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
     * Get the SMuFL glyph for the turn based on form or glyph.num
     */
    char32_t GetTurnGlyph() const;

    /**
     * Get the turn height ignoring slash
     */
    int GetTurnHeight(const Doc *doc, int staffSize) const;

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
    /**
     * The end point of a delayed turn when @startid is used
     */
    LayerElement *m_drawingEndElement;

private:
    //
};

} // namespace vrv

#endif

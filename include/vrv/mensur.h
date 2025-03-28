/////////////////////////////////////////////////////////////////////////////
// Name:        mensur.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_MENSUR_H__
#define __VRV_MENSUR_H__

#include "atts_mensural.h"
#include "atts_shared.h"
#include "atts_visual.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Mensur
//----------------------------------------------------------------------------

class ScoreDefInterface;

/**
 * This class models the MEI <mensur> element.
 */
class Mensur : public LayerElement,
               public AttColor,
               public AttCue,
               public AttDurationRatio,
               public AttMensuralShared,
               public AttMensurVis,
               public AttSlashCount,
               public AttStaffLoc {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes.
     */
    ///@{
    Mensur();
    virtual ~Mensur();
    Object *Clone() const override { return new Mensur(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "mensur"; }
    ///@}

    /** Override the method since alignment is required */
    bool HasToBeAligned() const override { return true; }

    /** Override the method since check is required */
    bool IsScoreDefElement() const override { return (this->GetParent() && this->GetFirstAncestor(SCOREDEF)); }

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

private:
    //
public:
    //----------------//
    // Static members //
    //----------------//

    /**
     * Static member for setting a value from a controller.
     * Used for example in SetValue
     */
    static const int s_num;
    /**
     * Static member for setting a value from a controller.
     * Used for examle in SetValue.
     */
    static const int s_numBase;

private:
};

} // namespace vrv

#endif

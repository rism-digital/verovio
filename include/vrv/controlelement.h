/////////////////////////////////////////////////////////////////////////////
// Name:        controlelement.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CONTROL_ELEMENT_H__
#define __VRV_CONTROL_ELEMENT_H__

#include "altsyminterface.h"
#include "atts_shared.h"
#include "atts_visual.h"
#include "devicecontextbase.h"
#include "floatingobject.h"
#include "linkinginterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// ControlElement
//----------------------------------------------------------------------------

/**
 * This class represents elements appearing within a measure.
 * It is not an abstract class but should not be instanciated directly.
 */
class ControlElement : public FloatingObject,
                       public AltSymInterface,
                       public LinkingInterface,
                       public AttColor,
                       public AttLabelled,
                       public AttTyped {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method resets all attribute classes
     */
    ///@{
    ControlElement();
    ControlElement(ClassId classId);
    virtual ~ControlElement();
    void Reset() override;
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    AltSymInterface *GetAltSymInterface() override { return vrv_cast<AltSymInterface *>(this); }
    const AltSymInterface *GetAltSymInterface() const override { return vrv_cast<const AltSymInterface *>(this); }
    LinkingInterface *GetLinkingInterface() override { return vrv_cast<LinkingInterface *>(this); }
    const LinkingInterface *GetLinkingInterface() const override { return vrv_cast<const LinkingInterface *>(this); }
    ///@}

    /**
     * Check if the ControlElement has a Rend child and return its @halign equivalent (if any)
     */
    data_HORIZONTALALIGNMENT GetChildRendAlignment() const;

    /**
     * Check if the ControlElement applies at a point where there is more than on layer.
     * Returns that placement accordingly - otherwise return the default passed as parameter.
     * Applied only for trill, mordent, and turn elements.
     */
    data_STAFFREL GetLayerPlace(data_STAFFREL defaultValue) const;

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
    //
private:
    //
};

} // namespace vrv

#endif

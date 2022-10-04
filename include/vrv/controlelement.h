/////////////////////////////////////////////////////////////////////////////
// Name:        controlelement.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CONTROL_ELEMENT_H__
#define __VRV_CONTROL_ELEMENT_H__

#include "atts_shared.h"
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
class ControlElement : public FloatingObject, public LinkingInterface, public AttLabelled, public AttTyped {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method resets all attribute classes
     */
    ///@{
    ControlElement();
    ControlElement(ClassId classId);
    ControlElement(ClassId classId, const std::string &classIdStr);
    virtual ~ControlElement();
    void Reset() override;
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
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
     * See Object::AdjustXOverflow
     */
    int AdjustXOverflow(FunctorParams *functorParams) override;

    /**
     * See Object::ResetData
     */
    int ResetData(FunctorParams *functorParams) override;

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif

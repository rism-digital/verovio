/////////////////////////////////////////////////////////////////////////////
// Name:        systemelement.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SYSTEM_ELEMENT_H__
#define __VRV_SYSTEM_ELEMENT_H__

#include "atts_shared.h"
#include "devicecontextbase.h"
#include "drawinginterface.h"
#include "floatingobject.h"

namespace vrv {

//----------------------------------------------------------------------------
// SystemElement
//----------------------------------------------------------------------------

/**
 * This class represents elements appearing within a measure.
 * It is not an abstract class but should not be instanciated directly.
 */
class SystemElement : public FloatingObject, public VisibilityDrawingInterface, public AttTyped {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method resets all attribute classes
     */
    ///@{
    SystemElement();
    SystemElement(ClassId classId);
    virtual ~SystemElement();
    void Reset() override;
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    VisibilityDrawingInterface *GetVisibilityDrawingInterface() override
    {
        return vrv_cast<VisibilityDrawingInterface *>(this);
    }
    const VisibilityDrawingInterface *GetVisibilityDrawingInterface() const override
    {
        return vrv_cast<const VisibilityDrawingInterface *>(this);
    }
    ///@}

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

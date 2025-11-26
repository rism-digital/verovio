/////////////////////////////////////////////////////////////////////////////
// Name:        offsetinterface.h
// Author:      Laurent Pugin
// Created:     2025
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_OFFSET_INTERFACE_H__
#define __VRV_OFFSET_INTERFACE_H__

#include "atts_shared.h"
#include "interface.h"
#include "vrvdef.h"

namespace vrv {

class Object;
class ResetDataFunctor;

//----------------------------------------------------------------------------
// OffsetInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for elements having a single visual offset, such as arpeg, reh, etc..
 * It is not an abstract class but should not be instantiated directly.
 */
class OffsetInterface : public Interface, public AttVisualOffsetHo, public AttVisualOffsetVo {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    OffsetInterface();
    virtual ~OffsetInterface();
    void Reset() override;
    InterfaceId IsInterface() const override { return INTERFACE_OFFSET; }
    ///@}

    //-----------------//
    // Pseudo functors //
    //-----------------//

    /**
     * We have functor code in the interface for avoiding code duplication in each implementation class.
     * Since we are in an interface, we need to pass the object (implementation) to
     * the pseudo functor method.
     */
    FunctorCode InterfaceResetData(ResetDataFunctor &functor, Object *object);
    ///@}

protected:
    //
private:
    //
public:
    //
protected:
    //
private:
};

//----------------------------------------------------------------------------
// OffsetSpanningInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for elements having spanning visual offsets, such as slur, hairpin, etc..
 * It is not an abstract class but should not be instantiated directly.
 */
class OffsetSpanningInterface : public Interface, public AttVisualOffset2Ho, public AttVisualOffset2Vo {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method resets all attribute classes
     */
    ///@{
    OffsetSpanningInterface();
    virtual ~OffsetSpanningInterface();
    void Reset() override;
    InterfaceId IsInterface() const override { return INTERFACE_OFFSET_SPANNING; }
    ///@}

    ///@{
    FunctorCode InterfaceResetData(ResetDataFunctor &functor, Object *object);
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
